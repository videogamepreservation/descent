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
; $Source: f:/miner/source/3d/rcs/3d.inc $
; $Revision: 1.30 $
; $Author: matt $
; $Date: 1995/02/09 22:00:50 $
;
; Header file for 3d library
;
; $Log: 3d.inc $
; Revision 1.30  1995/02/09  22:00:50  matt
; Removed dependence on divide overflow handler; we now check for overflow
; before dividing.  This fixed problems on some TI chips.
; 
; Revision 1.29  1994/11/11  19:22:15  matt
; Added new function, g3_calc_point_depth()
; 
; Revision 1.28  1994/09/09  14:24:13  matt
; Added support for glowing textures, to add engine glow to Descent.
; 
; Revision 1.27  1994/09/01  10:42:26  matt
; Blob routine, renamed g3_draw_bitmap(), now takes seperate 3d width & height.
; 
; Revision 1.26  1994/07/29  18:15:53  matt
; Added instance by angles, and corrected parms for g3_init()
; 
; Revision 1.25  1994/07/25  00:00:01  matt
; Made 3d no longer deal with point numbers, but only with pointers.
; 
; Revision 1.24  1994/07/22  17:57:57  matt
; Changed the name of the rod functions, and took out some debugging code
; 
; Revision 1.23  1994/06/07  16:49:35  matt
; Made interpreter take lighting value as parm, rather than in global var
; 
; Revision 1.22  1994/05/31  18:35:19  matt
; Added light value to g3_draw_facing_bitmap()
; 
; Revision 1.21  1994/05/30  22:48:15  matt
; Added support for morph effect
; 
; Revision 1.20  1994/05/30  11:36:25  matt
; Added g3_set_special_render() to allow a user to specify functions to
; call for 2d draws.
; 
; Revision 1.19  1994/05/19  21:46:30  matt
; Moved texture lighting out of 3d and into the game
; 
; Revision 1.18  1994/04/19  18:26:41  matt
; Added g3_draw_sphere() function.
; 
; Revision 1.17  1994/03/25  18:22:39  matt
; g3_draw_polygon_model() now takes ptr to list of angles
; 
; Revision 1.16  1994/03/15  21:23:14  matt
; Added interpreter functions
; 
; Revision 1.15  1994/02/15  17:37:35  matt
; New function, g3_draw_blob()
; 
; Revision 1.14  1994/02/10  18:00:10  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.13  1994/02/09  11:47:34  matt
; Added rod & delta point functions
; 
; Revision 1.12  1994/01/26  12:38:20  matt
; Added function g3_compute_lighting_value()
; 
; Revision 1.11  1994/01/24  14:08:24  matt
; Added instancing functions
; 
; Revision 1.10  1994/01/22  18:22:07  matt
; New lighting stuff now done in 3d; g3_draw_tmap() takes lighting parm
; 
; Revision 1.9  1994/01/20  17:21:46  matt
; New function g3_compute_sky_polygon()
; 
; Revision 1.8  1994/01/14  17:21:02  matt
; Added new function g3_draw_horizon()
; 
; Revision 1.6  1993/12/20  20:21:43  matt
; Added g3_point_2_vec()
; 
; Revision 1.5  1993/12/05  23:47:13  matt
; Added function g3_draw_line_ptrs()
; 
; Revision 1.4  1993/11/21  20:08:16  matt
; Added functions g3_rotate_point & g3_draw_object
; 
; Revision 1.3  1993/11/04  18:49:42  matt
; Made g3_init() have functionality of g3_set_points(), which I killed
; 
; Revision 1.2  1993/11/04  12:38:29  mike
; Add PF_LVS, flag for lighting value
; 
; Revision 1.1  1993/10/29  22:20:27  matt
; Initial revision
; 
;
;

ifndef _3D_INC
_3D_INC equ 1

MAX_POINTS_IN_POLY = 100

	include	fix.inc
	include	vecmat.inc

;Structures

;Stucture to store clipping codes in a word
g3s_codes	STRUCT 2t
cc_or	ubyte	?	;or is low byte
cc_and	ubyte	?	;and is high byte
g3s_codes	ENDS

;Used to store rotated points for mines.  Has frame count to indictate
;if rotated, and flag to indicate if projected.
g3s_point	struct
	union
p3_vec	 vms_vector <>
	 struct
x	  fix	?
y	  fix	?
z	  fix	?
	 ends
xyz	 fix	3t DUP (?)
	ends	;end of union
p3_u	fix	?	;u & v for this point
p3_v	fix	?
p3_l	fix	?
p3_sx	fix	?	;screen x & y
p3_sy	fix	?
p3_codes	db	?	;clipping codes
p3_flags	db	?	;projected?
p3_pad	dw	?	;keep longword aligned
g3s_point	ends

;Flags

;for g3_flags field of g3s_point
PF_PROJECTED	equ	1	;has been projected
PF_OVERFLOW	equ	2	;can't project
PF_TEMP_POINT	equ	4	;created during clip
PF_UVS	equ	8	;has uv values set
PF_LVS	equ	16	;has lighting values set

;for clipping codes
CC_OFF_LEFT	equ	1
CC_OFF_RIGHT	equ	2
CC_OFF_BOT	equ	4
CC_OFF_TOP	equ	8
CC_BEHIND	equ	80h

_DATA	segment	dword public USE32 'DATA'

;Global variables

	extdef	vms_vector,View_position,Matrix_scale,Window_scale
	extdef	vms_matrix,View_matrix,Unscaled_matrix
	extf	View_zoom

	extd	Vbuf0,Vbuf1,Num_points

	extd	Canvas_width,Canvas_height	;the actual width & height
	extf	Canv_w2,Canv_h2			;fixed-point width,height/2

	extf	Vertex_list

	;;extw	_Frame_count

_DATA	ends

_TEXT	segment	dword public USE32 'CODE'

;Routines

;from setup.asm
	extn	g3_init	;eax=point list, edx=vec list
	extn	g3_close	;takes nothing, ret nothing
	extn	g3_start_frame	;takes nothing, ret nothing
	extn	g3_end_frame	;takes nothing, ret nothing

;from matrix.asm
	extn	g3_set_view_angles	;esi=angvec,esi=pos,eax=zoom
	extn	g3_set_view_matrix	;esi=matrix,edi=pos,eax=zoom

;from points.asm
	;;extn	g3_set_points	;eax=point list, edx=vec list
	extn	g3_rotate_point	;edi=dest point, esi=src vec
	extn	g3_project_point	;esi=point
	extn	g3_point_2_vec	;esi=vec, eax,ebx = 2d point (shorts)
	extn	g3_code_point	;eax=point, fill in code, ret bl=code

	extn	g3_rotate_delta_x	;edi=dest vec, ebx=delta
	extn	g3_rotate_delta_y	;edi=dest vec, ebx=delta
	extn	g3_rotate_delta_z	;edi=dest vec, ebx=delta
	extn	g3_rotate_delta_vec	;edi=dest vec, esi=delta vec
	extn	g3_add_delta_vec	;eax=dest pnt, esi=src pnt, edi=delta vec. ret bl=codes

	extn	g3_calc_point_depth	;esi=point, ret eax=depth

	extn	proj_div0,proj_div1,div_overflow_handler,code_point
	extn	blob_div0,blob_div1,blob_overflow_handler
	extn	sphere_proj_div,sphere_div_overflow_handler

;from draw.asm
	extn	g3_draw_line	;esi,edi=points
	extn	g3_draw_poly	;ecx=cnt, esi=point nums, al=ret: 1 off screen, 0 drew
	extn	g3_check_and_draw_poly	;ecx=cnt, esi=point nums, edi=normal, al=ret: -1 not facing, 1 off screen, 0 drew
	extn	g3_draw_tmap	;ecx=cnt, esi=point nums, ebx=uvs, edx=bitmap, al=ret: 1 off screen, 0 drew
	extn	g3_check_and_draw_tmap	;ecx=cnt, esi=point nums, ebx=uvs, edx=bitmap, edi=normal, al=ret: -1 not facing, 1 off screen, 0 drew
	extn	g3_check_normal_facing	;esi=vec, edi=norm, ret al
	;;extn	g3_compute_lighting_value	;ecx=surface normal scale, esi=rotated point, ret ecx
	extn	g3_draw_sphere	;esi=pos (rotated), ecx=radius
	extn	g3_set_special_render ;eax=tmap draw, edx=flat drawer, ebx=line drawer

;from clipper.asm
	extn	clip_line	;esi,edi=points (ptrs)
	extn	free_temp_point	;esi=point (ptr)
	extn	clip_polygon	;esi=src list,edi=dest list,ecx=n, al=codes_or

;from objects.asm
	extn	g3_draw_object	;esi=pos,edi=mat,eax=size,ebx=obj_id


;from horizon.asm
	extn	g3_draw_horizon	;eax=sky_color, edx=ground_color
	extn	g3_compute_sky_polygon	;ebx=2d points, ecx=vecs

;from instance.asm
	extn	g3_start_instance_matrix	;esi=pos,edi=matrix. trashes esi,edi
	extn	g3_start_instance_angles	;esi=pos,edi=angvec. trashes esi,edi

	extn	g3_done_instance	;no parms

;from rod.asm
	extn	g3_draw_rod_tmap	;esi,edi=bot,top points,eax,edx=bot,top width, ebx=bitmap, ecx=lightval
	extn	g3_draw_rod_flat	;esi,edi=bot,top points,eax,edx=bot,top width
	extn	g3_draw_bitmap	;esi=pos,ebx=width3d,ecx=height3d,eax=bm, trashes eax,ecx,esi

;from interp.asm

	extn	g3_set_interp_points	;eax=ptr to list
	extn	g3_draw_polygon_model	;esi=model ptr, edi=bitmap list ptr, eax=anim angles, edx=light, ebx=ptr to glow values (or NULL)
	extn	g3_init_polygon_model	;esi=model ptr
	extn	g3_draw_morphing_model	;esi=model ptr, edi=bitmap list ptr, eax=anim angles, edx=light, ebx=new points

_TEXT	ends



;check for overflow before doing divide.
;assume numerator is in edx:eax.  Denominator and label to jump to if
;overflow are passed.
divcheck	macro	den,lbl
	local	not_neg1,not_neg2
	push	ecx
	mov	ecx,edx
	or	ecx,ecx
	jns	not_neg1
	neg	ecx
not_neg1:	shld	ecx,eax,1	;num * 2
	cmp	ecx,den
	pop	ecx
	jae	lbl
	endm


endif

