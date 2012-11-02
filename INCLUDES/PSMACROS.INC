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
; $Source: f:/miner/source/includes/rcs/psmacros.inc $
; $Revision: 1.12 $
; $Author: matt $
; $Date: 1994/12/06 14:32:19 $
;
; Useful macros.  Everyone should use these.
;
; $Log: psmacros.inc $
; Revision 1.12  1994/12/06  14:32:19  matt
; Made mprintf turn off if NMONO set
; 
; Revision 1.11  1994/11/27  23:18:19  matt
; Made mprintf go away when debugging turned off
; 
; Revision 1.10  1994/03/25  18:00:34  matt
; Added imulc of 6
; 
; Revision 1.9  1994/02/10  18:00:58  matt
; Changed 'if DEBUG_ON' to 'ifndef NDEBUG'
; 
; Revision 1.8  1993/11/22  23:47:06  matt
; debug macros were trashing eax
; 
; Revision 1.7  1993/11/04  12:39:25  mike
; Modify imulc macro to support fast multiply by 36
; and multiply anything using imul, but give warning.
; 
; Revision 1.6  1993/10/19  21:17:31  matt
; Added abs_eax macro
; 
; Revision 1.5  1993/09/26  22:28:21  matt
; Removed extra register pushes in mprintf
; Make DEBUG_ON all uppercase since we seem to have case sensitivity on
; 
; Revision 1.4  1993/09/26  19:22:30  matt
; Added imulc macro, to multiply a register by a constant.  It does not
; at this time support arbitrary constants, only 0,1,3,5,9, and powers of 2
; 
; Revision 1.3  1993/09/13  11:50:57  matt
; Added 'b' & 'w', aliases for 'byte ptr' and 'word ptr'
; 
; Revision 1.2  1993/09/03  19:00:03  matt
; Added breakpoint macros
; 
; Revision 1.1  1993/08/24  12:51:52  matt
; Initial revision
; 
;
;

;Shortcuts for casting

w equ word ptr
b equ byte ptr


;The macros @ArgCount() & @ArgRev() are from the file MACROS.INC, provided 
;with MASM.  I have included them here because MACROS.INC has bugs, so I 
;couldn't just include it.

; Utility Macros - Version 1.0 - for Microsoft Macro Assembler 6.0
; (C) Copyright Microsoft Corporation, 1987,1988,1989,1990

;* @ArgCount - Macro function returns the number of arguments in a
;* VARARG list.
;*
;* Params:  arglist - arguments to be counted

@ArgCount MACRO arglist:VARARG
    LOCAL count
    count = 0
    FOR arg, <arglist>
        count = count + 1
    ENDM
    EXITM %count
ENDM

;* @ArgRev - Macro function returns a reversed order version of a
;* VARARG list.
;*
;* Shows:   Operators           - <>         !        %
;*          String directive    - SUBSTR
;*          Predefined function - @SizeStr
;*
;* Params:  arglist - arguments to be reversed

@ArgRev MACRO arglist:vararg
    LOCAL txt, arg
    txt TEXTEQU <>
%   FOR arg, <arglist>
        txt CATSTR <arg>, <!,>, txt
    ENDM

    txt SUBSTR  txt, 1, @SizeStr( %txt ) - 1
    txt CATSTR  <!<>, txt, <!>>
    EXITM txt
ENDM

;These macros are used for decalaring external vars and functions

;this macro is used to declare several symbols of the same type
;usage is:  extdef  type,sym0,sym1,...
extdef	macro	type,syms:vararg
	for	sym,<syms>
	 externdef sym:type
	endm
	endm

;this macro is used to generate ext<type> macros 
extgen	macro	type,id
ext&id	macro	syms:vararg
	extdef	type,syms
	endm
	endm

;macros for common types, such as word (extw), byte (extb), & near (extn)

	extgen	word,w
	extgen	byte,b
	extgen	dword,d
	extgen	near,n


;compute the absolute value of eax.  afterwards, edx=sign (0 or -1)
abs_eax	macro
	cdq
	xor	eax,edx
	sub	eax,edx
	endm

;PUSHM & POPM are used for multiple registers.  Note that POPM pops in the
;reverse order from PUSHM, so the list of regs shouls be the same for both.
;You can also define a constant which is a register list, and use it as the
;argument to both macros.

;push multiple registers
pushm	macro	args:vararg
	local	arg
	for	arg,<args>
	 push	arg
	endm
	endm

;pop multiple registers
popm	macro	args:vararg
	local	arg
%	for	arg,@ArgRev(args)
	 pop	arg
	endm
	endm

;PUSHLONG pushes a long, zero extending the argument if necessary
;it trashes no registers
pushlong	macro	arg
	local	s
s = TYPE arg
	if	s EQ 0	;constant, I think
	 push	arg
	elseif	s LT 4
	 push	eax
	 movzx	eax,arg
	 xchg	eax,[esp]
	else
	 push	arg
	endif

	endm

;PUSHML is pushm using pushlong
pushml	macro	args:vararg
	local	arg
	for	arg,<args>
	 pushlong	arg
	endm
	endm


;DBSTR stores a string with occurances of \n converted to newlines
;this macro expects quotes around the string
;
;note the 'fudge' variable.  This fixes an odd problem with the way
;the string macros deal with backslashes - @InStr() treats them like
;any other character, but @SubStr() ignores them
dbstr	macro	str
	local	pos,oldpos,len,fudge

	oldpos = 2	;skip initial quote
	fudge = 0
	len = @SizeStr(str)

	pos = @InStr(oldpos,str,<\n>)

	while	pos GE oldpos

	 if	pos GT oldpos
	  %db	'&@SubStr(<&str>,&oldpos-&fudge,&pos-&oldpos)'
	 endif
	 db	10
	 oldpos = pos+2
	 fudge = fudge+1

	 pos = @InStr(oldpos,<str>,<\n>)

	endm

	if	oldpos LT len
;;;	 %db	'&@SubStr(&str,&oldpos-&fudge,&len-&oldpos-1)'
	 %db	'&@SubStr(&str,&oldpos-&fudge,&len-&oldpos)'
	endif

	endm


;MPRINTF is a macro interface to the mprintf funcion.  It puts the format
;string in the code segment at the current location, pushes the args, and
;calls mprintf. If window is not specified, zero is assumed
mprintf	macro	window:=<0>,format:req,args:vararg
	local	string,skip
	ifndef	NDEBUG
	ifndef	NMONO
	extn	_mprintf_
	jmp	skip
string	label	byte
	dbstr	format
	db	0
skip:
	ifnb	<args>
%	 pushml	@ArgRev(args)
	endif
	pushml	offset string,window
	call	_mprintf_
	add	esp,(@ArgCount(args)+2)*4	;fix stack
	endif
	endif
	endm


;MPRINTF_AT - version of mprintf with coordinates
mprintf_at	macro	window:=<0>,row,col,format:req,args:vararg
	local	string,skip
	ifndef	NDEBUG
	ifndef	NMONO
	extn	_mprintf_at_
	jmp	skip
string	label	byte
	dbstr	format
	db	0
skip:
	ifnb	<args>
%	 pushml	@ArgRev(args)
	endif
	pushml	offset string,col,row,window
	call	_mprintf_at_
	add	esp,(@ArgCount(args)+4)*4	;fix stack
	endif
	endif
	endm


;DEBUG calls mprintf with window 0, preserving all registers and flags
;is is conditionall assembled based on the DEBUG_ON flags
debug	macro	format:req,args:vararg
	ifndef	NDEBUG
	 pushf		;save flags
	 push	eax	;mprintf trashes eax
	 mprintf	,format,args
	 pop	eax
	 popf
	endif
	endm

;DEBUG_AT - version of debug with coordinates
debug_at	macro	row,col,format:req,args:vararg
	ifndef	NDEBUG
	 pushf		;save flags
	 push	eax	;mprintf trashes eax
	 mprintf_at ,row,col,format,args
	 pop	eax
	 popf
	endif
	endm

;Debugging breakpoint macros

;print a message, and do an int3 to pop into the debugger  
debug_brk	macro	str
	ifndef	NDEBUG
	 debug	str
	 int	3
	endif
	endm

break_if	macro	cc,str
	local	skip,yes_break
	ifndef	NDEBUG
	 j&cc	yes_break
	 jmp	skip
yes_break:	 debug_brk str
skip:
	endif
	endm

;returns the bit number of the highest bit
@HighBit	macro	n
	local	t,c
	if	n EQ 0
	 exitm	<-1>	;error!
	else
	 t = n
	 c = 0
	 while	t GT 1
	  t = t SHR 1
	  c = c+1
	 endm
	 exitm	<c>
	endif
	endm

;returns the bit number of the lowest bit
@LowBit	macro	n
	;local	t,c
	local	c
	if	n EQ 0
	 exitm	<-1>	;error!
	else
	 t = n
	 c = 0
	 while	(t and 1) EQ 0
	  t = t SHR 1
	  c = c+1
	 endm
	 exitm	<c>
	endif
	endm


;"multiply" the given register by a constant, using whatever method is
;best for the given constant
imulc	macro	reg,c
	local	low,high

	if	c EQ 0
	 xor	reg,reg
	elseif	c EQ 1
	elseif	c EQ 3
	 lea	reg,[reg*2+reg]
	elseif	c EQ 5
	 lea	reg,[reg*4+reg]
	elseif	c EQ 6
	 lea	reg,[reg*2+reg]	;*3
	 shl	reg,1	;*6
	elseif	c EQ 9
	 lea	reg,[reg*8+reg]
	elseif	c EQ 36
	 lea	reg,[reg*8+reg]
	 sal	reg,2
	else
	 low = @LowBit(c)
	 high = @HighBit(c)
	 if	low EQ high
	  shl	reg,@LowBit(c)
	 else
	  imul	reg,c
	  echo	Warning: Using imul, to perform multiply by &c
	  ;; .err
	 endif
	endif

	endm

