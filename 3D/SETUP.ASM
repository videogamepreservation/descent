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
; $Source: f:/miner/source/3d/rcs/setup.asm $
; $Revision: 1.13 $
; $Author: matt $
; $Date: 1995/02/09 22:00:53 $
;
; Source for setup,close,start & end frame routines
;
; $Log: setup.asm $
; Revision 1.13  1995/02/09  22:00:53  matt
; Removed dependence on divide overflow handler; we now check for overflow
; before dividing.  This fixed problems on some TI chips.
; 
; Revision 1.12  1994/11/10  21:29:48  mike
; call init_interface_vars_to_assembler.
; 
; Revision 1.11  1994/10/03  12:50:54  matt
; Fixed stupid mistake
; 
; Revision 1.10  1994/10/03  12:50:12  matt
; Took out unused routines & data
; 
; Revision 1.9  1994/07/25  00:00:05  matt
; Made 3d no longer deal with point numbers, but only with pointers.
; 
; Revision 1.8  1994/04/19  18:26:43  matt
; Added g3_draw_sphere() function.
; 
; Revision 1.7  1994/03/15  21:21:58  matt
; Put in check for div overflow in blob code
; 
; Revision 1.6  1994/02/10  18:00:43  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.5  1994/01/28  13:07:50  matt
; Added func, g3_get_view_pos_ptr_, sortof a hack, for the polygon objects.
; 
; Revision 1.4  1994/01/14  17:19:50  matt
; Added check for no points in start_frame()
; 
; Revision 1.2  1993/11/04  18:49:18  matt
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
	include	div0.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: setup.asm 1.13 1995/02/09 22:00:53 matt Exp $"
	align	4

 extd free_point_num

_DATA	ends



_TEXT	segment	dword public USE32 'CODE'

	extn	init_interface_vars_to_assembler_
	extn	atexit_	;C library function

;I don't what these are supposed to do, but here they are.

;sets up divide overflow handler, and sets the global points variables. 
;takes eax=points, edx=vecs, ecx=num points
;trashes eax,edx
g3_init:
	mov	eax,DM_ERROR
	call	div0_init_
	or	eax,eax	;check error
	jnz	dz_ok
	int	3
dz_ok:

;;@@;handlers for point projection
;;@@	lea	eax,cs:proj_div0
;;@@	lea	edx,cs:div_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@	lea	eax,cs:proj_div1
;;@@	lea	edx,cs:div_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@;handlers for blob draw
;;@@	lea	eax,cs:blob_div0
;;@@	lea	edx,cs:blob_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@	lea	eax,cs:blob_div1
;;@@	lea	edx,cs:blob_overflow_handler
;;@@	call	div0_set_handler_
;;@@
;;@@;handler for sphere projection
;;@@	lea	eax,cs:sphere_proj_div
;;@@	lea	edx,cs:sphere_div_overflow_handler
;;@@	call	div0_set_handler_
;;@@

	lea	eax,cs:g3_close
	call	atexit_

	ret


g3_close:	ret


;cv_bitmap equ 0
;bm_w equ 4
;bm_h equ 6

;start rendering a frame. sets up window vars
g3_start_frame:	pushm	eax,edx,esi

	mov	esi,grd_curcanv

	movzx	eax,w [esi].cv_bitmap.bm_w
	mov	Canvas_width,eax	;store width
	sal	eax,15	;get fixed-point width/2
	mov	Canv_w2,eax

	movzx	eax,w [esi].cv_bitmap.bm_h
	mov	Canvas_height,eax	;store height
	sal	eax,15	;get fixed-point height/2
	mov	Canv_h2,eax

;compute aspect ratio for this canvas
	mov	esi,grd_curscreen
	mov	eax,[esi].sc_aspect	;get pixel ratio
	imul	Canvas_height	;figure in window size
	idiv	Canvas_width

	cmp	eax,f1_0	; > 1?
	jg	scale_y
	mov	Window_scale.x,eax
	mov	Window_scale.y,f1_0
	jmp	got_win_scale
scale_y:	mov	esi,eax
	mov	eax,f1_0
	fixdiv	esi
	mov	Window_scale.y,eax
	mov	Window_scale.x,f1_0
got_win_scale:	mov	Window_scale.z,f1_0	;always 1

	call	init_interface_vars_to_assembler_

	popm	eax,edx,esi
	ret

;this doesn't do anything, but is here for completeness
g3_end_frame:
	ifndef	NDEBUG
	 test	free_point_num,-1
	 ;break_if	nz,"Error: free_point_num != 0 in g3_end_frame"
	 ;Commented the above line out, so it would compile
	endif
	ret


;@@ public g3_get_view_pos_ptr_
;@@g3_get_view_pos_ptr_:
;@@	lea	eax,View_position
;@@	ret


_TEXT	ends

	end
