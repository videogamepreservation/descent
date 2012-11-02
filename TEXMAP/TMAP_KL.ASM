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
; $Source: f:/miner/source/texmap/rcs/tmap_kl.asm $
; $Revision: 1.3 $
; $Author: mike $
; $Date: 1994/11/30 00:56:50 $
;
; Linear, lighted texture mapper inner loop with a constant lighting value.
;
; $Log: tmap_kl.asm $
; Revision 1.3  1994/11/30  00:56:50  mike
; optimization.
; 
; Revision 1.2  1994/11/12  16:39:38  mike
; jae to ja.
; 
; Revision 1.1  1994/05/10  09:37:52  mike
; Initial revision
; 
; Revision 1.1  1993/09/08  17:29:52  mike
; Initial revision
; 
;
;

	.386

	public	asm_tmap_scanline_lin_lighted_k_

	include	tmap_inc.asm

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'

	extrn	_fx_l:dword
;**	extrn	_fx_dl_dx:dword
	extrn	_dither_intensity_lighting:dword

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

	extern	_pixel_data_selector:word, _gr_fade_table_selector:word

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
asm_tmap_scanline_lin_lighted_k_:
	push	es
	push	fs
	pusha

	mov	es,_pixel_data_selector	; selector[0*2]
	mov	fs,_gr_fade_table_selector	; selector[1*2]	; fs = bmd_fade_table

; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;   esi	source pixel pointer = pixptr
;   edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
	mov	esi,_pixptr

; set edi = address of first pixel to modify
	mov	edi,_fx_y	; this is actually an int
	cmp	edi,_window_bottom
	ja	_none_to_do

	imul	edi,_bytes_per_row
	mov	ebx,_fx_xleft
	sar	ebx,16
	jns	ebx_ok
	sub	ebx,ebx
ebx_ok:
	add	edi,ebx
	add	edi,write_buffer

; set _loop_count = # of iterations
	mov	eax,_fx_xright
	sar	eax,16
	cmp	eax,_window_right
	jl	eax_ok1
	mov	eax,_window_right
eax_ok1:	cmp	eax,_window_left
	jg	eax_ok2
	mov	eax,_window_left
eax_ok2:

	sub	eax,ebx
	js	_none_to_do
	cmp	eax,_window_width
	jbe	_ok_to_do
	mov	eax,_window_width
_ok_to_do:
	mov	_loop_count,eax

;	edi	destination pixel pointer


	mov	ecx,offset _lighting_tables
	mov	eax,_fx_u	; get 32 bit u coordinate
	shr	eax,6	; get 6:10 int:frac u coordinate into low word
	mov	ebp,_fx_v	; get 32 bit v coordinate
	shl	ebp,10	; put 6:10 int:frac into high word
	mov	bp,ax	; put u coordinate in low word

	mov	eax,_fx_du_dx	; get 32 bit delta u
	shr	eax,6	; get 6:10 int:frac delta u into low word
	mov	edx,_fx_dv_dx	; get 32 bit delta v
	shl	edx,10	; put 6:10 int:frac into high word
	mov	dx,ax	; put delta u in low word

;; esi, ecx should be free
loop_test = 1	; set to 1 to run as loop for better profiling
 if loop_test
	mov	ebx,_fx_l
	shr	ebx,8

	mov	ecx,_loop_count
	inc	ecx
	shr	ecx,1
	je	one_more_pix
	pushf

	align	4
loop1:
	mov	eax,ebp	; get u, v
	shr	eax,26	; shift out all but int(v)
	shld	ax,bp,6	; shift in u, shifting up v

	mov	bl,es:[eax]	; get pixel from source bitmap

	mov	al,fs:[ebx]	; xlat pixel through lighting tables

	add	ebp,edx	; u += du, v += dv
	mov	[edi],al	; write pixel...
	inc	edi	; ...and advance

; --- ---

	mov	eax,ebp	; get u, v
	shr	eax,26	; shift out all but int(v)
	shld	ax,bp,6	; shift in u, shifting up v

	mov	bl,es:[eax]	; get pixel from source bitmap

	add	ebp,edx	; u += du, v += dv

	mov	al,fs:[ebx]	; xlat pixel through lighting tables
	mov	[edi],al	; write pixel...
	inc	edi	; ...and advance

	dec	ecx	; _loop_count
	jne	loop1

	popf
	jnc	all_done

one_more_pix:	mov	eax,ebp	; get u, v
	shr	eax,26	; shift out all but int(v)
	shld	ax,bp,6	; shift in u, shifting up v

	mov	bl,es:[eax]	; get pixel from source bitmap
	mov	al,fs:[ebx]	; xlat pixel through lighting tables
	mov	[edi],al	; write pixel...

all_done:	popa
	pop	fs
	pop	es
	ret


 endif

; usage:
;	eax	work
;	ebx	lighting value
;	ecx	_lighting_tables
;	edx	du, dv 6:10:6:10
;	ebp	u, v coordinates 6:10:6:10
;	esi	pointer to source bitmap
;	edi	write address

_size = (_end1 - _start1)/num_iters
	mov	ebx,_fx_l
	shr	ebx,8
	mov	eax,num_iters
	sub	eax,_loop_count
	imul	eax,eax,dword ptr _size
	add	eax,offset _start1
	jmp	eax

	align	4
_start1:

; usage:
;	eax	work
;	ebx	lighting value
;	ecx	_lighting_tables
;	edx	du, dv 6:10:6:10
;	ebp	u, v coordinates 6:10:6:10
;	esi	pointer to source bitmap
;	edi	write address

; do all but the last pixel in the unwound loop, last pixel done separately because less work is needed
  rept num_iters
 if 1

	mov	eax,ebp	; get u, v
	shr	eax,26	; shift out all but int(v)
	shld	ax,bp,6	; shift in u, shifting up v

	add	ebp,edx	; u += du, v += dv

	mov	bl,[esi+eax]	; get pixel from source bitmap
	mov	al,[ecx+ebx]	; xlat pixel through lighting tables
	mov	[edi],al	; write pixel...
	inc	edi	; ...and advance


  else

; version which assumes segment overrides are in place (which they are obviously not)
	mov	eax,ebp	; get u, v
	shr	eax,26	; shift out all but int(v)
	shld	ax,bp,6	; shift in u, shifting up v

	add	ebp,edx	; u += du, v += dv

	mov	bl,[eax]	; get pixel from source bitmap
	mov	al,[ebx]	; xlat pixel through lighting tables
	mov	[edi],al	; write pixel...
	inc	edi	; ...and advance
 endif
  endm

_end1:

; now do the leftover pixel
	mov	eax,ebp
	shr	eax,26	; shift in v coordinate
	shld	ax,bp,6	; shift in u coordinate while shifting up v coordinate
	mov	bl,[esi+eax]	; get pixel from source bitmap
	mov	al,_lighting_tables[ebx]	; xlat pixel through lighting tables
	mov	[edi],al	; write pixel...

_none_to_do:	popa
	pop	fs
	pop	es
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
