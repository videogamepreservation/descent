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
; $Source: f:/miner/source/texmap/rcs/tmap_skv.asm $
; $Revision: 1.5 $
; $Author: mike $
; $Date: 1994/11/30 00:57:03 $
; 
; Vertical scanner for sky bitmap rendering.
; 
; $Log: tmap_skv.asm $
; Revision 1.5  1994/11/30  00:57:03  mike
; optimization.
; 
; Revision 1.4  1994/11/12  16:41:13  mike
; jae -> ja.
; 
; Revision 1.3  1994/05/24  11:03:12  mike
; Make work for any sized (power of 2) bitmap.
; 
; Revision 1.2  1994/01/31  15:42:14  mike
; Vertical scanning sky texture mapper (in inner loop).
; 
; Revision 1.1  1994/01/30  14:10:55  mike
; Initial revision
; 
; 



DEBUG_ON	=	1

	.386

	option	oldstructs

	.nolist
	include	psmacros.inc
	.list


	public	asm_tmap_scanline_lin_sky_v_, asm_tmap_scanline_lin_v_

	include	tmap_inc.asm

sky_width_log_2	equ	10
sky_height_log_2	equ	7

width_log_2	equ	6
height_log_2	equ	6

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
asm_tmap_scanline_lin_sky_v_:
	pusha

; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
	mov	esi,_pixptr

; set edi = address of first pixel to modify
	mov	edi,_fx_xleft
	sar	edi,16
	jns	edi_ok
	sub	edi,edi
edi_ok:
	cmp	edi,_window_bottom
	ja	_none_to_do
	imul	edi,_bytes_per_row

	add	edi,_fx_y
	add	edi,write_buffer

; set _loop_count = # of iterations
	mov	eax,_fx_xright
	sar	eax,16
	mov	ebx,_fx_xleft
	sar	ebx,16
	sub	eax,ebx
	js	_none_to_do
	cmp	eax,_window_height
	jbe	_ok_to_do
	mov	eax,_window_height
_ok_to_do:
	mov	_loop_count,eax

;	edi	destination pixel pointer


	mov	ebx,_fx_u
	mov	ecx,_fx_du_dx
	mov	edx,_fx_dv_dx
	mov	ebp,_fx_v

	shl	ebx,16-sky_width_log_2
	shl	ebp,16-sky_height_log_2
	shl	edx,16-sky_height_log_2
	shl	ecx,16-sky_width_log_2

; eax	work
; ebx	u
; ecx	du_dx
; edx	dv_dx
; ebp	v
; esi	read address
; edi	write address

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

; "OPTIMIZATIONS" maybe not worth making
;    Getting rid of the esi from the mov al,[esi+eax] instruction.
;       This would require moving into eax at the top of the loop, rather than doing the sub eax,eax.
;       You would have to align your bitmaps so that the two shlds would create the proper base address.
;       In other words, your bitmap data would have to begin at 4096x (for 64x64 bitmaps).
;       I did timings without converting the sub to a mov eax,esi and setting esi to the proper value.
;       There was a speedup of about 1% to 1.5% without converting the sub to a mov.
;    Getting rid of the edi by doing a mov nnnn[edi],al instead of mov [edi],al.
;       The problem with this is you would have a dword offset for nnnn.  My timings indicate it is slower.  (I think.)
;    Combining u,v and du,dv into single longwords.
;       The problem with this is you then must do a 16 bit operation to extract them, and you don't have enough
;       instructions to separate a destination operand from being used by the next instruction.  It shaves out one
;       register instruction (an add reg,reg), but adds a 16 bit operation, and the setup is more complicated.
; usage:
;	eax	work
;	ebx	u coordinate
;	ecx	delta u
;	edx	delta v
;	ebp	v coordinate
;	esi	pointer to source bitmap
;	edi	write address
 rept num_iters
	mov	eax,ebp	; clear for 
	add	ebp,edx	; update v coordinate
	shr	eax,32-sky_height_log_2	; shift in v coordinate
	shld	eax,ebx,sky_width_log_2	; shift in u coordinate while shifting up v coordinate
	add	ebx,ecx	; update u coordinate
	mov	al,[esi+eax]	; get pixel from source bitmap
	mov	[edi],al
	add	edi,_bytes_per_row

 endm

_end1:

_none_to_do:	popa

	ret

; --------------------------------------------------------------------------------------------------------------------------------
	align	4
asm_tmap_scanline_lin_v_:
	pusha

; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
	mov	esi,_pixptr

; set edi = address of first pixel to modify
	mov	edi,_fx_xleft
	sar	edi,16
	jns	edi_ok_a
	sub	edi,edi
edi_ok_a:
	cmp	edi,_window_bottom
	ja	_none_to_do_a
	imul	edi,_bytes_per_row

	add	edi,_fx_y
	add	edi,write_buffer

; set _loop_count = # of iterations
	mov	eax,_fx_xright
	sar	eax,16
	mov	ebx,_fx_xleft
	sar	ebx,16
	sub	eax,ebx
	js	_none_to_do_a
	cmp	eax,_window_height
	jbe	_ok_to_do_a
	mov	eax,_window_height
_ok_to_do_a:
	mov	_loop_count,eax

;	edi	destination pixel pointer


	mov	ebx,_fx_u
	mov	ecx,_fx_du_dx
	mov	edx,_fx_dv_dx
	mov	ebp,_fx_v

	shl	ebx,16-width_log_2
	shl	ebp,16-height_log_2
	shl	edx,16-height_log_2
	shl	ecx,16-width_log_2

; eax	work
; ebx	u
; ecx	du_dx
; edx	dv_dx
; ebp	v
; esi	read address
; edi	write address

_size_a = (_end1_a - _start1_a)/num_iters
	mov	eax,num_iters-1
	sub	eax,_loop_count
	jns	j_eax_ok1_a
	inc	eax	; sort of a hack, but we can get -1 here and want to be graceful
	jns	j_eax_ok1_a	; if we jump, we had -1, which is kind of ok, if not, we int 3
	int	3	; oops, going to jump behind _start1, very bad...
	sub	eax,eax	; ok to continue
j_eax_ok1_a:	imul	eax,eax,dword ptr _size_a
	add	eax,offset _start1_a
	jmp	eax

	align	4
_start1_a:

; "OPTIMIZATIONS" maybe not worth making
;    Getting rid of the esi from the mov al,[esi+eax] instruction.
;       This would require moving into eax at the top of the loop, rather than doing the sub eax,eax.
;       You would have to align your bitmaps so that the two shlds would create the proper base address.
;       In other words, your bitmap data would have to begin at 4096x (for 64x64 bitmaps).
;       I did timings without converting the sub to a mov eax,esi and setting esi to the proper value.
;       There was a speedup of about 1% to 1.5% without converting the sub to a mov.
;    Getting rid of the edi by doing a mov nnnn[edi],al instead of mov [edi],al.
;       The problem with this is you would have a dword offset for nnnn.  My timings indicate it is slower.  (I think.)
;    Combining u,v and du,dv into single longwords.
;       The problem with this is you then must do a 16 bit operation to extract them, and you don't have enough
;       instructions to separate a destination operand from being used by the next instruction.  It shaves out one
;       register instruction (an add reg,reg), but adds a 16 bit operation, and the setup is more complicated.
; usage:
;	eax	work
;	ebx	u coordinate
;	ecx	delta u
;	edx	delta v
;	ebp	v coordinate
;	esi	pointer to source bitmap
;	edi	write address
 rept num_iters
	mov	eax,ebp	; clear for 
	add	ebp,edx	; update v coordinate
	shr	eax,32-height_log_2	; shift in v coordinate
	shld	eax,ebx,width_log_2	; shift in u coordinate while shifting up v coordinate
	add	ebx,ecx	; update u coordinate
	mov	al,[esi+eax]	; get pixel from source bitmap
	mov	[edi],al
	add	edi,_bytes_per_row

 endm

_end1_a:

_none_to_do_a:	popa

	ret



_TEXT	ends

	end
