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
; $Source: f:/miner/source/texmap/rcs/tmap_16r.asm $
; $Revision: 1.4 $
; $Author: mike $
; $Date: 1994/11/30 00:56:41 $
;
; inner loop of rgb lighted linear texture mapper for 16 bits/pixel
;
; $Log: tmap_16r.asm $
; Revision 1.4  1994/11/30  00:56:41  mike
; optimization.
; 
; Revision 1.3  1994/11/12  16:39:33  mike
; jae to ja.
; 
; Revision 1.2  1993/11/22  10:24:47  mike
; *** empty log message ***
; 
; Revision 1.1  1993/09/08  17:29:45  mike
; Initial revision
; 
;
;


	.386

	public	asm_tmap_scanline_lin_rgb_16_

	include	tmap_inc.asm

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'

	extrn	_fx_u:dword
	extrn	_fx_v:dword
	extrn	_fx_du_dx:dword
	extrn	_fx_dv_dx:dword
	extrn	_fx_y:dword
	extrn	_fx_xleft:dword
	extrn	_fx_xright:dword

	extrn	_pixptr:dword

	extrn	_x:dword
	extrn	_loop_count:dword

	extrn	_fx_rgb:dword,_fx_drgb_dx:dword

	extrn	_fx_r:dword,_fx_g:dword,_fx_b:dword,_fx_dr_dx:dword,_fx_dg_dx:dword,_fx_db_dx:dword

	align	4
preserve_high_5_bits dd	11111000000000000000000000000000b

satadd	label	byte
	db	00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	db	01,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	db	02,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31
	db	03,03,04,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31
	db	04,04,05,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31
	db	05,05,06,06,07,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31
	db	06,06,07,07,08,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31
	db	07,07,08,08,09,09,10,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31
	db	08,08,09,09,10,10,11,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31
	db	09,09,10,10,11,11,12,12,13,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31
	db	10,10,11,11,12,12,13,13,14,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31,31
	db	11,11,12,12,13,13,14,14,15,15,16,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31,31
	db	12,12,13,13,14,14,15,15,16,16,17,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31,31,31
	db	13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31,31,31
	db	14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,22,23,24,25,26,27,28,29,30,31,31,31,31,31,31,31,31
	db	15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,24,25,26,27,28,29,30,31,31,31,31,31,31,31,31
	db	16,16,17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,25,26,27,28,29,30,31,31,31,31,31,31,31,31,31
	db	17,17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,27,28,29,30,31,31,31,31,31,31,31,31,31
	db	18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,28,29,30,31,31,31,31,31,31,31,31,31,31
	db	19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,27,28,28,29,30,31,31,31,31,31,31,31,31,31,31
	db	20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,31,31,31,31,31,31,31,31,31,31,31
	db	21,21,22,22,23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31
	db	22,22,23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	25,25,26,26,27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	26,26,27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	27,27,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
	db	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31

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
asm_tmap_scanline_lin_rgb_16_:
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

	mov	ebx,_fx_u
	mov	ebp,_fx_v

	shl	ebx,10
	shl	ebp,10

	shl	_fx_du_dx,10
	shl	_fx_dv_dx,10

; shift up all light values and deltas so that the highest 5 bits are the integer portion
	shl	_fx_r,11
	shl	_fx_g,11
	shl	_fx_b,11

	shl	_fx_dr_dx,11
	shl	_fx_dg_dx,11
	shl	_fx_db_dx,11

; ---------------------------------------------------------------------------------------------------------------------

; register usage:
;  eax	work
;  ebx	_fx_du_dx
;  ecx	work
;  edx	work
;  ebp	_fx_dv_dx
;  esi	texture map pixel data start
;  edi	write address

; do the regular texture mapper linear interpolation to get the pixel from the source bitmap
_loop1:	sub	edx,edx
	shld	edx,ebp,6	; shift in v coordinate
	add	ebp,_fx_dv_dx	; update v coordinate
	shld	edx,ebx,6	; shift in u coordinate while shifting up v coordinate
	add	ebx,_fx_du_dx	; update u coordinate
	lea	edx,[esi+4*edx]	; get pixel from source bitmap

; edx = pointer to rgbX dword pixel value
	mov	eax,_fx_r	; light value in high 5 bits
	and	eax,preserve_high_5_bits
	mov	al,[edx]	; get red pixel value
	rol	eax,5	; now 10 low bits = light:pixel
	movzx	ecx,byte ptr satadd[eax]
	shl	ecx,5

	mov	eax,_fx_g	; light value in high 5 bits
	and	eax,preserve_high_5_bits
	mov	al,1[edx]	; get green pixel value
	rol	eax,5	; now 10 low bits = light:pixel
	or	cl,byte ptr satadd[eax]
	shl	ecx,5

	mov	eax,_fx_b	; light value in high 5 bits
	and	eax,preserve_high_5_bits
	mov	al,2[edx]	; get blue pixel value
	rol	eax,5	; now 10 low bits = light:pixel
	or	cl,byte ptr satadd[eax]

; now %ecx = r|g|b, each 5 bits, bit #15 clear, write the pixel
	mov	[edi],cx
	add	edi,2

; now add the deltas
	mov	eax,_fx_dr_dx
	add	_fx_r,eax

	mov	eax,_fx_dg_dx
	add	_fx_g,eax

	mov	eax,_fx_db_dx
	add	_fx_b,eax

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



