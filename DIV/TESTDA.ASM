;
; $Source: f:/miner/source/div/rcs/div0.asm $
; $Revision: 1.1 $
; $Author: john $
; $Date: 1993/09/17 12:37:51 $
;
; Divide by zero error handler functions
;
; $Log: div0.asm $
; Revision 1.1  1993/09/17  12:37:51  john
; Initial revision
;
;
;


.386

_DATA   SEGMENT BYTE PUBLIC USE32 'DATA'

_DATA   ENDS

DGROUP  GROUP _DATA


_TEXT   SEGMENT BYTE PUBLIC USE32 'CODE'

		ASSUME  DS:_DATA
		ASSUME  CS:_TEXT

		extern      div0_set_handler_:PROC

PUBLIC  correct_
correct_:

		mov     eax, 15
		jmp     Corrected



PUBLIC  testdivide_
testdivide_:


		push    ecx
		push    edx

PUBLIC  div0_my_add1_
div0_my_add1_:

		mov     edx, 1
		mov     eax, 1
		mov     ecx, 0

PUBLIC  div0_my_add_
div0_my_add_:
		idiv    ecx     ; should cause error

Corrected:
		pop     edx
		pop     ecx

		ret


PUBLIC setup_correct_

setup_correct_:

		push    eax
		push    edx

		mov     eax, offset div0_my_add1_
		mov     edx, offset correct_

		call    div0_set_handler_

		mov     eax, offset div0_my_add_
		mov     edx, offset correct_

		call    div0_set_handler_


		pop     edx
		pop     eax
		ret



_TEXT   ENDS


		END







