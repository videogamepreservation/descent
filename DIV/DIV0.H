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
/*
 * $Source: f:/miner/source/div/rcs/div0.h $
 * $Revision: 1.2 $
 * $Author: john $
 * $Date: 1994/01/14 15:34:25 $
 *
 * Header file for divide by zero handler
 *
 * $Log: div0.h $
 * Revision 1.2  1994/01/14  15:34:25  john
 * Added counters for number of overflows.
 * 
 * Revision 1.1  1993/09/17  12:37:58  john
 * Initial revision
 * 
 *
 */

// D I V 0 . H

//==========================================================================
// These are constants passed for the default mode.  If the address of the div
// instruction is not in the Callback (CB) list and also not in the
// Saturate (SAT) list, then the default mode will be used. It will either
// do an int 3 and then dump the registers and break to DOS (DM_ERROR)
// or else it will saturate the result and allow the program to continue
// if DM_SATURATE is specified.
#define DM_ERROR 0
#define DM_SATURATE 1


//==========================================================================
// This initializes and grabs the Divide by 0 Exception.  See above for mode.
// Returns 1=Installed OK, 0=Failed.  Failed probably means old DPMI host, but
// I think that as long as we use DOS4GW v1.90 or higher we're ok.
extern int div0_init(int mode);
extern void div0_close();           // Closes it.

//==========================================================================
// Sets the default handler behavior.  See above constant descriptions.
extern void div0_set_mode(int mode);

//==========================================================================
// Adds a handler to the list of handlers to jump to whem an overflow occurs.
// All registers, etc should be exactly as before the DIV instruction. This
// doesn't work within C. This returns 1 if ok, and 0 if there isn't any more
// "slots" available.  The number of slots can be changed in div0.asm by
// changing the MAX_SIZE constant.  MAX_SIZE = 100 now.
// In ASM, the parameters are:
// EAX = *div_addr, EDX=*handler_addr, Return value in EAX.
extern int div0_set_handler( void *div_addr, void *handler_addr );

//==========================================================================
// Same as above, but saturates the result instead of jumping to a handler.
// Doesn't need the void*handler_addr parameter.  Uses the same MAX_SIZE.
extern int div0_set_saturate( void *div_addr );

//==========================================================================
// These three variables count the number of times the divide by zero handler
// has been used. 
//   - div0_num_handled_by_cblist is the number of times a divide exception
//     has occurred and was corrected by calling a user-specified function.
//   - div0_num_handled_by_satlist is the number of times a divide exception
//     has occured and the result was saturated because it specifically was
//     in the saturation list.
//   - div0_num_saturated are the divides that aren't handled by the programmer
//     explicitly, but the handler went ahead and sautrated anyway.  These
//     probably shouldn't be happening very often.

extern int div0_num_handled_by_cblist;
extern int div0_num_handled_by_satlist;
extern int div0_num_saturated;





