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
; $Source: f:/miner/source/texmap/rcs/tmap_rgb.asm $
; $Revision: 1.4 $
; $Author: mike $
; $Date: 1994/11/30 00:57:02 $
;
; RGB interpolating texture mapper.
;
; $Log: tmap_rgb.asm $
; Revision 1.4  1994/11/30  00:57:02  mike
; optimization.
; 
; Revision 1.3  1994/11/12  16:41:11  mike
; jae -> ja.
; 
; Revision 1.2  1993/11/22  10:25:04  mike
; *** empty log message ***
; 
; Revision 1.1  1993/09/08  17:29:54  mike
; Initial revision
; 
;
;

	.386

	public	asm_tmap_scanline_lin_rgb_

	public	_fx_rgb,_fx_drgb_dx

	public	_fx_r,_fx_g,_fx_b,_fx_dr_dx,_fx_dg_dx,_fx_db_dx

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

_fx_rgb	dd	?
_fx_drgb_dx	dd	?

_fx_r	dd	?
_fx_g	dd	?
_fx_b	dd	?

_fx_dr_dx	dd	?
_fx_dg_dx	dd	?
_fx_db_dx	dd	?

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
asm_tmap_scanline_lin_rgb_:
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

	imul	edi,_bytes_per_row
	mov	eax,_fx_xleft
	sar	eax,16
	jns	eax_ok
	sub	eax,eax
eax_ok:
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

; rgb values are passed in the following peculiar, confidential, trade secreted, copyrighted, patented format:
; [ 5 bits ] [ 5 bits ] [ 5 bits ] [ 5 bits ] [ 2 bits ] [ 5 bits ] [ 5 bits ]
;   red int   red frac   blue int  blue frac    unused   green int  green frac
; The reason they are stored in the order red, blue, green is to optimize the process of packing together the three 5 bit
; values for red, green, blue in the conventional manner, suitable for an inverse table lookup

; convert fixed point values in _fx_dr_dx, _fx_dg_dx, _fx_db_dx to _fx_drgb_dx
;** int 3
	mov	eax,_fx_dg_dx	; get green value
	sar	eax,11	; after shift, low order 10 bits are what we want
	jns	dgok1
	inc	eax
dgok1:	shrd	ecx,eax,10	; shift green 5i.5f into destination

	shr	ecx,2	; shift in two don't care bits

	mov	eax,_fx_db_dx
	sar	eax,11
	jns	dbok1
	inc	eax
dbok1:	shrd	ecx,eax,10

	mov	eax,_fx_dr_dx
	sar	eax,11
	jns	drok1
	inc	eax
drok1:	shrd	ecx,eax,10	; now %ecx is correct!
	mov	_fx_drgb_dx,ecx

; convert fixed point values in _fx_r, _fx_g, _fx_b to _fx_rgb (which is the above peculiar format)
	mov	eax,_fx_g	; get green value
	sar	eax,11	; after shift, low order 10 bits are what we want
	jns	rok1
	sub	eax,eax
rok1:	shrd	ecx,eax,10	; shift green 5i.5f into destination

	shr	ecx,2	; shift in two don't care bits

	mov	eax,_fx_b
	sar	eax,11
	jns	bok1
	sub	eax,eax
bok1:	shrd	ecx,eax,10

	mov	eax,_fx_r
	sar	eax,11
	jns	gok1
	sub	eax,eax
gok1:	shrd	ecx,eax,10	; now %ecx is correct!

;**	mov	ecx,_fx_rgb


_size = (_end1 - _start1)/(num_iters-1)
	mov	eax,num_iters-1
	sub	eax,_loop_count
 jns itsok
 int 3
itsok:
	imul	eax,eax,dword ptr _size
	add	eax,offset _start1
;**	dec	edi	; inc edi moved before write to separate instructions
	jmp	eax

	align	4
_start1:

; usage:
;	eax	work
;	ebx	u coordinate
;	ecx	rgb (actually rbg, each i5.f5 with 2 bits between blue and green)
;	edx	work
;	ebp	v coordinate
;	esi	pointer to source bitmap
;	edi	write address

; do all but the last pixel in the unwound loop, last pixel done separately because less work is needed
  rept num_iters-1
; interpolate the rgb values
	sub	eax,eax
	mov	edx,ecx	; note, these are in order red, blue, green
	add	ecx,_fx_drgb_dx
	and	edx,11111000001111100000001111100000b	; preserve ri, bi, gi, strip out fractional portions and 2 extra bits
	shld	eax,edx,15	; shift in ri, bi
	or	ax,dx	; mask in gi

	mov	ah,_rgb_to_palette[eax]	; get color value for 15 bit rgb value

; do the regular texture mapper linear interpolation to get the pixel from the source bitmap
	sub	edx,edx
	shld	edx,ebp,6	; shift in v coordinate
	add	ebp,_fx_dv_dx	; update v coordinate
	shld	edx,ebx,6	; shift in u coordinate while shifting up v coordinate
	add	ebx,_fx_du_dx	; update u coordinate
	mov	al,[esi+edx]	; get pixel from source bitmap

; get color value based on looked up value from texture map (%al) and light value (%ah)
	mov	al,_pixel_average[eax]	; get color value based on palette light and color

; write the pixel
	mov	[edi],al
	inc	edi

  endm

_end1:

; now do the leftover pixel

; interpolate the rgb values
	sub	eax,eax
	mov	edx,ecx	; note, these are in order red, blue, green
	and	edx,11111000001111100000001111100000b	; preserve ri, bi, gi, strip out fractional portions and 2 extra bits
	shld	eax,edx,15	; shift in ri, bi
	or	ax,dx	; mask in gi

	mov	ah,_rgb_to_palette[eax]	; get color value for 15 bit rgb value

; do the regular texture mapper linear interpolation to get the pixel from the source bitmap
	sub	edx,edx
	shld	edx,ebp,6	; shift in v coordinate
	shld	edx,ebx,6	; shift in u coordinate while shifting up v coordinate
	mov	al,[esi+edx]	; get pixel from source bitmap

; get color value based on looked up value from texture map (%al) and light value (%ah)
	mov	al,_pixel_average[eax]	; get color value based on palette light and color

; write the pixel
	mov	[edi],al

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


