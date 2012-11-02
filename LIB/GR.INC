;THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
;SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
;END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
;ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
;IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
;SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
;FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
;CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
;AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
;COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
;
; $Source: f:/miner/source/2d/rcs/gr.inc $
; $Revision: 1.9 $
; $Author: mike $
; $Date: 1994/11/09 23:04:50 $
;
; Header file for 2d library
;
; $Log: gr.inc $
; Revision 1.9  1994/11/09  23:04:50  mike
; Add avg_color field.
; 
; Revision 1.8  1994/10/26  23:55:49  john
; Took out roller; Took out inverse table.
; 
; Revision 1.7  1994/10/25  12:05:29  john
; Added gr_fade_table and gr_palette.
; 
; Revision 1.6  1994/03/15  22:24:31  matt
; Added gr_setcolor_ and gr_inverse_table
; 
; Revision 1.5  1994/03/14  16:56:05  john
; Changed grs_bitmap structure to include bm_flags.
; 
; Revision 1.4  1994/02/04  13:21:12  matt
; Added extern for rotate_bitmap
; 
; Revision 1.3  1993/12/21  19:58:14  john
; added selector stuff
; 
; Revision 1.2  1993/10/15  16:26:24  john
; *** empty log message ***
; 
; Revision 1.1  1993/10/15  16:21:18  john
; Initial revision
; 
;
;


IFNDEF _gr_inc
_gr_inc EQU 1


	include	fix.inc
	include	psmacros.inc

;Note: this file was initially converted using H2INC, but extensively
;modified by hand.

; Begin of file gr.h


MAX_SCAN_LINES		EQU		1200t

grs_font		STRUCT 2t
ft_w		SWORD		?
ft_h		SWORD		?
ft_flags		SWORD		?
ft_baseline		SWORD		?
ft_minchar		BYTE		?
ft_maxchar		BYTE		?
ft_bytewidth		SWORD		?
ft_data		dd	?
ft_chars		dd	?
ft_widths		dd	?
ft_drawfunc		dd	?
grs_font		ENDS


BM_LINEAR		EQU		0t
BM_MODEX		EQU		1t
BM_SVGA		EQU		2t
BM_RGB15		EQU		3t

grs_bitmap		STRUCT 2t
bm_x		SWORD		?
bm_y		SWORD		?
bm_w		SWORD		?
bm_h		SWORD		?
bm_type		BYTE		?
bm_flags	BYTE		?
bm_rowsize		SWORD		?
bm_data		dd	?
bm_selector	dw	?
bm_avg_pixel	BYTE		?
bm_unused	BYTE		?
grs_bitmap		ENDS

grs_canvas		STRUCT 2t
;;cv_bitmap		grs_bitmap		<>
cv_bitmap		db	size grs_bitmap dup (?)
cv_color		SWORD		?
cv_drawmode		SWORD		?
cv_font		dd	?
cv_font_fg_color		SWORD		?
cv_font_bg_color		SWORD		?
grs_canvas		ENDS

grs_screen		STRUCT 2t
;;sc_canvas		grs_canvas		<>
sc_canvas		db	size grs_canvas dup (?)
sc_mode		SWORD		?
sc_w		SWORD		?
sc_h		SWORD		?
sc_aspect		fix		?
pal		BYTE		768t DUP (?)
grs_screen		ENDS

SM_320x200C		EQU		0t
SM_320x200U		EQU		1t
SM_320x240U		EQU		2t
SM_360x200U		EQU		3t
SM_360x240U		EQU		4t
SM_376x282U		EQU		5t
SM_320x400U		EQU		6t
SM_320x480U		EQU		7t
SM_360x400U		EQU		8t
SM_360x480U		EQU		9t
SM_360x360U		EQU		10t
SM_376x308U		EQU		11t
SM_376x564U		EQU		12t
SM_640x400V		EQU		13t
SM_640x480V		EQU		14t
SM_800x600V		EQU		15t
SM_1024x768V		EQU		16t

	extn	gr_init_
	extn	gr_close_
	extn	gr_create_canvas_
	extn	gr_create_sub_canvas_
	extn	gr_init_canvas_
	extn	gr_init_sub_canvas_
	extn	gr_free_canvas_
	extn	gr_free_sub_canvas_
	extn	gr_create_bitmap_
	extn	gr_create_bitmap_raw_
	extn	gr_create_sub_bitmap_
	extn	gr_free_bitmap_
	extn	gr_free_sub_bitmap_
	extn	gr_bm_pixel_
	extn	gr_bm_upixel_
	extn	gr_bm_ubitblt_
	extn	gr_bm_ubitbltm_
	extn	gr_set_palette_
	extn	gr_getcolor_
	extn	gr_set_drawmode_
	extn	gr_poly_
	extn	gr_upoly_
	extn	gr_pixel_
	extn	gr_upixel_
	extn	gr_gpixel_
	extn	gr_ugpixel_
	extn	gr_line_
	extn	gr_uline_
	extn	gr_bitmap_
	extn	gr_ubitmap_
	extn	gr_rect_
	extn	gr_urect_
	extn	gr_disk_
	extn	gr_udisk_
	extn	gr_circle_
	extn	gr_ucircle_
	extn	gr_box_
	extn	gr_ubox_
	extn	gr_scanline_
	extn	gr_uscanline_
	extn	gr_init_font_
	extn	gr_set_fontcolor_
	extn	gr_set_curfont_
	extn	gr_string_
	extn	gr_ustring_
	extn	gr_printf_
	extn	gr_uprintf_
	extn	gr_get_string_size_
	extn	FixDivide_
	extn	gr_show_canvas_
	extn	gr_set_current_canvas_
	extn	rotate_bitmap_
	extn	gr_setcolor_
	extn	gr_find_closest_color_15bpp_

EXTERNDEF		C grd_curcanv:PTR grs_canvas
EXTERNDEF		C grd_curscreen:PTR grs_screen
EXTERNDEF		C Test_bitmap_data:BYTE

EXTERNDEF		C gr_inverse_table:BYTE
EXTERNDEF		C gr_palette:BYTE
EXTERNDEF		C gr_fade_table:BYTE

FT_COLOR		EQU		1t
FT_PROPORTIONAL		EQU		2t


; End of file gr.h

ENDIF

