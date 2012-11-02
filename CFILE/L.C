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
 * $Revision: 1.7 $
 * $Author: matt $
 * $Date: 1994/04/13 23:44:59 $
 * 
 * Functions for accessing compressed files.
 * 
 * $Log: cfile.c $
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
static char rcsid[] = "$Id: cfile.c 1.7 1994/04/13 23:44:59 matt Exp $";
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

#include "mem.h"
#include "error.h"
#include "cfile.h"

#define BITS                       15
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
#define TABLE_SIZE                 35023L
#define END_OF_STREAM              256
#define BUMP_CODE                  257
#define FLUSH_CODE                 258
#define FIRST_CODE                 259
#define UNUSED                     -1

extern FILE * LibraryGetFileInfo( char *filename, int * others_use, int * lib_offset, int * file_size, int * org_size, int * compressed, char * buffer );

int raw_seek( CFILE * cfile, int pos )
{
	int i;

	cfile->raw_position = pos;
	//printf( "Seeking to %d + %d\n", cfile->lib_offset, cfile->raw_position );
	i = fseek( cfile->file, cfile->lib_offset+cfile->raw_position, SEEK_SET );
	if (i) Int3();
	return i;

}

int raw_getc( CFILE * cfile )
{	
	int c;

//	if (cfile->others_use)
//		fseek( cfile->file, cfile->lib_offset+cfile->raw_position, SEEK_SET );

	if (cfile->raw_position >= cfile->org_size ) return EOF;

	c = getc( cfile->file );
	if (c!=EOF) 
		cfile->raw_position++;

//	Assert( cfile->raw_position==(ftell(cfile->file)-cfile->lib_offset) );

	return c;
}

int raw_putc( int c, CFILE * cfile )
{
	int r;

	if (cfile->others_use)
		fseek( cfile->file, cfile->lib_offset+cfile->raw_position, SEEK_SET );

	r = fputc( c, cfile->file );

 	if (r!=EOF) 
		cfile->raw_position++;

	Assert( cfile->raw_position==(ftell(cfile->file)-cfile->lib_offset) );

	return r;
}

void compress_init_dict( CFILE * cfile )
{
	unsigned int i;

	for ( i = 0 ; i < TABLE_SIZE ; i++ )
		cfile->dict[i].code_value = UNUSED;

	cfile->next_code = FIRST_CODE;
	cfile->current_code_bits = 9;
	cfile->next_bump_code = 511;
}

void compress_init_storage( CFILE * cfile )
{
	MALLOC( cfile->dict, DICTIONARY, TABLE_SIZE );
	MALLOC( cfile->decode_stack, char, TABLE_SIZE );
}

void compress_free_storage( CFILE * cfile )
{
	free(cfile->dict);
	free(cfile->decode_stack);
}

unsigned int compress_find_child_node( CFILE * cfile, int parent_code, int child_character ) {
    unsigned int index;
    int offset;

    index = ( child_character << ( BITS - 8 ) ) ^ parent_code;
    if ( index == 0 )
        offset = 1;
    else
        offset = TABLE_SIZE - index;
    for ( ; ; ) {
		if ( cfile->dict[ index ].code_value == UNUSED )
            return( (unsigned int) index );
		if ( cfile->dict[ index ].parent_code == parent_code &&
			 cfile->dict[ index ].character == (char) child_character )
            return( index );
        if ( (int) index >= offset )
            index -= offset;
        else
            index += TABLE_SIZE - offset;
    }
}

unsigned int compress_decode_string( CFILE * cfile, unsigned int count, unsigned int code ) {
    while ( code > 255 ) {
		cfile->decode_stack[ count++ ] = cfile->dict[ code ].character;
		code = cfile->dict[ code ].parent_code;
    }
    cfile->decode_stack[ count++ ] = (char) code;
    return( count );
}

void compress_output_bit( CFILE *bit_file, int bit ) {
	if ( bit )
		bit_file->rack |= bit_file->mask;
	bit_file->mask >>= 1;
	if ( bit_file->mask == 0 ) {
		if ( raw_putc(bit_file->rack, bit_file ) != bit_file->rack )
			Error( "Error in compress_output_bit" );
		bit_file->rack = 0;
		bit_file->mask = 0x80;
	}
}

void compress_output_bits( CFILE *bit_file, unsigned int code, int count ) {
    unsigned int mask;

    mask = 1L << ( count - 1 );
    while ( mask != 0) {
        if ( mask & code )
            bit_file->rack |= bit_file->mask;
        bit_file->mask >>= 1;
        if ( bit_file->mask == 0 ) {
				if ( raw_putc(bit_file->rack, bit_file ) != bit_file->rack )
					Error( "Error in compress_output_bits" );
				bit_file->rack = 0;
            bit_file->mask = 0x80;
        }
        mask >>= 1;
    }
}

int compress_input_bit( CFILE *bit_file ) {
    int value;

    if ( bit_file->mask == 0x80 ) {
        bit_file->rack = raw_getc( bit_file );
        if ( bit_file->rack == EOF ) 
				return END_OF_STREAM;
        //Error( "Fatal error in compress_input_bit!" );
    }
    value = bit_file->rack & bit_file->mask;
    bit_file->mask >>= 1;
    if ( bit_file->mask == 0 )
    bit_file->mask = 0x80;
    return( value ? 1 : 0 );
}

unsigned int compress_input_bits( CFILE *bit_file, int bit_count ) {
	unsigned int mask;
	unsigned int return_value;

	mask = 1L << ( bit_count - 1 );
	return_value = 0;
	while ( mask != 0)	{
		if ( bit_file->mask == 0x80 ) {
			bit_file->rack = raw_getc( bit_file );
			if ( bit_file->rack == EOF ) 
				return END_OF_STREAM;
			//Error( "Fatal error in compress_input_bit!" );
    	}

		if ( bit_file->rack & bit_file->mask )
			return_value |= mask;
		mask >>= 1;
		bit_file->mask >>= 1;
		if ( bit_file->mask == 0 )
			bit_file->mask = 0x80;
	}
	return( return_value );
}

CFILE * cfopen(char * filename, char * mode ) {
	CFILE *cfile;
	char signature[3] = "CF";

	MALLOC(cfile, CFILE, 1);
	cfile->rack = 0;
	cfile->mask = 0x80;
	MALLOC(cfile->buffer,char,4096*2);
		
	if (strcmpi( mode, "rc")==0 )	{
		cfile->file = LibraryGetFileInfo( filename, &cfile->others_use, &cfile->lib_offset, &cfile->compressed_size, &cfile->org_size, &cfile->compressed, cfile->buffer );
		if (cfile->file==NULL )	{
			free(cfile->buffer);
			free(cfile);
			return NULL;
		}
		raw_seek( cfile, 0 );
		if (!cfile->compressed) 	{
			cfile->position = 0;
			cfile->readonly=1;	// Read
			return cfile;
		}
		compress_init_storage(cfile);
		compress_init_dict(cfile);
		cfile->old_code = UNUSED;
		cfile->readonly=1;	// Read
		cfile->position = 0;
		cfile->count = 0;
		//printf( "File is %d bytes, orginal is %d bytes\n", cfile->compressed_size, cfile->org_size );
		return cfile;
	} else if (strcmpi( mode, "wc")==0 )	{
		compress_init_storage(cfile);
		compress_init_dict(cfile);
		cfile->old_code = UNUSED;
		cfile->readonly=0;	// Write
		cfile->compressed=1;
		cfile->string_code = -1;
		cfile->position = 0;
		cfile->count = 0;
		cfile->compressed_size = 0;
		cfile->file=fopen( filename, "wb" );
		setvbuf( cfile->file, cfile->buffer, _IOFBF, 4096*2 );
		cfile->others_use = 0;
		cfile->lib_offset = 6;
		cfile->org_size = 0;
		fwrite( &signature, 2, sizeof(char), cfile->file );
		fwrite( &cfile->org_size, sizeof(int), 1, cfile->file );
		raw_seek( cfile, 0 );
		return cfile;
	}

	cfile->file = fopen( filename, mode );
	setvbuf( cfile->file, cfile->buffer, _IOFBF, 4096*2 );
	cfile->org_size = filelength( fileno( cfile->file ));
	cfile->compressed_size = cfile->org_size;
	cfile->readonly=0;	
	cfile->position = 0;
	cfile->raw_position = 0;
	cfile->compressed = 0;
	cfile->lib_offset = 0;
	cfile->others_use = 0;
	return cfile;
}

int cfilelength( CFILE *fp )
{
	if ( fp->org_size < fp->position )
		return fp->position;
	else
		return fp->org_size;
}

int cfputc( int c, CFILE *fp )
{
	int character;
	unsigned int index;

	if (!fp->compressed)
		return raw_putc( c, fp );

	character = c & 0xFF;

	if (fp->string_code < 0 )	{
		fp->string_code = character;
		fp->position++;
		return character;
	}
	
	fp->position++;
	index = compress_find_child_node( fp, fp->string_code, character );
	if ( fp->dict[ index ].code_value != - 1 )
		fp->string_code = fp->dict[ index ].code_value;
	else	{
		fp->dict[ index ].code_value = fp->next_code++;
		fp->dict[ index ].parent_code = fp->string_code;
		fp->dict[ index ].character = (char) character;
		compress_output_bits( fp, (unsigned long)fp->string_code, fp->current_code_bits );
		fp->string_code = character;
		if ( fp->next_code > MAX_CODE ) {
			compress_output_bits( fp,(unsigned long)FLUSH_CODE, fp->current_code_bits );
			compress_init_dict(fp);
		} else if ( fp->next_code > fp->next_bump_code ) {
			compress_output_bits( fp,(unsigned long)BUMP_CODE, fp->current_code_bits );
			fp->current_code_bits++;
			fp->next_bump_code <<= 1;
			fp->next_bump_code |= 1;
		}
	}
	return character;
}

int cfgetc( CFILE * fp ) {
	int new_character;

	if ( !fp->compressed )
		return raw_getc( fp );

	if (fp->position >= fp->org_size )
		return EOF;

	if ( fp->old_code == UNUSED )	{
		fp->old_code = (unsigned int) compress_input_bits( fp, fp->current_code_bits );
		if ( fp->old_code == END_OF_STREAM ) {
			fp->position = fp->org_size;
			return EOF;
		}
		fp->count = 0;
		fp->character = fp->old_code;
		fp->position++;
		return fp->character;
	}

	if ( fp->count > 0 ) {
		new_character = ( unsigned char )fp->decode_stack[ --fp->count ];
		fp->position++;
		if (fp->count==0 )	{
			fp->dict[ fp->next_code ].parent_code = fp->old_code;
			fp->dict[ fp->next_code ].character = (char) fp->character;
			fp->next_code++;
			fp->old_code = fp->new_code;
		}
		return new_character;
	}

	for ( ; ; )	{

		fp->new_code = (unsigned int) compress_input_bits( fp, fp->current_code_bits );

		switch( fp->new_code )
		{
		case END_OF_STREAM:
			fp->position = fp->org_size;
			return EOF;
	
		case FLUSH_CODE:
			compress_init_dict(fp);
			fp->old_code = (unsigned int) compress_input_bits( fp, fp->current_code_bits );
			if ( fp->old_code == END_OF_STREAM ) {
				fp->position = fp->org_size;
				return EOF;
			}
			fp->count = 0;
			fp->character = fp->old_code;
			fp->position++;
			return fp->character;
			
		case BUMP_CODE:
			fp->current_code_bits++;
			break;

		default:
			if ( fp->new_code >= fp->next_code ) {
				fp->decode_stack[ 0 ] = (char) fp->character;
				fp->count = compress_decode_string( fp, 1, fp->old_code );
			} else {
				fp->count = compress_decode_string( fp, 0, fp->new_code );
			}
	            
			fp->character = fp->decode_stack[ fp->count - 1 ];

			new_character = ( unsigned char )fp->decode_stack[ --fp->count ];
			fp->position++;
			if (fp->count==0 )	{
				fp->dict[ fp->next_code ].parent_code = fp->old_code;
				fp->dict[ fp->next_code ].character = (char) fp->character;
				fp->next_code++;
				fp->old_code = fp->new_code;
			}
			return new_character;
		}
	}
}

size_t cfread( void * buf, size_t elsize, size_t nelem, CFILE * fp ) 
{
	int c, i = 0;
	unsigned char * buffer = (unsigned char *)buf;

	if ( !fp->compressed )	{
		//if ((cfile->raw_position+(elsize*nelem)) > cfile->org_size ) return EOF;
		//i = fread( buf, elsize, nelem, fp->file );
		//fp->raw_position += i*elsize;
		//	return i;
		//return raw_fread( buf, elsize, nelem, fp->file );
		
		for ( i=0; i<(elsize*nelem); i++ )
		{
			c = raw_getc( fp );
			if ( c==EOF) return i/elsize;
			*buffer++ = c;
		}
	   return nelem;
	}

	//printf( "Reading %d compressed bytes..\n", elsize*nelem );

	for ( i=0; i<(elsize*nelem); i++ )
	{
		c = cfgetc( fp );
		if ( c==EOF) return i/elsize;
		*buffer++ = c;
	}

	return nelem;
}
		
size_t cfwrite( const void * buf, size_t elsize, size_t nelem, CFILE *fp )
{
	int c, i = 0;
	unsigned char * buffer = (unsigned char *)buf;

	if ( !fp->compressed )	{
		//return fwrite( buf, elsize, nelem, fp->file );
		for ( i=0; i<(elsize*nelem); i++ )
		{
			c = *buffer++;
			if (raw_putc( c, fp )!=c)
				return i/elsize;
		}
		return nelem;
	}
	
	for ( i=0; i<(elsize*nelem); i++ )
	{
		c = *buffer++;
		if (cfputc( c, fp )!=c)
			return i/elsize;
	}
	return nelem;
}

int cftell( CFILE *fp )	{
	if ( !fp->compressed )
		return fp->raw_position;

	return fp->position;
}

int cfseek( CFILE *fp, long int offset, int where )
{
	int c, goal_position;

	if ( !fp->readonly && fp->compressed ) return 3;	// Can only seek on readonly files.

	switch( where )
	{
	case SEEK_SET:
		goal_position = offset;
		break;
	case SEEK_CUR:
		if (fp->compressed)
			goal_position = fp->position+offset;
		else
			goal_position = fp->raw_position+offset;
		break;
	case SEEK_END:
		goal_position = fp->org_size+offset;
		break;
	default:
		return 1;
	}	

	if ( !fp->compressed )	{
		c = fseek( fp->file, fp->lib_offset + goal_position, SEEK_SET );
		fp->raw_position = ftell(fp->file)-fp->lib_offset;
		return c;
	}

	
	if ( (goal_position<0) || (goal_position>=fp->org_size) )
		return 1;

	if ( goal_position == fp->position ) 
		return 0;	// Already there!
	
	if ( goal_position < fp->position ) 	{
		// We need to back track...
		compress_init_dict(fp);
		fp->old_code = UNUSED;
		fp->position = 0;
		fp->raw_position = 0;
		fp->count = 0;
		fp->rack = 0;
		fp->mask = 0x80;
		raw_seek( fp, 0 );
		//fread( &signature, sizeof(unsigned int), 1, fp->file );
		//if (signature != 0xCFCF )	return 1;
		//fread( &fp->compressed_size, sizeof(int), 1, fp->file );
	}
	
	// Read forward to goal_position...
	while( fp->position < goal_position  )
	{
		c = cfgetc( fp );
		if (c==EOF) return 2;
	}
	return 0;
}

void cfclose( CFILE * fp ) {	
	char signature[3] = "CF";

	Assert( fp!=NULL );

	if (!fp->compressed)	{
		if ( !fp->others_use )
			fclose(fp->file);
		if (fp->buffer) free(fp->buffer);
		free(fp);
		return;
	}

	if (!fp->readonly)	{	
		compress_output_bits( fp, (unsigned long) fp->string_code, fp->current_code_bits );
		compress_output_bits( fp, (unsigned long) END_OF_STREAM, fp->current_code_bits);
		if ( fp->mask != 0x80 )	{
			if ( putc( fp->rack, fp->file ) != fp->rack )
				Error( "Fatal error in CloseBitFile" );
		}		 
		fp->org_size = fp->position;
		fseek( fp->file, 0L, SEEK_SET );
		fwrite( signature, 2, sizeof(char), fp->file );
		fwrite( &fp->org_size, 1, sizeof(int), fp->file );
		//printf( "File wrote is %d bytes originally\n", fp->org_size );
	}
	if (!fp->others_use )
		fclose(fp->file);
	compress_free_storage(fp);
	if (fp->buffer) free(fp->buffer);
	free(fp);

}



