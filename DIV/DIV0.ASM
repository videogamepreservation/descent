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
; $Source: f:/miner/source/div/rcs/div0.asm $
; $Revision: 1.4 $
; $Author: john $
; $Date: 1995/02/07 17:55:58 $
;
; Divide by zero error handler functions
;
; $Log: div0.asm $
; Revision 1.4  1995/02/07  17:55:58  john
; Made the debug counter variables public.
; 
; Revision 1.3  1994/01/14  15:34:12  john
; Added counters for number of overflows.
; 
; Revision 1.2  1993/10/26  17:25:45  john
; fixed bug in init, cx was being trashed.
; 
; Revision 1.1  1993/09/17  12:37:51  john
; Initial revision
; 
;
;



; DIV0.ASM   - Provides routines to capture divide overflow exceptions.
;              See DIV0.H for calling parameters.

.386



_DATA   SEGMENT BYTE PUBLIC USE32 'DATA'

		MAX_SIZE EQU 100

		Old_Ex_Sel      dw ?            ; Old Selector
		Old_Ex_Off      dd ?            ; Old offset

		Already_Init    dd  0           ; Equal to 1 when installed

		DefaultMode dd  0               ; What to do when not in list.

		; Callback List
		CB_Size     dw  0
		CB_Source   dd  MAX_SIZE DUP (0)
		CB_Dest     dd  MAX_SIZE DUP (0)

		; Saturation List
		SAT_Size    dw  0
		SAT_Source  dd  MAX_SIZE DUP (0)


	       	PUBLIC _div0_num_handled_by_cblist
		PUBLIC _div0_num_handled_by_satlist
		PUBLIC _div0_num_saturated

	       	_div0_num_handled_by_cblist	dd	0
		_div0_num_handled_by_satlist	dd	0
		_div0_num_saturated		dd	0

_DATA   ENDS

DGROUP  GROUP _DATA


_TEXT   SEGMENT BYTE PUBLIC USE32 'CODE'

		ASSUME  DS:_DATA
		ASSUME  CS:_TEXT

DivideByZeroException:

PREFIX_ADRSIZ   EQU 01100111b
PREFIX_OPSIZE   EQU 01100110b
PREFIX_LOCK     EQU 11110000b
PREFIX_CS       EQU 00101110b
PREFIX_DS       EQU 00111110b
PREFIX_ES       EQU 00100110b
PREFIX_FS       EQU 01100100b
PREFIX_GS       EQU 01100101b
PREFIX_SS       EQU 00110110b
OPCODE_NOP      EQU 10010000b


OVERFLOW_8BIT_UNSIGNED EQU 0FFh
OVERFLOW_16BIT_UNSIGNED EQU 0FFFFh
OVERFLOW_32BIT_UNSIGNED EQU 0FFFFFFFFh

OVERFLOW_8BIT_NEGATIVE EQU -128
OVERFLOW_16BIT_NEGATIVE EQU -32768
OVERFLOW_32BIT_NEGATIVE EQU -2147483648


OVERFLOW_8BIT_POSITIVE EQU 127
OVERFLOW_16BIT_POSITIVE EQU 32767
OVERFLOW_32BIT_POSITIVE EQU 2147483647



SAVED_REGS  EQU 32
SAVED_EAX   EQU DWORD PTR SS:[ESP+0]         ; 4

SAVED_AL    EQU BYTE PTR SS:[ESP+0]
SAVED_AH    EQU BYTE PTR SS:[ESP+1]
SAVED_AX    EQU WORD PTR SS:[ESP+0]

SAVED_EBX   EQU DWORD PTR SS:[ESP+4]         ; 4

SAVED_BL    EQU BYTE PTR SS:[ESP+4]
SAVED_BH    EQU BYTE PTR SS:[ESP+5]
SAVED_BX    EQU WORD PTR SS:[ESP+4]

SAVED_ECX   EQU DWORD PTR SS:[ESP+8]         ; 4

SAVED_CL    EQU BYTE PTR SS:[ESP+8]
SAVED_CH    EQU BYTE PTR SS:[ESP+9]
SAVED_CX    EQU WORD PTR SS:[ESP+8]

SAVED_EDX   EQU DWORD PTR SS:[ESP+12]        ; 4

SAVED_DL    EQU BYTE PTR SS:[ESP+12]
SAVED_DH    EQU BYTE PTR SS:[ESP+13]
SAVED_DX    EQU WORD PTR SS:[ESP+12]

SAVED_ESI   EQU DWORD PTR SS:[ESP+16]        ; 4
SAVED_SI    EQU WORD PTR SS:[ESP+16]

SAVED_EDI   EQU DWORD PTR SS:[ESP+20]        ; 4
SAVED_DI    EQU WORD PTR SS:[ESP+20]

SAVED_ES    EQU WORD PTR SS:[ESP+24]        ; 4

SAVED_DS    EQU WORD PTR SS:[ESP+28]        ; 4

BAD_EIP     EQU DWORD PTR SS:[ESP+0Ch+SAVED_REGS]
BAD_CS      EQU WORD PTR SS:[ESP+10h+SAVED_REGS]
SAVED_ESP   EQU DWORD PTR SS:[ESP+18h+SAVED_REGS]
SAVED_SP    EQU WORD PTR SS:[ESP+18h+SAVED_REGS]
SAVED_SS    EQU WORD PTR SS:[ESP+1Ch+SAVED_REGS]

		;int 3       ; Bypass by typing:  /eip++

		push    ds
		push    es
		push    edi
		push    esi
		push    edx
		push    ecx
		push    ebx
		push    eax

		mov     ax, DGROUP
		mov     ds, ax

		mov     eax, BAD_EIP

		xor     ecx, ecx

NextCB:
		cmp     cx, CB_Size
		je      CheckSatList
		cmp     CB_Source[ecx*4], eax
		jne     NotIt
		; Found the right Call Back item
		inc	_div0_num_handled_by_cblist
		mov     ecx, CB_Dest[ecx*4]
		mov     BAD_EIP, ecx
		jmp     NormalReturn
NotIt:
		inc     cx
		jmp     NextCB



CheckSatList:
		xor     ecx, ecx
NextSAT:
		cmp     cx, SAT_Size
		je      UseDefaultAction
		cmp     SAT_Source[ecx*4], eax
		jne     NotIt1
		; Found the right Saturation item
		inc	_div0_num_handled_by_satlist

		jmp     SaturateIt
NotIt1:
		inc     cx
		jmp     NextCB



UseDefaultAction:
		cmp     DefaultMode, 1
		je      DefaultSaturate

		;======  Return Error Code. =============
		;int     3               ; Pop into debugger if active
		call    div0_close_
		jmp     NormalReturn

DefaultSaturate:
		; This is hit when mode=DM_SATURATE and the instruction
		; isn't in any callback or saturate lists. For your handy debugging
		; information, the instruction that caused this exception is at
		; BX:EAX.
		inc	_div0_num_saturated 

		mov     eax, BAD_EIP
		mov     bx, BAD_CS
		int     3

SaturateIt:

		xor     ebx, ebx
		xor     ecx, ecx
		xor     esi, esi
		xor     edi, edi

		mov     es, SAVED_DS

		mov     eax, BAD_EIP

		; EAX = Pointer to Div instruction
		; BH = temp byte holder
		; BL = temp byte holder
		; CH = Bits 0=8bit, 1=16bit, 2=32bit, 3=DIV
		; CL = Temp bit shifter
		; EDX = temp 32-bit holder


PrefixCheck:
		mov     bl, BYTE PTR CS:[eax]
		cmp     bl, PREFIX_OPSIZE
		je      OperandSize
		cmp     bl, PREFIX_ADRSIZ
		je      OverPrefix
		cmp     bl, PREFIX_LOCK
		je      OverPrefix
		cmp     bl, PREFIX_CS
		je      PrefixCS
		cmp     bl, PREFIX_DS
		je      PrefixDS
		cmp     bl, PREFIX_ES
		je      PrefixES
		cmp     bl, PREFIX_FS
		je      PrefixFS
		cmp     bl, PREFIX_GS
		je      PrefixGS
		cmp     bl, PREFIX_SS
		je      PrefixSS
		jmp     DoneWithPrefixes

OverPrefix:
		inc     eax
		jmp     PrefixCheck

PrefixCS:
		mov     es, BAD_CS
		jmp     OverPrefix

PrefixDS:
		push    ds
		pop     es
		jmp     OverPrefix

PrefixES:
		mov     es, SAVED_ES

PrefixFS:
		push    fs
		pop     es
		jmp     OverPrefix

PrefixGS:
		push    gs
		pop     es
		jmp     OverPrefix

PrefixSS:
		mov     es, SAVED_SS
		jmp     OverPrefix

OperandSize:
		or      ch, 10b     ; Flag 16-bit
		jmp     OverPrefix

DoneWithPrefixes:

		; Check for the divide opcode.
		cmp     bl, 0F7h
		je      OP32OR16BIT

		cmp     bl, 0F6h
		je      OP8BIT

		; Something is wrong!!!!! This should never be reached!!!
		pop     eax
		pop     ebx
		pop     ecx
		pop     edx
		pop     esi
		pop     edi
		pop     es
		pop     ds

		int     3
		retf


OP32OR16BIT:
		test    ch, 10b
		jnz     OP16BIT

OP32BIT:
		or      ch, 100b    ; Flag 32-bit
		jmp     GotOpSize

OP8BIT:
		or      ch, 1b      ; Flag 8-bit
		jmp     GotOpSize

OP16BIT:


GotOpSize:
		inc     eax     ; We should be pointing to the mod,111,r/m byte
		mov     bl, BYTE PTR CS:[eax]
		inc     eax

		mov     bh, bl
		shr     bh, 6       ; BH = mod bits

		test    bl, 001000b
		;           110 = Div
		;           111 = iDiv
		jz      IsDIV
		; This is a signed division
		or      ch, 1000b   ; Flag Signed
IsDIV:
		and     bl, 111b    ; BL = r/m bits

		cmp     bh, 11b
		je      MOD_11


DoRMbits:

		cmp     bl, 000b
		je      RM_000
		cmp     bl, 001b
		je      RM_001
		cmp     bl, 010b
		je      RM_010
		cmp     bl, 011b
		je      RM_011
		cmp     bl, 100b
		je      RM_100
		cmp     bl, 101b
		je      RM_101
		cmp     bl, 110b
		je      RM_110
		jmp     RM_111

RM_000:
		add     esi, SAVED_EAX
		jmp     doneRM

RM_001:
		add     esi, SAVED_ECX
		jmp     doneRM

RM_010:
		add     esi, SAVED_EDX
		jmp     doneRM

RM_011:
		add     esi, SAVED_EBX
		jmp     doneRM

RM_100:
		jmp     RM_SIB


RM_101:
		cmp     bh, 00b
		je      DSdisp32

		add     esi, ebp
		mov     es, SAVED_SS
		jmp     doneRM

DSdisp32:
		mov     bh, 10b
		jmp     doneRM

RM_110:
		add     esi, SAVED_ESI
		jmp     doneRM

RM_111:
		add     esi, SAVED_EDI
		jmp     doneRM


RM_SIB:
		mov     bl, BYTE PTR CS:[eax]   ; BL = s-i-b byte

		mov     cl, bl
		shl     cl, 6

		shl     bl, 3
		and     bl, 111b

		cmp     bl, 000b
		je      INDEX_EAX
		cmp     bl, 001b
		je      INDEX_ECX
		cmp     bl, 010b
		je      INDEX_EDX
		cmp     bl, 011b
		je      INDEX_EBX
		cmp     bl, 100b
		je      doneSIB1
		cmp     bl, 101b
		je      INDEX_EBP
		cmp     bl, 110b
		je      INDEX_ESI
		jmp     INDEX_EDI


INDEX_EAX:
		mov     edx, SAVED_EAX
		jmp     doneSIB
INDEX_ECX:
		mov     edx, SAVED_ECX
		jmp     doneSIB
INDEX_EDX:
		mov     edx, SAVED_EDX
		jmp     doneSIB
INDEX_EBX:
		mov     edx, SAVED_EBX
		jmp     doneSIB
INDEX_EBP:
		mov     edx, ebp
		jmp     doneSIB
INDEX_ESI:
		mov     edx, SAVED_ESI
		jmp     doneSIB
INDEX_EDI:
		mov     edx, SAVED_EDI
		jmp     doneSIB

doneSIB:
		shl     edx, cl
		add     esi, edx


doneSIB1:
		mov     bl, BYTE PTR CS:[eax]   ; BL = s-i-b byte
		inc     eax

		mov     bh, bl
		shl     bh, 6
		and     bl, 111b
		jmp     DoRMbits


doneRM: ;BH = mod

		cmp     bh, 00b
		je      MOD_00
		cmp     bh, 01b
		je      MOD_01
		cmp     bh, 10b
		je      MOD_10
		jmp     MOD_11


MOD_00:     ; No displacement
		jmp     doneMOD

MOD_01: ; disp8
		movsx   edx, BYTE PTR CS:[eax]
		add     eax, 1
		add     esi, edx
		jmp     doneMOD

MOD_10:
		mov     edx, DWORD PTR CS:[eax]
		add     eax, 4
		add     esi, edx
		jmp     doneMOD

MOD_11: cmp     bl, 000b
		je      REG_000
		cmp     bl, 001b
		je      REG_001
		cmp     bl, 010b
		je      REG_010
		cmp     bl, 011b
		je      REG_011
		cmp     bl, 100b
		je      REG_100
		cmp     bl, 101b
		je      REG_101
		cmp     bl, 110b
		je      REG_110
		jmp     REG_111


REG_000:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_AL
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_AX
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_EAX
		jmp     GotValue

REG_001:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_CL
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_CX
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_ECX
		jmp     GotValue
REG_010:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_DL
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_DX
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_EDX
		jmp     GotValue
REG_011:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_BL
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_BX
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_EDX
		jmp     GotValue
REG_100:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_AH
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx    edx,    SAVED_SP
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_ESP
		jmp     GotValue
REG_101:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_CH
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    bp
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    ebp
		jmp     GotValue
REG_110:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_DH
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_SI
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_ESI
		jmp     GotValue
REG_111:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_BH
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    SAVED_DI
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    SAVED_EDI
		jmp     GotValue


doneMOD:
		test    ch, 1b              ; Check if 8-bit
		jz      @f                  ; Skip if not
		movsx   edx,    BYTE PTR ES:[esi]
@@:     test    ch, 10b             ; Check if 16-bit
		jz      @f                  ; Skip if not
		movsx   edx,    WORD PTR ES:[esi]
@@:     test    ch, 100b            ; Check if 32-bit
		jz      GotValue            ; skip if not
		mov     edx,    DWORD PTR ES:[esi]

GotValue:
		; EAX = Pointer to instruction right after DIV
		; CH = Bits 0=8bit, 1=16bit, 2=32bit, 3=DIV
		; EDX = 32-bit sign extended divisor (source operand)

		mov     BAD_EIP, eax    ; Point EIP to next instruction


		; if size=8  then AL=1..., AH=1...
		; if size=16 then AX=1..., DX=1...
		; if size=32 then EAX=1..., EDX=1...
		;
		; resultsign = - (Assume negative or unsigned)
		;
		; If signed then
		;    if size=8 then  resultsign = sign(ah:al) * sign(edx)
		;    if size=16 then resultsign = sign(dx:ax) * sign(edx)
		;    if size=32 then resultsign = sign(dx:ax) * sign(edx)
		;
		; if resultsign == + then
		;    if size=8 then  AL = 01...
		;    if size=16 then AX = 01...
		;    if size=32 then EAX = 01...
		;

		test    ch, 1000b
		jz      UnSigned

		shr     edx, 31

		; Signed return
		test    ch, 1b              ; Check if 8-bit
		jnz     Get8Sign
		test    ch, 10b             ; Check if 16-bit
		jnz     Get16Sign
		jmp     Get32Sign         ; Else is 32-bit

Get8Sign:
		mov     ah, SAVED_AH
		shr     ah, 7           ; AH = sign of divisor
		xor     ah, dl
		jnz     Neg8Return
		jmp     Pos8Return

Get16Sign:
		mov     ah, SAVED_DH
		shr     ah, 7           ; AH = sign of divisor
		xor     ah, dl
		jnz     Neg16Return
		jmp     Pos16Return

Get32Sign:
		mov     eax, SAVED_EDX
		shr     eax, 31           ; AL = sign of divisor
		xor     al, dl
		jnz     Neg32Return
		jmp     Pos32Return

UnSigned: ; We need to find the sign
		; Unsigned div return
		test    ch, 1b              ; Check if 8-bit
		jnz     Unsigned8Return
		test    ch, 10b             ; Check if 16-bit
		jnz     Unsigned16Return
		jmp     Unsigned32Return         ; Else is 32-bit


NormalReturn:
		pop     eax
		pop     ebx
		pop     ecx
		pop     edx
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf


Neg8Return:
		pop     eax
		mov     al, OVERFLOW_8BIT_NEGATIVE
		mov     ah, 0       ; Remainder = 0
		pop     ebx
		pop     ecx
		pop     edx
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Neg16Return:
		pop     eax
		mov     ax, OVERFLOW_16BIT_NEGATIVE
		pop     ebx
		pop     ecx
		pop     edx
		mov     dx, 0       ; Remainder = 0
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Neg32Return:
		pop     eax
		mov     eax, OVERFLOW_32BIT_NEGATIVE
		pop     ebx
		pop     ecx
		pop     edx
		mov     edx, 0          ; Remainder = 0
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf


Pos8Return:
		pop     eax
		mov     al, OVERFLOW_8BIT_POSITIVE
		mov     ah, 0           ; Remainder = 0
		pop     ebx
		pop     ecx
		pop     edx
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Pos16Return:
		pop     eax
		mov     ax, OVERFLOW_16BIT_POSITIVE
		pop     ebx
		pop     ecx
		pop     edx
		mov     dx, 0           ; Remainder = 0
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Pos32Return:
		pop     eax
		mov     eax, OVERFLOW_32BIT_POSITIVE
		pop     ebx
		pop     ecx
		pop     edx
		mov     edx, 0          ; Remainder = 0
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Unsigned8Return:
		pop     eax
		mov     al, OVERFLOW_8BIT_UNSIGNED
		mov     ah, 0       ; Remainder = 0
		pop     ebx
		pop     ecx
		pop     edx
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Unsigned16Return:
		pop     eax
		mov     ax, OVERFLOW_16BIT_UNSIGNED
		pop     ebx
		pop     ecx
		pop     edx
		mov     dx, 0       ; Remainder = 0
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf

Unsigned32Return:
		pop     eax
		mov     eax, OVERFLOW_32BIT_UNSIGNED
		pop     ebx
		pop     ecx
		pop     edx
		mov     edx, 0          ; Remainder = 0
		pop     esi
		pop     edi
		pop     es
		pop     ds
		retf


PUBLIC  div0_close_

div0_close_:
		push    eax
		push    ecx
		push    edx

		mov     Already_Init, 0

		mov     eax, 0203h
		mov     bl, 0
		mov     cx, Old_Ex_Sel
		mov     edx, Old_Ex_Off
		int     31h

		pop     edx
		pop     ecx
		pop     eax

		ret





PUBLIC  div0_set_saturate_

div0_set_saturate_:
		; EAX = div_addr

		push    ecx

		xor     ecx, ecx
		mov     cx, SAT_Size
		inc     cx
		cmp     cx, MAX_SIZE
		jae     TooMany1
		mov     SAT_Size, cx

		dec     ecx
		mov     SAT_Source[ecx*4], eax

		mov     eax, 1
		pop     ecx
		ret

TooMany1:
		mov     eax, 0
		pop     ecx
		ret


PUBLIC  div0_set_handler_

div0_set_handler_:
		; EAX = div_addr
		; EDX = handler_addr
		push    ecx

		xor     ecx, ecx
		mov     cx, CB_Size
		inc     cx
		cmp     cx, MAX_SIZE
		jae     TooMany
		mov     CB_Size, cx

		dec     ecx
		mov     CB_Source[ecx*4], eax
		mov     CB_Dest[ecx*4], edx

		mov     eax, 1
		pop     ecx
		ret

TooMany:
		mov     eax, 0
		pop     ecx
		ret



PUBLIC  div0_set_mode_

div0_set_mode_:
		and     eax, 1
		mov     DefaultMode, eax
		ret


PUBLIC  div0_init_

div0_init_:
		push    ds
		push    ebx
		push    ecx
		push    edx

		cmp     Already_Init, 1
		je      AlreadyInstalled

		mov     Already_Init, 1

	       	mov	_div0_num_handled_by_cblist, 0
		mov	_div0_num_handled_by_satlist, 0
		mov	_div0_num_saturated, 0

		and     eax, 1
		mov     DefaultMode, eax

		mov     SAT_Size, 0
		mov     CB_Size, 0

		mov     eax, 0202h
		mov     bl, 0
		int     31h
		jc      ToBadSoSadItFailed
		mov     Old_Ex_Sel,cx
		mov     Old_Ex_Off,edx

		mov     eax, 0203h
		mov     bl, 0
		mov     cx, cs
		mov     edx, offset DivideByZeroException
		int     31h
		jc      ToBadSoSadItFailed

AlreadyInstalled:
		mov     eax, 1
		pop     edx
		pop     ecx
		pop     ebx
		pop     ds
		ret

ToBadSoSadItFailed:
		mov     eax, 0
		pop     edx
		pop     ebx
		pop     ds
		ret

_TEXT   ENDS


		END



