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
; $Source: f:/miner/source/main/rcs/soscodec.asm $
; $Revision: 2.0 $
; $Author: john $
; $Date: 1995/02/27 11:29:36 $
; 
; Routines for compressing digital sounds.
; 
; $Log: soscodec.asm $
; Revision 2.0  1995/02/27  11:29:36  john
; New version 2.0, which has no anonymous unions, builds with
; Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
; 
; Revision 1.2  1994/11/30  14:08:46  john
; First version.
; 
; Revision 1.1  1994/11/29  14:33:51  john
; Initial revision
; 
; 



.386
	option	oldstructs

	.nolist
	include	types.inc
	include	psmacros.inc
	.list

	assume	cs:_TEXT, ds:_DATA

_DATA	segment	dword public USE32 'DATA'

rcsid	db	"$Id: soscodec.asm 2.0 1995/02/27 11:29:36 john Exp $"
	align	4

; public declarations
                        public   sosCODECDecompressData_
                        public   sosCODECCompressData_
                        public   sosCODECInitStream_

; structure definitions
sCompressInfo           struc

   Source             	dd    0           ; pointer to compressed data
   Dest               	dd    0           ; pointer to uncompressed data

   dwCompSize           dd    0           ; compressed size
   dwUnCompSize         dd    0           ; uncompressed size

   dwSampleIndex        dd    0           ; index into sample
   dwPredicted          dd    0           ; next predicted value
   dwDifference         dd    0           ; difference from last sample

   wCodeBuf             dw    0           ; holds 2 nibbles for decompression
   wCode                dw    0           ; current 4 bit code
   wStep                dw    0           ; step value in table
   wIndex               dw    0           ; index into step table

   wBitSize             dw    0           ; bit size for decompression

sCompressInfo           ends

; index table for stepping into step table
;
wCODECIndexTab          dw    -1,-1,-1,-1, 2, 4, 6, 8
			dw    -1,-1,-1,-1, 2, 4, 6, 8

wCODECStepTab           dw    7, 8, 9, 10, 11, 12, 13, 14
                        dw    16,  17,  19,  21,  23,  25,  28
                        dw    31,  34,  37,  41,  45,  50,  55
                        dw    60,  66,  73,  80,  88,  97, 107
                        dw    118, 130, 143, 157, 173, 190, 209
                        dw    230, 253, 279, 307, 337, 371, 408
                        dw    449, 494, 544, 598, 658, 724, 796
                        dw    876, 963,1060,1166,1282,1411,1552
                        dw    1707,1878
                        dw    2066,2272,2499,2749,3024,3327,3660,4026
                        dw    4428,4871,5358,5894,6484,7132,7845,8630
                        dw    9493,10442,11487,12635,13899,15289,16818
                        dw    18500,20350,22385,24623,27086,29794,32767

; byte index counter
wCODECByteIndex         dd    0

; total bytes processed
wCODECBytesProcessed    dd    0

; current mask value
wCODECMask              dw    0

; temp step value
dwCODECTempStep         dd    0


_DATA	ends



_TEXT	segment	dword public USE32 'CODE'



sosCODECInitStream_:
                        ; save used registers
                        push  ebx

                        ; 
                        ; set up pointer to _SOS_COMPRESS_INFO
                        ; structure in EBX.
                        ;
                        mov   ebx, eax

                        ;
                        ; init members of compression stream
                        ;
                        mov   word ptr [ ebx ].wIndex, 0
                        mov   word ptr [ ebx ].wStep, 7
                        mov   dword ptr [ ebx ].dwPredicted, 0
                        mov   dword ptr [ ebx ].dwSampleIndex, 0

                        ; restore registers
                        pop   ebx

                        ret
 
;
; DWORD sosCODECDecompressData( _SOS_COMPRESS_INFO * sSOSInfo, DWORD wBytes )
;
; decompress data from a 4:1 ADPCM compressed file.  the number of
; bytes decompressed is returned.
;
;
sosCODECDecompressData_:
		; eax = sSOSInfo
		; edx = wBytes

                        ;
                        ; save registers used
                        ;
                        push  esi
                        push  edi
                        push  ebx
                        push  ecx
                        push  edx

                        ;
                        ; set up EBX as a pointer to 
                        ; info structure.
                        ;
                        mov   ebx, eax

                        ;
                        ; set up byte counter. if 16 bit data
                        ; divide bytes by two since we will
                        ; spit out two bytes @ a time.
                        ;
                        mov   eax, edx
                        mov   wCODECBytesProcessed, eax

                        mov   wCODECByteIndex, eax

                        ;
                        ; set up source and destination 
                        ; pointers
                        ;
                        mov	esi, [ ebx ].Source
                        mov	edi, [ ebx ].Dest

Decompmainloop:
                        ;
                        ; determine if sample index is even
                        ; or odd. this will determine if we
                        ; need to get a new token or not.
                        ;
                        test  dword ptr [ ebx ].dwSampleIndex, 1
                        je    short DecompfetchToken

                        ;
                        ; get token 
                        ;
                        movzx eax, word ptr [ ebx ].wCodeBuf
                        shr   eax, 4
                        and   eax, 000fh
                        mov   word ptr [ ebx ].wCode, ax
                        jmp   short DecompcalcDifference
DecompfetchToken:
                        ;
                        ; fetch new token
                        ;
                        xor   eax, eax
                        mov   al, byte ptr [ esi ]
                        mov   word ptr [ ebx ].wCodeBuf, ax
                        inc   esi
                        and   eax, 000fh
                        mov   word ptr [ ebx ].wCode, ax
DecompcalcDifference:
                        ;
                        ; reset difference
                        ;
                        mov   dword ptr [ ebx ].dwDifference, 0

                        ;
                        ; set up ECX as the step value
                        ;
                        movzx ecx, word ptr [ ebx ].wStep

                        ; 
                        ; check for wCode & 4
                        ;
                        test  eax, 4
                        je    short Decompno4

                        ; add wStep
                        add   dword ptr [ ebx ].dwDifference, ecx
Decompno4:
                        ;
                        ; check for wCode & 2
                        ;
                        test  eax, 2
                        je    short Decompno2

                        ; add wStep >> 1
                        mov   edx, ecx
                        shr   edx, 1
                        add   dword ptr [ ebx ].dwDifference, edx

Decompno2:
                        ;
                        ; check for wCode & 1
                        ;
                        test  eax, 1
                        je    short Decompno1

                        ; add wStep >> 2
                        mov   edx, ecx
                        shr   edx, 2
                        add   dword ptr [ ebx ].dwDifference, edx
Decompno1:
                        ;
                        ; add in wStep >> 3
                        ;
                        mov   edx, ecx
                        shr   edx, 3
                        add   dword ptr [ ebx ].dwDifference, edx

                        ;
                        ; check for wCode & 8
                        ;
                        test  eax, 8
                        je    short Decompno8

                        ; negate difference
                        neg   dword ptr [ ebx ].dwDifference
Decompno8:
                        ;
                        ; add difference to predicted
                        ; value.
                        ;
                        mov   eax, [ ebx ].dwPredicted
                        add   eax, [ ebx ].dwDifference

                        ;
                        ; make sure there is no under or 
                        ; overflow.
                        ;
                        cmp   eax, 7fffh
                        jl    short DecompnoOverflow
                        mov   eax, 7fffh
DecompnoOverflow:
                        cmp   eax, 0ffff8000h
                        jg    short DecompnoUnderflow
                        mov   eax, 0ffff8000h
DecompnoUnderflow:
                        mov   dword ptr [ ebx ].dwPredicted, eax

                        ;
                        ; output 8 bit sample
                        ;
                        xor   ah, 80h
                        mov   al, ah
                        stosb

                        ;
                        ; adjust index 
                        ;
                        movzx ecx, word ptr [ ebx ].wCode
                        movzx eax, word ptr wCODECIndexTab[ ecx * 2 ]
                        add   word ptr [ ebx ].wIndex, ax

                        ;
                        ; check if wIndex < 0
                        ;
                        cmp   word ptr [ ebx ].wIndex, 8000h
                        jb    short DecompcheckOverflow

                        ; reset index to zero
                        mov   word ptr [ ebx ].wIndex, 0
                        jmp   short DecompadjustStep
DecompcheckOverflow:
                        ;
                        ; check if wIndex > 88
                        ;
                        cmp   word ptr [ ebx ].wIndex, 88
                        jbe   short DecompadjustStep

                        ; reset index to 88
                        mov   word ptr [ ebx ].wIndex, 88
DecompadjustStep:
                        ;
                        ; fetch wIndex so we can fetch
                        ; new step value
                        ;
                        movzx ecx, word ptr [ ebx ].wIndex
                        movzx eax, word ptr wCODECStepTab[ ecx * 2 ]

                        ;
                        ; advance index and store step value
                        ;
                        add   dword ptr [ ebx ].dwSampleIndex, 1
                        mov   word ptr [ ebx ].wStep, ax

                        ;
                        ; decrement bytes processed
                        ; and loop back.
                        ;
                        dec   wCODECByteIndex
                        jne   Decompmainloop

                        ;
                        ; save off ESI and EDI back into
                        ; compress info structure.
                        ;
                        mov   dword ptr [ ebx ].Source, esi
                        mov   dword ptr [ ebx ].Dest, edi

                        ;
                        ; set up return value for number
                        ; of bytes processed.
                        ;
                        mov   eax, wCODECBytesProcessed

                        ;
                        ; restore registers
                        ;
                        pop   edx
                        pop   ecx
                        pop   ebx
                        pop   edi
                        pop   esi

                        ret

;
; DWORD sosCODECCompressData( _SOS_COMPRESS_INFO * sSOSInfo, DWORD wBytes )
;
; Compresses a data stream into 4:1 ADPCM.  16 bit data is compressed 4:1
; 8 bit data is compressed 2:1.
;
;
sosCODECCompressData_:
		; eax = sSOSInfo 
		; edx = wBytes

                        ;
                        ; save registers used
                        ;
                        push  esi
                        push  edi
                        push  ebx
                        push  ecx
                        push  edx

                        ;
                        ; set up EBX as a pointer to 
                        ; info structure.
                        ;
                        mov   ebx, eax

                        ;
                        ; set up byte counter. if 16 bit data
                        ; divide bytes by two since we will
                        ; spit out two bytes @ a time.
                        ;
                        mov   eax, edx
                        mov   wCODECBytesProcessed, eax

                        ;
                        ; check for 16 bit decompression
                        ;
                        cmp   word ptr [ ebx ].wBitSize, 16
                        jne   short CompskipByteDivide

                        ;
                        ; divide size requested by two
                        ;
                        shr   eax, 1
CompskipByteDivide:
                        mov   wCODECByteIndex, eax

                        ;
                        ; set up source and destination 
                        ; pointers
                        ;
                        mov	esi, [ ebx ].Source
                        mov	edi, [ ebx ].Dest

Compmainloop:
                        ;
                        ; determine bit size for input
                        ;
                        cmp   word ptr [ ebx ].wBitSize, 16
                        jne   short Compinput8Bit

                        ;
                        ; get 16 bit sample
                        ;
                        movsx eax, word ptr [ esi ]
                        add   esi, 2
                        jmp   short CompcomputeDiff
Compinput8Bit:
                        ;
                        ; get 8 bit sample
                        ;
                        mov   ah, byte ptr [ esi ]
                        inc   esi
                        xor   al, al
                        xor   ah, 80h
                        movsx eax, ax
CompcomputeDiff:
                        ;
                        ; compute difference
                        ;
                        movsx ecx, word ptr [ ebx ].dwPredicted
                        sub   eax, ecx

                        ;
                        ; check if dwDifference > 0.  ECX is the
                        ; sign bit, it is initialized to positive.
                        ;
                        xor   ecx, ecx
                        cmp   eax, 0
                        jge   Comppositive

                        ;
                        ; negate difference
                        ;
                        neg   eax
                        or    ecx, 8
Comppositive:
                        ;
                        ; store code value
                        ;
                        mov   word ptr [ ebx ].wCode, cx

                        ;
                        ; set up to quantize difference. initialize 
                        ; wCODECTempStep = step value.
                        ;
                        movsx ecx, word ptr [ ebx ].wStep
                        mov   dwCODECTempStep, ecx
                        mov   edx, 4
                        mov   ecx, 3
CompquantizeLoop:
                        ;
                        ; check to see if difference > tempstep
                        ; value.
                        ;
                        cmp   eax, dwCODECTempStep
                        jl    short CompnextQLoop

                        ;
                        ; OR in mask value into code and
                        ; adjust difference.
                        ;
                        or    word ptr [ ebx ].wCode, dx
                        sub   eax, dwCODECTempStep
CompnextQLoop:
                        ;
                        ; shift down tempstep and mask
                        ;
                        shr   dwCODECTempStep, 1
                        shr   edx, 1

                        loop  CompquantizeLoop

                        ;
                        ; store off new difference value
                        ;
                        mov   dword ptr [ ebx ].dwDifference, eax

                        ;
                        ; determine if sample index is even
                        ; or odd. this will determine if we
                        ; need to get a new token or not.
                        ;
                        test  dword ptr [ ebx ].dwSampleIndex, 1
                        jne   short CompstoreToken

                        ;
                        ; get token 
                        ;
                        movzx eax, word ptr [ ebx ].wCode
                        and   eax, 0fh
                        mov   word ptr [ ebx ].wCodeBuf, ax
                        jmp   short CompcalcDifference
CompstoreToken:
                        ;
                        ; fetch new token
                        ;
                        movzx eax, word ptr [ ebx ].wCode
                        shl   eax, 4
                        or    ax, word ptr [ ebx ].wCodeBuf
                        mov   byte ptr [ edi ], al
                        inc   edi
CompcalcDifference:
                        ;
                        ; reset difference
                        ;
                        mov   dword ptr [ ebx ].dwDifference, 0

                        ;
                        ; set up ECX as the step value
                        ;
                        movzx ecx, word ptr [ ebx ].wStep

                        ;
                        ; get code in AX
                        ;
                        movzx eax, word ptr [ ebx ].wCode

                        ; 
                        ; check for wCode & 4
                        ;
                        test  eax, 4
                        je    short Compno4

                        ; add wStep
                        add   dword ptr [ ebx ].dwDifference, ecx
Compno4:
                        ;
                        ; check for wCode & 2
                        ;
                        test  eax, 2
                        je    short Compno2

                        ; add wStep >> 1
                        mov   edx, ecx
                        shr   edx, 1
                        add   dword ptr [ ebx ].dwDifference, edx

Compno2:
                        ;
                        ; check for wCode & 1
                        ;
                        test  eax, 1
                        je    short Compno1

                        ; add wStep >> 2
                        mov   edx, ecx
                        shr   edx, 2
                        add   dword ptr [ ebx ].dwDifference, edx
Compno1:
                        ;
                        ; add in wStep >> 3
                        ;
                        mov   edx, ecx
                        shr   edx, 3
                        add   dword ptr [ ebx ].dwDifference, edx

                        ;
                        ; check for wCode & 8
                        ;
                        test  eax, 8
                        je    short Compno8

                        ; negate difference
                        neg   dword ptr [ ebx ].dwDifference
Compno8:
                        ;
                        ; add difference to predicted
                        ; value.
                        ;
                        mov   eax, [ ebx ].dwPredicted
                        add   eax, [ ebx ].dwDifference

                        ;
                        ; make sure there is no under or 
                        ; overflow.
                        ;
                        cmp   eax, 7fffh
                        jl    short CompnoOverflow
                        mov   eax, 7fffh
CompnoOverflow:
                        cmp   eax, 0ffff8000h
                        jg    short CompnoUnderflow
                        mov   eax, 0ffff8000h
CompnoUnderflow:
                        mov   dword ptr [ ebx ].dwPredicted, eax

                        ;
                        ; adjust index 
                        ;
                        movzx ecx, word ptr [ ebx ].wCode
                        movzx eax, word ptr wCODECIndexTab[ ecx * 2 ]
                        add   word ptr [ ebx ].wIndex, ax

                        ;
                        ; check if wIndex < 0
                        ;
                        cmp   word ptr [ ebx ].wIndex, 8000h
                        jb    short CompcheckOverflow

                        ; reset index to zero
                        mov   word ptr [ ebx ].wIndex, 0
                        jmp   short CompadjustStep
CompcheckOverflow:
                        ;
                        ; check if wIndex > 88
                        ;
                        cmp   word ptr [ ebx ].wIndex, 88
                        jbe   short CompadjustStep

                        ; reset index to 88
                        mov   word ptr [ ebx ].wIndex, 88
CompadjustStep:
                        ;
                        ; fetch wIndex so we can fetch
                        ; new step value
                        ;
                        movzx ecx, word ptr [ ebx ].wIndex
                        movzx eax, word ptr wCODECStepTab[ ecx * 2 ]

                        ;
                        ; advance index and store step value
                        ;
                        add   dword ptr [ ebx ].dwSampleIndex, 1
                        mov   word ptr [ ebx ].wStep, ax

                        ;
                        ; decrement bytes processed
                        ; and loop back.
                        ;
                        dec   wCODECByteIndex
                        jne   Compmainloop

                        ;
                        ; save off ESI and EDI back into
                        ; compress info structure.
                        ;
                        mov   dword ptr [ ebx ].Source, esi
                        mov   dword ptr [ ebx ].Dest, edi

                        ;
                        ; set up return value for number
                        ; of bytes processed.
                        ;
                        mov   eax, wCODECBytesProcessed
                        shr   eax, 2

                        ;
                        ; restore registers
                        ;
                        pop   edx
                        pop   ecx
                        pop   ebx
                        pop   edi
                        pop   esi

                        ret


_TEXT	ends

	end


