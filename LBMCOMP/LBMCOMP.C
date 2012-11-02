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
 * $Source: h:/miner/source/xcolor/RCS/xcolor.c $
 * $Revision: 1.1 $
 * $Author: john $
 * $Date: 1994/01/24 11:09:24 $
 * 
 * .
 * 
 * $Log: xcolor.c $
 * Revision 1.1  1994/01/24  11:09:24  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: xcolor.c 1.1 1994/01/24 11:09:24 john Exp $";
#pragma on (unreferenced)

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "gr.h"
#include "iff.h"
#include "mem.h"

ubyte palette[768];

void dofile( char * filename )
{
	grs_bitmap * bitmap;

	MALLOC( bitmap, grs_bitmap, 1 );	

	printf( "Compressing %s... Reading,", filename );
	iff_read_bitmap( filename, bitmap, BM_LINEAR, palette );
	printf( "Writing" );
	iff_write_bitmap(filename,bitmap, palette );
	printf( "," );
	gr_free_bitmap(bitmap);
	printf( "Done.\n" );
}

void main(int argc, char * argv[])	{
	int numfiles = 0;
 	struct find_t find;
	char * cp;
	char * cp1;

	setbuf(stdout, NULL);	// unbuffered output via printf

	argv++; argc--;
	for (;argc--;argv++)
	{
		if( !_dos_findfirst( *argv, 0xffff, &find ) )
		{
			dofile( find.name );
			numfiles++;
			while( !_dos_findnext( &find ) )	{
				numfiles++;
				dofile( find.name );
			}
	 	}
	}

	if ( numfiles < 1 )	{
		printf( "lbmcomp- this program compresses bbm files by removing\n");
		printf( "unused chunks. The command line is the file specs\n" );
		printf( "to convert. ie.. lbmcomp john*.bbm adam*.bbm\n\n" );
		exit(1);
	}


}






