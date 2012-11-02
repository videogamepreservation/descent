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
; $Source: f:/miner/source/main/rcs/ipx_esr.asm $
; $Revision: 2.0 $
; $Author: john $
; $Date: 1995/02/27 11:32:31 $
; 
; Ipx esr
; 
; $Log: ipx_esr.asm $
; Revision 2.0  1995/02/27  11:32:31  john
; New version 2.0, which has no anonymous unions, builds with
; Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
; 
; Revision 1.2  1994/08/04  19:17:25  john
; Inbetween version of network stuff.
; 
; Revision 1.1  1994/08/02  11:41:09  john
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

rcsid	db	"$Id: ipx_esr.asm 2.0 1995/02/27 11:32:31 john Exp $"
	align	4

REALREGS    STRUCT
	RealEDI     dd      ?
	RealESI     dd      ?
	RealEBP     dd      ?
	Reserved    dd      ?
	RealEBX     dd      ?
	RealEDX     dd      ?
	RealECX     dd      ?
	RealEAX     dd      ?
	RealFlags   dw      ?
	RealES      dw      ?
	RealDS      dw      ?
	RealFS      dw      ?
	RealGS      dw      ?
	RealIP      dw      ?
	RealCS      dw      ?
	RealSP      dw      ?
	RealSS      dw      ?
REALREGS    ENDS


_DATA	ends

DGROUP  GROUP _DATA


_TEXT	segment	dword public USE32 'CODE'

	public ipx_esr_

ipx_esr_:	
;	pusha
;	push ds
;	push es
;	push fs
;	push gs


	push	eax
	push	esi

	; ds:esi = real mode ss:sp
	; es:edi = real mode call structure
	; ss:esp = locked protected mode API stack

;	mov	eax, es:[edi].RealEAX
;	cmp	al, 0h
;	je	good
;	cmp	al, 0ffh
;	je	good

	push	ds
	push	ebx
	mov	bx, DGROUP
	mov	ds, bx
	mov	ebx, 0b0000h
	mov	eax, es:[edi].RealEAX
	mov	byte ptr ds:[ebx], al 
	pop	ebx
	pop	ds

good:
	and 	esi, 0ffffh	
	mov	ax, ds:[esi]	
	add	esi, 2
	mov	es:[edi].RealIP, ax

	mov	ax, ds:[esi]	
	add	esi, 2
	mov	es:[edi].RealCS, ax

	mov	es:[edi].RealSP, si
	
	pop	esi
	pop	eax
;	pop gs
;	pop fs
;	pop es
;	pop ds
;	popa
	
	iretd

_TEXT	ends

	end


