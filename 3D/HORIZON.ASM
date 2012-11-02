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
; $Source: f:/miner/source/3d/rcs/horizon.asm $
; $Revision: 1.5 $
; $Author: matt $
; $Date: 1994/06/14 18:14:40 $
;
; Source for horizon drawer
;
; $Log: horizon.asm $
; Revision 1.5  1994/06/14  18:14:40  matt
; Use texture-mapper flat shader
; 
; Revision 1.4  1994/02/10  18:00:42  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.3  1994/01/22  18:22:16  matt
; Fixed bug in horizon bitmap code
; 
; Revision 1.2  1994/01/20  17:21:47  matt
; New function g3_compute_sky_polygon()
; 
; Revision 1.1  1994/01/14  17:22:08  matt
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

rcsid	db	"$Id: horizon.asm 1.5 1994/06/14 18:14:40 matt Exp $"
	align	4

sky_color	dd	?
ground_color	dd	?
top_color	dd	?
bot_color	dd	?

color_swap	dw	?	;flag for if we swapped
sky_ground_flag	dw	?	;0=both, 1=all sky, -1=all gnd

horizon_vec	vms_vector <>	;unscaled up vector

up_right	fix	?
down_right	fix	?
down_left	fix	?
up_left	fix	?

left_point	fix	?,?	;coords of left point
left_point_edge	dd	?	;which edge point is on
right_point	fix	?,?	;coords of right point
right_point_edge	dd	?	;which edge point is on

;the coordinates of the four corners of the screen (values are placeholders)
corners	fix	0,0	;up left
	fix	319,0	;up right
	fix	319,199	;down right
	fix	0,199	;down left

horizon_poly	dw	5 dup (?,?)	;max of 5 points

;for g3_compute_horz_vecs
xfrac	fix	?
yfrac	fix	?

vec_ptr	dd	?
corner_num	dd	?

;for compute corner vec
m13	fix	?	;m1-m3
m46	fix	?
m79	fix	?
m56	fix	?
m23	fix	?
m89	fix	?

_DATA	ends


_TEXT	segment	dword public USE32 'CODE'

	extn	gr_setcolor_,gr_clear_canvas_
	extn	gr_upoly_tmap_

;draws a horizon. takes eax=sky_color, edx=ground_color
g3_draw_horizon:	pushm	eax,ebx,ecx,edx,esi,edi

	mov	sky_color,eax
	mov	ground_color,edx

	mov	color_swap,0	;assume no swap
	mov	sky_ground_flag,0	;assume both

	test	View_matrix.uvec.y,-1	;check up
	jg	sky_up
	js	swap_colors
;y is zero. check x
	test	View_matrix.uvec.x,-1
	js	sky_up
swap_colors:	xchg	eax,edx	;make sky down
	mov	color_swap,-1	;we swapped
sky_up:	mov	top_color,eax
	mov	bot_color,edx

;compute horizon_vector
	mov	eax,Unscaled_matrix.rvec.y
	fixmul	Matrix_scale.y
	fixmul	Matrix_scale.z
	mov	horizon_vec.x,eax
	mov	esi,eax

	mov	eax,Unscaled_matrix.uvec.y
	fixmul	Matrix_scale.x
	fixmul	Matrix_scale.z
	mov	horizon_vec.y,eax
	mov	edi,eax

	mov	eax,Unscaled_matrix.fvec.y
	fixmul	Matrix_scale.x
	fixmul	Matrix_scale.y
	mov	horizon_vec.z,eax

;now compute values & flag for 4 corners. eax = z componant
	mov	ebx,0ff00h	;and/or flags

	add	eax,esi
	add	eax,edi	;eax = x+y+z
	mov	up_right,eax
	cdq
	and	bh,dl	;flags_and
	or	bl,dl	;flags_or

	sal	esi,1
	sal	edi,1	;get x*2, y*2

	sub	eax,edi	;eax = x-y+z
	mov	down_right,eax
	cdq
	and	bh,dl	;flags_and
	or	bl,dl	;flags_or

	sub	eax,esi	;eax = -x-y+z
	mov	down_left,eax
	cdq
	and	bh,dl	;flags_and
	or	bl,dl	;flags_or

	add	eax,edi	;eax = -x+y+z
	mov	up_left,eax
	cdq

;;	debug	"u: %x %x %x  c: %x %x %x %x\n",unscaled_matrix.uvec.y,unscaled_matrix.uvec.z,unscaled_matrix.uvec.x,up_left,up_right,down_right,down_left

;check flags for all sky or all ground. dl=flag from last corner

	and	bh,dl	;flags_and
	jnz	all_ground
	or	bl,dl	;flags_or
	jz	all_sky

;fill in values for corners

	mov	eax,Canvas_width
	sal	eax,16	;make a fix
	mov	corners+8,eax
	mov	corners+16,eax

	mov	eax,Canvas_height
	sal	eax,16	;make a fix
	mov	corners+20,eax
	mov	corners+28,eax


;check for intesection with each of four edges & compute horizon line
	lea	ebx,left_point

;check intersection with left edge

	mov	eax,up_left
	xor	eax,down_left	;signs different?
	jns	not_left	;..no, so no isect with left

	mov	eax,corners+20
	fixmuldiv	up_left,edi	;eax = up_left * height / y*2
	abs_eax

	mov	4[ebx],eax	;y
	xor	eax,eax
	mov	[ebx],eax	;x=0
	mov	8[ebx],eax	;edge 0
	add	ebx,12
not_left:
	
;check intersection with top edge

	mov	eax,up_left
	xor	eax,up_right	;signs different?
	jns	not_top	;..no, so no isect with left

	mov	eax,corners+16
	fixmuldiv	up_left,esi	;eax = up_right * width / x*2
	abs_eax

	mov	[ebx],eax	;x
	xor	eax,eax
	mov	4[ebx],eax	;y=0
	inc	eax
	mov	8[ebx],eax	;edge 1
	add	ebx,12
not_top:
	
;check intersection with right edge

	mov	eax,up_right
	xor	eax,down_right	;signs different?
	jns	not_right	;..no, so no isect with left

	mov	eax,corners+20
	fixmuldiv	up_right,edi	;eax = up_right * height / y*2
	abs_eax

	;neg	eax
	;add	eax,corners+20

	mov	4[ebx],eax	;y
	mov	eax,corners+16
	mov	[ebx],eax	;x=width-1
	mov	eax,2
	mov	8[ebx],eax	;edge 2
	add	ebx,12
not_right:
	
;check intersection with bottom edge

	mov	eax,down_right
	xor	eax,down_left	;signs different?
	jns	not_bot	;..no, so no isect with left

	mov	eax,corners+16
	fixmuldiv	down_left,esi	;eax = down_right * width / x*2
	abs_eax

	mov	[ebx],eax	;x
	mov	eax,corners+20
	mov	4[ebx],eax	;y=height-1
	mov	eax,3
	mov	8[ebx],eax	;edge 3
	add	ebx,12
not_bot:

;make sure we wrote two and only two ends of horizon line
	ifndef	NDEBUG
	 cmp	ebx,offset left_point+24
	 break_if	ne,'Wrong number of values for horizon_line'
	endif

;make sure first edge is left

	mov	eax,left_point
	cmp	eax,right_point
	jle	line_ok

;swap left and right
	xchg	eax,right_point
	mov	left_point,eax

	mov	eax,left_point+4
	xchg	eax,right_point+4
	mov	left_point+4,eax

	mov	eax,left_point+8
	xchg	eax,right_point+8
	mov	left_point+8,eax

line_ok:

;draw two polygons from horizon line

	mov	eax,top_color
	cmp	eax,-1	;no draw?
	je	no_draw_top
	call	gr_setcolor_
	lea	esi,left_point
	lea	edi,right_point
	call	draw_horz_poly
no_draw_top:

	mov	eax,bot_color
	cmp	eax,-1	;no draw?
	je	no_draw_bot
	call	gr_setcolor_
	lea	esi,right_point
	lea	edi,left_point
	call	draw_horz_poly
no_draw_bot:

;draw lines for diagnostics
;;	xor	eax,eax
;;	call	gr_setcolor_
;;	mov	eax,left_point
;;	mov	edx,left_point+4
;;	mov	ebx,right_point
;;	mov	ecx,right_point+4
;;	call	gr_line_
	
done_horizon:	popm	eax,ebx,ecx,edx,esi,edi
	ret

;we see all sky or all ground.  clear screen with appropriate color
all_ground:  	mov	eax,ground_color
	mov	sky_ground_flag,-1	;all ground
	jmp	do_clear
all_sky:	mov	eax,sky_color
	mov	sky_ground_flag,1	;all sky
do_clear:	call	gr_clear_canvas_
	jmp	done_horizon


;draw a polygon (one half of horizon) from the horizon line
draw_horz_poly:	lea	ebx,horizon_poly

;copy horizon line as first points in poly

	mov	eax,[edi]
	mov	[ebx],eax
	mov	eax,4[edi]
	mov	4[ebx],eax

	mov	eax,[esi]
	mov	8[ebx],eax
	mov	eax,4[esi]
	mov	12[ebx],eax

;add corners to polygon

	mov	eax,8[esi]	;edge number of start edge

	mov	ecx,8[edi]	;edge number of end point
	sub	ecx,eax	;number of edges
	jns	edgenum_ok
	add	ecx,4
edgenum_ok:
	mov	edx,ecx	;save count
	sal	eax,3	;edge * 8
	lea	esi,corners[eax]	;first corner
	lea	edi,16[ebx]	;rest of poly
corner_loop:	movsd
	movsd		;copy a corner
	cmp	esi,offset corners+8*4	;end of list?
	jne	no_wrap
	lea	esi,corners
no_wrap:	loop	corner_loop

;now draw the polygon
	mov	eax,edx	;get corner count
	add	eax,2	;..plus horz line end points
	lea	edx,horizon_poly	;get the points
;;	call	gr_poly_	;draw it!
 call gr_upoly_tmap_
	ret

;return information on the polygon that is the sky. 
;takes ebx=ptr to x,y pairs, ecx=ptr to vecs for each point
;returns eax=number of points
;IMPORTANT: g3_draw_horizon() must be called before this routine.
g3_compute_sky_polygon:
	test	sky_ground_flag,-1	;what was drawn
	js	was_all_ground
	jg	was_all_sky	

	pushm	ebx,ecx,edx,esi,edi

	lea	esi,left_point
	lea	edi,right_point
	test	color_swap,-1
	jz	no_swap_ends
	xchg	esi,edi	;sky isn't top
no_swap_ends:

;copy horizon line as first points in poly

	mov	eax,[edi]	;copy end point
	mov	[ebx],eax
	mov	eax,4[edi]
	mov	4[ebx],eax

	mov	eax,[esi]	;copy start point
	mov	8[ebx],eax
	mov	eax,4[esi]
	mov	12[ebx],eax

	pushm	ebx,ecx
	push	edi	;save end point
	push	esi	;save start point
	mov	esi,edi	;end point is first point
	mov	edi,ecx	;dest buffer
	call	compute_horz_end_vec

	pop	esi	;get back start point
	add	edi,12	;2nd vec
	call	compute_horz_end_vec

	pop	edi	;get back end point
	popm	ebx,ecx
	add	ebx,16	;past two x,y pairs
	add	ecx,24	;past two vectors

	mov	vec_ptr,ecx

;add corners to polygon

	mov	eax,8[esi]	;edge number of start edge
	mov	corner_num,eax

	mov	ecx,8[edi]	;edge number of end point
	sub	ecx,eax	;number of edges
	jns	edgenum_ok2
	add	ecx,4
edgenum_ok2:
	push	ecx	;save count
	sal	eax,3	;edge * 8
	lea	esi,corners[eax]	;first corner
	mov	edi,ebx	;rest of poly 2d points
corner_loop2:
	movsd
	movsd		;copy a corner
	cmp	esi,offset corners+8*4	;end of list?
	jne	no_wrap2
	lea	esi,corners
no_wrap2:
	pushm	ecx,esi,edi
	mov	edi,vec_ptr
	mov	eax,corner_num
	call	compute_corner_vec
	add	vec_ptr,12
	inc	corner_num
	popm	ecx,esi,edi

	loop	corner_loop2

;now return with count
	pop	eax	;get corner count
	add	eax,2	;..plus horz line end points

	popm	ebx,ecx,edx,esi,edi

	ret

;we drew all ground, so there was no horizon drawn
was_all_ground:	xor	eax,eax	;no points in poly
	ret

;we drew all sky, so find 4 corners
was_all_sky:	pushm	ebx,ecx,edx,esi,edi
	push	ecx
	lea	esi,corners
	mov	edi,ebx
	mov	ecx,8
	rep	movsd
	pop	edi

	mov	ecx,4
	xor	eax,eax	;start corner 0
sky_loop:	pushm	eax,ecx,edi
	call	compute_corner_vec
	popm	eax,ecx,edi
	add	edi,12
	inc	eax
	loop	sky_loop
	mov	eax,4	;4 corners
	popm	ebx,ecx,edx,esi,edi
	ret

;compute vector describing horizon intersection with a point.
;takes esi=2d point, edi=vec. trashes eax,ebx,ecx,edx
compute_horz_end_vec:

;compute rotated x/z & y/z ratios

	mov	eax,[esi]	;get x coord
  	sub	eax,Canv_w2
	fixdiv	Canv_w2
	mov	xfrac,eax	;save

	mov	eax,4[esi]	;get y coord
  	sub	eax,Canv_h2
	fixdiv	Canv_h2
	neg	eax	;y inversion
	mov	yfrac,eax	;save

;compute fraction unrotated x/z

	mov	eax,xfrac
	add	eax,yfrac
	mov	ecx,eax	;save
	fixmul	View_matrix.m9
	sub	eax,View_matrix.m7
	sub	eax,View_matrix.m8
	mov	ebx,eax	;save numerator

	mov	eax,ecx
	fixmul	View_matrix.m3
	mov	ecx,eax
	mov	eax,View_matrix.m1
	add	eax,View_matrix.m2
	sub	eax,ecx

;now eax/ebx = z/x. do divide in way to give result < 0

	pushm	eax,ebx
	abs_eax
	xchg	eax,ebx
	abs_eax
	cmp	eax,ebx	;which is bigger?
	popm	eax,ebx
	jl	do_xz

;x is bigger, so do as z/x

	fixdiv	ebx
	
;now eax = z/x ratio.  Compute vector by normalizing and correcting sign

	push	eax	;save ratio

	imul	eax	;compute z*z
	inc	edx	;+ x*x (x==1)
	call	quad_sqrt

	mov	ecx,eax	;mag in ecx
	pop	eax	;get ratio, x part

	fixdiv	ecx
	mov	[edi].z,eax

	mov	eax,f1_0
	fixdiv	ecx

	mov	[edi].x,eax

	jmp	finish_end

;z is bigger, so do as x/z
do_xz:
	xchg	eax,ebx
	fixdiv	ebx
	
;now eax = x/z ratio.  Compute vector by normalizing and correcting sign

	push	eax	;save ratio

	imul	eax	;compute x*x
	inc	edx	;+ z*z (z==1)
	call	quad_sqrt

	mov	ecx,eax	;mag in ecx
	pop	eax	;get ratio, x part

	fixdiv	ecx
	mov	[edi].x,eax

	mov	eax,f1_0
	fixdiv	ecx

	mov	[edi].z,eax

finish_end:	xor	eax,eax	;y = 0
	mov	[edi].y,eax

;now make sure that this vector is in front of you, not behind

	mov	eax,[edi].x
	imul	View_matrix.m3
	mov	ebx,eax
	mov	ecx,edx
	mov	eax,[edi].z
	imul	View_matrix.m9
	add	eax,ebx
	adc	edx,ecx
	jns	vec_ok	;has positive z, ok

;z is neg, flip vector

	neg	[edi].x
	neg	[edi].z
vec_ok:
	ret

MIN_DEN equ 7fffh

sub2	macro	dest,src
	mov	eax,src
	sal	eax,1
	sub	dest,eax
	endm

;compute vector decribing a corner of the screen.
;takes edi=vector, eax=corner num
compute_corner_vec:

	cmp	eax,4
	jl	num_ok
	sub	eax,4
num_ok:

;compute all deltas
	mov	ebx,View_matrix.m1
	mov	ecx,View_matrix.m4
	mov	edx,View_matrix.m7

	or	eax,eax
	jz	neg_x
	cmp	eax,3
	jne	no_neg_x
neg_x:
	neg	ebx
	neg	ecx
	neg	edx
no_neg_x:	
	sub	ebx,View_matrix.m3
	mov	m13,ebx	;m1-m3
	sub	ecx,View_matrix.m6
	mov	m46,ecx	;m4-m6
	sub	edx,View_matrix.m9
	mov	m79,edx	;m7-m9

	mov	ebx,View_matrix.m5
	mov	ecx,View_matrix.m2
	mov	edx,View_matrix.m8

	cmp	eax,2
	jl	no_neg_y
neg_y:
	neg	ebx
	neg	ecx
	neg	edx
no_neg_y:	
	sub	ebx,View_matrix.m6
	mov	m56,ebx	;m5-m6
	sub	ecx,View_matrix.m3
	mov	m23,ecx	;m2-m3
	sub	edx,View_matrix.m9
	mov	m89,edx	;m8-m9

;compute x/z ratio

;compute denomonator

	mov	eax,m46
	fixmul	m23
	mov	ebx,eax	;save

	mov	eax,m56
	fixmul	m13
	sub	eax,ebx	;eax = denominator

;now we have the denominator.  If it is too small, try x/y, z/y or z/x, y/x

	mov	ecx,eax	;save den

	abs_eax
	cmp	eax,MIN_DEN
	jl	z_too_small

z_too_small:

;now do x/z numerator

	mov	eax,m79
	fixmul	m56	;* (m5-m6)
	mov	ebx,eax

	mov	eax,m89
	fixmul	m46	;* (m4-m6)
	sub	eax,ebx

;now, eax/ecx = x/z ratio

	fixdiv	ecx	;eax = x/z

	mov	[edi].x,eax	;save x

;now do y/z

	mov	eax,m89
	fixmul	m13
	mov	ebx,eax

	mov	eax,m79
	fixmul	m23
	sub	eax,ebx

;now eax/ecx = y/z ratio

	fixdiv	ecx

	mov	[edi].y,eax

	mov	[edi].z,f1_0

	mov	esi,edi
	call	vm_vec_normalize

;make sure this vec is pointing in right direction

	lea	edi,View_matrix.fvec
	call	vm_vec_dotprod
	or	eax,eax	;check sign
	jg	vec_sign_ok

	neg	[esi].x
	neg	[esi].y
	neg	[esi].z
vec_sign_ok:

	ret


_TEXT	ends

	end
