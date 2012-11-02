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
; $Source: f:/miner/source/main/rcs/int3hand.asm $
; $Revision: 2.0 $
; $Author: john $
; $Date: 1995/02/27 11:26:43 $
; 
; Rountine to handle int3's when debugger not present
; 
; $Log: int3hand.asm $
; Revision 2.0  1995/02/27  11:26:43  john
; New version 2.0, which has no anonymous unions, builds with
; Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
; 
; Revision 1.2  1993/10/15  11:29:45  matt
; Made stack bigger
; 
; Revision 1.1  1993/10/15  11:27:45  matt
; Initial revision
; 
; 


INT3_STACK_SIZE = 2048	;big enough for mprintf() 1000-byte buffer

.386
	option	oldstructs

	.nolist
	include	types.inc
	include	psmacros.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: int3hand.asm 2.0 1995/02/27 11:26:43 john Exp $"

int3_message	db	"int3 handler called",0ah,0

int3_stack	db	INT3_STACK_SIZE dup (?)
end_int3_stack	label	byte

save_ss	dw	?
save_esp	dd	?

	extd	__stacklow

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

	extn	mprintf_

	public	install_int3_handler_,int3_handler

install_int3_handler_:
	pusha
	mov	ax,0203h
	mov	bl,3
	mov	cx,cs
	mov	edx,offset int3_handler
	int	31h

	popa
	ret

int3_handler:	pusha
	pushm	ds,es

	mov	ax,_DATA
	mov	ds,ax
	mov	es,ax

	push	__stacklow

	mov	save_ss,ss
	mov	save_esp,esp

	sti
	mov	ss,ax
	mov	esp,offset end_int3_stack
	mov	__stacklow,offset int3_stack
	cli


	push	offset int3_message	;push string
	push	0	;window zero
	call	mprintf_
	add	esp,8	;kill stuff on stack

	sti
	mov	ss,save_ss
	mov	esp,save_esp
	cli

	pop	__stacklow

	popm	ds,es
	popa
	retf


_TEXT	ends

	end

