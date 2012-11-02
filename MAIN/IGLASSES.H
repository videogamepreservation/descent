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
 * $Source: f:/miner/source/main/rcs/iglasses.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:28:38 $
 * 
 * Routines for the i-glasses VR stuff.
 * 
 * $Log: iglasses.h $
 * Revision 2.0  1995/02/27  11:28:38  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.2  1995/02/09  22:01:01  john
 * Added i-glasses tracking.
 * 
 * Revision 1.1  1995/02/09  15:54:01  john
 * Initial revision
 * 
 * 
 */

 

#ifndef _IGLASSES_H
#define _IGLASSES_H

#include "fix.h"

extern int iglasses_headset_installed;

void iglasses_init_tracking(int serial_port);
int iglasses_read_headset( fix *yaw, fix *pitch, fix *roll );

#endif

