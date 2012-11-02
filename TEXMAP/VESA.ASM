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
; $Source: f:/miner/source/texmap/rcs/vesa.asm $
; $Revision: 1.1 $
; $Author: mike $
; $Date: 1993/09/08 17:29:56 $
;
; Routine to copy buffer to VESA memory.
;
; $Log: vesa.asm $
; Revision 1.1  1993/09/08  17:29:56  mike
; Initial revision
; 
;
;

.386

INCLUDE VGAREGS.INC


_DATA   SEGMENT BYTE PUBLIC USE32 'DATA'


		BufferPtr   dd  ?
		BufferSeg   dw  ?
		GoalMode    dw  ?
		LastPage    dd  0FFFFFFFFh

		; Information from VESA return SuperVGA Information

		VESAVersion         dw  ?
		OEMStringPtr        dd  ?
		Capabilities        dd  ?
		VideoModePtr        dd  ?
		TotalMemory         dw  ?
		WinGranularity      dw  ?
		WinSize             dw  ?
		WinFuncPtr          dd  ?
		BytesPerScanLine    dd  ?
		PageSizeShift       db  ?

		VESA_Signature      = 041534556h

REALREGS    STRUCT
		RealEDI     dd      ?
		RealESI     dd      ?
		RealEBP     dd      ?
		Reserved    dd      ?
		RealEBX     dd      ?
		RealEDX     dd      ?
		RealECX     dd      ?
		RealEAX     dd      ?
		RealFlags   dw      ?
		RealES      dw      ?
		RealDS      dw      ?
		RealFS      dw      ?
		RealGS      dw      ?
		RealIP      dw      ?
		RealCS      dw      ?
		RealSP      dw      ?
		RealSS      dw      ?
REALREGS    ENDS

		regs    REALREGS    < >


_DATA   ENDS

DGROUP  GROUP _DATA


_TEXT   SEGMENT BYTE PUBLIC USE32 'CODE'

		ASSUME  DS:_DATA
		ASSUME  CS:_TEXT

EBXFarTo32:
			push    ecx
			mov     ecx, ebx
			and     ecx, 0FFFF0000h
			shr     ecx, 12
			and     ebx, 0FFFFh
			add     ebx, ecx
			pop     ecx
			ret

PUBLIC  gr_vesa_setmode_

gr_vesa_setmode_:

			push    ebx
			push    ecx
			push    edx
			push    edi

			mov     GoalMode, ax

			; Allocate a 256 byte block of DOS memory using DPMI
			mov     ax, 0100h
			mov     bx, 64      ; 64 paragraphs = 256 byte
			int     31h
			jc      NoMemory
			; AX = real mode segment of allocated block
			and     eax, 0FFFFh
			mov     BufferSeg, ax
			shl     eax, 4      ; EAX = 32-bit pointer to DOS memory
			mov     BufferPtr, eax

			; Get SuperVGA information
			mov     ax, BufferSeg
			mov     regs.RealEDI, 0
			mov     regs.RealESI, 0
			mov     regs.RealEBP, 0
			mov     regs.Reserved, 0
			mov     regs.RealEBX, 0
			mov     regs.RealEDX, 0
			mov     regs.RealECX, 0
			mov     regs.RealEAX, 04f00h
			mov     regs.RealFlags, 0
			mov     regs.RealES, ax
			mov     regs.RealDS, 0
			mov     regs.RealFS, 0
			mov     regs.RealGS, 0
			mov     regs.RealIP, 0
			mov     regs.RealCS, 0
			mov     regs.RealSP, 0
			mov     regs.RealSS, 0

			mov     bl, 10h
			xor     bh, bh
			xor     cx, cx
			mov     edi, offset regs
			mov     ax, 0300h
			int     31h

			mov     eax, regs.RealEAX
			cmp     ax, 04fh
			jne     BadStatus

			; Make sure there is a VESA signature
			mov     eax, BufferPtr
			cmp     dword ptr[eax+0], VESA_Signature
			jne     NoVESADriver

			; We now have a valid VESA driver loaded

			mov     bx, word ptr [eax+4]
			mov     VESAVersion, bx

			mov     ebx, dword ptr [eax+6]
			call    EBXFarTo32
			mov     OEMStringPtr, ebx

			mov     ebx, dword ptr [eax+10]
			mov     Capabilities, ebx

			mov     bx, word ptr [eax+18]
			mov     TotalMemory, bx

			mov     ebx, dword ptr [eax+14]
			call    EBXFarTo32
			mov     VideoModePtr, ebx

TryAnotherMode:
			mov     ax, word ptr [ebx]
			add     ebx, 2
			cmp     ax, GoalMode
			je      ModeSupported
			cmp     ax, -1
			je      ModeNotSupported
			jmp     TryAnotherMode

ModeSupported:

			; Get SuperVGA information
			mov     ax, BufferSeg
			movzx   ecx, GoalMode
			mov     regs.RealEDI, 0
			mov     regs.RealESI, 0
			mov     regs.RealEBP, 0
			mov     regs.Reserved, 0
			mov     regs.RealEBX, 0
			mov     regs.RealEDX, 0
			mov     regs.RealECX, ecx
			mov     regs.RealEAX, 04f01h
			mov     regs.RealFlags, 0
			mov     regs.RealES, ax
			mov     regs.RealDS, 0
			mov     regs.RealFS, 0
			mov     regs.RealGS, 0
			mov     regs.RealIP, 0
			mov     regs.RealCS, 0
			mov     regs.RealSP, 0
			mov     regs.RealSS, 0

			mov     bl, 10h
			xor     bh, bh
			xor     cx, cx
			mov     edi, offset regs
			mov     ax, 0300h
			int     31h

			mov     eax, regs.RealEAX
			cmp     ax, 04fh
			jne     BadStatus


			; Check if this mode supported by hardware.
			mov     eax, BufferPtr
			mov     bx, [eax]
			bt      bx, 0
			jnc     HardwareNotSupported


			mov     bx, [eax+4]
			cmp     bx, 64
			jne     @f
			mov     PageSizeShift, 0
			jmp     GranularityOK
@@:         cmp     bx, 32
			jne     @f
			mov     PageSizeShift, 1
			jmp     GranularityOK
@@:         cmp     bx, 16
			jne     @f
			mov     PageSizeShift, 2
			jmp     GranularityOK
@@:         cmp     bx, 8
			jne     @f
			mov     PageSizeShift, 3
			jmp     GranularityOK
@@:         cmp     bx, 4
			jne     @f
			mov     PageSizeShift, 4
			jmp     GranularityOK
@@:         cmp     bx, 2
			jne     @f
			mov     PageSizeShift, 5
			jmp     GranularityOK
@@:         cmp     bx, 1
			jne     WrongGranularity
			mov     PageSizeShift, 6

GranularityOK:
			shl     bx, 10
			mov     WinGranularity, bx

			mov     bx, [eax+6]
			mov     WinSize, bx

			mov     ebx, [eax+12]
			call    EBXFarTo32
			mov     WinFuncPtr, ebx

			mov     bx, [eax+16]
			and     ebx, 0ffffh
			mov     BytesPerScanLine, ebx

			; Set the mode!
			mov     ax, 04f02h
			mov     bx, GoalMode
			int     10h

			mov     eax, 0

			jmp     Done

WrongLogicalWidth:
			mov     eax, 1
			jmp     Done

WrongGranularity:
			mov     eax, 2
			jmp     Done

HardwareNotSupported:
			mov     eax, 3
			jmp     Done

ModeNotSupported:
			mov     eax, 4
			jmp     Done

NoVESADriver:
			mov     eax, 5
			jmp     Done

BadStatus:
			mov     eax, 6
			jmp     Done

NoMemory:
			mov     eax, 7

Done:
			pop     edi
			pop     edx
			pop     ecx
			pop     ebx

			ret

PUBLIC  gr_vesa_setpage_

gr_vesa_setpage_:

			; EAX = 64K Page number

			cmp     eax, LastPage
			jne     @f
			ret
@@:         mov     LastPage, eax
			push    edx
			push    ebx
			push    ecx
			mov     edx, eax
			mov     cl, PageSizeShift
			shl     edx, cl         ; Convert from 64K pages to GranUnit pages.
			xor     ebx, ebx        ; BH=Select window, BL=Window A
			mov     eax, 04f05h     ; AX=Super VGA video memory window control
			int     10h
			pop     ecx
			pop     ebx
			pop     edx
			ret

PUBLIC  gr_vesa_incpage_

gr_vesa_incpage_:

			push    eax
			push    ebx
			push    ecx
			push    edx
			inc     LastPage
			mov     edx, LastPage
			mov     cl, PageSizeShift
			shl     edx, cl         ; Convert from 64K pages to GranUnit pages.
			xor     ebx, ebx        ; BH=Select window, BL=Window A
			mov     eax, 04f05h     ; AX=Super VGA video memory window control
			int     10h
			pop     edx
			pop     ecx
			pop     ebx
			pop     eax
			ret


PUBLIC  gr_vesa_setstart_

gr_vesa_setstart_:

			; EAX = First column
			; EDX = First row
			push    ebx
			push    ecx
			mov     ecx, eax
			mov     eax, 4f07h
			xor     ebx, ebx
			int     10h
			pop     ecx
			pop     ebx
			ret


PUBLIC  gr_vesa_setlogical_

gr_vesa_setlogical_:

			; EAX = line width
			push    ebx
			push    ecx
			push    edx

			mov     cx, ax
			mov     ax, 04f06h
			mov     bl, 0
			int     10h
			and     ebx, 0ffffh
			mov     BytesPerScanLine, ebx
			mov     ax, cx

			pop     edx
			pop     ecx
			pop     ebx
			ret




PUBLIC gr_vesa_pixel8_

gr_vesa_pixel8_:

			; EAX = x
			; EDX = y
			; EBX = 8-bit color

			imul    edx, BytesPerScanLine
			add     edx, eax        ; EDX = y*bpr+x1

			mov     eax, edx
			and     edx, 00FFFFh
			shr     eax, 16

			cmp     eax, LastPage
			jne     @f
			mov     [edx+0A0000h], bl
			ret

@@:         mov     LastPage, eax
			push    edx
			push    ecx
			push    ebx
			mov     edx, eax
			mov     cl, PageSizeShift
			shl     edx, cl         ; Convert from 64K pages to GranUnit pages.
			xor     ebx, ebx        ; BH=Select window, BL=Window A
			mov     eax, 04f05h     ; AX=Super VGA video memory window control
			int     10h
			pop     ebx
			pop     ecx
			pop     edx
			mov     [edx+0A0000h], bl
			ret

PUBLIC gr_vesa_pixel15_

gr_vesa_pixel15_:

			; EAX = x
			; EDX = y
			; EBX = 15-bit color

			imul    edx, BytesPerScanLine
			shl     eax, 1
			add     edx, eax        ; EDX = y*bpr+x1*2

			mov     eax, edx
			and     edx, 00FFFFh
			shr     eax, 16

			cmp     eax, LastPage
			jne     @f
			mov     [edx+0A0000h], bx
			ret

@@:         mov     LastPage, eax
			push    edx
			push    ecx
			push    ebx
			mov     edx, eax
			mov     cl, PageSizeShift
			shl     edx, cl         ; Convert from 64K pages to GranUnit pages.
			xor     ebx, ebx        ; BH=Select window, BL=Window A
			mov     eax, 04f05h     ; AX=Super VGA video memory window control
			int     10h
			pop     ebx
			pop     ecx
			pop     edx
			mov     [edx+0A0000h], bx
			ret



PUBLIC gr_vesa_copy_canvas_

gr_vesa_copy_canvas_:
			; EAX = pointer to canvas

			push    ebx
			push    ecx
			push    edx
			push    edi
			push    esi

			mov     esi, eax

; This is code generated by BLOCK.BAS that moves a block of system
; memory to SVGA memory.
; This particular routine moves a block of size  300 x 200 x 2  bpp from
; ESI to ( 0 , 0 ) on the SVGA screen. 
        MOV  EBX,  0 
        MOV  EDX,  150 
        MOV  EAX,  0 
        CALL gr_vesa_setpage_
; Draw line  0 
        MOV EDI, 0A0000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  1 
        MOV EDI, 0A0500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  2 
        MOV EDI, 0A0A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  3 
        MOV EDI, 0A0F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  4 
        MOV EDI, 0A1400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  5 
        MOV EDI, 0A1900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  6 
        MOV EDI, 0A1E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  7 
        MOV EDI, 0A2300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  8 
        MOV EDI, 0A2800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  9 
        MOV EDI, 0A2D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  10 
        MOV EDI, 0A3200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  11 
        MOV EDI, 0A3700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  12 
        MOV EDI, 0A3C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  13 
        MOV EDI, 0A4100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  14 
        MOV EDI, 0A4600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  15 
        MOV EDI, 0A4B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  16 
        MOV EDI, 0A5000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  17 
        MOV EDI, 0A5500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  18 
        MOV EDI, 0A5A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  19 
        MOV EDI, 0A5F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  20 
        MOV EDI, 0A6400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  21 
        MOV EDI, 0A6900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  22 
        MOV EDI, 0A6E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  23 
        MOV EDI, 0A7300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  24 
        MOV EDI, 0A7800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  25 
        MOV EDI, 0A7D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  26 
        MOV EDI, 0A8200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  27 
        MOV EDI, 0A8700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  28 
        MOV EDI, 0A8C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  29 
        MOV EDI, 0A9100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  30 
        MOV EDI, 0A9600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  31 
        MOV EDI, 0A9B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  32 
        MOV EDI, 0AA000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  33 
        MOV EDI, 0AA500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  34 
        MOV EDI, 0AAA00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  35 
        MOV EDI, 0AAF00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  36 
        MOV EDI, 0AB400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  37 
        MOV EDI, 0AB900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  38 
        MOV EDI, 0ABE00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  39 
        MOV EDI, 0AC300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  40 
        MOV EDI, 0AC800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  41 
        MOV EDI, 0ACD00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  42 
        MOV EDI, 0AD200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  43 
        MOV EDI, 0AD700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  44 
        MOV EDI, 0ADC00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  45 
        MOV EDI, 0AE100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  46 
        MOV EDI, 0AE600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  47 
        MOV EDI, 0AEB00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  48 
        MOV EDI, 0AF000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  49 
        MOV EDI, 0AF500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  50 
        MOV EDI, 0AFA00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  51 
        MOV EDI, 0AFF00h
        MOV ECX,  64 
        REP MOVSD
        CALL gr_vesa_incpage_
; Draw line  51 
        MOV EDI, 0A0000h
        MOV ECX,  86 
        REP MOVSD
; Draw line  52 
        MOV EDI, 0A0400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  53 
        MOV EDI, 0A0900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  54 
        MOV EDI, 0A0E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  55 
        MOV EDI, 0A1300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  56 
        MOV EDI, 0A1800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  57 
        MOV EDI, 0A1D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  58 
        MOV EDI, 0A2200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  59 
        MOV EDI, 0A2700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  60 
        MOV EDI, 0A2C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  61 
        MOV EDI, 0A3100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  62 
        MOV EDI, 0A3600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  63 
        MOV EDI, 0A3B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  64 
        MOV EDI, 0A4000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  65 
        MOV EDI, 0A4500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  66 
        MOV EDI, 0A4A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  67 
        MOV EDI, 0A4F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  68 
        MOV EDI, 0A5400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  69 
        MOV EDI, 0A5900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  70 
        MOV EDI, 0A5E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  71 
        MOV EDI, 0A6300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  72 
        MOV EDI, 0A6800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  73 
        MOV EDI, 0A6D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  74 
        MOV EDI, 0A7200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  75 
        MOV EDI, 0A7700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  76 
        MOV EDI, 0A7C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  77 
        MOV EDI, 0A8100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  78 
        MOV EDI, 0A8600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  79 
        MOV EDI, 0A8B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  80 
        MOV EDI, 0A9000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  81 
        MOV EDI, 0A9500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  82 
        MOV EDI, 0A9A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  83 
        MOV EDI, 0A9F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  84 
        MOV EDI, 0AA400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  85 
        MOV EDI, 0AA900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  86 
        MOV EDI, 0AAE00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  87 
        MOV EDI, 0AB300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  88 
        MOV EDI, 0AB800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  89 
        MOV EDI, 0ABD00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  90 
        MOV EDI, 0AC200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  91 
        MOV EDI, 0AC700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  92 
        MOV EDI, 0ACC00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  93 
        MOV EDI, 0AD100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  94 
        MOV EDI, 0AD600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  95 
        MOV EDI, 0ADB00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  96 
        MOV EDI, 0AE000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  97 
        MOV EDI, 0AE500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  98 
        MOV EDI, 0AEA00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  99 
        MOV EDI, 0AEF00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  100 
        MOV EDI, 0AF400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  101 
        MOV EDI, 0AF900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  102 
        MOV EDI, 0AFE00h
        MOV ECX,  128 
        REP MOVSD
        CALL gr_vesa_incpage_
; Draw line  102 
        MOV EDI, 0A0000h
        MOV ECX,  22 
        REP MOVSD
; Draw line  103 
        MOV EDI, 0A0300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  104 
        MOV EDI, 0A0800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  105 
        MOV EDI, 0A0D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  106 
        MOV EDI, 0A1200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  107 
        MOV EDI, 0A1700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  108 
        MOV EDI, 0A1C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  109 
        MOV EDI, 0A2100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  110 
        MOV EDI, 0A2600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  111 
        MOV EDI, 0A2B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  112 
        MOV EDI, 0A3000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  113 
        MOV EDI, 0A3500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  114 
        MOV EDI, 0A3A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  115 
        MOV EDI, 0A3F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  116 
        MOV EDI, 0A4400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  117 
        MOV EDI, 0A4900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  118 
        MOV EDI, 0A4E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  119 
        MOV EDI, 0A5300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  120 
        MOV EDI, 0A5800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  121 
        MOV EDI, 0A5D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  122 
        MOV EDI, 0A6200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  123 
        MOV EDI, 0A6700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  124 
        MOV EDI, 0A6C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  125 
        MOV EDI, 0A7100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  126 
        MOV EDI, 0A7600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  127 
        MOV EDI, 0A7B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  128 
        MOV EDI, 0A8000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  129 
        MOV EDI, 0A8500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  130 
        MOV EDI, 0A8A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  131 
        MOV EDI, 0A8F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  132 
        MOV EDI, 0A9400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  133 
        MOV EDI, 0A9900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  134 
        MOV EDI, 0A9E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  135 
        MOV EDI, 0AA300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  136 
        MOV EDI, 0AA800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  137 
        MOV EDI, 0AAD00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  138 
        MOV EDI, 0AB200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  139 
        MOV EDI, 0AB700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  140 
        MOV EDI, 0ABC00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  141 
        MOV EDI, 0AC100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  142 
        MOV EDI, 0AC600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  143 
        MOV EDI, 0ACB00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  144 
        MOV EDI, 0AD000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  145 
        MOV EDI, 0AD500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  146 
        MOV EDI, 0ADA00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  147 
        MOV EDI, 0ADF00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  148 
        MOV EDI, 0AE400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  149 
        MOV EDI, 0AE900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  150 
        MOV EDI, 0AEE00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  151 
        MOV EDI, 0AF300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  152 
        MOV EDI, 0AF800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  153 
        MOV EDI, 0AFD00h
        MOV ECX, EDX
        REP MOVSD
        CALL gr_vesa_incpage_
; Draw line  154 
        MOV EDI, 0A0200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  155 
        MOV EDI, 0A0700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  156 
        MOV EDI, 0A0C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  157 
        MOV EDI, 0A1100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  158 
        MOV EDI, 0A1600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  159 
        MOV EDI, 0A1B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  160 
        MOV EDI, 0A2000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  161 
        MOV EDI, 0A2500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  162 
        MOV EDI, 0A2A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  163 
        MOV EDI, 0A2F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  164 
        MOV EDI, 0A3400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  165 
        MOV EDI, 0A3900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  166 
        MOV EDI, 0A3E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  167 
        MOV EDI, 0A4300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  168 
        MOV EDI, 0A4800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  169 
        MOV EDI, 0A4D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  170 
        MOV EDI, 0A5200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  171 
        MOV EDI, 0A5700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  172 
        MOV EDI, 0A5C00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  173 
        MOV EDI, 0A6100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  174 
        MOV EDI, 0A6600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  175 
        MOV EDI, 0A6B00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  176 
        MOV EDI, 0A7000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  177 
        MOV EDI, 0A7500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  178 
        MOV EDI, 0A7A00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  179 
        MOV EDI, 0A7F00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  180 
        MOV EDI, 0A8400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  181 
        MOV EDI, 0A8900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  182 
        MOV EDI, 0A8E00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  183 
        MOV EDI, 0A9300h
        MOV ECX, EDX
        REP MOVSD
; Draw line  184 
        MOV EDI, 0A9800h
        MOV ECX, EDX
        REP MOVSD
; Draw line  185 
        MOV EDI, 0A9D00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  186 
        MOV EDI, 0AA200h
        MOV ECX, EDX
        REP MOVSD
; Draw line  187 
        MOV EDI, 0AA700h
        MOV ECX, EDX
        REP MOVSD
; Draw line  188 
        MOV EDI, 0AAC00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  189 
        MOV EDI, 0AB100h
        MOV ECX, EDX
        REP MOVSD
; Draw line  190 
        MOV EDI, 0AB600h
        MOV ECX, EDX
        REP MOVSD
; Draw line  191 
        MOV EDI, 0ABB00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  192 
        MOV EDI, 0AC000h
        MOV ECX, EDX
        REP MOVSD
; Draw line  193 
        MOV EDI, 0AC500h
        MOV ECX, EDX
        REP MOVSD
; Draw line  194 
        MOV EDI, 0ACA00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  195 
        MOV EDI, 0ACF00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  196 
        MOV EDI, 0AD400h
        MOV ECX, EDX
        REP MOVSD
; Draw line  197 
        MOV EDI, 0AD900h
        MOV ECX, EDX
        REP MOVSD
; Draw line  198 
        MOV EDI, 0ADE00h
        MOV ECX, EDX
        REP MOVSD
; Draw line  199 
        MOV EDI, 0AE300h
        MOV ECX, EDX
        REP MOVSD
			pop     esi
			pop     edi
			pop     edx
			pop     ecx
			pop     ebx

			ret


_TEXT   ENDS


		END


