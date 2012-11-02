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
 * $Source: f:/miner/source/main/rcs/playsave.c $
 * $Revision: 2.3 $
 * $Author: john $
 * $Date: 1995/05/26 16:16:23 $
 * 
 * Functions to load & save player games
 * 
 * $Log: playsave.c $
 * Revision 2.3  1995/05/26  16:16:23  john
 * Split SATURN into define's for requiring cd, using cd, etc.
 * Also started adding all the Rockwell stuff.
 * 
 * Revision 2.2  1995/03/24  17:48:21  john
 * Made player files from saturn excrement the highest level for
 * normal descent levels.
 * 
 * Revision 2.1  1995/03/21  14:38:49  john
 * Ifdef'd out the NETWORK code.
 * 
 * Revision 2.0  1995/02/27  11:27:59  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.57  1995/02/13  20:34:55  john
 * Lintized
 * 
 * Revision 1.56  1995/02/13  13:23:24  john
 * Fixed bug with new player joystick selection.
 * 
 * Revision 1.55  1995/02/13  12:01:19  john
 * Fixed bug with joystick throttle still asking for 
 * calibration with new pilots.
 * 
 * Revision 1.54  1995/02/13  10:29:12  john
 * Fixed bug with creating new player not resetting everything to default.
 * 
 * Revision 1.53  1995/02/03  10:58:46  john
 * Added code to save shareware style saved games into new format...
 * Also, made new player file format not have the saved game array in it.
 * 
 * Revision 1.52  1995/02/02  21:09:28  matt
 * Let player start of level 8 if he made it to level 7 in the shareware
 * 
 * Revision 1.51  1995/02/02  18:50:14  john
 * Added warning for FCS when new pilot chooses.
 * 
 * Revision 1.50  1995/02/02  11:21:34  john
 * Made joystick calibrate when new user selects.
 * 
 * Revision 1.49  1995/02/01  18:06:38  rob
 * Put defaults macros into descent.tex
 * 
 * Revision 1.48  1995/01/25  14:37:53  john
 * Made joystick only prompt for calibration once...
 * 
 * Revision 1.47  1995/01/24  19:37:12  matt
 * Took out incorrect mprintf
 * 
 * Revision 1.46  1995/01/22  18:57:22  matt
 * Made player highest level work with missions
 * 
 * Revision 1.45  1995/01/21  16:36:05  matt
 * Made starting level system work for now, pending integration with
 * mission code.
 * 
 * Revision 1.44  1995/01/20  22:47:32  matt
 * Mission system implemented, though imcompletely
 * 
 * Revision 1.43  1995/01/04  14:58:39  rob
 * Fixed for shareware build.
 * 
 * Revision 1.42  1995/01/04  11:36:43  rob
 * Added compatibility with older shareware pilot files.
 * 
 * Revision 1.41  1995/01/03  11:01:58  rob
 * fixed a default macro.
 * 
 * Revision 1.40  1995/01/03  10:44:06  rob
 * Added default taunt macros.
 * 
 * Revision 1.39  1994/12/13  10:01:16  allender
 * pop up message box when unable to correctly save player file
 * 
 * Revision 1.38  1994/12/12  11:37:14  matt
 * Fixed auto leveling defaults & saving
 * 
 * Revision 1.37  1994/12/12  00:26:59  matt
 * Added support for no-levelling option
 * 
 * Revision 1.36  1994/12/10  19:09:54  matt
 * Added assert for valid player number when loading game
 * 
 * Revision 1.35  1994/12/08  10:53:07  rob
 * Fixed a bug in highest_level tracking.
 * 
 * Revision 1.34  1994/12/08  10:01:36  john
 * Changed the way the player callsign stuff works.
 * 
 * Revision 1.33  1994/12/07  18:30:38  rob
 * Load highest level along with player (used to be only if higher)
 * Capped at LAST_LEVEL in case a person loads a registered player in shareware.
 * 
 * Revision 1.32  1994/12/03  16:01:12  matt
 * When player file has bad version, force player to choose another
 * 
 * Revision 1.31  1994/12/02  19:54:00  yuan
 * Localization.
 * 
 * Revision 1.30  1994/12/02  11:01:36  yuan
 * Localization.
 * 
 * Revision 1.29  1994/11/29  03:46:28  john
 * Added joystick sensitivity; Added sound channels to detail menu.  Removed -maxchannels
 * command line arg.
 * 
 * Revision 1.28  1994/11/29  01:10:23  john
 * Took out code that allowed new players to
 * configure keyboard.
 * 
 * Revision 1.27  1994/11/25  22:47:10  matt
 * Made saved game descriptions longer
 * 
 * Revision 1.26  1994/11/22  12:10:42  rob
 * Fixed file handle left open if player file versions don't
 * match.
 * 
 * Revision 1.25  1994/11/21  19:35:30  john
 * Replaced calls to joy_init with if (joy_present)
 * 
 * Revision 1.24  1994/11/21  17:29:34  matt
 * Cleaned up sequencing & game saving for secret levels
 * 
 * Revision 1.23  1994/11/21  11:10:01  john
 * Fixed bug with read-only .plr file making the config file 
 * not update.
 * 
 * Revision 1.22  1994/11/20  19:03:08  john
 * Fixed bug with if not having a joystick, default 
 * player input device is cyberman.
 * 
 * Revision 1.21  1994/11/17  12:24:07  matt
 * Made an array the right size, to fix error loading games
 * 
 * Revision 1.20  1994/11/14  17:52:54  allender
 * add call to WriteConfigFile when player files gets written
 * 
 * Revision 1.19  1994/11/14  17:19:23  rob
 * Removed gamma, joystick calibration, and sound settings from player file.
 * Added default difficulty and multi macros.
 * 
 * Revision 1.18  1994/11/07  14:01:23  john
 * Changed the gamma correction sequencing.
 * 
 * Revision 1.17  1994/11/05  17:22:49  john
 * Fixed lots of sequencing problems with newdemo stuff.
 * 
 * Revision 1.16  1994/11/01  16:40:11  john
 * Added Gamma correction.
 * 
 * Revision 1.15  1994/10/24  19:56:50  john
 * Made the new user setup prompt for config options.
 * 
 * Revision 1.14  1994/10/24  17:44:21  john
 * Added stereo channel reversing.
 * 
 * Revision 1.13  1994/10/24  16:05:12  matt
 * Improved handling of player names that are the names of DOS devices
 * 
 * Revision 1.12  1994/10/22  00:08:51  matt
 * Fixed up problems with bonus & game sequencing
 * Player doesn't get credit for hostages unless he gets them out alive
 * 
 * Revision 1.11  1994/10/19  19:59:57  john
 * Added bonus points at the end of level based on skill level.
 * 
 * Revision 1.10  1994/10/19  15:14:34  john
 * Took % hits out of player structure, made %kills work properly.
 * 
 * Revision 1.9  1994/10/19  12:44:26  john
 * Added hours field to player structure.
 * 
 * Revision 1.8  1994/10/17  17:24:34  john
 * Added starting_level to player struct.
 * 
 * Revision 1.7  1994/10/17  13:07:15  john
 * Moved the descent.cfg info into the player config file.
 * 
 * Revision 1.6  1994/10/09  14:54:31  matt
 * Made player cockpit state & window size save/restore with saved games & automap
 * 
 * Revision 1.5  1994/10/08  23:08:09  matt
 * Added error check & handling for game load/save disk io
 * 
 * Revision 1.4  1994/10/05  17:40:54  rob
 * Bumped save_file_version to 5 due to change in player.h
 * 
 * Revision 1.3  1994/10/03  23:00:54  matt
 * New file version for shorter callsigns
 * 
 * Revision 1.2  1994/09/28  17:25:05  matt
 * Added first draft of game save/load system
 * 
 * Revision 1.1  1994/09/27  14:39:12  matt
 * Initial revision
 * 
 * 
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: playsave.c 2.3 1995/05/26 16:16:23 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <io.h>

#include "error.h"

#include "gameseq.h"
#include "player.h"
#include "playsave.h"
#include "joy.h"
#include "kconfig.h"
#include "digi.h"
#include "newmenu.h"
#include "joydefs.h"
#include "palette.h"
#include "multi.h"
#include "menu.h"
#include "config.h"
#include "text.h"
#include "mono.h"
#include "state.h"

#define SAVE_FILE_ID			'DPLR'

//this is for version 5 and below
typedef struct save_info_v5 {
	int	id;
	short	saved_game_version,player_struct_version;
	int 	highest_level;
	int	default_difficulty_level;
	int	default_leveling_on;
} save_info_v5;

//this is for version 6 and above 
typedef struct save_info {
	int	id;
	short	saved_game_version,player_struct_version;
	int	n_highest_levels;				//how many highest levels are saved
	int	default_difficulty_level;
	int	default_leveling_on;
} save_info;

typedef struct hli {
	char	shortname[9];
	ubyte	level_num;
} hli;

int n_highest_levels;

hli highest_levels[MAX_MISSIONS];

#define SAVED_GAME_VERSION		7		//increment this every time saved_game struct changes

//version 5 -> 6: added new highest level information
//version 6 -> 7: stripped out the old saved_game array.

//the shareware is level 4

#define COMPATIBLE_SAVED_GAME_VERSION		4
#define COMPATIBLE_PLAYER_STRUCT_VERSION	16

typedef struct saved_game {
	char		name[GAME_NAME_LEN+1];		//extra char for terminating zero
	player	player;
	int		difficulty_level;		//which level game is played at
	int		primary_weapon;		//which weapon selected
	int		secondary_weapon;		//which weapon selected
	int		cockpit_mode;			//which cockpit mode selected
	int		window_w,window_h;	//size of player's window
	int		next_level_num;		//which level we're going to
	int		auto_leveling_on;		//does player have autoleveling on?
} saved_game;

saved_game saved_games[N_SAVE_SLOTS];

int Default_leveling_on=1;

void init_game_list()
{
	int i;

	for (i=0;i<N_SAVE_SLOTS;i++)
		saved_games[i].name[0] = 0;
}

int new_player_config()
{
	int i,j,control_choice;
	newmenu_item m[7];

RetrySelection:
	for (i=0; i<CONTROL_MAX_TYPES; i++ )	{
		m[i].type = NM_TYPE_MENU; m[i].text = CONTROL_TEXT(i);
	}
	m[0].text = TXT_CONTROL_KEYBOARD;

	control_choice = Config_control_type;				// Assume keyboard

	control_choice = newmenu_do1( NULL, TXT_CHOOSE_INPUT, CONTROL_MAX_TYPES, m, NULL, control_choice );

	if ( control_choice < 0 )
		return 0;

	for (i=0;i<CONTROL_MAX_TYPES; i++ )
		for (j=0;j<MAX_CONTROLS; j++ )
			kconfig_settings[i][j] = default_kconfig_settings[i][j];
	kc_set_controls();

	Config_control_type = control_choice;

	if ( Config_control_type==CONTROL_THRUSTMASTER_FCS)	{
		i = nm_messagebox( TXT_IMPORTANT_NOTE, 2, "Choose another", TXT_OK, TXT_FCS );
		if (i==0) goto RetrySelection;
	}
	
	if ( (Config_control_type>0) && 	(Config_control_type<5) )	{
		joydefs_calibrate();
	}

	Player_default_difficulty = 1;
	Auto_leveling_on = Default_leveling_on = 1;
	n_highest_levels = 1;
	highest_levels[0].shortname[0] = 0;			//no name for mission 0
	highest_levels[0].level_num = 1;				//was highest level in old struct
	Config_joystick_sensitivity = 8;

	// Default taunt macros
	#ifdef NETWORK
	strcpy(Network_message_macro[0], TXT_DEF_MACRO_1);
	strcpy(Network_message_macro[1], TXT_DEF_MACRO_2);
	strcpy(Network_message_macro[2], TXT_DEF_MACRO_3);
	strcpy(Network_message_macro[3], TXT_DEF_MACRO_4);
	#endif

	return 1;
}

//read in the player's saved games.  returns errno (0 == no error)
int read_player_file()
{
	char filename[13];
	FILE *file;
	save_info info;
	int errno_ret = EZERO;

	Assert(Player_num>=0 && Player_num<MAX_PLAYERS);

	sprintf(filename,"%8s.plr",Players[Player_num].callsign);
	file = fopen(filename,"rb");

	//check filename
	if (file && isatty(fileno(file))) {
		//if the callsign is the name of a tty device, prepend a char
		fclose(file);
		sprintf(filename,"$%.7s.plr",Players[Player_num].callsign);
		file = fopen(filename,"rb");
	}

	if (!file) {
		return errno;
	}

	if (fread(&info,sizeof(info),1,file) != 1) {
		errno_ret = errno;
		fclose(file);
		return errno_ret;
	}

	if (info.id!=SAVE_FILE_ID) {
		nm_messagebox(TXT_ERROR, 1, TXT_OK, "Invalid player file");
		fclose(file);
		return -1;
	}

	if (info.saved_game_version<COMPATIBLE_SAVED_GAME_VERSION || info.player_struct_version<COMPATIBLE_PLAYER_STRUCT_VERSION) {
		nm_messagebox(TXT_ERROR, 1, TXT_OK, TXT_ERROR_PLR_VERSION);
		fclose(file);
		return -1;
	}

	if (info.saved_game_version <= 5) {

		//deal with old-style highest level info

		n_highest_levels = 1;

		highest_levels[0].shortname[0] = 0;							//no name for mission 0
		highest_levels[0].level_num = info.n_highest_levels;	//was highest level in old struct

		//This hack allows the player to start on level 8 if he's made it to
		//level 7 on the shareware.  We do this because the shareware didn't
		//save the information that the player finished level 7, so the most
		//we know is that he made it to level 7.
		if (info.n_highest_levels==7)
			highest_levels[0].level_num = 8;
		
	}
	else {	//read new highest level info

		n_highest_levels = info.n_highest_levels;

		if (fread(highest_levels,sizeof(hli),n_highest_levels,file) != n_highest_levels) {
			errno_ret = errno;
			fclose(file);
			return errno_ret;
		}
	}

	Player_default_difficulty = info.default_difficulty_level;
	Default_leveling_on = info.default_leveling_on;

	if ( info.saved_game_version < 7 )	{			// Read old saved games.
		if (fread(saved_games,sizeof(saved_games),1,file) != 1) {
			errno_ret = errno;
			fclose(file);
			return errno_ret;
		}
	}

	//read taunt macros
	{
		int i,len;

		len = (info.saved_game_version == 4)?SHAREWARE_MAX_MESSAGE_LEN:MAX_MESSAGE_LEN;

		#ifdef NETWORK
		for (i = 0; i < 4; i++)
			if (fread(Network_message_macro[i], len, 1, file) != 1)
				{errno_ret = errno; break;}
		#else
		i = 0;
		fseek( file, 48*len, SEEK_CUR );
		#endif
	}

	//read kconfig data
	{
		if (fread( kconfig_settings, MAX_CONTROLS*CONTROL_MAX_TYPES, 1, file )!=1)
			errno_ret=errno;
		else if (fread(&Config_control_type, sizeof(ubyte), 1, file )!=1)
			errno_ret=errno;
		else if (fread(&Config_joystick_sensitivity, sizeof(ubyte), 1, file )!=1)
			errno_ret=errno;

		if (errno_ret==EZERO)	{
			kc_set_controls();
		}
	}

	if (fclose(file) && errno_ret==EZERO)
		errno_ret = errno;

	if ( info.saved_game_version == COMPATIBLE_SAVED_GAME_VERSION )		{
		int i;
		
		Assert( N_SAVE_SLOTS == 10 );

		for (i=0; i<N_SAVE_SLOTS; i++ )	{
			if ( saved_games[i].name[0] )	{
				state_save_old_game(i, saved_games[i].name, &saved_games[i].player, 
             		saved_games[i].difficulty_level, saved_games[i].primary_weapon, 
          			saved_games[i].secondary_weapon, saved_games[i].next_level_num );
			}
		}
		write_player_file();
	}

	return errno_ret;

}

//finds entry for this level in table.  if not found, returns ptr to 
//empty entry.  If no empty entries, takes over last one 
int find_hli_entry()
{
	int i;

	for (i=0;i<n_highest_levels;i++)
		if (!stricmp(highest_levels[i].shortname,Mission_list[Current_mission_num].filename))
			break;

	if (i==n_highest_levels) {		//not found.  create entry

		if (i==MAX_MISSIONS)
			i--;		//take last entry
		else
			n_highest_levels++;

		strcpy(highest_levels[i].shortname,Mission_list[Current_mission_num].filename);
		highest_levels[i].level_num = 0;
	}

	return i;
}

//set a new highest level for player for this mission
void set_highest_level(int levelnum)
{
	int ret,i;

	if ((ret=read_player_file()) != EZERO)
		if (ret != ENOENT)		//if file doesn't exist, that's ok
			return;

	i = find_hli_entry();

	if (levelnum > highest_levels[i].level_num)
		highest_levels[i].level_num = levelnum;

	write_player_file();
}

//gets the player's highest level from the file for this mission
int get_highest_level(void)
{
	int i;
	int highest_saturn_level = 0;
	read_player_file();
#ifndef DEST_SAT
	if (strlen(Mission_list[Current_mission_num].filename)==0 )	{
		for (i=0;i<n_highest_levels;i++)
			if (!stricmp(highest_levels[i].shortname, "DESTSAT")) 	//	Destination Saturn.
		 		highest_saturn_level = highest_levels[i].level_num; 
	}
#endif
   i = highest_levels[find_hli_entry()].level_num;
	if ( highest_saturn_level > i )
   	i = highest_saturn_level;
	return i;
}


//write out player's saved games.  returns errno (0 == no error)
int write_player_file()
{
	char filename[13];
	FILE *file;
	save_info info;
	int errno_ret;

	errno_ret = WriteConfigFile();

	info.id = SAVE_FILE_ID;
	info.saved_game_version = SAVED_GAME_VERSION;
	info.player_struct_version = PLAYER_STRUCT_VERSION;
	info.saved_game_version = SAVED_GAME_VERSION;
	info.player_struct_version = PLAYER_STRUCT_VERSION;
	info.default_difficulty_level = Player_default_difficulty;
	info.default_leveling_on = Auto_leveling_on;

	info.n_highest_levels = n_highest_levels;

	sprintf(filename,"%s.plr",Players[Player_num].callsign);
	file = fopen(filename,"wb");

	//check filename
	if (file && isatty(fileno(file))) {

		//if the callsign is the name of a tty device, prepend a char

		fclose(file);
		sprintf(filename,"$%.7s.plr",Players[Player_num].callsign);
		file = fopen(filename,"wb");
	}

	if (!file)
		return errno;

	errno_ret = EZERO;

	if (fwrite(&info,sizeof(info),1,file) != 1) {
		errno_ret = errno;
		fclose(file);
		return errno_ret;
	}

	//write higest level info
	if ((fwrite(highest_levels, sizeof(hli), n_highest_levels, file) != n_highest_levels)) {
		errno_ret = errno;
		fclose(file);
		return errno_ret;
	}

//	if (fwrite(saved_games,sizeof(saved_games),1,file) != 1) {
//		errno_ret = errno;
//		fclose(file);
//		return errno_ret;
//	}

	#ifdef NETWORK
	if ((fwrite(Network_message_macro, MAX_MESSAGE_LEN, 4, file) != 4)) {
		errno_ret = errno;
		fclose(file);
		return errno_ret;
	}
	#else
	fseek( file, MAX_MESSAGE_LEN * 4, SEEK_CUR );
	#endif

	//write kconfig info
	{
		if (fwrite( kconfig_settings, MAX_CONTROLS*CONTROL_MAX_TYPES, 1, file )!=1)
			errno_ret=errno;
		else if (fwrite( &Config_control_type, sizeof(ubyte), 1, file )!=1)
			errno_ret=errno;
		else if (fwrite( &Config_joystick_sensitivity, sizeof(ubyte), 1, file )!=1)
			errno_ret=errno;
	}

	if (fclose(file))
		errno_ret = errno;

	if (errno_ret != EZERO) {
		remove(filename);			//delete bogus file
		nm_messagebox(TXT_ERROR, 1, TXT_OK, "%s\n\n%s",TXT_ERROR_WRITING_PLR, strerror(errno_ret));
	}
		

	return errno_ret;

}

//returns errno (0 == no error)
int save_player_game(int slot_num,char *text)
{
	int ret;

	if ((ret=read_player_file()) != EZERO)
		if (ret != ENOENT)		//if file doesn't exist, that's ok
			return ret;

	Assert(slot_num < N_SAVE_SLOTS);

	strcpy(saved_games[slot_num].name,text);

	saved_games[slot_num].player = Players[Player_num];

	saved_games[slot_num].difficulty_level	= Difficulty_level;
	saved_games[slot_num].auto_leveling_on	= Auto_leveling_on;
	saved_games[slot_num].primary_weapon	= Primary_weapon;
	saved_games[slot_num].secondary_weapon	= Secondary_weapon;
	saved_games[slot_num].cockpit_mode		= Cockpit_mode;
	saved_games[slot_num].window_w			= Game_window_w;
	saved_games[slot_num].window_h			= Game_window_h;
	saved_games[slot_num].next_level_num	= Next_level_num;

	return write_player_file();
}


//returns errno (0 == no error)
int load_player_game(int slot_num)
{
	char save_callsign[CALLSIGN_LEN+1];
	int ret;

	Assert(slot_num < N_SAVE_SLOTS);

	if ((ret=read_player_file()) != EZERO)
		return ret;

	Assert(saved_games[slot_num].name[0] != 0);

	strcpy(save_callsign,Players[Player_num].callsign);
	Players[Player_num] = saved_games[slot_num].player;
	strcpy(Players[Player_num].callsign,save_callsign);

	Difficulty_level	= saved_games[slot_num].difficulty_level;
	Auto_leveling_on	= saved_games[slot_num].auto_leveling_on;
	Primary_weapon		= saved_games[slot_num].primary_weapon;
	Secondary_weapon	= saved_games[slot_num].secondary_weapon;
	Cockpit_mode		= saved_games[slot_num].cockpit_mode;
	Game_window_w		= saved_games[slot_num].window_w;
	Game_window_h		= saved_games[slot_num].window_h;

	Players[Player_num].level = saved_games[slot_num].next_level_num;

	return EZERO;
}

//fills in a list of pointers to strings describing saved games
//returns the number of non-empty slots
//returns -1 if this is a new player
int get_game_list(char *game_text[N_SAVE_SLOTS])
{
	int i,count,ret;

	ret = read_player_file();

	for (i=count=0;i<N_SAVE_SLOTS;i++) {
		if (game_text)
			game_text[i] = saved_games[i].name;

		if (saved_games[i].name[0])
			count++;
	}

	return (ret==EZERO)?count:-1;		//-1 means new file was created

}

//update the player's highest level.  returns errno (0 == no error)
int update_player_file()
{
	int ret;

	if ((ret=read_player_file()) != EZERO)
		if (ret != ENOENT)		//if file doesn't exist, that's ok
			return ret;

	return write_player_file();
}

