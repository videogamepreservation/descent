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
 * $Source: f:/miner/source/2d/rcs/pixel.c $
 * $Revision: 1.5 $
 * $Author: john $
 * $Date: 1994/11/18 22:50:26 $
 *
 * Graphical routines for setting a pixel.
 *
 * $Log: pixel.c $
 * Revision 1.5  1994/11/18  22:50:26  john
 * Changed shorts to ints in parameters.
 * 
 * Revision 1.4  1993/10/15  16:22:26  john
 * *** empty log message ***
 * 
 * Revision 1.3  1993/09/29  17:31:27  john
 * optimized vesa pixel stuff
 * 
 * Revision 1.2  1993/09/29  16:15:15  john
 * optimized
 * 
 * Revision 1.1  1993/09/08  11:44:09  john
 * Initial revision
 * 
 *
 */

#include "mem.h"

#include "gr.h"
#include "grdef.h"


void gr_upixel( int x, int y )
{
	switch (TYPE)
	{
	case BM_LINEAR:
		DATA[ ROWSIZE*y+x ] = COLOR;
		return;
	case BM_MODEX:
		gr_modex_setplane( (x+XOFFSET) & 3 );
		gr_video_memory[(ROWSIZE * (y+YOFFSET)) + ((x+XOFFSET)>>2)] = COLOR;
		return;
	case BM_SVGA:
		gr_vesa_pixel( COLOR, (unsigned int)DATA + (unsigned int)ROWSIZE * y + x);
		return;
	}
}

void gr_pixel( int x, int y )
{
	if ((x<0) || (y<0) || (x>=WIDTH) || (y>=HEIGHT)) return;

	switch (TYPE)
	{
	case BM_LINEAR:
		DATA[ ROWSIZE*y+x ] = COLOR;
		return;
	case BM_MODEX:
		gr_modex_setplane( (x+XOFFSET) & 3 );
		gr_video_memory[(ROWSIZE * (y+YOFFSET)) + ((x+XOFFSET)>>2)] = COLOR;
		return;
	case BM_SVGA:
		gr_vesa_pixel( COLOR, (unsigned int)DATA + (unsigned int)ROWSIZE * y + x);
		return;
	}
}

void gr_bm_upixel( grs_bitmap * bm, int x, int y, unsigned char color )
{
	switch (bm->bm_type)
	{
	case BM_LINEAR:
		bm->bm_data[ bm->bm_rowsize*y+x ] = color;
		return;
	case BM_MODEX:
		x += bm->bm_x;
		y += bm->bm_y;
		gr_modex_setplane( x & 3 );
		gr_video_memory[(bm->bm_rowsize * y) + (x/4)] = color;
		return;
	case BM_SVGA:
		gr_vesa_pixel(color,(unsigned int)bm->bm_data + (unsigned int)bm->bm_rowsize * y + x);
		return;
	}
}

void gr_bm_pixel( grs_bitmap * bm, int x, int y, unsigned char color )
{
	if ((x<0) || (y<0) || (x>=bm->bm_w) || (y>=bm->bm_h)) return;

	switch (bm->bm_type)
	{
	case BM_LINEAR:
		bm->bm_data[ bm->bm_rowsize*y+x ] = color;
		return;
	case BM_MODEX:
		x += bm->bm_x;
		y += bm->bm_y;
		gr_modex_setplane( x & 3 );
		gr_video_memory[(bm->bm_rowsize * y) + (x/4)] = color;
		return;
	case BM_SVGA:
		gr_vesa_pixel(color,(unsigned int)bm->bm_data + (unsigned int)bm->bm_rowsize * y + x);
		return;
	}
}


