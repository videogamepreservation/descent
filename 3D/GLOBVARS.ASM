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
; $Source: f:/miner/source/3d/rcs/globvars.asm $
; $Revision: 1.6 $
; $Author: matt $
; $Date: 1994/11/19 17:25:27 $
;
; Global variables for 3d system
;
; $Log: globvars.asm $
; Revision 1.6  1994/11/19  17:25:27  matt
; Took out unneeded var define
; 
; Revision 1.5  1994/07/25  00:00:03  matt
; Made 3d no longer deal with point numbers, but only with pointers.
; 
; Revision 1.4  1994/01/28  15:23:23  matt
; Added _Matrix_scale so C can get at those vars
; 
; Revision 1.3  1994/01/13  15:38:29  mike
; rename Frame_count to _Frame_count, can you guess why?
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
	include	3d.inc
	.list

	assume	ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: globvars.asm 1.6 1994/11/19 17:25:27 matt Exp $"
	align	4

View_position	vms_vector <>
View_zoom	fix	?

Unscaled_matrix	vms_matrix <>	;before scaling
View_matrix	vms_matrix <>

Window_scale	vms_vector <>	;scaling for window aspect
Matrix_scale	vms_vector <>	;how the matrix is scaled
			;window_scale * zoom
Canvas_width	dd	?	;the actual width
Canvas_height	dd	?	;the actual height

Canv_w2	fix	?	;fixed-point width/2
Canv_h2	fix	?	;fixed-point height/2

;vertex buffers for polygon drawing and clipping
Vbuf0	dd	MAX_POINTS_IN_POLY dup (?)
Vbuf1	dd	MAX_POINTS_IN_POLY dup (?)

;list of 2d coords
Vertex_list	fix	MAX_POINTS_IN_POLY*2 dup (?)

;;;used for determining whether to rotate a frame
;;_Frame_count	dw	-1	;first frame will inc to 0

_DATA	ends

	end

