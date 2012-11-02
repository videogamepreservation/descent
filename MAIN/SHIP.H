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
 * $Source: f:/miner/source/main/rcs/ship.h $
 * $Revision: 1.2 $
 * $Author: yuan $
 * $Date: 1994/03/01 18:12:56 $
 * 
 * Functions that affect the ship (Player_stats)
 * 
 * $Log: ship.h $
 * Revision 1.2  1994/03/01  18:12:56  yuan
 * Walls, switches, and triggers...
 * 
 * Revision 1.1  1994/02/25  15:52:13  yuan
 * Initial revision
 * 
 * 
 */



#ifndef _SHIP_H
#define _SHIP_H

#include "game.h"

void ship_damage(fix damage);

void ship_repair(fix damage);

void ship_repair_full();

void ship_energy_full();

void ship_energy_drain(fix damage);

void ship_energy_empty();

#endif

