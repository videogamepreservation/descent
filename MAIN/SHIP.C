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
 * $Source: f:/miner/source/main/rcs/ship.c $
 * $Revision: 1.2 $
 * $Author: yuan $
 * $Date: 1994/03/01 18:12:37 $
 * 
 * Functions to affect the ship.
 * 
 * $Log: ship.c $
 * Revision 1.2  1994/03/01  18:12:37  yuan
 * Wallswitches, triggers, and more!
 * 
 * Revision 1.1  1994/02/25  15:51:53  yuan
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: ship.c 1.2 1994/03/01 18:12:37 yuan Exp $";
#pragma on (unreferenced)

#include "inferno.h"
#include "game.h"

void ship_damage(fix damage)
{
	Player_stats.shields -= damage;
}

void ship_repair(fix damage)
{
	Player_stats.shields += damage;
}

void ship_repair_full()
{
	Player_stats.shields = INITIAL_SHIELDS;
}

void ship_energy_full()
{
	Player_stats.energy = INITIAL_ENERGY;
}

void ship_energy_drain(fix damage)
{
	Player_stats.energy -= damage;
}

void ship_energy_empty()
{
	Player_stats.energy = 0;
}



