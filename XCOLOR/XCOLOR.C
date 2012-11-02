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
	ubyte tc1, tc2;
	int i;
	grs_bitmap * bitmap;

	//MALLOC( bitmap, grs_bitmap, 1 );//Hack by KRB
	bitmap = (grs_bitmap *)malloc(1*sizeof(grs_bitmap));

	printf( "Reading %s...", filename );
	iff_read_bitmap( filename, bitmap, BM_LINEAR, palette );
	printf( "\n" );

	if ( iff_has_transparency )	{
		tc1 = bitmap->bm_data[0];
		tc2 = iff_transparent_color;
		printf( "Mapping all color %d's to transparent color %d...", tc1, tc2 );
		for (i=0; i < bitmap->bm_h * bitmap->bm_w; i++ )	{
			if  ( bitmap->bm_data[i]==tc1 )
				bitmap->bm_data[i] = tc2;
		}		
	} else {
		tc1 = bitmap->bm_data[0];
		iff_has_transparency = 1;
		iff_transparent_color = tc1;
		tc2 = iff_transparent_color;
		printf( "Making color %d the new transparency color...", tc1 );
	}
	printf( "\n" );

	printf( "Writing %s...", filename );
	iff_write_bitmap(filename,bitmap, palette );
	printf( "\n" );

	gr_free_bitmap(bitmap);
}

void main(int argc, char * argv[])	{
	int numfiles = 0;
 	struct find_t find;
	char * cp;
	char * cp1;

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
		printf( "xcolor- this program corrects a .bbm file's transparency\n");
		printf( "by making all pixels that are the same as the upper-left\n");
		printf( "pixel be transparent. The command line is the file specs\n" );
		printf( "to convert. ie.. xcolor john*.bbm adam*.bbm\n\n" );
		exit(1);
	}


}






