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
; $Source: f:/miner/source/2d/rcs/temp.asm $
; $Revision: 1.2 $
; $Author: john $
; $Date: 1994/01/18 10:56:48 $
; 
; .
; 
; $Log: temp.asm $
; Revision 1.2  1994/01/18  10:56:48  john
; *** empty log message ***
; 
; Revision 1.1  1994/01/12  14:38:08  john
; Initial revision
; 
; 



.386
	option	oldstructs

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: temp.asm 1.2 1994/01/18 10:56:48 john Exp $"
	align	4

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

			push	ebx
			push	ecx
			push	edx
			mov	ecx, 0x12345678
			mov	ebx, 0x12345678
			mov	edx, 0x12345678
			mov	ecx, ebx	;( ebx or edx )

			; this will be repeated n times
			mov	al, [esi]
			mov	ecx, ebx	; one or
			mov	ecx, edx	; the other
			inc	esi
			cmp	al, ah
			je	@f
			rep	stosb
		@@:	add	edi, ecx
		
			pop	edx
			pop	ecx
			pop	ebx
			ret	

_TEXT	ends

	end


