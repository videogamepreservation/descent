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
 * $Source: f:/miner/source/main/rcs/gamefont.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:30:14 $
 * 
 * Fonts for the game.
 * 
 * $Log: gamefont.c $
 * Revision 2.0  1995/02/27  11:30:14  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.8  1994/11/18  16:41:39  adam
 * trimmed some meat
 * 
 * Revision 1.7  1994/11/17  13:07:11  adam
 * removed unused font
 * 
 * Revision 1.6  1994/11/03  21:36:12  john
 * Added code for credit fonts.
 * 
 * Revision 1.5  1994/08/17  20:20:02  matt
 * Took out alternate-color versions of font3, since this is a mono font
 * 
 * Revision 1.4  1994/08/12  12:03:44  adam
 * tweaked fonts.
 * 
 * Revision 1.3  1994/08/11  12:43:40  adam
 * changed font filenames
 * 
 * Revision 1.2  1994/08/10  19:57:15  john
 * Changed font stuff; Took out old menu; messed up lots of
 * other stuff like game sequencing messages, etc.
 * 
 * Revision 1.1  1994/08/10  17:20:09  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: gamefont.c 2.0 1995/02/27 11:30:14 john Exp $";
#pragma on (unreferenced)

#include <stdlib.h>

#include "gr.h"
#include "gamefont.h"

char * Gamefont_filenames[] = { 	"font1-1.fnt",			// Font 0
											"font2-1.fnt",			// Font 1
											"font2-2.fnt",			// Font 2
											"font2-3.fnt",			// Font 3
											"font3-1.fnt",			// Font 4
										};

grs_font *Gamefonts[MAX_FONTS];

int Gamefont_installed=0;

void gamefont_init()
{
	int i;

	if (Gamefont_installed) return;
	Gamefont_installed = 1;

	for (i=0; i<MAX_FONTS; i++ )
		Gamefonts[i] = gr_init_font(Gamefont_filenames[i]);

	atexit( gamefont_close );
}


void gamefont_close()
{
	int i;

	if (!Gamefont_installed) return;
	Gamefont_installed = 0;

	for (i=0; i<MAX_FONTS; i++ )	{
		gr_close_font( Gamefonts[i] );
		Gamefonts[i] = NULL;
	}

}

