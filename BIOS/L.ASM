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
PUBLIC joy_read_stick_friendly_

joy_read_stick_friendly_:	
		; ebx = read mask
		; edi = pointer to event buffer
		; ecx = timeout value
		; returns in eax the number of events

		mov	RetryCount, 0
		mov	_joy_bogus_reading, 0

joy_read_stick_friendly_retry:
		inc	RetryCount
		cmp	RetryCount, 3
		jbe	@f
		mov	_joy_bogus_reading, 1
		inc	_joy_retries
		mov	eax, 0
		ret

@@:
		push	ecx
		push	ebx
		push	edi

		and	ebx, 01111b		; Make sure we only check the right values										
						; number of events we found will be in bh, so this also clears it to zero.

		mov     dx, JOY_PORT

		cli				; disable interrupts while reading time...
		call	joy_get_timer		; Returns counter in EAX
		sti				; enable interrupts after reading time...
		mov	LastTick, eax

waitforstable:	in	al, dx
		and	al, bl
		jz	ready 			; Wait for the port in question to be done reading...
		
		cli				; disable interrupts while reading time...
		call	joy_get_timer		; Returns counter in EAX
		sti				; enable interrupts after reading time...
		xchg	eax, LastTick
		cmp	eax, LastTick
		jb	@f
		sub	eax, LastTick
@@:		; Higher...
		add	TotalTicks, eax
		cmp	TotalTicks, ecx		; Timeout at 1/200'th of a second
		jae	ready
		jmp	waitforstable
		
ready:
		cli			
		mov     al, 0ffh
		out     dx, al			; Start joystick a readin'

		call	joy_get_timer		; Returns counter in EAX
		mov	LastTick, eax
		mov	TotalTicks, 0
		
		mov	[edi], eax		; Store initial count
		add	edi, 4		

	again:	in	al, dx			; Read Joystick port
		not	al
		and	al, bl			; Mask off channels we don't want to read
		jnz	flip			; See if any of the channels flipped

		; none flipped -- check any interrupts...
		mov	al, 0Ah
		out 	20h, al
		in	al, 20h		; Get interrupts pending
		cmp	al, 0
		je	NoInts

		; Need to do an interrupt
		sti
		nop	; let the interrupt go on...
		cli
		
		; See if any axis turned
		in	al, dx
		not	al
		and	al, bl
		jz	NoInts

		; At this point, an interrupt occured, making one or more
		; of the axis values bogus.  So, we will restart this process...

		pop	edi
		pop	ebx
		pop	ecx

		jmp	joy_read_stick_friendly_retry

NoInts:
		call	joy_get_timer		; Returns counter in EAX
		
		xchg	eax, LastTick
		cmp	eax, LastTick
		jb	@f
		sub	eax, LastTick
@@:		; Higher...
		add	TotalTicks, eax
		cmp	TotalTicks, ecx		; Timeout at 1/200'th of a second
		jae	timedout
		jmp	again

	flip:	and	eax, 01111b		; Only care about axis values
		mov	[edi], eax		; Record what channel(s) flipped
		add	edi, 4	
		xor	bl, al			; Unmark the channels that just tripped

		call	joy_get_timer		; Returns counter in EAX
		mov	[edi], eax		; Record the time this channel flipped
		add	edi, 4		
		inc	bh			; Increment number of events

		cmp	bl, 0	
		jne	again			; If there are more channels to read, keep looping

	timedout:
		sti

		movzx	eax, bh			; Return number of events

		pop	edi
		pop	ebx
		pop	ecx

		ret

