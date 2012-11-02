/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/
/*
 * $Source: f:/miner/source/cfile/rcs/cfile.c $
 * $Revision: 1.24 $
 * $Author: john $
 * $Date: 1995/03/15 14:20:27 $
 * 
 * Functions for accessing compressed files.
 * 
 * $Log: cfile.c $
 * Revision 1.24  1995/03/15  14:20:27  john
 * Added critical error checker.
 * 
 * Revision 1.23  1995/03/13  15:16:53  john
 * Added alternate directory stuff.
 * 
 * Revision 1.22  1995/02/09  23:08:47  matt
 * Increased the max number of files in hogfile to 250
 * 
 * Revision 1.21  1995/02/01  20:56:47  john
 * Added cfexist function
 * 
 * Revision 1.20  1995/01/21  17:53:48  john
 * Added alternate pig file thing.
 * 
 * Revision 1.19  1994/12/29  15:10:02  john
 * Increased hogfile max files to 200.
 * 
 * Revision 1.18  1994/12/12  13:20:57  john
 * Made cfile work with fiellentth.
 * 
 * Revision 1.17  1994/12/12  13:14:25  john
 * Made cfiles prefer non-hog files.
 * 
 * Revision 1.16  1994/12/09  18:53:26  john
 * *** empty log message ***
 * 
 * Revision 1.15  1994/12/09  18:52:56  john
 * Took out mem, error checking.
 * 
 * Revision 1.14  1994/12/09  18:10:31  john
 * Speed up cfgets, which was slowing down the reading of
 * bitmaps.tbl, which was making POF loading look slow.
 * 
 * Revision 1.13  1994/12/09  17:53:51  john
 * Added error checking to number of hogfiles..
 * 
 * Revision 1.12  1994/12/08  19:02:55  john
 * Added cfgets.
 * 
 * Revision 1.11  1994/12/07  21:57:48  john
 * Took out data dir.
 * 
 * Revision 1.10  1994/12/07  21:38:02  john
 * Made cfile not return error..
 * 
 * Revision 1.9  1994/12/07  21:35:34  john
 * Made it read from data directory.
 * 
 * Revision 1.8  1994/12/07  21:33:55  john
 * Stripped out compression stuff...
 * 
 * Revision 1.7  1994/04/13  23:44:59  matt
 * When file cannot be opened, free up the buffer for that file.
 * 
 * Revision 1.6  1994/02/18  12:38:20  john
 * Optimized a bit
 * 
 * Revision 1.5  1994/02/15  18:13:20  john
 * Fixed more bugs.
 * 
 * Revision 1.4  1994/02/15  13:27:58  john
 * Works ok...
 * 
 * Revision 1.3  1994/02/15  12:51:57  john
 * Crappy inbetween version
 * 
 * Revision 1.2  1994/02/14  20:12:29  john
 * First version working with new cfile stuff.
 * 
 * Revision 1.1  1994/02/14  15:51:33  john
 * Initial revision
 * 
 * Revision 1.1  1994/02/10  15:45:12  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: cfile.c 1.24 1995/03/15 14:20:27 john Exp $";
#pragma on (unreferenced)

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <conio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <errno.h>
#include <string.h>

#include "cfile.h"

typedef struct hogfile {
	char	name[13];
	int	offset;
	int 	length;
} hogfile;

#define MAX_HOGFILES 250

hogfile HogFiles[MAX_HOGFILES];
char Hogfile_initialized = 0;
int Num_hogfiles = 0;

hogfile AltHogFiles[MAX_HOGFILES];
char AltHogfile_initialized = 0;
int AltNum_hogfiles = 0;
char AltHogFilename[64];

char AltHogDir[64];
char AltHogdir_initialized = 0;

void cfile_use_alternate_hogdir( char * path )
{
	if ( path )	{
		strcpy( AltHogDir, path );
		AltHogdir_initialized = 1;
	} else {
		AltHogdir_initialized = 0;
	}
}

extern int descent_critical_error;

FILE * cfile_get_filehandle( char * filename, char * mode )
{
	FILE * fp;
	char temp[128];
	
	descent_critical_error = 0;
	fp = fopen( filename, mode );
	if ( fp && descent_critical_error )	{
		fclose(fp);
		fp = NULL;
	}
	if ( (fp==NULL) && (AltHogdir_initialized) )	{
		strcpy( temp, AltHogDir );
		strcat( temp, filename );
		descent_critical_error = 0;
		fp = fopen( temp, mode );
		if ( fp && descent_critical_error )	{
			fclose(fp);
			fp = NULL;
		}
	} 
	return fp;
}

void cfile_init_hogfile(char *fname, hogfile * hog_files, int * nfiles )
{
	char id[4];
	FILE * fp;
	int i, len;

	*nfiles = 0;

	fp = cfile_get_filehandle( fname, "rb" );
	if ( fp == NULL ) return;

	fread( id, 3, 1, fp );
	if ( strncmp( id, "DHF", 3 ) )	{
		fclose(fp);
		return;
	}

	while( 1 )	
	{	
		if ( *nfiles >= MAX_HOGFILES ) {
			printf( "ERROR: HOGFILE IS LIMITED TO %d FILES\n",  MAX_HOGFILES );
			fclose(fp);
			exit(1);
		}
		i = fread( hog_files[*nfiles].name, 13, 1, fp );
		if ( i != 1 )	{
			fclose(fp);
			return;
		}
		i = fread( &len, 4, 1, fp );
		if ( i != 1 )	{
			fclose(fp);
			return;
		}
		hog_files[*nfiles].length = len;
		hog_files[*nfiles].offset = ftell( fp );
		*nfiles = (*nfiles) + 1;
		// Skip over
		i = fseek( fp, len, SEEK_CUR );
	}
}

FILE * cfile_find_libfile(char * name, int * length)
{
	FILE * fp;
	int i;

	if ( AltHogfile_initialized )	{
		for (i=0; i<AltNum_hogfiles; i++ )	{
			if ( !stricmp( AltHogFiles[i].name, name ))	{
				fp = cfile_get_filehandle( AltHogFilename, "rb" );
				if ( fp == NULL ) return NULL;
				fseek( fp,  AltHogFiles[i].offset, SEEK_SET );
				*length = AltHogFiles[i].length;
				return fp;
			}
		}
	}

	if ( !Hogfile_initialized ) 	{
		cfile_init_hogfile( "DESCENT.HOG", HogFiles, &Num_hogfiles );
		Hogfile_initialized = 1;
	}

	for (i=0; i<Num_hogfiles; i++ )	{
		if ( !stricmp( HogFiles[i].name, name ))	{
			fp = cfile_get_filehandle( "DESCENT.HOG", "rb" );
			if ( fp == NULL ) return NULL;
			fseek( fp,  HogFiles[i].offset, SEEK_SET );
			*length = HogFiles[i].length;
			return fp;
		}
	}
	return NULL;
}

void cfile_use_alternate_hogfile( char * name )
{
	if ( name )	{
		strcpy( AltHogFilename, name );
		cfile_init_hogfile( AltHogFilename, AltHogFiles, &AltNum_hogfiles );
		AltHogfile_initialized = 1;
	} else {
		AltHogfile_initialized = 0;
	}
}

int cfexist( char * filename )
{
	int length;
	FILE *fp;

	fp = cfile_get_filehandle( filename, "rb" );		// Check for non-hog file first...
	if ( fp )	{
		fclose(fp);
		return 1;
	}

	fp = cfile_find_libfile(filename, &length );
	if ( fp )	{
		fclose(fp);
		return 2;		// file found in hog
	}

	return 0;		// Couldn't find it.
}


CFILE * cfopen(char * filename, char * mode ) 
{
	int length;
	FILE * fp;
	CFILE *cfile;
	
	if (strcmpi( mode, "rb"))	{
		printf( "CFILES CAN ONLY BE OPENED WITH RB\n" );
		exit(1);
	}

	fp = cfile_get_filehandle( filename, mode );		// Check for non-hog file first...
	if ( !fp ) {
		fp = cfile_find_libfile(filename, &length );
		if ( !fp )
			return NULL;		// No file found
		cfile = malloc ( sizeof(CFILE) );
		if ( cfile == NULL ) {
			fclose(fp);
			return NULL;
		}
		cfile->file = fp;
		cfile->size = length;
		cfile->lib_offset = ftell( fp );
		cfile->raw_position = 0;
		return cfile;
	} else {
		cfile = malloc ( sizeof(CFILE) );
		if ( cfile == NULL ) {
			fclose(fp);
			return NULL;
		}
		cfile->file = fp;
		cfile->size = filelength( fileno(fp) );
		cfile->lib_offset = 0;
		cfile->raw_position = 0;
		return cfile;
	}
}

int cfilelength( CFILE *fp )
{
	return fp->size;
}

int cfgetc( CFILE * fp ) 
{
	int c;

	if (fp->raw_position >= fp->size ) return EOF;

	c = getc( fp->file );
	if (c!=EOF) 
		fp->raw_position++;

//	Assert( fp->raw_position==(ftell(fp->file)-fp->lib_offset) );

	return c;
}

char * cfgets( char * buf, size_t n, CFILE * fp )
{
	char * t = buf;
	int i;
	int c;

	for (i=0; i<n-1; i++ )	{
		do {
			if (fp->raw_position >= fp->size ) {
				*buf = 0;
				return NULL;
			}
			c = fgetc( fp->file );
			fp->raw_position++;
		} while ( c == 13 );
		*buf++ = c;
		if ( c=='\n' ) break;
	}
	*buf++ = 0;
	return  t;
}

size_t cfread( void * buf, size_t elsize, size_t nelem, CFILE * fp ) 
{
	int i;
	if ((fp->raw_position+(elsize*nelem)) > fp->size ) return EOF;
	i = fread( buf, elsize, nelem, fp->file );
	fp->raw_position += i*elsize;
	return i;
}


int cftell( CFILE *fp )	
{
	return fp->raw_position;
}

int cfseek( CFILE *fp, long int offset, int where )
{
	int c, goal_position;

	switch( where )	{
	case SEEK_SET:
		goal_position = offset;
		break;
	case SEEK_CUR:
		goal_position = fp->raw_position+offset;
		break;
	case SEEK_END:
		goal_position = fp->size+offset;
		break;
	default:
		return 1;
	}	
	c = fseek( fp->file, fp->lib_offset + goal_position, SEEK_SET );
	fp->raw_position = ftell(fp->file)-fp->lib_offset;
	return c;
}

void cfclose( CFILE * fp ) 
{	
	fclose(fp->file);
	free(fp);
	return;
}




