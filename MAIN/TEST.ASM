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
	.386

	public	john_test_, mike_test_
	public	_test_count

_DATA	SEGMENT DWORD PUBLIC USE32 'DATA'

_Big_test_count	dd	40000h
uv	dd	0
_test_count	dd	0
bonk	db	4096 dup (0)

_DATA	ENDS

DGROUP	GROUP	_DATA


_TEXT   SEGMENT DWORD PUBLIC USE32 'CODE'
	ASSUME	DS:_DATA
	ASSUME	CS:_TEXT

john_test_:
	pusha

	mov	eax, _Big_test_count
	mov	_test_count, eax
john_again:

	mov	edi, offset bonk

	sub	eax,eax
	sub	esi,esi

	mov	dl,3fh

; .833
;    REPT 8
;	mov	ah, byte ptr uv+3	; get v
;	mov	al, byte ptr uv+1	; get u
;	and	ah, 3fh
;	add	uv, ecx	; inc u,v
;	mov	al, [esi+eax]	; get source pixel
;	mov	ah, bh	; form lighting lookup value
;	mov	al, [eax]	; get lit pixel from fadetable
;	mov	[edi], al	; write pixel to frame buffer
;	inc	edi	; inc dest
;	add	ebx, ebp	; inc lighting value
;    ENDM

; .833
    REPT 8
	mov	ah, byte ptr uv+3	; get v
	mov	al, byte ptr uv+1	; get u
	and	ah, dl
	add	uv, ecx	; inc u,v
	mov	al, [esi+eax]	; get source pixel
	mov	ah, bh	; form lighting lookup value
	mov	al, [eax]	; get lit pixel from fadetable
	add	ebx, ebp	; inc lighting value
	mov	[edi], al	; write pixel to frame buffer
	inc	edi	; inc dest
    ENDM

	dec	_test_count
	jne	john_again

	popa
	ret

mike_test_:
	pusha

	mov	eax, _Big_test_count
	mov	_test_count, eax

mike_again:
	mov	edi, offset bonk

; .733
;    REPT 8
;	mov	eax, esi	; get u,v
;	shr	eax, 26	; shift out all but int(v)
;	shld	ax,si,6	; shift in u, shifting up v
;	add	esi, edx	; inc u,v
;	mov 	al, [eax]	; get pixel from source bitmap
;	mov	ah, bh	; form lighting table lookup value
;	add	ebx, ebp	; update lighting value
;	mov	al, [eax]	; xlat thru lighting table into dest buffer
;	mov	[edi], al
;	inc	edi
;
;    ENDM

; .780
;    REPT 8
;	mov	eax, esi	; get u,v
;	shr	eax, 10	; shift out all but int(v)
;	mov	ax,si	; shift in u, shifting up v
;	shr	eax,10	; inc u,v
;	add	esi,edx
;	mov 	al, [eax]	; get pixel from source bitmap
;	mov	ah, bh	; form lighting table lookup value
;	add	ebx, ebp	; update lighting value
;	mov	al, [eax]	; xlat thru lighting table into dest buffer
;	mov	[edi], al
;	inc	edi
;
;    ENDM

    REPT 8
	mov	eax, esi	; get u,v
	shr	eax, 10	; shift out all but int(v)
	mov	ax,si	; shift in u, shifting up v
	shr	eax,10	; inc u,v
	add	esi,edx
	mov 	al, [eax]	; get pixel from source bitmap
	mov	ah, bh	; form lighting table lookup value
	add	ebx, ebp	; update lighting value
	mov	al, [eax]	; xlat thru lighting table into dest buffer
	mov	[edi], al
	inc	edi

    ENDM

	dec	_test_count
	jne	mike_again

	popa
	ret

_TEXT	ENDS

	END

