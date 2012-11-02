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
	xor     ax, ax          ; Send
	cli                     ; Disable interrupts while using timer
	out     043h, al                                                                                                               
	in      al, 040h                                                                                                
	in      al, 040h                                                                                                                                
	sti                                                                                                                                             
	push    ax                                                                                                                      
	mov     ax, [n_milliseconds]
	mov     bx, 37                                                                                                                                                                                  
	imul    bx                                                                                                                      
	mov     si, dx                                                                                                                                                                                  
	mov     di, ax                                                                                                                                                                                  
	mov     cl, 3                                                                                                                                                                                           
	shr     di, cl                                                                                                                                                                                  
	shr     si, cl                    
	mov     cl, 13                                                                                                                                                                                  
	shl     dx, cl                                                                                                                                                                                  
	or      di, dx                                                          
	xor     cx, cx                                  
	xor     bx, bx                                                                                                                                                                                  
	pop     ax                                                              
MsTimerLoop:    
	mov     dx, ax                                                                                  
	xor     ax, ax                                                                                          
	cli 
	out     043h, al                                                                                        
	in      al, 040h                                                                
	in      al, 040h                                                                        
	sti                                                                                     
	cmp     ax, dx                                                                  
	jbe     MsNoWrap                                                                                                                                                                                        
	add     dx, 256                                 
MsNoWrap:       
	sub     dx, ax                                  
	add     bx, dx                                  
	adc     cx, 0                                           
	cmp     cx, si                                  
	ja      MsDone                                  
	jb      MsTimerLoop                                                                                                                                                                             
	cmp     bx, di                                                                                                                                                                                  
	jb      MsTimerLoop                                                                                                                                                                             
MsDone: ret

