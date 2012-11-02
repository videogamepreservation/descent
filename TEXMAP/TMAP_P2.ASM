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
; $Source: f:/miner/source/texmap/rcs/tmap_p2.asm $
; $Revision: 1.5 $
; $Author: mike $
; $Date: 1994/11/30 00:56:58 $
;
; Perspective texture mapper with scan doubling inner loop.
;
; $Log: tmap_p2.asm $
; Revision 1.5  1994/11/30  00:56:58  mike
; optimization.
; 
; Revision 1.4  1994/11/12  16:41:07  mike
; jae -> ja.
; 
; Revision 1.3  1994/03/14  15:44:42  mike
; streamline code.
; 
; Revision 1.2  1994/01/14  14:02:02  mike
; *** empty log message ***
; 
;

	.386

	public	asm_tmap_scanline_per_doubled_

	include	tmap_inc.asm


	extern	_fx_l:dword
	extern 	_fx_dl_dx:dword
	extern	_dither_intensity_lighting:dword
	extern	_Lighting_on:dword,_per2_flag:dword

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'
	align	4

	extrn	_pixel_data_selector:word, _gr_fade_table_selector:word
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

; ----------^^ These are passed in by the C caller ^^----------

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

_DATA	ENDS

DGROUP	GROUP	_DATA


wnr	macro	ofs, reg
	push	edi
	add	edi, _bytes_per_row
	mov	ofs[edi], reg
	pop	edi
	endm


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


TMAP_NORMAL MACRO
	LOCAL NoLight1,skip1

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
	shld 	ebx,eax,6	; esi = v*64+u

	; read 1  pixel
	mov	al,es:[ebx]	; get pixel from source bitmap

	cmp	_Lighting_on, 0
	je	NoLight1
	;LIGHTING CODE
	mov	ebx, _fx_l	; get temp copy of lighting value
	mov	ah, bh	; get lighting level
	and	eax, 0fffh
	add	ebx, _fx_dl_dx1	; update lighting value
	mov	al, fs:[eax]	; xlat pixel thru lighting tables
	mov	_fx_l, ebx	; save temp copy of lighting value
					
	;DITHERING CODE
	mov 	ebx, _fx_dl_dx1
	xchg	ebx, _fx_dl_dx2
	mov	_fx_dl_dx1, ebx
NoLight1:
	
	; write 1 pixel
	cmp	al,255
	je	skip1
	mov	[edi],al
	wnr	0, al
skip1:	inc	edi
	
	; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx

	xchg	esi, ebx
ENDM

asm_tmap_scanline_per_doubled_:
	push	es
	push	fs
	pusha
	
;---------------------------- setup for loop ---------------------------------
; Setup for loop:	_loop_count  iterations = (int) xright - (int) xleft
;	esi	source pixel pointer = pixptr
;	edi	initial row pointer = y*320+x

; set esi = pointer to start of texture map data
;**	mov	esi,_pixptr
	mov	es,_pixel_data_selector	; selector[0*2]
	mov	fs,_gr_fade_table_selector	; selector[1*2]	; fs = bmd_fade_table

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

	cmp	_Lighting_on, 0
	je	tmap_loop_fast_nolight
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

	align	4
_none_to_do:	
	popa
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
;	; Pixel 0
;		pixels = texmap[V0*64+U0];
;		U0 += DUDX; V0 += DVDX
;	; Pixel 1
;		pixels = (pixels<<8)+texmap[V0*64+U0];
;		U0 += DUDX; V0 += DVDX
;	; Pixel 2
;		pixels = (pixels<<8)+texmap[V0*64+U0];
;		U0 += DUDX; V0 += DVDX
;	; Pixel 3
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

tmap_loop_fast:

	align	4
NotDwordAligned1:
	test	edi, 11b
	jz	DwordAligned1

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
	shld 	ebx,eax,6	; esi = v*64+u

	; read 1  pixel
	mov	al,es:[ebx]	; get pixel from source bitmap

	;LIGHTING CODE
	mov	ebx, _fx_l	; get temp copy of lighting value
	mov	ah, bh	; get lighting level
	add	ebx, _fx_dl_dx1	; update lighting value
	and	eax, 0fffh	; make sure no garbage in EAX
	mov	al, fs:[eax]	; xlat pixel thru lighting tables
	mov	_fx_l, ebx	; save temp copy of lighting value
					
	;DITHERING CODE
	mov 	ebx, _fx_dl_dx1
	xchg	ebx, _fx_dl_dx2
	mov	_fx_dl_dx1, ebx

	; write 1 pixel
	cmp	al,255
	je	skip2
	mov	[edi],al
	wnr	0, al
skip2:	inc	edi
	
	; update deltas
	add	ebp,_fx_dv_dx
	add	esi,_fx_du_dx
	add	ecx,_fx_dz_dx

	xchg	esi, ebx

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
	lea	eax, [eax*8]
	mov	DU1, eax
	mov	eax, _fx_dv_dx
	lea	eax, [eax*8]
	mov	DV1, eax
	mov	eax, _fx_dz_dx
	lea	eax, [eax*8]
	mov	DZ1, eax

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
;**	mov	ebp, _fx_dl_dx1
	mov	ebp, _bytes_per_row

	REPT (1 SHL (NBITS-2))
	local	skip3
	REPT 2
; Do even pixel
;-ORIGINAL-	mov	eax, esi	; get u,v
;-ORIGINAL-	shr	eax, 26	; shift out all but int(v)
;-ORIGINAL-	shld	ax,si,6	; shift in u, shifting up v
;-ORIGINAL-	mov 	al, es:[eax]	; get pixel from source bitmap
;-ORIGINAL-	add	esi, edx	; inc u,v
;-ORIGINAL-	mov	ah, bh	; form lighting table lookup value
;-ORIGINAL-	add	ebx, _fx_dl_dx1	; update lighting value
;-ORIGINAL-	and	eax, 0fffh	; SAFETY CHECK on lighting value 
;-ORIGINAL-	mov	cl, fs:[eax]	; xlat thru lighting table into dest buffer
;-ORIGINAL-	ror	ecx, 8	; move to next dest pixel position
;-ORIGINAL-
;-ORIGINAL-; Do odd pixel
;-ORIGINAL-	mov	eax, esi	; get u,v
;-ORIGINAL-	shr	eax, 26	; shift out all but int(v)
;-ORIGINAL-	shld	ax,si,6	; shift in u, shifting up v
;-ORIGINAL-	mov 	al, es:[eax]	; get pixel from source bitmap
;-ORIGINAL-	add	esi, edx	; inc u,v
;-ORIGINAL-	mov	ah, bh	; form lighting table lookup value
;-ORIGINAL-	add	ebx, _fx_dl_dx2	; update lighting value
;-ORIGINAL-	and	eax, 0fffh	; SAFETY CHECK on lighting value 
;-ORIGINAL-	mov	cl, fs:[eax]	; xlat thru lighting table into dest buffer
;-ORIGINAL-	ror	ecx, 8	; move to next dest pixel position

	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	add	esi, edx	; inc u,v
	mov 	al, es:[eax]	; get pixel from source bitmap
	mov	ah, bh	; form lighting table lookup value
	and	ah,0fh
	add	ebx, _fx_dl_dx1	; update lighting value
	mov	cl, fs:[eax]	; xlat thru lighting table into dest buffer

; Do odd pixel
	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	add	esi, edx	; inc u,v
	mov 	al, es:[eax]	; get pixel from source bitmap
	mov	ah, bh	; form lighting table lookup value
	and	ah, 0fh
	add	ebx, _fx_dl_dx2	; update lighting value
	mov	ch, fs:[eax]	; xlat thru lighting table into dest buffer

	ror	ecx, 16	; move to next double dest pixel position

	ENDM
					
	cmp	ecx,-1
	je	skip3
	mov 	[edi],ecx	; Draw 4 pixels to display
	mov	[edi+ebp], ecx
skip3:	add 	edi,4
	ENDM

; LIGHTING CODE
	mov	_fx_l, ebx
	pop	ebp
	pop	ecx
	pop	ebx
	dec	_loop_count
	jnz	TopOfLoop4

EndOfLoop4:
	cmp	num_left_over, 0
	je	_none_to_do

DoEndPixels:
	add	ebx, DU1
	add	ebp, DV1
	add	ecx, DZ1

	push	edi	; use edi as a temporary variable

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

	pop	edi	; Restore EDI before using it
		
	mov	ecx, num_left_over

; LIGHTING CODE
	mov	ebx, _fx_l
;**	mov	ebp, _fx_dl_dx1
	mov	ebp, _bytes_per_row

	ITERATION = 0
	REPT (1 SHL (NBITS-1))
	local	skip4,skip6
; Do even pixel
	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	mov 	al, es:[eax]	; get pixel from source bitmap
	add	esi, edx	; inc u,v
	mov	ah, bh	; form lighting table lookup value
	add	ebx, _fx_dl_dx1	; update lighting value
	and	ah,0fh
	mov	al, fs:[eax]	; xlat thru lighting table into dest buffer
	cmp	al,255
	je	skip4
	mov	[edi+ITERATION], al	; write pixel
	mov	[edi+ebp+ITERATION], al
skip4:	dec	ecx
	jz	_none_to_do
	ITERATION = ITERATION + 1

; Do odd pixel
	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	mov 	al, es:[eax]	; get pixel from source bitmap
	add	esi, edx	; inc u,v
	mov	ah, bh	; form lighting table lookup value
	add	ebx, _fx_dl_dx2	; update lighting value
	and	ah, 0fh	; SAFETY CHECK on lighting value 
	mov	al, fs:[eax]	; xlat thru lighting table into dest buffer
	cmp	al,255
	je	skip6
	mov	[edi+ITERATION], al	; write pixel
	mov	[edi+ebp+ITERATION], al
skip6:	dec	ecx
	jz	_none_to_do
	ITERATION = ITERATION + 1
	ENDM

; Should never get here!!!!
	int	3
	jmp	_none_to_do

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
	shld 	ebx,eax,6	; esi = v*64+u

	; read 1  pixel
	mov	al,es:[ebx]	; get pixel from source bitmap
	; write 1 pixel
	cmp	al,255
	je	skip7
	mov	[edi],al
	mov	edx,_bytes_per_row
	mov	[edi+edx], al
skip7:	inc	edi
	
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
		
	REPT (1 SHL (NBITS-2))
	local	skip8
	REPT 4	
; Do 1 pixel 
	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	mov	cl, es:[eax]	; load into buffer register
	add	esi, edx	; inc u,v
	ror	ecx, 8	; move to next dest pixel

	ENDM
		
	cmp	ecx,-1
	je	skip8
	mov 	[edi],ecx	; Draw 4 pixels to display
	mov	ebp, _bytes_per_row
	mov	[edi+ebp], ecx
skip8:	add 	edi,4
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

	push	edi	; use edi as a temporary variable

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

	pop	edi	; Restore EDI before using it
		
	mov	ecx, num_left_over

	mov	ebp,_bytes_per_row

	ITERATION = 0
	REPT (1 SHL NBITS)
	local	skip9
; Do 1 pixel 
	mov	eax, esi	; get u,v
	shr	eax, 26	; shift out all but int(v)
	shld	ax,si,6	; shift in u, shifting up v
	mov	al, es:[eax]	; load into buffer register
	add	esi, edx	; inc u,v
	cmp	al,255
	je	skip9
	mov	[edi+ITERATION], al	; write pixel
	mov	[edi+ebp+ITERATION], al
skip9:	dec	ecx
	jz	_none_to_do
	ITERATION = ITERATION + 1
	ENDM

; Should never get here!!!!!
	int	3
	jmp	_none_to_do

	

_TEXT	ends

	end


