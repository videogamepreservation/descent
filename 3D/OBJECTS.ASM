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
; $Source: f:/miner/source/3d/rcs/objects.asm $
; $Revision: 1.5 $
; $Author: matt $
; $Date: 1993/12/07 23:04:47 $
; 
; Code to draw objects
; 
; $Log: objects.asm $
; Revision 1.5  1993/12/07  23:04:47  matt
; Misc changes.
; NOTE: This file will not be used anymore.  See object.c in main.
; 
; Revision 1.4  1993/12/02  16:09:50  matt
; Removed debugging print to mono screen
; 
; Revision 1.3  1993/11/24  16:58:49  matt
; Changed bitmap number of cube object
; 
; Revision 1.2  1993/11/23  14:29:53  matt
; Fixed bitmap objects, hopefully.
; 
; Revision 1.1  1993/11/21  20:09:02  matt
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
	include	fix.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: objects.asm 1.5 1993/12/07 23:04:47 matt Exp $"
	align	4

obj_pos	dd	?	;ptr to pos vector
obj_type	dd	?
obj_matrix	dd	?

new_obj_matrix	vms_matrix <>

obj_vec_g	vms_vector <>	;object->eye, global frame
obj_vec_obj	vms_vector <>	;in objects frame of ref.

vec_matrix_g	vms_matrix <>

;3d points for corners of bitmap
up_left_v	vms_vector <>
up_right_v	vms_vector <>
down_right_v	vms_vector <>

up_left_p	g3s_point <>
up_right_p	g3s_point <>
down_right_p	g3s_point <>

bitmap_points	dd	up_left_p,up_right_p,down_right_p

bitmap_number	dd	?

up_vec	vms_vector <>
right_vec	vms_vector <>

	public	draw_bitmap_lines
draw_bitmap_lines	dd	0

	extb	_Vector_to_viewnum,_up_vecs
	extb	_Vector_to_viewnum2,_up_vecs2


_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

	extn	render_object_

;draws a bitmap object.  takes esi=pos, edi=matrix, eax=size, ebx=obj type
g3_draw_object:	pushm	eax,ebx,ecx,edx,esi,edi

	mov	obj_type,ebx	;save objnum
	mov	obj_pos,esi	;save pos
	mov	obj_matrix,edi	;save matrix

	push	eax	;save size

;get vector from object to eye
	lea	eax,obj_vec_g	;dest
	mov	edi,esi	;obj position
	lea	esi,View_position	;eye positon
	call	vm_vec_sub

;normalize vector
	lea	esi,obj_vec_g
	call	vm_vec_normalize

;rotate into object's frame of reference
	lea	eax,obj_vec_obj
	lea	esi,obj_vec_g
	mov	edi,obj_matrix
	call	vm_vec_rotate

;select the right bitmap by looking in a table.
;get five bits for each of x,y,z, make a 15-bit index, and look in table

	mov	eax,obj_vec_obj.x
	add	eax,f1_0	;make in range 0..2
	break_if	s,'bad value in vector'
	cmp	eax,f2_0
	jl	not_max_x
	dec	eax	;make less than 1
not_max_x:	sar	eax,2
	and	eax,111110000000000b
	mov	ebx,eax

	mov	eax,obj_vec_obj.y
	add	eax,f1_0	;make in range 0..2
	break_if	s,'bad value in vector'
	cmp	eax,f2_0
	jl	not_max_y
	dec	eax	;make less than 1
not_max_y:	sar	eax,2+5
	and	eax,1111100000b
	or	ebx,eax

	mov	eax,obj_vec_obj.z
	add	eax,f1_0	;make in range 0..2
	break_if	s,'bad value in vector'
	cmp	eax,f2_0
	jl	not_max_z
	dec	eax	;make less than 1
not_max_z:	sar	eax,2+10
	and	eax,11111b
	or	ebx,eax

	movzx	eax,_Vector_to_viewnum[ebx]
	cmp	obj_type,1	;other object?
	jne	got_viewnum
	movzx	eax,_Vector_to_viewnum2[ebx]
got_viewnum:
	mov	bitmap_number,eax

;;	debug	"bm=%d\n",eax

;build matrix with vec from obj to eye & bitmap's up vector

	mov	eax,bitmap_number
	sal	eax,1	;*2
	add	eax,bitmap_number	;*3
	sal	eax,2	;*12
	lea	eax,_up_vecs[eax]

	cmp	obj_type,1	;other object/
	jne	got_up_vec
	add	eax,offset _up_vecs2
	sub	eax,offset _up_vecs
got_up_vec:

	lea	edi,vec_matrix_g
	lea	esi,obj_vec_obj	;obj_vec_g
	call	vm_vector_2_matrix

;rotate object's matrix through new matrix

	lea	eax,new_obj_matrix
	mov	esi,obj_matrix
	lea	edi,vec_matrix_g
	call	vm_matrix_x_matrix

;now, up and right describe edges

	vm_copy	up_vec,new_obj_matrix.uvec
	vm_copy	right_vec,new_obj_matrix.rvec

;calculate corner points of bitmap

	pop	ecx	;get size
	sar	ecx,1

	lea	ebx,up_vec
	call	vm_vec_scale

	lea	ebx,right_vec
	call	vm_vec_scale

;upper left
	lea	eax,up_left_v
	mov	esi,obj_pos
	lea	edi,right_vec
	call	vm_vec_add

	mov	edi,eax
	lea	esi,up_vec
	call	vm_vec_add2

;upper right
	lea	eax,up_right_v
	lea	esi,up_left_v
	lea	edi,right_vec
	call	vm_vec_sub

	mov	esi,edi	;rvec
	mov	edi,eax	;dest=up_right
	call	vm_vec_sub2

;lower right
	lea	eax,down_right_v
	lea	esi,up_right_v
	lea	edi,up_vec
	call	vm_vec_sub

	mov	esi,edi	;uvec
	mov	edi,eax	;dest=down_right
	call	vm_vec_sub2

;now take three points, rotate & project

	lea	esi,up_left_v
	lea	edi,up_left_p
	call	g3_rotate_point
	or	bl,bl
	js	obj_off_screen

	lea	esi,up_right_v
	lea	edi,up_right_p
	call	g3_rotate_point
	or	bl,bl
	js	obj_off_screen

	lea	esi,down_right_v
	lea	edi,down_right_p
	call	g3_rotate_point
	or	bl,bl
	js	obj_off_screen

;set all z values same before project
	mov	eax,up_left_p.z
	mov	up_right_p.z,eax
	mov	down_right_p.z,eax

	lea	esi,up_left_p
	call	g3_project_point
	lea	esi,up_right_p
	call	g3_project_point
	lea	esi,down_right_p
	call	g3_project_point


	mov	eax,obj_type
	mov	edx,bitmap_number
	lea	ebx,bitmap_points
	call	render_object_

	test	draw_bitmap_lines,-1
	jz	skip_lines

 extn gr_setcolor_,gr_line_

	mov	eax,15
	call	gr_setcolor_

	mov	eax,up_left_p.p3_sx
	mov	edx,up_left_p.p3_sy

	mov	ebx,up_right_p.p3_sx
	mov	ecx,up_right_p.p3_sy

	call	gr_line_

	mov	ebx,up_right_p.p3_sx
	mov	ecx,up_right_p.p3_sy

	mov	eax,down_right_p.p3_sx
	mov	edx,down_right_p.p3_sy

	call	gr_line_
skip_lines:

obj_off_screen:
	popm	eax,ebx,ecx,edx,esi,edi
	ret



_TEXT	ends

	end

