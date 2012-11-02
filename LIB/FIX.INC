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
; $Source: f:/miner/source/fix/rcs/fix.inc $
; $Revision: 1.7 $
; $Author: matt $
; $Date: 1994/01/19 23:12:00 $
;
; Header for fixed-point library
;
; $Log: fix.inc $
; Revision 1.7  1994/01/19  23:12:00  matt
; Made fix_atan2() left-handed, like our coordinate system
; 
; Revision 1.6  1993/10/20  01:08:49  matt
; Add fix_asin(), improved fix_atan2()
; 
; Revision 1.5  1993/10/19  23:53:36  matt
; Added fix_atan2()
; 
; Revision 1.4  1993/10/19  22:32:12  matt
; Added fix_acos()
; 
; Revision 1.3  1993/09/13  12:09:42  matt
; Added extf,extfa macros to generate externdef's of fixed-point types
; 
; Revision 1.2  1993/09/10  11:54:12  matt
; Added missing 'endif' at end of file
; 
; Revision 1.1  1993/08/24  12:59:36  matt
; Initial revision
; 
;
;

ifndef	fix_inc
fix_inc equ 1

	include	types.inc
	include	psmacros.inc

;Fixed-point types

fix	typedef	dword
fixang	typedef	word

;Externdef macros for fixed-point types

	extgen	fix,f	;generates extf
	extgen	fixang,fa	;generates extfa

;Some handy constants

f0_0	equ 0
f1_0	equ 10000h
f2_0	equ 20000h
f3_0	equ 30000h
f10_0	equ 0a0000h
f0_5	equ 8000h
f0_1	equ 199ah

;Macros

;fixed-point multiply. one parm in eax, other passed to macro. result in eax
;trashes edx
fixmul	macro	n
	imul	n
	shrd	eax,edx,16
	endm

;fixed-point divide. numerator in eax, divisor passed to macro. result in eax
;trashes edx. made sure parameter is not edx
fixdiv	macro	n
	mov	edx,eax
	sar	edx,16
	shl	eax,16
	idiv	n
	endm

;fixed-point multiply and divide. result in eax
;trashes edx. made sure neither parameter is edx
fixmuldiv	macro	a,b
	imul	a
	idiv	b
	endm


;Functions
	extn	fix_fastsincos	;ax=ang, ret eax=sin, ebx=cos
	extn	fix_sincos	;ax=ang, ret eax=sin, ebx=cos
	extn	fix_asin	;takes eax=sin, ret ax=angle
	extn	fix_acos	;takes eax=cos, ret ax=angle
	extn	fix_atan2	;takes eax,ebx = cos,sin, ret ax=angle
	extn	long_sqrt	;takes eax, returns ax
	extn	fix_sqrt	;takes eax, returns eax	
	extn	quad_sqrt	;takes eds:eax, returns eax
endif
