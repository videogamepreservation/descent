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
; $Source: f:/miner/source/2d/rcs/scalea.asm $
; $Revision: 1.2 $
; $Author: john $
; $Date: 1994/01/12 18:02:52 $
; 
; Asm inner loop for scaler.
; 
; $Log: scalea.asm $
; Revision 1.2  1994/01/12  18:02:52  john
; Asm code for the scaler... first iteration here
; has compiled code that works!!
; 
; Revision 1.1  1994/01/12  12:20:11  john
; Initial revision
; 
; 



.386
	option	oldstructs

	.nolist
	include	types.inc
	include	psmacros.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: scalea.asm 1.2 1994/01/12 18:02:52 john Exp $"
	align	4
	

	public _scale_trans_color		
	public _scale_error_term		
	public _scale_initial_pixel_count	
	public _scale_adj_up			
	public _scale_adj_down			
	public _scale_final_pixel_count	
	public _scale_ydelta_minus_1		
	public _scale_whole_step		
	public _scale_source_ptr		
	public _scale_dest_ptr			

		
	_scale_trans_color		db	?
	_scale_error_term		dd	?
	_scale_initial_pixel_count	dd	?
	_scale_adj_up			dd	?
	_scale_adj_down			dd	?
	_scale_final_pixel_count	dd	?
	_scale_ydelta_minus_1		dd	?
	_scale_whole_step		dd	?
	_scale_source_ptr		dd	?
	_scale_dest_ptr			dd	?
	_scale_cc_jump_spot		dd	?
	_scale_slice_length_1		dd	?
	_scale_slice_length_2		dd	?

_DATA	ends


_TEXT	segment	dword public USE32 'CODE'


PUBLIC rls_stretch_scanline_asm_

rls_stretch_scanline_asm_:
		cld
		pusha
		mov	esi, _scale_source_ptr
		mov	edi, _scale_dest_ptr
		mov	edx, _scale_ydelta_minus_1
		mov	ebx, _scale_whole_step
		mov	ah,  _scale_trans_color
		mov	ebp, _scale_error_term

		mov	ecx, _scale_initial_pixel_count
		lodsb
		cmp	al, ah
		je	first_pixel_transparent
		rep	stosb
		jmp	next_pixel

first_pixel_transparent:
		add	edi, ecx
		jmp	next_pixel

skip_this_pixel:	add	edi, ecx
			dec	edx
			jz	done

next_pixel:		lodsb				; get next source pixel
			mov	ecx, ebx					
			add	ebp, _scale_adj_up
			jle	no_inc_error_term
			inc	ecx
			sub	ebp, _scale_adj_down
no_inc_error_term:						
			cmp	al, ah
			je	skip_this_pixel
			rep	stosb			; write source pixel to n locations
			dec	edx
			jnz	next_pixel

done:			lodsb
			cmp	al, ah
			je	exit_sub
			mov	ecx, _scale_final_pixel_count
			rep	stosb

exit_sub:
			popa
			ret

PUBLIC scale_do_cc_scanline_
PUBLIC rls_do_cc_setup_asm_

scale_do_cc_scanline_:
	cld
	pusha
	mov	esi, _scale_source_ptr
	mov	edi, _scale_dest_ptr
	mov	ah,  _scale_trans_color
	mov	ebx, _scale_slice_length_1
	mov	edx, _scale_slice_length_2

	mov	ecx, _scale_initial_pixel_count

	; Do the first texture pixel
	mov	ecx, _scale_initial_pixel_count
	lodsb
	cmp	al, ah
	je	@f
	rep	stosb
@@:	add	edi, ecx

	mov	ecx, _scale_cc_jump_spot
	jmp	ecx

	; This is the compiled code to do the middle pixels...
scale_cc_start:		mov	al, [esi]
scale_cc_changer:	mov	ecx, ebx	;<==== CODE CHANGES TO EBX OR EDX !!!!!!
			inc	esi
			cmp	al, ah
			je	@f
			rep	stosb
		@@:	add	edi, ecx
scale_cc_end:

	REPT 319
			mov	al, [esi]
			mov	ecx, ebx	;<==== CODE CHANGES TO EBX OR EDX !!!!!!
			inc	esi
			cmp	al, ah
			je	@f
			rep	stosb
		@@:	add	edi, ecx
	ENDM
last_cc_instruction:

	mov	ecx, _scale_final_pixel_count
	lodsb
	cmp	al, ah
	je	last_one_transparent
	rep	stosb
last_one_transparent:

	popa
	ret

rls_do_cc_setup_asm_:

		pusha

		mov	ebx, _scale_whole_step
		mov  	_scale_slice_length_1, ebx
		inc	ebx
		mov  	_scale_slice_length_2, ebx

		mov	ebp, _scale_error_term
		mov	edx, _scale_ydelta_minus_1

		mov	ebx, offset scale_cc_end
		sub	ebx, offset scale_cc_start	; ebx = distance to next changer inst.

		mov	eax, _scale_ydelta_minus_1

		
		imul	eax, ebx		; eax = sizeof 1 iteration * numiterations.
						
		mov	edi, offset last_cc_instruction
		sub	edi, eax		; edi = address of first iteration that we need
						;       to jump to
		mov	_scale_cc_jump_spot, edi

		mov	ecx, offset scale_cc_changer
		sub	ecx, offset scale_cc_start	; ecx = distance from start to to next changer inst.
		
		add	edi, ecx

next_pixel1:		add	ebp, _scale_adj_up
			jle	no_inc_error_term1
			; Modify code in scale_do_cc_scanline_ to write 'edx' pixels
		mov	al, byte ptr [edi]
		cmp	al, 08bh
		jne	BigError
			mov	word ptr [edi], 0CA8Bh	; 0x8BCA = mov ecx, edx
			add	edi, ebx		
			sub	ebp, _scale_adj_down
			dec	edx
			jnz	next_pixel1
			jmp	done1

no_inc_error_term1:	; Modify code in scale_do_cc_scanline_ to write 'ebx' pixels
		mov	al, byte ptr [edi]
		cmp	al, 08bh
		jne	BigError
			mov	word ptr [edi], 0CB8Bh	; 0x8BCB = mov ecx, ebx
			add	edi, ebx		
			dec	edx
			jnz	next_pixel1

done1:		popa
		ret

BigError:	int 3	; Stop, buddy!!
		popa
		ret

_TEXT	ends

	end


