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
 * $Source: f:/miner/source/bios/rcs/mousec.c $
 * $Revision: 1.1 $
 * $Author: john $
 * $Date: 1994/08/24 12:19:13 $
 * 
 * Routines to access a mouse..
 * 
 * $Log: mousec.c $
 * Revision 1.1  1994/08/24  12:19:13  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: mousec.c 1.1 1994/08/24 12:19:13 john Exp $";
#pragma on (unreferenced)

#define MAX_BUTTONS	11

#define MB_LEFT			0
#define MB_RIGHT			1
#define MB_MIDDLE			2
#define MB_Z_UP			3
#define MB_Z_DOWN			4
#define MB_PITCH_LEFT	5
#define MB_PITCH_RIGHT	6
#define MB_BANK_LEFT		7
#define MB_BANK_RIGHT	8
#define MB_HEAD_LEFT		9
#define MB_HEAD_RIGHT	10

typedef struct mouse_info {
	ubyte		pressed[MAX_BUTTONS];
	fix		time_went_down[MAX_BUTTONS];
	fix		time_held_down[MAX_BUTTONS];
	uint		num_downs[MAX_BUTTONS];
	uint		num_ups[MAX_BUTTONS];
} mouse_info;
 

int
	 

