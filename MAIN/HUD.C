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
 * $Source: f:/miner/source/main/rcs/hud.c $
 * $Revision: 2.2 $
 * $Author: mike $
 * $Date: 1995/03/30 16:36:40 $
 * 
 * Routines for displaying HUD messages...
 * 
 * $Log: hud.c $
 * Revision 2.2  1995/03/30  16:36:40  mike
 * text localization.
 * 
 * Revision 2.1  1995/03/06  15:23:50  john
 * New screen techniques.
 * 
 * Revision 2.0  1995/02/27  11:30:41  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.27  1995/01/23  16:51:30  mike
 * Show hud messages on 3d if window in three largest sizes.
 * 
 * Revision 1.26  1995/01/17  17:42:45  rob
 * Made message timeout for HUD messages longer.
 * 
 * Revision 1.25  1995/01/04  11:39:03  rob
 * Made HUD text get out of the way of large HUD messages.
 * 
 * Revision 1.24  1995/01/01  14:20:32  rob
 * longer timer for hud messages.
 * 
 * 
 * Revision 1.23  1994/12/15  13:04:34  mike
 * Replace Players[Player_num].time_total references with GameTime.
 * 
 * Revision 1.22  1994/12/13  12:55:12  mike
 * move press any key to continue message when you are dead to bottom of window.
 * 
 * Revision 1.21  1994/12/07  17:08:01  rob
 * removed unnecessary debug info.
 * 
 * Revision 1.20  1994/12/07  16:24:16  john
 * Took out code that kept track of messages differently for different
 * screen modes... I made it so they just draw differently depending on screen mode.
 * 
 * Revision 1.19  1994/12/07  15:42:57  rob
 * Added a bunch of debug stuff to look for HUD message problems in net games...
 * 
 * Revision 1.18  1994/12/06  16:30:35  yuan
 * Localization
 * 
 * Revision 1.17  1994/12/05  00:32:36  mike
 * fix randomness of color on status bar hud messages.
 * 
 * Revision 1.16  1994/11/19  17:05:53  rob
 * Moved dead_player_message down to avoid overwriting HUD messages.
 * 
 * Revision 1.15  1994/11/18  23:37:56  john
 * Changed some shorts to ints.
 * 
 * Revision 1.14  1994/11/12  16:38:25  mike
 * clear some annoying debug messages.
 * 
 * Revision 1.13  1994/11/11  15:36:39  mike
 * write hud messages on background if 3d window small enough
 * 
 * Revision 1.12  1994/10/20  09:49:31  mike
 * Reduce number of messages.
 * 
 * Revision 1.11  1994/10/17  10:49:15  john
 * Took out some warnings.
 * 
 * Revision 1.10  1994/10/17  10:45:13  john
 * Made the player able to abort death by pressing any button or key.
 * 
 * Revision 1.9  1994/10/13  15:17:33  mike
 * Remove afterburner references.
 * 
 * Revision 1.8  1994/10/11  12:06:32  mike
 * Only show message advertising death sequence abort after player exploded.
 * 
 * Revision 1.7  1994/10/10  17:21:53  john
 * Made so instead of saying too many messages, it scrolls off the
 * oldest message.
 * 
 * Revision 1.6  1994/10/07  23:05:39  john
 * Fixed bug with HUD not drawing stuff sometimes...
 * ( I had a circular buffer that I was stepping thru
 * to draw text that went: for (i=first;i<last;i++)...
 * duh!! last could be less than first.)
 * /
 * 
 * Revision 1.5  1994/09/16  13:08:20  mike
 * Arcade stuff, afterburner stuff.
 * 
 * Revision 1.4  1994/09/14  16:26:57  mike
 * player_dead_message.
 * 
 * Revision 1.3  1994/08/18  16:35:45  john
 * Made gauges messages stay up a bit longer.
 * 
 * Revision 1.2  1994/08/18  12:10:21  john
 * Made HUD messages scroll.
 * 
 * Revision 1.1  1994/08/18  11:22:09  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: hud.c 2.2 1995/03/30 16:36:40 mike Exp $";
#pragma on (unreferenced)



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "inferno.h"
#include "game.h"
#include "screens.h"
#include "gauges.h"
#include "physics.h"
#include "error.h"

#include "menu.h"			// For the font.
#include "mono.h"
#include "collide.h"
#include "newdemo.h"
#include "player.h"
#include "gamefont.h"

#include "wall.h"
#include "arcade.h"
#include "screens.h"
#include "text.h"

int hud_first = 0;
int hud_last = 0;

#define HUD_MESSAGE_LENGTH	150
#define HUD_MAX_NUM 4

int 	HUD_nmessages = 0;
fix	HUD_message_timer = 0;		// Time, relative to Players[Player_num].time (int.frac seconds.frac), at which to erase gauge message
char  HUD_messages[HUD_MAX_NUM][HUD_MESSAGE_LENGTH+5];

extern void copy_background_rect(int left,int top,int right,int bot);
char Displayed_background_message[HUD_MESSAGE_LENGTH] = "";
int	Last_msg_ycrd = -1;
int	Last_msg_height = 6;
int	HUD_color = -1;

//	-----------------------------------------------------------------------------
void clear_background_messages(void)
{
	if ((Cockpit_mode == CM_STATUS_BAR) && (Last_msg_ycrd != -1) && (VR_render_sub_buffer[0].cv_bitmap.bm_y >= 6)) {
		grs_canvas	*canv_save = grd_curcanv;
		gr_set_current_canvas(get_current_game_screen());
		copy_background_rect(0, Last_msg_ycrd, grd_curcanv->cv_bitmap.bm_w, Last_msg_ycrd+Last_msg_height-1);
		gr_set_current_canvas(canv_save);
		Displayed_background_message[0] = 0;
		Last_msg_ycrd = -1;
	}

}

void HUD_clear_messages()
{
	int i;
	HUD_nmessages = 0;
	hud_first = hud_last = 0;
	HUD_message_timer = 0;
	clear_background_messages();
	for (i = 0; i < HUD_MAX_NUM; i++)
		sprintf(HUD_messages[i], "SlagelSlagel!!");
}

//	-----------------------------------------------------------------------------
//	Writes a message on the HUD and checks its timer.
void HUD_render_message_frame()
{
	int i, y,n;
	int h,w,aw;

	if (( HUD_nmessages < 0 ) || (HUD_nmessages > HUD_MAX_NUM))
		Int3(); // Get Rob!

	if ( HUD_nmessages < 1 ) return;

	HUD_message_timer -= FrameTime;

	if ( HUD_message_timer < 0 )	{
		// Timer expired... get rid of oldest message...
		if (hud_last!=hud_first)	{
			//&HUD_messages[hud_first][0] is deing deleted...;
			hud_first = (hud_first+1) % HUD_MAX_NUM;
			HUD_message_timer = F1_0*2;
			HUD_nmessages--;
			clear_background_messages();			//	If in status bar mode and no messages, then erase.
		}
	}

	if (HUD_nmessages > 0 )	{

		gr_set_curfont( Gamefonts[GFONT_SMALL] );    
		if (HUD_color == -1)
			HUD_color = BM_XRGB(0,28,0);

		if ((Cockpit_mode == CM_STATUS_BAR) && (VR_render_sub_buffer[0].cv_bitmap.bm_y >= 19)) {
			// Only display the most recent message in this mode
			char	*message = HUD_messages[(hud_first+HUD_nmessages-1) % HUD_MAX_NUM];

			if (strcmp(Displayed_background_message, message)) {
				int	ycrd;
				grs_canvas	*canv_save = grd_curcanv;

				ycrd = grd_curcanv->cv_bitmap.bm_y - 9;

				if (ycrd < 0)
					ycrd = 0;

				gr_set_current_canvas(get_current_game_screen());

				gr_get_string_size(message, &w, &h, &aw );
				clear_background_messages();
				gr_set_fontcolor( HUD_color, -1);
				gr_printf((grd_curcanv->cv_bitmap.bm_w-w)/2, ycrd, message );

				strcpy(Displayed_background_message, message);
				gr_set_current_canvas(canv_save);
				Last_msg_ycrd = ycrd;
				Last_msg_height = h;
			}
		} else {
			y = 3;

		  	for (i=0; i<HUD_nmessages; i++ )	{	
				n = (hud_first+i) % HUD_MAX_NUM;
				if ((n < 0) || (n >= HUD_MAX_NUM))
					Int3(); // Get Rob!!
				if (!strcmp(HUD_messages[n], "This is a bug."))
					Int3(); // Get Rob!!
				gr_get_string_size(&HUD_messages[n][0], &w, &h, &aw );
				gr_set_fontcolor( HUD_color, -1);
				gr_printf((grd_curcanv->cv_bitmap.bm_w-w)/2,y, &HUD_messages[n][0] );
				y += h+1;
			}
		}
	}

	gr_set_curfont( GAME_FONT );    
}

void HUD_init_message(char * format, ... )
{
	va_list args;
	int temp;
	char *message = NULL;
	char *last_message=NULL;

	if ( (hud_last < 0) || (hud_last >= HUD_MAX_NUM))
		Int3(); // Get Rob!!

		va_start(args, format );
		message = &HUD_messages[hud_last][0];
		vsprintf(message,format,args);
		va_end(args);
//		mprintf((0, "Hud_message: [%s]\n", message));

		if (HUD_nmessages > 0)	{
			if (hud_last==0)
				last_message = &HUD_messages[HUD_MAX_NUM-1][0];
			else
				last_message = &HUD_messages[hud_last-1][0];
		}

		temp = (hud_last+1) % HUD_MAX_NUM;

		if ( temp==hud_first )	{
			// If too many messages, remove oldest message to make room
			hud_first = (hud_first+1) % HUD_MAX_NUM;
			HUD_nmessages--;
		}

		if (last_message && (!strcmp(last_message, message))) {
			HUD_message_timer = F1_0*3;		// 1 second per 5 characters
			return;	// ignore since it is the same as the last one
		}

		hud_last = temp;
		// Check if memory has been overwritten at this point.
		if (strlen(message) >= HUD_MESSAGE_LENGTH)
			Error( "Your message to HUD is too long.  Limit is %i characters.\n", HUD_MESSAGE_LENGTH);
		#ifdef NEWDEMO
		if (Newdemo_state == ND_STATE_RECORDING )
			newdemo_record_hud_message( message );
		#endif
		HUD_message_timer = F1_0*3;		// 1 second per 5 characters
		HUD_nmessages++;
}


void player_dead_message(void)
{
	if (!Arcade_mode && Player_exploded) { //(ConsoleObject->flags & OF_EXPLODING)) {
		gr_set_curfont( Gamefonts[GFONT_SMALL] );    
		if (HUD_color == -1)
			HUD_color = BM_XRGB(0,28,0);
		gr_set_fontcolor( HUD_color, -1);

		gr_printf(0x8000, grd_curcanv->cv_bitmap.bm_h-8, TXT_PRESS_ANY_KEY);
		gr_set_curfont( GAME_FONT );    
	}

}

// void say_afterburner_status(void)
// {
// 	if (Players[Player_num].flags & PLAYER_FLAGS_AFTERBURNER)
// 		HUD_init_message("Afterburner engaged.");
// 	else
// 		HUD_init_message("Afterburner disengaged.");
// }
