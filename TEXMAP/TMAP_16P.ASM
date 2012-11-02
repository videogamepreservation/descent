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
; $Source: f:/miner/source/texmap/rcs/tmap_16p.asm $
; $Revision: 1.4 $
; $Author: mike $
; $Date: 1994/11/30 00:56:40 $
;
; Perspective 16 bit per pixel texture mapper
;
; $Log: tmap_16p.asm $
; Revision 1.4  1994/11/30  00:56:40  mike
; optimization.
; 
; Revision 1.3  1994/11/12  16:39:32  mike
; jae to ja.
; 
; Revision 1.2  1993/11/22  10:24:43  mike
; *** empty log message ***
; 
; Revision 1.1  1993/09/08  17:29:21  mike
; Initial revision
; 
;
;

	.386

	include	tmap_inc.asm

	public	asm_tmap_scanline_per_16_

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'
	extrn	_fx_u:dword
	extrn	_fx_v:dword
	extrn	_fx_z:dword
	extrn	_fx_du_dx:dword
	extrn	_fx_dv_dx:dword
	extrn	_fx_dz_dx:dword
	extrn	_fx_y:dword
	extrn	_fx_xleft:dword
	extrn	_fx_xright:dword

	extrn	_pixptr:dword

	extrn	_x:dword
	extrn	_loop_count:dword

temp	dd	?

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
;	_z	fixed point initial z coordinate
;	_du_dx	fixed point du/dx
;	_dv_dx	fixed point dv/dx
;	_dz_dx	fixed point dz/dx

;   for (x = (int) xleft; x <= (int) xright; x++) {
;      _setcolor(read_pixel_from_tmap(srcb,((int) (u/z)) & 63,((int) (v/z)) & 63));
;      _setpixel(x,y);
;
;      u += du_dx;
;      v += dv_dx;
;      z += dz_dx;
;   }

asm_tmap_scanline_per_16_:
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
;** dec eax ; don't write rightmost pixel
;** js _none_to_do
	mov	_loop_count,eax

; set initial values
	mov	ebx,_fx_u
	mov	ebp,_fx_v
	mov	ecx,_fx_z

; Usage in loop:	eax	division, pixel value
;	ebx	u
;	ecx	z
;	edx	division
;	ebp	v
;	esi	source pixel pointer
;	edi	destination pixel pointer

	align	4
tmap_loop:
; cmp ecx,_max_ecx
; jl nonewmax
; mov _max_ecx,ecx
;nonewmax:
; cmp ecx,_min_ecx
; ja nonewmin
; mov _min_ecx,ecx
;nonewmin:

;** cmp ecx,0
;** jle skip 

; compute u coordinate
	mov	eax,ebx	; get u
	cdq
	idiv	ecx	; eax = (u/z)
	and	eax,3fh	; mask with width-1
	mov	temp,eax

; compute v coordinate
	mov	eax,ebp	; get v
	cdq
	idiv	ecx	; eax = (v/z)
	and	eax,3fh	; mask with height-1
	shl	eax,6	; multiply by 64 (bitmap width)
	add	eax,temp

; read/write pixel
	mov	ax,[esi+2*eax]	; get pixel from source bitmap

; update deltas
	mov	[edi],ax
	add	edi,2

skip:	add	ebp,_fx_dv_dx
	add	ebx,_fx_du_dx
	add	ecx,_fx_dz_dx

	dec	_loop_count
	jns	tmap_loop

_none_to_do:	popa

	ret

_TEXT	ends

	end

