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
 * $Source: $
 * $Revision: $
 * $Author: $
 * $Date: $
 * 
 * .
 * 
 * $Log: $
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: $";
#pragma on (unreferenced)

#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "fix.h"
#include "types.h"
#include "gr.h"
#include "ui.h"
#include "key.h"

typedef struct {
	short x, y, w, h;
	char * text;
	short level;
	short item;
	short nextlevel;
	short active;
	short displayed;
	short isdisplayed;
	short hotkey;
	grs_bitmap * background;
} MITEM;

MITEM MenuSystem[] = { \
 {   0,  0, 40, 20, "&File",    0, 0,  1,  0,0,0,0, NULL},
 {   0, 20, 90, 20, "&New",     1, 0, -1,  0,0,0,0, NULL },
 {   0, 40, 90, 20, "&Open",    1, 1, -1,  0,0,0,0, NULL },
 {   0, 60, 90, 20, "&Merge",   1, 2, -1,  0,0,0,0, NULL },
 {   0, 80, 90, 20, "E&xit",    1, 3, -1,  0,0,0,0, NULL },
{  40,  0, 40, 20, "&Edit",    0, 1,  2,  0,0,0,0, NULL},
 {  40, 20, 90, 20, "&Cut",     2, 0, -1,  0,0,0,0, NULL },
 {  40, 40, 90, 20, "C&opy",    2, 1, -1,  0,0,0,0, NULL },
 {  40, 60, 90, 20, "&Past",    2, 2, -1,  0,0,0,0, NULL },
{  80,  0, 40, 20, "&View",    0, 2,  3,  0,0,0,0, NULL },
 {  80, 20, 90, 20, "&All",      3, 0, -1,  0,0,0,0, NULL },
 {  80, 40, 90, 20, "&Some",     3, 1, -1,  0,0,0,0, NULL },
 {  80, 60, 90, 20, "&Few",      3, 2, -1,  0,0,0,0, NULL } };

static int show_citem[10];
static int citem[10];
static int num_items = 13;
static int level = -1;

static int state;

void menubar_draw(int i)
{
	int w = MenuSystem[i].w;
	int h = MenuSystem[i].h;
	int x = MenuSystem[i].x;
	int y = MenuSystem[i].y;

	if (MenuSystem[i].displayed != MenuSystem[i].isdisplayed )
	{
		if ( MenuSystem[i].isdisplayed )
		{
			gr_bm_ubitblt(w, h, x, y, 0, 0, MenuSystem[i].background, &(grd_curscreen->sc_canvas.cv_bitmap));
		} else {
			gr_bm_ubitblt(w, h, 0, 0, x, y, &(grd_curscreen->sc_canvas.cv_bitmap), MenuSystem[i].background);
		}
		MenuSystem[i].isdisplayed = MenuSystem[i].displayed;
	}

	if (MenuSystem[i].displayed)
	{
		gr_set_current_canvas( NULL );

		if (citem[MenuSystem[i].level] == MenuSystem[i].item && show_citem[MenuSystem[i].level])
			gr_set_fontcolor( CWHITE, CBLACK );
		else
			gr_set_fontcolor( CBLACK, CGREY );

		gr_ustring( x, y, MenuSystem[i].text );
	}
}

void show_level( int level )
{
	int i;
	for (i=0; i< num_items; i++ )
		if (MenuSystem[i].level == level)
			MenuSystem[i].displayed = 1;
}

void hide_level( int level )
{
	int i;
	for (i=0; i< num_items; i++ )
		if (MenuSystem[i].level == level)
			MenuSystem[i].displayed = 0;
}

void deactive_level( int level )
{
	int i;
	for (i=0; i< num_items; i++ )
		if (MenuSystem[i].level == level)
			MenuSystem[i].active = 0;
}

static state2_alt_down;
static state3_alt_down;

void menubar_do( int keypress )
{
	int i;

	switch(state)
	{
		case 0:
			if (keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT] )
			{
				state = 1;
				show_citem[0] = 0;
				show_level(0);
				deactive_level( 0 );
			}
			break;
		case 1:
			if (!keyd_pressed[KEY_LALT] && !keyd_pressed[KEY_RALT] )
			{
				state = 2;
				state2_alt_down = 0;
				show_citem[0] = 1;
			}
			break;

		case 2:
			if (keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT] )
				state2_alt_down = 1;

			if (!keyd_pressed[KEY_LALT] && !keyd_pressed[KEY_RALT] && state2_alt_down )
			{
				state = 0;
				hide_level(0);
				hide_level(1);
				hide_level(2);
				hide_level(3);
				break;
			}			

			if (keypress==KEY_ESC)
			{
				state = 0;
				hide_level(0);
				hide_level(1);
				hide_level(2);
				hide_level(3);
				break;
			}
			if (keypress==KEY_LEFT)
				citem[0]--;
			if (keypress==KEY_RIGHT)
				citem[0]++;
		
			if (keypress==KEY_ENTER || keypress==KEY_DOWN )
			{
				state3_alt_down = 0;
				state = 3;	
				show_level( citem[0]+1 );
				show_citem[	citem[0]+1 ] = 1;
			}
			
			break;

		case 3:
			if (keyd_pressed[KEY_LALT] || keyd_pressed[KEY_RALT] )
				state3_alt_down = 1;

			if (!keyd_pressed[KEY_LALT] && !keyd_pressed[KEY_RALT] && state3_alt_down )
			{
				hide_level( citem[0]+1 );
				state = 2;
				break;
			}		

			if (keypress==KEY_ESC)
			{
				state = 0;
				hide_level(0);
				hide_level(1);
				hide_level(2);
				hide_level(3);
			}
			if (keypress==KEY_DOWN)
				citem[ citem[0]+1 ]++;
			if (keypress==KEY_UP)
				citem[ citem[0]+1 ]--;
			
			if (keypress==KEY_RIGHT )
			{
				hide_level( citem[0]+1 );
				for (i=0; i<num_items;i++ )
					menubar_draw(i);
				citem[0]++;
				show_citem[	citem[0]+1 ] = 1;
				show_level( citem[0]+1 );
			}
			if (keypress==KEY_LEFT )
			{
				hide_level( citem[0]+1 );
				for (i=0; i<num_items;i++ )
					menubar_draw(i);
				citem[0]--;
				show_citem[	citem[0]+1 ] = 1;
				show_level( citem[0]+1 );
			}
					
			break;
		default:
			state = 0;
	}

	for (i=0; i<num_items;i++ )
		menubar_draw(i);

}

void menubar_init()
{
	int i;

	state = 0;

	citem[0] = 0;
	citem[1] = 0;
	citem[2] = 0;
	citem[3] = 0;
	show_citem[0] = 0;
	show_citem[1] = 0;
	show_citem[2] = 0;
	show_citem[3] = 0;


	for (i=0; i< num_items; i++ )
	{
		MenuSystem[i].background = gr_create_bitmap( MenuSystem[i].w, MenuSystem[i].h );
		menubar_draw(i);
	}
}

void menubar_close()
{
	int i;

	for (i=0; i< num_items; i++ )
	{
		MenuSystem[i].displayed = 0;
		menubar_draw(i);
		gr_free_bitmap( MenuSystem[i].background );
	}
}


