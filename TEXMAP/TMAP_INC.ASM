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
; $Source: f:/miner/source/texmap/rcs/tmap_inc.asm $
; $Revision: 1.10 $
; $Author: john $
; $Date: 1995/02/20 18:22:52 $
;
; Mike's include file for the texture mapper library.
;
; $Log: tmap_inc.asm $
; Revision 1.10  1995/02/20  18:22:52  john
; Put all the externs in the assembly modules into tmap_inc.asm.
; Also, moved all the C versions of the inner loops into a new module, 
; scanline.c.
; 
; Revision 1.9  1994/12/02  23:29:45  mike
; Add y_pointers.
; 
; Revision 1.8  1994/11/12  16:39:36  mike
; jae to ja.
; 
; Revision 1.7  1994/10/26  23:27:39  john
; Took out references to gr_inverse_table.
; 
; Revision 1.6  1994/10/26  23:21:55  mike
; kill unused stuff.
; 
; Revision 1.5  1994/07/27  18:39:20  john
; Took out references to blend table
; 
; Revision 1.4  1994/01/31  15:40:17  mike
; Add window_height.
; 
; Revision 1.3  1993/12/07  12:27:48  john
; Moved bmd_palette to gr_palette
; 
; Revision 1.2  1993/11/22  10:24:10  mike
; *** empty log message ***
; 
; Revision 1.1  1993/09/08  17:29:47  mike
; Initial revision
; 
;
;

; VESA in this file must be the same as VESA in tmap.h
VESA	equ	0

direct_to_video	equ	0

  if VESA

; for vesa mode
WINDOW_LEFT	=	0
WINDOW_RIGHT	=	300
WINDOW_TOP	=	0
WINDOW_BOTTOM	=	200
WINDOW_WIDTH	=	WINDOW_RIGHT - WINDOW_LEFT
BYTES_PER_ROW	=	300*2

  else

; for non-vesa mode
WINDOW_LEFT	=	58
WINDOW_RIGHT	=	262
WINDOW_TOP	=	34
WINDOW_BOTTOM	=	167
WINDOW_WIDTH	=	WINDOW_RIGHT - WINDOW_LEFT
BYTES_PER_ROW	=	320		; number of bytes between rows

  endif

; for vesa, 15 bit color, 640x480x2
SCREEN_WIDTH	=	640
SCREEN_HEIGHT	=	480
BYTES_PER_PIXEL	=	2


	extrn	_gr_fade_table:byte
;NO_INVERSE_TABLE	extrn	_gr_inverse_table:byte
	extrn	_write_buffer:dword
	extrn	_window_left:dword,_window_right:dword,_window_top:dword,_window_bottom:dword,_window_width:dword,_bytes_per_row:dword,_window_height:dword
	extrn	_y_pointers:dword

_lighting_tables	equ	_gr_fade_table
;NO_INVERSE_TABLE _rgb_to_palette	equ	_gr_inverse_table
write_buffer	equ	_write_buffer
;NO_INVERSE_TABLE _pixel_average	equ 	_gr_inverse_table		; should be blend table, but i took it out -john

max_window_width	equ	320
num_iters	=	max_window_width

  if num_iters and 1
num_iters = num_iters + 1
  endif

	extern _per2_flag:dword
	extern _tmap_flat_cthru_table:dword
	extern _tmap_flat_color:byte
	extern _tmap_flat_shade_value:byte
	extern _dither_intensity_lighting:dword
	extern _Lighting_on:dword
	extern _pixel_data_selector:word, _gr_fade_table_selector:word, _transparency_on:dword
	extern _fx_u:dword
	extern _fx_v:dword
	extern _fx_z:dword
	extern _fx_l:dword
	extern _fx_du_dx:dword
	extern _fx_dv_dx:dword
	extern _fx_dz_dx:dword
	extern _fx_dl_dx:dword
	extern _fx_y:dword
	extern _fx_xleft:dword
	extern _fx_xright:dword
	extern _pixptr:dword

