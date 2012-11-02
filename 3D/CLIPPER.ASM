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
; $Source: f:/miner/source/3d/rcs/clipper.asm $
; $Revision: 1.6 $
; $Author: matt $
; $Date: 1994/07/25 00:00:02 $
;
; Source for clipper
;
; $Log: clipper.asm $
; Revision 1.6  1994/07/25  00:00:02  matt
; Made 3d no longer deal with point numbers, but only with pointers.
; 
; Revision 1.5  1994/02/10  18:00:38  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.4  1994/01/13  15:39:09  mike
; Change usage of Frame_count to _Frame_count
; 
; Revision 1.3  1993/11/04  18:48:39  matt
; Added system to only rotate points once per frame
; 
; Revision 1.2  1993/11/04  12:36:25  mike
; Add clipping for lighting value.
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

rcsid	db	"$Id: clipper.asm 1.6 1994/07/25 00:00:02 matt Exp $"
	align	4

 public free_point_num

;buffer of temp points for when clipping creates a new point
temp_points	db	MAX_POINTS_IN_POLY * size g3s_point dup (?)

free_points	label	dword
p = temp_points
	rept	MAX_POINTS_IN_POLY
	 dd	p
	 p = p+size g3s_point
	endm

free_point_num	dd	0
	
;Vars for polygon clipper

nverts	dd	?	;number of verts in poly
nv_cnt	dd	?	;loop variable

plane	db	?

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

;get a temporary point. returns ebp=point.
get_temp_point:	mov	ebp,free_point_num
	mov	ebp,free_points[ebp*4]
	inc	free_point_num
	mov	[ebp].p3_flags,PF_TEMP_POINT	;clear proj,set temp
	ifndef	NDEBUG
	 cmp	free_point_num,MAX_POINTS_IN_POLY
	 break_if	e,"temp_point buf empty!"
	endif
	ret

;free a temporary point. takes esi=point
free_temp_point:	push	eax
	dec	free_point_num
	mov	eax,free_point_num
	mov	free_points[eax*4],esi
	pop	eax
	ret

;clip a particular value (eg. x, y, u). 
;assumes esi,edi=start,end points, ebp=dest point, and ebx/ecx=fraction
;stores new value and returns it in eax
clip_value	macro	ofs
	mov	eax,[edi].ofs	;end
	sub	eax,[esi].ofs	;-start
	imul	ebx
	idiv	ecx
	add	eax,[esi].ofs	;+start
	mov	[ebp].ofs,eax
	endm

;clips an edge against one plane. 
;takes esi (on) ,edi (off)=points, cl=plane flag (1,2,4,8 = left,right,bot,top)
;returns esi,edi=clipped points (edi new), bl=codes
;trashes eax,edx,ebp
clip_edge:

;compute clipping value k = (xs-zs) / (xs-xe-zs+ze)
;use x or y as appropriate, and negate x/y value as appropriate

	mov	ebx,[esi].x	;eax = xs
	mov	edx,[edi].x	;ebx = xe

	test	cl,CC_OFF_TOP+CC_OFF_BOT	;top or bot (y)?
	jz	not_y
	mov	ebx,[esi].y	;eax = ys
	mov	edx,[edi].y	;ebx = ye
not_y:
	test	cl,CC_OFF_LEFT+CC_OFF_BOT ;right or top (neg)?
	jz	not_neg
	neg	ebx
	neg	edx
not_neg:
	mov	plane,cl	;store
	push	ecx	;save plane

	sub	ebx,[esi].z	;ebx = xs-zs
	mov	ecx,ebx	;ecx = xs-zs
	sub	ecx,edx	;edx = xs-xe-zs
	add	ecx,[edi].z	;edx = xs-xe+zs+ze

;now frac=ebx/ecx

	call	get_temp_point	;ret ebp=point
	;;mov	ax,_Frame_count
	;;mov	[ebp].p3_frame,ax	;this point valid 

	clip_value x
	mov	[ebp].z,eax	;assume z=x

	clip_value y

	test	plane,CC_OFF_TOP+CC_OFF_BOT	;top or bot (y)?
	jz	not_y2
	mov	[ebp].z,eax	;z=y
not_y2:

;check if uv values present, and clip if so
	test	[esi].p3_flags,PF_UVS	;uv values here?
	jz	no_clip_uv	;..nope

	clip_value p3_u
	clip_value p3_v
	or	[ebp].p3_flags,PF_UVS	;new point has uv set
no_clip_uv:

;check if lv values present, and clip if so
	test	[esi].p3_flags,PF_LVS	;lv values here?
	jz	no_clip_lv	;..nope

	clip_value p3_l
	or	[ebp].p3_flags,PF_LVS	;new point has lv set
no_clip_lv:
	pop	ecx	;get plane back

;negate z if clipping against left or bot
	test	cl,CC_OFF_LEFT+CC_OFF_BOT ;right or top (neg)?
	jz	not_neg2
	neg	[ebp].z	;z=-x (or -y)
not_neg2:

;see if discarded point is temp point, and free it if so

	mov	edi,ebp	;return correct point
	mov	eax,edi
	jmp	code_point	;returns bl=codes


;clips a line to the viewing pyramid.
;takes esi,edi=points (pointers), al=codes_or
;return esi,edi=clipped points, one or both new
clip_line:	pushm	ebx,ecx,edx,ebp

	mov	ecx,1	;plane flag

l_plane_loop:	test	al,cl	;off this plane?
	jz	l_not_this_plane

	test	[esi].p3_codes,cl	;this one on?
	jz	order_ok	;..yep
	xchg	esi,edi	;..nope
order_ok:
	push	edi	;save old off-screen point
	call	clip_edge	;esi=on, edi=off
			;returns bl=new edi codes

	pop	eax	;get discarded point
;see if must free rejected point
	test	[eax].p3_flags,PF_TEMP_POINT
	jz	not_temp_eax
	xchg	esi,eax
	call	free_temp_point
	mov	esi,eax
not_temp_eax:

	mov	al,[esi].p3_codes	;get esi codes
	test	al,bl	;clipped away?
	jnz	l_clipped_away	;..yep
	or	al,bl	;get new codes_or
l_not_this_plane:
	add	ecx,ecx	;next plane
	cmp	ecx,16	;done?
	jne	l_plane_loop	;..nope

l_clipped_away:	popm	ebx,ecx,edx,ebp
	ret


;TEMPORARY - inline this code when working
;takes esi,edi=lists
;returns edi=end of new list, dx=new codes. trashes eax,ebx,edx,ebp,esi
clip_plane:
	mov	ebx,nverts

;copy first two verts to end
	mov	eax,[esi]
	mov	[esi+ebx*4],eax
	mov	eax,4[esi]
	mov	4[esi+ebx*4],eax

	mov	nv_cnt,ebx	;loop variable

	add	esi,4	;point at second

	mov	edx,0ff00h	;initialize codes

point_loop:	mov	eax,[esi]	;get current point
	add	esi,4

;go though list of points. 

	test	[eax].p3_codes,cl	;cur point off?
	jz	cur_not_off	;..nope

;cur point is off. check prev and next

	mov	ebx,[esi-8]	;get prev
	test	[ebx].p3_codes,cl	;prev off?
	jnz	prev_off	;..yup, so nothing to clip

	push	eax	;save current point

;must clip cur (off screen) to prev (on screen)
	pushm	edx,esi,edi	;save codes,list pointers

	mov	esi,ebx	;esi=on screen
	mov	edi,eax	;edi=off screen
	call	clip_edge
	mov	eax,edi	;save new point

	popm	edx,esi,edi	;get codes,list pointers

	mov	[edi],eax	;store in dest list
	add	edi,4
	or	dl,bl	;update codes_or
	and	dh,bl	;update codes_and

	pop	eax	;restore current point
prev_off:
	mov	ebx,[esi]	;get next
	test	[ebx].p3_codes,cl	;next off?
	jnz	next_off	;..yes

	push	eax	;save current point

;must clip cur (off screen) to next (on screen)
	pushm	edx,esi,edi	;save codes,list pointers

	mov	esi,ebx	;esi=on screen
	mov	edi,eax	;edi=off screen
	call	clip_edge
	mov	eax,edi	;save new point

	popm	edx,esi,edi	;get codes,list pointers

	mov	[edi],eax	;store in dest list
	add	edi,4
	or	dl,bl	;update codes_or
	and	dh,bl	;update codes_and

	pop	eax	;get current back
next_off:

;see if must free discarded point
	test	[eax].p3_flags,PF_TEMP_POINT
	jz	not_temp
	xchg	esi,eax
	call	free_temp_point
	mov	esi,eax
not_temp:

	jmp	do_next_point

cur_not_off:	mov	[edi],eax	;store cur in dest buffer
	add	edi,4
	or	dl,[eax].p3_codes	;update codes_or
	and	dh,[eax].p3_codes	;update codes_and

do_next_point:	dec	nv_cnt
	jnz	point_loop

	ret


;3d clip a polygon.  
;takes esi=src list, edi=dest list, ecx=nverts, al=codes_or
;returns esi=list of clipped points, some of them new, ecx=new nverts, dx=codes
;esi at exit will be either of esi,edi at entry
clip_polygon:	push	ebp

	mov	nverts,ecx	;save

;now loop through each plane, a-clipping as we go

	mov	ecx,1	;plane flag

	mov	dl,al	;dl = codes_or 

;clipping from [esi] -> [edi]
p_plane_loop:	test	dl,cl	;off this plane?
	jz	p_not_this_plane

	push	esi	;save src ptr
	push	edi	;save dest ptr

;clip against this plane
	call	clip_plane	;when working, inline this code

;done clip for this plane
	mov	eax,edi	;end of dest loop
	pop	esi	;get start of dest buffer

	sub	eax,esi	;get delta
	sar	eax,2	;get new vert count
	mov	nverts,eax	;save new value

	pop	edi	;new dest = old scr

	test	dh,dh	;check new codes_and
	jnz	p_clipped_away	;polygon all off screen

p_not_this_plane:	add	ecx,ecx	;next plane
	cmp	ecx,16	;done?
	jne	p_plane_loop	;..nope

p_clipped_away:	pop	ebp
	mov	ecx,nverts	;new value
	ret


_TEXT	ends



;should I write a combined clipper/projector?

	end

