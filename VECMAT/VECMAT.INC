;
; $Source: f:/miner/source/vecmat/rcs/vecmat.inc $
; $Revision: 1.21 $
; $Author: matt $
; $Date: 1994/12/13 14:44:21 $
;
; Header file for vector/matrix library
;
; $Log: vecmat.inc $
; Revision 1.21  1994/12/13  14:44:21  matt
; Added vm_vector_2_matrix_norm()
; 
; Revision 1.20  1994/09/11  19:23:04  matt
; Added vm_vec_normalized_dir_quick()
; 
; Revision 1.19  1994/07/19  18:52:55  matt
; Added vm_vec_normalize_quick() and vm_vec_copy_normalize_quick()
; 
; Revision 1.18  1994/06/16  18:24:31  matt
; Added vm_vec_mag_quick()
; 
; Revision 1.17  1994/05/19  12:07:20  matt
; Fixed globals and macros and added a constant
; 
; Revision 1.16  1994/05/18  22:28:55  matt
; Added function vm_vec_normalized_dir()
; Added C macros IS_ZERO_VEC(), vm_vec_zero(), and vm_set_identity()
; Added C global static vars vmd_zero_vector & vmd_identity_matrix
; 
; Revision 1.15  1994/05/18  21:45:05  matt
; Added functions:
;   vm_extract_angles_vector()
;   vm_extract_angles_vector_normalized()
;   vm_vec_copy_normalize()
; 
; Revision 1.14  1994/05/13  12:42:09  matt
; Added new function, vm_vec_dist_quick(), which does an approximation.
; 
; Revision 1.13  1994/03/30  15:43:54  matt
; Added two functions, vm_vec_scale_add() & vm_vec_scale_add2()
; 
; Revision 1.12  1994/01/31  19:45:24  matt
; Added function vm_extract_angles_matrix()
; 
; Revision 1.11  1993/12/21  19:46:29  matt
; Added function vm_dist_to_plane()
; 
; Revision 1.10  1993/12/13  17:26:40  matt
; Added vm_vec_dist()
; 
; Revision 1.9  1993/12/02  12:44:04  matt
; New functions: vm_vec_copy_scale(), vm_vec_scale2()
; 
; Revision 1.8  1993/10/29  22:39:08  matt
; Changed matrix order, making direction vectors the rows
; 
; Revision 1.7  1993/10/25  11:49:58  matt
; Made vm_vec_delta_ang() take optional forward vector to return signed delta
; 
; Revision 1.6  1993/10/20  01:10:04  matt
; Added vm_vec_delta_ang(), vm_vec_delta_ang_norm(), and vm_vec_ang_2_matrix()
; 
; Revision 1.5  1993/09/28  12:16:04  matt
; Added func vm_vector_2_matrix()
; 
; Revision 1.4  1993/09/24  21:19:14  matt
; Added vm_vec_avg() and vm_vec_avg4()
; 
; Revision 1.3  1993/09/20  14:56:35  matt
; Added new function, vm_vec_perp()
; 
; Revision 1.2  1993/09/17  11:09:57  matt
; Added vm_vec_add2() and vm_vec_sub2(), which take 2 args (dest==src0)
; 
; Revision 1.1  1993/09/16  20:19:29  matt
; Initial revision
; 
;
;

ifndef _VECMAT_INC
_VECMAT_INC equ 1

	include	fix.inc

;Structures

vms_vector	struct
	union
	 struct
x	  fix	?
y	  fix	?
z	  fix	?
	 ends
xyz	 fix	3 dup (?)
	ends
vms_vector	ends


vms_svec	struct
	union
	 struct
sv_x	  dw	?
sv_y	  dw	?
sv_z	  dw	?
	 ends
sv_xyz	 dw	3 dup (?)
	ends
vms_svec	ends

vms_angvec	struct
	union
	struct
pitch	 fixang	?
bank	 fixang	?
head	 fixang	?
	ends
	struct
;p	 fixang	?
;b	 fixang	?
;h	 fixang	?
	ends
	ends
vms_angvec	ends

vms_matrix	struct
	union
	 struct
m1	  fix	?
m4	  fix	?
m7	  fix	?
m2	  fix	?
m5	  fix	?
m8	  fix	?
m3	  fix	?
m6	  fix	?
m9	  fix	?
	 ends
	 struct
rvec	  fix	?,?,?
uvec	  fix	?,?,?
fvec	  fix	?,?,?
	 ends
;;mm	 fix	9 dup (?)
	ends
vms_matrix	ends


;Macros

;copies one vector to another, using the register specified.  If none
;specified, uses eax
vm_copy	macro	dest,src,reg:=<eax>
	for	ofs,<x,y,z>
	 mov	reg,[src].ofs
	 mov	[dest].ofs,reg
	endm
	endm

;copies one angvec to another, using the register specified.  If none
;specified, uses eax (and ax). Note the trick to get the word part of
;the register without knowing what the register is.
vm_acopy	macro	dest,src,reg:=<eax>
	mov	reg,fix ptr [src].p	;copy two at once
	mov	fix ptr [dest].p,reg
	db	66h	;size override, use short
	mov	reg,fix ptr [src].h ;copy last angle
	db	66h	;size override, use short
	mov	fix ptr [dest].h,reg
	endm

;Global contants
	extdef	vms_vector,_vmd_zero_vector
	extdef	vms_matrix,_vmd_identity_matrix

;Routines

;register usage appears here, but see VECMAT.H for other info

	extn	vm_vec_add	;eax=dest, esi,edi=srcs
	extn	vm_vec_sub	;eax=dest, esi,edi=srcs
	extn	vm_vec_add2	;edi=dest, esi=source
	extn	vm_vec_sub2	;edi=dest, esi=source
	extn	vm_vec_avg	;eax=dest, esi,edi=srcs
	extn	vm_vec_avg4	;eax=dest, esi,edi,ecx,edx=srcs
	extn	vm_vec_scale	;ebx=vec, ecx=scale
	extn	vm_vec_copy_scale	;edi=dest, ebx=src, ecx=scale
	extn	vm_vec_scale2	;edi=vec, ebx=n,edx=d
	extn	vm_vec_mag	;esi=vec, returns eax=mag
	extn	vm_vec_dist	;esi,edi=vecs, returns eax=dist
	extn	vm_vec_mag_quick	;esi=vec, returns eax=approx dist
	extn	vm_vec_dist_quick	;esi,edi=vecs, returns eax=approx dist
	extn	vm_vec_normalize	;esi=vec, returns ecx=mag
	extn	vm_vec_normalize_quick	;esi=vec, returns ecx=mag
	extn	vm_vec_copy_normalize	;edi=dest, esi=src
	extn	vm_vec_copy_normalize_quick	;edi=dest, esi=src
	extn	vm_vec_normalized_dir	;edi=dest, esi=endpoint, ebx=startpoint
	extn	vm_vec_normalized_dir_quick	;edi=dest, esi=endpoint, ebx=startpoint
	extn	vm_vec_dotprod	;esi,edi=vecs, ret eax=dotprod
	extn	vm_vec_crossprod	;eax=dest, esi,edi=srcs
	extn	vm_vec_normal	;ebx=dest, eax,esi,edi=srcs
	extn	vm_vec_perp	;ebx=dest, eax,esi,edi=srcs
	extn	vm_vec_delta_ang	;esi,edi=vecs, eax=(optional) fvec, ret ax=angle
	extn	vm_vec_delta_ang_norm ;esi,edi=vec, ret ax=angle
	extn	vm_angles_2_matrix	;edi=dest, esi=angvec
	extn	vm_vector_2_matrix	;edi=dest, esi=fwdvec, eax=upvec, ebx=rightvec
	extn	vm_vector_2_matrix_norm	;edi=dest, esi=fwdvec, eax=upvec, ebx=rightvec
	extn	vm_vec_rotate	;eax=dest, esi=src, edi=matrix
	extn	vm_transpose_matrix	;edi=matrix (transpose in place)
	extn	vm_copy_transpose_matrix ;edi=dest, esi=src
	extn	vm_matrix_x_matrix	;eax=dest, esi,edi=srcs
	extn	vm_vec_ang_2_matrix	;esi=vector, eax=angle, edi=matrix
	extn	vm_dist_to_plane	;ebx=norm, edi=plane pnt, esi=check pnt, ret eax=dist
	extn	vm_extract_angles_matrix	;edi=angles, esi=matrix
	extn	vm_vec_scale_add	;edi=dest, ebx=src1, esi=src2, ecx=scale
	extn	vm_vec_scale_add2	;edi=dest, esi=src, ecx=scale
	extn	vm_extract_angles_vector	;edi=angvec, esi=vec TRASHES ESI
	extn	vm_extract_angles_vector_normalized	;edi=angvec, esi=vec

endif

