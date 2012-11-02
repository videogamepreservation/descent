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
 * $Source: f:/miner/source/2d/rcs/vesatest.c $
 * $Revision: 1.3 $
 * $Author: john $
 * $Date: 1993/12/21 11:40:47 $
 * 
 * .
 * 
 * $Log: vesatest.c $
 * Revision 1.3  1993/12/21  11:40:47  john
 * *** empty log message ***
 * 
 * Revision 1.2  1993/12/09  17:14:14  john
 * VESA test program.
 * 
 * Revision 1.1  1993/12/09  16:53:01  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: vesatest.c 1.3 1993/12/21 11:40:47 john Exp $";
#pragma on (unreferenced)

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>

#include "gr.h"

char palette[256*3];
extern void gr_pal_setblock( int start, int number, unsigned char * pal );

void CreatePalette( )
{
	int i;

	for (i=0; i<64; i++ )
	{
		// Make 0-63 be red shades
		palette[ i*3+0 ] = i;	// Red
		palette[ i*3+1 ] = 0;	// Green		
		palette[ i*3+2 ] = 0;	// Blue

		// Make 64-127 be green shades
		palette[ i*3+0+64*3 ] = 0;	// Red
		palette[ i*3+1+64*3 ] = i;	// Green		
		palette[ i*3+2+64*3 ] = 0;	// Blue

		// Make 128-191 be blue shades
		palette[ i*3+0+128*3 ] = 0;	// Red
		palette[ i*3+1+128*3 ] = 0;	// Green		
		palette[ i*3+2+128*3 ] = i;	// Blue
		
		// Make 192-255 be greyscale
		palette[ i*3+0+192*3 ] = i;	// Red
		palette[ i*3+1+192*3 ] = i;	// Green		
		palette[ i*3+2+192*3 ] = i;	// Blue
	}
}

main()
{
	int i;

	printf( "This program tests VESA mode 103 (800x600x256).\n" );
	printf( "You should see a dark blue screen with a red border,\n" );
	printf( "When you hit return once, there should be three\n" );
	printf( "horizontal color bands, each one going through\n" );
	printf( "shades of red,green,blue, and white, respectively,\n" );
	printf( "and all this surrounded by a white frame.\n" );
	printf( "Press any key...\n" );

	getch();

	CreatePalette();

	i = gr_init( SM_800x600V );

	if (i)	{
		printf( "Couldn't initialize because " );
		
		switch(i)
		{
			case 1:	printf( "no VGA adapter is installed.\n" ); break;
			case 2:  printf( "Inferno doesn't support this cards granularity.\n" ); break;
			case 3:	printf( "your monitor doesn't support 800x600.\n" ); break;
			case 4:	printf( "your video card doesn't support that VESA mode.\n" ); break;
			case 5:	printf( "no VESA driver was detected.\n" ); break;
			case 6:	printf( "a VESA call returned a bad status code.\n" ); break;
			case 7:	printf( "there isn't enough low memory.\n" ); break;
			case 8:	printf( "a DPMI call failed.\n" ); break;
			case 9:  printf( "your card couldn't set the correct logical line width.\n" ); break;
			default: printf( "of unknown error %d\n" ); break;
			exit(1);
		}
	}
	
	gr_setcolor( 1 );
	gr_urect( 0, 0, 799, 599 );
	gr_setcolor( 12 );
	gr_ubox(0,0,799, 599);

	getch();

	gr_pal_setblock( 0, 256, palette );

	gr_setcolor( 0 );
	gr_urect( 0, 0, grd_curcanv->cv_bitmap.bm_w-1, grd_curcanv->cv_bitmap.bm_h-1 );

	for (i=0; i< 256; i++ )
	{
			gr_setcolor( i );
			gr_urect( 0, i*2, 799, i*2+1 );
	}
	gr_setcolor( 255 ); gr_ubox(0,0,799, 599);
	gr_setcolor( 245 ); gr_ubox(1,1,798, 598);
	gr_setcolor( 235 ); gr_ubox(2,2,797, 597);
	gr_setcolor( 225 ); gr_ubox(3,3,796, 596);
	gr_setcolor( 215 ); gr_ubox(4,4,795, 595);
	gr_setcolor( 205 ); gr_ubox(5,5,794, 594);

	getch();

}
	






