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
 * $Source: f:/miner/source/main/rcs/victor.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:27:31 $
 * 
 * Prototypes for the VictorMaxx VR helment.
 * 
 * $Log: victor.h $
 * Revision 2.0  1995/02/27  11:27:31  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.4  1994/11/12  14:46:55  john
 * Added support for victor head tracking.
 * 
 * Revision 1.3  1994/07/21  21:31:12  john
 * First cheapo version of VictorMaxx tracking.
 * 
 * Revision 1.2  1994/07/20  21:04:32  john
 * Add VictorMax VR helment support.
 * 
 * Revision 1.1  1994/07/20  20:49:19  john
 * Initial revision
 * 
 * 
 */



#ifndef _VICTOR_H
#define _VICTOR_H

#include "fix.h"

// Sets up the victor card.
void victor_init_graphics(); 

extern int Victor_headset_installed;		// Set to one if init_tracking has been called successfully

// Initializes VictorMaxx headset tracking on serial port 1 or 2
extern void victor_init_tracking(int serial_port);
// Stops reading victormaxx
extern void victor_close_tracking();

// Reads headset... Returns 3 values, yaw=0-360, roll & pitch=-45 to 45
extern void victor_read_headset( fix *yaw, fix *pitch, fix *roll );
extern void victor_read_headset_filtered( fix *yaw, fix *pitch, fix *roll );

#endif

