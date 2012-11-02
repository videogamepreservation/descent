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
; $Source: f:/miner/source/texmap/rcs/tmap_per.asm $
; $Revision: 1.8 $
; $Author: mike $
; $Date: 1993/12/16 18:37:52 $
;
; Perspective texture mapper inner loop.
;
; $Log: tmap_per.asm $
; Revision 1.8  1993/12/16  18:37:52  mike
; Align some stuff on 4 byte boundaries.
; 
; Revision 1.7  1993/11/30  08:44:18  john
; Made selector set check for < 64*64 bitmaps.
; 
; Revision 1.6  1993/11/23  17:25:26  john
; Added safety "and eax, 0fffh" in lighting lookup.
; 
; Revision 1.5  1993/11/23  15:08:52  mike
; Fixed lighting bug. 
; 
; Revision 1.4  1993/11/23  14:38:50  john
; optimized NORMAL code by switching EBX and ESI, so BH can be used in
; the lighting process.
; 
; Revision 1.3  1993/11/23  14:30:53  john
; Made the perspective tmapper do 1/8 divides; added lighting.
; 
; Revision 1.2  1993/11/22  10:24:59  mike
; *** empty log message ***
; 
; Revision 1.1  1993/09/08  17:29:53  mike
; Initial revision
; 
;
;

	.386

	public	_fx_u,_fx_v,_fx_z,_fx_du_dx,_fx_dv_dx,_fx_dz_dx,_fx_y,_fx_xleft,_fx_xright
	public	_pixptr
	public	asm_tmap_scanline_per_
;**	public	copy_canvas_to_a000_,clear_canvas_

	include	tmap_inc.asm

;**	public	_v_window_left,_v_window_right,_v_window_top,_v_window_bottom

 public _max_ecx,_min_ecx

	extern	_fx_l:dword
	extern 	_fx_dl_dx:dword
	extern	_dither_intensity_lighting:dword
	extern _Lighting_on:dword

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'
	align	4
;**_v_window_left	dd	_window_left
;**_v_window_right	dd	_window_right
;**_v_window_top	dd	_window_top
;**_v_window_bottom	dd	_window_bottom

; ---------- These are passed in by the C caller ----------
_fx_u	dd	?	
_fx_v	dd	?
_fx_z	dd	?
_fx_du_dx	dd	?
_fx_dv_dx	dd	?
_fx_dz_dx	dd	?

_fx_y	dd	?
_fx_xleft	dd	?
_fx_xright	dd	?

_pixptr	dd	?
; ----------^^ These are passed in by the C caller ^^----------

	public	_x,_loop_count
	align	4
_x	dd	?
_loop_count	dd	?
;**temp	dd	?

_max_ecx	dd	0
_min_ecx	dd	55555555h

	public	_per2_flag
_per2_flag	dd	1

	public	_tmap_aliasing

_tmap_aliasing	dd 0

;---------- local variables
	align	4
req_base	dd ?
req_size	dd ?
U0		dd ?
U1		dd ?
V0		dd ?
V1		dd ?
temp		dd ?
num_left_over	dd ?
DU1		dd ?
DV1		dd ?
DZ1		dd ?
_fx_dl_dx1	dd ?
_fx_dl_dx2	dd ?

initialized	db 10 dup (?)
selector	dw 10 dup (?)

_DATA	ENDS

DGROUP	GROUP	_DATA


_TEXT   SEGMENT DWORD PUBLIC USE32 'CODE'
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


TMAP_NORMAL MACRO
	LOCAL NoLight1

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

	cmp	_Lighting_on, 0
	je	NoLight1
	;LIGHTING CODE
	mov	ebx, _fx_l	; get temp copy of lighting value
	mov	ah, bh		; get lighting level
	add	ebx, _fx_dl_dx1	; update lighting value
	and	eax, 0fffh	; make sure no garbage in EAX
	mov	al, fs:[eax]	; xlat pixel thru lighting tables
	mov	_fx_l, ebx	; save temp copy of lighting value
					
	;DITHERING CODE
	mov 	ebx, _fx_dl_dx1
	xchg	ebx, _fx_dl_dx2
	mov	_fx_dl_dx1, ebx
NoLight1:
	
	cmp	_tmap_aliasing, 0
	je	@f
	;ANTI-ALIASING CODE
	mov	bl, [edi-320] 	; pixel above
	mov	bh, [edi-1]	; pixel to left
	and	ebx, 0ffffh
	mov	ah, gs:[ebx]	; average those two
	and	eax, 0ffffh
	mov	al, gs:[eax]	; average with current
@@:	
	; write 1 pixel
	mov	[edi],al
	inc	edi
	
	; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx

	xchg	esi, ebx
ENDM

asm_tmap_scanline_per_:
	push	es
	push	fs
	push	gs
	pusha
	
;---------------------------- setup for loop ---------------------------------
; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
;**	mov	esi,_pixptr
	mov	es,selector[0*2]
	mov	fs,selector[1*2]	; fs = bmd_fade_table
	mov	gs,selector[2*2]  	; gs = bmd_blend_table
	cmp	_tmap_aliasing, 0
	jne	@f
	mov	gs,selector[1*2]  	; gs = bmd_fade_table
	@@:	


; set edi = address of first pixel to modify
	mov	edi,_fx_y
	cmp	edi,_window_bottom
	jae	_none_to_do

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

;-------------------------- setup for dithering -----------------------------
; lighting values are passed in fixed point, but need to be in 8 bit integer, 8 bit fraction so we can easily
; get the integer by reading %bh
	sar	_fx_l, 8
	sar	_fx_dl_dx,8
	jns	dl_dx_ok
	inc	_fx_dl_dx	; round towards 0 for negative deltas
dl_dx_ok:

; do dithering, use lighting values which are .5 less than and .5 more than actual value
	mov	ebx,80h	; assume dithering on
	test	_dither_intensity_lighting,-1
	jne	do_dither
	sub	ebx,ebx	; no dithering
do_dither:
	mov	eax,_fx_dl_dx
	add	eax,ebx	; add 1/2
	mov	_fx_dl_dx1,eax
	sub	eax,ebx
	sub	eax,ebx
	mov	_fx_dl_dx2,eax
	mov	ebx,_fx_xleft
	shr	ebx,16
	xor	ebx,_fx_y
	and	ebx,1
	jne	dith_1
	xchg	eax,_fx_dl_dx1
dith_1:	mov	_fx_dl_dx2,eax


; set initial values
	mov	ebx,_fx_u
	mov	ebp,_fx_v
	mov	ecx,_fx_z

	test _per2_flag,-1
	je   tmap_loop

	jmp	tmap_loop_fast

;================ PERSPECTIVE TEXTURE MAP INNER LOOPS ========================
;
; Usage in loop:	eax	division, pixel value
;	ebx	u
;	ecx	z
;	edx	division
;	ebp	v
;	esi	source pixel pointer
;	edi	destination pixel pointer

;-------------------- NORMAL PERSPECTIVE TEXTURE MAP LOOP -----------------
	align	4
tmap_loop:
	TMAP_NORMAL
	dec	_loop_count
	jns	tmap_loop

_none_to_do:	
	popa
	pop	gs
	pop	fs
	pop	es
	ret

;-------------------------- PER/4 TMAPPER ----------------
; 
;	x = x1
;	U0 = u/w; V0 = v/w;
;	while ( 1 )
;		u += du_dx*4; v+= dv_dx*4
;		U1 = u/w; V1 = v/w;
;		DUDX = (U1-U0)/4; DVDX = (V1-V0)/4;
;
;		; Pixel 0
;		pixels = texmap[V0*64+U0];
;		U0 += DUDX; V0 += DVDX
;		; Pixel 1
;		pixels = (pixels<<8)+texmap[V0*64+U0];
;		U0 += DUDX; V0 += DVDX
;		; Pixel 2
;		pixels = (pixels<<8)+texmap[V0*64+U0];
;		U0 += DUDX; V0 += DVDX
;		; Pixel 3
;		pixels = (pixels<<8)+texmap[V0*64+U0];
;
;		screen[x] = pixel
;		x += 4;
;		U0 = U1; V0 = V1 

NBITS = 3
ZSHIFT = 3


PDIV MACRO
	; Returns EAX/ECX in 16.16 format in EAX. Trashes EDX
	;          sig bits   6.3
	cdq
	shld	edx,eax, ZSHIFT
	shl	eax, ZSHIFT
	idiv	ecx	; eax = (v/z)
	shl	eax, 16-ZSHIFT
ENDM

public tmap_loop_fast
tmap_loop_fast:

NotDwordAligned1:
	test	edi, 11b
	jz	DwordAligned1
	TMAP_NORMAL
	dec	_loop_count
	jns	NotDwordAligned1
	jmp	_none_to_do

DwordAligned1:
	mov	eax, _loop_count
	inc	eax
	mov	num_left_over, eax
	shr	eax, NBITS

	cmp	eax, 0		
	je	tmap_loop
	
	mov 	_loop_count, eax	; _loop_count = pixels / NPIXS
	shl	eax, NBITS
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
	shl	eax, NBITS
	mov	DU1, eax
	mov	eax, _fx_dv_dx
	shl	eax, NBITS
	mov	DV1, eax
	mov	eax, _fx_dz_dx
	shl	eax, NBITS
	mov	DZ1, eax
	
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
		;**sar	eax, NBITS  	; EAX = U1-U0 / 2^NBITS
		;**shr	eax, 6		; put 6:10 int:frac into low word
		sar	eax, NBITS+6
		mov	edx, ebp
		sub	edx, edi
		;**sar	edx, NBITS		; EDX = V1-V0 / 4
		;**shl	edx, 10		; put 6:10 int:frac into low word
		shl	edx, 10-NBITS
		mov	dx, ax		; put delta u in low word

		; Save the U1 and V1 so we don't have to divide on the
		; next iteration
		mov	U0, ebx
		mov	V0, ebp

		pop	edi		; Restore EDI before using it
		
		cmp	_Lighting_on, 0
		je	NoLight
				
		;LIGHTING CODE
		mov	ebx, _fx_l
		mov	ebp, _fx_dl_dx1

		mov	cl, [edi-1]
						
		REPT (1 SHL (NBITS-2))
			ITERATION = 0
			REPT 2
				; Do even pixel
				mov	eax, esi	; get u,v
				shr	eax, 26		; shift out all but int(v)
				shld	ax,si,6		; shift in u, shifting up v
				add	esi, edx	; inc u,v

				;LIGHTING CODE
				mov 	al, es:[eax]	; get pixel
				mov	ah, bh		; get lighting table
				;SAFETY CHECK
				and	eax, 0fffh
				add	ebx, ebp	; update lighting value

				;ANTI-ALIASING CODE
				cmp	_tmap_aliasing, 0
				je	@f
				mov	ch, fs:[eax]
				mov	al, [edi-(320+ITERATION)] 	; pixel above
				mov	ah, cl 		; pixel to left
				;**and	eax, 0ffffh
				mov	al, gs:[eax]	; average those two
				mov	ah, ch
				@@:				
				
				ror	ecx, 8
				mov	cl, gs:[eax]	; average with current

				ITERATION = ITERATION + 1
				; Do odd pixel
				mov	eax, esi	; get u,v
				shr	eax, 26		; shift out all but int(v)
				shld	ax,si,6		; shift in u, shifting up v
				add	esi, edx	; inc u,v

				;LIGHTING CODE
				mov 	al, es:[eax]	; get pixel
				mov	ah, bh		; get lighting table
				;SAFETY CHECK
				and	eax, 0fffh
				add	ebx, _fx_dl_dx2	; update lighting value

				;ANTI-ALIASING CODE
				cmp	_tmap_aliasing, 0
				je	@f
				mov	ch, fs:[eax]
				mov	al, [edi-(320+ITERATION)] 	; pixel above
				mov	ah, cl 		; pixel to left
				;**and	eax, 0ffffh
				mov	al, gs:[eax]	; average those two
				mov	ah, ch
				@@:				
				
				ror	ecx, 8
				mov	cl, gs:[eax]	; average with current

				ITERATION = ITERATION + 1


			ENDM
					
			ror ecx, 8
			mov [edi],ecx		; Draw 4 pixels to display
			rol ecx, 8	; for aliasing
			add edi,4
		ENDM

		;LIGHTING CODE
		mov	_fx_l, ebx
		pop	ebp
		pop	ecx
		pop	ebx
		dec	_loop_count
		jnz	TopOfLoop4
		jmp	EndOfLoop4

NoLight:	
		mov	cl, [edi-1]

		REPT (1 SHL (NBITS-2))
			ITERATION = 0
			REPT 4	
				LOCAL NoAA	
				LOCAL AA
				; Do 1 pixel 
				mov	eax, esi	; get u,v
				shr	eax, 26		; shift out all but int(v)
				shld	ax,si,6		; shift in u, shifting up v
				add	esi, edx	; inc u,v

				;ANTI-ALIASING CODE
				cmp	_tmap_aliasing, 0
				je	NoAA
				mov	ch, es:[eax]
				mov	al, [edi-(320+ITERATION)] 	; pixel above
				mov	ah, cl 		; pixel to left
				mov	al, gs:[eax]	; average those two
				mov	ah, ch
				ror	ecx, 8
				mov	cl, gs:[eax]	; average with current
				jmp	AA		

NoAA:				ror	ecx, 8
				mov	cl, es:[eax]	; average with current

AA:					
				ITERATION = ITERATION + 1
			ENDM
					
			ror	ecx, 8
			mov 	[edi],ecx		; Draw 4 pixels to display
			rol	ecx, 8
			add 	edi,4
		ENDM

		pop	ebp
		pop	ecx
		pop	ebx
		dec	_loop_count
		jnz	TopOfLoop4
EndOfLoop4:

	cmp	num_left_over, 0
	je	_none_to_do

DoEndPixels:
	TMAP_NORMAL
	dec	num_left_over
	jnz	DoEndPixels
	jmp	_none_to_do
	

PUBLIC 	tmap_set_selector_

	; EAX = Selector number	
	; EDX = 32-bit base address #1
	; EBX = Size in bytes #1

	; Returns:  EAX = 0 if ok, otherwise, EAX = non-zero (1)

	; Uses variables: initialized	db 10 dup (?)
	;                 selector	dw 10 dup (?) 
	;		  req_base	dd ?
	;		  req_size	dd ?
		
tmap_set_selector_:

	pusha
	
	mov	esi, eax
	
	mov	req_base, edx
	mov	req_size, ebx

	cmp	req_size, 64*64
	jge	@f

	;Too small of a texture map!!! (Not 64x64)
	int	3

@@:	cmp 	initialized[esi], 1
	je	already_initialized
	mov	initialized[esi], 1

	; Allocate a selector using DPMI
	mov	eax, 0
	mov	ecx, 1
	int	31h
	jc	failed
	mov	selector[esi*2], ax


already_initialized:	

	; Set the selector's base address using DPMI
	mov	eax, 7
	mov	bx, selector[esi*2]
	mov	ecx, req_base
	mov	edx, ecx
	shr	ecx, 16
	int	31h
	jc 	failed

	; Set the selector's size using DPMI
	mov	ax, 08h
	mov	bx, selector[esi*2]
	mov	ecx, req_size
	mov	edx, ecx
	shr	ecx, 16
	int	31h
	jc	failed

	popa
	mov	eax, 0
	ret
failed:
	popa
	mov	eax, 1
	ret



_TEXT	ends

	end

