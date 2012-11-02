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

	align	4
TopOfLoop4:
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

	mov	ecx, U0	; ecx = U0 until pop's
	mov	edi, V0	; edi = V0 until pop's

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
	mov	dx, ax	; put delta u in low word

; Save the U1 and V1 so we don't have to divide on the next iteration
	mov	U0, ebx
	mov	V0, ebp

	pop	edi	; Restore EDI before using it
		
; LIGHTING CODE
	mov	ebx, _fx_l
	mov	ebp, _fx_dl_dx1

	REPT (1 SHL (NBITS-2))
		REPT 2
			mov	eax, esi	; get u,v
			shr	eax, 26		; shift out all but int(v)
			shld	ax,si,6		; shift in u, shifting up v
			add	esi, edx	; inc u,v
			mov 	al, es:[eax]	; get pixel from source bitmap
			mov	ah, bh		; form lighting table lookup value
			add	ebx, ebp	; update lighting value
			mov	cl, fs:[eax]	; xlat thru lighting table into dest buffer

			; eax = 0
			; ebx = l in fixed 24.8 (really 8.8)
			; ecx = (dv << 16) | du   (in fixed 8.8)
			; edx = free
			; esi = ptr to source bitmap
			; edi = destination ptr
			; ebp = dldx   (in fixed 24.8) (really 8.8)
			; MEM [0..3] = (u<<16) | v  (in fixed 8.8)
						
		; Code for 8-bit destination, lighting
			mov	al, [0]			; get u
			mov	ah, [2]			; get v
			add	[0], ecx		; inc u,v
			mov	al, [esi+eax]		; get source pixel
			mov	ah, bh			; form lighting lookup value
			mov	al, FadeTable[eax]	; get lit pixel
			mov	ah, [edi]		; get prev pixel
			mov	al, Transparent[eax]	; get transluscent pixel value
			mov	[edi], al		; write pixel to frame buffer
			inc	edi			; inc dest
			add	ebx, ebp		; inc lighting value

		; Code for 8-bit destination, no lighting
			mov	al, [0]		; get u
			mov	ah, [2]		; get v
			add	[0], ecx	; inc u,v
			mov	al, [esi+eax]	; get source pixel
			mov	[edi], al	; write pixel to frame buffer
			inc	edi		; inc dest
			add	ebx, ebp	; inc dl


			; old way
			mov	eax,ebp	; clear for 
			add	ebp,edx	; update v coordinate
			shr	eax,26	; shift in v coordinate
			shld	eax,ebx,6	; shift in u coordinate while shifting up v coordinate
			add	ebx,ecx	; update u coordinate
			mov	al,[esi+eax]	; get pixel from source bitmap
			mov	[edi],al
			inc	edi



		; Code for 15-bit destination, with fade table in hardware
			mov	al, ch		; get u,v
			mov	ah, dh		;
			mov	al, es:[eax]
			mov	ah, bh
			mov	[edi], ax	; write pixel to frame buffer
			add	edi, 2
			add	ecx, esi
			bswap	esi
			add	edx, esi
			bswap 	esi
			add	ebx, ebp

		; Code for 15-bit destination, with RGB 15 bit in hardware...
			mov	al, ch		; get u,v
			mov	ah, dh		;
			mov	al, es:[eax]	
			mov	ah, bh
			mov	ax, gs:[eax*2]	
			mov	[edi], ax	; write pixel to frame buffer
			add	edi, 2
			add	ecx, esi
			bswap	esi
			add	edx, esi
			bswap 	esi
			add	ebx, ebp


			

			mov	eax, esi	; get u,v
			shr	eax, 26		; shift out all but int(v)
			shld	ax,si,6		; shift in u, shifting up v
			add	esi, edx	; inc u,v
			mov 	al, es:[eax]	; get pixel from source bitmap
			mov	ah, bh		; form lighting table lookup value


		esi, ebp

			
	
			esi, ebp, 
			






		
			; Do odd pixel
			mov	eax, esi	; get u,v
			shr	eax, 26		; shift out all but int(v)
			shld	ax,si,6		; shift in u, shifting up v
			add	esi, edx	; inc u,v
			mov 	al, es:[eax]	; get pixel from source bitmap
			mov	ah, bh		; form lighting table lookup value
			add	ebx, ebp	; update lighting value
			mov	ch, fs:[eax]	; xlat thru lighting table into dest buffer
		
			ror	ecx, 16		; move to next double dest pixel position
		ENDM
					
		mov 	[edi],ecx	; Draw 4 pixels to display
		add 	edi,4
	ENDM

; LIGHTING CODE
	mov	_fx_l, ebx
	pop	ebp
	pop	ecx
	pop	ebx
	dec	_loop_count
	jnz	TopOfLoop4



	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	add	esi, edx	; inc u,v
	mov 	al, es:[eax]	; get pixel from source bitmap
	cmp	al,255
	je	skipa1
	mov	ah, bh	; form lighting table lookup value
	add	ebx, ebp	; update lighting value
	mov	al, fs:[eax]	; xlat thru lighting table into dest buffer
	mov	[edi],al


	mov	eax, esi	; get u,v
	shr	eax, 26		; shift out all but int(v)
	shld	ax,si,6		; shift in u, shifting up v
	add	esi, edx	; inc u,v
	mov 	al, es:[eax]	; get pixel from source bitmap
	mov	ah, bh		; form lighting table lookup value
	add	ebx, ebp	; update lighting value
	mov	al, fs:[eax]	; xlat thru lighting table into dest buffer
	mov	ah, [edi]	; get pixel already drawn here
	mov	al, gs:[eax]	; lookup in translation table
	mov	[edi],al	; write pixel


	; Code for 8-bit destination, lighting, transluscency
	mov	dx, [edi]
	mov	al, [0]			; get u
	mov	ah, [2]			; get v
	add	[0], ecx		; inc u,v
	mov	al, [esi+eax]		; get source pixel
	mov	ah, bh			; form lighting lookup value
	mov	al, FadeTable[eax]	; get lit pixel
	mov	ah, dl			; get prev pixel
	add	ebx, ebp		; inc lighting value
	mov	dl, TransTable[eax]	; get transluscent pixel value

	mov	al, [0]			; get u
	mov	ah, [2]			; get v
	add	[0], ecx		; inc u,v
	mov	al, [esi+eax]		; get source pixel
	mov	ah, bh			; form lighting lookup value
	mov	al, FadeTable[eax]	; get lit pixel
	mov	ah, dh			; get prev pixel
	add	ebx, ebp		; inc lighting value
	mov	dh, TransTable[eax]	; get transluscent pixel value

	mov	[edi], dx		; write 2 pixels to frame buffer
	inc	edi			; move to next pixel
	inc	edi			; move to next pixel


	; compute v coordinate
	mov	eax,ebp	; get v
	cdq
	idiv	ecx	; eax = (v/z)
	and	eax,3fh	; mask with height-1
	mov	ebx,eax

	; compute u coordinate
	mov	eax,esi	; get u
	cdq
	idiv	ecx		; eax = (u/z)
	shl 	eax,26		
	shld 	ebx,eax,6	; ebx = v*64+u

	; read 1  pixel
	movzx	eax,byte ptr es:[ebx]	; get pixel from source bitmap

; LIGHTING CODE
	mov	ebx, _fx_l	; get temp copy of lighting value
	mov	ah, bh		; get lighting level
	add	ebx, _fx_dl_dx	; update lighting value
	mov	al, fs:[eax]	; xlat pixel thru lighting tables
	mov	_fx_l, ebx	; save temp copy of lighting value

	mov	[edi],al
	inc	edi
	
; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx

	xchg	esi, ebx

	dec	_loop_count
	jns	tmap_loop



fix fixdiv(fix a,fix b);
#pragma aux fixdiv parm [eax] [ebx] modify exact [eax edx] = \
	"cdq	"	\
	"idiv	ebx"	\
	


	fix u = u0;
	fix v = v0;
	fix z = z0;
	fix l = l0;

	for ( x=x1; x<=x2; x++ )	{
		if ( z < zbuffer[x] )	{
			zbuffer[x] = z;
			color = bitmap[(u/z)&63][(v/z)&63];
			if ( color != xparent )	{
				color = LightingTable[color][f2i(l)&31];
				pixel[x] = color;
			}
		}
		u += du;
		v += dv;
		z += dz;
		l += dl;
	}

;================ PERSPECTIVE TEXTURE MAP INNER LOOPS ========================
;
; Usage in loop:
;	eax	division, pixel value
;	ebx	source pixel pointer
;	ecx	z
;	edx	division
;	ebp	v
;	esi	u
;	edi	destination pixel pointer


rept niters
	mov	eax, [_fx_z]			; get z-value
	cmp	zbuffer[edi], eax		; check if closer
	jge	skip1				; don't do if z farther away
	mov	zbuffer[edi], eax		; update z-buffer
	mov	eax,[_fx_v]			; get v		
	cdq					; sign extend into edx:eax
	idiv	[_fx_z]				; eax = (v/z)
	and	eax,63				; mask with height-1
	mov	ebx,eax				; start build bitmap address
	mov	eax,[_fx_u]			; get u
	cdq					; sign extend into edx:eax
	idiv	[_fx_z]				; eax = (u/z)
	shl 	eax,26				; continue building bitmap address
	shld 	ebx,eax,6			; ebx = v*64+u
	mov	al,byte ptr es:[ebx]		; get pixel from source bitmap
	cmp	al,255				; check if it's transparent
	je	skip1				; don't write if transparent
	mov	bx, [_fx_l]			; get lighting value
	mov	bl, al				; build lighting table lookup index
	mov	al, lighting[ebx]		; lookup lighting value
	mov	video[edi],al			; write the pixel
skip1:	inc	edi				; go to next pixel address
	add	[_fx_l],[_fx_dl_dx]		; increment lighting
	add	[_fx_u],[_fx_du_dx]		; increment u
	add	[_fx_v],[_fx_dv_dx]		; incrememt v
	add	[_fx_z],[_fx_dz_dx]		; increment z
	je	_div_0_abort			; would be dividing by 0, so abort
endm


	mov	eax, F1_0
	mov	ebx, [z]
	mov	edx,eax
	sar	edx,16
	shl	eax,16
	idiv	ebx

	"imul	edx"				\
	"shrd	eax,edx,16";

	// assume 8.12 fixed point

rept niters
	mov	eax, [_fx_z]			; get z-value
	cmp	zbuffer[edi], eax		; check if closer
	jge	skip1				; don't do if z farther away
	mov	zbuffer[edi], eax		; update z-buffer
	mov	eax,[_fx_v]			; get v		
	cdq		
	idiv	eax, [z]
	mov	ebx,eax				; start build bitmap address
	mov	eax,[_fx_u]			; get u
	imul	eax, [z]
	shl 	eax,26				; continue building bitmap address
	shld 	ebx,eax,6			; ebx = v*64+u
	mov	al,byte ptr es:[ebx]		; get pixel from source bitmap
	cmp	al,255				; check if it's transparent
	je	skip1				; don't write if transparent
	mov	bx, [_fx_l]			; get lighting value
	mov	bl, al				; build lighting table lookup index
	mov	al, lighting[ebx]		; lookup lighting value
	mov	video[edi],al			; write the pixel
skip1:	inc	edi				; go to next pixel address
	add	[_fx_l],[_fx_dl_dx]		; increment lighting
	add	[_fx_u],[_fx_du_dx]		; increment u
	add	[_fx_v],[_fx_dv_dx]		; incrememt v
	add	[_fx_z],[_fx_dz_dx]		; increment z
	je	_div_0_abort			; would be dividing by 0, so abort
endm


;================ PERSPECTIVE TEXTURE MAP INNER LOOPS ========================
;
; Usage in loop:
;	eax	division, pixel value
;	ebx	source pixel pointer
;	ecx	z
;	edx	division
;	ebp	v
;	esi	u
;	edi	destination pixel pointer


rept niters
	cmp	zbuffer[edi], ecx		; check if closer
	jge	skip1				; don't do if z farther away
	mov	zbuffer[edi], ecx		; update z-buffer
	mov	eax,[_fx_v]			; get v		
	mov	edx,[_fx_u]			; get u
	idiv	ecx				; eax = (v/z)
	and	eax,63				; mask with height-1
	mov	ebx,eax				; start build bitmap address
	mov	eax,[_fx_u]			; get u
	cdq					; sign extend into edx:eax
	idiv	ecx				; eax = (u/z)
	shl 	eax,26				; continue building bitmap address
	shld 	ebx,eax,6			; ebx = v*64+u
	mov	al,byte ptr es:[ebx]		; get pixel from source bitmap
	cmp	al,255				; check if it's transparent
	je	skip1				; don't write if transparent
	mov	bx, [_fx_l]			; get lighting value
	mov	bl, al				; build lighting table lookup index
	mov	al, lighting[ebx]		; lookup lighting value
	mov	video[edi],al			; write the pixel
skip1:	inc	edi				; go to next pixel address
	add	[_fx_l],[_fx_dl_dx]		; increment lighting
	add	[_fx_u],[_fx_du_dx]		; increment u
	add	[_fx_v],[_fx_dv_dx]		; incrememt v
	add	ecx,[_fx_dz_dx]			; increment z
	je	_div_0_abort			; would be dividing by 0, so abort
endm

