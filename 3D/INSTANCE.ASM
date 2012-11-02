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
; $Source: f:/miner/source/3d/rcs/instance.asm $
; $Revision: 1.8 $
; $Author: matt $
; $Date: 1994/07/29 18:16:16 $
; 
; Code for handling instanced 3d objects
; 
; $Log: instance.asm $
; Revision 1.8  1994/07/29  18:16:16  matt
; Added instance by angles, and corrected parms for g3_init()
; 
; Revision 1.7  1994/07/25  00:00:02  matt
; Made 3d no longer deal with point numbers, but only with pointers.
; 
; Revision 1.6  1994/06/16  17:52:31  matt
; If NULL passed for instance matrix, don't rotate (just do offset)
; 
; Revision 1.5  1994/06/01  18:10:22  matt
; Fixed register trash in g3_start_instance_matrix()
; 
; Revision 1.4  1994/03/25  17:09:20  matt
; Increase MAX_INSTANCE_DEPTH to 5 (from 3)
; 
; Revision 1.3  1994/02/10  18:00:43  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.2  1994/01/24  14:08:45  matt
; Added code to this previously dull file
; 
; Revision 1.1  1994/01/23  15:22:52  matt
; Initial revision
; 
; 



.386
	option	oldstructs

	.nolist
	include	types.inc
	include	psmacros.inc
	include	vecmat.inc
	include	3d.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: instance.asm 1.8 1994/07/29 18:16:16 matt Exp $"
	align	4

MAX_INSTANCE_DEPTH	equ	5

inst_context_size	equ	((size vms_matrix) + (size vms_vector))

instance_stack	db	MAX_INSTANCE_DEPTH*inst_context_size dup (?)

instance_depth	dd	0

tempv	vms_vector <>
tempm	vms_matrix <>
tempm2	vms_matrix <>

instmat	vms_matrix <>

_DATA	ends


_TEXT	segment	dword public USE32 'CODE'

;start instancing, using angles (called vm_angles_2_matrix)
;takes esi=position, edi=angvec. trashes esi,edi
;if angles==NULL, don't modify matrix.  This will be like doing an offset
g3_start_instance_angles:
	or	edi,edi
	jz	g3_start_instance_matrix	;no new matrix

	push	esi
	mov	esi,edi
	lea	edi,instmat
	call	vm_angles_2_matrix
	pop	esi

;	fall	g3_start_instance_matrix

;start instancing, using a matrix
;takes esi=position, edi=matrix. trashes esi,edi
;if matrix==NULL, don't modify matrix.  This will be like doing an offset   
g3_start_instance_matrix:
	pushm	eax,ebx,ecx

	push	edi	;save matrix
	push	esi	;save position

;save current context
	mov	eax,instance_depth
	inc	instance_depth

	ifndef	NDEBUG
	 cmp	eax,MAX_INSTANCE_DEPTH
	 break_if	e,'Already at max depth'
	endif

	imul	eax,inst_context_size
	lea	edi,instance_stack[eax]

	lea	esi,View_position
	mov	ecx,3
	rep	movsd
	lea	esi,View_matrix
	mov	ecx,9
	rep	movsd


;step 1: subtract object position from view position
	pop	esi	;object position
	lea	edi,View_position
	call	vm_vec_sub2

;step 2: rotate view vector through object matrix

	pop	edi	;get object matrix
	or	edi,edi	;null matrix?
	jz	no_inst_matrix
	lea	esi,View_position
	lea	eax,tempv
	call	vm_vec_rotate
	vm_copy	esi,eax,ebx

;step 3: rotate object matrix through view_matrix (vm = ob * vm)

	mov	esi,edi	;object matrix
 lea edi,tempm2
 call vm_copy_transpose_matrix
 lea esi,tempm2
	lea	edi,View_matrix
	lea	eax,tempm
	call	vm_matrix_x_matrix
	mov	esi,eax
	mov	ecx,9
	rep	movsd

no_inst_matrix:

;now we are done! 
	popm	eax,ebx,ecx
	ret

;we are done instancing
g3_done_instance:	pushm	eax,ecx,esi,edi
	dec	instance_depth
	break_if	s,'Instance stack underflow!'
	mov	eax,instance_depth
	imul	eax,inst_context_size
	lea	esi,instance_stack[eax]
	lea	edi,View_position
	mov	ecx,3
	rep	movsd
	lea	edi,View_matrix
	mov	ecx,9
	rep	movsd
	popm	eax,ecx,esi,edi
	ret


_TEXT	ends

	end

