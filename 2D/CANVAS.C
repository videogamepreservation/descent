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
 * $Source: f:/miner/source/2d/rcs/canvas.c $
 * $Revision: 1.14 $
 * $Author: john $
 * $Date: 1995/03/06 09:18:45 $
 *
 * Graphical routines for manipulating grs_canvas's.
 *
 * $Log: canvas.c $
 * Revision 1.14  1995/03/06  09:18:45  john
 * Made modex page flipping wait for retrace be default.
 * 
 * Revision 1.13  1995/03/01  15:37:40  john
 * Better ModeX support.
 * 
 * Revision 1.12  1994/11/28  17:08:29  john
 * Took out some unused functions in linear.asm, moved
 * gr_linear_movsd from linear.asm to bitblt.c, made sure that
 * the code in ibiblt.c sets the direction flags before rep movsing.
 * 
 * Revision 1.11  1994/11/18  22:50:24  john
 * Changed shorts to ints in parameters.
 * 
 * Revision 1.10  1994/11/10  15:59:33  john
 * Fixed bugs with canvas's being created with bogus bm_flags.
 * 
 * Revision 1.9  1994/06/24  17:26:34  john
 * Made rowsizes bigger than actual screen work with SVGA.
 * 
 * Revision 1.8  1994/05/06  12:50:41  john
 * Added supertransparency; neatend things up; took out warnings.
 * 
 * Revision 1.7  1993/12/08  16:41:26  john
 * fixed color = -1 bug
 * 
 * Revision 1.6  1993/10/15  16:22:25  john
 * *** empty log message ***
 * 
 * Revision 1.5  1993/09/29  16:14:07  john
 * added globol variables describing current canvas
 * 
 * Revision 1.4  1993/09/14  16:03:40  matt
 * Added new function, gr_clear_canvas()
 * 
 * Revision 1.3  1993/09/14  13:51:38  matt
 * in gr_init_sub_canvas(), copy bm_rowsize from source canvas
 * 
 * Revision 1.2  1993/09/08  17:37:34  john
 * Checking for potential errors
 * 
 * Revision 1.1  1993/09/08  11:43:18  john
 * Initial revision
 * 
 *
 */

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include "mem.h"


#include "gr.h"
#include "grdef.h"

grs_canvas * grd_curcanv;    //active canvas
grs_screen * grd_curscreen;  //active screen

grs_canvas *gr_create_canvas(int w, int h)
{
	unsigned char * data;
	grs_canvas *new;
	
	new = (grs_canvas *)malloc( sizeof(grs_canvas) );
	data = (unsigned char *)malloc(w*h);

	new->cv_bitmap.bm_x = 0;
	new->cv_bitmap.bm_y = 0;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
	new->cv_bitmap.bm_type = BM_LINEAR;
	new->cv_bitmap.bm_rowsize = w;
	new->cv_bitmap.bm_data = data;

	new->cv_color = 0;
	new->cv_drawmode = 0;
	new->cv_font = NULL;
	new->cv_font_fg_color = 0;
	new->cv_font_bg_color = 0;
	return new;
}

grs_canvas *gr_create_sub_canvas(grs_canvas *canv, int x, int y, int w, int h)
{
    grs_canvas *new;

    new = (grs_canvas *)malloc( sizeof(grs_canvas) );

	new->cv_bitmap.bm_x = x+canv->cv_bitmap.bm_x;
	new->cv_bitmap.bm_y = y+canv->cv_bitmap.bm_y;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
	new->cv_bitmap.bm_type = canv->cv_bitmap.bm_type;
	new->cv_bitmap.bm_rowsize = canv->cv_bitmap.bm_rowsize;

	new->cv_bitmap.bm_data = canv->cv_bitmap.bm_data;
	new->cv_bitmap.bm_data += y*canv->cv_bitmap.bm_rowsize;
	new->cv_bitmap.bm_data += x;

	new->cv_color = canv->cv_color;
   new->cv_drawmode = canv->cv_drawmode;
   new->cv_font = canv->cv_font;
	new->cv_font_fg_color = canv->cv_font_fg_color;
	new->cv_font_bg_color = canv->cv_font_bg_color;
   return new;
}

void gr_init_canvas(grs_canvas *canv, unsigned char * pixdata, int pixtype, int w, int h)
{
    canv->cv_color = 0;
    canv->cv_drawmode = 0;
    canv->cv_font = NULL;
	canv->cv_font_fg_color = 0;
	canv->cv_font_bg_color = 0;

	canv->cv_bitmap.bm_x = 0;
	canv->cv_bitmap.bm_y = 0;
	if (pixtype==BM_MODEX)
		canv->cv_bitmap.bm_rowsize = w / 4;
	else
		canv->cv_bitmap.bm_rowsize = w;
	canv->cv_bitmap.bm_w = w;
	canv->cv_bitmap.bm_h = h;
	canv->cv_bitmap.bm_flags = 0;
	canv->cv_bitmap.bm_type = pixtype;
	canv->cv_bitmap.bm_data = pixdata;

}

void gr_init_sub_canvas(grs_canvas *new, grs_canvas *src, int x, int y, int w, int h)
{
	new->cv_color = src->cv_color;
	new->cv_drawmode = src->cv_drawmode;
	new->cv_font = src->cv_font;
	new->cv_font_fg_color = src->cv_font_fg_color;
	new->cv_font_bg_color = src->cv_font_bg_color;

	new->cv_bitmap.bm_x = src->cv_bitmap.bm_x+x;
	new->cv_bitmap.bm_y = src->cv_bitmap.bm_y+y;
	new->cv_bitmap.bm_w = w;
	new->cv_bitmap.bm_h = h;
	new->cv_bitmap.bm_flags = 0;
	new->cv_bitmap.bm_type = src->cv_bitmap.bm_type;
	new->cv_bitmap.bm_rowsize = src->cv_bitmap.bm_rowsize;


	new->cv_bitmap.bm_data = src->cv_bitmap.bm_data;
	new->cv_bitmap.bm_data += y*src->cv_bitmap.bm_rowsize;
	new->cv_bitmap.bm_data += x;
}

void gr_free_canvas(grs_canvas *canv)
{
	free(canv->cv_bitmap.bm_data );
    free(canv);
}

void gr_free_sub_canvas(grs_canvas *canv)
{
    free(canv);
}

int gr_wait_for_retrace = 1;

void gr_show_canvas( grs_canvas *canv )
{
	if (canv->cv_bitmap.bm_type == BM_MODEX )
		gr_modex_setstart( canv->cv_bitmap.bm_x, canv->cv_bitmap.bm_y, gr_wait_for_retrace );

	else if (canv->cv_bitmap.bm_type == BM_SVGA )
		gr_vesa_setstart( canv->cv_bitmap.bm_x, canv->cv_bitmap.bm_y );

		//	else if (canv->cv_bitmap.bm_type == BM_LINEAR )
		// Int3();		// Get JOHN!
		//gr_linear_movsd( canv->cv_bitmap.bm_data, (void *)0xA0000, 320*200);
}

void gr_set_current_canvas( grs_canvas *canv )
{
	if (canv==NULL)
		grd_curcanv = &(grd_curscreen->sc_canvas);
	else
		grd_curcanv = canv;

	if ( (grd_curcanv->cv_color >= 0) && (grd_curcanv->cv_color <= 255) )	{
		gr_var_color = grd_curcanv->cv_color;
	} else
		gr_var_color  = 0;
	gr_var_bitmap = grd_curcanv->cv_bitmap.bm_data;
	gr_var_bwidth = grd_curcanv->cv_bitmap.bm_rowsize;

}

void gr_clear_canvas(int color)
{
	gr_setcolor(color);
	gr_rect(0,0,WIDTH-1,HEIGHT-1);
}

void gr_setcolor(int color)
{
	grd_curcanv->cv_color=color;

	gr_var_color = color;

}

