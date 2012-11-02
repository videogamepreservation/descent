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
; $Source: f:/miner/source/texmap/rcs/tmap_z.asm $
; $Revision: 1.4 $
; $Author: mike $
; $Date: 1994/11/30 00:57:06 $
; 
; Assembly routines to do a flat-shaded z-buffer tmap.
; 
; $Log: tmap_z.asm $
; Revision 1.4  1994/11/30  00:57:06  mike
; optimization.
; 
; Revision 1.3  1994/11/12  16:41:14  mike
; jae -> ja.
; 
; Revision 1.2  1994/07/08  17:43:14  john
; Added flat-shaded-zbuffered polygon.
; 
; Revision 1.1  1994/07/08  10:58:18  john
; Initial revision
; 
; 

	.386

	public	asm_tmap_scanline_flat_z_
	public	_tmap_flat_color_z
	public	_tmap_z_buffer

	include	tmap_inc.asm

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'

	extrn	_fx_y:dword
	extrn	_fx_xleft:dword
	extrn	_fx_xright:dword
	extrn	_fx_z:dword
	extrn	_fx_dz_dx:dword
		
;**	extrn	_pixptr:dword

_tmap_flat_color_z	db	?
_tmap_z_buffer		dd	0
_loop_count		dd	0

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
;**;	_pixptr	address of source pixel map

;   for (x = (int) xleft; x <= (int) xright; x++) {
;      _setcolor(read_pixel_from_tmap(srcb,((int) (u/z)) & 63,((int) (v/z)) & 63));
;      _setpixel(x,y);
;
;      z += dz_dx;
;   }

asm_tmap_scanline_flat_z_:
	pusha

; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;**;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
;**	mov	esi,_pixptr

; set edi = address of first pixel to modify
	mov	edi,_fx_y
	shr	edi,16
	cmp	edi,_window_bottom
	ja	_none_to_do

	imul	edi,_bytes_per_row
	mov	eax,_fx_xleft
	sar	eax,16
	jns	eax_ok
	sub	eax,eax
eax_ok:
	add	edi,eax
	mov	esi,edi
	shl	esi, 2
	add	esi, _tmap_z_buffer
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
	mov	_loop_count, eax

	mov	cl,_tmap_flat_color_z
	mov	edx, _fx_z
	mov	ebp, _fx_dz_dx
	mov	ebx, 4
	
_size = (_end1 - _start1)/num_iters
	mov	eax,num_iters-1
	sub	eax,_loop_count
	jns	j_eax_ok1
	inc	eax	; sort of a hack, but we can get -1 here and want to be graceful
	jns	j_eax_ok1	; if we jump, we had -1, which is kind of ok, if not, we int 3
	int	3	; oops, going to jump behind _start1, very bad...
	sub	eax,eax	; ok to continue
j_eax_ok1:	imul	eax,eax,dword ptr _size
	add	eax,offset _start1
	jmp	eax

	align	4
_start1:

rept num_iters
local l1
	cmp	edx, [esi]
	jge	l1
	mov	[esi], edx
	mov	[edi], cl
l1:	add	esi, ebx
	add	edx, ebp
	inc	edi
endm
_end1:

_none_to_do:	popa
	ret


_TEXT	ends

	end
