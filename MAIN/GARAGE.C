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
 * $Source: f:/miner/source/main/rcs/garage.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:30:32 $
 * 
 * .
 * 
 * $Log: garage.c $
 * Revision 2.0  1995/02/27  11:30:32  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.2  1994/08/17  16:50:28  john
 * Added damaging fireballs, missiles.
 * 
 * Revision 1.1  1994/08/16  12:45:12  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: garage.c 2.0 1995/02/27 11:30:32 john Exp $";
#pragma on (unreferenced)

#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>

#include "types.h"
#include "args.h"
#include "timer.h"
#include "mono.h"
#include "ipx.h"
#include "newmenu.h"
#include "key.h"
#include "gauges.h"
#include "object.h"
#include "error.h"
#include "lzw.h"
#include "netmisc.h"
#include "laser.h"
#include "gamesave.h"
#include "gamemine.h"
#include "player.h"
#include "gameseq.h"
#include "fireball.h"

void garage_tweak_ship()
{
	int r;
	newmenu_item m[20];
	char mass[10],drag[10];
	char low_thrust[10],high_thrust[10],reverse_thrust[10];
	char rot_speed[10];		//arbitary units

	sprintf( mass, "%.1f", f2fl(Player_ship->mass) );
	sprintf( drag, "%.1f", f2fl(Player_ship->drag) );
	sprintf( low_thrust, "%.1f", f2fl(Player_ship->low_thrust) );
	sprintf( high_thrust, "%.1f", f2fl(Player_ship->high_thrust) );
	sprintf( reverse_thrust, "%.1f", f2fl(Player_ship->reverse_thrust) );
	sprintf( rot_speed, "%d", Player_ship->rot_speed );
	
	m[0].type=NM_TYPE_TEXT; m[0].text = "Mass (Default:4.0)"; 
	m[1].type=NM_TYPE_INPUT; m[1].text = mass; m[1].text_len = 10;

	m[2].type=NM_TYPE_TEXT; m[2].text = "Drag (Default:2.0)"; 
	m[3].type=NM_TYPE_INPUT; m[3].text = drag; m[3].text_len = 10;

	m[4].type=NM_TYPE_TEXT; m[4].text = "Low thrust (Default:200.0)"; 
	m[5].type=NM_TYPE_INPUT; m[5].text = low_thrust; m[5].text_len = 10;

	m[6].type=NM_TYPE_TEXT; m[6].text = "High thrust (Default:500.0)"; 
	m[7].type=NM_TYPE_INPUT; m[7].text = high_thrust; m[7].text_len = 10;

	m[8].type=NM_TYPE_TEXT; m[8].text = "Reverse thrust (Default:300.0)"; 
	m[9].type=NM_TYPE_INPUT; m[9].text = reverse_thrust; m[9].text_len = 10;

	m[10].type=NM_TYPE_TEXT; m[10].text = "Rotation speed (Default:20)"; 
	m[11].type=NM_TYPE_INPUT; m[11].text = rot_speed; m[11].text_len = 10;

	r = newmenu_do( "Garage", NULL, 12, m, NULL );

	if ( r < 0 ) return;

	Player_ship->mass = fl2f(atof(mass));
	Player_ship->drag = fl2f(atof(drag));
	Player_ship->low_thrust = fl2f(atof(low_thrust));
	Player_ship->high_thrust = fl2f(atof(high_thrust));
	Player_ship->reverse_thrust = fl2f(atof(reverse_thrust));
	Player_ship->rot_speed = (int)atof(rot_speed);

}















