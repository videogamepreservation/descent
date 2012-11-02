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
 * $Source: f:/miner/source/main/rcs/kconfig.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:29:38 $
 * 
 * Prototypes for reading controls
 * 
 * $Log: kconfig.h $
 * Revision 2.0  1995/02/27  11:29:38  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.23  1995/01/12  11:41:44  john
 * Added external control reading.
 * 
 * Revision 1.22  1994/12/07  16:15:30  john
 * Added command to check to see if a joystick axes has been used.
 * 
 * Revision 1.21  1994/11/29  03:46:29  john
 * Added joystick sensitivity; Added sound channels to detail menu.  Removed -maxchannels
 * command line arg.
 * 
 * Revision 1.20  1994/11/22  16:54:45  mike
 * autorepeat on missiles.
 * 
 * Revision 1.19  1994/11/07  14:01:27  john
 * Changed the gamma correction sequencing.
 * 
 * Revision 1.18  1994/11/01  16:40:02  john
 * Added Gamma correction.
 * 
 * Revision 1.17  1994/10/25  23:09:24  john
 * Made the automap key configurable.
 * 
 * Revision 1.16  1994/10/24  19:56:51  john
 * Made the new user setup prompt for config options.
 * 
 * Revision 1.15  1994/10/24  17:44:18  john
 * Added stereo channel reversing.
 * 
 * Revision 1.14  1994/10/22  13:19:33  john
 * Took out toggle primary/secondary weapons.  Fixed black
 * background for 'axes' and 'buttons' text.
 * 
 * Revision 1.13  1994/10/17  13:06:51  john
 * Moved the descent.cfg info into the player config file.
 * 
 * Revision 1.12  1994/10/14  12:14:47  john
 * Changed code so that by doing DEL+F12 saves the current kconfig
 * values as default. Added support for drop_bomb key.  Took out
 * unused slots for keyboard.  Made keyboard use control_type of 0
 * save slots.
 * 
 * Revision 1.11  1994/10/13  19:21:33  john
 * Added separate config saves for different devices.
 * Made all the devices work together better, such as mice won't
 * get read when you're playing with the joystick.
 * 
 * Revision 1.10  1994/10/13  15:18:41  john
 * Started ripping out old afterburner, show message, show automap
 * keys in the keyboard config stuff.
 * 
 * Revision 1.9  1994/10/13  11:35:27  john
 * Made Thrustmaster FCS Hat work.  Put a background behind the
 * keyboard configure.  Took out turn_sensitivity.  Changed sound/config
 * menu to new menu. Made F6 be calibrate joystick.
 * 
 * Revision 1.8  1994/10/06  14:10:50  matt
 * New function reset_cruise()
 * 
 * Revision 1.7  1994/10/03  14:58:25  john
 * Added rear_view_down_state so that the rear view can
 * work like the automap.
 * 
 * Revision 1.6  1994/09/30  12:37:25  john
 * Added midi,digi volume to configuration.
 * 
 * Revision 1.5  1994/09/19  18:49:59  john
 * Added switch to disable joystick.
 * 
 * Revision 1.4  1994/09/15  16:11:21  john
 * Added support for VFX1 head tracking. Fixed bug with memory over-
 * write when using stereo mode.
 * 
 * Revision 1.3  1994/09/12  11:47:38  john
 * Made stupid cruise work better.  Make kconfig values get
 * read/written to disk.
 * 
 * Revision 1.2  1994/09/10  15:46:55  john
 * First version of new keyboard configuration.
 * 
 * Revision 1.1  1994/09/10  13:51:40  john
 * Initial revision
 * 
 * 
 */



#ifndef _KCONFIG_H
#define _KCONFIG_H

typedef struct control_info {
	fix	pitch_time;						
	fix	vertical_thrust_time;
	fix	heading_time;
	fix	sideways_thrust_time;
	fix	bank_time;
	fix	forward_thrust_time;
	
	ubyte	rear_view_down_count;	
	ubyte	rear_view_down_state;	
	
	ubyte	fire_primary_down_count;
	ubyte	fire_primary_state;
	ubyte	fire_secondary_state;
	ubyte	fire_secondary_down_count;
	ubyte	fire_flare_down_count;

	ubyte	drop_bomb_down_count;	

	ubyte	automap_down_count;
	ubyte	automap_state;
		
} control_info;

extern control_info Controls;
extern void controls_read_all();
extern void kconfig(int n, char * title );

extern ubyte Config_digi_volume;
extern ubyte Config_midi_volume;
extern ubyte Config_control_type;
extern ubyte Config_channels_reversed;
extern ubyte Config_joystick_sensitivity;

#define CONTROL_NONE 0
#define CONTROL_JOYSTICK 1
#define CONTROL_FLIGHTSTICK_PRO 2
#define CONTROL_THRUSTMASTER_FCS 3
#define CONTROL_GRAVIS_GAMEPAD 4
#define CONTROL_MOUSE 5
#define CONTROL_CYBERMAN 6
#define CONTROL_MAX_TYPES 7

#define NUM_KEY_CONTROLS 46
#define NUM_OTHER_CONTROLS 27
#define MAX_CONTROLS 50

extern ubyte kconfig_settings[CONTROL_MAX_TYPES][MAX_CONTROLS];
extern ubyte default_kconfig_settings[CONTROL_MAX_TYPES][MAX_CONTROLS];

extern char *control_text[CONTROL_MAX_TYPES];

extern void kc_set_controls();

// Tries to use vfx1 head tracking.
void kconfig_sense_init();

//set the cruise speed to zero
extern void reset_cruise(void);

extern int kconfig_is_axes_used(int axis);

extern void kconfig_init_external_controls(int intno, int address);


#endif

