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
 * $Source: f:/miner/source/main/rcs/arcade.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:29:12 $
 * 
 * Routines for arcade version
 * 
 * $Log: arcade.c $
 * Revision 2.0  1995/02/27  11:29:12  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.7  1994/09/21  15:48:52  matt
 * Made digital joystick less "soft"
 * 
 * Revision 1.6  1994/09/16  18:50:47  matt
 * Read vfx1 head sensing when in arcade mode
 * 
 * Revision 1.5  1994/09/16  16:15:00  john
 * Added acrade sequencing.
 * 
 * Revision 1.4  1994/09/16  12:52:07  mike
 * fix compile errors.
 * 
 * Revision 1.3  1994/09/16  12:36:48  john
 * Fixed ramping pitch, heading.
 * 
 * Revision 1.2  1994/09/16  11:51:00  john
 * Added first version of arcade joystick support;
 * Also fixed some bugs in kconfig.c, such as reading non-present
 * joysticks, which killed frame rate, and not reading key_down_time
 * when in slide mode or bank mode.
 * 
 * Revision 1.1  1994/09/16  09:59:16  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: arcade.c 2.0 1995/02/27 11:29:12 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <stdarg.h>
#include <ctype.h>

#include "error.h"
#include "types.h"
#include "gr.h"
#include "mono.h"
#include "key.h"
#include "palette.h"
#include "game.h"
#include "gamefont.h"
#include "iff.h"
#include "mem.h"
#include "joy.h"
#include "mouse.h"
#include "kconfig.h"
#include "gauges.h"
#include "joydefs.h"
#include "vfx1.h"
#include "render.h"
#include "arcade.h"

int Arcade_mode = 0;
fix Arcade_timer = 0;

#define	PH_SCALE	1	//8

void arcade_init()
{
	Arcade_mode = 1;
}

extern read_vfx1_sense();

void arcade_read_controls()
{
	fix pitch, heading;

	if (!Arcade_mode) return;

	pitch = Controls.pitch_time;
	heading = Controls.heading_time;
	memset( &Controls, 0, sizeof(control_info) );
	Controls.pitch_time = pitch;
	Controls.heading_time = heading;
	
	pitch = heading = 0;
				
//----------- Read fire_primary_down_count
	Controls.fire_primary_down_count += joy_get_button_down_cnt(20);

//----------- Read fire_primary_state
	Controls.fire_primary_state |= joy_get_button_state(20);

//------------ Read forward_thrust_time ---------
	Controls.forward_thrust_time += joy_get_button_down_time( 21 );
	Controls.forward_thrust_time -= joy_get_button_down_time( 22 );

//------------ Read pitch_time -----------
	pitch += joy_get_button_down_time(24)/(PH_SCALE*2);
	pitch -= joy_get_button_down_time(25)/(PH_SCALE*2);

//------------ Read heading_time -----------
	heading += joy_get_button_down_time(27)/PH_SCALE;
	heading -= joy_get_button_down_time(26)/PH_SCALE;

	//@@if ( pitch!=0)	{
	//@@	if ((pitch>0) && (Controls.pitch_time<0))
	//@@		Controls.pitch_time=0;
	//@@	if ((pitch<0) && (Controls.pitch_time>0))
	//@@		Controls.pitch_time=0;
	//@@	Controls.pitch_time += pitch;
	//@@} else
	//@@	Controls.pitch_time = 0;
	//@@if ( heading!=0)	{
	//@@	if ((heading>0) && (Controls.heading_time<0))
	//@@		Controls.heading_time=0;
	//@@	if ((heading<0) && (Controls.heading_time>0))
	//@@		Controls.heading_time=0;
	//@@	Controls.heading_time += heading;
	//@@} else
	//@@	Controls.heading_time = 0;

	Controls.pitch_time = pitch;
	Controls.heading_time = heading;

//------- saturate to +/- frame_time -----------
	if (Controls.pitch_time>FrameTime) Controls.pitch_time=FrameTime;
	if (Controls.pitch_time<-FrameTime) Controls.pitch_time=-FrameTime;
	if (Controls.heading_time>FrameTime) Controls.heading_time=FrameTime;
	if (Controls.heading_time<-FrameTime) Controls.heading_time=-FrameTime;

	read_vfx1_sense();
}


