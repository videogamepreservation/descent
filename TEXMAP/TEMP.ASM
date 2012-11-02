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
public tmap_loop_fast_nolight
tmap_loop_fast_nolight:

	align	4
NotDwordAligned1_nolight:
	test	edi, 11b
	jz	DwordAligned1_nolight

	xchg	ebx, esi

	; compute v coordinate
	mov	eax,ebp	; get v
	cdq
	idiv	ecx	; eax = (v/z)

	and	eax,3fh	; mask with height-1
	mov	ebx,eax

	; compute u coordinate
	mov	eax,esi	; get u
	cdq
	idiv	ecx	; eax = (u/z)

	shl 	eax,26
	shld 	ebx,eax,6		; esi = v*64+u

	; read 1  pixel
	mov	al,es:[ebx]	; get pixel from source bitmap
	; write 1 pixel
	mov	[edi],al
	inc	edi
	
	; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx

	xchg	esi, ebx

	dec	_loop_count
	jns	NotDwordAligned1_nolight
	jmp	_none_to_do

DwordAligned1_nolight:
	mov	eax, _loop_count
	inc	eax
	mov	num_left_over, eax
	shr	eax, NBITS

	cmp	eax, 0		
	je	tmap_loop
	
	mov 	_loop_count, eax	; _loop_count = pixels / NPIXS
	;OPshl	eax, NBITS
	lea	eax, [eax*8]
	sub	num_left_over, eax	; num_left_over = obvious
		
	; compute initial v coordinate
	mov	eax,ebp	; get v
	PDIV
	mov	V0, eax

	; compute initial u coordinate
	mov	eax,ebx	; get u
	PDIV	
	mov	U0, eax

	; Set deltas to NPIXS pixel increments
	mov	eax, _fx_du_dx
	;OPshl	eax, NBITS
	lea	eax, [eax*8]
	mov	DU1, eax
	mov	eax, _fx_dv_dx
	;OPshl	eax, NBITS
	lea	eax, [eax*8]
	mov	DV1, eax
	mov	eax, _fx_dz_dx
	;OPshl	eax, NBITS
	lea	eax, [eax*8]
	mov	DZ1, eax

	align	4
TopOfLoop4_nolight:
		add	ebx, DU1
		add	ebp, DV1
		add	ecx, DZ1

		; Done with ebx, ebp, ecx until next iteration
		push	ebx
		push	ecx
		push	ebp
		push	edi
	
		; Find fixed U1		
		mov	eax, ebx
		PDIV
		mov	ebx, eax	; ebx = U1 until pop's

		; Find fixed V1		
		mov	eax, ebp
		PDIV
		mov	ebp, eax	; ebp = V1 until pop's

		mov	ecx, U0		; ecx = U0 until pop's
		mov	edi, V0		; edi = V0 until pop's

		; Make ESI =  V0:U0 in 6:10,6:10 format
		mov	eax, ecx
		shr	eax, 6
		mov	esi, edi
		shl	esi, 10
		mov	si, ax
		
		; Make EDX = DV:DU in 6:10,6:10 format
		mov	eax, ebx
		sub	eax, ecx
		sar	eax, NBITS+6
		mov	edx, ebp
		sub	edx, edi
		shl	edx, 10-NBITS	; EDX = V1-V0/ 4 in 6:10 int:frac
		mov	dx, ax		; put delta u in low word

		; Save the U1 and V1 so we don't have to divide on the
		; next iteration
		mov	U0, ebx
		mov	V0, ebp

		pop	edi		; Restore EDI before using it
		
		REPT (1 SHL (NBITS-2))
			REPT 4	
				; Do 1 pixel 
				mov	eax, esi	; get u,v
				shr	eax, 26		; shift out all but int(v)
				shld	ax,si,6		; shift in u, shifting up v
				mov	cl, es:[eax]	; load into buffer register
				add	esi, edx	; inc u,v
				ror	ecx, 8		; move to next dest pixel

			ENDM
					
			mov 	[edi],ecx		; Draw 4 pixels to display
			add 	edi,4
		ENDM

		pop	ebp
		pop	ecx
		pop	ebx
		dec	_loop_count
		jnz	TopOfLoop4_nolight

EndOfLoop4_nolight:

	cmp	num_left_over, 0
	je	_none_to_do

DoEndPixels_nolight:
		add	ebx, DU1
		add	ebp, DV1
		add	ecx, DZ1

		push	edi		; use edi as a temporary variable

		; Find fixed U1		
		mov	eax, ebx
		PDIV
		mov	ebx, eax	; ebx = U1 until pop's

		; Find fixed V1		
		mov	eax, ebp
		PDIV
		mov	ebp, eax	; ebp = V1 until pop's

		mov	ecx, U0		; ecx = U0 until pop's
		mov	edi, V0		; edi = V0 until pop's

		; Make ESI =  V0:U0 in 6:10,6:10 format
		mov	eax, ecx
		shr	eax, 6
		mov	esi, edi
		shl	esi, 10
		mov	si, ax
		
		; Make EDX = DV:DU in 6:10,6:10 format
		mov	eax, ebx
		sub	eax, ecx
		sar	eax, NBITS+6
		mov	edx, ebp
		sub	edx, edi
		shl	edx, 10-NBITS	; EDX = V1-V0/ 4 in 6:10 int:frac
		mov	dx, ax		; put delta u in low word

		pop	edi		; Restore EDI before using it
		
		mov	ecx, num_left_over

		ITERATION = 0
		REPT (1 SHL NBITS)
			; Do 1 pixel 
			mov	eax, esi	; get u,v
			shr	eax, 26		; shift out all but int(v)
			shld	ax,si,6		; shift in u, shifting up v
			mov	al, es:[eax]	; load into buffer register
			add	esi, edx	; inc u,v
			mov	[edi+ITERATION], al	; write pixel
			dec	ecx
			jz	_none_to_do
			ITERATION = ITERATION + 1
		ENDM

		; Should never get here!!!!!
		int	3
		jmp	_none_to_do

