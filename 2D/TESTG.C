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
 * $Source: f:/miner/source/2d/rcs/testg.c $
 * $Revision: 1.19 $
 * $Author: john $
 * $Date: 1994/09/12 19:28:10 $
 *
 * Program to test the graphics package.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#include "gr.h"
#include "key.h"
#include "timer.h"
#include "grdef.h"
#include "mono.h"
#include "fix.h"
#include "iff.h"
#include "palette.h"
#include "rle.h"
#include "pcx.h"

void ReadBitmap( char * filename, grs_bitmap * bmp, ubyte * palette )
{
	int iff_error;

	iff_error=iff_read_bitmap( filename, bmp, BM_LINEAR, palette );
	if (iff_error != IFF_NO_ERROR)	{
		printf("Error loading bitmap <%s>, error=%d",filename,iff_error);
		exit(1);
	}
			
}


ubyte getpixel( grs_bitmap * bmp, int x, int y )
{
	return bmp->bm_data[y*320+x];
}

#define EPSILON (0.0000001)

float cfactor = 1.57;

int myx = 0;

ubyte getpixel1( grs_bitmap * bmp, int x, int y )
{
	float xmid, ymid, xx, yy, delta, dist, factor;
	
	xmid = (bmp->bm_w - 1 ) / 2.0;	xx = x - xmid;
	ymid = (bmp->bm_h - 1 ) / 2.0;   yy = y - ymid;

	delta = xx*xx + yy*yy;

	if ( delta < EPSILON )
		dist = 0.0;
	else
		dist = 1.01 * sqrt(delta) / min(xmid,ymid);
	
	if ( dist > 1.0 )
		return 0;
	else if ( dist < EPSILON )
		factor = 1.0 / cfactor;
	else
		factor = asin(dist) / (dist * cfactor );
	
	xx = xx * factor; yy = yy * factor;
	x = xx + xmid + myx; y = yy+ymid;
	x = x % bmp->bm_w;
	
	return bmp->bm_data[y*320+x];
}


void circle_in_bitmap( grs_bitmap * bmp )
{
	int i, x, y;

	for ( i=0; i < 10; i++ )	{
		for ( y=0; y<200; y++ )	{
			for ( x=0; x<319; x++ )	{
			 	gr_setcolor(getpixel1(bmp,x,y));
				gr_pixel( x, y );
			}
		}
		myx += 2;
		getch();
	}
}

int descent_critical_error;


main(int argc, char * argv[] )
{
	int x, y;
	grs_bitmap bmp;
	grs_bitmap bmp1;
	ubyte palette[768];

	minit();

	printf( "Reading %s...\n", "john.pcx" );
	gr_init( SM_320x200U );
	bmp.bm_data = NULL;
	pcx_read_bitmap( "big.pcx", &bmp, BM_LINEAR, palette );
	gr_palette_load( palette );
	key_init();

	x = y = 0;
	while(!keyd_pressed[KEY_ESC]) {
		y += keyd_pressed[KEY_UP] - keyd_pressed[KEY_DOWN];
		x += keyd_pressed[KEY_LEFT] - keyd_pressed[KEY_RIGHT];
		gr_bitmap( x, y, &bmp );
	}

}



