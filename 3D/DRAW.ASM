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
; $Source: f:/miner/source/3d/rcs/draw.asm $
; $Revision: 1.30 $
; $Author: matt $
; $Date: 1995/02/15 02:26:52 $
;
; Source for drawing routines
;
; $Log: draw.asm $
; Revision 1.30  1995/02/15  02:26:52  matt
; Put in added handling for odd clipping error
; 
; Revision 1.29  1995/02/10  16:41:41  matt
; Put in check for bad points after clip
; 
; Revision 1.28  1995/02/09  22:00:52  matt
; Removed dependence on divide overflow handler; we now check for overflow
; before dividing.  This fixed problems on some TI chips.
; 
; Revision 1.27  1994/11/30  00:59:32  mike
; optimizations.
; 
; Revision 1.26  1994/10/03  12:52:04  matt
; Fixed typo
; 
; Revision 1.25  1994/10/03  12:49:56  matt
; Took out unused routines & data
; 
; Revision 1.24  1994/07/24  23:59:34  matt
; Made 3d no longer deal with point numbers, but only with pointers.
; 
; Revision 1.23  1994/05/30  11:36:26  matt
; Added g3_set_special_render() to allow a user to specify functions to
; call for 2d draws.
; 
; Revision 1.22  1994/05/19  21:46:13  matt
; Moved texture lighting out of 3d and into the game
; 
; Revision 1.21  1994/05/13  17:06:18  matt
; Finished ripping out intersected side lighting code
; 
; Revision 1.20  1994/05/13  17:02:58  matt
; Took our special side lighting code
; 
; Revision 1.19  1994/04/19  18:45:19  matt
; Now call the clipped disk, which it should have done all along, but
; John had only implemented the unclipped disk, and it clipped anyway.
; 
; Revision 1.18  1994/04/19  18:26:42  matt
; Added g3_draw_sphere() function.
; 
; Revision 1.17  1994/04/19  17:03:40  matt
; For polygons, call the texture-mapper's flat shader
; 
; Revision 1.16  1994/03/18  15:58:37  matt
; Fixed bug that caused light vals to be screwed up
; 
; Revision 1.15  1994/03/14  12:37:31  matt
; Made draw routines check for rotated points
; 
; Revision 1.14  1994/02/10  18:00:39  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.13  1994/02/09  11:48:16  matt
; Changes return codes for drawing functions
; 
; Revision 1.12  1994/02/01  13:23:05  matt
; Added use_beam var to turn off beam lighting
; 
; Revision 1.11  1994/01/26  20:27:18  mike
; bright light on very near side.
; 
; Revision 1.10  1994/01/26  12:49:26  matt
; Made lighting computation a seperate function, g3_compute_lighting_value.
; Note the unwieldy function name, courtesy of Mike.
; 
; Revision 1.9  1994/01/25  16:38:02  yuan
; Fixed beam_brightness
; 
; Revision 1.8  1994/01/24  11:08:49  matt
; Added beam_brightness variable
; 
; Revision 1.7  1994/01/22  18:22:09  matt
; New lighting stuff now done in 3d; g3_draw_tmap() takes lighting parm
; 
; Revision 1.6  1993/12/05  23:47:14  matt
; Added function g3_draw_line_ptrs()
; 
; Revision 1.5  1993/11/22  10:51:29  matt
; Changed texture map function comments to reflect uvl (not just uv) struct
; 
; Revision 1.4  1993/11/17  10:40:02  matt
; Added check for zero-length normal in do_facing_check
; 
; Revision 1.3  1993/11/04  18:49:14  matt
; Added system to only rotate points once per frame
; 
; Revision 1.2  1993/11/04  12:36:36  mike
; Add support for static lighting value.
; 
; Revision 1.1  1993/10/29  22:20:27  matt
; Initial revision
; 
;
;

DONT_USE_UPOLY = 1

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

rcsid	db	"$Id: draw.asm 1.30 1995/02/15 02:26:52 matt Exp $"
	align	4

tempv	vms_vector <>

n_verts	dd	?
n_verts_4	dd	?	; added by mk, 11/29/94, point coding optimization.
bitmap_ptr	dd	?

uvl_list_ptr	dd	?

tmap_drawer_ptr	dd	draw_tmap_
flat_drawer_ptr	dd	gr_upoly_tmap_
line_drawer_ptr	dd	gr_line_

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

	extn	gr_upoly_tmap_
	extn	draw_tmap_

;specifies new routines to call to draw polygons
;takes eax=ptr to tmap routine, edx=ptr to flat routine, ebx=line rtn
g3_set_special_render:
	or	eax,eax
	jnz	got_tmap_ptr
	lea	eax,cs:draw_tmap_
got_tmap_ptr:	mov	tmap_drawer_ptr,eax

	or	edx,edx
	jnz	got_flat_ptr
	lea	edx,cs:gr_upoly_tmap_
got_flat_ptr:	mov	flat_drawer_ptr,edx

	or	ebx,ebx
	jnz	got_line_ptr
	lea	ebx,cs:gr_line_
got_line_ptr:	mov	line_drawer_ptr,ebx

	ret


;alternate entry takes pointers to points. esi,edi = point pointers
g3_draw_line:
	;;ifndef	NDEBUG
	;; mov	ax,_Frame_count	;curren frame
	;; cmp	ax,[esi].p3_frame	;rotated this frame?
	;; break_if	ne,'Point not rotated in draw_line'
	;; cmp	ax,[edi].p3_frame	;rotated this frame?
	;; break_if	ne,'Point not rotated in draw_line'
	;;endif

	mov	al,[esi].p3_codes
	mov	ah,[edi].p3_codes

;check codes for reject, clip, or no clip

	test	al,ah	;check codes_and
	jnz	no_draw_line	;both off same side

	or	al,ah	;al=codes_or
	js	must_clip_line	;neg z means must clip
	
	test	[esi].p3_flags,PF_PROJECTED
	jnz	p0_projected
	call	g3_project_point
p0_projected:	test	[esi].p3_flags,PF_OVERFLOW
	jnz	must_clip_line

	test	[edi].p3_flags,PF_PROJECTED
	jnz	p1_projected
	xchg	esi,edi	;get point in esi
	call	g3_project_point
	xchg	esi,edi
p1_projected:	test	[edi].p3_flags,PF_OVERFLOW
	jnz	must_clip_line

	pushm	ebx,ecx,edx	;save regs

	test	al,al	;check codes or

	mov	eax,[esi].p3_sx
	mov	edx,[esi].p3_sy

	mov	ebx,[edi].p3_sx
	mov	ecx,[edi].p3_sy

	jz	unclipped_line	;cc set from test above

;call clipping line drawer
;;	call	gr_line_	;takes eax,edx,ebx,ecx
 push esi
 mov esi,line_drawer_ptr
 call esi
 pop esi
	popm	ebx,ecx,edx
	ret	;return value from gr_line

;we know this one is on screen
unclipped_line:	;;call	gr_uline_	;takes eax,edx,ebx,ecx
;; call gr_line_
 push esi
 mov esi,line_drawer_ptr
 call esi
 pop esi
	popm	ebx,ecx,edx
	mov	al,1	;definitely drew
	ret

;both points off same side, no do draw
no_draw_line:	xor	al,al	;not drawn
	ret

;jumped here from out of g3_draw_line. esi,edi=points, al=codes_or
must_clip_line:	call	clip_line	;do the 3d clip
	call	g3_draw_line	;try draw again

;free up temp points
	test	[esi].p3_flags,PF_TEMP_POINT
	jz	not_temp_esi
	call	free_temp_point
not_temp_esi:	test	[edi].p3_flags,PF_TEMP_POINT
	jz	not_temp_edi
	xchg	esi,edi
	call	free_temp_point
not_temp_edi:
	ret	;ret code set from g3_draw_line


;returns true if a plane is facing the viewer. takes the unrotated surface 
;normal of the plane, and a point on it.  The normal need not be normalized
;takes esi=vector (unrotated point), edi=normal. 
;returns al=true if facing, cc=g if facing
;trashes esi,edi
g3_check_normal_facing:
	push	edi	;save normal
	lea	eax,tempv
	mov	edi,esi
	lea	esi,View_position
	call	vm_vec_sub

	mov	esi,eax	;view vector
	pop	edi	;normal
	call	vm_vec_dotprod

	or	eax,eax	;check sign
	setg	al	;true if positive

	ret

;see if face is visible and draw if it is.
;takes ecx=nv, esi=point list, edi=normal, ebx=point.
;normal can be NULL, which will for compution here (which will be slow).
;returns al=-1 if not facing, else 0
;Trashes ecx,esi,edx,edi
g3_check_and_draw_poly:
	call	do_facing_check
	or	al,al
	jnz	g3_draw_poly
	mov	al,-1
	ret

g3_check_and_draw_tmap:
	push	ebx
	mov	ebx,eax
	call	do_facing_check
	pop	ebx
	or	al,al
	jnz	g3_draw_tmap
	mov	al,-1
	ret


;takes edi=normal or NULL, esi=list of vert nums, ebx=pnt
;returns al=facing?
do_facing_check:
	test	edi,edi	;normal passed?
	jz	compute_normal	;..nope

;for debug, check for NULL normal
	ifndef	NDEBUG
	 mov	eax,[edi].x
	 or	eax,[edi].y
	 or	eax,[edi].z
	 break_if	z,'Zero-length normal in do_facing_check'
	endif

;we have the normal. check if facing
	push	esi
	mov	esi,ebx
	call	g3_check_normal_facing	;edi=normal
	pop	esi
	setg	al	;set al true if facing
	ret

;normal not specified, so must compute
compute_normal:
	pushm	ebx,ecx,edx,esi

;get three points (rotated) and compute normal

	mov	eax,[esi]	;get point
	mov	edi,8[esi]	;get point
	mov	esi,4[esi]	;get point

	lea	ebx,tempv
	call	vm_vec_perp	;get normal
	mov	edi,eax	;normal in edi
	call	vm_vec_dotprod	;point in esi

	or	eax,eax	;check result
	popm	ebx,ecx,edx,esi
	sets	al	;al=true if facing
	ret


;draw a flat-shaded face.  returns true if drew
;takes ecx=nv, esi=list of pointers to points
;returns al=0 if called 2d, 1 if all points off screen  
;Trashes ecx,esi,edx
g3_draw_poly:
	pushm	ebx,edi

	lea	edi,Vbuf0	;list of ptrs
	xor	ebx,ebx	;counter
	mov	dx,0ff00h	;init codes

codes_loop:	mov	eax,[esi+ebx*4]	;get point number
	mov	[edi+ebx*4],eax	;store in ptr array

	;;ifndef	NDEBUG
	;; push	ebx
	;; mov	bx,_Frame_count	;curren frame
	;; cmp	bx,[eax].p3_frame	;rotated this frame?
	;; break_if	ne,'Point not rotated in draw_poly'
	;; pop	ebx
	;;endif

	and	dh,[eax].p3_codes	;update codes_and
	or	dl,[eax].p3_codes	;update codes_or

	inc	ebx
	cmp	ebx,ecx	;done?
	jne	codes_loop	;..nope

;now dx = codes
	test	dh,dh	;check codes_and
	jnz	face_off_screen	;not visible at all

	test	dl,dl	;how about codes_or
	js	must_clip_face	;neg z means must clip
 jnz must_clip_face

;reentry point for jump back from clipper
draw_poly_reentry:
	push	edx	;save codes_or

;now make list of 2d coords (& check for overflow)

	mov	edx,edi	;edx=Vbuf0
	xor	ebx,ebx
	lea	edi,Vertex_list

coords_loop:	mov	esi,[edx+ebx*4]	;esi = point
	test	[esi].p3_flags,PF_PROJECTED
	jnz	pnt_projected
	call	g3_project_point
pnt_projected:	test	[esi].p3_flags,PF_OVERFLOW
	jnz	must_clip_face2
	mov	eax,[esi].p3_sx
	mov	[edi+ebx*8],eax
	mov	eax,[esi].p3_sy
	mov	4[edi+ebx*8],eax

	inc	ebx
	cmp	ebx,ecx
	jne	coords_loop

	mov	eax,ecx	;eax=nverts
	mov	edx,edi	;edx=vertslist

;check for trivial accept
	pop	ebx	;get codes_or
ife DONT_USE_UPOLY
	test	bl,bl
	jz	no_clip_face
endif
	;;call	gr_poly_	;takes eax,edx
;;; call gr_upoly_tmap_
 push ecx
 mov ecx,flat_drawer_ptr
 call ecx
 pop ecx

	popm	ebx,edi
	xor	al,al	;say it drew
	ret

;all on screen.  call non-clipping version
no_clip_face:	;;call	gr_upoly_	;takes eax,edx
;; call gr_poly_
 call gr_upoly_tmap_
	popm	ebx,edi
	xor	al,al	;say it drew
	ret

;all the way off screen. return
face_off_screen:	popm	ebx,edi
	mov	al,1	;no draw
	ret

;we require a 3d clip
must_clip_face2:	pop	edx	;get codes back
must_clip_face:	lea	esi,Vbuf0	;src
	lea	edi,Vbuf1	;dest
	mov	al,dl	;codes in al
	call	clip_polygon	;count in ecx

	mov	edi,esi	;new list in edi

;clipped away?
	jecxz	clipped_away
	or	dh,dh	;check codes_and
	jnz	clipped_away
 test dl,dl	;check codes or
 js clipped_away	;some points still behind eye

	push	edi	;need edi to free temp pnts
	push	offset cs:reentry_return
	pushm	ebx,edi	;match what draw has pushed
	jmp	draw_poly_reentry

reentry_return:
	pop	edi
clipped_away:	
	push	eax	;save ret codes
;free temp points
	xor	ebx,ebx
free_loop:	mov	esi,[edi+ebx*4]	;get point
	test	[esi].p3_flags,PF_TEMP_POINT
	jz	not_temp
	call	free_temp_point
not_temp:	inc	ebx
	cmp	ebx,ecx
	jne	free_loop

	pop	eax	;get ret codes back

	popm	ebx,edi
	ret


;draw a texture-mapped face.  returns true if drew
;takes ecx=nv, esi=point list, ebx=uvl_list, edx=bitmap
;returns al=0 if called 2d, 1 if all points off screen  
;Trashes ecx,esi,edx
g3_draw_tmap:
	mov	bitmap_ptr,edx	;save

	pushm	ebx,edi

	lea	edi,Vbuf0	;list of ptrs
	push	ebp	;save ebp
	mov	ebp,ebx	;ebp=uvl list
 mov uvl_list_ptr, ebx
	mov	n_verts,ecx	;save in memory
	shl	ecx, 2

;loop to check codes, make list of point ptrs, and copy uvl's into points
	mov	dh, 0ffh	; init codes (this pipelines nicely)
	mov	n_verts_4, ecx
	xor	ebx, ebx
	xor	dl, dl	; init codes (this pipelines nicely)

t_codes_loop:	mov	eax,[esi+ebx]	;get point number
	mov	[edi+ebx],eax	;store in ptr array

	and	dh,[eax].p3_codes	;update codes_and
	or	dl,[eax].p3_codes	;update codes_or

	mov	ecx,[ebp]	;get u
	add	ebp, 4	; (this pipelines better ..mk, 11/29/94 ((my 33rd birthday...)))
	mov	[eax].p3_u,ecx
	mov	ecx,[ebp]	;get v
	add	ebp, 4
	mov	[eax].p3_v,ecx
	mov	ecx,[ebp]	;get l
	add	ebp, 4
	mov	[eax].p3_l,ecx
	or	[eax].p3_flags,PF_UVS + PF_LVS	;this point's got em

	add	ebx,4
	cmp	ebx, n_verts_4
	jne	t_codes_loop	;..nope

	pop	ebp	;restore ebp
;now dx = codes
	test	dh,dh	;check codes_and
	jnz	t_face_off_screen	;not visible at all

	test	dl,dl	;how about codes_or
	jnz	t_must_clip_face	;non-zero means must clip

;reentry point for jump back from clipper
t_draw_poly_reentry:

;make sure all points projected
	mov	edx,edi	;edx=Vbuf0
	xor	ebx,ebx

t_proj_loop:	mov	esi,[edx+ebx*4]	;esi = point

	;;ifndef	NDEBUG
	;; mov	ax,_Frame_count	;curren frame
	;; cmp	ax,[esi].p3_frame	;rotated this frame?
	;; break_if	ne,'Point not rotated in draw_tmap'
	;;endif

	test	[esi].p3_flags,PF_PROJECTED
	jnz	t_pnt_projected
	call	g3_project_point
t_pnt_projected:
	test	[esi].p3_flags,PF_OVERFLOW
	break_if	nz,'Should not overflow after clip'
	jnz	t_face_off_screen

	inc	ebx
	cmp	ebx,n_verts
	jne	t_proj_loop

;now call the texture mapper
	mov	eax,bitmap_ptr	;eax=bitmap ptr
	mov	edx,n_verts	;edx=count
	mov	ebx,edi	;ebx=points
;;;	call	draw_tmap_
 push ecx
 mov ecx,tmap_drawer_ptr
 call ecx
 pop ecx
	popm	ebx,edi
	xor	al,al	;say it drew
	ret

;all the way off screen. return
t_face_off_screen:	popm	ebx,edi
	mov	al,1	;no draw
	ret

;we require a 3d clip
t_must_clip_face:	lea	esi,Vbuf0	;src
	lea	edi,Vbuf1	;dest
	mov	al,dl	;codes in al
	mov	ecx,n_verts
	call	clip_polygon	;count in ecx
	mov	n_verts,ecx

	mov	edi,esi	;new list in edi

;clipped away?
	jecxz	t_clipped_away
	or	dh,dh	;check codes_and
	jnz	t_clipped_away
 test dl,dl	;check codes or
 js t_clipped_away	;some points still behind eye

	push	edi	;need edi to free temp pnts
	push	offset cs:t_reentry_return
	pushm	ebx,edi	;match what draw has pushed
	jmp	t_draw_poly_reentry

t_reentry_return:
	pop	edi
t_clipped_away:	
; free temp points
	mov	ebx, ecx
	push	eax	;save ret code
	dec	ebx
	shl	ebx, 2

t_free_loop:	mov	esi,[edi+ebx]	;get point
	test	[esi].p3_flags,PF_TEMP_POINT
	jz	t_not_temp
	call	free_temp_point
t_not_temp:	sub	ebx, 4
	jns	t_free_loop

	pop	eax	;get ret code

	popm	ebx,edi
	ret



;draw a sortof-sphere. takes esi=point, ecx=radius
g3_draw_sphere:	test	[esi].p3_codes,CC_BEHIND
	jnz	reject_sphere

	test	[esi].p3_flags,PF_PROJECTED
	jnz	sphere_p_projected
	call	g3_project_point
sphere_p_projected:
	test	[esi].p3_flags,PF_OVERFLOW
	jnz	reject_sphere


;calc radius.  since disk doesn't take width & height, let's just
;say the the radius is the width

	pushm	eax,ebx,ecx,edx

	mov	eax,ecx
	fixmul	Matrix_scale.x
	imul	Canv_w2
sphere_proj_div:	divcheck	[esi].z,sphere_div_overflow_handler
	idiv	[esi].z

	mov	ebx,eax

	mov	eax,[esi].p3_sx
	mov	edx,[esi].p3_sy
	call	gr_disk_

sphere_div_overflow_handler:

	popm	eax,ebx,ecx,edx


reject_sphere:	ret


_TEXT	ends

	end
