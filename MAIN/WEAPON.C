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
 * $Source: f:/miner/source/main/rcs/weapon.c $
 * $Revision: 2.1 $
 * $Author: john $
 * $Date: 1995/03/21 14:38:43 $
 * 
 * Functions for weapons...
 * 
 * $Log: weapon.c $
 * Revision 2.1  1995/03/21  14:38:43  john
 * Ifdef'd out the NETWORK code.
 * 
 * Revision 2.0  1995/02/27  11:27:25  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.54  1995/02/15  15:21:48  mike
 * make smart missile select if mega missiles used up.
 * 
 * 
 * Revision 1.53  1995/02/12  02:12:30  john
 * Fixed bug with state restore making weapon beeps.
 * 
 * Revision 1.52  1995/02/09  20:42:15  mike
 * change weapon autoselect, always autoselect smart, mega.
 * 
 * Revision 1.51  1995/02/07  20:44:26  mike
 * autoselect mega, smart when you pick them up.
 * 
 * Revision 1.50  1995/02/07  13:32:25  rob
 * Added include of multi.h
 * 
 * Revision 1.49  1995/02/07  13:21:33  yuan
 * Fixed 2nd typo
 * 
 * Revision 1.48  1995/02/07  13:16:39  yuan
 * Fixed typo.
 * 
 * Revision 1.47  1995/02/07  12:53:12  rob
 * Added network sound prop. to weapon switch.
 * 
 * Revision 1.46  1995/02/06  15:53:17  mike
 * don't autoselect smart or mega missile when you pick it up.
 * 
 * Revision 1.45  1995/02/02  21:43:34  mike
 * make autoselection better.
 * 
 * Revision 1.44  1995/02/02  16:27:21  mike
 * make concussion missiles trade up.
 * 
 * Revision 1.43  1995/02/01  23:34:57  adam
 * messed with weapon change sounds
 * 
 * Revision 1.42  1995/02/01  17:12:47  mike
 * Make smart missile, mega missile not auto-select.
 * 
 * Revision 1.41  1995/02/01  15:50:54  mike
 * fix bogus weapon selection sound code.
 * 
 * Revision 1.40  1995/01/31  16:16:31  mike
 * Separate smart blobs for robot and player.
 * 
 * Revision 1.39  1995/01/30  21:12:11  mike
 * Use new weapon selection sounds, different for primary and secondary.
 * 
 * Revision 1.38  1995/01/29  13:46:52  mike
 * Don't auto-select fusion cannon when you run out of energy.
 * 
 * Revision 1.37  1995/01/20  11:11:13  allender
 * record weapon changes again.  (John somehow lost my 1.35 changes).
 * 
 * Revision 1.36  1995/01/19  17:00:46  john
 * Made save game work between levels.
 * 
 * Revision 1.34  1995/01/09  17:03:48  mike
 * fix autoselection of weapons.
 * 
 * Revision 1.33  1995/01/05  15:46:31  john
 * Made weapons not rearm when starting a saved game.
 * 
 * Revision 1.32  1995/01/03  12:34:23  mike
 * autoselect next lower weapon if run out of smart or mega missile.
 * 
 * Revision 1.31  1994/12/12  21:39:37  matt
 * Changed vulcan ammo: 10K max, 5K w/weapon, 1250 per powerup
 * 
 * Revision 1.30  1994/12/09  19:55:04  matt
 * Added weapon name in "not available in shareware" message
 * 
 * Revision 1.29  1994/12/06  13:50:24  adam
 * added shareware msg. when choosing 4 top weapons
 * 
 * Revision 1.28  1994/12/02  22:07:13  mike
 * if you gots 19 concussion missiles and you runs over 4, say you picks up 1, not 4, we do the math, see?
 * 
 * Revision 1.27  1994/12/02  20:06:24  matt
 * Made vulcan ammo print at approx 25 times actual
 * 
 * Revision 1.26  1994/12/02  15:05:03  matt
 * Fixed bogus weapon constants and arrays
 * 
 * Revision 1.25  1994/12/02  10:50:34  yuan
 * Localization
 * 
 * Revision 1.24  1994/11/29  15:48:28  matt
 * selecting weapon now makes sound
 * 
 * Revision 1.23  1994/11/28  11:26:58  matt
 * Cleaned up hud message printing for picking up weapons
 * 
 * Revision 1.22  1994/11/27  23:13:39  matt
 * Made changes for new mprintf calling convention
 * 
 * Revision 1.21  1994/11/12  16:38:34  mike
 * clean up default ammo stuff.
 * 
 * Revision 1.20  1994/11/07  17:41:18  mike
 * messages for when you try to fire a weapon you don't have or don't have ammo for.
 * 
 * Revision 1.19  1994/10/21  20:40:05  mike
 * fix double vulcan ammo.
 * 
 * Revision 1.18  1994/10/20  09:49:05  mike
 * kill messages no one liked...*sniff* *sniff*
 * 
 * Revision 1.17  1994/10/19  11:17:07  mike
 * Limit amount of player ammo.
 * 
 * Revision 1.16  1994/10/12  08:04:18  mike
 * Fix proximity/homing confusion.
 * 
 * Revision 1.15  1994/10/11  18:27:58  matt
 * Changed auto selection of secondary weapons
 * 
 * Revision 1.14  1994/10/08  23:37:54  matt
 * Don't pick up weapons you already have; also fixed auto_select bug
 * for seconary weapons
 * 
 * Revision 1.13  1994/10/08  14:55:47  matt
 * Fixed bug that selected vulcan cannon when picked up ammo, even though
 * you didn't have the weapon.
 * 
 * Revision 1.12  1994/10/08  12:50:32  matt
 * Fixed bug that let you select weapons you don't have
 * 
 * Revision 1.11  1994/10/07  23:37:56  matt
 * Made weapons select when pick up better one
 * 
 * Revision 1.10  1994/10/07  16:02:08  matt
 * Fixed problem with weapon auto-select
 * 
 * Revision 1.9  1994/10/05  17:00:20  matt
 * Made player_has_weapon() public and moved constants to header file
 * 
 * Revision 1.8  1994/09/26  11:27:13  mike
 * Fix auto selection of weapon when you run out of ammo.
 * 
 * Revision 1.7  1994/09/13  16:40:45  mike
 * Add rearm delay and missile firing delay.
 * 
 * Revision 1.6  1994/09/13  14:43:12  matt
 * Added cockpit weapon displays
 * 
 * Revision 1.5  1994/09/03  15:23:06  mike
 * Auto select next weaker weapon when one runs out, clean up code.
 * 
 * Revision 1.4  1994/09/02  16:38:19  mike
 * Eliminate a pile of arrays, associate weapon data with Weapon_info.
 * 
 * Revision 1.3  1994/09/02  11:57:10  mike
 * Add a bunch of stuff, I forget what.
 * 
 * Revision 1.2  1994/06/03  16:26:32  john
 * Initial version.
 * 
 * Revision 1.1  1994/06/03  14:40:43  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: weapon.c 2.1 1995/03/21 14:38:43 john Exp $";
#pragma on (unreferenced)

#include "game.h"
#include "weapon.h"
#include "mono.h"
#include "player.h"
#include "gauges.h"
#include "error.h"
#include "sounds.h"
#include "text.h"
#include "powerup.h"
#include "newdemo.h"
#include "multi.h"

//	Note, only Vulcan cannon requires ammo.
//ubyte	Default_primary_ammo_level[MAX_PRIMARY_WEAPONS] = {255, 0, 255, 255, 255};
//ubyte	Default_secondary_ammo_level[MAX_SECONDARY_WEAPONS] = {3, 0, 0, 0, 0};

//	Convert primary weapons to indices in Weapon_info array.
ubyte Primary_weapon_to_weapon_info[MAX_PRIMARY_WEAPONS] = {0, 11, 12, 13, 14};
ubyte Secondary_weapon_to_weapon_info[MAX_SECONDARY_WEAPONS] = {8, 15, 16, 17, 18};

int Primary_ammo_max[MAX_PRIMARY_WEAPONS] = {0, VULCAN_AMMO_MAX, 0, 0, 0};
ubyte Secondary_ammo_max[MAX_SECONDARY_WEAPONS] = {20, 10, 10, 5, 5};

weapon_info Weapon_info[MAX_WEAPON_TYPES];
int	N_weapon_types=0;
byte	Primary_weapon, Secondary_weapon;

//char	*Primary_weapon_names[MAX_PRIMARY_WEAPONS] = {
//	"Laser Cannon",
//	"Vulcan Cannon",
//	"Spreadfire Cannon",
//	"Plasma Cannon",
//	"Fusion Cannon"
//};

//char	*Secondary_weapon_names[MAX_SECONDARY_WEAPONS] = {
//	"Concussion Missile",
//	"Homing Missile",
//	"Proximity Bomb",
//	"Smart Missile",
//	"Mega Missile"
//};

//char	*Primary_weapon_names_short[MAX_PRIMARY_WEAPONS] = {
//	"Laser",
//	"Vulcan",
//	"Spread",
//	"Plasma",
//	"Fusion"
//};

//char	*Secondary_weapon_names_short[MAX_SECONDARY_WEAPONS] = {
//	"Concsn\nMissile",
//	"Homing\nMissile",
//	"Proxim.\nBomb",
//	"Smart\nMissile",
//	"Mega\nMissile"
//};

//	------------------------------------------------------------------------------------
//	Return:
// Bits set:
//		HAS_WEAPON_FLAG
//		HAS_ENERGY_FLAG
//		HAS_AMMO_FLAG	
// See weapon.h for bit values
int player_has_weapon(int weapon_num, int secondary_flag)
{
	int	return_value = 0;
	int	weapon_index;

	//	Hack! If energy goes negative, you can't fire a weapon that doesn't require energy.
	//	But energy should not go negative (but it does), so find out why it does!
	if (Players[Player_num].energy < 0)
		Players[Player_num].energy = 0;

	if (!secondary_flag) {
		weapon_index = Primary_weapon_to_weapon_info[weapon_num];

		if (Players[Player_num].primary_weapon_flags & (1 << weapon_num))
			return_value |= HAS_WEAPON_FLAG;

		if (Weapon_info[weapon_index].ammo_usage <= Players[Player_num].primary_ammo[weapon_num])
			return_value |= HAS_AMMO_FLAG;

		if (Weapon_info[weapon_index].energy_usage <= Players[Player_num].energy)
			return_value |= HAS_ENERGY_FLAG;

	} else {
		weapon_index = Secondary_weapon_to_weapon_info[weapon_num];

		if (Players[Player_num].secondary_weapon_flags & (1 << weapon_num))
			return_value |= HAS_WEAPON_FLAG;

		if (Weapon_info[weapon_index].ammo_usage <= Players[Player_num].secondary_ammo[weapon_num])
			return_value |= HAS_AMMO_FLAG;

		if (Weapon_info[weapon_index].energy_usage <= Players[Player_num].energy)
			return_value |= HAS_ENERGY_FLAG;
	}


	return return_value;
}

//	------------------------------------------------------------------------------------
//if message flag set, print message saying selected
select_weapon(int weapon_num, int secondary_flag, int print_message, int wait_for_rearm)
{
	char	*weapon_name;

#ifndef SHAREWARE
	if (Newdemo_state==ND_STATE_RECORDING )
		newdemo_record_player_weapon(secondary_flag, weapon_num);
#endif

	if (!secondary_flag) {
		if (Primary_weapon != weapon_num) {
			if (wait_for_rearm) digi_play_sample_once( SOUND_GOOD_SELECTION_PRIMARY, F1_0 );
			#ifdef NETWORK
			if (Game_mode & GM_MULTI)	{
				if (wait_for_rearm) multi_send_play_sound(SOUND_GOOD_SELECTION_PRIMARY, F1_0);
			}
			#endif
			if (wait_for_rearm)
				Next_laser_fire_time = GameTime + REARM_TIME;
			else
				Next_laser_fire_time = 0;
			Global_laser_firing_count = 0;
		} else 	{
			if (wait_for_rearm) digi_play_sample( SOUND_ALREADY_SELECTED, F1_0 );
		}
		Primary_weapon = weapon_num;
		weapon_name = PRIMARY_WEAPON_NAMES(weapon_num);
	} else {

		if (Secondary_weapon != weapon_num) {
			if (wait_for_rearm) digi_play_sample_once( SOUND_GOOD_SELECTION_SECONDARY, F1_0 );
			#ifdef NETWORK
			if (Game_mode & GM_MULTI)	{
				if (wait_for_rearm) multi_send_play_sound(SOUND_GOOD_SELECTION_PRIMARY, F1_0);
			}
			#endif
			if (wait_for_rearm)
				Next_missile_fire_time = GameTime + REARM_TIME;
			else
				Next_missile_fire_time = 0;
			Global_missile_firing_count = 0;
		} else	{
			if (wait_for_rearm) digi_play_sample_once( SOUND_ALREADY_SELECTED, F1_0 );
		}
		Secondary_weapon = weapon_num;
		weapon_name = SECONDARY_WEAPON_NAMES(weapon_num);
	}

	if (print_message)
		HUD_init_message("%s %s", weapon_name, TXT_SELECTED);

}

//	------------------------------------------------------------------------------------
//	Select a weapon, primary or secondary.
void do_weapon_select(int weapon_num, int secondary_flag)
{
	int	weapon_status = player_has_weapon(weapon_num, secondary_flag);
	char	*weapon_name;


	#ifdef SHAREWARE	// do special hud msg. for picking registered weapon in shareware version.
	if (weapon_num >= NUM_SHAREWARE_WEAPONS) {
		weapon_name = secondary_flag?SECONDARY_WEAPON_NAMES(weapon_num):PRIMARY_WEAPON_NAMES(weapon_num);
		HUD_init_message("%s %s!", weapon_name,TXT_NOT_IN_SHAREWARE);
		digi_play_sample( SOUND_BAD_SELECTION, F1_0 );
		return;
	}
	#endif

	if (!secondary_flag) {
		weapon_name = PRIMARY_WEAPON_NAMES(weapon_num);
		if ((weapon_status & HAS_WEAPON_FLAG) == 0) {
			HUD_init_message("%s %s!", TXT_DONT_HAVE, weapon_name);
			digi_play_sample( SOUND_BAD_SELECTION, F1_0 );
			return;
		}
		else if ((weapon_status & HAS_AMMO_FLAG) == 0) {
			HUD_init_message("%s %s!", TXT_DONT_HAVE_AMMO, weapon_name);
			digi_play_sample( SOUND_BAD_SELECTION, F1_0 );
			return;
		}
	}
	else {
		weapon_name = SECONDARY_WEAPON_NAMES(weapon_num);
		if (weapon_status != HAS_ALL) {
			HUD_init_message("%s %s%s",TXT_HAVE_NO, weapon_name, TXT_SX);
			digi_play_sample( SOUND_BAD_SELECTION, F1_0 );
			return;
		}
	}

	select_weapon(weapon_num, secondary_flag, 1, 1);
}

//	----------------------------------------------------------------------------------------
//	Automatically select next best weapon if unable to fire current weapon.
// Weapon type: 0==primary, 1==secondary
void auto_select_weapon(int weapon_type)
{
	int	r;

	if (weapon_type==0) {
		r = player_has_weapon(Primary_weapon, 0);
		if (r != HAS_ALL) {
			int	cur_weapon;
			int	try_again = 1;
	
			cur_weapon = Primary_weapon;
	
			while (try_again) {
				cur_weapon--;
				if (cur_weapon < 0)
					cur_weapon = MAX_PRIMARY_WEAPONS-1;
	
				//	Hack alert!  Because the fusion uses 0 energy at the end (it's got the weird chargeup)
				//	it looks like it takes 0 to fire, but it doesn't, so never auto-select.
				if (cur_weapon == FUSION_INDEX)
					continue;

				if (cur_weapon == Primary_weapon) {
					HUD_init_message(TXT_NO_PRIMARY);
					try_again = 0;				// Tried all weapons!
					select_weapon(0, 0, 0, 1);
				} else if (player_has_weapon(cur_weapon, 0) == HAS_ALL) {
					select_weapon(cur_weapon, 0, 1, 1 );
					try_again = 0;
				}
			}
		}

	} else {

		Assert(weapon_type==1);

		if (Secondary_weapon != PROXIMITY_INDEX) {
			if (!(player_has_weapon(Secondary_weapon, 1) == HAS_ALL)) {
				if (Secondary_weapon > SMART_INDEX)
					if (player_has_weapon(SMART_INDEX, 1) == HAS_ALL) {
						select_weapon(SMART_INDEX, 1, 1, 1);
						goto weapon_selected;
					}
				if (player_has_weapon(HOMING_INDEX, 1) == HAS_ALL)
					select_weapon(HOMING_INDEX, 1, 1, 1);
				else if (player_has_weapon(CONCUSSION_INDEX, 1) == HAS_ALL)
					select_weapon(CONCUSSION_INDEX, 1, 1, 1);
weapon_selected: ;
			}
		}
	}

}

#ifndef RELEASE

//	----------------------------------------------------------------------------------------
//	Show player which weapons he has, how much ammo...
//	Looks like a debug screen now because it writes to mono screen, but that will change...
void show_weapon_status(void)
{
	int	i;

	for (i=0; i<MAX_PRIMARY_WEAPONS; i++) {
		if (Players[Player_num].primary_weapon_flags & (1 << i))
			mprintf((0, "HAVE"));
		else
			mprintf((0, "    "));

		mprintf((0, "  Weapon: %20s, charges: %4i\n", PRIMARY_WEAPON_NAMES(i), Players[Player_num].primary_ammo[i]));
	}

	mprintf((0, "\n"));
	for (i=0; i<MAX_SECONDARY_WEAPONS; i++) {
		if (Players[Player_num].secondary_weapon_flags & (1 << i))
			mprintf((0, "HAVE"));
		else
			mprintf((0, "    "));

		mprintf((0, "  Weapon: %20s, charges: %4i\n", SECONDARY_WEAPON_NAMES(i), Players[Player_num].secondary_ammo[i]));
	}

	mprintf((0, "\n"));
	mprintf((0, "\n"));

}

#endif

//	---------------------------------------------------------------------
//called when one of these weapons is picked up
//when you pick up a secondary, you always get the weapon & ammo for it
//	Returns true if powerup picked up, else returns false.
int pick_up_secondary(int weapon_index,int count)
{
	int	num_picked_up;

	if (Players[Player_num].secondary_ammo[weapon_index] >= Secondary_ammo_max[weapon_index]) {
		HUD_init_message("%s %i %ss!", TXT_ALREADY_HAVE, Players[Player_num].secondary_ammo[weapon_index],SECONDARY_WEAPON_NAMES(weapon_index));
		return 0;
	}

	Players[Player_num].secondary_weapon_flags |= (1<<weapon_index);
	Players[Player_num].secondary_ammo[weapon_index] += count;

	num_picked_up = count;
	if (Players[Player_num].secondary_ammo[weapon_index] > Secondary_ammo_max[weapon_index]) {
		num_picked_up = count - (Players[Player_num].secondary_ammo[weapon_index] - Secondary_ammo_max[weapon_index]);
		Players[Player_num].secondary_ammo[weapon_index] = Secondary_ammo_max[weapon_index];
	}

	//if you pick up a homing, and you're currently using concussion,
	//and you had no homings before, then upgrade
	if ((Secondary_weapon<weapon_index) && (weapon_index != PROXIMITY_INDEX)) //  && (Players[Player_num].secondary_ammo[weapon_index] == count))
		select_weapon(weapon_index,1, 0, 1);

	//if you pick up a concussion, and you've got homing (or smart or mega) selected but are out,
	//then select concussion
	if ((weapon_index != PROXIMITY_INDEX) && (Players[Player_num].secondary_ammo[Secondary_weapon] == 0))
		select_weapon(weapon_index,1, 0, 1);

	//note: flash for all but concussion was 7,14,21
	if (count>1) {
		PALETTE_FLASH_ADD(15,15,15);
		HUD_init_message("%d %s%s",num_picked_up,SECONDARY_WEAPON_NAMES(weapon_index), TXT_SX);
	}
	else {
		PALETTE_FLASH_ADD(10,10,10);
		HUD_init_message("%s!",SECONDARY_WEAPON_NAMES(weapon_index));
	}

	return 1;
}

//called when a primary weapon is picked up
//returns true if actually picked up
int pick_up_primary(int weapon_index)
{
	ubyte old_flags = Players[Player_num].primary_weapon_flags;
	ubyte flag = 1<<weapon_index;

	if (Players[Player_num].primary_weapon_flags & flag) {		//already have
		HUD_init_message("%s %s!", TXT_ALREADY_HAVE_THE, PRIMARY_WEAPON_NAMES(weapon_index));
		return 0;
	}

	Players[Player_num].primary_weapon_flags |= flag;

	if (!(old_flags & flag) && weapon_index>Primary_weapon)
		select_weapon(weapon_index,0,0,1);

	PALETTE_FLASH_ADD(7,14,21);
	HUD_init_message("%s!",PRIMARY_WEAPON_NAMES(weapon_index));

	return 1;
}

//called when ammo (for the vulcan cannon) is picked up
//	Return true if ammo picked up, else return false.
int pick_up_ammo(int class_flag,int weapon_index,int ammo_count)
{
	int old_ammo=class_flag;		//kill warning

	Assert(class_flag==CLASS_PRIMARY && weapon_index==VULCAN_INDEX);

	if (Players[Player_num].primary_ammo[weapon_index] == Primary_ammo_max[weapon_index])
		return 0;

	old_ammo = Players[Player_num].primary_ammo[weapon_index];

	Players[Player_num].primary_ammo[weapon_index] += ammo_count;

	if (Players[Player_num].primary_ammo[weapon_index] > Primary_ammo_max[weapon_index])
		Players[Player_num].primary_ammo[weapon_index] = Primary_ammo_max[weapon_index];

	if (Players[Player_num].primary_weapon_flags&(1<<weapon_index) && weapon_index>Primary_weapon && old_ammo==0)
		select_weapon(weapon_index,0,0,1);

	return 1;
}


