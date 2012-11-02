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
; $Source: f:/miner/source/main/rcs/cockpit.asm $
; $Revision: 2.0 $
; $Author: john $
; $Date: 1995/02/27 11:26:51 $
; 
; Routine to copy the 3d view to the screen, leaving out
; parts that are in the cockpit.
; 
; $Log: cockpit.asm $
; Revision 2.0  1995/02/27  11:26:51  john
; New version 2.0, which has no anonymous unions, builds with
; Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
; 
; Revision 1.2  1994/01/24  18:55:36  john
; initial version.
; 
; Revision 1.1  1994/01/24  17:58:16  john
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

rcsid	db	"$Id: cockpit.asm 2.0 1995/02/27 11:26:51 john Exp $"
	align	4

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

PUBLIC cockpit_copy_

cockpit_copy_:

	; eax = source ptr
	; edx = dest ptr

	push 	esi
	push	edi
	push	ecx

	mov	esi, eax
	mov	edi, edx

	include cockpit.inc

	pop	ecx
	pop	edi
	pop	esi
	ret
	


_TEXT	ends

	end


