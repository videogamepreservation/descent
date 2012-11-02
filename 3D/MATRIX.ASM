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
; $Source: f:/miner/source/3d/rcs/matrix.asm $
; $Revision: 1.4 $
; $Author: matt $
; $Date: 1994/11/20 15:59:34 $
;
; Source for matrix setup & manipulation routines
;
; $Log: matrix.asm $
; Revision 1.4  1994/11/20  15:59:34  matt
; instance functions were trashing eax
; 
; Revision 1.3  1994/01/04  12:06:17  matt
; Made scale_matrix save and restore ecx, which it was trashing.
; 
; Revision 1.2  1993/11/04  18:49:16  matt
; Added system to only rotate points once per frame
; 
; Revision 1.1  1993/10/29  22:20:27  matt
; Initial revision
; 
;
;

.386
	option	oldstructs

	.nolist
	include	types.inc
	include	psmacros.inc
	include	gr.inc
	include	3d.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: matrix.asm 1.4 1994/11/20 15:59:34 matt Exp $"
	align	4


_DATA	ends



_TEXT	segment	dword public USE32 'CODE'


;set view from x,y,z, p,b,h, & zoom. 
;takes esi=orient vec, edi=pos vec, eax=zoom. returns nothing
g3_set_view_angles:
	pushm	eax,edi

	mov	View_zoom,eax

	vm_copy	View_position,edi	;trashes eax

	lea	edi,View_matrix
	call	vm_angles_2_matrix

	jmp	scale_matrix		;leave eax,edi pushed


;set view from x,y,z, matrix, & zoom.
;takes esi=orientation matrix, edi=pos vec, eax=zoom. returns nothing
g3_set_view_matrix:
	pushm	eax,edi
	pushm	ecx,esi

	mov	View_zoom,eax

	vm_copy	View_position,edi	;trashes eax

	lea	edi,View_matrix
	mov	ecx,9
	rep	movsd	;slow? easy anyway...

	popm	ecx,esi

	jmp	scale_matrix	;leave eax,edi pushed


;scale memory location dest by register s
scale	macro	dest,s
	mov	eax,dest
	fixmul	s
	mov	dest,eax
	endm


;scale for aspect ratio, zoom, etc. eax,edi already pushed
scale_matrix:	pushm	esi,ebx,ecx,edx

	lea	esi,View_matrix
	lea	edi,Unscaled_matrix
	mov	ecx,9
	rep	movsd	;save before scaling

	vm_copy	Matrix_scale,Window_scale
			;get aspect ratio scale
;set scale for zoom
	mov	ebx,View_zoom
	cmp	ebx,f1_0	;> 1?
	jg	zoom_out	;..yep

	scale	Matrix_scale.z,ebx	;zoom in by scaling z
	jmp	done_zoom

zoom_out:	mov	eax,f1_0
	fixdiv	ebx	;get 1/zoom
	mov	ebx,eax
	scale	Matrix_scale.x,ebx
	scale	Matrix_scale.y,ebx	;zoom out by scaling x&y
done_zoom:

;now scale matrix elements

	mov	ebx,Matrix_scale.x
	scale	View_matrix.m1,ebx
	scale	View_matrix.m4,ebx
	scale	View_matrix.m7,ebx

	mov	ebx,Matrix_scale.y
	scale	View_matrix.m2,ebx
	scale	View_matrix.m5,ebx
	scale	View_matrix.m8,ebx

	mov	ebx,Matrix_scale.z
	scale	View_matrix.m3,ebx
	scale	View_matrix.m6,ebx
	scale	View_matrix.m9,ebx


	popm	esi,ebx,ecx,edx
	popm	eax,edi
	ret



_TEXT	ends

	end

