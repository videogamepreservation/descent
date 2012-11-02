/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

#define	TDATA       	0x40
#define	TCOMMAND    	0x43
#define  TMODE2			0x34			// Set mode 2

#define disable _disable
#define enable _enable
#define outportb outp
#define inportb inp

// n_milliseconds 0-32768...
void ms_delay_asm(short n_milliseconds);
#pragma aux ms_delay_asm parm [edx] modify exact [esi edi eax ebx ecx edx] = 			\
"	mov	dx, ax																							"\
"	sti							//Disable interrupts													"\
"	xor	ah, ah				//Load timer latch command (0) into ah							"\
"	out	TCOMMAND, ah		//Send command															"\
"	in		al, TDATA			//Read and throw away low byte									"\
"	in		al, TDATA			//Read high byte														"\
"	sti						   //Re-enable interrupts												"\
"	push	ax						//Store initial timer count										"\
"																															 " \
"	mov	ax, dx;	  //n_milliseconds; All this code is to calculate n_milliseconds * 4.659	 " \
"	mov	bx, 37																						  "	\
"	imul	bx					  // dx:ax = n_milliseconds * 37									  "	\
"	mov	si, dx																						  "	\
"	mov	di, ax																						  "	\
"	mov	cl, 3																							  "	\
"	shr	di, cl																						  "	\
"	shr	si, cl				//si:di /= 8														  "	\
"	mov	cl, 5																							  "	\
"	shl	dx, cl																						  "	\
"	or		di, dx				//Our target value is stored in SI:DI now					  "	\
"	xor	cx, cx				//Clear our 32-bit counter in cx:dx							  "	\
"	xor	bx, bx																						  "	\
"																											  "	\
"	pop	ax						//Get initial timer value, so that all the divides		  "	\
"									//above count towards the total time waiting.			  "	\
"																											  "	\
"MsTimerLoop:																							  "	\
"	mov	dx, ax				//Store last read timer value									  "	\
"	sti							//Disable interrupts												  "	\
"	xor	ah, ah				//Clearing AH timer latch command							  "	\
"	out	TCOMMAND, ah		//Send command														  "	\
"	in		al, TDATA			//Read and throw away low byte								  "	\
"	in		al, TDATA			//Read high byte													  "	\
"	sti							//Re-enable interrupts											  "	\
"	cmp	ax, dx				//Check for timer counter wrapping							  "	\
"	jl		MsNoWrap																						  "	\
"	add	dx, 256				//If it wrapped, add 256 to total count					  "	\
"MsNoWrap:																								  "	\
"	sub	dx, ax				//Calculate delta time											  "	\
"	add	bx, dx				//Add delta time to total counter in cx:bx				  "	\
"	adc	cx, 0					//(32 bit add)														  "	\
"																											  "	\
"	cmp	cx, si				//See if we are done by checking 							  "	\
"	ja		MsDone				// to see if cx:bx > si:di										  "	\
"	jb		MsTimerLoop																					  "	\
"	cmp	bx, di																						  "	\
"	jb		MsTimerLoop																					  "	\
"MsDone:							//cx:bx is > si:di, so we're done							  "\

// n_milliseconds 0-32768...
void ms_delay(short n_milliseconds)
{
	unsigned long count, target;		
	unsigned char reading;
	unsigned short delta;

	disable();
	outportb( TCOMMAND, 0 );		// Latch timer 0
	inportb( TDATA );					// Low byte
	reading = inportb( TDATA );	// High byte
	enable();

	target = (unsigned long)n_milliseconds*37;		// Should be x 4.659
	target /= 8;
	count = 0;
	delta = reading;

	while(1)	{
		disable();
		outportb( TCOMMAND, 0 );		// Latch timer 0
		inportb( TDATA );					// Low byte
		reading = inportb( TDATA );	// High byte
		enable();
		if ( reading > delta )	
			delta += 256;
		delta -= reading;
		count += delta;
		if ( count > target ) return;
		delta = reading;
	}
}

void main()
{
	int i;
	disable();
	outportb( TCOMMAND, TMODE2 );		// Set timer to count in mode 2
	outportb( TDATA, 0 );
	outportb( TDATA, 0 );
	enable();
	printf( "Press a key to start\n" );
	getch();
	printf( "Go...\n" );
	for (i=0;i<3000; i++ )	{
		ms_delay_asm( 10 );
	}
	printf( "Stop\n" );

}


