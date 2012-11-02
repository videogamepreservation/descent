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
 * $Source: f:/miner/source/2d/rcs/gr.c $
 * $Revision: 1.56 $
 * $Author: john $
 * $Date: 1995/05/08 11:22:47 $
 *
 * Graphical routines for setting video modes, etc.
 *
 * $Log: gr.c $
 * Revision 1.56  1995/05/08  11:22:47  john
 * Added 320x400 3dbios mode.
 * 
 * Revision 1.55  1995/02/02  16:44:05  john
 * Again with prev.
 * 
 * Revision 1.54  1995/02/02  16:42:18  john
 * Fixed palette with text fading out.
 * 
 * Revision 1.53  1995/02/02  14:26:20  john
 * Made palette fades work better with gamma thingy..
 * 
 * Revision 1.52  1995/02/02  14:08:59  john
 * Made palette gamma reset to 0 before exiting to Dos.
 * 
 * Revision 1.51  1995/01/30  18:06:35  john
 * Added text screen fade in/out, and restored video mode properly.
 * 
 * Revision 1.50  1995/01/24  17:58:16  john
 * Added code to return to 80x25 when graphics close.
 * 
 * Revision 1.49  1994/11/30  20:16:05  john
 * Fixed bug that the grd_curscreen flags were never initialized.
 * 
 * Revision 1.48  1994/11/24  13:24:43  john
 * Made sure that some rep movs had the cld set first.
 * Took some unused functions out.
 * 
 * Revision 1.47  1994/11/18  22:50:20  john
 * Changed shorts to ints in parameters.
 * 
 * Revision 1.46  1994/11/15  18:28:36  john
 * Made text screen fade in.
 * 
 * Revision 1.45  1994/11/15  17:55:11  john
 * Made text palette fade in when game over.
 * 
 * Revision 1.44  1994/11/07  12:07:30  john
 * Made save/restore cursor work a bit better.
 * 
 * Revision 1.43  1994/11/05  12:46:44  john
 * Changed palette stuff a bit.
 * 
 * Revision 1.42  1994/10/26  23:55:50  john
 * Took out roller; Took out inverse table.
 * 
 * Revision 1.41  1994/10/10  13:59:50  john
 * *** empty log message ***
 * 
 * Revision 1.40  1994/10/10  13:58:50  john
 * Added better VGA detection scheme.
 * 
 * Revision 1.39  1994/09/29  10:09:15  john
 * Hacked out VGA card detection for now.
 * 
 * Revision 1.38  1994/09/22  17:35:35  john
 * Fixed bug with palette not reloading in
 * gr_set_mode
 * 
 * Revision 1.37  1994/09/22  16:08:42  john
 * Fixed some palette stuff.
 * 
 * Revision 1.36  1994/09/19  11:44:23  john
 * Changed call to allocate selector to the dpmi module.
 * 
 * Revision 1.35  1994/09/12  19:28:11  john
 * Fixed bug with unclipped fonts clipping.
 * 
 * Revision 1.34  1994/09/12  18:20:18  john
 * Made text fade out
 * 
 * Revision 1.33  1994/09/12  14:40:15  john
 * Neatend.
 * 
 * Revision 1.32  1994/08/15  15:01:01  matt
 * Set ptr to NULL after freeing
 * 
 * Revision 1.31  1994/07/27  18:30:28  john
 * Took away the blending table.
 * 
 * Revision 1.30  1994/06/24  17:26:59  john
 * Made rowsizes bigger than actual screen work with SVGA.
 * 
 * Revision 1.29  1994/05/12  17:33:16  john
 * Added circle code.
 * 
 * Revision 1.28  1994/05/10  19:51:49  john
 * Added 320x100 mode.
 * 
 * Revision 1.27  1994/05/06  12:50:23  john
 * Added supertransparency; neatend things up; took out warnings.
 * 
 * Revision 1.26  1994/05/03  19:39:00  john
 * *** empty log message ***
 * 
 * Revision 1.25  1994/04/08  16:59:34  john
 * Add fading poly's; Made palette fade 32 instead of 16.
 * 
 * Revision 1.24  1994/03/14  16:56:16  john
 * Changed grs_bitmap structure to include bm_flags.
 * 
 * Revision 1.23  1994/02/18  15:32:27  john
 * *** empty log message ***
 * 
 * Revision 1.22  1994/01/25  11:40:44  john
 * Added gr_check_mode function.
 * 
 * Revision 1.21  1993/12/21  19:57:48  john
 * added selector stuff.
 * 
 * Revision 1.20  1993/12/21  11:40:32  john
 * *** empty log message ***
 * 
 * Revision 1.19  1993/12/09  15:02:13  john
 * Changed palette stuff majorly
 * 
 * Revision 1.18  1993/11/16  11:28:36  john
 * *** empty log message ***
 * 
 * Revision 1.17  1993/10/26  13:17:53  john
 * *** empty log message ***
 * 
 * Revision 1.16  1993/10/15  16:23:42  john
 * y
 * 
 * Revision 1.15  1993/09/29  16:15:21  john
 * optimized
 * 
 * Revision 1.14  1993/09/28  19:06:51  john
 * made gr_set_mode change the grd_cursreen->sc_mode variable.
 * 
 * Revision 1.13  1993/09/28  12:52:12  matt
 * Set aspect ratio of current screen in gr_init() and gr_set_mode().
 * 
 * Revision 1.12  1993/09/27  13:00:24  john
 * made gr_set_mode not call mode_reset on fail
 * 
 * Revision 1.11  1993/09/27  12:51:27  john
 * fixed gr_set_mode to return values
 * 
 * Revision 1.10  1993/09/26  18:59:12  john
 * fade in/out stuff
 * 
 * Revision 1.9  1993/09/21  14:00:41  john
 * added code to save 43/50 line modes.
 * 
 * Revision 1.8  1993/09/20  14:48:48  john
 * *** empty log message ***
 * 
 * Revision 1.7  1993/09/16  17:27:48  john
 * Added code to save/restore video mode.
 * 
 * Revision 1.6  1993/09/16  16:30:15  john
 * Made gr_close retore Mode 3 always.
 * 
 * Revision 1.5  1993/09/14  18:40:07  john
 * Made it so that gr_setmode doesn't change curcanv font and colors.
 * 
 * Revision 1.4  1993/09/14  16:27:01  matt
 * Changes gr_change_mode() to be gr_set_mode()
 * After gr_set_mode(), grd_curcanv is the canvas of the new screen
 * Made gr_set_mode() work with the new grd_curcanv, not the old one
 * 
 * Revision 1.3  1993/09/14  13:08:37  john
 * Added gr_changemode
 * 
 * Revision 1.2  1993/09/08  17:36:37  john
 * Looking for error for Yuan... Neatened the nested ifs in setmode.
 * 
 * Revision 1.1  1993/09/08  11:43:51  john
 * Initial revision
 * 
 *
 */

#include <dos.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "types.h"
#include "mem.h"
#include "gr.h"
#include "grdef.h"
#include "error.h"
#include "mono.h"
#include "dpmi.h"
#include "palette.h"
#include "dpmi.h"

unsigned char * gr_video_memory = (unsigned char *)0xA0000;

char gr_pal_default[768];

int gr_installed = 0;

ubyte * pVideoMode =  (volatile ubyte *)0x449;
ushort * pNumColumns = (volatile ushort *)0x44a;
ubyte * pNumRows = (volatile ubyte *)0x484;
ushort * pCharHeight = (volatile ushort *)0x485;
ushort * pCursorPos = (volatile ushort *)0x450;
ushort * pCursorType = (volatile ushort *)0x460;
ushort * pTextMemory = (volatile ushort *)0xb8000;

typedef struct screen_save {
	ubyte 	video_mode;
	ubyte 	is_graphics;
	ushort	char_height;
	ubyte		width;
	ubyte		height;
	ubyte		cursor_x, cursor_y;
	ubyte		cursor_sline, cursor_eline;
	ushort * video_memory;
} screen_save;

screen_save gr_saved_screen;

int gr_show_screen_info = 0;

void gr_set_cellheight( ubyte height )
{
	ubyte temp;

   outp( 0x3d4, 9 );
	temp = inp( 0x3d5 );
   temp &= 0xE0;
	temp |= height;
	outp( 0x3d5, temp );
}

void gr_set_linear()
{
	outpw( 0x3c4, 0xE04 );		  // enable chain4 mode
	outpw( 0x3d4, 0x4014 );		  // turn on dword mode
	outpw( 0x3d4, 0xa317 );		  // turn off byte mode
}

void gr_16_to_256()
{
	outpw( 0x3ce, 0x4005 );	 	// set Shift reg to 1

	inp( 0x3da );					// dummy input

	outp( 0x3c0, 0x30 );
	outp( 0x3c0, 0x61 );		   // turns on PCS & PCC

	inp( 0x3da );					// dummy input

	outp( 0x3c0, 0x33 );
	outp( 0x3c0, 0 );
}

void gr_turn_screen_off()
{
	ubyte temp;
	temp = inp( 0x3da );
	outp( 0x3c0, 0 );
}

void gr_turn_screen_on()
{
	ubyte temp;
	temp = inp( 0x3da );
	outp( 0x3c0, 0x20 );
}

void gr_set_misc_mode( uint mode )
{
	union REGS regs;

	memset( &regs, 0, sizeof(regs) );
	regs.w.ax = mode;
	int386( 0x10, &regs, &regs );

}

void gr_set_3dbios_mode( uint mode )
{
	union REGS regs;
	memset( &regs, 0, sizeof(regs) );
	regs.w.ax = 0x4fd0;
	regs.w.bx = 0x3d00 | (mode & 0xff);
	int386( 0x10, &regs, &regs );
}



void gr_set_text_25()
{
	union REGS regs;

	regs.w.ax = 3;
	int386( 0x10, &regs, &regs );

}

void gr_set_text_43()
{
	union REGS regs;

	regs.w.ax = 0x1201;
	regs.w.bx = 0x30;
	int386( 0x10, &regs, &regs );

	regs.w.ax = 3;
	int386( 0x10, &regs, &regs );

	regs.w.ax = 0x1112;
	regs.w.bx = 0x0;
	int386( 0x10, &regs, &regs );
}

void gr_set_text_50()
{
	union REGS regs;

	regs.w.ax = 0x1202;
	regs.w.bx = 0x30;
	int386( 0x10, &regs, &regs );

	regs.w.ax = 3;
	int386( 0x10, &regs, &regs );

	regs.w.ax = 0x1112;
	regs.w.bx = 0x0;
	int386( 0x10, &regs, &regs );
}

ubyte is_graphics_mode()
{
	byte tmp;
	tmp = inp( 0x3DA );		// Reset flip-flip
	outp( 0x3C0, 0x30 );		// Select attr register 10
	tmp = inp( 0x3C1 );	// Get graphics/text bit
	return tmp & 1;
}

void gr_setcursor(ubyte x, ubyte y, ubyte sline, ubyte eline)
{
	union REGS regs;

	memset( &regs, 0, sizeof(regs) );
	regs.w.ax = 0x0200;
	regs.w.bx = 0;
	regs.h.dh = y;
	regs.h.dl = x;
	int386( 0x10, &regs, &regs );

	memset( &regs, 0, sizeof(regs) );
	regs.w.ax = 0x0100;
	regs.h.ch = sline & 0xf;
	regs.h.cl = eline & 0xf;
	int386( 0x10, &regs, &regs );
}

void gr_getcursor(ubyte *x, ubyte *y, ubyte * sline, ubyte * eline)
{
	union REGS regs;

	memset( &regs, 0, sizeof(regs) );
	regs.w.ax = 0x0300;
	regs.w.bx = 0;
	int386( 0x10, &regs, &regs );
	*y = regs.h.dh;
	*x = regs.h.dl;
	*sline = regs.h.ch;
	*eline = regs.h.cl;
}


int gr_save_mode()
{
	int i;

	gr_saved_screen.is_graphics = is_graphics_mode();
	gr_saved_screen.video_mode = *pVideoMode;
	
	if (!gr_saved_screen.is_graphics)	{
		gr_saved_screen.width = *pNumColumns;
		gr_saved_screen.height = *pNumRows+1;
		gr_saved_screen.char_height = *pCharHeight;
		gr_getcursor(&gr_saved_screen.cursor_x, &gr_saved_screen.cursor_y, &gr_saved_screen.cursor_sline, &gr_saved_screen.cursor_eline );
		//MALLOC(gr_saved_screen.video_memory,ushort, gr_saved_screen.width*gr_saved_screen.height );//Hack by Krb
		gr_saved_screen.video_memory=(ushort *)malloc((gr_saved_screen.width*gr_saved_screen.height)*sizeof(ushort));
		for (i=0; i < gr_saved_screen.width*gr_saved_screen.height; i++ )
			gr_saved_screen.video_memory[i] = pTextMemory[i];
	}

	if (gr_show_screen_info )	{
		printf( "Current video mode 0x%x:\n",  gr_saved_screen.video_mode );
		if (gr_saved_screen.is_graphics)
			printf( "Graphics mode\n" );
		else	{
			printf( "Text mode\n" );
			printf( "( %d columns by %d rows)\n", gr_saved_screen.width, gr_saved_screen.height );
			printf( "Char height is %d pixel rows\n", gr_saved_screen.char_height );
			printf( "Cursor of type 0x%x,0x%x is at (%d, %d)\n", gr_saved_screen.cursor_sline, gr_saved_screen.cursor_eline,gr_saved_screen.cursor_x, gr_saved_screen.cursor_y );
		}
	}

	return 0;
}

int isvga()
{
	union REGS regs;

	memset( &regs, 0, sizeof(regs) );
	regs.w.ax = 0x1a00;
	int386( 0x10, &regs, &regs );

	if ( regs.h.al == 0x1a )
		 return 1;

	return 0;
}

void gr_restore_mode()
{
	int i;

	//gr_set_text_25(); 

	gr_palette_fade_out( gr_palette, 32, 0 );
	gr_palette_set_gamma(0);

	if ( gr_saved_screen.video_mode == 3 )	{
		switch( gr_saved_screen.height )	  {
		case 43:	gr_set_text_43(); break;
		case 50:	gr_set_text_50(); break;
		default:	gr_set_text_25(); break;
		}
	} else {
		gr_set_misc_mode(gr_saved_screen.video_mode);	
	}

	if (gr_saved_screen.is_graphics==0)	{
		gr_sync_display();
		gr_sync_display();
		gr_palette_read( gr_pal_default );
		gr_palette_clear();

		for (i=0; i < gr_saved_screen.width*gr_saved_screen.height; i++ )	
			pTextMemory[i]=gr_saved_screen.video_memory[i];
		gr_setcursor( gr_saved_screen.cursor_x, gr_saved_screen.cursor_y, gr_saved_screen.cursor_sline, gr_saved_screen.cursor_eline );
		gr_palette_faded_out = 1;
		gr_palette_fade_in( gr_pal_default, 32, 0 );
	}

}

int gr_close()
{
	if (gr_installed==1)
	{
		gr_installed = 0;
		gr_restore_mode();
		free(grd_curscreen);
  		if( gr_saved_screen.video_memory ) {
			free(gr_saved_screen.video_memory);
			gr_saved_screen.video_memory = NULL;
		}
	}

	return 0;
}

int gr_vesa_setmode( int mode )
{
	int retcode;

	retcode=gr_vesa_checkmode( mode );
	if ( retcode ) return retcode;

	return gr_vesa_setmodea( mode );
}


int gr_set_mode(int mode)
{
	int retcode;
	unsigned int w,h,t,data, r;

	//JOHNgr_disable_default_palette_loading();

	switch(mode)
	{
	case SM_ORIGINAL:
		return 0;
	case 0:
		if (!isvga()) return 1;
		gr_set_misc_mode(0x13);	
		w = 320; r = 320; h = 200; t=BM_LINEAR; data = 0xA0000;
		break;
	case SM_640x400V:
		retcode = gr_vesa_setmode( 0x100 ); 
		if (retcode !=0 ) return retcode;
		w = 640; r = 640; h = 400; t=BM_SVGA; data = 0;
		break;
	case SM_640x480V:
		retcode = gr_vesa_setmode( 0x101 ); 
		if (retcode !=0 ) return retcode;
		w = 640; r = 640; h = 480; t=BM_SVGA; data = 0;
		break;
	case SM_800x600V:
		retcode = gr_vesa_setmode( 0x103 ); 
		if (retcode !=0 ) return retcode;
		w = 800; r = 800; h = 600; t=BM_SVGA; data = 0;
		break;
	case SM_1024x768V:
		retcode = gr_vesa_setmode( 0x105 ); 
		if (retcode !=0 ) return retcode;
		w = 1024; r = 1024; h = 768; t=BM_SVGA; data = 0;
		break;
	case SM_640x480V15:
		retcode = gr_vesa_setmode( 0x110 ); 
		if (retcode !=0 ) return retcode;
		w = 640; r = 640*2; h=480; t=BM_SVGA15; data = 0;
		break;
	case SM_800x600V15:
		retcode = gr_vesa_setmode( 0x113 ); 
		if (retcode !=0 ) return retcode;
		w = 800; r = 800*2; h=600; t=BM_SVGA15; data = 0;
		break;
	case 19:
		if (!isvga()) return 1;
		gr_set_misc_mode(0x13);	
//		{
//			ubyte x;
//			x = inp( 0x3c5 );
//			x |= 8;
//			outp( 0x3c5, x );
//		}
		gr_set_cellheight( 3 );

		w = 320; r = 320; h = 100; t=BM_LINEAR; data = 0xA0000;
		break;
	case 20:
		retcode = gr_vesa_setmode( 0x102 ); 
		//gr_enable_default_palette_loading();
		if (retcode !=0 ) return retcode;
		gr_16_to_256();
		gr_set_linear();
		//gr_set_cellheight( 1 );
		gr_vesa_setlogical( 400 );
		w = 400; r = 400; h = 600; t=BM_SVGA; data = 0;
		break;
	case 21:
		if (!isvga()) return 1;
		gr_set_misc_mode(0xd);	
		gr_16_to_256();
		gr_set_linear();
		gr_set_cellheight( 3 );
		w = 160; r = 160; h = 100; t=BM_LINEAR; data = 0xA0000;
		break;
	case 22:			// 3dmax 320x400
		if (!isvga()) return 1;
		gr_set_3dbios_mode(0x31);
		//w = 320; r = 320/4; h = 400; t=BM_MODEX; data = 0;
		w = 320; r = 320; h = 400; t=BM_SVGA; data = 0;
		break;
	default:
		if (!isvga()) return 1;
		w = gr_modex_setmode( mode );
		//gr_enable_default_palette_loading();
		h = w & 0xffff; w = w >> 16; r = w / 4;t = BM_MODEX; data = 0;
		break;
	}
	gr_palette_clear();

	memset( grd_curscreen, 0, sizeof(grs_screen));
	grd_curscreen->sc_mode = mode;
	grd_curscreen->sc_w = w;
	grd_curscreen->sc_h = h;
	grd_curscreen->sc_aspect = fixdiv(grd_curscreen->sc_w*3,grd_curscreen->sc_h*4);
	grd_curscreen->sc_canvas.cv_bitmap.bm_x = 0;
	grd_curscreen->sc_canvas.cv_bitmap.bm_y = 0;
	grd_curscreen->sc_canvas.cv_bitmap.bm_w = w;
	grd_curscreen->sc_canvas.cv_bitmap.bm_h = h;
	grd_curscreen->sc_canvas.cv_bitmap.bm_rowsize = r;
	grd_curscreen->sc_canvas.cv_bitmap.bm_type = t;
	grd_curscreen->sc_canvas.cv_bitmap.bm_data = (unsigned char *)data;
	gr_set_current_canvas(NULL);

	//gr_enable_default_palette_loading();

	return 0;
}

int gr_init(int mode)
{
	int org_gamma;
	int retcode;

	// Only do this function once!
	if (gr_installed==1)
		return 1;

	if (gr_init_A0000())
		return 10;

	// Save the current text screen mode
	if (gr_save_mode()==1)
		return 1;

	// Save the current palette, and fade it out to black.
	gr_palette_read( gr_pal_default );
	gr_palette_faded_out = 0;
	org_gamma = gr_palette_get_gamma();
	gr_palette_set_gamma( 0 );
	gr_palette_fade_out( gr_pal_default, 32, 0 );
	gr_palette_clear();
	gr_palette_set_gamma( org_gamma );
	gr_sync_display();
	gr_sync_display();

	//MALLOC( grd_curscreen,grs_screen,1 );//Hack by KRB
	grd_curscreen=(grs_screen*)malloc(1*sizeof(grs_screen));
	memset( grd_curscreen, 0, sizeof(grs_screen));

	// Set the mode.
	if (retcode=gr_set_mode(mode))
	{
		gr_restore_mode();
		return retcode;
	}
	//JOHNgr_disable_default_palette_loading();

	// Set all the screen, canvas, and bitmap variables that
	// aren't set by the gr_set_mode call:
	grd_curscreen->sc_canvas.cv_color = 0;
	grd_curscreen->sc_canvas.cv_drawmode = 0;
	grd_curscreen->sc_canvas.cv_font = NULL;
	grd_curscreen->sc_canvas.cv_font_fg_color = 0;
	grd_curscreen->sc_canvas.cv_font_bg_color = 0;
	gr_set_current_canvas( &grd_curscreen->sc_canvas );

	if (!dpmi_allocate_selector( &gr_fade_table, 256*GR_FADE_LEVELS, &gr_fade_table_selector ))
		Error( "Error allocating fade table selector!" );

	if (!dpmi_allocate_selector( &gr_palette, 256*3, &gr_palette_selector ))
		Error( "Error allocating palette selector!" );

//	if (!dpmi_allocate_selector( &gr_inverse_table, 32*32*32, &gr_inverse_table_selector ))
//		Error( "Error allocating inverse table selector!" );


	// Set flags indicating that this is installed.
	gr_installed = 1;
	atexit(gr_close);

	return 0;
}

int gr_mode13_checkmode()
{
	if (isvga()) 
		return 0;
	else
		return 1;
}

//  0=Mode set OK
//  1=No VGA adapter installed
//  2=Program doesn't support this VESA granularity
//  3=Monitor doesn't support that VESA mode.:
//  4=Video card doesn't support that VESA mode.
//  5=No VESA driver found.
//  6=Bad Status after VESA call/
//  7=Not enough DOS memory to call VESA functions.
//  8=Error using DPMI.
//  9=Error setting logical line width.
// 10=Error allocating selector for A0000h
// 11=Not a valid mode support by gr.lib

int gr_check_mode(int mode)
{
	switch(mode)
	{
	case 19:
	case SM_320x200C:
	case SM_320x200U:
	case SM_320x240U:
	case SM_360x200U:
	case SM_360x240U:
	case SM_376x282U:
	case SM_320x400U:
	case SM_320x480U:
	case SM_360x400U:
	case SM_360x480U:
	case SM_360x360U:
	case SM_376x308U:
	case SM_376x564U:		return gr_mode13_checkmode();
	case SM_640x400V:		return gr_vesa_checkmode( 0x100 ); 
	case SM_640x480V: 	return gr_vesa_checkmode( 0x101 ); 
	case SM_800x600V: 	return gr_vesa_checkmode( 0x103 ); 
	case SM_1024x768V:	return gr_vesa_setmode( 0x105 ); 
	case SM_640x480V15:	return gr_vesa_setmode( 0x110 ); 
	case SM_800x600V15:	return gr_vesa_setmode( 0x113 ); 
	}
	return 11;
}

