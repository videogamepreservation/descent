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
; $Source: f:/miner/source/texmap/rcs/tmap_16g.asm $
; $Revision: 1.4 $
; $Author: mike $
; $Date: 1994/11/30 00:56:37 $
;
; inner loop of rgb lighted linear texture mapper for 16 bits/pixel
;
; $Log: tmap_16g.asm $
; Revision 1.4  1994/11/30  00:56:37  mike
; optimization.
; 
; Revision 1.3  1994/11/12  16:39:23  mike
; jae to ja.
; 
; Revision 1.2  1993/11/22  10:24:28  mike
; *** empty log message ***
; 
; Revision 1.1  1993/09/08  17:29:17  mike
; Initial revision
; 
;
;


	.386

	option	oldstructs

	.nolist
	include	psmacros.inc
	.list

	public	asm_tmap_scanline_lin_rgb_16g_

	include	tmap_inc.asm

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'

	extd	_fx_u
	extd	_fx_v
	extd	_fx_du_dx
	extd	_fx_dv_dx
	extd	_fx_y
	extd	_fx_xleft
	extd	_fx_xright

	extd	_pixptr

	extd	_x
	extd	_loop_count

	extd	_fx_rgb,_fx_drgb_dx

	extd	_fx_r,_fx_g,_fx_b,_fx_dr_dx,_fx_dg_dx,_fx_db_dx

	align	4

_DATA	ENDS

DGROUP	GROUP	_DATA

_TEXT   SEGMENT PARA PUBLIC USE32 'CODE'

	ASSUME	DS:_DATA
	ASSUME	CS:_TEXT

; --------------------------------------------------------------------------------------------------
; Enter:
;	_xleft	fixed point left x coordinate
;	_xright	fixed point right x coordinate
;	_y	fixed point y coordinate
;	_pixptr	address of source pixel map
;	_u	fixed point initial u coordinate
;	_v	fixed point initial v coordinate
;	_du_dx	fixed point du/dx
;	_dv_dx	fixed point dv/dx

;   for (x = (int) xleft; x <= (int) xright; x++) {
;      _setcolor(read_pixel_from_tmap(srcb,((int) (u/z)) & 63,((int) (v/z)) & 63));
;      _setpixel(x,y);
;
;      u += du_dx;
;      v += dv_dx;
;      z += dz_dx;
;   }

	align	4
asm_tmap_scanline_lin_rgb_16g_:
	pusha

; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
	mov	esi,_pixptr

; set edi = address of first pixel to modify
	mov	edi,_fx_y
	cmp	edi,_window_bottom
	ja	_none_to_do

	sub	edi,_window_top
	imul	edi,_bytes_per_row
	mov	eax,_fx_xleft
	sar	eax,16
	jns	eax_ok
	sub	eax,eax
eax_ok:
	sub	eax,_window_left
	add	edi,eax
	add	edi,eax
	add	edi,write_buffer

; set _loop_count = # of iterations
	mov	eax,_fx_xright
	sar	eax,16
	cmp	eax,_window_right
	jb	eax_ok1
	mov	eax,_window_right
eax_ok1:	cmp	eax,_window_left
	ja	eax_ok2
	mov	eax,_window_left
eax_ok2:

	mov	ebx,_fx_xleft
	sar	ebx,16
	sub	eax,ebx
	js	_none_to_do
	cmp	eax,_window_width
	jbe	_ok_to_do
	mov	eax,_window_width
_ok_to_do:
	mov	_loop_count,eax

;	edi	destination pixel pointer

; shift up all light values and deltas so that the highest 5 bits are the integer portion
	shl	_fx_r,11
	shl	_fx_g,11
	shl	_fx_b,11

	shl	_fx_dr_dx,11
	shl	_fx_dg_dx,11
	shl	_fx_db_dx,11

; ---------------------------------------------------------------------------------------------------------------------
	mov	eax,_fx_g
	mov	edx,_fx_b
	mov	ebx,_fx_r
	mov	ebp,_fx_dr_dx
	mov	esi,_fx_dg_dx

; register usage:
;  eax	_fx_g
;  ebx	_fx_r
;  ecx	work
;  edx	_fx_b
;  ebp	_fx_dr_dx
;  esi	_fx_dg_dx
;  edi	write address

; do the regular texture mapper linear interpolation to get the pixel from the source bitmap
_loop1:
	mov	ecx,ebx	; light value in high 5 bits
	shr	ecx,27	; shift out all but light value
	shld	ecx,eax,5
	shld	ecx,edx,5

; now %ecx = r|g|b, each 5 bits, bit #15 clear, write the pixel
	mov	[edi],cx
	add	edi,2

; now add the deltas
	add	ebx,ebp
	add	eax,esi
	add	edx,_fx_db_dx

	dec	_loop_count
	jns	_loop1

; ---------------------------------------------------------------------------------------------------------------------

_none_to_do:	popa

	ret

; -- Code to get rgb 5 bits integer, 5 bits fraction value into 5 bits integer (for each gun)
; suitable for inverse color lookup
;**__test:
;** int 3
;**;	              rrrrrfffffrrrrrfffffxxbbbbbfffff
;**	mov	eax,11111001001010101110101101110111b
;**	and	eax,11111000001111100000001111100000b
;**	shld	ebx,eax,15
;**	or	bx,ax

_TEXT	ends

	end



