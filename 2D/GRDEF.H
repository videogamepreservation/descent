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
 * $Source: f:/miner/source/2d/rcs/grdef.h $
 * $Revision: 1.9 $
 * $Author: john $
 * $Date: 1995/03/01 12:31:13 $
 *
 * Internal definitions for graphics lib.
 *
 * $Log: grdef.h $
 * Revision 1.9  1995/03/01  12:31:13  john
 * Added wait for retrace thingy in modex setstart.
 * 
 * Revision 1.8  1994/05/06  12:50:09  john
 * Added supertransparency; neatend things up; took out warnings.
 * 
 * Revision 1.7  1994/01/25  11:40:29  john
 * Added gr_check_mode function.
 * 
 * Revision 1.6  1993/10/15  16:22:53  john
 * y
 * 
 * Revision 1.5  1993/09/29  17:31:00  john
 * added gr_vesa_pixel
 * 
 * Revision 1.4  1993/09/29  16:14:43  john
 * added global canvas descriptors.
 * 
 * Revision 1.3  1993/09/08  17:38:02  john
 * Looking for errors
 * 
 * Revision 1.2  1993/09/08  15:54:29  john
 * *** empty log message ***
 * 
 * Revision 1.1  1993/09/08  11:37:57  john
 * Initial revision
 * 
 *
 */

extern int  gr_modex_setmode(short mode);
extern void gr_modex_setplane(short plane);
extern void gr_modex_setstart(short x, short y, int wait_for_retrace);
extern void gr_modex_uscanline( short x1, short x2, short y, unsigned char color );

extern void gr_pal_setblock( int start, int n, unsigned char * palette );
extern void gr_pal_getblock( int start, int n, unsigned char * palette );
extern void gr_pal_setone( int index, unsigned char red, unsigned char green, unsigned char blue );

extern int  gr_vesa_setmodea(int mode);
extern int  gr_vesa_checkmode(int mode);
extern void gr_vesa_setstart(short x, short y );
extern void gr_vesa_setpage(int page);
extern void gr_vesa_incpage();
extern void gr_vesa_scanline(short x1, short x2, short y, unsigned char color );
extern int  gr_vesa_setlogical(int pixels_per_scanline);
extern void gr_vesa_bitblt( unsigned char * source_ptr, unsigned int vesa_address, int height, int width );
extern void gr_vesa_pixel( unsigned char color, unsigned int offset );

void gr_linear_movsb( void * source, void * dest, unsigned short nbytes);
void gr_linear_movsw( void * source, void * dest, unsigned short nbytes);
void gr_linear_movsd( void * source, void * dest, unsigned short nbytes);
void gr_linear_stosd( void * source, unsigned char color, unsigned short nbytes);
extern unsigned int gr_var_color;
extern unsigned int gr_var_bwidth;
extern unsigned char * gr_var_bitmap;

void gr_linear_line( int x0, int y0, int x1, int y1);

extern unsigned int Table8to32[256];

extern unsigned char * gr_video_memory;

#define WIDTH   grd_curcanv->cv_bitmap.bm_w
#define HEIGHT  grd_curcanv->cv_bitmap.bm_h
#define MINX    0
#define MINY    0
#define MAXX    (WIDTH-1)
#define MAXY    (HEIGHT-1)
#define TYPE    grd_curcanv->cv_bitmap.bm_type
#define DATA    grd_curcanv->cv_bitmap.bm_data
#define XOFFSET grd_curcanv->cv_bitmap.bm_x
#define YOFFSET grd_curcanv->cv_bitmap.bm_y
#define ROWSIZE grd_curcanv->cv_bitmap.bm_rowsize
#define COLOR   grd_curcanv->cv_color


void order( int *x1, int *x2 );


