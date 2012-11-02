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




		; eax = 0
		; ebx = l in fixed 24.8 (really 8.8)
		; ecx = (dv << 16) | du   (in fixed 8.8)
		; edx = free
		; esi = ptr to source bitmap ( 64x64 at 256bpr)
		; edi = destination ptr
		; ebp = dldx   (in fixed 24.8) (really 8.8)
		; MEM [0..3] = (u<<16) | v  (in fixed 8.8)
		; Code for 8-bit destination, lighting

		REPT 8
			mov	al, [0]			; get u
			mov	ah, [2]			; get v
			add	[0], ecx		; inc u,v
			mov	al, [esi+eax]		; get source pixel
			mov	ah, bh			; form lighting lookup value
			mov	al, FadeTable[eax]	; get lit pixel
			mov	[edi], al		; write pixel to frame buffer
			inc	edi			; inc dest
			add	ebx, ebp		; inc lighting value
		ENDM


		REPT 8
			mov	al, [0]			; get u
			mov	ah, [2]			; get v
			add	[0], ecx		; inc u,v
			mov	al, [esi+eax]		; get source pixel
			mov	ah, bh			; form lighting lookup value
			mov	ax, FadeTable[eax*2]	; get lit pixel
			mov	[edi], ax		; write pixel to frame buffer
			inc	edi			; inc dest
			add	ebx, ebp		; inc lighting value
		ENDM


		REPT NUM_ITERATIONS
			mov	al, [0]			; get u
			mov	ah, [2]			; get v
			add	[0], ecx		; inc u,v
			mov	al, [esi+eax]		; get source pixel
			mov	[edi+OFFSET], al	; write pixel to frame buffer
		ENDM

	mov	eax,ebp	; clear for 
	add	ebp,edx	; update v coordinate
	shr	eax,26	; shift in v coordinate
	shld	eax,ebx,6	; shift in u coordinate while shifting up v coordinate
	add	ebx,ecx	; update u coordinate
	mov	al,[esi+eax]	; get pixel from source bitmap
	mov	[edi],al
	inc	edi		; XPARENT ADDED BY JOHN



; compute v coordinate
	mov	eax,[_fx_v]	; get v
	cdq
	idiv	ecx	; eax = (v/z)

	and	eax,3fh	; mask with height-1
	mov	ebx,eax

; compute u coordinate
	mov	eax,esi	; get u
	cdq
	idiv	ecx	; eax = (u/z)

	shl 	eax,26
	shld 	ebx,eax,6	; esi = v*64+u

; read 1  pixel
	movzx	eax,byte ptr es:[ebx]	; get pixel from source bitmap

	cmp	_Lighting_on, 0
	je	NoLight1

; LIGHTING CODE
	mov	ebx, _fx_l	; get temp copy of lighting value
	mov	ah, bh	; get lighting level
	add	ebx, _fx_dl_dx	; update lighting value
	mov	al, fs:[eax]	; xlat pixel thru lighting tables
	mov	_fx_l, ebx	; save temp copy of lighting value

; transparency check
NoLight1:	cmp	al,255
	je	skip1

	mov	[edi],al
skip1:	inc	edi
	
; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx
	je	_div_0_abort	; would be dividing by 0, so abort

	xchg	esi, ebx

	dec	_loop_count
	jns	tmap_loop

