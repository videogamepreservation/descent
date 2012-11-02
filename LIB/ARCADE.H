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
 * $Source: f:/miner/source/main/rcs/arcade.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:27:27 $
 * 
 * Routines for arcade version.
 * 
 * $Log: arcade.h $
 * Revision 2.0  1995/02/27  11:27:27  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.4  1994/09/16  21:44:06  matt
 * Made arcade code based on #define
 * 
 * Revision 1.3  1994/09/16  16:14:52  john
 * Added acrade sequencing.
 * 
 * Revision 1.2  1994/09/16  11:51:01  john
 * Added first version of arcade joystick support;
 * Also fixed some bugs in kconfig.c, such as reading non-present
 * joysticks, which killed frame rate, and not reading key_down_time
 * when in slide mode or bank mode.
 * 
 * Revision 1.1  1994/09/16  09:59:29  john
 * Initial revision
 * 
 * 
 */



#ifndef _ARCADE_H
#define _ARCADE_H

#ifdef ARCADE

extern int Arcade_mode;
extern fix Arcade_timer;

extern void arcade_init();
extern void arcade_read_controls();

#else

#define Arcade_mode 0		//code gets optimized out

#define arcade_init()
#define arcade_read_controls()

#endif

#endif
