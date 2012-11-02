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
 * $Source: f:/miner/source/2d/rcs/scanline.c $
 * $Revision: 1.7 $
 * $Author: john $
 * $Date: 1994/11/18 22:50:48 $
 *
 * Graphical routines for drawing solid scanlines.
 *
 * $Log: scanline.c $
 * Revision 1.7  1994/11/18  22:50:48  john
 * Changed a bunch of shorts to ints in calls.
 * 
 * Revision 1.6  1994/09/02  11:40:32  john
 * fixed bug with urect scanline drakening still
 * only using 16 levels of fade.
 * 
 * Revision 1.5  1994/04/08  16:59:12  john
 * Add fading poly's; Made palette fade 32 instead of 16.
 * 
 * Revision 1.4  1994/03/22  18:36:27  john
 * Added darkening scanlines
 * 
 * Revision 1.3  1993/10/15  16:22:52  john
 * y
 * 
 * Revision 1.2  1993/09/08  11:56:29  john
 * neatened
 * 
 * Revision 1.1  1993/09/08  11:44:27  john
 * Initial revision
 * 
 *
 */

#include "mem.h"

#include "gr.h"
#include "grdef.h"

int Gr_scanline_darkening_level = GR_FADE_LEVELS;

void gr_linear_darken( ubyte * dest, int darkening_level, int count, ubyte * fade_table );
#pragma aux gr_linear_darken parm [edi] [eax] [ecx] [edx] modify exact [eax ebx ecx edx edi] = \
"					xor	ebx, ebx					"	\
"					mov	bh, al					"  \
"gld_loop:		mov	bl, [edi]				"	\
"					mov	al, [ebx+edx]			"	\
"					mov	[edi], al				"	\
"					inc	edi						"	\
"					dec	ecx						"	\
"					jnz	gld_loop					"	

void gr_uscanline( int x1, int x2, int y )
{
	if (Gr_scanline_darkening_level >= GR_FADE_LEVELS )	{
		switch(TYPE)
		{
		case BM_LINEAR:
			gr_linear_stosd( DATA + ROWSIZE*y + x1, COLOR, x2-x1+1);
			break;
		case BM_MODEX:
			gr_modex_uscanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		case BM_SVGA:
			gr_vesa_scanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		}
	} else {
		switch(TYPE)
		{
		case BM_LINEAR:
			gr_linear_darken( DATA + ROWSIZE*y + x1, Gr_scanline_darkening_level, x2-x1+1, gr_fade_table);
			break;
		case BM_MODEX:
			gr_modex_uscanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		case BM_SVGA:
			gr_vesa_scanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		}
	}
}

void gr_scanline( int x1, int x2, int y )
{
	if ((y<0)||(y>MAXY)) return;

	if (x2 < x1 ) x2 ^= x1 ^= x2;

	if (x1 > MAXX) return;
	if (x2 < MINX) return;

	if (x1 < MINX) x1 = MINX;
	if (x2 > MAXX) x2 = MAXX;

	if (Gr_scanline_darkening_level >= GR_FADE_LEVELS )	{
		switch(TYPE)
		{
		case BM_LINEAR:
			gr_linear_stosd( DATA + ROWSIZE*y + x1, COLOR, x2-x1+1);
			break;
		case BM_MODEX:
			gr_modex_uscanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		case BM_SVGA:
			gr_vesa_scanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		}
	} else {
		switch(TYPE)
		{
		case BM_LINEAR:
			gr_linear_darken( DATA + ROWSIZE*y + x1, Gr_scanline_darkening_level, x2-x1+1, gr_fade_table);
			break;
		case BM_MODEX:
			gr_modex_uscanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		case BM_SVGA:
			gr_vesa_scanline( x1+XOFFSET, x2+XOFFSET, y+YOFFSET, COLOR );
			break;
		}
	}
}


