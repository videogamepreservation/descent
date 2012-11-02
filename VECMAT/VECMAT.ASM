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
; $Source: f:/miner/source/vecmat/rcs/vecmat.asm $
; $Revision: 1.54 $
; $Author: matt $
; $Date: 1995/01/31 00:14:50 $
;
; Source for vector/matrix library
;
; $Log: vecmat.asm $
; Revision 1.54  1995/01/31  00:14:50  matt
; Took out int3 from dotprod overflow, since it saturates now
; 
; Revision 1.53  1994/12/14  18:29:33  matt
; Made dotprod overflow check stay in, and return saturated value
; 
; Revision 1.52  1994/12/14  12:34:13  matt
; Disabled now-unused vector_2_matrix_norm()
; 
; Revision 1.51  1994/12/13  16:55:13  matt
; Ripped out optimization from last version, which was bogus
; 
; Revision 1.50  1994/12/13  14:55:18  matt
; Use quick normalize in a couple of places where it was safe to do so
; 
; Revision 1.49  1994/12/13  14:44:12  matt
; Added vm_vector_2_matrix_norm()
; 
; Revision 1.48  1994/12/13  13:26:49  matt
; Fixed overflow check
; 
; Revision 1.47  1994/12/03  15:39:54  matt
; Gracefully handle some vector_2_matrix problems
; 
; Revision 1.46  1994/11/19  17:15:05  matt
; Assemble out some code not used in DESCENT
; 
; Revision 1.45  1994/11/17  11:41:05  matt
; Put handling in extract_angles_from_matrix to deal with bad matrices
; 
; Revision 1.44  1994/11/16  11:48:10  matt
; Added error checking to vm_extract_angles_matrix()
; 
; Revision 1.43  1994/09/19  22:00:10  matt
; Fixed register trash
; 
; Revision 1.42  1994/09/11  19:23:05  matt
; Added vm_vec_normalized_dir_quick()
; 
; Revision 1.41  1994/08/14  13:28:38  matt
; Put in check for zero-length vector in extract angles
; 
; Revision 1.40  1994/07/19  18:52:53  matt
; Added vm_vec_normalize_quick() and vm_vec_copy_normalize_quick()
; 
; Revision 1.39  1994/06/16  18:24:22  matt
; Added vm_vec_mag_quick()
; 
; Revision 1.38  1994/06/10  23:18:38  matt
; Added new code for vm_vec_ang_2_matrix() which may be better, but may
; not be.
; 
; Revision 1.37  1994/05/22  18:17:29  mike
; Optimize vm_vec_dist_quick, using jns in place of abs_eax.
; 
; Revision 1.36  1994/05/19  12:07:04  matt
; Fixed globals and macros and added a constant
; 
; Revision 1.35  1994/05/19  09:19:00  matt
; Made vm_vec_normalized_dir() return mag of vector
; 
; Revision 1.34  1994/05/18  22:28:01  matt
; Added function vm_vec_normalized_dir()
; Added C macros IS_ZERO_VEC(), vm_vec_zero(), and vm_set_identity()
; Added C global static vars vmd_zero_vector & vmd_identity_matrix
; 
; Revision 1.33  1994/05/18  21:44:16  matt
; Added functions:
;   vm_extract_angles_vector()
;   vm_extract_angles_vector_normalized()
;   vm_vec_copy_normalize()
; 
; Revision 1.32  1994/05/13  12:41:51  matt
; Added new function, vm_vec_dist_quick(), which does an approximation.
; 
; Revision 1.31  1994/05/04  17:41:31  mike
; Comment out debug_brk on null vector.
; 
; Revision 1.30  1994/04/15  21:41:31  matt
; Check for foward vector straigt up in extract angles routine
; 
; Revision 1.29  1994/03/30  15:45:05  matt
; Added two functions, vm_vec_scale_add() & vm_vec_scale_add2()
; 
; Revision 1.28  1994/02/26  19:23:35  matt
; Do an int3 when we get a null vector when computing surface normal
; 
; Revision 1.27  1994/02/10  18:29:45  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.26  1994/02/10  18:28:55  matt
; Fixed bugs in extract angles function
; 
; Revision 1.25  1994/01/31  22:46:07  matt
; Added vm_extract_angles_matrix() function
; 
; Revision 1.24  1994/01/30  19:29:55  matt
; Put in debug_brk when vm_vec_2_matrix got zero-length vector
; 
; Revision 1.23  1994/01/25  15:27:59  matt
; Added debugging check for dotprod overflow
; 
; Revision 1.22  1994/01/24  11:52:59  matt
; Added checking for dest==src for several functions where this is not allowed
; 
; Revision 1.21  1994/01/19  23:13:02  matt
; Fixed bug in vm_vec_ang_2_matrix()
; 
; Revision 1.20  1994/01/04  12:33:43  mike
; Prevent divide overflow in vm_vec_scale2
; 
; Revision 1.19  1993/12/21  19:46:26  matt
; Added function vm_dist_to_plane()
; 
; Revision 1.18  1993/12/13  17:26:23  matt
; Added vm_vec_dist()
; 
; Revision 1.17  1993/12/02  12:43:39  matt
; New functions: vm_vec_copy_scale(), vm_vec_scale2()
; 
; Revision 1.16  1993/10/29  22:39:29  matt
; Changed matrix order, making direction vectors the rows
; 
; Revision 1.15  1993/10/29  18:06:01  matt
; Fixed vm_vector_2_matrix() bug when forward vector was straight down
; 
; Revision 1.14  1993/10/26  18:51:26  matt
; Fixed some register trashes in vm_vec_ang_2_matrix()
; 
; Revision 1.13  1993/10/25  11:49:37  matt
; Made vm_vec_delta_ang() take optional forward vector to return signed delta
; 
; Revision 1.12  1993/10/20  01:09:42  matt
; Added vm_vec_delta_ang(), vm_vec_delta_ang_norm(), and vm_vec_ang_2_matrix()
; 
; Revision 1.11  1993/10/17  17:03:08  matt
; vm_vector_2_matrix() now takes optional right vector
; 
; Revision 1.10  1993/10/10  18:11:42  matt
; Changed angles_2_matrix so that heading & bank rotate in the 
; correct directions.
; 
; Revision 1.9  1993/09/30  16:17:59  matt
; Fixed bug in vector_2_matrix() by adding another normalize
; 
; Revision 1.8  1993/09/29  10:51:58  matt
; Fixed bad register trashes in crossprod, perp, & normal
; 
; Revision 1.7  1993/09/28  12:16:46  matt
; Fixed bugs in cross product
; Added func vm_vector_2_matrix()
; 
; Revision 1.6  1993/09/24  21:19:37  matt
; Added vm_vec_avg() and vm_vec_avg4()
; 
; Revision 1.5  1993/09/20  18:15:07  matt
; Trap zero-length vectors in vm_vec_normalize(), vm_vec_perp(), and vm_vec_normal()
; 
; Revision 1.4  1993/09/20  14:56:43  matt
; Fixed bug in vm_vec_normal(), made that routine normalize the results,
; and added new function vm_vec_perp().
; 
; Revision 1.3  1993/09/20  10:12:06  mike
; no changes
; 
; Revision 1.2  1993/09/17  11:10:33  matt
; Added vm_vec_add2() and vm_vec_sub2(), which take 2 args (dest==src0)
; 
; Revision 1.1  1993/09/16  20:10:24  matt
; Initial revision
; 
;
;

.386
	option	oldstructs

	.nolist
	include	psmacros.inc
	include	vecmat.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: vecmat.asm 1.54 1995/01/31 00:14:50 matt Exp $"
	even

;these symbols enable/disable code which is unused in DESCENT
AVG4_ENABLED	= 0
V2MN_ENABLED	= 0

;temporary vectors for surface normal calculation
tempv0	vms_vector <>
tempv1	vms_vector <>

xvec	vms_vector <>
yvec	vms_vector <>
zvec	vms_vector <>

tempav	vms_angvec <>

;sine & cosine values for angles_2_matrix
sinp	fix	?
cosp	fix	?
sinb	fix	?
cosb	fix	?
sinh	fix	?
cosh	fix	?

;These should never be changed!
_vmd_zero_vector	label	vms_vector
	fix	0,0,0

_vmd_identity_matrix label	vms_matrix
	fix	f1_0,0,0
	fix	0,f1_0,0
	fix	0,0,f1_0
 
_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

;add two vectors, filling in dest
;takes eax=dest, esi,edi=sources, returns eax=dest
vm_vec_add:	push	ebx	;save work reg

	for	ofs,<x,y,z>
	 mov	ebx,[esi].ofs
	 add	ebx,[edi].ofs
	 mov	[eax].ofs,ebx
	endm

	pop	ebx

	ret

;subtracts two vectors, filling in dest
;takes eax=dest, esi,edi=sources, returns eax=dest
vm_vec_sub:	push	ebx	;save work reg

	for	ofs,<x,y,z>
	 mov	ebx,[esi].ofs
	 sub	ebx,[edi].ofs
	 mov	[eax].ofs,ebx
	endm

	pop	ebx

	ret


;adds one vector to antother
;takes edi=dest, esi=source, returns edi=dest
vm_vec_add2:	push	ebx	;save work reg

	for	ofs,<x,y,z>
	 mov	ebx,[esi].ofs
	 add	[edi].ofs,ebx
	endm

	pop	ebx

	ret

;subtract one vector from another
;takes edi=dest, esi=source, returns edi=dest
vm_vec_sub2:	push	ebx	;save work reg

	for	ofs,<x,y,z>
	 mov	ebx,[esi].ofs
	 sub	[edi].ofs,ebx
	endm

	pop	ebx

	ret

;averages two vectors. takes eax=dest, esi,edi=srcs
vm_vec_avg:	push	ebx

	for	ofs,<x,y,z>
	 mov	ebx,[esi].ofs
	 add	ebx,[edi].ofs
	 sar	ebx,1
	 mov	[eax].ofs,ebx
	endm

	pop	ebx
	ret

	if	AVG4_ENABLED
;averages four vectors. takes eax=dest, esi,edi,ecx,edx=srcs
vm_vec_avg4:	push	ebx

	for	ofs,<x,y,z>
	 mov	ebx,[esi].ofs
	 add	ebx,[edi].ofs
	 add	ebx,[ecx].ofs
	 add	ebx,[edx].ofs
	 sar	ebx,2
	 mov	[eax].ofs,ebx
	endm

	pop	ebx
	ret
	endif

;scales a vector in place.  takes ebx=vector, ecx=scale. returns ebx=vector
vm_vec_scale:	pushm	eax,edx

	for	ofs,<x,y,z>
	 mov	eax,[ebx].ofs
	 fixmul	ecx
	 mov	[ebx].ofs,eax
	endm

	popm	eax,edx
	ret

;scales and copies a vector.  takes edi=dest, ebx=src, ecx=scale. returns edi=vector
vm_vec_copy_scale:	pushm	eax,edx

	for	ofs,<x,y,z>
	 mov	eax,[ebx].ofs
	 fixmul	ecx
	 mov	[edi].ofs,eax
	endm

	popm	eax,edx
	ret

;scales a vector, adds it to another, and stores in a 3rd
;takes edi=dest, ebx=src1, esi=src2, ecx=scale. returns edi=vector
vm_vec_scale_add:	pushm	eax,edx

	for	ofs,<x,y,z>
	 mov	eax,[esi].ofs
	 fixmul	ecx
	 add	eax,[ebx].ofs
	 mov	[edi].ofs,eax
	endm

	popm	eax,edx
	ret

;scales a vector and adds it to another.  takes edi=dest, esi=src, ecx=scale. returns edi=vector
vm_vec_scale_add2:	pushm	eax,edx

	for	ofs,<x,y,z>
	 mov	eax,[esi].ofs
	 fixmul	ecx
	 add	[edi].ofs,eax
	endm

	popm	eax,edx
	ret

;scales a vector in place, taking n/d for scale.  takes edi=vector, ebx=n,ecx=d. returns edi=vector
vm_vec_scale2:	pushm	eax,edx

	or	ecx,ecx	; @mk, 01/04/94, prevent divide overflow
	je	vmvs_out
	for	ofs,<x,y,z>
	 mov	eax,[edi].ofs
	 imul	ebx
	 idiv	ecx
	 mov	[edi].ofs,eax
	endm

	popm	eax,edx
vmvs_out:	ret

;compute the distance between two points. (does sub and mag)
;takes esi,edi=points, returns eax=dist
vm_vec_dist:	pushm	ebx,ecx,edx

	mov	eax,[esi].x
	sub	eax,[edi].x
	imul	eax
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y
	sub	eax,[edi].y
	imul	eax
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z
	sub	eax,[edi].z
	imul	eax
	add	eax,ebx
	adc	edx,ecx

	call	quad_sqrt

	popm	ebx,ecx,edx
	ret


;computes an approximation of the magnitude of a vector
;uses dist = largest + next_largest*3/8 + smallest*3/16
;takes esi=vector, returns eax=dist
	align	4
vm_vec_mag_quick:	pushm	ebx,ecx,edx

	mov	eax,[esi].x
	or	eax,eax
	jns	eax_ok2
	neg	eax
eax_ok2:

	mov	ebx,[esi].y
	or	ebx,ebx
	jns	ebx_ok2
	neg	ebx
ebx_ok2:

	mov	ecx,[esi].z
	or	ecx,ecx
	jns	ecx_ok2
	neg	ecx
ecx_ok2:

mag_quick_eax_ebx_ecx:

	cmp	eax,ebx
	jg	no_swap_ab
	xchg	eax,ebx
no_swap_ab:	cmp	ebx,ecx
	jg	do_add
	xchg	ebx,ecx
	cmp	eax,ebx
	jg	do_add
	xchg	eax,ebx

do_add:	sar	ebx,2	;    b*1/4
	sar	ecx,3	;            c*1/8
	add	ebx,ecx	;    b*1/4 + c*1/8
	add	eax,ebx	;a + b*1/4 + c*1/8
	sar	ebx,1	;    b*1/8 + c*1/16
	add	eax,ebx	;a + b*3/4 + c*3/16

	popm	ebx,ecx,edx
	ret

;computes an approximation of the distance between two points.
;uses dist = largest + next_largest*3/8 + smallest*3/16
;takes esi,edi=points, returns eax=dist
	align	4
vm_vec_dist_quick:	pushm	ebx,ecx,edx

	mov	ebx,[esi].x
	sub	ebx,[edi].x
	jns	ebx_ok
	neg	ebx
ebx_ok:

	mov	ecx,[esi].y
	sub	ecx,[edi].y
	jns	ecx_ok
	neg	ecx
ecx_ok:
	mov	eax,[esi].z
	sub	eax,[edi].z
	jns	eax_ok
	neg	eax
eax_ok:
	jmp	mag_quick_eax_ebx_ecx


;compute magnitude of vector. takes esi=vector, returns eax=mag
vm_vec_mag:	pushm	ebx,ecx,edx

	mov	eax,[esi].x
	imul	eax
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y
	imul	eax
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z
	imul	eax
	add	eax,ebx
	adc	edx,ecx

	call	quad_sqrt

	popm	ebx,ecx,edx
	ret


;return the normalized direction vector between two points
;dest = normalized(end - start).
;takes edi=dest, esi=endpoint, ebx=startpoint.  Returns mag of dir vec
;NOTE: the order of the parameters matches the vector subtraction
vm_vec_normalized_dir:
	pushm	ebp,eax,ebx,edx
	mov	ebp,ebx

	mov	eax,[esi].x
	sub	eax,[ebp].x
	mov	[edi].x,eax
	imul	eax
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y
	sub	eax,[ebp].y
	mov	[edi].y,eax
	imul	eax
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z
	sub	eax,[ebp].z
	mov	[edi].z,eax
	imul	eax
	add	eax,ebx
	adc	edx,ecx

	call	quad_sqrt

	mov	ecx,eax	;mag in ecx
	jecxz	no_div2

	for	ofs,<x,y,z>
	 mov	eax,[edi].ofs
	 fixdiv	ecx
	 mov	[edi].ofs,eax
	endm

no_div2:
	;return value (mag) in ecx

	popm	ebp,eax,ebx,edx
	ret

	
;save as vm_vec_normalized_dir, but with quick sqrt
;takes edi=dest, esi=endpoint, ebx=startpoint.  Returns mag of dir vec
vm_vec_normalized_dir_quick:
	pushm	eax,edi,esi

	mov	eax,edi
	mov	edi,ebx	;regs right for sub
	call	vm_vec_sub

	mov	esi,eax
	call	vm_vec_normalize_quick

	;return value (mag) in ecx

	popm	eax,edi,esi
	ret

	

;normalize a vector in place.  takes esi=vector
;returns ecx=mag of source vec. trashes edi
vm_vec_normalize:	push	edi
	mov	edi,esi
	call	vm_vec_copy_normalize
	pop	edi
	ret

;normalize a vector.  takes edi=dest, esi=vector
;returns ecx=mag of source vec
vm_vec_copy_normalize:
	pushm	eax,ebx,edx

	mov	eax,[esi].x
	imul	eax
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y
	imul	eax
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z
	imul	eax
	add	eax,ebx
	adc	edx,ecx

	call	quad_sqrt

	mov	ecx,eax	;mag in ecx
	jecxz	no_div

	for	ofs,<x,y,z>
	 mov	eax,[esi].ofs
	 fixdiv	ecx
	 mov	[edi].ofs,eax
	endm

no_div:	popm	eax,ebx,edx

	ret

;normalize a vector in place.  takes esi=vector
;returns ecx=mag of source vec. trashes edi
;uses approx. dist
vm_vec_normalize_quick:
	push	edi
	mov	edi,esi
	call	vm_vec_copy_normalize_quick
	pop	edi
	ret

;normalize a vector.  takes edi=dest, esi=vector
;returns ecx=mag of source vec
;uses approx. dist
vm_vec_copy_normalize_quick:
	pushm	eax,ebx,edx

	call	vm_vec_mag_quick

	mov	ecx,eax	;mag in ecx
	jecxz	no_div_q

	for	ofs,<x,y,z>
	 mov	eax,[esi].ofs
	 fixdiv	ecx
	 mov	[edi].ofs,eax
	endm

no_div_q:	popm	eax,ebx,edx

	ret

;compute dot product of two vectors. takes esi,edi=vectors, returns eax=dotprod
vm_vec_dotprod:	pushm	ebx,ecx,edx

	mov	eax,[esi].x
	imul	[edi].x
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y
	imul	[edi].y
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z
	imul	[edi].z
	add	eax,ebx
	adc	edx,ecx

	shrd	eax,edx,16

	;ifndef	NDEBUG	;check for overflow
	;always do overflow check, and return saturated value
	sar	edx,16	;get real sign from high word
	mov	ebx,edx
	cdq		;get sign of our result 
	cmp	bx,dx	;same sign?
	je	no_oflow
	;;debug_brk	'overflow in vm_vec_dotprod'
	mov	eax,7fffffffh
	or	ebx,ebx	;check desired sign
	jns	no_oflow
	neg	eax
no_oflow:
	;endif

	popm	ebx,ecx,edx

	ret


;computes cross product of two vectors. takes eax=dest, esi,edi=src vectors
;returns eax=dest.  Note: this magnitude of the resultant vector is the
;product of the magnitudes of the two source vectors.  This means it is
;quite easy for this routine to overflow and underflow.  Be careful that
;your inputs are ok.
vm_vec_crossprod:
	ifndef	NDEBUG
	 cmp	eax,esi
	 break_if	e,'crossprod: dest==src0'
	 cmp	eax,edi
	 break_if	e,'crossprod: dest==src1'
	endif

	pushm	ebx,ecx,edx,ebp

	mov	ebp,eax

	mov	eax,[edi].y
	imul	[esi].z
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,[edi].z
	imul	[esi].y
	sub	eax,ebx
	sbb	edx,ecx
	shrd	eax,edx,16
	ifndef	NDEBUG	;check for overflow
	 mov	ebx,edx	;save
	 cdq		;get sign of result
	 shr	ebx,16	;get high 16 of quad result
	 cmp	dx,bx	;sign extension the same?
	 break_if	ne,'overflow in crossprod'
	endif
	mov	[ebp].x,eax

	mov	eax,[edi].z
	imul	[esi].x
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,[edi].x
	imul	[esi].z
	sub	eax,ebx
	sbb	edx,ecx
	shrd	eax,edx,16
	ifndef	NDEBUG	;check for overflow
	 mov	ebx,edx	;save
	 cdq		;get sign of result
	 shr	ebx,16	;get high 16 of quad result
	 cmp	dx,bx	;sign extension the same?
	 break_if	ne,'overflow in crossprod'
	endif
	mov	[ebp].y,eax

	mov	eax,[edi].x
	imul	[esi].y
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,[edi].y
	imul	[esi].x
	sub	eax,ebx
	sbb	edx,ecx
	shrd	eax,edx,16
	ifndef	NDEBUG	;check for overflow
	 mov	ebx,edx	;save
	 cdq		;get sign of result
	 shr	ebx,16	;get high 16 of quad result
	 cmp	dx,bx	;sign extension the same?
	 break_if	ne,'overflow in crossprod'
	endif
	mov	[ebp].z,eax

	mov	eax,ebp	;return dest in eax

	popm	ebx,ecx,edx,ebp

	ret

abs_eax	macro
	cdq
	xor	eax,edx
	sub	eax,edx
	endm

;computes surface normal from three points. takes ebx=dest, eax,esi,edi=vecs
;returns eax=dest. Result vector is normalized.
vm_vec_normal:
	call	vm_vec_perp	;get unnormalized
	push	ecx
	xchg	esi,eax	;get in esi, save esi
	call	vm_vec_normalize
	xchg	eax,esi
	pop	ecx
	ret


;make sure a vector is reasonably sized to go into a cross product
;trashes eax,ebx,cl,edx
check_vec:	mov	eax,[esi].x
	abs_eax
	mov	ebx,eax
	mov	eax,[esi].y
	abs_eax
	or	ebx,eax
	mov	eax,[esi].z
	abs_eax
	or	ebx,eax
	jz	null_vector

	xor	cl,cl	;init shift count

	test	ebx,0fffc0000h	;too big
	jz	not_too_big
check_4_down:	test	ebx,000f00000h
	jz	check_2_down
	add	cl,4
	sar	ebx,4
	jmp	check_4_down
check_2_down:	test	ebx,0fffc0000h
	jz	not_2_down
	add	cl,2
	sar	ebx,2
	jmp	check_2_down
not_2_down:
	sar	[esi].x,cl
	sar	[esi].y,cl
	sar	[esi].z,cl
	ret

;maybe too small...
not_too_big:	test	ebx,0ffff8000h
	jnz	not_too_small
check_4_up:	test	ebx,0fffff000h
	jnz	check_2_up
	add	cl,4
	sal	ebx,4
	jmp	check_4_up
check_2_up:	test	ebx,0ffff8000h
	jnz	not_2_up
	add	cl,2
	sal	ebx,2
	jmp	check_2_up
not_2_up:
	sal	[esi].x,cl
	sal	[esi].y,cl
	sal	[esi].z,cl

not_too_small:	ret

null_vector:
; debug_brk commented out by mk on 05/04/94
;**	debug_brk	"null vector in check_vec"
	ret


;computes surface normal from three points. takes ebx=dest, eax,esi,edi=vecs
;returns eax=dest. Result vector is NOT normalized, but this routine does
;make an effort that cross product does not overflow or underflow  
vm_vec_perp:	pushm	esi,edi	;save for return

	push	eax	;save src0
	
	xchg	esi,edi
	lea	eax,tempv1
	call	vm_vec_sub	;src2 - src1

	mov	esi,edi	;get src1 in esi
	pop	edi	;get src0 in edi
	lea	eax,tempv0
	call	vm_vec_sub	;src1 - src0

	mov	esi,eax	;tempv0 in esi
	lea	edi,tempv1	;tempv1 in edi

	pushm	ebx,ecx,edx
	call	check_vec	;make sure reasonable value
	xchg	esi,edi
	call	check_vec	;make sure reasonable value
	xchg	esi,edi
	popm	ebx,ecx,edx

	mov	eax,ebx	;get dest in eax
	call	vm_vec_crossprod

	popm	esi,edi	;restore regs
	ret


;compute a rotation matrix from three angles. takes edi=dest matrix, 
;esi=angles vector.  returns edi=dest matrix. 
vm_angles_2_matrix:
	pushm	eax,edx,ebx,ecx,esi

;get sines & cosines
	mov	ax,[esi].pitch
	call	fix_sincos
	mov	sinp,eax
	mov	cosp,ebx

	mov	ax,[esi].bank
	call	fix_sincos
	mov	sinb,eax
	mov	cosb,ebx

	mov	ax,[esi].head
	call	fix_sincos
	mov	sinh,eax
	mov	cosh,ebx

;alternate entry point with sines & cosines already computed.  
;Note all the registers already pushed.
sincos_2_matrix:

;now calculate the 9 elements

	mov	eax,sinb
	fixmul	sinh
	mov	ecx,eax	;save sbsh
	fixmul	sinp
	mov	ebx,eax
	mov	eax,cosb
	fixmul	cosh
	mov	esi,eax	;save cbch
	add	eax,ebx
	mov	[edi].m1,eax	;m1=cbch+sbspsh

	mov	eax,esi	;get cbch
	fixmul	sinp
	add	eax,ecx	;add sbsh
	mov	[edi].m8,eax	;m8=sbsh+cbchsp


	mov	eax,cosb
	fixmul	sinh
	mov	ecx,eax	;save cbsh
	fixmul	sinp
	mov	ebx,eax
	mov	eax,sinb
	fixmul	cosh
	mov	esi,eax	;save sbch
	sub	ebx,eax
	mov	[edi].m2,ebx	;m2=cbshsp-sbch

	mov	eax,esi	;get sbch
	fixmul	sinp
	sub	eax,ecx	;sub from cbsh
	mov	[edi].m7,eax	;m7=sbchsp-cbsh


	mov	eax,sinh
	fixmul	cosp
	mov	[edi].m3,eax	;m3=shcp

	mov	eax,sinb
	fixmul	cosp
	mov	[edi].m4,eax	;m4=sbcp

	mov	eax,cosb
	fixmul	cosp
	mov	[edi].m5,eax	;m5=cbcp

	mov	eax,sinp
	neg	eax
	mov	[edi].m6,eax	;m6=-sp

	mov	eax,cosh
	fixmul	cosp
	mov	[edi].m9,eax	;m9=chcp

	popm	eax,edx,ebx,ecx,esi
	ret

m2m	macro	dest,src
	mov	eax,src
	mov	dest,eax
	endm

m2m_neg	macro	dest,src
	mov	eax,src
	neg	eax
	mov	dest,eax
	endm

;create a rotation matrix from one or two vectors. 
;requires forward vec, and assumes zero bank if up & right vecs==NULL
;up/right vector need not be exactly perpendicular to forward vec
;takes edi=matrix, esi=forward vec, eax=up vec, ebx=right vec. 
;returns edi=matrix.  trashes eax,ebx,esi
;Note: this routine loses precision as the forward vector approaches 
;straigt up or down (I think)
vm_vector_2_matrix:
	pushm	ecx

	ifndef	NDEBUG
	 or	esi,esi
	 break_if	z,"vm_vector_2_matrix: forward vec cannot be NULL!"
	endif

	or	eax,eax	;up vector present?
	jnz	use_up_vec	;..yep

	or	ebx,ebx	;right vector present?
	jz	just_forward_vec	;..nope
;use_right_vec
	push	edi	;save matrix
	mov	edi,ebx	;save right vec

	vm_copy	zvec,[esi]
	lea	esi,zvec
	call	vm_vec_normalize
	or	ecx,ecx
	jecxz	bad_vector2

	lea	esi,xvec
	vm_copy	[esi],[edi]
	call	vm_vec_normalize
	jecxz	bad_vector2

	lea	eax,yvec	;dest = y
	mov	edi,esi	;src1 = x
	lea	esi,zvec	;scr0 = z
	call	vm_vec_crossprod	;get y = z cross x

;normalize new perpendicular vector
	mov	esi,eax	;get new vec (up) in esi
	call	vm_vec_normalize
	jecxz	bad_vector2

;now recompute right vector, in case it wasn't entirely perpendiclar

	lea	eax,xvec	;dest = x
	lea	edi,zvec
	call	vm_vec_crossprod	;x = y cross z

	pop	edi	;get matrix back

	jmp	copy_into_matrix


;one of the non-forward vectors caused a problem, so ignore them and
;use just the forward vector
bad_vector2:	pop	edi
	lea	esi,zvec
	jmp	just_forward_vec

;use forward and up vectors
use_up_vec:	push	edi	;save matrix
	mov	edi,eax	;save up vec

	vm_copy	zvec,[esi]
	lea	esi,zvec
	call	vm_vec_normalize
	jecxz	bad_vector2

	lea	esi,yvec
	vm_copy	[esi],[edi]
	call	vm_vec_normalize
	jecxz	bad_vector2

	lea	eax,xvec	;dest = x
	lea	edi,zvec	;scr0 = y, scr1 = z
	call	vm_vec_crossprod	;get x vector

;normalize new perpendicular vector
	xchg	esi,eax	;get new vec in esi
	call	vm_vec_normalize
	jecxz	bad_vector2	;bad_vector3

;now recompute up vector, in case it wasn't entirely perpendiclar

	xchg	esi,edi	;dest = y, src0 = z, src1 = x
	call	vm_vec_crossprod	;get x vector

	pop	edi	;get matrix back

copy_into_matrix:	vm_copy	[edi].rvec,xvec
	vm_copy	[edi].uvec,yvec
	vm_copy	[edi].fvec,zvec

	jmp	done_v2m

bad_vector3:	pop	edi
bad_vector:	pop	ecx
	debug_brk	'0-len vec in vec_2_mat'
	ret

;only the forward vector is present
just_forward_vec:	vm_copy	zvec,[esi]
	lea	esi,zvec
	call	vm_vec_normalize
	jecxz	bad_vector

	vm_copy	[edi].fvec,[esi]

	mov	eax,[esi].x
	or	eax,[esi].z	;check both x & z == 0
	jnz	not_up

;forward vector is straight up (or down)

	mov	[edi].m1,f1_0
	push	edx
	mov	eax,[esi].y	;get y componant
	cdq		;get y sign
	mov	eax,-f1_0
	xor	eax,edx
	sub	eax,edx	;make sign correct
	mov	[edi].m8,eax
	pop	edx
	xor	eax,eax
	mov	[edi].m4,eax
	mov	[edi].m7,eax
	mov	[edi].m2,eax
	mov	[edi].m5,eax
	jmp	done_v2m

not_up:
	m2m	xvec.x,[esi].z
	mov	xvec.y,0
	m2m_neg	xvec.z,[esi].x
	lea	esi,xvec
	call	vm_vec_normalize

	vm_copy	[edi].rvec,[esi]

	push	edi	;save matrix
	mov	edi,esi	;scr1 = x
	lea	esi,zvec	;src0 = z
	lea	eax,yvec	;dest = y
	call	vm_vec_crossprod
	pop	edi

	vm_copy	[edi].uvec,[yvec]

done_v2m:	pop	ecx
	ret

	if	V2MN_ENABLED
;this version requires that the vectors be more-or-less normalized
vm_vector_2_matrix_norm:
	pushm	ecx

	ifndef	NDEBUG
	 or	esi,esi
	 break_if	z,"vm_vector_2_matrix_norm: forward vec cannot be NULL!"
	endif

	or	eax,eax	;up vector present?
	jnz	_use_up_vec	;..yep

	or	ebx,ebx	;right vector present?
	jz	_just_forward_vec	;..nope
;use_right_vec
	push	edi	;save matrix
	mov	edi,ebx	;save right vec

	vm_copy	zvec,[esi]
	vm_copy	xvec,[edi]

	lea	eax,yvec	;dest = y
	call	vm_vec_crossprod	;get y = z cross x

;normalize new perpendicular vector
	xchg	esi,eax	;get new vec in esi
	call	vm_vec_normalize
	jecxz	_bad_vector2	;bad_vector3

;now recompute right vector, in case it wasn't entirely perpendiclar

	lea	eax,xvec	;dest = x
	lea	edi,zvec
	call	vm_vec_crossprod	;x = y cross z

	pop	edi	;get matrix back

	jmp	_copy_into_matrix


;one of the non-forward vectors caused a problem, so ignore them and
;use just the forward vector
_bad_vector2:	pop	edi
	lea	esi,zvec
	jmp	_just_forward_vec

;use forward and up vectors
_use_up_vec:	push	edi	;save matrix
	mov	edi,eax	;save up vec

	vm_copy	zvec,[esi]
	vm_copy	yvec,[edi]

	lea	eax,xvec	;dest = x
	lea	esi,yvec	;scr0 = y
	lea	edi,zvec	;scr1 = z
	call	vm_vec_crossprod	;get x vector

;normalize new perpendicular vector
	xchg	esi,eax	;get new vec in esi
	call	vm_vec_normalize
	jecxz	_bad_vector2	;bad_vector3

;now recompute up vector, in case it wasn't entirely perpendiclar

	xchg	esi,edi	;dest = y, src0 = z, src1 = x
	call	vm_vec_crossprod	;get x vector

	pop	edi	;get matrix back

_copy_into_matrix:	vm_copy	[edi].rvec,xvec
	vm_copy	[edi].uvec,yvec
	vm_copy	[edi].fvec,zvec

	jmp	_done_v2m

_bad_vector3:	pop	edi
_bad_vector:	pop	ecx
	debug_brk	'0-len vec in vec_2_mat'
	ret

;only the forward vector is present
_just_forward_vec:	vm_copy	zvec,[esi]
	vm_copy	[edi].fvec,[esi]

	mov	eax,[esi].x
	or	eax,[esi].z	;check both x & z == 0
	jnz	_not_up

;forward vector is straight up (or down)

	mov	[edi].m1,f1_0
	push	edx
	mov	eax,[esi].y	;get y componant
	cdq		;get y sign
	mov	eax,-f1_0
	xor	eax,edx
	sub	eax,edx	;make sign correct
	mov	[edi].m8,eax
	pop	edx
	xor	eax,eax
	mov	[edi].m4,eax
	mov	[edi].m7,eax
	mov	[edi].m2,eax
	mov	[edi].m5,eax
	jmp	_done_v2m

_not_up:
	m2m	xvec.x,[esi].z
	mov	xvec.y,0
	m2m_neg	xvec.z,[esi].x
	lea	esi,xvec
	call	vm_vec_normalize

	vm_copy	[edi].rvec,[esi]

	push	edi	;save matrix
	mov	edi,esi	;scr1 = x
	lea	esi,zvec	;src0 = z
	lea	eax,yvec	;dest = y
	call	vm_vec_crossprod
	pop	edi

	vm_copy	[edi].uvec,[yvec]

_done_v2m:	pop	ecx
	ret

	endif	;V2MN_ENABLED


;multiply (dot) two vectors. assumes dest ptr in ebp, src pointers in esi,edi.
;trashes ebx,ecx,edx
vv_mul	macro	dest,x0,y0,z0,x1,y1,z1

	mov	eax,[esi].x0
	imul	[edi].x1
	mov	ebx,eax
	mov	ecx,edx

	mov	eax,[esi].y0
	imul	[edi].y1
	add	ebx,eax
	adc	ecx,edx

	mov	eax,[esi].z0
	imul	[edi].z1
	add	ebx,eax
	adc	ecx,edx

	shrd	ebx,ecx,16	;fixup ebx
	mov	[ebp].dest,ebx

	endm

;rotate a vector by a rotation matrix
;eax=dest vector, esi=src vector, edi=matrix. returns eax=dest vector
vm_vec_rotate:
	ifndef	NDEBUG
	 cmp	eax,esi
	 break_if	e,'vec_rotate: dest==src'
	endif

	pushm	ebx,ecx,edx,ebp

	mov	ebp,eax	;dest in ebp

;compute x
	vv_mul	x, x,y,z, m1,m4,m7
	vv_mul	y, x,y,z, m2,m5,m8
	vv_mul	z, x,y,z, m3,m6,m9

	mov	eax,ebp	;return eax=dest
	popm	ebx,ecx,edx,ebp

	ret


;transpose a matrix in place.  Takes edi=matrix. returns edi=matrix
vm_transpose_matrix:
	push	eax

	mov	eax,[edi].m2
	xchg	eax,[edi].m4
	mov	[edi].m2,eax

	mov	eax,[edi].m3
	xchg	eax,[edi].m7
	mov	[edi].m3,eax

	mov	eax,[edi].m6
	xchg	eax,[edi].m8
	mov	[edi].m6,eax

	pop	eax

	ret



;copy and transpose a matrix.  Takes edi=dest, esi=src. returns edi=dest
vm_copy_transpose_matrix:
	push	eax

	mov	eax,[esi].m1
	mov	[edi].m1,eax

	mov	eax,[esi].m2
	mov	[edi].m4,eax

	mov	eax,[esi].m3
	mov	[edi].m7,eax

	mov	eax,[esi].m4
	mov	[edi].m2,eax

	mov	eax,[esi].m5
	mov	[edi].m5,eax

	mov	eax,[esi].m6
	mov	[edi].m8,eax

	mov	eax,[esi].m7
	mov	[edi].m3,eax

	mov	eax,[esi].m8
	mov	[edi].m6,eax

	mov	eax,[esi].m9
	mov	[edi].m9,eax

	pop	eax

	ret



;mulitply 2 matrices, fill in dest.  returns eax=ptr to dest
;takes eax=dest, esi=src0, edi=scr1
vm_matrix_x_matrix:
	ifndef	NDEBUG
	 cmp	eax,esi
	 break_if	e,'matrix_x_matrix: dest==src0'
	 cmp	eax,edi
	 break_if	e,'matrix_x_matrix: dest==src1'
	endif

	pushm	ebx,ecx,edx,ebp

	mov	ebp,eax	;ebp=dest

;;This code would do the same as the nine lines below it, but I'm sure
;;Mike would disapprove
;;	for	s0,<<m1,m2,m3>,<m4,m5,m6>,<m7,m8,m9>>
;;	 for	s1,<<m1,m4,m7>,<m2,m5,m8>,<m3,m6,m9>>
;;	  vv_mul	@ArgI(1,s0)+@ArgI(1,s1), s0, s1
;;	 endm
;;	endm

	vv_mul	m1, m1,m2,m3, m1,m4,m7
	vv_mul	m2, m1,m2,m3, m2,m5,m8
	vv_mul	m3, m1,m2,m3, m3,m6,m9

	vv_mul	m4, m4,m5,m6, m1,m4,m7
	vv_mul	m5, m4,m5,m6, m2,m5,m8
	vv_mul	m6, m4,m5,m6, m3,m6,m9

	vv_mul	m7, m7,m8,m9, m1,m4,m7
	vv_mul	m8, m7,m8,m9, m2,m5,m8
	vv_mul	m9, m7,m8,m9, m3,m6,m9

	mov	eax,ebp	;eax=ptr to dest
	popm	ebx,ecx,edx,ebp

	ret

;computes the delta angle between two vectors
;two entry points: normalized and non-normalized vectors
;takes esi,edi=vectors, eax=optional forward vector
;returns ax=delta angle
;if the forward vector is NULL, the absolute values of the delta angle
;is returned.  If it is specified, the rotation around that vector from
;esi to edi is returned. 
vm_vec_delta_ang:	push	eax	;save forward vec

	push	ecx	;trashed by normalize
	call	vm_vec_normalize
	xchg	esi,edi
	call	vm_vec_normalize
	xchg	esi,edi	;in case forward vec specified
	pop	ecx
	jmp	do_vda_dot

vm_vec_delta_ang_norm:
	push	eax	;save forward vec
do_vda_dot:	call	vm_vec_dotprod
	call	fix_acos	;now angle in ax
	pop	ebx	;get forward vec
	or	ebx,ebx	;null?
	jz	done_vda	;..yes
;do cross product to find sign of angle
	push	edx	;save edx
	push	eax	;save angle
	push	ebx	;save forward vec
	lea	eax,tempv0	;new vec
	call	vm_vec_crossprod	;esi,edi still set
	mov	esi,eax	;new vector
	pop	edi	;forward vec
	call	vm_vec_dotprod	;eax=dotprod
	cdq		;get sign
	pop	eax	;get angle
	xor	eax,edx
	sub	eax,edx	;make sign correct
	pop	edx	;restore edx
done_vda:	ret


;compute a rotation matrix from the forward vector and a rotation around
;that vector. takes esi=vector, ax=angle, edi=matrix. returns edi=dest matrix. 
;trashes esi,eax
vm_vec_ang_2_matrix:
	pushm	eax,edx,ebx,ecx,esi

	call	fix_sincos
	mov	sinb,eax
	mov	cosb,ebx


;extract heading & pitch from vector

	mov	eax,[esi].y	;m6=-sp
	neg	eax
	mov	sinp,eax
	fixmul	eax
	sub	eax,f1_0
	neg	eax
	call	fix_sqrt	;eax=cp
	mov	cosp,eax
	mov	ebx,eax

	mov	eax,[esi].x	;sh
	fixdiv	ebx
	mov	sinh,eax

	mov	eax,[esi].z	;ch
	fixdiv	ebx
	mov	cosh,eax

	jmp	sincos_2_matrix

;compute the distance from a point to a plane.  takes the normalized normal
;of the plane (ebx), a point on the plane (edi), and the point to check (esi).
;returns distance in eax
;distance is signed, so negative dist is on the back of the plane
vm_dist_to_plane:	pushm	esi,edi

	lea	eax,tempv0
	call	vm_vec_sub	;vecs in esi,edi

	mov	esi,eax	;vector plane -> point
	mov	edi,ebx	;normal
	call	vm_vec_dotprod

	popm	esi,edi
	ret

;extract the angles from a matrix.  takes esi=matrix, fills in edi=angvec
vm_extract_angles_matrix:
	pushm	eax,ebx,edx,ecx

;extract heading & pitch from forward vector

	mov	eax,[esi].fvec.z	;ch
	mov	ebx,[esi].fvec.x	;sh

	mov	ecx,ebx	;check for z==x==0
	or	ecx,eax
	jz	zero_head	;zero, use head=0
	call	fix_atan2
zero_head:	mov	[edi].head,ax	;save heading
	
	call	fix_sincos	;get back sh

	push	eax	;save sine
	abs_eax
	mov	ecx,eax	;save abs(sine)
	mov	eax,ebx
	abs_eax		;abs(cos)
	cmp	eax,ecx	;which is larger?
	pop	eax	;get sine back
	jg	use_cos

;sine is larger, so use it
	mov	ebx,eax	;ebx=sine heading
	mov	eax,[esi].m3	;cp = shcp / sh
	jmp	get_cosp

;cosine is larger, so use it
use_cos:

	mov	eax,[esi].fvec.z	;get chcp
get_cosp:	fixdiv	ebx	;cp = chcp / ch


	push	eax	;save cp

	;eax = x (cos p)
	mov	ebx,[esi].fvec.y	;fvec.y = -sp
	neg	ebx	;ebx = y (sin)
	mov	ecx,ebx	;check for z==x==0
	or	ecx,eax
	jz	zero_pitch	;bogus vec, set p=0
	call	fix_atan2
zero_pitch:	mov	[edi].pitch,ax

	pop	ecx	;get cp
	jecxz	cp_zero

	mov	eax,[esi].m4	;m4 = sbcp
	fixdiv	ecx	;get sb
	mov	ebx,eax	;save sb

	mov	eax,[esi].m5	;get cbcp
	fixdiv	ecx	;get cb
	mov	ecx,ebx	;check for z==x==0
	or	ecx,eax
	jz	zero_bank	;bogus vec, set n=0
	call	fix_atan2
zero_bank:	mov	[edi].bank,ax

m_extract_done:
	popm	eax,ebx,edx,ecx

	ret

;the cosine of pitch is zero.  we're pitched straight up. say no bank
cp_zero:	mov	[edi].bank,0	;no bank

	popm	eax,ebx,edx,ecx
	ret


;extract the angles from a vector, assuming zero bank. 
;takes esi=vec, edi=angvec
;note versions for normalized and not normalized vector
;unnormalized version TRASHES ESI
vm_extract_angles_vector:
	push	edi
	lea	edi,tempv0
	call	vm_vec_copy_normalize	;ecx=mag
	mov	esi,edi
	pop	edi
	jecxz	extract_done

vm_extract_angles_vector_normalized:
	pushm	eax,ebx

	mov	[edi].bank,0	;always zero bank

	mov	eax,[esi].y
	neg	eax
	call	fix_asin
	mov	[edi].pitch,ax	;p = asin(-y)

	mov	eax,[esi].z
	mov	ebx,[esi].x
	or	ebx,eax
	jz	zero_head2	;check for up vector

	mov	ebx,[esi].x	;get x again
	call	fix_atan2
zero_head2:	mov	[edi].head,ax	;h = atan2(x,z) (or zero)


	popm	eax,ebx
extract_done:	ret


_TEXT	ends
	end
