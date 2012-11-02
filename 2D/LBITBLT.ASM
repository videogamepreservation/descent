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
; $Source: f:/miner/source/2d/rcs/lbitblt.asm $
; $Revision: 1.7 $
; $Author: mike $
; $Date: 1994/08/12 17:38:28 $
; 
; Bitblt from linear to linear
; 
; $Log: lbitblt.asm $
; Revision 1.7  1994/08/12  17:38:28  mike
; Fix bug.
; 
; Revision 1.6  1994/08/11  17:59:33  mike
; Assembler versions for 3 rotations of merge functions.
; 
; Revision 1.5  1994/01/22  14:35:16  john
; Added transparency as color index 255.
; 
; Revision 1.4  1994/01/18  21:42:41  john
; unrolled loops in gr_merge_textures...
; 
; Revision 1.3  1994/01/18  10:56:04  john
; Added code to merge two ttextures.
; 
; Revision 1.2  1993/12/28  12:07:26  john
; initial version
; 
; Revision 1.1  1993/12/22  11:05:46  john
; Initial revision
; 
; 



.386
	option	oldstructs

	.nolist
	include	types.inc
	include	psmacros.inc
	include gr.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: lbitblt.asm 1.7 1994/08/12 17:38:28 mike Exp $"
	align	4


	JumpTable	dd	RowAligned0, RowAligned1, RowAligned2, RowAligned3

row_count	dd	?

_DATA	ends


_TEXT	segment	dword public USE32 'CODE'

; C-calling:
; void gr_lbitblt( grs_bitmap * source, grs_bitmap * dest, int height, int width )

PUBLIC	gr_lbitblt_

gr_lbitblt_:

	; EAX = grs_bitmap * source
	; EDX = grs_bitmap * dest
	; EBX = height
	; ECX = width

	push	esi
	push	edi
	push	ebp

	dec	ebx
	js	Done

	mov	esi, [eax].bm_data
	mov	edi, [edx].bm_data

	movzx	eax, [eax].bm_rowsize
	sub	eax, ecx

	movzx	edx, [edx].bm_rowsize
	sub	edx, ecx

	; Can use ebp, eax, edx, ecx
	mov	ebp, ecx
	cmp	ebp, 16
	jb	RowAligned0		; If not 16 pixels wide, don't worry about alignment

	mov	ecx, edi
	and	ecx, 011b
	jmp     NEAR32 PTR JumpTable[ecx*4]

RowAligned0:
NextRowAligned0:	
	mov	ecx, ebp
	shr	ecx, 2
	rep     movsd
	mov	ecx, ebp
	and	ecx, 11b
	rep	movsb

	add	esi, eax
	add	edi, edx
	dec	ebx
	jns	NextRowAligned0

	pop	ebp
	pop	edi
	pop	esi
	ret

RowAligned1:
	sub	ebp, 3

NextRowAligned1:
	movsw	
	movsb	
	mov	ecx, ebp
	shr	ecx, 2
	rep     movsd
	mov	ecx, ebp
	and	ecx, 11b
	rep	movsb

	add	esi, eax
	add	edi, edx
	dec	ebx
	jns	NextRowAligned1

	pop	ebp
	pop	edi
	pop	esi
	ret

RowAligned2:
	sub	ebp, 2
NextRowAligned2:
	movsw
	mov	ecx, ebp
	shr	ecx, 2
	rep     movsd
	mov	ecx, ebp
	and	ecx, 11b
	rep	movsb

	add	esi, eax
	add	edi, edx
	dec	ebx
	jns	NextRowAligned2

	pop	ebp
	pop	edi
	pop	esi
	ret

RowAligned3:
	dec	ebp
NextRowAligned3:
	movsb
	mov	ecx, ebp
	shr	ecx, 2
	rep     movsd
	mov	ecx, ebp
	and	ecx, 11b
	rep	movsb

	add	esi, eax
	add	edi, edx
	dec	ebx
	jns	NextRowAligned3

Done:	pop	ebp
	pop	edi
	pop	esi
	ret


; C-calling:
; void gr_merge_textures( ubyte * lower, ubyte * upper, ubyte * dest )

PUBLIC	gr_merge_textures_, gr_merge_textures_1_, gr_merge_textures_2_, gr_merge_textures_3_

; case 1:
;    // 
;    for (y=0; y<64; y++ )
;       for (x=0; x<64; x++ )   {
;          c = top_data[ 64*x+(63-y) ];      
;          if (c==255)
;             c = bottom_data[ 64*y+x ];
;          *dest_data++ = c;
;       }
;    break;
; case 2:
;    // Normal
;    for (y=0; y<64; y++ )
;       for (x=0; x<64; x++ )   {
;          c = top_data[ 64*(63-y)+(63-x) ];
;          if (c==255)
;             c = bottom_data[ 64*y+x ];
;          *dest_data++ = c;
;       }
;    break;
; case 3:
;    // Normal
;    for (y=0; y<64; y++ )
;       for (x=0; x<64; x++ )   {
;          c = top_data[ 64*(63-x)+y  ];
;          if (c==255)
;             c = bottom_data[ 64*y+x ];
;          *dest_data++ = c;
;       }
;    break;
gr_merge_textures_:

	; EAX = lower data
	; EDX = upper data
	; EBX = dest data

	push	ebp
	push	edi
	push	ecx
	push	esi

	mov	ebp, edx
	mov	edi, ebx
	mov	bl, 255
	mov	bh, bl
	and	ebx, 0ffffh
	and	edx, 0ffffh
	mov	esi, eax
	mov	ecx, (64*64)/2

	jmp	gmt1

second_must_be_not_trans:
	mov	ah, dh
	mov	[edi],ax
	add	edi,2
	dec	ecx
	je	donegmt

gmt1:	mov	dx, [ebp]
	add 	ebp, 2
	cmp	edx, ebx
	jne	not_transparent

; both transparent
	movsw
	dec	ecx
	jne	gmt1
	jmp	donegmt

; DH OR DL ARE INVISIBLE
not_transparent:
	mov	ax,[esi]
	add	esi,2
		
	cmp	dl, bl
	je	second_must_be_not_trans
	mov	al, dl
	cmp	dh, bh
	je	@f
	mov	ah, dh
@@:	mov	[edi],ax
	add	edi,2
	dec	ecx
	jne	gmt1

donegmt:

	pop	esi
	pop	ecx	
	pop	edi
	pop	ebp
	ret

; -----------------------------------------------------------------------------------------
; case 1, normal in x, flip in y
gr_merge_textures_1_:

; eax = background data
; edx = foreground data
; ebx = destination address

	push	ebp
	push	edi
	push	ecx
	push	esi
	push	ebx

	mov	ch, 255	; transparent color, stick in a register, is this faster?

	mov	esi, 63	; esi will be the offset to the current pixel
	mov	row_count, esi

	mov	ebp, 64	; do for 64 pixels

	align	4
gmt1_1:	mov	cl, [edx + esi]	; get foreground pixel
	add 	esi, 64	; point at next row
	cmp	cl, ch	; see if transparent
	jne	not_transparent_1	; dl contains a solid color, just write it

	mov	cl,[eax]	; get background pixel

not_transparent_1:	mov	[ebx], cl	; write pixel
	inc	ebx	; point at next destination address
	inc	eax

	dec	ebp	; see if done a whole row
	jne	gmt1_1	; no, so do next pixel

	mov	ebp, 64	; do for 64 pixels

	dec	row_count	; advance to next row
	mov	esi, row_count	; doing next row, get offset, DANGER: DOESN'T SET FLAGS, BEWARE ON 68000, POWERPC!!
	jns	gmt1_1	; no (going down to 0)

	pop	ebx
	pop	esi
	pop	ecx	
	pop	edi
	pop	ebp
	ret

; -----------------------------------------------------------------------------------------
; case 1, normal in x, flip in y
gr_merge_textures_2_:

; eax = background data
; edx = foreground data
; ebx = destination address

	push	ebp
	push	edi
	push	ecx
	push	esi
	push	ebx

	mov	ch, 255	; transparent color, stick in a register, is this faster?

	mov	esi, 63 + 64*63	; esi will be the offset to the current pixel

	align	4
gmt1_2:	mov	cl, [edx + esi]	; get foreground pixel
	cmp	cl, ch	; see if transparent
	jne	not_transparent_2	; dl contains a solid color, just write it

	mov	cl,[eax]	; get background pixel

not_transparent_2:	mov	[ebx], cl	; write pixel
	inc	ebx	; point at next destination address
	inc	eax

	dec	esi	; advance to next row
	jns	gmt1_2	; no (going down to 0)

	pop	ebx
	pop	esi
	pop	ecx	
	pop	edi
	pop	ebp
	ret

; -----------------------------------------------------------------------------------------
; case 1, normal in x, flip in y
gr_merge_textures_3_:

; eax = background data
; edx = foreground data
; ebx = destination address

	push	ebp
	push	edi
	push	ecx
	push	esi
	push	ebx

	mov	ch, 255	; transparent color, stick in a register, is this faster?

	mov	esi, 64*63	; esi will be the offset to the current pixel
	mov	row_count, 64

	mov	ebp, 32	; do for 64 pixels (2x loop)

; first copy of loop
	align	4
gmt1_3:	mov	cl, [edx + esi]	; get foreground pixel
	sub 	esi, 64	; point at next row
	cmp	cl, ch	; see if transparent
	jne	not_transparent_3	; dl contains a solid color, just write it

	mov	cl,[eax]	; get background pixel

not_transparent_3:	inc	eax
	mov	[ebx], cl	; write pixel
	inc	ebx	; point at next destination address

; second copy of loop
	mov	cl, [edx + esi]	; get foreground pixel
	sub 	esi, 64	; point at next row
	cmp	cl, ch	; see if transparent
	jne	nt_3a	; dl contains a solid color, just write it

	mov	cl,[eax]	; get background pixel

nt_3a:	inc	eax
	mov	[ebx], cl	; write pixel
	inc	ebx	; point at next destination address

	dec	ebp	; see if done a whole row
	jne	gmt1_3	; no, so do next pixel

	mov	ebp, 32	; do for 64 pixels

	add	esi, 64*64+1	; doing next row, get offset, DANGER: DOESN'T SET FLAGS, BEWARE ON 68000, POWERPC!!

	dec	row_count	; advance to next row
	jne	gmt1_3	; no (going down to 0)

	pop	ebx
	pop	esi
	pop	ecx	
	pop	edi
	pop	ebp
	ret

_TEXT	ends

	end

