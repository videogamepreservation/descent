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
 * $Source: f:/miner/source/main/rcs/songs.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:30:52 $
 * 
 * .
 * 
 * $Log: songs.h $
 * Revision 2.0  1995/02/27  11:30:52  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.6  1995/02/11  22:21:44  adam
 * *** empty log message ***
 * 
 * Revision 1.5  1995/02/11  19:10:49  adam
 * *** empty log message ***
 * 
 * Revision 1.4  1995/02/11  18:34:40  adam
 * *** empty log message ***
 * 
 * Revision 1.3  1995/02/11  18:04:51  adam
 * upped songs
 * 
 * Revision 1.2  1995/02/11  12:42:12  john
 * Added new song method, with FM bank switching..
 * 
 * Revision 1.1  1995/02/11  10:20:18  john
 * Initial revision
 * 
 * 
 */

#ifndef _SONGS_H
#define _SONGS_H

typedef struct song_info {
	char	filename[16];
	char	melodic_bank_file[16];
	char	drum_bank_file[16];
} song_info;

#define MAX_SONGS 27

extern song_info Songs[];

#define SONG_TITLE			0
#define SONG_BRIEFING		1
#define SONG_ENDLEVEL		2
#define SONG_ENDGAME			3
#define SONG_CREDITS			4
#define SONG_LEVEL_MUSIC	5
#define NUM_GAME_SONGS 		22

void songs_play_song( int songnum, int repeat );
void songs_play_level_song( int levelnum );

#endif

