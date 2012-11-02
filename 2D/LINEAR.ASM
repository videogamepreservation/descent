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
; $Source: f:/miner/source/2d/rcs/linear.asm $
; $Revision: 1.20 $
; $Author: john $
; $Date: 1994/11/28 17:08:30 $
;
; Routines to access linear VGA memory
;
; $Log: linear.asm $
; Revision 1.20  1994/11/28  17:08:30  john
; Took out some unused functions in linear.asm, moved
; gr_linear_movsd from linear.asm to bitblt.c, made sure that
; the code in ibiblt.c sets the direction flags before rep movsing.
; 
; Revision 1.19  1994/11/27  22:57:56  john
; Took out some code that was never called.
; 
; Revision 1.18  1994/09/12  14:40:16  john
; Neatend.
; 
; Revision 1.17  1994/07/27  18:30:30  john
; Took away the blending table.
; 
; Revision 1.16  1994/04/08  16:59:28  john
; Add fading poly's; Made palette fade 32 instead of 16.
; 
; Revision 1.15  1993/12/21  20:10:03  john
; *** empty log message ***
; 
; Revision 1.14  1993/12/21  19:58:31  john
; added selector stuff
; 
; Revision 1.13  1993/12/21  11:40:51  john
; *** empty log message ***
; 
; Revision 1.12  1993/12/09  15:01:52  john
; Changed palette stuff majorly
; 
; Revision 1.11  1993/12/08  16:41:02  john
; *** empty log message ***
; 
; Revision 1.10  1993/12/08  11:50:17  john
; Fixed bug with gr_init
; 
; Revision 1.9  1993/12/07  12:32:12  john
; moved bmd_palette to gr_palette
; 
; Revision 1.8  1993/12/03  12:11:25  john
; *** empty log message ***
; 
; Revision 1.7  1993/11/16  11:29:08  john
; *** empty log message ***
; 
; Revision 1.6  1993/10/15  16:22:13  john
; *** empty log message ***
; 
; Revision 1.5  1993/09/29  16:15:28  john
; added assembler linear_line
; 
; Revision 1.4  1993/09/26  18:59:27  john
; fade stuff
; 
; Revision 1.3  1993/09/21  14:00:59  john
; added code to save 43/50 line text modes.
; 
; Revision 1.2  1993/09/16  17:28:06  john
; added code to save/restore video mode
; 
; Revision 1.1  1993/09/08  11:41:30  john
; Initial revision
; 
;
;


.386



_DATA   SEGMENT BYTE PUBLIC USE32 'DATA'


INCLUDE VGAREGS.INC


		; Put data here

		PUBLIC _gr_var_color
		PUBLIC _gr_var_bitmap
		PUBLIC _gr_var_bwidth
		_gr_var_color   dd  ?
		_gr_var_bitmap  dd  ?
		_gr_var_bwidth  dd  ?

		; Local variables for gr_linear_line_
		AdjUp           dd  ?   ;error term adjust up on each advance
		AdjDown         dd  ?   ;error term adjust down when error term turns over
		WholeStep       dd  ?   ;minimum run length
		XAdvance        dd  ?   ;1 or -1, for direction in which X advances
		XStart          dd  ?
		YStart          dd  ?
		XEnd            dd  ?
		YEnd            dd  ?


_DATA   ENDS

DGROUP  GROUP _DATA


_TEXT   SEGMENT BYTE PUBLIC USE32 'CODE'

		ASSUME  DS:_DATA
		ASSUME  CS:_TEXT

		include psmacros.inc
		; Args passed in EAX, EDX, EBX, ECX


; Fast run length slice line drawing implementation for mode 0x13, the VGA's
; 320x200 256-color mode.
; Draws a line between the specified endpoints in color Color.


PUBLIC gr_linear_line_
gr_linear_line_:

	cld

	push    esi  ;preserve C register variables
	push    edi
	push    ebp

	mov     XStart, eax
	mov     YStart, edx
	mov     XEnd, ebx
	mov     YEnd, ecx

	mov     ebp, _gr_var_bwidth

; We'll draw top to bottom, to reduce the number of cases we have to handle,
; and to make lines between the same endpoints always draw the same pixels.

	mov     eax,YStart
	cmp     eax,YEnd
	jle     LineIsTopToBottom
	xchg    YEnd, eax    ;swap endpoints
	mov     YStart, eax
	mov     ebx, XStart
	xchg    XEnd, ebx
	mov     XStart, ebx

LineIsTopToBottom:

; Point EDI to the first pixel to draw.
	mov     edx,ebp
	mul     edx             ;YStart * ebp
	mov     esi, XStart
	mov     edi, esi
	add     edi, _gr_var_bitmap
	add     edi, eax        ;EDI = YStart * ebp + XStart
							; = offset of initial pixel

; Figure out how far we're going vertically (guaranteed to be positive).
	mov     ecx, YEnd
	sub     ecx, YStart     ;ECX = YDelta

; Figure out whether we're going left or right, and how far we're going
; horizontally. In the process, special-case vertical lines, for speed and
; to avoid nasty boundary conditions and division by 0.

	mov     edx, XEnd
	sub     edx, esi        ;XDelta
	jnz     NotVerticalLine ;XDelta == 0 means vertical line
							;it is a vertical line
							;yes, special case vertical line

	mov     eax, _gr_var_color
VLoop:
	mov     [edi],al
	add     edi,ebp
	dec     ecx
	jns     VLoop
	jmp     Done

; Special-case code for horizontal lines.

IsHorizontalLine:
	mov     eax, _gr_var_color
	mov     ah,al           ;duplicate in high byte for word access
	and     ebx,ebx         ;left to right?
	jns     DirSet          ;yes
	sub     edi, edx        ;currently right to left, point to left end so we
							; can go left to right (avoids unpleasantness with
							; right to left REP STOSW)
DirSet:
	mov     ecx, edx
	inc     ecx             ;# of pixels to draw

	shr     ecx, 1          ;# of words to draw
	rep     stosw           ;do as many words as possible
	adc     ecx, ecx
	rep     stosb           ;do the odd byte, if there is one

	jmp     Done

; Special-case code for diagonal lines.

IsDiagonalLine:
	mov     eax, _gr_var_color
	add     ebx, ebp    ;advance distance from one pixel to next

DLoop:
	mov     [edi],al
	add     edi, ebx
	dec     ecx
	jns     DLoop
	jmp     Done

NotVerticalLine:
	mov     ebx,1               ;assume left to right, so XAdvance = 1
								;***leaves flags unchanged***
	jns     LeftToRight         ;left to right, all set
	neg     ebx                 ;right to left, so XAdvance = -1
	neg     edx                 ;|XDelta|

LeftToRight:
; Special-case horizontal lines.

	and     ecx, ecx            ;YDelta == 0?
	jz      IsHorizontalLine    ;yes

; Special-case diagonal lines.
	cmp     ecx, edx            ;YDelta == XDelta?
	jz      IsDiagonalLine      ;yes

; Determine whether the line is X or Y major, and handle accordingly.
	cmp     edx, ecx
	jae     XMajor
	jmp     YMajor

; X-major (more horizontal than vertical) line.

XMajor:
		and     ebx,ebx         ;left to right?
		jns     DFSet           ;yes, CLD is already set
	std                     ;right to left, so draw backwards
DFSet:
		mov     eax,edx         ;XDelta
		sub     edx,edx         ;prepare for division
		div     ecx             ;EAX = XDelta/YDelta
				; (minimum # of pixels in a run in this line)
								;EDX = XDelta % YDelta
		mov     ebx,edx         ;error term adjust each time Y steps by 1;
		add     ebx,ebx         ; used to tell when one extra pixel should be
		mov     AdjUp, ebx      ; drawn as part of a run, to account for
				; fractional steps along the X axis per
				; 1-pixel steps along Y
		mov     esi, ecx        ;error term adjust when the error term turns
		add     esi, esi        ; over, used to factor out the X step made at
		mov     AdjDown, esi    ; that time

; Initial error term; reflects an initial step of 0.5 along the Y axis.
		sub     edx, esi        ;(XDelta % YDelta) - (YDelta * 2)
				;DX = initial error term
; The initial and last runs are partial, because Y advances only 0.5 for
; these runs, rather than 1. Divide one full run, plus the initial pixel,
; between the initial and last runs.
		mov     esi, ecx        ;SI = YDelta
		mov     ecx, eax        ;whole step (minimum run length)
		shr     ecx,1
		inc     ecx             ;initial pixel count = (whole step / 2) + 1;
				; (may be adjusted later). This is also the
		; final run pixel count
		push    ecx             ;remember final run pixel count for later
; If the basic run length is even and there's no fractional advance, we have
; one pixel that could go to either the initial or last partial run, which
; we'll arbitrarily allocate to the last run.
; If there is an odd number of pixels per run, we have one pixel that can't
; be allocated to either the initial or last partial run, so we'll add 0.5 to
; the error term so this pixel will be handled by the normal full-run loop.
		add     edx,esi         ;assume odd length, add YDelta to error term
		; (add 0.5 of a pixel to the error term)
	test    al,1            ;is run length even?
	jnz     XMajorAdjustDone ;no, already did work for odd case, all set
		sub     edx,esi         ;length is even, undo odd stuff we just did
		and     ebx,ebx         ;is the adjust up equal to 0?
	jnz     XMajorAdjustDone ;no (don't need to check for odd length,
								; because of the above test)
		dec     ecx             ;both conditions met; make initial run 1
				; shorter

XMajorAdjustDone:
		mov     WholeStep,eax   ;whole step (minimum run length)
		mov     eax, _gr_var_color       ;AL = drawing color
; Draw the first, partial run of pixels.
	rep     stosb           ;draw the final run
		add     edi,ebp ;advance along the minor axis (Y)
; Draw all full runs.
		cmp     esi,1           ;are there more than 2 scans, so there are
								; some full runs? (SI = # scans - 1)
	jna     XMajorDrawLast  ;no, no full runs
		dec     edx             ;adjust error term by -1 so we can use
				; carry test
		shr     esi,1           ;convert from scan to scan-pair count
	jnc     XMajorFullRunsOddEntry  ;if there is an odd number of scans,
					; do the odd scan now
XMajorFullRunsLoop:
		mov     ecx, WholeStep  ;run is at least this long
		add     edx,ebx         ;advance the error term and add an extra
	jnc     XMajorNoExtra   ; pixel if the error term so indicates
		inc     ecx             ;one extra pixel in run
		sub     edx,AdjDown     ;reset the error term
XMajorNoExtra:
		rep     stosb           ;draw this scan line's run
		add     edi,ebp ;advance along the minor axis (Y)
XMajorFullRunsOddEntry:         ;enter loop here if there is an odd number
				; of full runs
		mov     ecx,WholeStep   ;run is at least this long
		add     edx,ebx         ;advance the error term and add an extra
	jnc     XMajorNoExtra2  ; pixel if the error term so indicates
		inc     ecx             ;one extra pixel in run
		sub     edx,AdjDown     ;reset the error term
XMajorNoExtra2:
		rep     stosb           ;draw this scan line's run
		add     edi,ebp ;advance along the minor axis (Y)

		dec     esi
	jnz     XMajorFullRunsLoop
; Draw the final run of pixels.
XMajorDrawLast:
		pop     ecx             ;get back the final run pixel length
	rep     stosb           ;draw the final run

	cld                     ;restore normal direction flag
	jmp     Done
; Y-major (more vertical than horizontal) line.
YMajor:
		mov     XAdvance,ebx    ;remember which way X advances
		mov     eax,ecx         ;YDelta
		mov     ecx,edx         ;XDelta
		sub     edx,edx         ;prepare for division
		div     ecx             ;EAX = YDelta/XDelta
				; (minimum # of pixels in a run in this line)
								;EDX = YDelta % XDelta
		mov     ebx,edx         ;error term adjust each time X steps by 1;
		add     ebx,ebx         ; used to tell when one extra pixel should be
		mov     AdjUp,ebx       ; drawn as part of a run, to account for
				; fractional steps along the Y axis per
				; 1-pixel steps along X
		mov     esi,ecx         ;error term adjust when the error term turns
		add     esi,esi         ; over, used to factor out the Y step made at
		mov     AdjDown, esi    ; that time

; Initial error term; reflects an initial step of 0.5 along the X axis.
		sub     edx,esi         ;(YDelta % XDelta) - (XDelta * 2)
				;DX = initial error term
; The initial and last runs are partial, because X advances only 0.5 for
; these runs, rather than 1. Divide one full run, plus the initial pixel,
; between the initial and last runs.
		mov     esi,ecx         ;SI = XDelta
		mov     ecx,eax         ;whole step (minimum run length)
		shr     ecx,1
		inc     ecx             ;initial pixel count = (whole step / 2) + 1;
				; (may be adjusted later)
		push    ecx             ;remember final run pixel count for later

; If the basic run length is even and there's no fractional advance, we have
; one pixel that could go to either the initial or last partial run, which
; we'll arbitrarily allocate to the last run.
; If there is an odd number of pixels per run, we have one pixel that can't
; be allocated to either the initial or last partial run, so we'll add 0.5 to
; the error term so this pixel will be handled by the normal full-run loop.
		add     edx,esi         ;assume odd length, add XDelta to error term
	test    al,1            ;is run length even?
	jnz     YMajorAdjustDone ;no, already did work for odd case, all set
		sub     edx,esi         ;length is even, undo odd stuff we just did
		and     ebx,ebx         ;is the adjust up equal to 0?
	jnz     YMajorAdjustDone ;no (don't need to check for odd length,
		 ; because of the above test)
		dec     ecx             ;both conditions met; make initial run 1
				; shorter
YMajorAdjustDone:
		mov     WholeStep,eax   ;whole step (minimum run length)
		mov     eax,_gr_var_color        ;AL = drawing color
		mov     ebx, XAdvance   ;which way X advances

; Draw the first, partial run of pixels.
YMajorFirstLoop:
		mov     [edi],al        ;draw the pixel
		add     edi,ebp ;advance along the major axis (Y)
		dec     ecx
	jnz     YMajorFirstLoop
		add     edi,ebx           ;advance along the minor axis (X)
; Draw all full runs.
		cmp     esi,1            ;# of full runs. Are there more than 2
		; columns, so there are some full runs?
		; (SI = # columns - 1)
	jna     YMajorDrawLast  ;no, no full runs
		dec     edx              ;adjust error term by -1 so we can use
				; carry test
		shr     esi,1            ;convert from column to column-pair count
	jnc     YMajorFullRunsOddEntry  ;if there is an odd number of
					; columns, do the odd column now
YMajorFullRunsLoop:
		mov     ecx,WholeStep   ;run is at least this long
		add     edx,AdjUp       ;advance the error term and add an extra
	jnc     YMajorNoExtra   ; pixel if the error term so indicates
		inc     ecx              ;one extra pixel in run
		sub     edx,AdjDown     ;reset the error term
YMajorNoExtra:
				;draw the run
YMajorRunLoop:
		mov     [edi],al        ;draw the pixel
		add     edi,ebp ;advance along the major axis (Y)
		dec     ecx
	jnz     YMajorRunLoop
		add     edi,ebx         ;advance along the minor axis (X)
YMajorFullRunsOddEntry:         ;enter loop here if there is an odd number
				; of full runs
		mov     ecx,WholeStep   ;run is at least this long
		add     edx,AdjUp       ;advance the error term and add an extra
	jnc     YMajorNoExtra2  ; pixel if the error term so indicates
		inc     ecx              ;one extra pixel in run
		sub     edx, AdjDown    ;reset the error term
YMajorNoExtra2:
				;draw the run
YMajorRunLoop2:
		mov     [edi],al         ;draw the pixel
		add     edi,ebp ;advance along the major axis (Y)
		dec     ecx
	jnz     YMajorRunLoop2
		add     edi,ebx           ;advance along the minor axis (X)

		dec     esi
	jnz     YMajorFullRunsLoop
; Draw the final run of pixels.
YMajorDrawLast:
		pop     ecx              ;get back the final run pixel length
YMajorLastLoop:
		mov     [edi],al         ;draw the pixel
		add     edi,ebp ;advance along the major axis (Y)
		dec     ecx
	jnz     YMajorLastLoop
Done:
	pop ebp
	pop edi
	pop esi  ;restore C register variables
    ret

PUBLIC gr_linear_stosd_

gr_linear_stosd_:

			; EAX -> Destination buffer
			; EDX -> Byte to store
			; EBX -> Number of bytes to move

			push    ecx
			push    edi
			mov     edi, eax
			mov     dh, dl
			mov     ax, dx
			shl     eax, 16
			mov     ax, dx
			cld
			mov     ecx, ebx
			shr     ecx, 2
			rep     stosd
			mov     ecx, ebx
			and     ecx, 011b
			rep     stosb
			pop     edi
			pop     ecx
			ret

PUBLIC gr_update_buffer_

gr_update_buffer_:

		; EAX = Latest source buffer
		; EDX = Earlier source buffer
		; EBX = Dest source buffer
		; ECX = Size of buffer


		push    esi
		push    edi
		push    ebp
		cld

		mov     esi, eax
		mov     edi, edx

		mov     ebp, esi
		shr     ecx, 2

FindNextMismatch:
		repe    cmpsd

		mov     eax, [esi-4]

		mov     edx, ebx
		add     edx, esi
		sub     edx, ebp
		mov     [edx], eax       ; EDX = dest + size - bytes to end

		cmp     ecx, 0
		jne     FindNextMismatch

Done11:
		pop     ebp
		pop     edi
		pop     esi
		ret


;--NOT USED!!-- X0  dd  ?               ;X coordinate of line start point
;--NOT USED!!-- Y0  dd  ?               ;Y coordinate of line start point
;--NOT USED!!-- X1  dd  ?               ;X coordinate of line end point
;--NOT USED!!-- Y1  dd  ?               ;Y coordinate of line end point
;--NOT USED!!-- BaseColor db    ?       ;color # of first color in block used for
;--NOT USED!!--                                                 ; antialiasing, the 100% intensity version of the
;--NOT USED!!--                                                 ; drawing color
;--NOT USED!!-- NumLevels db    16      ;size of color block, with BaseColor+NumLevels-1
;--NOT USED!!--                                                 ; being the 0% intensity version of the drawing color
;--NOT USED!!--                                                 ; (maximum NumLevels = 256)
;--NOT USED!!-- IntensityBits db 4      ;log base 2 of NumLevels; the # of bits used to
;--NOT USED!!--                                                 ; describe the intensity of the drawing color.
;--NOT USED!!--                                                 ; 2**IntensityBits==NumLevels
;--NOT USED!!--                                                 ; (maximum IntensityBits = 8)
;--NOT USED!!-- ; C near-callable function to draw an antialiased line from
;--NOT USED!!-- ; (X0,Y0) to (X1,Y1), in mode 13h, the VGA's standard 320x200 256-color
;--NOT USED!!-- ; mode. Uses an antialiasing approach published by Xiaolin Wu in the July
;--NOT USED!!-- ; 1991 issue of Computer Graphics. Requires that the palette be set up so
;--NOT USED!!-- ; that there are NumLevels intensity levels of the desired drawing color,
;--NOT USED!!-- ; starting at color BaseColor (100% intensity) and followed by (NumLevels-1)
;--NOT USED!!-- ; levels of evenly decreasing intensity, with color (BaseColor+NumLevels-1)
;--NOT USED!!-- ; being 0% intensity of the desired drawing color (black). No clipping is
;--NOT USED!!-- ; performed in DrawWuLine. Handles a maximum of 256 intensity levels per
;--NOT USED!!-- ; antialiased color. This code is suitable for use at screen resolutions,
;--NOT USED!!-- ; with lines typically no more than 1K long; for longer lines, 32-bit error
;--NOT USED!!-- ; arithmetic must be used to avoid problems with fixed-point inaccuracy.
;--NOT USED!!-- ; Tested with TASM 3.0.
;--NOT USED!!-- ;
;--NOT USED!!-- ; C near-callable as:
;--NOT USED!!-- ;   void DrawWuLine(int X0, int Y0, int X1, int Y1, int BaseColor,
;--NOT USED!!-- ;       int NumLevels, unsigned int IntensityBits);
;--NOT USED!!-- 
;--NOT USED!!-- SCREEN_WIDTH_IN_BYTES equ 320   ;# of bytes from the start of one scan line
;--NOT USED!!--                                                                 ; to the start of the next
;--NOT USED!!-- SCREEN_SEGMENT  equ 0a000h  ;segment in which screen memory resides
;--NOT USED!!-- 
;--NOT USED!!-- ; Parameters passed in stack frame.
;--NOT USED!!-- 
;--NOT USED!!-- 
;--NOT USED!!--                 ;_gr_var_color, _gr_var_bitmap, _gr_var_bwidth
;--NOT USED!!-- 
;--NOT USED!!--                 public  gr_linear_aaline_
;--NOT USED!!-- gr_linear_aaline_:
;--NOT USED!!-- 
;--NOT USED!!--                 push    ebp
;--NOT USED!!--                 push    esi  ;preserve C's register variables
;--NOT USED!!--                 push    edi
;--NOT USED!!--                 cld     ;make string instructions increment their pointers
;--NOT USED!!-- 
;--NOT USED!!--                 mov     X0, eax
;--NOT USED!!--                 mov     Y0, edx
;--NOT USED!!--                 mov     X1, ebx
;--NOT USED!!--                 mov     Y1, ecx
;--NOT USED!!-- 
;--NOT USED!!--                 mov     eax, _gr_var_color
;--NOT USED!!--                 mov     BaseColor, al
;--NOT USED!!-- 
;--NOT USED!!-- ; Make sure the line runs top to bottom.
;--NOT USED!!--                 mov     esi,X0
;--NOT USED!!--                 mov     eax,Y0
;--NOT USED!!--                 cmp     eax,Y1       ;swap endpoints if necessary to ensure that
;--NOT USED!!--                 jna     NoSwap      ; Y0 <= Y1
;--NOT USED!!--                 xchg    Y1,eax
;--NOT USED!!--                 mov     Y0,eax
;--NOT USED!!--                 xchg    X1,esi
;--NOT USED!!--                 mov     X0,esi
;--NOT USED!!-- NoSwap:
;--NOT USED!!-- 
;--NOT USED!!-- ; Draw the initial pixel, which is always exactly intersected by the line
;--NOT USED!!-- ; and so needs no weighting.
;--NOT USED!!-- 
;--NOT USED!!--                 mov     edx,_gr_var_bwidth
;--NOT USED!!--                 mul     edx         ;Y0 * SCREEN_WIDTH_IN_BYTES yields the offset
;--NOT USED!!--                                                         ; of the start of the row start the initial
;--NOT USED!!--                                                         ; pixel is on
;--NOT USED!!--                 add     esi,eax         ;point DS:SI to the initial pixel
;--NOT USED!!--                 add     esi,_gr_var_bitmap
;--NOT USED!!--                 mov     al, BaseColor ;color with which to draw
;--NOT USED!!--                 mov     [esi],al     ;draw the initial pixel
;--NOT USED!!-- 
;--NOT USED!!--                 mov     ebx,1        ;XDir = 1; assume DeltaX >= 0
;--NOT USED!!--                 mov     ecx,X1
;--NOT USED!!--                 sub     ecx,X0  ;DeltaX; is it >= 1?
;--NOT USED!!--                 jns     DeltaXSet   ;yes, move left->right, all set
;--NOT USED!!--                                                                 ;no, move right->left
;--NOT USED!!--                 neg     ecx      ;make DeltaX positive
;--NOT USED!!--                 neg     ebx      ;XDir = -1
;--NOT USED!!-- DeltaXSet:
;--NOT USED!!-- 
;--NOT USED!!-- ; Special-case horizontal, vertical, and diagonal lines, which require no
;--NOT USED!!-- ; weighting because they go right through the center of every pixel.
;--NOT USED!!--                 mov     edx,Y1
;--NOT USED!!--                 sub     edx,Y0  ;DeltaY; is it 0?
;--NOT USED!!--                 jnz     NotHorz     ;no, not horizontal
;--NOT USED!!--                                                                 ;yes, is horizontal, special case
;--NOT USED!!--                 and     ebx,ebx       ;draw from left->right?
;--NOT USED!!--                 jns     DoHorz      ;yes, all set
;--NOT USED!!--                 std         ;no, draw right->left
;--NOT USED!!-- DoHorz:
;--NOT USED!!--                 lea     edi,[ebx+esi]  ;point DI to next pixel to draw
;--NOT USED!!--                                                                 ;point ES:DI to next pixel to draw
;--NOT USED!!--                 mov     al,BaseColor ;color with which to draw
;--NOT USED!!--                                                                 ;CX = DeltaX at this point
;--NOT USED!!--                 rep     stosb       ;draw the rest of the horizontal line
;--NOT USED!!--                 cld                 ;restore default direction flag
;--NOT USED!!--                 jmp     DoneAA        ;and we're done
;--NOT USED!!-- 
;--NOT USED!!-- NotHorz:
;--NOT USED!!--                 and     ecx,ecx       ;is DeltaX 0?
;--NOT USED!!--                 jnz     NotVert     ;no, not a vertical line
;--NOT USED!!--                                                                 ;yes, is vertical, special case
;--NOT USED!!--                 mov     al,BaseColor ;color with which to draw
;--NOT USED!!-- VertLoop:
;--NOT USED!!--                 add     esi,_gr_var_bwidth ;point to next pixel to draw
;--NOT USED!!--                 mov     [esi], al     ;draw the next pixel
;--NOT USED!!--                 dec     edx           ;--DeltaY
;--NOT USED!!--                 jnz     VertLoop
;--NOT USED!!--                 jmp     DoneAA        ;and we're done
;--NOT USED!!-- 
;--NOT USED!!-- NotVert:
;--NOT USED!!--                 cmp     ecx,edx       ;DeltaX == DeltaY?
;--NOT USED!!--                 jnz     NotDiag     ;no, not diagonal
;--NOT USED!!--                                                                 ;yes, is diagonal, special case
;--NOT USED!!--                 mov     al,BaseColor ;color with which to draw
;--NOT USED!!-- DiagLoop:
;--NOT USED!!--                 lea     esi,[esi+ebx]
;--NOT USED!!--                 add     esi,_gr_var_bwidth
;--NOT USED!!--                                                                 ;advance to next pixel to draw by
;--NOT USED!!--                                                                 ; incrementing Y and adding XDir to X
;--NOT USED!!--                 mov     [esi],al     ;draw the next pixel
;--NOT USED!!--                 dec     edx      ;--DeltaY
;--NOT USED!!--                 jnz     DiagLoop
;--NOT USED!!--                 jmp     DoneAA        ;and we're done
;--NOT USED!!-- 
;--NOT USED!!-- ; Line is not horizontal, diagonal, or vertical.
;--NOT USED!!-- NotDiag:
;--NOT USED!!-- ; Is this an X-major or Y-major line?
;--NOT USED!!--                 cmp     edx,ecx
;--NOT USED!!--                 jb      XMajorAA          ;it's X-major
;--NOT USED!!-- 
;--NOT USED!!-- ; It's a Y-major line. Calculate the 16-bit fixed-point fractional part of a
;--NOT USED!!-- ; pixel that X advances each time Y advances 1 pixel, truncating the result
;--NOT USED!!-- ; to avoid overrunning the endpoint along the X axis.
;--NOT USED!!--                 xchg    edx,ecx       ;DX = DeltaX, CX = DeltaY
;--NOT USED!!--                 sub     ax,ax       ;make DeltaX 16.16 fixed-point value in DX:AX
;--NOT USED!!--                 div     cx      ;AX = (DeltaX << 16) / DeltaY. Won't overflow
;--NOT USED!!--                                                                 ; because DeltaX < DeltaY
;--NOT USED!!--                 mov     di,cx       ;DI = DeltaY (loop count)
;--NOT USED!!--                 sub     esi,ebx       ;back up the start X by 1, as explained below
;--NOT USED!!--                 mov     dx,-1       ;initialize the line error accumulator to -1,
;--NOT USED!!--                                                                 ; so that it will turn over immediately and
;--NOT USED!!--                                                                 ; advance X to the start X. This is necessary
;--NOT USED!!--                                                                 ; properly to bias error sums of 0 to mean
;--NOT USED!!--                                                                 ; "advance next time" rather than "advance
;--NOT USED!!--                                                                 ; this time," so that the final error sum can
;--NOT USED!!--                                                                 ; never cause drawing to overrun the final X
;--NOT USED!!--                                                                 ; coordinate (works in conjunction with
;--NOT USED!!--                                                                 ; truncating ErrorAdj, to make sure X can't
;--NOT USED!!--                                                                 ; overrun)
;--NOT USED!!--                 mov     cl,8           ;CL = # of bits by which to shift
;--NOT USED!!--                 sub     cl,IntensityBits   ; ErrorAcc to get intensity level (8
;--NOT USED!!--                                                                                 ; instead of 16 because we work only
;--NOT USED!!--                                                                                 ; with the high byte of ErrorAcc)
;--NOT USED!!--                 mov     ch,NumLevels ;mask used to flip all bits in an
;--NOT USED!!--                 dec     ch             ; intensity weighting, producing
;--NOT USED!!--                                                                                    ; result (1 - intensity weighting)
;--NOT USED!!--                 mov     bp, ax
;--NOT USED!!--                 mov     al, BaseColor
;--NOT USED!!--                          ;BP = ErrorAdj, AL = BaseColor,
;--NOT USED!!--                          ; AH = scratch register
;--NOT USED!!-- 
;--NOT USED!!-- 
;--NOT USED!!-- 
;--NOT USED!!-- ; Draw all remaining pixels.
;--NOT USED!!-- YMajorLoop:
;--NOT USED!!--                 add     dx,bp           ;calculate error for next pixel
;--NOT USED!!--                         jnc     NoXAdvance              ;not time to step in X yet
;--NOT USED!!--                                                                                                                                                                 ;the error accumulator turned over,
;--NOT USED!!--                                                                                                                                                                 ; so advance the X coord
;--NOT USED!!--                                 add     esi,ebx                   ;add XDir to the pixel pointer
;--NOT USED!!-- NoXAdvance:
;--NOT USED!!--                                 add     esi,_gr_var_bwidth ;Y-major, so always advance Y
;--NOT USED!!-- 
;--NOT USED!!-- ; The IntensityBits most significant bits of ErrorAcc give us the intensity
;--NOT USED!!-- ; weighting for this pixel, and the complement of the weighting for the
;--NOT USED!!-- ; paired pixel.
;--NOT USED!!--                 mov     ah,dh   ;msb of ErrorAcc
;--NOT USED!!--                 shr     ah,cl   ;Weighting = ErrorAcc >> IntensityShift;
;--NOT USED!!--                 ;add     ah,al   ;BaseColor + Weighting
;--NOT USED!!-- 
;--NOT USED!!--                 ; Make ah =   _gr_fade_table + BaseColor+(15-ah)*256
;--NOT USED!!--                 push    ecx
;--NOT USED!!--                 push    ebx
;--NOT USED!!--                 mov     ebx, 15
;--NOT USED!!--                 sub     bl, ah
;--NOT USED!!--                 shl     ebx, 8
;--NOT USED!!--                 movzx   ecx, BaseColor
;--NOT USED!!--                 add     ebx, ecx
;--NOT USED!!--                 add     ebx, offset _gr_fade_table
;--NOT USED!!--                 mov     ah, [ebx]
;--NOT USED!!--                 pop     ebx
;--NOT USED!!--                 pop     ecx
;--NOT USED!!-- 
;--NOT USED!!--                 mov     [esi],ah ;DrawPixel(X, Y, BaseColor + Weighting);
;--NOT USED!!-- 
;--NOT USED!!--                 mov     ah,dh   ;msb of ErrorAcc
;--NOT USED!!--                 shr     ah,cl   ;Weighting = ErrorAcc >> IntensityShift;
;--NOT USED!!--                 xor     ah,ch   ;Weighting ^ WeightingComplementMask
;--NOT USED!!--                 ;add     ah,al   ;BaseColor + (Weighting ^ WeightingComplementMask)
;--NOT USED!!-- 
;--NOT USED!!--                 ; Make ah =   _gr_fade_table + BaseColor+(15-ah)*256
;--NOT USED!!--                 push    ecx
;--NOT USED!!--                 push    ebx
;--NOT USED!!--                 mov     ebx, 15
;--NOT USED!!--                 sub     bl, 0
;--NOT USED!!--                 shl     ebx, 8
;--NOT USED!!--                 movzx   ecx, BaseColor
;--NOT USED!!--                 add     ebx, ecx
;--NOT USED!!--                 add     ebx, offset _gr_fade_table
;--NOT USED!!--                 mov     ah, [ebx]
;--NOT USED!!--                 pop     ebx
;--NOT USED!!--                 pop     ecx
;--NOT USED!!-- 
;--NOT USED!!-- 
;--NOT USED!!--                 mov     [esi+ebx],ah ;DrawPixel(X+XDir, Y,
;--NOT USED!!--                                                 ; BaseColor + (Weighting ^ WeightingComplementMask));
;--NOT USED!!--                 dec     di  ;--DeltaY
;--NOT USED!!--                 jnz     YMajorLoop
;--NOT USED!!--                 jmp     DoneAA    ;we're done with this line
;--NOT USED!!-- 
;--NOT USED!!-- ; It's an X-major line.
;--NOT USED!!-- XMajorAA:
;--NOT USED!!-- ; Calculate the 16-bit fixed-point fractional part of a pixel that Y advances
;--NOT USED!!-- ; each time X advances 1 pixel, truncating the result to avoid overrunning
;--NOT USED!!-- ; the endpoint along the X axis.
;--NOT USED!!--                 sub     eax,eax       ;make DeltaY 16.16 fixed-point value in DX:AX
;--NOT USED!!--                 div     ecx      ;AX = (DeltaY << 16) / Deltax. Won't overflow
;--NOT USED!!--                                                                 ; because DeltaY < DeltaX
;--NOT USED!!--                 mov     edi,ecx       ;DI = DeltaX (loop count)
;--NOT USED!!--                 sub     esi,_gr_var_bwidth ;back up the start X by 1, as
;--NOT USED!!--                                                                 ; explained below
;--NOT USED!!--                 mov     edx,-1       ;initialize the line error accumulator to -1,
;--NOT USED!!--                                                                 ; so that it will turn over immediately and
;--NOT USED!!--                                                                 ; advance Y to the start Y. This is necessary
;--NOT USED!!--                                                                 ; properly to bias error sums of 0 to mean
;--NOT USED!!--                                                                 ; "advance next time" rather than "advance
;--NOT USED!!--                                                                 ; this time," so that the final error sum can
;--NOT USED!!--                                                                 ; never cause drawing to overrun the final Y
;--NOT USED!!--                                                                 ; coordinate (works in conjunction with
;--NOT USED!!--                                                                 ; truncating ErrorAdj, to make sure Y can't
;--NOT USED!!--                                                                 ; overrun)
;--NOT USED!!--                 mov     cl,8            ;CL = # of bits by which to shift
;--NOT USED!!--                 sub     cl,IntensityBits   ; ErrorAcc to get intensity level (8
;--NOT USED!!--                                                                                 ; instead of 16 because we work only
;--NOT USED!!--                                                                                 ; with the high byte of ErrorAcc)
;--NOT USED!!--                 mov     ch,NumLevels ;mask used to flip all bits in an
;--NOT USED!!--                 dec     ch             ; intensity weighting, producing
;--NOT USED!!--                                                                                    ; result (1 - intensity weighting)
;--NOT USED!!-- 
;--NOT USED!!--                 mov     ebp, eax
;--NOT USED!!--                 mov     al, BaseColor
;--NOT USED!!-- 
;--NOT USED!!-- ; Draw all remaining pixels.
;--NOT USED!!-- XMajorLoop:
;--NOT USED!!--                 add     edx,ebp           ;calculate error for next pixel
;--NOT USED!!--                         jnc     NoYAdvance              ;not time to step in Y yet
;--NOT USED!!--                                                                                                                                                                 ;the error accumulator turned over,
;--NOT USED!!--                                                                                                                                                                 ; so advance the Y coord
;--NOT USED!!--                                 add     esi,_gr_var_bwidth ;advance Y
;--NOT USED!!-- NoYAdvance:
;--NOT USED!!--                                 add     esi,ebx       ;X-major, so add XDir to the pixel pointer
;--NOT USED!!-- 
;--NOT USED!!-- ; The IntensityBits most significant bits of ErrorAcc give us the intensity
;--NOT USED!!-- ; weighting for this pixel, and the complement of the weighting for the
;--NOT USED!!-- ; paired pixel.
;--NOT USED!!--                 mov     ah,dh   ;msb of ErrorAcc
;--NOT USED!!--                 shr     ah,cl   ;Weighting = ErrorAcc >> IntensityShift;
;--NOT USED!!--                 ;add     ah,al   ;BaseColor + Weighting
;--NOT USED!!-- 
;--NOT USED!!--                 ; Make ah =   _gr_fade_table + BaseColor+(15-ah)*256
;--NOT USED!!--                 push    ecx
;--NOT USED!!--                 push    ebx
;--NOT USED!!--                 mov     ebx, 15
;--NOT USED!!--                 sub     bl, ah
;--NOT USED!!--                 shl     ebx, 8
;--NOT USED!!--                 movzx   ecx, BaseColor
;--NOT USED!!--                 add     ebx, ecx
;--NOT USED!!--                 add     ebx, offset _gr_fade_table
;--NOT USED!!--                 mov     ah, [ebx]
;--NOT USED!!--                 pop     ebx
;--NOT USED!!--                 pop     ecx
;--NOT USED!!-- 
;--NOT USED!!-- 
;--NOT USED!!--                 mov     [esi],ah ;DrawPixel(X, Y, BaseColor + Weighting);
;--NOT USED!!--                 mov     ah,dh   ;msb of ErrorAcc
;--NOT USED!!--                 shr     ah,cl   ;Weighting = ErrorAcc >> IntensityShift;
;--NOT USED!!--                 xor     ah,ch   ;Weighting ^ WeightingComplementMask
;--NOT USED!!--                 ;add     ah,al   ;BaseColor + (Weighting ^ WeightingComplementMask)
;--NOT USED!!--                 add     esi, _gr_var_bwidth
;--NOT USED!!-- 
;--NOT USED!!--                 ; Make ah =   _gr_fade_table + BaseColor+(15-ah)*256
;--NOT USED!!--                 push    ecx
;--NOT USED!!--                 push    ebx
;--NOT USED!!--                 mov     ebx, 15
;--NOT USED!!--                 sub     bl, ah
;--NOT USED!!--                 shl     ebx, 8
;--NOT USED!!--                 movzx   ecx, BaseColor
;--NOT USED!!--                 add     ebx, ecx
;--NOT USED!!--                 add     ebx, offset _gr_fade_table
;--NOT USED!!--                 mov     ah, [ebx]
;--NOT USED!!--                 pop     ebx
;--NOT USED!!--                 pop     ecx
;--NOT USED!!-- 
;--NOT USED!!-- 
;--NOT USED!!--                 mov     [esi],ah    ;DrawPixel(X, Y+SCREEN_WIDTH_IN_BYTES,
;--NOT USED!!--                                                 ; BaseColor + (Weighting ^ WeightingComplementMask));
;--NOT USED!!--                 sub     esi, _gr_var_bwidth
;--NOT USED!!--                 dec     edi  ;--DeltaX
;--NOT USED!!--                 jnz     XMajorLoop
;--NOT USED!!-- 
;--NOT USED!!-- DoneAA:               ;we're done with this line
;--NOT USED!!--                 pop edi  ;restore C's register variables
;--NOT USED!!--                 pop esi
;--NOT USED!!--                 pop ebp  ;restore caller's stack frame
;--NOT USED!!--                 ret     ;done



_TEXT   ENDS


		END
