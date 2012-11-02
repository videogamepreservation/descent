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
 * $Source: f:/miner/source/main/rcs/tempgame.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:28:06 $
 *
 * Game loop for Inferno
 *
 * $Log: tempgame.c $
 * Revision 2.0  1995/02/27  11:28:06  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.747  1995/02/06  12:24:45  allender
 * force Endlevel_sequence in game() to fix rare bug
 * 
 * Revision 1.745  1995/02/02  15:57:52  john
 * Added turbo mode cheat.
 * 
 * Revision 1.744  1995/02/02  14:43:39  john
 * Uppped frametime limit to 150 Hz.
 * 
 * Revision 1.743  1995/02/02  13:37:16  mike
 * move T-?? message down in certain modes.
 * 
 * Revision 1.742  1995/02/02  01:26:59  john
 * Took out no key repeating.
 * 
 * Revision 1.741  1995/01/29  21:36:44  mike
 * make fusion cannon not make pitching slow.
 * 
 * Revision 1.740  1995/01/28  15:57:57  john
 * Made joystick calibration be only when wrong detected in
 * menu or joystick axis changed.
 * 
 * Revision 1.739  1995/01/28  15:21:03  yuan
 * Added X-tra life cheat.
 * 
 * Revision 1.738  1995/01/27  14:08:31  rob
 * Fixed a bug.
 * 
 * Revision 1.737  1995/01/27  14:04:59  rob
 * Its not my fault, Mark told me to do it!
 * 
 * Revision 1.736  1995/01/27  13:12:18  rob
 * Added charging noises to play across net.
 * 
 * Revision 1.735  1995/01/27  11:48:28  allender
 * check for newdemo_state to be paused and stop recording.  We might be
 * in between levels
 * 
 * Revision 1.734  1995/01/26  22:11:41  mike
 * Purple chromo-blaster (ie, fusion cannon) spruce up (chromification)
 * 
 * Revision 1.733  1995/01/26  17:03:04  mike
 * make fusion cannon have more chrome, make fusion, mega rock you!
 * 
 * Revision 1.732  1995/01/25  14:37:25  john
 * Made joystick only prompt for calibration once...
 * 
 * Revision 1.731  1995/01/24  15:49:14  john
 * Made typeing in long net messages wrap on 
 * small screen sizes.
 * 
 * Revision 1.730  1995/01/24  15:23:42  mike
 * network message tweaking.
 * 
 * Revision 1.729  1995/01/24  12:00:47  john
 * Fixed bug with defing macro passing keys to controls.
 * 
 * Revision 1.728  1995/01/24  11:53:35  john
 * Added better macro defining code.
 * 
 * Revision 1.727  1995/01/23  22:17:15  john
 * Fixed bug with not clearing key buffer when leaving f8.
 * 
 * Revision 1.726  1995/01/23  22:07:09  john
 * Added flush to game inputs during F8.
 * 
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: tempgame.c 2.0 1995/02/27 11:28:06 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdarg.h>

#include "inferno.h"
#include "game.h"
#include "key.h"
#include "object.h"
#include "physics.h"
#include "error.h"
#include "joy.h"
#include "mono.h"
#include "iff.h"
#include "pcx.h"
#include "timer.h"
#include "render.h"
#include "laser.h"
#include "screens.h"
#include "textures.h"
#include "slew.h"
#include "gauges.h"
#include "texmap.h"
#include "3d.h"
#include "effects.h"
#include "effect2d.h"
#include "menu.h"
#include "gameseg.h"
#include "wall.h"
#include "ai.h"
#include "hostage.h"
#include "fuelcen.h"
#include "switch.h"
#include "digi.h"
#include "gamesave.h"
#include "scores.h"
#include "ibitblt.h"
#include "mem.h"
#include "palette.h"
#include "morph.h"
#include "lighting.h"
#include "newdemo.h"
#include "titles.h"
#include "collide.h"
#include "weapon.h"
#include "sounds.h"
#include "args.h"
#include "gameseq.h"
#include "automap.h"
#include "text.h"
#include "powerup.h"
#include "fireball.h"
#include "controls.h"
#include "newmenu.h"
#include "network.h"
#include "gamefont.h"
#include "endlevel.h"
#include "joydefs.h"
#include "kconfig.h"
#include "mouse.h"
#include "coindev.h"
#include "multi.h"
#include "desc_id.h"
#include "cntrlcen.h"
#include "pcx.h"
#include "dpmi.h"
#include "state.h"
#include "piggy.h"
#include "multibot.h"

//#define TEST_TIMER	1		//if this is set, do checking on timer

#define FINAL_CHEATS 1

#ifdef ARCADE
#include "arcade.h"
#else
#define Arcade_mode 0
#endif

#define VICTOR 1
#ifdef VICTOR
#include "victor.h"
#endif

#define VFX 1
#ifdef VFX
#include "vfx.h"
#endif

#ifdef EDITOR
#include "editor\editor.h"
#endif

//#define _MARK_ON 1
//#include <wsample.h>            //should come after inferno.h to get mark setting//disabled by KRB

#ifndef NDEBUG
int	Mark_count = 0;                 // number of debugging marks set
int	Speedtest_on = 0;
int	Speedtest_start_time;
int	Speedtest_segnum;
int	Speedtest_sidenum;
int	Speedtest_frame_start;
int	Speedtest_count=0;				//	number of times to do the debug test.
#endif

static fix last_timer_value=0;

#if defined(TIMER_TEST) && !defined(NDEBUG)
fix _timer_value,actual_last_timer_value,_last_frametime;
int stop_count,start_count;
int time_stopped,time_started;
#endif

ubyte * Game_cockpit_copy_code = NULL;

//these are instances of canvases, pointed to by variables below
grs_canvas _canv_3d;                                    //the 3d window, on screen
grs_canvas _canv_3d_offscrn;            //the 3d window, off screen

//these are pointers to our canvases
grs_canvas *Canv_game=NULL;                          //what you acutally see on screen
grs_canvas *Canv_game_offscrn=NULL;  //off-screen buffer the size of the screen 
grs_canvas *Canv_3d=&_canv_3d;  //the 3d window, on screen
grs_canvas *Canv_3d_offscrn=NULL;            //the 3d window, off screen
#ifdef VFX
grs_canvas *Canv_vfx[2][2];			// Eye, buffer
#endif
#ifdef VICTOR
grs_canvas *Canv_victor[2] = { NULL, NULL };			// Eye pages
grs_canvas Canv_victor_3d[2] ;			// Eye pages
ubyte Victor_eye_switch=0;
#endif

int Debug_pause=0;				//John's debugging pause system

int Cockpit_mode=CM_FULL_COCKPIT;		//set game.h for values
int old_cockpit_mode=-1;
int force_cockpit_redraw=0;

int framerate_on=0;

int PaletteRedAdd, PaletteGreenAdd, PaletteBlueAdd;

//	Toggle_var points at a variable which gets !ed on ctrl-alt-T press.
int	Dummy_var;
int	*Toggle_var = &Dummy_var;

#ifdef EDITOR
//flag for whether initial fade-in has been done
char faded_in;
#endif

#ifndef NDEBUG                          //these only exist if debugging

int Game_double_buffer = 1;     //double buffer by default
fix fixed_frametime=0;          //if non-zero, set frametime to this

#endif

int Game_suspended=0;           //if non-zero, nothing moves but player

#ifdef VFX
	int Game_vfx_flag = 0;
#endif

#ifdef VICTOR
	int Game_victor_flag = 0;
#endif

fix RealFrameTime;
fix	Auto_fire_fusion_cannon_time = 0;
fix	Fusion_charge = 0;
fix	Fusion_next_sound_time = 0;

int Debug_spew = 1;
int Game_turbo_mode = 0;

int Game_mode = GM_GAME_OVER;

int	Global_laser_firing_count = 0;
int	Global_missile_firing_count = 0;

grs_bitmap background_bitmap;

int Game_aborted;

#define BACKGROUND_NAME "statback.pcx"

//	==============================================================================================

#ifndef NDEBUG
void speedtest_init(void)
{
	Speedtest_start_time = timer_get_fixed_seconds();
	Speedtest_on = 1;
	Speedtest_segnum = 0;
	Speedtest_sidenum = 0;
	Speedtest_frame_start = FrameCount;

	mprintf((0, "Starting speedtest.  Will be %i frames.  Each . = 10 frames.\n", Highest_segment_index+1));
}

void speedtest_frame(void)
{
	vms_vector	view_dir, center_point;

	Speedtest_sidenum=Speedtest_segnum % MAX_SIDES_PER_SEGMENT;

	compute_segment_center(&Viewer->pos, &Segments[Speedtest_segnum]);
	Viewer->pos.x += 0x10;		Viewer->pos.y -= 0x10;		Viewer->pos.z += 0x17;

	obj_relink(Viewer-Objects, Speedtest_segnum);
	compute_center_point_on_side(&center_point, &Segments[Speedtest_segnum], Speedtest_sidenum);
	vm_vec_normalized_dir_quick(&view_dir, &center_point, &Viewer->pos);
	vm_vector_2_matrix(&Viewer->orient, &view_dir, NULL, NULL);

	if (((FrameCount - Speedtest_frame_start) % 10) == 0)
		mprintf((0, "."));

	Speedtest_segnum++;

	if (Speedtest_segnum > Highest_segment_index) {
		mprintf((0, "\nSpeedtest done:  %i frames, %7.3f seconds, %7.3f frames/second.\n", 
			FrameCount-Speedtest_frame_start,
			f2fl(timer_get_fixed_seconds() - Speedtest_start_time),
			(float) (FrameCount-Speedtest_frame_start) / f2fl(timer_get_fixed_seconds() - Speedtest_start_time)));
		Speedtest_count--;
		if (Speedtest_count == 0)
			Speedtest_on = 0;
		else
			speedtest_init();
	}				

}

#endif

//this is called once per game
void init_game()
{
	ubyte pal[256*3];
	int pcx_error;

	atexit(close_game);             //for cleanup

	#ifdef VFX
	if ( FindArg( "-vfx" ) )
		Game_vfx_flag = 1;
	#endif

	#ifdef VICTOR
	if ( FindArg( "-cybermaxx" ) )
		Game_victor_flag = 1;
	#endif

	init_objects();

	hostage_init();

	init_special_effects();

	init_ai_system();

	init_gauge_canvases();

	init_exploding_walls();

	background_bitmap.bm_data=NULL;
	pcx_error = pcx_read_bitmap(BACKGROUND_NAME,&background_bitmap,BM_LINEAR,pal);
	if (pcx_error != PCX_ERROR_NONE)
		Error("File %s - PCX error: %s",BACKGROUND_NAME,pcx_errormsg(pcx_error));
	gr_remap_bitmap_good( &background_bitmap, pal, -1, -1 );

	Clear_window = 2;		//	do portal only window clear.
}


void reset_palette_add()
{
	PaletteRedAdd 		= 0;
	PaletteGreenAdd	= 0;
	PaletteBlueAdd		= 0;
	//gr_palette_step_up( PaletteRedAdd, PaletteGreenAdd, PaletteBlueAdd );
}


game_show_warning(char *s)
{

	if (!((Game_mode & GM_MULTI) && (Function_mode == FMODE_GAME)))
		stop_time();

	nm_messagebox( TXT_WARNING, 1, TXT_OK, s );

	if (!((Game_mode & GM_MULTI) && (Function_mode == FMODE_GAME)))
		start_time();
}


//these should be in gr.h 
#define cv_w  cv_bitmap.bm_w
#define cv_h  cv_bitmap.bm_h

#define LETTERBOX_HEIGHT 140

int Game_window_w,Game_window_h,max_window_h;

#ifdef VFX
int special_halfsize_window=0;
#endif

int last_drawn_cockpit = -1;

extern int Rear_view;

// This actually renders the new cockpit onto the screen.
void update_cockpits(int force_redraw)
{
	int x, y, w, h;

	if (Cockpit_mode != last_drawn_cockpit || force_redraw )
		last_drawn_cockpit = Cockpit_mode;
	else
		return;

	//Redraw the on-screen cockpit bitmaps
	if (Screen_mode != SCREEN_GAME )	return;

	switch( Cockpit_mode )	{
	case CM_FULL_COCKPIT:
	case CM_REAR_VIEW:
		gr_set_current_canvas(Canv_game);
		PIGGY_PAGE_IN(cockpit_bitmap[Cockpit_mode]);
		gr_ubitmapm(0,0, cockpit_bitmap[Cockpit_mode]);	
		break;
	case CM_FULL_SCREEN:
		break;
	case CM_STATUS_BAR:
		gr_set_current_canvas(Canv_game);
		PIGGY_PAGE_IN(cockpit_bitmap[Cockpit_mode]);
		gr_ubitmapm(0,max_window_h,cockpit_bitmap[Cockpit_mode]);
		w = Game_window_w;
		h = Game_window_h;
		x = (Canv_game->cv_bitmap.bm_w - w)/2;
		y = (max_window_h - h)/2;
		fill_background(x,y,w,h,x,y);
		break;
	case CM_LETTERBOX:
		gr_set_current_canvas(Canv_game);
		gr_clear_canvas( BM_XRGB(0,0,0) );
		break;
	}

	gr_set_current_canvas(Canv_game);
	
	if (Cockpit_mode==CM_FULL_COCKPIT || Cockpit_mode==CM_STATUS_BAR)
		init_gauges();

}

//initialize the various canvases on the game screen
//called every time the screen mode or cockpit changes
void init_game_screen()
{
	int minx, maxx, miny, maxy;
	grs_bitmap *bmp;

	//Initialize the on-screen canvases

	Canv_game->cv_font = GAME_FONT;

	switch( Screen_mode )	{

		case SCREEN_GAME:
			if (Game_cockpit_copy_code)
				free(Game_cockpit_copy_code);
			Game_cockpit_copy_code  = NULL;
		
			if (Cockpit_mode==CM_FULL_COCKPIT ||  Cockpit_mode==CM_REAR_VIEW) {
				grs_bitmap *bm = cockpit_bitmap[Cockpit_mode];

				PIGGY_PAGE_IN(bm);
				//calculate copy code for current cockpit
				gr_set_current_canvas(Canv_game_offscrn);
				gr_bitmap( 0, 0, bm );
				bm = &Canv_game_offscrn->cv_bitmap;
				bm->bm_flags = BM_FLAG_TRANSPARENT;
				gr_ibitblt_find_hole_size ( bm, &minx, &miny, &maxx, &maxy );
				Game_cockpit_copy_code  = gr_ibitblt_create_mask( bm, minx, miny, maxx-minx+1, maxy-miny+1, Canv_game_offscrn->cv_bitmap.bm_rowsize );
				bm->bm_flags = 0;		// Clear all flags for offscreen canvas
				gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, minx, miny, maxx-minx+1, maxy-miny+1 );
			}
			else if (Cockpit_mode == CM_FULL_SCREEN) {

				minx = miny = 0;
				maxx = Canv_game->cv_w-1;
				maxy = Canv_game->cv_h-1;

				#ifdef VFX
				if (special_halfsize_window) {
					maxx /= 2;
					maxy /= 2;
				}
				#endif

				gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, minx, miny, maxx-minx+1, maxy-miny+1 );

				Game_window_w = maxx-minx+1;
				Game_window_h = maxy-miny+1;

			}
			else if (Cockpit_mode == CM_STATUS_BAR) {
 				int x,y,w,h;

				if (Game_window_h > max_window_h) {
					Game_window_h = max_window_h;
					Game_window_w = Canv_game->cv_w;
				}

				w = Game_window_w;
				h = Game_window_h;

				x = (Canv_game->cv_bitmap.bm_w - w)/2;
				y = (max_window_h - h)/2;

				gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, x, y, w, h );

			}
			else if (Cockpit_mode == CM_LETTERBOX) {
				int x,y,w,h;
			
				x = 0; w = Canv_game->cv_bitmap.bm_w;
				h = LETTERBOX_HEIGHT;
				y = (Canv_game->cv_bitmap.bm_h-h)/2;
			
				gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, x, y, w, h );
			}

			bmp =	&_canv_3d_offscrn.cv_bitmap;
			gr_init_sub_canvas( &_canv_3d, Canv_game, bmp->bm_x, bmp->bm_y, bmp->bm_w, bmp->bm_h );

			Canv_3d = &_canv_3d;
			Canv_3d_offscrn = &_canv_3d_offscrn;

			//@@if (Cockpit_mode==CM_FULL_SCREEN || Cockpit_mode==CM_STATUS_BAR)
			//@@	init_reticle();

			gr_set_current_canvas(Canv_game);

			break;

		#ifdef VFX
		case SCREEN_VFX: {
			#ifdef ARCADE
			int i;

			// Use different lighting with VFX
			i = FindArg( "-vfxlight" );
			if ( i ) {
				fix beam = fl2f(atof(Args[i+1]));

				if (beam < 0)
					Lighting_on = 0;
				else
					Beam_brightness = beam;
			}
			else
			#endif
				//NOTE LINK TO ABOVE!!
				Beam_brightness=0x38000;

			Cockpit_mode = CM_FULL_SCREEN;
	
			if ( Game_vfx_flag == 1 )	{
				minx = 0; miny = 0;
				maxx = 159; maxy = 119;
			} else {
				minx = 0; miny = 0;
				maxx = 159; maxy = 239;
			}
			gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, minx, miny, maxx-minx+1, maxy-miny+1 );
			Game_cockpit_copy_code = NULL;
			bmp =	&_canv_3d_offscrn.cv_bitmap;
			gr_init_sub_canvas( &_canv_3d, Canv_game, bmp->bm_x, bmp->bm_y, bmp->bm_w, bmp->bm_h );
			gr_set_current_canvas(Canv_game);
			Canv_3d = &_canv_3d;
			Canv_3d_offscrn = &_canv_3d_offscrn;
			break;
		}
		#endif
	
		#ifdef VICTOR
		case SCREEN_VICTOR:
			//minx = (Canv_game->cv_bitmap.bm_w - Game_window_w)/2;
			//miny = (max_window_h - Game_window_h)/2;
			//maxx = minx + Game_window_w - 1;
			//maxy = miny + Game_window_h - 1;
		
			minx = 0; miny = 0;
			maxx = 319; maxy = 199;

			gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, minx, miny, maxx-minx+1, maxy-miny+1 );
			Game_cockpit_copy_code = NULL;
			gr_set_current_canvas(Canv_game);
			bmp =	&Canv_game->cv_bitmap;
			gr_init_sub_canvas( &_canv_3d, Canv_game, bmp->bm_x, bmp->bm_y, bmp->bm_w, bmp->bm_h );
			Canv_3d = &_canv_3d;
			Canv_3d_offscrn = &_canv_3d_offscrn;
	
			if (Canv_victor[0]==NULL)
				Canv_victor[0] = gr_create_canvas( 320,100 );
			if (Canv_victor[1]==NULL)
				Canv_victor[1] = gr_create_canvas( 320,100 );

			gr_init_sub_canvas(&Canv_victor_3d[0], Canv_victor[0], 0, 0, maxx-minx+1, (maxy-miny+1)/2 );
			gr_init_sub_canvas(&Canv_victor_3d[1], Canv_victor[1], 0, 0, maxx-minx+1, (maxy-miny+1)/2 );
	
			Assert( Canv_victor[0] != NULL );
			Assert( Canv_victor[1] != NULL );
	
			break;
		#endif
	
		case SCREEN_EDITOR:
			Canv_3d = Canv_game;
			Canv_3d_offscrn = Canv_game_offscrn;
			break;

		default:
			Error( "Invalid screen type in game.c" );
	
	}
}

//selects a given cockpit (or lack of one).  See types in game.h
void select_cockpit(int mode)
{
	if (mode != Cockpit_mode) {		//new mode
		Cockpit_mode=mode;
		init_game_screen();
	}
}

//force cockpit redraw next time. call this if you've trashed the screen
void reset_cockpit()
{
	force_cockpit_redraw=1;

}

void HUD_clear_messages();

toggle_cockpit()
{
	int new_mode;

	switch (Cockpit_mode) {

		case CM_FULL_COCKPIT:
			if (Game_window_h > max_window_h)			//too big for scalable
				new_mode = CM_FULL_SCREEN;
			else
				new_mode = CM_STATUS_BAR;
			break;

		case CM_STATUS_BAR:
		case CM_FULL_SCREEN:
			if (Rear_view)
				return;
			new_mode = CM_FULL_COCKPIT;
			break;

		case CM_REAR_VIEW:
		case CM_LETTERBOX:
			return;			//do nothing
			break;

	}

	select_cockpit(new_mode);
	HUD_clear_messages();
}

#define WINDOW_W_DELTA 24	//20
#define WINDOW_H_DELTA 12	//10

#define WINDOW_MIN_W		160

grow_window()
{
	if (Cockpit_mode == CM_FULL_COCKPIT) {
		Game_window_h = max_window_h;
		Game_window_w = Canv_game->cv_w;
		toggle_cockpit();
		grow_window();
		HUD_init_message("Press F3 to return to Cockpit mode");
		return;
	}

	if (Cockpit_mode != CM_STATUS_BAR)
		return;

	if (Canv_3d->cv_bitmap.bm_h == max_window_h) {

		//select_cockpit(CM_FULL_COCKPIT);
		select_cockpit(CM_FULL_SCREEN);

	}
	else {
		int x,y,w,h;

		w = Canv_3d->cv_bitmap.bm_w + WINDOW_W_DELTA;
		h = Canv_3d->cv_bitmap.bm_h + WINDOW_H_DELTA;

		if (w > Canv_game->cv_bitmap.bm_w)
			w = Canv_game->cv_bitmap.bm_w;

		if (h > max_window_h)
			h = max_window_h;

		x = (Canv_game->cv_bitmap.bm_w - w)/2;
		y = (max_window_h - h)/2;

		gr_init_sub_canvas( &_canv_3d, Canv_game, x, y, w,h  );
		gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, x, y, w, h );

		Game_window_w = w;
		Game_window_h = h;

		//@@init_reticle();			
	}
HUD_clear_messages();	//	@mk, 11/11/94
}

grs_bitmap background_bitmap;

copy_background_rect(int left,int top,int right,int bot)
{
	grs_bitmap *bm = &background_bitmap;
	int x,y;
	int tile_left,tile_right,tile_top,tile_bot;
	int ofs_x,ofs_y;
	int dest_x,dest_y;

	tile_left = left / bm->bm_w;
	tile_right = right / bm->bm_w;
	tile_top = top / bm->bm_h;
	tile_bot = bot / bm->bm_h;

	ofs_y = top % bm->bm_h;
	dest_y = top;

	for (y=tile_top;y<=tile_bot;y++) {
		int w,h;

		ofs_x = left % bm->bm_w;
		dest_x = left;

		//h = (bot < dest_y+bm->bm_h)?(bot-dest_y+1):(bm->bm_h-ofs_y);
		h = min(bot-dest_y+1,bm->bm_h-ofs_y);

		for (x=tile_left;x<=tile_right;x++) {

			//w = (right < dest_x+bm->bm_w)?(right-dest_x+1):(bm->bm_w-ofs_x);
			w = min(right-dest_x+1,bm->bm_w-ofs_x);
		
			gr_bm_ubitblt(w,h,dest_x,dest_y,ofs_x,ofs_y,
					&background_bitmap,&grd_curcanv->cv_bitmap);

			ofs_x = 0;
			dest_x += w;
		}

		ofs_y = 0;
		dest_y += h;
	}


}

fill_background(int x,int y,int w,int h,int dx,int dy)
{
	gr_set_current_canvas(Canv_game);
	copy_background_rect(x-dx,y-dy,x-1,y+h+dy-1);
	copy_background_rect(x+w,y-dy,x+w+dx-1,y+h+dy-1);
	copy_background_rect(x,y-dy,x+w-1,y-1);
	copy_background_rect(x,y+h,x+w-1,y+h+dy-1);
}

shrink_window()
{
	if (Cockpit_mode == CM_FULL_COCKPIT) {
		Game_window_h = max_window_h;
		Game_window_w = Canv_game->cv_w;
		toggle_cockpit();
		shrink_window();
		shrink_window();
		HUD_init_message("Press F3 to return to Cockpit mode");
		return;
	}

	if (Cockpit_mode == CM_FULL_SCREEN )	{
		Game_window_h = max_window_h;
		select_cockpit(CM_STATUS_BAR);
		return;
	}

	if (Cockpit_mode != CM_STATUS_BAR)
		return;

	if (Canv_3d->cv_bitmap.bm_w > WINDOW_MIN_W) {
		int x,y,w,h,dx,dy;

		dx = WINDOW_W_DELTA/2;
		dy = WINDOW_H_DELTA/2;

		w = Canv_3d->cv_bitmap.bm_w - WINDOW_W_DELTA;
		h = Canv_3d->cv_bitmap.bm_h - WINDOW_H_DELTA;

		x = (Canv_game->cv_bitmap.bm_w - w)/2;
		y = (max_window_h - h)/2;

		fill_background(x,y,w,h,dx,dy);

		gr_init_sub_canvas( &_canv_3d, Canv_game, x, y, w,h  );
		gr_init_sub_canvas(&_canv_3d_offscrn, Canv_game_offscrn, x, y, w, h );

		Game_window_w = w;
		Game_window_h = h;

		//@@init_reticle();			
	}

	HUD_clear_messages();

}


//called to change the screen mode. Parameter sm is the new mode, one of
//SMODE_GAME or SMODE_EDITOR. returns mode acutally set (could be other
//mode if cannot init requested mode)
int set_screen_mode(int sm)
{
	#ifdef VFX
	if ( (sm==SCREEN_GAME) && (Game_vfx_flag) ) 
		sm=SCREEN_VFX;
	#endif

	#ifdef VICTOR
	if ( (sm==SCREEN_GAME) && (Game_victor_flag) ) 
		sm=SCREEN_VICTOR;
	#endif

	if (Screen_mode == sm) return Screen_mode;              //already set

	//create our off-screen buffer

	if (Canv_game_offscrn == NULL )	{
		#ifdef VFX
		if ( Game_vfx_flag )
			Canv_game_offscrn = gr_create_canvas(320,240);
		else
		#endif
			Canv_game_offscrn = gr_create_canvas(GAMESCREEN_MAX_W,GAMESCREEN_MAX_H);
	}

	Screen_mode = sm;

	switch( sm )	{
	case SCREEN_GAME:
		if (grd_curscreen->sc_mode != GRMODE_GAME )	
			if (gr_set_mode(GRMODE_GAME)) Error(TXT_CANNOT_SET_SCREEN);

		Canv_game = &grd_curscreen->sc_canvas;

		Game_window_w = Canv_game->cv_w;
		max_window_h = Game_window_h = Canv_game->cv_h - cockpit_bitmap[CM_STATUS_BAR]->bm_h;

		#ifdef EDITOR
			Canv_editor = NULL;
		#endif

		set_warn_func(game_show_warning);
		break;

	#ifdef VFX
	case SCREEN_VFX:
		if (grd_curscreen->sc_mode != GRMODE_VFX)	
			if (gr_set_mode(GRMODE_VFX)) Error("Cannot set screen mode for vfx mode");
		Cockpit_mode = CM_FULL_SCREEN;

		Canv_vfx[0][0] = gr_create_sub_canvas(grd_curcanv,0,0,320,240);
		Canv_vfx[0][0]->cv_bitmap.bm_rowsize *= 2;
		Canv_vfx[0][1] = gr_create_sub_canvas(grd_curcanv,0,1,320,240);
		Canv_vfx[0][1]->cv_bitmap.bm_rowsize *= 2;

		Canv_vfx[1][0] = gr_create_sub_canvas(grd_curcanv,320,0,320,240);
		Canv_vfx[1][0]->cv_bitmap.bm_rowsize *= 2;
		Canv_vfx[1][1] = gr_create_sub_canvas(grd_curcanv,320,1,320,240);
		Canv_vfx[1][1]->cv_bitmap.bm_rowsize *= 2;

		vfx_init_graphics();
	
		Canv_game = &grd_curscreen->sc_canvas;
		Canv_game->cv_bitmap.bm_rowsize *= 2;

		#ifdef EDITOR
			Canv_editor = NULL;
		#endif

		set_warn_func(game_show_warning);
		break;
	#endif

	#ifdef VICTOR
	case SCREEN_VICTOR:
		if (grd_curscreen->sc_mode != GRMODE_VICTOR)	
			if (gr_set_mode(GRMODE_VICTOR)) Error("Cannot set screen mode for victor mode");
		Cockpit_mode = CM_FULL_SCREEN;

		//victor_init_graphics();
		
		{
			char *vswitch = getenv( "CYBERMAXX" );
			if ( vswitch )	{
				char *p = strstr( vswitch, "/E:R" ); 
				if ( p )	{
					Victor_eye_switch = 1;
				} else 
					Victor_eye_switch = 0;
			} else {		
			 	Victor_eye_switch = 0;
			}
		}
		Canv_game = &grd_curscreen->sc_canvas;

		#ifdef EDITOR
			Canv_editor = NULL;
		#endif

		set_warn_func(game_show_warning);
		break;
	#endif

	#ifdef EDITOR
	case SCREEN_EDITOR:	{
		int gr_error;
		if (grd_curscreen->sc_mode != GRMODE_EDITOR)	
			if ((gr_error=gr_set_mode(GRMODE_EDITOR))!=0) { //force into game scrren
				Warning("Cannot init editor screen (error=%d)",gr_error);
				return set_screen_mode(SCREEN_GAME);
			}
			gr_palette_load( gr_palette );
		Canv_editor = &grd_curscreen->sc_canvas;

		init_editor_screen();   //setup other editor stuff
		}
		break;
	#endif

	default:
		Error("Invalid screen mode %d",sm);
	}

	init_game_screen();     //reinitialize canvases for new screen

	return Screen_mode;
}

#ifndef RELEASE
fix frame_time_list[8] = {0,0,0,0,0,0,0,0};
fix frame_time_total=0;
int frame_time_cntr=0;

show_framerate()
{
	float rate;
			
	frame_time_total += RealFrameTime - frame_time_list[frame_time_cntr];
	frame_time_list[frame_time_cntr] = RealFrameTime;
	frame_time_cntr = (frame_time_cntr+1)%8;

	rate = f2fl(fixdiv(f1_0*8,frame_time_total));

	gr_set_curfont( GAME_FONT );	
	gr_set_fontcolor(gr_getcolor(0,31,0),-1 );

	gr_printf(grd_curcanv->cv_w-50,grd_curcanv->cv_h-20,"FPS: %2.2f ",rate);

}
#endif

static timer_paused=0;		

void stop_time()
{
	if (timer_paused==0) {
		fix time;
		time = timer_get_fixed_seconds();
		last_timer_value = time - last_timer_value;
		if (last_timer_value < 0) {
			#if defined(TIMER_TEST) && !defined(NDEBUG)
			Int3();		//get Matt!!!!
			#endif
			last_timer_value = 0;
		}
		#if defined(TIMER_TEST) && !defined(NDEBUG)
		time_stopped = time;
		#endif
	}
	timer_paused++;

	#if defined(TIMER_TEST) && !defined(NDEBUG)
	stop_count++;
	#endif
}

void start_time()
{
	timer_paused--;
	Assert(timer_paused >= 0);
	if (timer_paused==0) {
		fix time;
		time = timer_get_fixed_seconds();
		#if defined(TIMER_TEST) && !defined(NDEBUG)
		if (last_timer_value < 0)
			Int3();		//get Matt!!!!
		}
		#endif
		last_timer_value = time - last_timer_value;
		#if defined(TIMER_TEST) && !defined(NDEBUG)
		time_started = time;
		#endif
	}

	#if defined(TIMER_TEST) && !defined(NDEBUG)
	start_count++;
	#endif
}

void game_flush_inputs()
{
	int dx,dy;
	key_flush();
	joy_flush();			
	mouse_flush();
	mouse_get_delta( &dx, &dy );	// Read mouse
	memset(&Controls,0,sizeof(control_info));
}

void reset_time()
{
	last_timer_value = timer_get_fixed_seconds();

}

void calc_frame_time()
{
	fix timer_value,last_frametime = FrameTime;

	#if defined(TIMER_TEST) && !defined(NDEBUG)
	_last_frametime = last_frametime;
	#endif

	timer_value = timer_get_fixed_seconds();
	FrameTime = timer_value - last_timer_value;

	#if defined(TIMER_TEST) && !defined(NDEBUG)
	_timer_value = timer_value;
	#endif
	
	#ifndef NDEBUG
	if (!(((FrameTime > 0) && (FrameTime <= F1_0)) || (Function_mode == FMODE_EDITOR) || (Newdemo_state == ND_STATE_PLAYBACK))) {
		mprintf((1,"Bad FrameTime - value = %x\n",FrameTime));
		if (FrameTime == 0)
			Int3();	//	Call Mike or Matt or John!  Your interrupts are probably trashed!
//		if ( !dpmi_virtual_memory )
//			Int3();		//Get MATT if hit this!
	}
	#endif

	#if defined(TIMER_TEST) && !defined(NDEBUG)
	actual_last_timer_value = last_timer_value;
	#endif

	if ( Game_turbo_mode )
		FrameTime *= 2;

	// Limit frametime to be between 5 and 150 fps.
	RealFrameTime = FrameTime;
	if ( FrameTime < F1_0/150 ) FrameTime = F1_0/150;
	if ( FrameTime > F1_0/5 ) FrameTime = F1_0/5;

	last_timer_value = timer_value;

	if (FrameTime < 0)						//if bogus frametime...
		FrameTime = last_frametime;		//...then use time from last frame

	#ifndef NDEBUG
	if (fixed_frametime) FrameTime = fixed_frametime;
	#endif

	#ifndef NDEBUG
	// Pause here!!!
	if ( Debug_pause )      {
		int c;
		c = 0;
		while( c==0 )
			c = key_peekkey();
			
		if ( c == KEY_P )       {
			Debug_pause = 0;
			c = key_inkey();
		}
		last_timer_value = timer_get_fixed_seconds();
	}
	#endif

	#if Arcade_mode
		FrameTime /= 2;
	#endif

	#if defined(TIMER_TEST) && !defined(NDEBUG)
	stop_count = start_count = 0;
	#endif

	//	Set value to determine whether homing missile can see target.
	//	The lower frametime is, the more likely that it can see its target.
	if (FrameTime <= F1_0/16)
		Min_trackable_dot = 3*(F1_0 - MIN_TRACKABLE_DOT)/4 + MIN_TRACKABLE_DOT;
	else if (FrameTime < F1_0/4)
		Min_trackable_dot = fixmul(F1_0 - MIN_TRACKABLE_DOT, F1_0-4*FrameTime) + MIN_TRACKABLE_DOT;
	else
		Min_trackable_dot = MIN_TRACKABLE_DOT;

}

//--unused-- int Auto_flythrough=0;  //if set, start flythough automatically

void move_player_2_segment(segment *seg,int side)
{
	vms_vector vp;

	compute_segment_center(&ConsoleObject->pos,seg);
	compute_center_point_on_side(&vp,seg,side);
	vm_vec_sub2(&vp,&ConsoleObject->pos);
	vm_vector_2_matrix(&ConsoleObject->orient,&vp,NULL,NULL);

	obj_relink( ConsoleObject-Objects, SEG_PTR_2_NUM(seg) );
	
}

fix Show_view_text_timer = -1;

#ifndef NDEBUG

draw_window_label()
{
	if ( Show_view_text_timer > 0 )
	{
		char *viewer_name,*control_name;

		Show_view_text_timer -= FrameTime;
		gr_set_curfont( GAME_FONT );

		switch( Viewer->type )
		{
			case OBJ_FIREBALL:	viewer_name = "Fireball"; break;
			case OBJ_ROBOT:		viewer_name = "Robot"; break;
			case OBJ_HOSTAGE:		viewer_name = "Hostage"; break;
			case OBJ_PLAYER:		viewer_name = "Player"; break;
			case OBJ_WEAPON:		viewer_name = "Weapon"; break;
			case OBJ_CAMERA:		viewer_name = "Camera"; break;
			case OBJ_POWERUP:		viewer_name = "Powerup"; break;
			case OBJ_DEBRIS:		viewer_name = "Debris"; break;
			case OBJ_CNTRLCEN:	viewer_name = "Control Center"; break;
			default:					viewer_name = "Unknown"; break;
		}

		switch ( Viewer->control_type) {
			case CT_NONE:			control_name = "Stopped"; break;
			case CT_AI:				control_name = "AI"; break;
			case CT_FLYING:		control_name = "Flying"; break;
			case CT_SLEW:			control_name = "Slew"; break;
			case CT_FLYTHROUGH:	control_name = "Flythrough"; break;
			case CT_MORPH:			control_name = "Morphing"; break;
			default:					control_name = "Unknown"; break;
		}

		gr_set_fontcolor( gr_getcolor(31, 0, 0), -1 );
		gr_printf( 0x8000, 45, "%s View - %s",viewer_name,control_name );

	}
}
#endif

#ifdef VFX
int vfx_page=0;

extern void gr_bm_ubitblt02_2x(short w, short h, short dx, short dy, short sx, short sy, grs_bitmap * src, grs_bitmap * dest);
extern void gr_bm_ubitblt00_2x(short w, short h, short dx, short dy, short sx, short sy, grs_bitmap * src, grs_bitmap * dest);


fix eye_offset = F1_0/2;

//render a frame for the game in stereo
game_render_frame_stereo_vfx()
{
	int i,w,h,y;
	fix save_aspect;

	if (grd_curscreen->sc_mode != GRMODE_VFX )	{
		gr_set_mode( GRMODE_VFX );
		vfx_init_graphics();
	}

	save_aspect = grd_curscreen->sc_aspect;
	if ( Game_vfx_flag == 2 )
		grd_curscreen->sc_aspect /= 2;	//Muck with aspect ratio

	vfx_page ^= 1;
	for ( i=0; i<2; i++ )	{

//		if ( keyd_pressed[KEY_F11]) eye_offset -= F1_0/100;
//		if ( keyd_pressed[KEY_F12]) eye_offset += F1_0/100;
//		if ( keyd_pressed[KEY_F11] || keyd_pressed[KEY_F12] )	{
//			mprintf( 0, "Eye offset is %.8f\n", f2fl(eye_offset) );
//		}
		gr_set_current_canvas(Canv_3d_offscrn);

		if (i==0)
			render_frame(-eye_offset);		// Left eye
		else
			render_frame(eye_offset);		// Right eye

		#ifdef ARCADE
		arcade_frame_info();
		#endif

		Canv_3d = Canv_vfx[i][vfx_page];

		w = Canv_3d_offscrn->cv_bitmap.bm_w;
		h = Canv_3d_offscrn->cv_bitmap.bm_h;

		if ( Game_vfx_flag == 1 )	{
			for (y=0; y<h; y++ )	{	
				gr_bm_ubitblt02_2x( w*2, 1, 0, y*2, 0, y, &Canv_3d_offscrn->cv_bitmap, &Canv_3d->cv_bitmap);
				gr_bm_ubitblt02_2x( w*2, 1, 0, y*2+1, 0, y, &Canv_3d_offscrn->cv_bitmap, &Canv_3d->cv_bitmap);
			}
		} else {
			for (y=0; y<h; y++ )	{	
				gr_bm_ubitblt02_2x( w*2, 1, 0, y, 0, y, &Canv_3d_offscrn->cv_bitmap, &Canv_3d->cv_bitmap);
			}
			//gr_bm_ubitblt( w, h, 0, 0, 0, 0, &Canv_3d_offscrn->cv_bitmap, &Canv_3d->cv_bitmap);
		} 
	}

	vfx_set_page(vfx_page);		// 0 or 1

	grd_curscreen->sc_aspect = save_aspect; //restore aspect


}
#endif


#ifdef VICTOR
//render a frame for the game in stereo
game_render_frame_stereo_victor()
{
	int i,w,h,y;
	fix save_aspect;

	save_aspect = grd_curscreen->sc_aspect;
	grd_curscreen->sc_aspect *= 2;	//Muck with aspect ratio

	for ( i=0; i<2; i++ )	{

		gr_set_current_canvas(&Canv_victor_3d[i]);

		if (i==0)
			render_frame(-F1_0);		// Left eye
		else
			render_frame(F1_0);		// Right eye

	}

	//Canv_3d = &grd_curscreen->sc_canvas;
	w = Canv_3d->cv_bitmap.bm_w;
	h = Canv_3d->cv_bitmap.bm_h;

	if (!Victor_eye_switch)	{
		for (y=0; y<h/2; y++ )	{
			gr_bm_ubitblt( w, 1, 0, y*2+0, 0, y, &Canv_victor_3d[1].cv_bitmap, &Canv_3d->cv_bitmap);
			gr_bm_ubitblt( w, 1, 0, y*2+1, 0, y, &Canv_victor_3d[0].cv_bitmap, &Canv_3d->cv_bitmap);
		}
	} else {			// switch l/r eyes
		for (y=0; y<h/2; y++ )	{					 
			gr_bm_ubitblt( w, 1, 0, y*2+0, 0, y, &Canv_victor_3d[0].cv_bitmap, &Canv_3d->cv_bitmap);
			gr_bm_ubitblt( w, 1, 0, y*2+1, 0, y, &Canv_victor_3d[1].cv_bitmap, &Canv_3d->cv_bitmap);
		}
	}
	grd_curscreen->sc_aspect=save_aspect;

}
#endif


extern fix Cruise_speed;

// Returns the length of the first 'n' characters of a string.
int string_width( char * s, int n )
{
	int w,h,aw;
	char p;
	p = s[n];
	s[n] = 0;
	gr_get_string_size( s, &w, &h, &aw );
	s[n] = p;
	return w;
}

// Draw string 's' centered on a canvas... if wider than
// canvas, then wrap it.
void draw_centered_text( int y, char * s )
{
	int i, l;
	char p;

	l = strlen(s);

	if ( string_width( s, l ) < grd_curcanv->cv_bitmap.bm_w )	{
		gr_string( 0x8000, y, s );
		return;
	}

	for (i=0; i<l; i++ )	{
		if ( string_width(s,i) > (grd_curcanv->cv_bitmap.bm_w - 16) )	{
			p = s[i];
			s[i] = 0;
			gr_string( 0x8000, y, s );
			s[i] = p;
			gr_string( 0x8000, y+grd_curcanv->cv_font->ft_h+1, &s[i] );
			return;
		}
	}
}

//render a frame for the game
void game_render_frame_mono(void)
{
	char temp_string[MAX_MULTI_MESSAGE_LEN+25];
	#ifdef EDITOR
	fix save_aspect;
	if (Screen_mode == SCREEN_EDITOR) {
		save_aspect = grd_curscreen->sc_aspect;
		#ifndef NDEBUG
		Game_double_buffer=1;           //can't tmap to VESA screen
		#endif
		grd_curscreen->sc_aspect = 0x13333;                     //make like 320x200 screen
	}
	#endif

	gr_set_current_canvas(Game_double_buffer?Canv_3d_offscrn:Canv_3d);

	render_frame(0);

	//NEW CODE BY JOHN


	#ifndef NDEBUG
	if (Debug_pause) {
		gr_set_curfont( HELP_FONT );
		gr_set_fontcolor( gr_getcolor(31, 31, 31), -1 ); // gr_getcolor(31,0,0));
		gr_ustring( 0x8000, 85/2, "Debug Pause - Press P to exit" );
	}
	#endif

//--repair-- 	if (RepairObj) {
//--repair-- 		gr_set_curfont( HELP_FONT );
//--repair-- 		gr_set_fontcolor( gr_getcolor(31, 31, 31), -1 );
//--repair-- 		gr_ustring( 124, 10, "Repairing");
//--repair-- 		gr_ustring( 0x8000, 20, "Press SHIFT-Q to abort" );
//--repair-- 	}

	#ifdef CROSSHAIR
	if ( Viewer->type == OBJ_PLAYER )
		laser_do_crosshair(Viewer);
	#endif
	
	#ifndef NDEBUG
	draw_window_label();
	#endif

	if ( (Game_mode&GM_MULTI) && (multi_sending_message))	{
		gr_set_curfont( GAME_FONT );    //GAME_FONT );
		gr_set_fontcolor(gr_getcolor(0,63,0), -1 );
		sprintf( temp_string, "Message: %s_", Network_message );
		draw_centered_text(grd_curcanv->cv_bitmap.bm_h/2-16, temp_string );

	}

	if ( (Game_mode&GM_MULTI) && (multi_defining_message))	{
		gr_set_curfont( GAME_FONT );    //GAME_FONT );
		gr_set_fontcolor(gr_getcolor(0,63,0), -1 );
		sprintf( temp_string, "Macro #%d: %s_", multi_defining_message, Network_message );
		draw_centered_text(grd_curcanv->cv_bitmap.bm_h/2-16, temp_string );
	}

	if ((Newdemo_state == ND_STATE_PLAYBACK) || (Newdemo_state == ND_STATE_RECORDING)) {
		char message[128];
		int h,w,aw;

		if (Newdemo_state == ND_STATE_PLAYBACK) {
			if (Newdemo_vcr_state != ND_STATE_PRINTSCREEN) {
			  	sprintf(message, "%s (%d%%%% %s)", TXT_DEMO_PLAYBACK, newdemo_get_percent_done(), TXT_DONE);
			} else {
				sprintf (message, "");
			}
		} else 
			sprintf (message, TXT_DEMO_RECORDING);

		gr_set_curfont( GAME_FONT );    //GAME_FONT );
		gr_set_fontcolor(gr_getcolor(27,0,0), -1 );

		gr_get_string_size(message, &w, &h, &aw );
		if (Cockpit_mode == CM_FULL_COCKPIT)
			h += 15;
		else if ( Cockpit_mode == CM_LETTERBOX )
			h += 7;
		if (Cockpit_mode != CM_REAR_VIEW)
			gr_printf((grd_curcanv->cv_bitmap.bm_w-w)/2, grd_curcanv->cv_bitmap.bm_h - h - 2, message );
	}

	if (!Endlevel_sequence && Fuelcen_control_center_destroyed  && (Fuelcen_seconds_left>-1) && (Fuelcen_seconds_left<127))	{
		int	y;

		gr_set_curfont( Gamefonts[4] );    //GAME_FONT );
		gr_set_fontcolor(gr_getcolor(0,63,0), -1 );
		y = 20;
		if (!((Cockpit_mode == CM_STATUS_BAR) && (Canv_3d->cv_bitmap.bm_y >= 19)))
			y += 5;
		gr_printf(0x8000, y, "T-%d s", Fuelcen_seconds_left );
	}

	if ( Player_num > -1 && Viewer->type==OBJ_PLAYER && Viewer->id==Player_num )	{
		int	x = 3;
		int	y = grd_curcanv->cv_bitmap.bm_h;

		gr_set_curfont( GAME_FONT );
		gr_set_fontcolor( gr_getcolor(0, 31, 0), -1 );
		if (Cruise_speed > 0)
			if (Cockpit_mode==CM_FULL_SCREEN) {
				if (Game_mode & GM_MULTI)
					y -= 64;
				else
					y -= 24;
			} else if (Cockpit_mode == CM_STATUS_BAR) {
				if (Game_mode & GM_MULTI)
					y -= 48;
				else
					y -= 24;
			} else {
				y = 12;
				x = 20;
			}

			gr_printf( x, y, "%s %2d%%", TXT_CRUISE, f2i(Cruise_speed) );

//		gr_printf( 3, y-20, "Laser: %2d", Players[Player_num].laser_level+1 );
//		gr_printf( 3, y-10, "Missiles: %2d", Players[Player_num].secondary_ammo[CONCUSSION_INDEX] );
	}

	#ifdef ARCADE
	arcade_frame_info();
	#endif

#ifndef RELEASE
	if (framerate_on)
		show_framerate();
#endif

#ifndef SHAREWARE
	if ( (Newdemo_state == ND_STATE_PLAYBACK) )
		Game_mode = Newdemo_game_mode;
#endif

	draw_hud();

#ifndef SHAREWARE
	if ( (Newdemo_state == ND_STATE_PLAYBACK) )
		Game_mode = GM_NORMAL;
#endif

	if ( Player_is_dead )
		player_dead_message();

	#ifdef VFX
	if (Cockpit_mode==CM_FULL_SCREEN && special_halfsize_window) {
		int y,w,h;

		w = Canv_3d_offscrn->cv_bitmap.bm_w;
		h = Canv_3d_offscrn->cv_bitmap.bm_h;

		for (y=0; y<h; y++ )	{	
			gr_bm_ubitblt00_2x( w*2, 1, 0, y*2, 0, y, &Canv_3d_offscrn->cv_bitmap, &Canv_game->cv_bitmap);
			gr_bm_ubitblt00_2x( w*2, 1, 0, y*2+1, 0, y, &Canv_3d_offscrn->cv_bitmap, &Canv_game->cv_bitmap);
		}

		return;
	}
	#endif

	if (Game_double_buffer) {		//copy to visible screen
		if ( (Screen_mode == SCREEN_EDITOR) || (Game_cockpit_copy_code==NULL) )
			gr_bm_ubitblt( Canv_3d->cv_bitmap.bm_w, Canv_3d->cv_bitmap.bm_h, 0, 0, 0, 0, &Canv_3d_offscrn->cv_bitmap, &Canv_3d->cv_bitmap);
		else
			if (Cockpit_mode==CM_FULL_SCREEN || Cockpit_mode==CM_STATUS_BAR) {
				Int3();		//this case can't happen, can it?  Get Matt if it does.
				gr_ibitblt( &Canv_3d_offscrn->cv_bitmap, &Canv_3d->cv_bitmap, Game_cockpit_copy_code );
			}
			else
				gr_ibitblt( &Canv_3d_offscrn->cv_bitmap, &Canv_game->cv_bitmap, Game_cockpit_copy_code );
	}

	#ifdef EDITOR
	if (Screen_mode == SCREEN_EDITOR) {
		grd_curscreen->sc_aspect = save_aspect; //restore aspect
	}
	#endif

	play_homing_warning();
	if (Cockpit_mode==CM_FULL_COCKPIT || Cockpit_mode==CM_STATUS_BAR) {

#ifndef SHAREWARE
		if ( (Newdemo_state == ND_STATE_PLAYBACK) )
			Game_mode = Newdemo_game_mode;
#endif

		render_gauges();

#ifndef SHAREWARE
		if ( (Newdemo_state == ND_STATE_PLAYBACK) )
			Game_mode = GM_NORMAL;
#endif
	}

}

void game_render_frame()
{
//--repair-- 	if (!check_lsegments_validity()) {
//--repair-- 		mprintf(1, "Oops, Lsegments array appears to be bogus.  Recomputing.\n");
//--repair-- 		create_local_segment_data();
//--repair-- 	}

	update_cockpits(0);

	#ifdef VFX									
	if (Screen_mode == SCREEN_VFX) 
		game_render_frame_stereo_vfx();
	#endif

	#ifdef VICTOR
	if (Screen_mode == SCREEN_VICTOR) 
		game_render_frame_stereo_victor();
	#endif

	if (Screen_mode == SCREEN_GAME )
		game_render_frame_mono();		

	#ifdef EDITOR
	if (Screen_mode == SCREEN_EDITOR )
		game_render_frame_mono();		
	#endif

	// Make sure palette is faded in
	stop_time();
	gr_palette_fade_in( gr_palette, 32, 0 );
	start_time();

}

void do_photos();
void level_with_floor();

void save_screen_shot(int automap_flag)
{
	fix t1;
	char message[100];
	grs_canvas *screen_canv=&grd_curscreen->sc_canvas;
	grs_font *save_font;
	static savenum=0;
	grs_canvas *temp_canv,*save_canv;
	char savename[13];
	ubyte pal[768];
	int w,h,aw,x,y;

	stop_time();

	save_canv = grd_curcanv;
	temp_canv = gr_create_canvas(screen_canv->cv_bitmap.bm_w,screen_canv->cv_bitmap.bm_h);
	gr_set_current_canvas(temp_canv);
	gr_ubitmap(0,0,&screen_canv->cv_bitmap);

	if ( savenum > 99 ) savenum = 0;
	sprintf(savename,"screen%02d.pcx",savenum++);
	sprintf( message, "%s '%s'", TXT_DUMPING_SCREEN, savename );

	gr_set_current_canvas(Canv_game);
	save_font = grd_curcanv->cv_font;
	gr_set_curfont(GAME_FONT);
	gr_set_fontcolor(gr_find_closest_color_current(0,31,0),-1);
	gr_get_string_size(message,&w,&h,&aw);
	x = (grd_curcanv->cv_bitmap.bm_w-w)/2;
	y = (grd_curcanv->cv_bitmap.bm_h-h)/2;

	if (automap_flag) {
		modex_print_message(32, y, message);
	} else {
		gr_setcolor(gr_find_closest_color_current(0,0,0));
		gr_rect(x-2,y-2,x+w+2,y+h+2);
		gr_printf(x,y,message);
		gr_set_curfont(save_font);
	}
	t1 = timer_get_fixed_seconds() + F1_0;

	gr_palette_read(pal);		//get actual palette from the hardware
	pcx_write_bitmap(savename,&temp_canv->cv_bitmap,pal);

	while ( timer_get_fixed_seconds() < t1 );		// Wait so that messag stays up at least 1 second.

	gr_set_current_canvas(screen_canv);

	if (!automap_flag)
		gr_ubitmap(0,0,&temp_canv->cv_bitmap);

	gr_free_canvas(temp_canv);

	gr_set_current_canvas(save_canv);
	key_flush();
	start_time();
}

//initialize flying
fly_init(object *obj)
{
	obj->control_type = CT_FLYING;
	obj->movement_type = MT_PHYSICS;

	vm_vec_zero(&obj->phys_info.velocity);
	vm_vec_zero(&obj->phys_info.thrust);
	vm_vec_zero(&obj->phys_info.rotvel);
	vm_vec_zero(&obj->phys_info.rotthrust);
}
	
//void morph_test(), morph_step();

int sound_nums[] = {10,11,20,21,30,31,32,33,40,41,50,51,60,61,62,70,80,81,82,83,90,91};

#define N_TEST_SOUNDS (sizeof(sound_nums) / sizeof(*sound_nums))

int test_sound_num=0;

play_test_sound()
{

	digi_play_sample(sound_nums[test_sound_num], F1_0);
}

//	------------------------------------------------------------------------------------
advance_sound()
{
	if (++test_sound_num == N_TEST_SOUNDS)
		test_sound_num=0;

}

test_anim_states();

#include "fvi.h"

//put up the help message
void do_show_help()
{
//	if (!((Game_mode & GM_MULTI) && (Function_mode == FMODE_GAME)))
//		stop_time();

	show_help();

//	if (!((Game_mode & GM_MULTI) && (Function_mode == FMODE_GAME)))
//		start_time();

}

//--unused-- int save_newdemo_state;

extern int been_in_editor;

//--killed--//	------------------------------------------------------------------------------------
//--killed--//	Return 1 if toggled, else return 0.
//--killed--//	Might not be able to toggle if off and not allowed to turn on.
//--killed--int toggle_afterburner_status(void)
//--killed--{
//--killed--	player	*pp = &Players[Player_num];
//--killed--
//--killed--	if (!(pp->flags & PLAYER_FLAGS_AFTERBURNER)) {
//--killed--		//	Was off, try to turn on.
//--killed--		if ((pp->afterburner_time > AFTERBURNER_MAX_TIME/4) && (pp->energy > 0)) {
//--killed--			pp->flags |= PLAYER_FLAGS_AFTERBURNER;
//--killed--		} else {
//--killed--			mprintf(0, "Cannot turn on afterburner due to energy or recharge.\n");
//--killed--			return 0;
//--killed--		}
//--killed--		return 1;
//--killed--	} else {
//--killed--		pp->flags ^= PLAYER_FLAGS_AFTERBURNER;
//--killed--		return 1;
//--killed--	}
//--killed--}

//	------------------------------------------------------------------------------------
void do_cloak_stuff(void)
{
	int i;
	for (i = 0; i < N_players; i++)
		if (Players[i].flags & PLAYER_FLAGS_CLOAKED) {
			// mprintf(0, "Cloak time left: %7.3f\n", f2fl(CLOAK_TIME_MAX - (GameTime - Players[Player_num].cloak_time)));
			if (GameTime - Players[i].cloak_time > CLOAK_TIME_MAX) {
				Players[i].flags &= ~PLAYER_FLAGS_CLOAKED;
				if (i == Player_num) {
					digi_play_sample( SOUND_CLOAK_OFF, F1_0);
					if (Game_mode & GM_MULTI)
						multi_send_play_sound(SOUND_CLOAK_OFF, F1_0);
					maybe_drop_net_powerup(POW_CLOAK);
					multi_send_decloak(); // For demo recording
//					mprintf((0, " --- You have been DE-CLOAKED! ---\n"));
				}
			}
		}
}

//	------------------------------------------------------------------------------------
void do_invulnerable_stuff(void)
{
	if (Players[Player_num].flags & PLAYER_FLAGS_INVULNERABLE) {
		if (GameTime - Players[Player_num].invulnerable_time > INVULNERABLE_TIME_MAX) {
			Players[Player_num].flags ^= PLAYER_FLAGS_INVULNERABLE;
			maybe_drop_net_powerup(POW_INVULNERABILITY);
			digi_play_sample( SOUND_INVULNERABILITY_OFF, F1_0);
			if (Game_mode & GM_MULTI)
				multi_send_play_sound(SOUND_INVULNERABILITY_OFF, F1_0);
			mprintf((0, " --- You have been DE-INVULNERABLEIZED! ---\n"));
		}
	}
}
	
//--killed--//	------------------------------------------------------------------------------------
//--killed--void do_afterburner_stuff(void)
//--killed--{
//--killed--	player	*pp = &Players[Player_num];
//--killed--
//--killed--//	mprintf(0, "[Afterburner] Time: %7.3f, status = %i\n", f2fl(pp->afterburner_time), pp->flags & PLAYER_FLAGS_AFTERBURNER);
//--killed--
//--killed--	if (pp->flags & PLAYER_FLAGS_AFTERBURNER) {
//--killed--		if (pp->afterburner_time > 0) {
//--killed--			pp->afterburner_time -= FrameTime;
//--killed--			pp->energy -= FrameTime/2;
//--killed--			if (pp->afterburner_time <= 0) {
//--killed--				pp->afterburner_time = 0;
//--killed--				pp->flags &= ~PLAYER_FLAGS_AFTERBURNER;
//--killed--				say_afterburner_status();
//--killed--			}
//--killed--			if (pp->energy <= 0) {
//--killed--				pp->energy = 0;
//--killed--				pp->flags &= ~PLAYER_FLAGS_AFTERBURNER;
//--killed--				say_afterburner_status();
//--killed--			}
//--killed--		}
//--killed--	} else {
//--killed--		//	Since afterburner is probably almost always max, do the check, we save time.
//--killed--		if (pp->afterburner_time < AFTERBURNER_MAX_TIME) {
//--killed--			pp->afterburner_time += FrameTime/2;
//--killed--			if (pp->afterburner_time > AFTERBURNER_MAX_TIME)
//--killed--				pp->afterburner_time = AFTERBURNER_MAX_TIME;
//--killed--		}
//--killed--	}
//--killed--
//--killed--}

//	Amount to diminish guns towards normal, per second.
#define	DIMINISH_RATE	16		//	gots to be a power of 2, else change the code in diminish_palette_towards_normal

//	------------------------------------------------------------------------------------
//	Diminish palette effects towards normal.
void diminish_palette_towards_normal(void)
{
	int	dec_amount = 0;

	//	Diminish at DIMINISH_RATE units/second.
	//	For frame rates > DIMINISH_RATE Hz, use randomness to achieve this.
	if (FrameTime < F1_0/DIMINISH_RATE) {
		if (rand() < FrameTime*DIMINISH_RATE/2)	//	Note: rand() is in 0..32767, and 8 Hz means decrement every frame
			dec_amount = 1;
	} else {
		dec_amount = f2i(FrameTime*DIMINISH_RATE);		// one second = DIMINISH_RATE counts
		if (dec_amount == 0)
			dec_amount++;						// make sure we decrement by something
	}

	if (PaletteRedAdd > 0 ) { PaletteRedAdd -= dec_amount; if (PaletteRedAdd < 0 ) PaletteRedAdd = 0; }
	if (PaletteRedAdd < 0 ) { PaletteRedAdd += dec_amount; if (PaletteRedAdd > 0 ) PaletteRedAdd = 0; }

	if (PaletteGreenAdd > 0 ) { PaletteGreenAdd -= dec_amount; if (PaletteGreenAdd < 0 ) PaletteGreenAdd = 0; }
	if (PaletteGreenAdd < 0 ) { PaletteGreenAdd += dec_amount; if (PaletteGreenAdd > 0 ) PaletteGreenAdd = 0; }

	if (PaletteBlueAdd > 0 ) { PaletteBlueAdd -= dec_amount; if (PaletteBlueAdd < 0 ) PaletteBlueAdd = 0; }
	if (PaletteBlueAdd < 0 ) { PaletteBlueAdd += dec_amount; if (PaletteBlueAdd > 0 ) PaletteBlueAdd = 0; }

	if ( (Newdemo_state==ND_STATE_RECORDING) && (PaletteRedAdd || PaletteGreenAdd || PaletteBlueAdd) )
		newdemo_record_palette_effect(PaletteRedAdd, PaletteGreenAdd, PaletteBlueAdd);

	gr_palette_step_up( PaletteRedAdd, PaletteGreenAdd, PaletteBlueAdd );

	//mprintf(0, "%2i %2i %2i\n", PaletteRedAdd, PaletteGreenAdd, PaletteBlueAdd);
}

int	Redsave, Bluesave, Greensave;

void palette_save(void)
{
	Redsave = PaletteRedAdd; Bluesave = PaletteBlueAdd; Greensave = PaletteGreenAdd;
}

void palette_restore(void)
{
	PaletteRedAdd = Redsave; PaletteBlueAdd = Bluesave; PaletteGreenAdd = Greensave;
	gr_palette_step_up( PaletteRedAdd, PaletteGreenAdd, PaletteBlueAdd );
}

extern void dead_player_frame(void);

#ifndef RELEASE
do_cheat_menu()
{
	int mmn;
	newmenu_item mm[16];
	char score_text[21];

	sprintf( score_text, "%d", Players[Player_num].score );

	mm[0].type=NM_TYPE_CHECK; mm[0].value=Players[Player_num].flags & PLAYER_FLAGS_INVULNERABLE; mm[0].text="Invulnerability";
	mm[1].type=NM_TYPE_CHECK; mm[1].value=Players[Player_num].flags & PLAYER_FLAGS_IMMATERIAL; mm[1].text="Immaterial";
	mm[2].type=NM_TYPE_CHECK; mm[2].value=0; mm[2].text="All keys";
	mm[3].type=NM_TYPE_NUMBER; mm[3].value=f2i(Players[Player_num].energy); mm[3].text="% Energy"; mm[3].min_value=0; mm[3].max_value=200;
	mm[4].type=NM_TYPE_NUMBER; mm[4].value=f2i(Players[Player_num].shields); mm[4].text="% Shields"; mm[4].min_value=0; mm[4].max_value=200;
	mm[5].type=NM_TYPE_TEXT; mm[5].text = "Score:";
	mm[6].type=NM_TYPE_INPUT; mm[6].text_len = 10; mm[6].text = score_text;
	mm[7].type=NM_TYPE_RADIO; mm[7].value=(Players[Player_num].laser_level==0); mm[7].group=0; mm[7].text="Laser level 1";
	mm[8].type=NM_TYPE_RADIO; mm[8].value=(Players[Player_num].laser_level==1); mm[8].group=0; mm[8].text="Laser level 2";
	mm[9].type=NM_TYPE_RADIO; mm[9].value=(Players[Player_num].laser_level==2); mm[9].group=0; mm[9].text="Laser level 3";
	mm[10].type=NM_TYPE_RADIO; mm[10].value=(Players[Player_num].laser_level==3); mm[10].group=0; mm[10].text="Laser level 4";
	mm[11].type=NM_TYPE_NUMBER; mm[11].value=Players[Player_num].secondary_ammo[CONCUSSION_INDEX]; mm[11].text="Missiles"; mm[11].min_value=0; mm[11].max_value=200;

	mmn = newmenu_do("Wimp Menu",NULL,12, mm, NULL );

	if (mmn > -1 )	{
		if ( mm[0].value )  {
			Players[Player_num].flags |= PLAYER_FLAGS_INVULNERABLE;
			Players[Player_num].invulnerable_time = GameTime+i2f(1000);
		} else
			Players[Player_num].flags &= ~PLAYER_FLAGS_INVULNERABLE;
		if ( mm[1].value ) 
			Players[Player_num].flags |= PLAYER_FLAGS_IMMATERIAL;
		else
			Players[Player_num].flags &= ~PLAYER_FLAGS_IMMATERIAL;
		if (mm[2].value) Players[Player_num].flags |= PLAYER_FLAGS_BLUE_KEY | PLAYER_FLAGS_RED_KEY | PLAYER_FLAGS_GOLD_KEY;
		Players[Player_num].energy=i2f(mm[3].value);
		Players[Player_num].shields=i2f(mm[4].value);
		Players[Player_num].score = atoi(mm[6].text);
		if (mm[7].value) Players[Player_num].laser_level=0;
		if (mm[8].value) Players[Player_num].laser_level=1;
		if (mm[9].value) Players[Player_num].laser_level=2;
		if (mm[10].value) Players[Player_num].laser_level=3;
		Players[Player_num].secondary_ammo[CONCUSSION_INDEX] = mm[11].value;
		init_gauges();
	}
}
#endif

//	--------------------------------------------------------------------------------------------------
int allowed_to_fire_laser(void)
{
	if (Player_is_dead) {
		Global_missile_firing_count = 0;
		return 0;
	}

	//	Make sure enough time has elapsed to fire laser, but if it looks like it will
	//	be a long while before laser can be fired, then there must be some mistake!
	if (Next_laser_fire_time > GameTime)
		if (Next_laser_fire_time < GameTime + 2*F1_0)
			return 0;

	return 1;
}

fix	Next_flare_fire_time = 0;

int allowed_to_fire_flare(void)
{
	if (Next_flare_fire_time > GameTime)
		if (Next_flare_fire_time < GameTime + F1_0)	//	In case time is bogus, never wait > 1 second.
			return 0;

	Next_flare_fire_time = GameTime + F1_0/4;

	return 1;
}

int allowed_to_fire_missile(void)
{
// mprintf(0, "Next fire = %7.3f, Cur time = %7.3f\n", f2fl(Next_missile_fire_time), f2fl(GameTime));
	//	Make sure enough time has elapsed to fire missile, but if it looks like it will
	//	be a long while before missile can be fired, then there must be some mistake!
	if (Next_missile_fire_time > GameTime)
		if (Next_missile_fire_time < GameTime + 5*F1_0)
			return 0;

	return 1;
}

typedef struct bkg {
	short x, y, w, h;			// The location of the menu.
	grs_bitmap * bmp;			// The background under the menu.
} bkg;

bkg bg = {0,0,0,0,NULL};

//show a message in a nice little box
show_boxed_message(char *msg)
{	
	int w,h,aw;
	int x,y;

	gr_set_current_canvas(Canv_game);
	gr_set_curfont( HELP_FONT );

	gr_get_string_size(msg,&w,&h,&aw);

	x = (grd_curscreen->sc_w-w)/2;
	y = (grd_curscreen->sc_h-h)/2;

	if (bg.bmp) {
		gr_free_bitmap(bg.bmp);
		bg.bmp = NULL;
	}

	// Save the background of the display
	bg.x=x; bg.y=y; bg.w=w; bg.h=h;

	bg.bmp = gr_create_bitmap( w+30, h+30 );
	gr_bm_ubitblt(w+30, h+30, 0, 0, x-15, y-15, &(grd_curscreen->sc_canvas.cv_bitmap), bg.bmp );

	nm_draw_background(x-15,y-15,x+w+15-1,y+h+15-1);

	gr_set_fontcolor( gr_getcolor(31, 31, 31), -1 );
	gr_ustring( 0x8000, y, msg );

}

clear_boxed_message()
{

	if (bg.bmp) {

		gr_bitmap(bg.x-15, bg.y-15, bg.bmp);

		gr_free_bitmap(bg.bmp);

		bg.bmp = NULL;
	}
}

extern int Death_sequence_aborted;

//Process selected keys until game unpaused. returns key that left pause (p or esc)
int do_game_pause(int allow_menu)
{
	int paused;
	int key;

	if (Game_mode & GM_MULTI)
	{
		HUD_init_message(TXT_CANT_PAUSE);
		return(KEY_PAUSE);
	}

	digi_pause_all();
	stop_time();

	palette_save();
	reset_palette_add();

	game_flush_inputs();

	paused=1;

	show_boxed_message(TXT_PAUSE);

	while (paused) {

		key = key_getch();

		switch (key) {
			case 0:
				break;

			case KEY_ESC:
				if (allow_menu)
					Function_mode = FMODE_MENU;
 				clear_boxed_message();
				paused=0;
				break;

			case KEY_F1:
 				clear_boxed_message();
				do_show_help();
				show_boxed_message(TXT_PAUSE);
				break;

			case KEY_PRINT_SCREEN:
				save_screen_shot(0);
				break;

			#ifndef RELEASE
			case KEY_BACKSP: Int3(); break;
			#endif

			default:
 				clear_boxed_message();
				paused=0;
				break;

		}

	}

	game_flush_inputs();

	palette_restore();

	start_time();
	digi_resume_all();

	return key;
}


void show_help()
{
	newmenu_item m[14];

	m[ 0].type = NM_TYPE_TEXT; m[ 0].text = TXT_HELP_ESC;
	m[ 1].type = NM_TYPE_TEXT; m[ 1].text = TXT_HELP_ALT_F2;
	m[ 2].type = NM_TYPE_TEXT; m[ 2].text = TXT_HELP_ALT_F3;
	m[ 3].type = NM_TYPE_TEXT; m[ 3].text = TXT_HELP_F2;
	m[ 4].type = NM_TYPE_TEXT; m[ 4].text = TXT_HELP_F3;
	m[ 5].type = NM_TYPE_TEXT; m[ 5].text = TXT_HELP_F4;
	m[ 6].type = NM_TYPE_TEXT; m[ 6].text = TXT_HELP_F5;
	m[ 7].type = NM_TYPE_TEXT; m[ 7].text = TXT_HELP_PAUSE;
	m[ 8].type = NM_TYPE_TEXT; m[ 8].text = TXT_HELP_MINUSPLUS;
	m[ 9].type = NM_TYPE_TEXT; m[ 9].text = TXT_HELP_PRTSCN;
	m[10].type = NM_TYPE_TEXT; m[10].text = TXT_HELP_1TO5;
	m[11].type = NM_TYPE_TEXT; m[11].text = TXT_HELP_6TO10;
	m[12].type = NM_TYPE_TEXT; m[12].text = "";
	m[13].type = NM_TYPE_TEXT; m[13].text = TXT_HELP_TO_VIEW;

	newmenu_do( NULL, TXT_KEYS, sizeof(m)/sizeof(*m), m, NULL );
}


#ifdef ARCADE
void arcade_frame_info()
{
	if (!Arcade_mode) return;

	if ( Newdemo_state == ND_STATE_PLAYBACK )	{
		gr_set_curfont( GAME_FONT );    //game_font );
		gr_set_fontcolor(gr_getcolor(0,31,0), -1 );
		gr_printf(0x8000, 5, "Insert Coins to Play" );
		return;
	} 

	if (Arcade_timer > 0 )	{
		gr_set_curfont( GAME_FONT );
		gr_set_fontcolor( gr_getcolor(0, 31, 0), -1 );
		gr_printf( 0x8000, 5, "%d seconds left", f2i(Arcade_timer) );
 	} else {
		gr_set_curfont( Gamefonts[GFONT_BIG_1] );    //GAME_FONT );
		gr_printf(0x8000, 40, "Game Over" );
	
		gr_set_curfont( Gamefonts[GFONT_MEDIUM_2] );    //GAME_FONT );
		gr_printf(0x8000, 60, "Insert Coins to Continue" );
		gr_printf(0x8000, 75, "%d", f2i(Arcade_timer)+10 );
	}	
}
#endif

//temp function until Matt cleans up game sequencing
extern void temp_reset_stuff_on_level();

int old_cockpit_mode;

//deal with rear view - switch it on, or off, or whatever
check_rear_view()
{

	#define LEAVE_TIME 0x4000		//how long until we decide key is down

	static int leave_mode;
	static fix entry_time;

	if ( Controls.rear_view_down_count )	{		//key/button has gone down

		if (Rear_view) {
			Rear_view = 0;
			if (Cockpit_mode==CM_REAR_VIEW) {
				select_cockpit(old_cockpit_mode);
			}
			if (Newdemo_state == ND_STATE_RECORDING)
				newdemo_record_restore_rearview();
		}
		else {
			Rear_view = 1;
			leave_mode = 0;		//means wait for another key
			entry_time = timer_get_fixed_seconds();
			if (Cockpit_mode == CM_FULL_COCKPIT) {
				old_cockpit_mode = Cockpit_mode;
				select_cockpit(CM_REAR_VIEW);
			}
			if (Newdemo_state == ND_STATE_RECORDING)
				newdemo_record_rearview();
		}
	}
	else
		if (Controls.rear_view_down_state) {

			if (leave_mode==0 && (timer_get_fixed_seconds()-entry_time)>LEAVE_TIME)
				leave_mode = 1;
		}
		else {

			//@@if (leave_mode==1 && Cockpit_mode==CM_REAR_VIEW) {

			if (leave_mode==1 && Rear_view) {
				Rear_view = 0;
				if (Cockpit_mode==CM_REAR_VIEW) {
					select_cockpit(old_cockpit_mode); 
				}
				if (Newdemo_state == ND_STATE_RECORDING)
					newdemo_record_restore_rearview();
			}
		}
}

void reset_rear_view(void)
{
	Rear_view = 0;

	if (Cockpit_mode == CM_REAR_VIEW)
		select_cockpit(old_cockpit_mode);
}

#ifdef ARCADE
int keys_override;
#endif

int Automap_flag;
int Config_menu_flag;

jmp_buf LeaveGame;


#ifndef FINAL_CHEATS
ubyte cheat_enable_keys[] = {KEY_G,KEY_A,KEY_B,KEY_B,KEY_A,KEY_G,KEY_A,KEY_B,KEY_B,KEY_A,KEY_H,KEY_E,KEY_Y};
#endif

int cheat_enable_index;
#define CHEAT_ENABLE_LENGTH (sizeof(cheat_enable_keys) / sizeof(*cheat_enable_keys))

#ifdef FINAL_CHEATS
ubyte cheat_enable_keys[] = {KEY_G,KEY_A,KEY_B,KEY_B,KEY_A,KEY_G,KEY_A,KEY_B,KEY_B,KEY_A,KEY_H,KEY_E,KEY_Y};

ubyte cheat_wowie[] = {KEY_S,KEY_C,KEY_O,KEY_U,KEY_R,KEY_G,KEY_E};
ubyte cheat_allkeys[] = {KEY_M,KEY_I,KEY_T,KEY_Z,KEY_I};
ubyte cheat_invuln[] = {KEY_R,KEY_A,KEY_C,KEY_E,KEY_R,KEY_X};
ubyte cheat_cloak[] = {KEY_G,KEY_U,KEY_I,KEY_L,KEY_E};
ubyte cheat_shield[] = {KEY_T,KEY_W,KEY_I,KEY_L,KEY_I,KEY_G,KEY_H,KEY_T};
ubyte cheat_warp[] = {KEY_F,KEY_A,KEY_R,KEY_M,KEY_E,KEY_R,KEY_J,KEY_O,KEY_E};
ubyte cheat_turbomode[] = { KEY_B, KEY_U, KEY_G, KEY_G, KEY_I, KEY_N };
ubyte cheat_faster[] = { KEY_F, KEY_L, KEY_A, KEY_S, KEY_H };

#define CHEAT_WOWIE_LENGTH (sizeof(cheat_wowie) / sizeof(*cheat_wowie))
#define CHEAT_ALLKEYS_LENGTH (sizeof(cheat_allkeys) / sizeof(*cheat_allkeys))
#define CHEAT_INVULN_LENGTH (sizeof(cheat_invuln) / sizeof(*cheat_invuln))
#define CHEAT_CLOAK_LENGTH (sizeof(cheat_cloak) / sizeof(*cheat_cloak))
#define CHEAT_SHIELD_LENGTH (sizeof(cheat_shield) / sizeof(*cheat_shield))
#define CHEAT_WARP_LENGTH (sizeof(cheat_warp) / sizeof(*cheat_warp))
#define CHEAT_TURBOMODE_LENGTH (sizeof(cheat_turbomode) / sizeof(*cheat_turbomode))
#define CHEAT_FASTER_LENGTH (sizeof(cheat_faster) / sizeof(*cheat_faster))

int cheat_wowie_index;
int cheat_allkeys_index;
int cheat_invuln_index;
int cheat_cloak_index;
int cheat_shield_index;
int cheat_warp_index;
int cheat_turbomode_index;
int cheat_faster_index;

#endif

int Cheats_enabled=0;

void game_disable_cheats()
{
	Game_turbo_mode = 0;
	Cheats_enabled=0;
}

//	------------------------------------------------------------------------------------
//this function is the game.  called when game mode selected.  runs until
//editor mode or exit selected
void game()
{
	//@@int demo_playing=0;
	//@@int multi_game=0;

	Game_aborted = 0;
	last_drawn_cockpit = -1;				// Force cockpit to redraw next time a frame renders.
	Endlevel_sequence = 0;					// force to zero to avoid being in exit sequence when starting game or demo

	cheat_enable_index = 0;

#ifdef FINAL_CHEATS
	cheat_wowie_index = cheat_allkeys_index = cheat_invuln_index = cheat_cloak_index = cheat_shield_index = cheat_warp_index = 0;
	cheat_turbomode_index = cheat_faster_index = 0;
#endif

	//@@if ( Newdemo_state == ND_STATE_PLAYBACK )
	//@@	demo_playing = 1;
	//@@if ( Game_mode & GM_MULTI )
	//@@	multi_game = 1;

	set_screen_mode(SCREEN_GAME);
	reset_palette_add();

	#ifdef ARCADE
	keys_override = FindArg("-keys");
	#endif

	set_warn_func(game_show_warning);

	#ifdef VFX
	if ((FindArg("-vfxmono"))) {
		Cockpit_mode = CM_FULL_SCREEN;
		special_halfsize_window=1;
		vfx_set_palette();
	}
	#endif

	init_game_screen();
	init_gauges();
	//digi_init_sounds();

	//keyd_repeat = 0;                // Don't allow repeat in game
	keyd_repeat = 1;                // Do allow repeat in game

//	_MARK_("start of game");//Nuked to compile -KRB

	#ifdef EDITOR
		if (Segments[ConsoleObject->segnum].segnum == -1)      //segment no longer exists
			obj_relink( ConsoleObject-Objects, SEG_PTR_2_NUM(Cursegp) );

		if (!check_obj_seg(ConsoleObject))
			move_player_2_segment(Cursegp,Curside);
	#endif

	Viewer = ConsoleObject;
	fly_init(ConsoleObject);

	Game_suspended = 0;

	#ifdef ARCADE
	if (Arcade_mode)	{
		NewGame(1);
		newdemo_start_playback(NULL);
	}
	#endif

	reset_time();
	FrameTime = 0;			//make first frame zero

	#ifdef EDITOR
	if (Current_level_num == 0) {			//not a real level
		init_player_stats_game();
		init_ai_objects();
	}
	#endif

	fix_object_segs();

	game_flush_inputs();

	if ( setjmp(LeaveGame)==0 )	{
		while (1) {
			// GAME LOOP!
			Automap_flag = 0;
			Config_menu_flag = 0;

			GameLoop( 1, 1 );		// Do game loop with rendering and reading controls.

			if (Config_menu_flag) 	{
				if (!(Game_mode&GM_MULTI)) palette_save();
				do_options_menu();
				if (!(Game_mode&GM_MULTI)) palette_restore();		
			}

			if (Automap_flag) {
				int save_w=Game_window_w,save_h=Game_window_h;
				do_automap(0);
				Screen_mode=-1; set_screen_mode(SCREEN_GAME);
				Game_window_w=save_w; Game_window_h=save_h;
				init_game_screen();
				last_drawn_cockpit = -1;
			}

			if ( (Function_mode != FMODE_GAME) && Auto_demo && (Newdemo_state != ND_STATE_NORMAL) )	{
				int choice, fmode;
				fmode = Function_mode;
				Function_mode = FMODE_GAME;
				choice=nm_messagebox( NULL, 2, TXT_YES, TXT_NO, TXT_ABORT_AUTODEMO );
				Function_mode = fmode;
				if (choice==0)	{
					Auto_demo = 0;	
					newdemo_stop_playback();
					Function_mode = FMODE_MENU;
				} else {
					Function_mode = FMODE_GAME;
				}
			}
	
			if ( (Function_mode != FMODE_GAME ) && (Newdemo_state != ND_STATE_PLAYBACK ) && (Function_mode!=FMODE_EDITOR) )		{
				int choice, fmode;
				fmode = Function_mode;
				Function_mode = FMODE_GAME;
				choice=nm_messagebox( NULL, 2, TXT_YES, TXT_NO, TXT_ABORT_GAME );
				Function_mode = fmode;
				if (choice != 0)
					Function_mode = FMODE_GAME;
			}

			if (Function_mode != FMODE_GAME)
				longjmp(LeaveGame,0);
		}
	} 

	digi_stop_all();

	if ( (Newdemo_state == ND_STATE_RECORDING) || (Newdemo_state == ND_STATE_PAUSED) )
		newdemo_stop_recording();

	multi_leave_game();

	if ( Newdemo_state == ND_STATE_PLAYBACK )	
 		newdemo_stop_playback();

	if (Function_mode != FMODE_EDITOR)
		gr_palette_fade_out(gr_palette,32,0);			// Fade out before going to menu

	//@@if ( (!demo_playing) && (!multi_game) && (Function_mode != FMODE_EDITOR) )	{
	//@@	scores_maybe_add_player(Game_aborted);
	//@@}

//	_MARK_("end of game");//Nuked to compile -KRB

	clear_warn_func(game_show_warning);     //don't use this func anymore

}

//called at the end of the program
void close_game() 
{

	gr_free_canvas(Canv_game_offscrn);

	close_gauge_canvases();

	restore_effect_bitmap_icons();

	Canv_3d_offscrn = NULL;

	if (Game_cockpit_copy_code)	{
		free(Game_cockpit_copy_code);
		Game_cockpit_copy_code = NULL;
	}

	#ifdef VFX
	if (Canv_vfx[0][0]) free( Canv_vfx[0][0] );
	if (Canv_vfx[0][1]) free( Canv_vfx[0][1] );
	if (Canv_vfx[1][0]) free( Canv_vfx[1][0] );
	if (Canv_vfx[1][1]) free( Canv_vfx[1][1] );
	#endif

	#ifdef VICTOR
	if (Canv_victor[0]) gr_free_canvas(Canv_victor[0]);
	if (Canv_victor[1]) gr_free_canvas(Canv_victor[1]);
	#endif

	if (background_bitmap.bm_data)
		free(background_bitmap.bm_data);

	clear_warn_func(game_show_warning);     //don't use this func anymore
}

grs_canvas * get_current_game_canvas()
{
	switch(Screen_mode)	{
	case SCREEN_VFX:
		vfx_set_page(vfx_page);
		return Canv_vfx[0][vfx_page];
	default:
		return Canv_game;
	}
}

ubyte exploding_flag = 0;

extern dump_used_textures_all();


void ReadControls()
{
	int key;
	fix key_time;
	static fix newdemo_single_frame_time;

		#ifdef ARCADE
		if (Arcade_mode)	{
			int coins;
			if (Newdemo_state!=ND_STATE_PLAYBACK)	{
				Arcade_timer -= 2*FrameTime;
				if ( Arcade_timer < (-F1_0*10) )	{
					newdemo_toggle_playback();
				}
			}
			coins = coindev_count(0);
			if (coins>0)	{
				if ( Newdemo_state==ND_STATE_PLAYBACK )	{
					newdemo_toggle_playback();
					Arcade_timer = F1_0 * ARCADE_FIRST_SECONDS;		// Two minutes to play...
					if (coins>1)
						Arcade_timer += F1_0 * ARCADE_CONTINUE_SECONDS*(coins-1);		// Two minutes to play...
					NewGame(1);
				} else {
					if (Arcade_timer < 0 )
						Arcade_timer = 0;
					Arcade_timer += F1_0 * ARCADE_CONTINUE_SECONDS*coins;		// Two minutes to play...
				}
			}
		}
		#endif

		Player_fired_laser_this_frame=-1;

#ifndef NDEBUG
		if (Speedtest_on)
			speedtest_frame();
#endif

		if (!Endlevel_sequence && !Player_is_dead) {

			#ifdef ARCADE
			if (Arcade_mode)	{
				if ( Arcade_timer > 0 )
					if (Newdemo_state == ND_STATE_PLAYBACK )
						memset( &Controls, 0, sizeof(control_info) );
					else
						controls_read_all();		//NOTE LINK TO ABOVE!!!
			} else
			#endiffo) );
					else
						controls_read_all();		//NOTE LINK TO ABOVE!!!
			} else
			#endif
				if ( (Newdemo_state == ND_STATE_PLAYBACK) || multi_sending_message || multi_defining_message )
					memset( &Controls, 0, sizeof(control_info) );
				else
					controls_read_all();		//NOTE LINK TO ABOVE!!!

			check_rear_view();
						
			//	If automap key pressed, enable automap unless you are in network mode, control center destroyed and < 10 seconds left
			if ( Controls.automap_down_count && !((Game_mode & GM_MULTI) && Fuelcen_control_center_destroyed && (Fuelcen_seconds_left < 10)))
				Automap_flag = 1;			

			if (Controls.fire_flare_down_count)
				if (allowed_to_fire_flare())
					Flare_create(ConsoleObject);

			if (allowed_to_fire_missile())
				Global_missile_firing_count += Weapon_info[Secondary_weapon_to_weapon_info[Secondary_weapon]].fire_count * (Controls.fire_secondary_state || Controls.fire_secondary_down_count);

			if (Global_missile_firing_count) {
				do_missile_firing();
				Global_missile_firing_count--;
			}

			if (Global_missile_firing_count < 0)
				Global_missile_firing_count = 0;

			//	Drop proximity bombs.
			if (Controls.drop_bomb_down_count) {
				int	ssw_save = Secondary_weapon;
				Secondary_weapon = PROXIMITY_INDEX;
				while (Controls.drop_bomb_down_count--)
					do_missile_firing();
				Secondary_weapon = ssw_save;
			}
		}

		if (Player_exploded) { //Player_is_dead && (ConsoleObject->flags & OF_EXPLODING) ) {
			if (exploding_flag==0)	{
				exploding_flag = 1;	 		// When player starts exploding, clear all input devices...
				game_flush_inputs();
			} else {	
				int i;
				if (key_down_count(KEY_BACKSP))
					Int3();
				if (key_down_count(KEY_PRINT_SCREEN))
					save_screen_shot(0);
				for (i=0; i<4; i++ )	
					if (joy_get_button_down_cnt(i)>0) Death_sequence_aborted = 1;
				for (i=0; i<3; i++ )	
					if (mouse_button_down_count(i)>0) Death_sequence_aborted = 1;
				for (i=0; i<256; i++ )	
					if (key_down_count(i)>0) Death_sequence_aborted = 1;
				if (Death_sequence_aborted)
					game_flush_inputs();
			}
		} else {
			exploding_flag=0;
		}

		if (Newdemo_state == ND_STATE_PLAYBACK )	{
			if ((keyd_pressed[KEY_LSHIFT] || keyd_pressed[KEY_RSHIFT]) && keyd_pressed[KEY_RIGHT])
				Newdemo_vcr_state = ND_STATE_FASTFORWARD;
			else if ((keyd_pressed[KEY_LSHIFT] || keyd_pressed[KEY_RSHIFT]) && keyd_pressed[KEY_LEFT])
				Newdemo_vcr_state = ND_STATE_REWINDING;
			else if (!(keyd_pressed[KEY_LCTRL] || keyd_pressed[KEY_RCTRL]) && keyd_pressed[KEY_RIGHT] && ((timer_get_fixed_seconds() - newdemo_single_frame_time) >= F1_0))
				Newdemo_vcr_state = ND_STATE_ONEFRAMEFORWARD;
			else if (!(keyd_pressed[KEY_LCTRL] || keyd_pressed[KEY_RCTRL]) && keyd_pressed[KEY_LEFT] && ((timer_get_fixed_seconds() - newdemo_single_frame_time) >= F1_0))
				Newdemo_vcr_state = ND_STATE_ONEFRAMEBACKWARD;
			else if ((Newdemo_vcr_state == ND_STATE_FASTFORWARD) || (Newdemo_vcr_state == ND_STATE_REWINDING))
				Newdemo_vcr_state = ND_STATE_PLAYBACK;
		}

		while ((key=key_inkey_time(&key_time)) != 0)	{

			if ( (Game_mode&GM_MULTI) && (multi_sending_message || multi_defining_message ))	{
				multi_message_input_sub( key );
				key = 0;		// Wipe out key!
			}

			if (!(Game_mode&GM_MULTI) && key == cheat_enable_keys[cheat_enable_index]) {
				if (++cheat_enable_index == CHEAT_ENABLE_LENGTH) {
					HUD_init_message(TXT_CHEATS_ENABLED);
					digi_play_sample( SOUND_CHEATER, F1_0);
					Cheats_enabled = 1;
					Players[Player_num].score = 0;
				}
			}
			else
				cheat_enable_index = 0;


#ifdef FINAL_CHEATS
		if (Cheats_enabled) {
			if (!(Game_mode&GM_MULTI) && key == cheat_wowie[cheat_wowie_index]) {
				if (++cheat_wowie_index == CHEAT_WOWIE_LENGTH) {
						int i;

						HUD_init_message(TXT_WOWIE_ZOWIE);
						digi_play_sample( SOUND_CHEATER, F1_0);

						#ifndef SHAREWARE
							Players[Player_num].primary_weapon_flags = 0xff;
							Players[Player_num].secondary_weapon_flags = 0xff;
						#else
							Players[Player_num].primary_weapon_flags = 0xff ^ (HAS_PLASMA_FLAG | HAS_FUSION_FLAG);
							Players[Player_num].secondary_weapon_flags = 0xff ^ (HAS_SMART_FLAG | HAS_MEGA_FLAG);
						#endif

						for (i=0; i<MAX_PRIMARY_WEAPONS; i++)
							Players[Player_num].primary_ammo[i] = Primary_ammo_max[i];
						
						for (i=0; i<MAX_SECONDARY_WEAPONS; i++)
							Players[Player_num].secondary_ammo[i] = Secondary_ammo_max[i];
						
						if (Newdemo_state == ND_STATE_RECORDING)
							newdemo_record_laser_level(Players[Player_num].laser_level, MAX_LASER_LEVEL);

						Players[Player_num].energy = MAX_ENERGY;
						Players[Player_num].laser_level = MAX_LASER_LEVEL;
						Players[Player_num].flags |= PLAYER_FLAGS_QUAD_LASERS;
						update_laser_weapon_info();
				}
			}
			else
				cheat_wowie_index = 0;

			if (!(Game_mode&GM_MULTI) && key == cheat_allkeys[cheat_allkeys_index]) {
				if (++cheat_allkeys_index == CHEAT_ALLKEYS_LENGTH) {
						HUD_init_message(TXT_ALL_KEYS);
						digi_play_sample( SOUND_CHEATER, F1_0);
						Players[Player_num].flags |= PLAYER_FLAGS_BLUE_KEY | PLAYER_FLAGS_RED_KEY | PLAYER_FLAGS_GOLD_KEY;
				}
			}
			else
				cheat_allkeys_index = 0;


			if (!(Game_mode&GM_MULTI) && key == cheat_invuln[cheat_invuln_index]) {
				if (++cheat_invuln_index == CHEAT_INVULN_LENGTH) {
						Players[Player_num].flags ^= PLAYER_FLAGS_INVULNERABLE;
						HUD_init_message("%s %s!", TXT_INVULNERABILITY, (Players[Player_num].flags&PLAYER_FLAGS_INVULNERABLE)?TXT_ON:TXT_OFF);
						digi_play_sample( SOUND_CHEATER, F1_0);
						Players[Player_num].invulnerable_time = GameTime+i2f(1000);
				}
			}
			else
				cheat_invuln_index = 0;

			if (!(Game_mode&GM_MULTI) && key == cheat_cloak[cheat_cloak_index]) {
				if (++cheat_cloak_index == CHEAT_CLOAK_LENGTH) {
						Players[Player_num].flags ^= PLAYER_FLAGS_CLOAKED;
						HUD_init_message("%s %s!", TXT_CLOAK, (Players[Player_num].flags&PLAYER_FLAGS_CLOAKED)?TXT_ON:TXT_OFF);
						digi_play_sample( SOUND_CHEATER, F1_0);
						if (Players[Player_num].flags & PLAYER_FLAGS_CLOAKED) {
							ai_do_cloak_stuff();
							Players[Player_num].cloak_time = GameTime;
						}
				}
			}
			else
				cheat_cloak_index = 0;

			if (!(Game_mode&GM_MULTI) && key == cheat_shield[cheat_shield_index]) {
				if (++cheat_shield_index == CHEAT_SHIELD_LENGTH) {
						HUD_init_message(TXT_FULL_SHIELDS);
						digi_play_sample( SOUND_CHEATER, F1_0);
						Players[Player_num].shields = MAX_SHIELDS;
				}
			}
			else
				cheat_shield_index = 0;

			if (!(Game_mode&GM_MULTI) && key == cheat_warp[cheat_warp_index]) {
				if (++cheat_warp_index == CHEAT_WARP_LENGTH) {
						newmenu_item m;
						char text[10]="";
						int new_level_num;
						int item;
						digi_play_sample( SOUND_CHEATER, F1_0);
						m.type=NM_TYPE_INPUT; m.text_len = 10; m.text = text;
						item = newmenu_do( NULL, TXT_WARP_TO_LEVEL, 1, &m, NULL );
						if (item != -1) {
							new_level_num = atoi(m.text);
							if (new_level_num!=0 && new_level_num>=0 && new_level_num<=Last_level)
								StartNewLevel(new_level_num);
						}
				}
			}
			else
				cheat_warp_index = 0;

			if (!(Game_mode&GM_MULTI) && key == cheat_turbomode[cheat_turbomode_index]) {
				if (++cheat_turbomode_index == CHEAT_TURBOMODE_LENGTH) {
						Game_turbo_mode ^= 1;
						HUD_init_message("%s %s!", "Turbo mode", Game_turbo_mode?TXT_ON:TXT_OFF);
						digi_play_sample( SOUND_CHEATER, F1_0);
				}
			}
			else
				cheat_turbomode_index = 0;


		}
#endif

			#ifndef RELEASE
			if ((key&KEY_DEBUGGED)&&(Game_mode&GM_MULTI))	{
				Network_message_reciever = 100;		// Send to everyone...
				sprintf( Network_message, "%s %s", TXT_I_AM_A, TXT_CHEATER);
			}
			#endif
	
			if (Endlevel_sequence) {

	  			if (key==KEY_PRINT_SCREEN)
					save_screen_shot(0);

				if (key == KEY_PAUSE)
					key = do_game_pause(0);		//so esc from pause will end level

				if (key == KEY_ESC)	{
					stop_endlevel_sequence();
					last_drawn_cockpit=-1;
					return;
				}

				if (key == KEY_BACKSP)
					Int3();

				break;		//don't process any other keys
			}

			if (Player_is_dead) {

	  			if (key==KEY_PRINT_SCREEN)
					save_screen_shot(0);

				if (key == KEY_PAUSE)	{
					key = do_game_pause(0);		//so esc from pause will end level
					Death_sequence_aborted  = 0;		// Clear because code above sets this for any key.
				}
					
				if (key == KEY_ESC) {
					if (ConsoleObject->flags & OF_EXPLODING)	
						Death_sequence_aborted = 1;
				}

				if (key == KEY_BACKSP)	{
					Death_sequence_aborted  = 0;		// Clear because code above sets this for any key.
					Int3();
				}

				break;		//don't process any other keys
			}

			//mprintf( 0, "Key %d pressed at %f\n", key, f2fl(key_time) );

//			#ifdef VFX
//			// Toggle VFX between lo and hi res. 1=low, 2=hi
//			if (key==KEY_F12)	{
//				if ( Game_vfx_flag == 1 )	{
//					Game_vfx_flag = 2;
//					init_game_screen();
//				} else if ( Game_vfx_flag==2 )	{
//					Game_vfx_flag = 1;
//					init_game_screen();
//				}
//				key=0;		// forget this key was pressed
//			}
//			#endif

			#ifdef ARCADE
			if (Arcade_mode && !keys_override)	{
				if (key==KEY_ESC)	{
					if (Newdemo_state!=ND_STATE_PLAYBACK)	{
						newdemo_toggle_playback();	//turn on demo
					} else {
						newdemo_toggle_playback();	// turn off demo
						// exit to dos
						#ifdef EDITOR
							if (! SafetyCheck()) break;
							if (Screen_mode == SCREEN_EDITOR )
								close_editor_screen();
						#endif
						Function_mode=FMODE_EXIT;
					}
				} 
				continue;
			}
			#endif


			if (Newdemo_state == ND_STATE_PLAYBACK )	{
				switch (key) {

				case KEY_DEBUGGED + KEY_I:	
					Newdemo_do_interpolate = !Newdemo_do_interpolate;
					if (Newdemo_do_interpolate)
						mprintf ((0, "demo playback interpolation now on\n"));
					else
						mprintf ((0, "demo playback interpolation now off\n"));
					break;
				case KEY_F3:				toggle_cockpit();			break;
				case KEY_SHIFTED+KEY_MINUS:
				case KEY_MINUS:			shrink_window();			break;
				case KEY_SHIFTED+KEY_EQUAL:
				case KEY_EQUAL:			grow_window();				break;
				case KEY_F2:				Config_menu_flag = 1;	break;
				case KEY_F7:				
					Show_kill_list = (Show_kill_list+1) % ((Newdemo_game_mode & GM_TEAM) ? 3 : 2);
					break;
				case KEY_BACKSP:
					Int3();
					break;
				case KEY_ESC: 				
					Function_mode = FMODE_MENU;
					break;
				case KEY_UP:
					Newdemo_vcr_state = ND_STATE_PLAYBACK;
					break;
				case KEY_DOWN:
					Newdemo_vcr_state = ND_STATE_PAUSED;
					break;
				case KEY_LEFT:
					newdemo_single_frame_time = timer_get_fixed_seconds();
					Newdemo_vcr_state = ND_STATE_ONEFRAMEBACKWARD;
					break;
				case KEY_RIGHT:
					newdemo_single_frame_time = timer_get_fixed_seconds();
					Newdemo_vcr_state = ND_STATE_ONEFRAMEFORWARD;
					break;
				case KEY_CTRLED + KEY_RIGHT:
					newdemo_goto_end();
					break;
				case KEY_CTRLED + KEY_LEFT:
					newdemo_goto_beginning();
					break;
	  			case KEY_PRINT_SCREEN: {
					int old_state;
			
					old_state = Newdemo_vcr_state;
					Newdemo_vcr_state = ND_STATE_PRINTSCREEN;
					game_render_frame_mono();
				 	save_screen_shot(0);
					Newdemo_vcr_state = old_state;
					break;
					}
				}
				break;
	  		}


#ifndef FINAL_CHEATS
			//Here are the "legal" cheats
			if (Cheats_enabled && !(Game_mode&GM_MULTI))
				switch (key) {
					case KEY_ALTED+KEY_1: {
						int i;

						HUD_init_message(TXT_WOWIE_ZOWIE);

						#ifndef SHAREWARE
							Players[Player_num].primary_weapon_flags = 0xff;
							Players[Player_num].secondary_weapon_flags = 0xff;
						#else
							Players[Player_num].primary_weapon_flags = 0xff ^ (HAS_PLASMA_FLAG | HAS_FUSION_FLAG);
							Players[Player_num].secondary_weapon_flags = 0xff ^ (HAS_SMART_FLAG | HAS_MEGA_FLAG);
						#endif

						for (i=0; i<MAX_PRIMARY_WEAPONS; i++)
							Players[Player_num].primary_ammo[i] = Primary_ammo_max[i];
						
						for (i=0; i<MAX_SECONDARY_WEAPONS; i++)
							Players[Player_num].secondary_ammo[i] = Secondary_ammo_max[i];
						
						if (Newdemo_state == ND_STATE_RECORDING)
							newdemo_record_laser_level(Players[Player_num].laser_level, MAX_LASER_LEVEL);

						Players[Player_num].energy = MAX_ENERGY;
						Players[Player_num].laser_level = MAX_LASER_LEVEL;
						Players[Player_num].flags |= PLAYER_FLAGS_QUAD_LASERS;
						update_laser_weapon_info();

						break;
					}
	
					case KEY_ALTED+KEY_2:
						HUD_init_message(TXT_ALL_KEYS);
						Players[Player_num].flags |= PLAYER_FLAGS_BLUE_KEY | PLAYER_FLAGS_RED_KEY | PLAYER_FLAGS_GOLD_KEY;
						break;
	
					case KEY_ALTED+KEY_3:
						Players[Player_num].flags ^= PLAYER_FLAGS_INVULNERABLE;
						HUD_init_message("%s %s!", TXT_INVULNERABILITY, (Players[Player_num].flags&PLAYER_FLAGS_INVULNERABLE)?TXT_ON:TXT_OFF);
						Players[Player_num].invulnerable_time = GameTime+i2f(1000);
						break;
	
					case KEY_ALTED+KEY_4:
						Players[Player_num].flags ^= PLAYER_FLAGS_CLOAKED;
						HUD_init_message("%s %s!", TXT_CLOAK, (Players[Player_num].flags&PLAYER_FLAGS_CLOAKED)?TXT_ON:TXT_OFF);
						if (Players[Player_num].flags & PLAYER_FLAGS_CLOAKED) {
							ai_do_cloak_stuff();
							Players[Player_num].cloak_time = GameTime;
						}
						break;

					case KEY_ALTED+KEY_5:
						HUD_init_message(TXT_FULL_SHIELDS);
						Players[Player_num].shields = MAX_SHIELDS;
						break;

					case KEY_ALTED+KEY_6: {
						newmenu_item m;
						char text[10]="";
						int new_level_num;
						int item;
						m.type=NM_TYPE_INPUT; m.text_len = 10; m.text = text;
						item = newmenu_do( NULL, TXT_WARP_TO_LEVEL, 1, &m, NULL );
						if (item != -1) {
							new_level_num = atoi(m.text);
							if (new_level_num!=0 && new_level_num>=0 && new_level_num<=LAST_LEVEL)
								StartNewLevel(new_level_num);
						}
						break;

					}
				}
#endif

			//--killed--Players[Player_num].flags &= ~PLAYER_FLAGS_AFTERBURNER;	//	Turn off, keypress might turn it on.

			switch (key) {

				//	================================================================================================
				//FIRST ARE ALL THE REAL GAME KEYS.  PUT TEST AND DEBUG KEYS LATER.

				//--killed--case KEY_SHIFTED+KEY_A:	toggle_afterburner_status();	break;
				case KEY_ESC:
					Game_aborted=1; 				
					Function_mode = FMODE_MENU;
					break;
				case KEY_F1: 				do_show_help();			break;
				case KEY_F2:				Config_menu_flag = 1;	break;
				case KEY_F3:				toggle_cockpit();			break;
				case KEY_F4:				palette_save(); joydefs_calibrate(); palette_restore(); break;
				case KEY_F5:	
						if ( Newdemo_state == ND_STATE_RECORDING )
							newdemo_stop_recording();
						else if ( Newdemo_state == ND_STATE_NORMAL )			
							newdemo_start_recording();
						break;
				case KEY_F6:				
					Show_reticle_name = (Show_reticle_name+1)%2;
					break;	
				case KEY_F7:				
					Show_kill_list = (Show_kill_list+1) % ((Game_mode & GM_TEAM) ? 3 : 2);
					break;
				case KEY_F8:	
					multi_send_message_start();
					break;
				case KEY_F9:
				case KEY_F10:
				case KEY_F11:
				case KEY_F12:
					multi_send_macro(key);
					break;		// send taunt macros
	
				case KEY_ALTED + KEY_F9:
					multi_send_audio_taunt(0);
					break;
				case KEY_ALTED + KEY_F10:
					multi_send_audio_taunt(1);
					break;
				case KEY_ALTED + KEY_F11:
					multi_send_audio_taunt(2);
					break;
				case KEY_ALTED + KEY_F12:
					multi_send_audio_taunt(3);
					break;

				case KEY_SHIFTED + KEY_F9:
				case KEY_SHIFTED + KEY_F10:
				case KEY_SHIFTED + KEY_F11:
				case KEY_SHIFTED + KEY_F12:
					multi_define_macro(key);
					break;		// redefine taunt macros

				case KEY_PAUSE:			do_game_pause(1); 		break;
	  			case KEY_PRINT_SCREEN: 	save_screen_shot(0);		break;

				case KEY_SHIFTED+KEY_MINUS:
				case KEY_MINUS:			shrink_window();			break;
				case KEY_SHIFTED+KEY_EQUAL:
				case KEY_EQUAL:			grow_window();				break;

				//	Select primary or secondary weapon.
				case KEY_1:
				case KEY_2:
				case KEY_3:
				case KEY_4:
				case KEY_5:

					do_weapon_select(key - KEY_1 , 0);
					break;

				case KEY_6:
				case KEY_7:
				case KEY_8:
				case KEY_9:
				case KEY_0:

					do_weapon_select(key - KEY_6 , 1);
					break;

				case KEY_SHIFTED + KEY_ESC:	//quick exit
					#ifdef EDITOR
						if (! SafetyCheck()) break;
						if (Screen_mode == SCREEN_EDITOR )
							close_editor_screen();
					#endif

					Game_aborted=1;
					Function_mode=FMODE_EXIT;
					break;

#ifdef SHAREWARE
				case KEY_ALTED+KEY_F2:
				case KEY_ALTED+KEY_F3:
					HUD_init_message( TXT_ONLY_REGISTERED );
					digi_play_sample( SOUND_BAD_SELECTION, F1_0 );
					break;
#else
				case KEY_ALTED+KEY_F2:	state_save_all( 0 );		break;	// 0 means not between levels.
				case KEY_ALTED+KEY_F3:	state_restore_all();		break;
#endif

				//use function keys for window sizing

				//	================================================================================================
				//ALL KEYS BELOW HERE GO AWAY IN RELEASE VERSION

			#ifndef RELEASE
	
				case KEY_DEBUGGED+KEY_0:	show_weapon_status();	break;

				case KEY_DEBUGGED+KEY_1:	create_special_path();	break;

				case KEY_DEBUGGED+KEY_Y:
					do_controlcen_destroyed_stuff(NULL);
					break;

#ifndef SHAREWARE
				case KEY_SHIFTED+KEY_F1:	multi_dump_robots(); 	break;
#endif
				case KEY_BACKSP:
				case KEY_CTRLED+KEY_BACKSP:
				case KEY_ALTED+KEY_BACKSP:
				case KEY_SHIFTED+KEY_BACKSP:
				case KEY_SHIFTED+KEY_ALTED+KEY_BACKSP:
				case KEY_CTRLED+KEY_ALTED+KEY_BACKSP:
				case KEY_SHIFTED+KEY_CTRLED+KEY_BACKSP:
				case KEY_SHIFTED+KEY_CTRLED+KEY_ALTED+KEY_BACKSP:

 						Int3(); break;

				case KEY_DEBUGGED+KEY_S:			digi_reset(); break;

				case KEY_DEBUGGED+KEY_P:
	 				if (Game_suspended & SUSP_ROBOTS)
	 					Game_suspended &= ~SUSP_ROBOTS;         //robots move
	 				else
	 					Game_suspended |= SUSP_ROBOTS;          //robots don't move
					break;
	
#ifdef VICTOR
				case KEY_V + KEY_ALTED:
					victor_init_graphics();
					break;
				case KEY_SHIFTED+KEY_F6:
					Victor_eye_switch = !Victor_eye_switch;
					break;
#endif
	
	
				case KEY_DEBUGGED+KEY_K:	Players[Player_num].shields = 1;	break;						//	a virtual kill
				case KEY_DEBUGGED+KEY_SHIFTED + KEY_K:	Players[Player_num].shields = -1;	break;	//	an actual kill
				case KEY_DEBUGGED+KEY_X: Players[Player_num].lives++; break; // Extra life cheat key.
				case KEY_DEBUGGED+KEY_H:
//					if (!(Game_mode & GM_MULTI) )	{
						Players[Player_num].flags ^= PLAYER_FLAGS_CLOAKED;
						if (Players[Player_num].flags & PLAYER_FLAGS_CLOAKED) {
							if (Game_mode & GM_MULTI)
								multi_send_cloak();
							ai_do_cloak_stuff();
							Players[Player_num].cloak_time = GameTime;
							mprintf((0, "You are cloaked!\n"));
						} else
							mprintf((0, "You are DE-cloaked!\n"));
//					}
					break;


				case KEY_DEBUGGED+KEY_R:
					Robot_firing_enabled = !Robot_firing_enabled;
					break;

				#ifdef EDITOR		//editor-specific functions

					case KEY_E + KEY_DEBUGGED:
							network_leave_game();
							Function_mode = FMODE_EDITOR; 
							break;
	
					case KEY_C + KEY_SHIFTED + KEY_DEBUGGED: 
						if (!( Game_mode & GM_MULTI ))
							move_player_2_segment(Cursegp,Curside); 
						break;   //move eye to curseg
	
					case KEY_S + KEY_SHIFTED + KEY_DEBUGGED:	//toggle screen (between editor & game screens)
						if (Screen_mode == SCREEN_EDITOR )      
							close_editor_screen();
						set_screen_mode(Screen_mode ^ (SCREEN_GAME^SCREEN_EDITOR));
						break;
	
					case KEY_DEBUGGED+KEY_W:	draw_world_from_game();	break;

				#endif	//#ifdef EDITOR
	
				//flythrough keys
				// case KEY_DEBUGGED+KEY_SHIFTED+KEY_F:	toggle_flythrough(); break;
				// case KEY_LEFT:          ft_preference=FP_LEFT; break;
				// case KEY_RIGHT: 			ft_preference=FP_RIGHT; break;
				// case KEY_UP:            ft_preference=FP_UP; break;
				// case KEY_DOWN:          ft_preference=FP_DOWN; break;
	
				case KEY_DEBUGGED+KEY_LAPOSTRO: Show_view_text_timer = 0x30000; object_goto_next_viewer(); break;
				case KEY_DEBUGGED+KEY_SHIFTED+KEY_LAPOSTRO: Viewer=ConsoleObject; break;
	
#ifndef NDEBUG
	  			case KEY_DEBUGGED+KEY_O: toggle_outline_mode(); break;
#endif
	  			case KEY_DEBUGGED+KEY_T:
					*Toggle_var = !*Toggle_var;
					mprintf((0, "Variable at %08x set to %i\n", Toggle_var, *Toggle_var));
					break;
	  			case KEY_DEBUGGED + KEY_L: 
					if (++Lighting_on >= 2) Lighting_on = 0; break;
	  			case KEY_DEBUGGED + KEY_SHIFTED + KEY_L: 
					Beam_brightness=0x38000-Beam_brightness; break;
	  			case KEY_PAD5: slew_stop(); break;
	
	  			case KEY_DEBUGGED + KEY_F11: play_test_sound(); break;
	  			case KEY_DEBUGGED + KEY_SHIFTED+KEY_F11: advance_sound(); play_test_sound(); break;

				case KEY_DEBUGGED +KEY_F4: {
					//fvi_info hit_data;
					//vms_vector p0 = {-0x1d99a7,-0x1b20000,0x186ab7f};
					//vms_vector p1 = {-0x217865,-0x1b20000,0x187de3e};
					//find_vector_intersection(&hit_data,&p0,0x1b9,&p1,0x40000,0x0,NULL,-1);
					break;
				}
	
				case KEY_DEBUGGED + KEY_M:
					Debug_spew = !Debug_spew;
					if (Debug_spew) {
						mopen( 0, 8, 1, 78, 16, "Debug Spew");
						HUD_init_message( "Debug Spew: ON" );
					} else {
						mclose( 0 );
						HUD_init_message( "Debug Spew: OFF" );
					}
					break;		
	
				case KEY_DEBUGGED + KEY_C:	

						do_cheat_menu(); 
						break;
				case KEY_DEBUGGED + KEY_A:	{
						do_megawow_powerup();
//						if ( Game_mode & GM_MULTI )	{
//							nm_messagebox( NULL, 1, "Damn", "CHEATER!\nYou cannot use the\nmega-thing in network mode." );
//							Network_message_reciever = 100;		// Send to everyone...
//							sprintf( Network_message, "%s cheated!", Players[Player_num].callsign);
//						} else {
//							do_megawow_powerup();
//						}
						break;
				}
	
	 			case KEY_DEBUGGED+KEY_F:	framerate_on = !framerate_on; break;
 	
 				case KEY_DEBUGGED+KEY_SPACEBAR:              //KEY_F7:                       // Toggle physics flying
 					slew_stop();
					game_flush_inputs();
 					if ( ConsoleObject->control_type != CT_FLYING ) {
 						fly_init(ConsoleObject);
 						Game_suspended &= ~SUSP_ROBOTS;         //robots move
 					} else {
 						slew_init(ConsoleObject);                                              //start player slewing
 						Game_suspended |= SUSP_ROBOTS;          //robots don't move
					}
					break;
		
				case KEY_DEBUGGED+KEY_COMMA: Render_zoom = fixmul(Render_zoom,62259); break;
				case KEY_DEBUGGED+KEY_PERIOD: Render_zoom = fixmul(Render_zoom,68985); break;
	
				case KEY_DEBUGGED+KEY_P+KEY_SHIFTED: Debug_pause = 1; break;
	
				//case KEY_F7: {
				//	char mystr[30];
				//	sprintf(mystr,"mark %i start",Mark_count);
				//	_MARK_(mystr);
				//	break;
				//}
				//case KEY_SHIFTED+KEY_F7: {
				//	char mystr[30];
				//	sprintf(mystr,"mark %i end",Mark_count);
				//	Mark_count++;
				//	_MARK_(mystr);
				//	break;
				//}
	
		
#ifndef NDEBUG
				case KEY_DEBUGGED+KEY_F8: speedtest_init(); Speedtest_count = 1;	break;
				case KEY_DEBUGGED+KEY_F9: speedtest_init(); Speedtest_count = 10;	break;

				case KEY_DEBUGGED+KEY_D:
					if ((Game_double_buffer = !Game_double_buffer)!=0)
						init_game_screen();
					break;
#endif

#ifdef EDITOR
				case KEY_DEBUGGED+KEY_Q:
					stop_time();
					dump_used_textures_all();
					start_time();
					break;
#endif

				#endif		//#ifndef RELEASE

				default:        break;

			}       //switch (key)
		}




}

#ifndef	NDEBUG
int	Debug_slowdown=0;
#endif

#ifdef EDITOR
extern void player_follow_path(object *objp);
extern void check_create_player_path(void);

#endif

extern	int	Do_appearance_effect;

void GameLoop(int RenderFlag, int ReadControlsFlag )
{
	static int desc_dead_countdown=100;   /*  used if player shouldn't be playing */

#ifndef	NDEBUG
	//	Used to slow down frame rate for testing things.
//	RenderFlag = 1; // DEBUG
	if (Debug_slowdown) {
		int	h, i, j=0;

		for (h=0; h<Debug_slowdown; h++)
			for (i=0; i<1000; i++)
				j += i;
	}
#endif

		if (desc_id_exit_num) {				 // are we supposed to be checking
			if (!(--desc_dead_countdown))  // if so, at zero, then pull the plug
				Error ("Loading overlay -- error number: %d\n", (int)desc_id_exit_num);
		}
				
		#ifndef RELEASE
		if (FindArg("-invulnerability"))
			Players[Player_num].flags |= PLAYER_FLAGS_INVULNERABLE;
		#endif

		update_player_stats();
		diminish_palette_towards_normal();		//	Should leave palette effect up for as long as possible by putting right before render.
		//--killed--do_afterburner_stuff();
		do_cloak_stuff();
		do_invulnerable_stuff();
		remove_obsolete_stuck_objects();
#ifdef EDITOR
check_create_player_path();
player_follow_path(ConsoleObject);
#endif
		if (Game_mode & GM_MULTI)
			multi_do_frame();
	
		if (RenderFlag) {
			if (force_cockpit_redraw) {			//screen need redrawing?
				init_game_screen();
				force_cockpit_redraw=0;
			}
			game_render_frame();
		}

//		mprintf(0,"Velocity %2.2f\n", f2fl(vm_vec_mag(&ConsoleObject->phys_info.velocity)));

		calc_frame_time();

		dead_player_frame();
		if (Newdemo_state != ND_STATE_PLAYBACK)
			do_controlcen_dead_frame();

		if (ReadControlsFlag)
			ReadControls();
		else
			memset(&Controls, 0, sizeof(Controls));

		GameTime += FrameTime;

		digi_sync_sounds();

		if (Endlevel_sequence) {
			do_endlevel_frame();
			powerup_grab_cheat_all();
			do_special_effects();
			return;					//skip everything else
		}

		#ifdef ARCADE
		if (Arcade_mode && (Arcade_timer<0) && (Newdemo_state != ND_STATE_PLAYBACK) )	{
			memset( &Controls, 0, sizeof(Controls) );
			continue;
		}
		#endif

		if (Newdemo_state != ND_STATE_PLAYBACK)
			do_exploding_wall_frame();
		if ((Newdemo_state != ND_STATE_PLAYBACK) || (Newdemo_vcr_state != ND_STATE_PAUSED)) {
			do_special_effects();
			wall_frame_process();
			triggers_frame_process();
		}


		if (Fuelcen_control_center_destroyed)	{
			if (Newdemo_state==ND_STATE_RECORDING )
				newdemo_record_control_center_destroyed();
			flash_frame();
		}
		
		if ( Newdemo_state == ND_STATE_PLAYBACK )	{
			newdemo_playback_one_frame();
			if ( Newdemo_state != ND_STATE_PLAYBACK )		{
//	  			gr_palette_fade_out( gr_palette, 32, 0 );
//				show_title_screen( "descent.pcx", 1 );
//				Screen_mode=-1; set_screen_mode(SCREEN_GAME);
//				clear_palette();
//				init_game_screen();
//				last_cockpit_drawn = -1;
//				#ifdef ARCADE
//				if ( Arcade_timer > 0 ) {
//					NewGame(1);
//				} else
//				#endif
//					newdemo_toggle_playback();			//NOTE LINK TO ABOVE!
				longjmp( LeaveGame, 0 );		// Go back to menu
			}
		} else	
		{ // Note the link to above!

			Players[Player_num].homing_object_dist = -1;		//	Assume not being tracked.  Laser_do_weapon_sequence modifies this.

			object_move_all();	
			powerup_grab_cheat_all();

			if (Endlevel_sequence)	//might have been started during move
				return;

			fuelcen_update_all();

			do_ai_frame_all();

			if (allowed_to_fire_laser()) {
				Global_laser_firing_count += Weapon_info[Primary_weapon_to_weapon_info[Primary_weapon]].fire_count * (Controls.fire_primary_state || Controls.fire_primary_down_count);
				if ((Primary_weapon == FUSION_INDEX) && (Global_laser_firing_count)) {
					if ((Players[Player_num].energy < F1_0*5) && (Auto_fire_fusion_cannon_time == 0)) {
						Global_laser_firing_count = 0;
					} else {
						if (Fusion_charge == 0)
							Players[Player_num].energy -= F1_0*5;

						Fusion_charge += FrameTime;
						Players[Player_num].energy -= FrameTime*4;

						if (Players[Player_num].energy <= 0) {
							Players[Player_num].energy = 0;
							Auto_fire_fusion_cannon_time = GameTime -1;				//	Fire now!
						} else
							Auto_fire_fusion_cannon_time = GameTime + FrameTime/2 + 1;		//	Fire the fusion cannon at this time in the future.

						if (Fusion_charge < F1_0*2)
							PALETTE_FLASH_ADD(Fusion_charge >> 11, 0, Fusion_charge >> 11);
						else
							PALETTE_FLASH_ADD(Fusion_charge >> 11, Fusion_charge >> 11, 0);

						if (Fusion_next_sound_time < GameTime) {
							if (Fusion_charge > F1_0*2) {
								digi_play_sample( 11, F1_0 );
								apply_damage_to_player(ConsoleObject, ConsoleObject, rand() * 4);
							} else {
								digi_play_sample( SOUND_FUSION_WARMUP, F1_0 );
								if (Game_mode & GM_MULTI)
									multi_send_play_sound(SOUND_FUSION_WARMUP, F1_0);
							}
							Fusion_next_sound_time = GameTime + F1_0/8 + rand()/4;
						}
					}
				}
			}

			if (Auto_fire_fusion_cannon_time) {
				if (Primary_weapon != FUSION_INDEX)
					Auto_fire_fusion_cannon_time = 0;
				else if (GameTime + FrameTime/2 >= Auto_fire_fusion_cannon_time) {
					Auto_fire_fusion_cannon_time = 0;
					Global_laser_firing_count = 1;
				} else {
					vms_vector	rand_vec;
					fix			bump_amount;

					Global_laser_firing_count = 0;

					ConsoleObject->phys_info.rotvel.x += (rand() - 16384)/8;
					ConsoleObject->phys_info.rotvel.z += (rand() - 16384)/8;
					make_random_vector(&rand_vec);

					bump_amount = F1_0*4;

					if (Fusion_charge > F1_0*2)
						bump_amount = Fusion_charge*4;

					bump_one_object(ConsoleObject, &rand_vec, bump_amount);
				}
			}

			if (Global_laser_firing_count) {
				if (Fusion_charge > F1_0*2)
					Fusion_charge = F1_0*2;
				Global_laser_firing_count -= do_laser_firing_player();	//do_laser_firing(Players[Player_num].objnum, Primary_weapon);
			}

			if (Global_laser_firing_count < 0)
				Global_laser_firing_count = 0;
		}

	if (Do_appearance_effect) {
		create_player_appearance_effect(ConsoleObject);
		Do_appearance_effect = 0;
	}

}

//	-------------------------------------------------------------------------------------------------------
//	If player is close enough to objnum, which ought to be a powerup, pick it up!
//	This could easily be made difficulty level dependent.
void powerup_grab_cheat(object *player, int objnum)
{
	fix	powerup_size;
	fix	player_size;
	fix	dist;

	Assert(Objects[objnum].type == OBJ_POWERUP);

	powerup_size = Objects[objnum].size;
	player_size = player->size;

	dist = vm_vec_dist_quick(&Objects[objnum].pos, &player->pos);

	if ((dist < 2*(powerup_size + player_size)) && !(Objects[objnum].flags & OF_SHOULD_BE_DEAD)) {
		vms_vector	collision_point;

		vm_vec_avg(&collision_point, &Objects[objnum].pos, &player->pos);
		collide_player_and_powerup(player, &Objects[objnum], &collision_point);
	}
}

//	-------------------------------------------------------------------------------------------------------
//	Make it easier to pick up powerups.
//	For all powerups in this segment, pick them up at up to twice pickuppable distance based on dot product
//	from player to powerup and player's forward vector.
//	This has the effect of picking them up more easily left/right and up/down, but not making them disappear
//	way before the player gets there.
void powerup_grab_cheat_all(void)
{
	segment	*segp;
	int		objnum;

	segp = &Segments[ConsoleObject->segnum];
	objnum = segp->objects;

	while (

