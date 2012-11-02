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
 * $Source: f:/miner/source/main/rcs/songs.c $
 * $Revision: 2.1 $
 * $Author: john $
 * $Date: 1995/05/02 16:15:21 $
 * 
 * Routines to manage the songs in Descent.
 * 
 * $Log: songs.c $
 * Revision 2.1  1995/05/02  16:15:21  john
 * Took out printf.
 * 
 * Revision 2.0  1995/02/27  11:27:13  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.2  1995/02/11  12:42:12  john
 * Added new song method, with FM bank switching..
 * 
 * Revision 1.1  1995/02/11  10:20:33  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: songs.c 2.1 1995/05/02 16:15:21 john Exp $";
#pragma on (unreferenced)

#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "types.h"
#include "songs.h"
#include "mono.h"
#include "cfile.h"
#include "digi.h"

song_info Songs[MAX_SONGS];
int Songs_initialized = 0;

void songs_init()
{
	int i;
	char inputline[80+1];
	CFILE * fp;

	if ( Songs_initialized ) return;

	fp = cfopen( "descent.sng", "rb" );
	if ( fp == NULL )	{
		Error( "Couldn't open descent.sng" );
	}

	i = 0;
	while (cfgets(inputline, 80, fp )) {
		char *p = strchr(inputline,'\n');
		if (p) *p = '\0';
		if ( strlen( inputline ) )	{
			Assert( i < MAX_SONGS );
			sscanf( inputline, "%s %s %s", Songs[i].filename, Songs[i].melodic_bank_file, Songs[i].drum_bank_file );
			//printf( "%d. '%s' '%s' '%s'\n",i,  Songs[i].filename, Songs[i].melodic_bank_file, Songs[i].drum_bank_file );
			i++;
		}
	}
	Songs_initialized = 1;
	cfclose(fp);
}

void songs_play_song( int songnum, int repeat )
{
	if ( !Songs_initialized ) songs_init();
	digi_play_midi_song( Songs[songnum].filename, Songs[songnum].melodic_bank_file, Songs[songnum].drum_bank_file, repeat );
}

void songs_play_level_song( int levelnum )
{
	int songnum;

	Assert( levelnum != 0 );

	if ( !Songs_initialized ) songs_init();

	if (levelnum < 0)	
		songnum = (-levelnum) % NUM_GAME_SONGS;
	else 
		songnum = (levelnum-1) % NUM_GAME_SONGS;
	
	songnum += SONG_LEVEL_MUSIC;
	digi_play_midi_song( Songs[songnum].filename, Songs[songnum].melodic_bank_file, Songs[songnum].drum_bank_file, 1 );
}

