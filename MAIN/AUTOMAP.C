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
 * $Source: f:/miner/source/main/rcs/automap.c $
 * $Revision: 2.2 $
 * $Author: john $
 * $Date: 1995/03/21 14:41:26 $
 * 
 * Routines for displaying the auto-map.
 * 
 * $Log: automap.c $
 * Revision 2.2  1995/03/21  14:41:26  john
 * Ifdef'd out the NETWORK code.
 * 
 * Revision 2.1  1995/03/20  18:16:06  john
 * Added code to not store the normals in the segment structure.
 * 
 * Revision 2.0  1995/02/27  11:32:55  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.117  1995/02/22  14:11:31  allender
 * remove anonymous unions from object structure
 * 
 * Revision 1.116  1995/02/22  13:24:39  john
 * Removed the vecmat anonymous unions.
 * 
 * Revision 1.115  1995/02/09  14:57:02  john
 * Reduced mem usage. Made automap slide farther.
 * 
 * Revision 1.114  1995/02/07  20:40:44  rob
 * Allow for anarchy automap of player pos by option.
 * 
 * Revision 1.113  1995/02/07  15:45:33  john
 * Made automap memory be static.
 * 
 * Revision 1.112  1995/02/02  12:24:00  adam
 * played with automap labels
 * 
 * Revision 1.111  1995/02/02  01:52:52  john
 * Made the automap use small font.
 * 
 * Revision 1.110  1995/02/02  01:34:34  john
 * Made Reset in automap not change segmentlimit.
 * 
 * Revision 1.109  1995/02/02  01:23:11  john
 * Finalized the new automap partial viewer.
 * 
 * Revision 1.108  1995/02/02  00:49:45  mike
 * new automap segment-depth functionality.
 * 
 * Revision 1.107  1995/02/02  00:23:04  john
 * Half of the code for new connected distance stuff in automap.
 * 
 * Revision 1.106  1995/02/01  22:54:00  john
 * Made colored doors not fade in automap. Made default
 * viewing area be maxxed.
 * 
 * Revision 1.105  1995/02/01  13:16:13  john
 * Added great grates. 
 * 
 * Revision 1.104  1995/01/31  12:47:06  john
 * Made Alt+F only work with cheats enabled.
 * 
 * Revision 1.103  1995/01/31  12:41:23  john
 * Working with new controls.
 * 
 * Revision 1.102  1995/01/31  12:04:19  john
 * Version 2 of new key control.
 * 
 * Revision 1.101  1995/01/31  11:32:00  john
 * First version of new automap system.
 * 
 * Revision 1.100  1995/01/28  16:55:48  john
 * Made keys draw in automap in the segments that you have
 * visited.
 * 
 * Revision 1.99  1995/01/28  14:44:51  john
 * Made hostage doors show up on automap.
 * 
 * Revision 1.98  1995/01/22  17:03:49  rob
 * Fixed problem drawing playerships in automap coop/team mode
 * 
 * Revision 1.97  1995/01/21  17:23:11  john
 * Limited S movement in map. Made map bitmap load from disk
 * and then freed it.
 * 
 * Revision 1.96  1995/01/19  18:55:38  john
 * Don't draw players in automap if not obj_player.
 * 
 * Revision 1.95  1995/01/19  18:48:13  john
 * Made player colors better in automap.
 * 
 * Revision 1.94  1995/01/19  17:34:52  rob
 * Added team colorizations in automap.
 * 
 * Revision 1.93  1995/01/19  17:15:36  rob
 * Trying to add player ships into map for coop and team mode.
 * 
 * Revision 1.92  1995/01/19  17:11:09  john
 * Added code for Rob to draw Multiplayer ships in automap.
 * 
 * Revision 1.91  1995/01/12  13:35:20  john
 * Fixed bug with Segment 0 not getting displayed 
 * in automap if you have EDITOR compiled in.
 * 
 * Revision 1.90  1995/01/08  16:17:14  john
 * Added code to draw player's up vector while in automap.
 * 
 * Revision 1.89  1995/01/08  16:09:41  john
 * Fixed problems with grate.
 * 
 * Revision 1.88  1994/12/14  22:54:17  john
 * Fixed bug that didn't show hostages in automap.
 * 
 * Revision 1.87  1994/12/09  00:41:03  mike
 * fix hang in automap print screen
 * 
 * Revision 1.86  1994/12/05  23:37:15  matt
 * Took out calls to warning() function
 * 
 * Revision 1.85  1994/12/03  22:35:28  yuan
 * Localization 412
 * 
 * Revision 1.84  1994/12/02  15:05:45  matt
 * Added new "official" cheats
 * 
 * Revision 1.83  1994/11/30  12:10:49  adam
 * added support for PCX titles/brief screens
 * 
 * Revision 1.82  1994/11/27  23:15:12  matt
 * Made changes for new mprintf calling convention
 * 
 * Revision 1.81  1994/11/27  15:35:52  matt
 * Enable screen shots even when debugging is turned off
 * 
 * Revision 1.80  1994/11/26  22:51:43  matt
 * Removed editor-only fields from segment structure when editor is compiled
 * out, and padded segment structure to even multiple of 4 bytes.
 * 
 * Revision 1.79  1994/11/26  16:22:48  matt
 * Reduced leave_time
 * 
 * Revision 1.78  1994/11/23  22:00:10  mike
 * show level number.
 * 
 * Revision 1.77  1994/11/21  11:40:33  rob
 * Tweaked the game-loop for automap in multiplayer games.
 * 
 * Revision 1.76  1994/11/18  16:42:06  adam
 * removed a font
 * 
 * Revision 1.75  1994/11/17  13:06:48  adam
 * changed font
 * 
 * Revision 1.74  1994/11/14  20:47:17  john
 * Attempted to strip out all the code in the game 
 * directory that uses any ui code.
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: automap.c 2.2 1995/03/21 14:41:26 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "error.h"
#include "3d.h"
#include "inferno.h"
#include "mem.h"
#include "render.h"
#include "object.h"
#include "vclip.h"
#include "game.h"
#include "mono.h"
#include "polyobj.h"
#include "sounds.h"
#include "player.h"
#include "bm.h"
#include "key.h"
#include "screens.h"
#include "textures.h"
#include "mouse.h"
#include "timer.h"
#include "segpoint.h"
#include "joy.h"
#include "iff.h"
#include "pcx.h"
#include "palette.h"
#include "wall.h"
#include "hostage.h"
#include "fuelcen.h"
#include "gameseq.h"
#include "gamefont.h"
#include "network.h"
#include "kconfig.h"
#include "multi.h"
#include "endlevel.h"
#include "text.h"
#include "gauges.h"
#include "powerup.h"
#include "network.h" 

#define EF_USED			1		// This edge is used
#define EF_DEFINING		2		// A structure defining edge that should always draw.
#define EF_FRONTIER		4		// An edge between the known and the unknown.
#define EF_SECRET			8		// An edge that is part of a secret wall.
#define EF_GRATE			16		// A grate... draw it all the time.
#define EF_NO_FADE		32		// An edge that doesn't fade with distance
#define EF_TOO_FAR		64		// An edge that is too far away

typedef struct Edge_info {
	union {
		short	verts[2];		// 4 bytes
		long vv;
	};
	ubyte sides[4];			// 4 bytes
	short segnum[4];			// 8 bytes	// This might not need to be stored... If you can access the normals of a side.
	ubyte flags;				// 1 bytes 	// See the EF_??? defines above.
	ubyte color;				// 1 bytes
	ubyte num_faces;			// 1 bytes	// 19 bytes...
} Edge_info;

//OLD BUT GOOD -- #define MAX_EDGES_FROM_VERTS(v)   ((v*5)/2)
// THE following was determined by John by loading levels 1-14 and recording
// numbers on 10/26/94. 
//#define MAX_EDGES_FROM_VERTS(v)   (((v)*21)/10)
#define MAX_EDGES_FROM_VERTS(v)		((v)*4)
//#define MAX_EDGES (MAX_EDGES_FROM_VERTS(MAX_VERTICES))

#define MAX_EDGES 6000		// Determined by loading all the levels by John & Mike, Feb 9, 1995

#define	WALL_NORMAL_COLOR 				BM_XRGB( 29, 29, 29 )
#define	WALL_DOOR_COLOR					BM_XRGB( 21, 31, 11 )
#define	WALL_DOOR_BLUE						BM_XRGB( 0, 0, 31)
#define	WALL_DOOR_GOLD						BM_XRGB( 31, 31, 0)
#define	WALL_DOOR_RED						BM_XRGB( 31, 0, 0)

// Segment visited list
ubyte Automap_visited[MAX_SEGMENTS];

// Edge list variables
static int Num_edges=0;
static int Max_edges;		//set each frame
static int Highest_edge_index = -1;
static Edge_info Edges[MAX_EDGES];
static short DrawingListBright[MAX_EDGES];

//static short DrawingListBright[MAX_EDGES];
//static short Edge_used_list[MAX_EDGES];				//which entries in edge_list have been used

// Map movement defines
#define PITCH_DEFAULT 9000
#define ZOOM_DEFAULT i2f(20*10)
#define ZOOM_MIN_VALUE i2f(20*5)
#define ZOOM_MAX_VALUE i2f(20*100)

#define SLIDE_SPEED 				(350)
#define ZOOM_SPEED_FACTOR		(1500)
#define ROT_SPEED_DIVISOR		(115000)

// Screen anvas variables
static int current_page=0;
static grs_canvas Pages[2];
static grs_canvas DrawingPages[2];

// Flags
static int Automap_cheat = 0;		// If set, show everything

// Rendering variables
static fix Automap_zoom = 0x9000;
static vms_vector view_target;
static fix Automap_farthest_dist = (F1_0 * 20 * 50);		// 50 segments away
static vms_matrix	ViewMatrix;
static fix ViewDist=0;

void automap_clear_visited()	
{
	int i;
	for (i=0; i<MAX_SEGMENTS; i++ )
		Automap_visited[i] = 0;
}

grs_canvas *name_canv;

void draw_player( object * obj )
{
	vms_vector arrow_pos, head_pos;
	g3s_point sphere_point, arrow_point, head_point;

	// Draw Console player -- shaped like a ellipse with an arrow.
	g3_rotate_point(&sphere_point,&obj->pos);
	g3_draw_sphere(&sphere_point,obj->size);

	// Draw shaft of arrow
	vm_vec_scale_add( &arrow_pos, &obj->pos, &obj->orient.fvec, obj->size*3 );
	g3_rotate_point(&arrow_point,&arrow_pos);
	g3_draw_line( &sphere_point, &arrow_point );

	// Draw right head of arrow
	vm_vec_scale_add( &head_pos, &obj->pos, &obj->orient.fvec, obj->size*2 );
	vm_vec_scale_add2( &head_pos, &obj->orient.rvec, obj->size*1 );
	g3_rotate_point(&head_point,&head_pos);
	g3_draw_line( &arrow_point, &head_point );

	// Draw left head of arrow
	vm_vec_scale_add( &head_pos, &obj->pos, &obj->orient.fvec, obj->size*2 );
	vm_vec_scale_add2( &head_pos, &obj->orient.rvec, obj->size*(-1) );
	g3_rotate_point(&head_point,&head_pos);
	g3_draw_line( &arrow_point, &head_point );

	// Draw player's up vector
	vm_vec_scale_add( &arrow_pos, &obj->pos, &obj->orient.uvec, obj->size*2 );
	g3_rotate_point(&arrow_point,&arrow_pos);
	g3_draw_line( &sphere_point, &arrow_point );
}


void draw_automap()
{
	int i;
	int color;
	object * objp;
	vms_vector viewer_position;
	g3s_point sphere_point;

	current_page ^= 1;
	gr_set_current_canvas(&DrawingPages[current_page]);

	gr_clear_canvas(0);

	g3_start_frame();
	render_start_frame();
	
	vm_vec_scale_add(&viewer_position,&view_target,&ViewMatrix.fvec,-ViewDist );

	g3_set_view_matrix(&viewer_position,&ViewMatrix,Automap_zoom);

	draw_all_edges();

	// Draw player...
#ifdef NETWORK
	if (Game_mode & GM_TEAM)
		color = get_team(Player_num);
	else
#endif	
		color = Player_num;	// Note link to above if!
	gr_setcolor(gr_getcolor(player_rgb[color].r,player_rgb[color].g,player_rgb[color].b));
	draw_player(&Objects[Players[Player_num].objnum]);
				
	// Draw player(s)...
#ifdef NETWORK
	if ( (Game_mode & (GM_TEAM | GM_MULTI_COOP)) || (Netgame.game_flags & NETGAME_FLAG_SHOW_MAP) )	{
		for (i=0; i<N_players; i++)		{
			if ( (i != Player_num) && ((Game_mode & GM_MULTI_COOP) || (get_team(Player_num) == get_team(i)) || (Netgame.game_flags & NETGAME_FLAG_SHOW_MAP)) )	{
				if ( Objects[Players[i].objnum].type == OBJ_PLAYER )	{
					if (Game_mode & GM_TEAM)
						color = get_team(i);
					else
						color = i;
					gr_setcolor(gr_getcolor(player_rgb[color].r,player_rgb[color].g,player_rgb[color].b));
					draw_player(&Objects[Players[i].objnum]);
				}
			}
		}
	}
#endif

	objp = &Objects[0];
	for (i=0;i<=Highest_object_index;i++,objp++) {
		switch( objp->type )	{
		case OBJ_HOSTAGE:
			gr_setcolor(BM_XRGB(0,31,0));
			g3_rotate_point(&sphere_point,&objp->pos);
			g3_draw_sphere(&sphere_point,objp->size);	
			break;
		case OBJ_POWERUP:
			if ( Automap_visited[objp->segnum] )	{
				if ( (objp->id==POW_KEY_RED) || (objp->id==POW_KEY_BLUE) || (objp->id==POW_KEY_GOLD) )	{
					switch (objp->id) {
					case POW_KEY_RED:		gr_setcolor(gr_getcolor(63, 5, 5));	break;
					case POW_KEY_BLUE:	gr_setcolor(gr_getcolor(5, 5, 63)); break;
					case POW_KEY_GOLD:	gr_setcolor(gr_getcolor(63, 63, 10)); break;
					}
					g3_rotate_point(&sphere_point,&objp->pos);
					g3_draw_sphere(&sphere_point,objp->size*4);	
				}
			}
			break;
		}
	}

	g3_end_frame();

	gr_bitmapm(5,5,&name_canv->cv_bitmap);

	gr_show_canvas( &Pages[current_page] );
	
}

#define LEAVE_TIME 0x4000

//print to canvas & double height
grs_canvas *print_to_canvas(char *s,grs_font *font, int fc, int bc)
{
	int y;
	ubyte *data;
	int rs;
	grs_canvas *temp_canv,*save_canv;

	save_canv = grd_curcanv;

	temp_canv = gr_create_canvas(font->ft_w*strlen(s),font->ft_h*2);

	gr_set_current_canvas(temp_canv);
	gr_set_curfont(font);
	gr_clear_canvas(255);						//trans color
	gr_set_fontcolor(fc,bc);
	gr_printf(0,0,s);

	//now double it, since we're drawing to 400-line modex screen

	data = temp_canv->cv_bitmap.bm_data;
	rs = temp_canv->cv_bitmap.bm_rowsize;

	for (y=temp_canv->cv_bitmap.bm_h/2;y--;) {
		memcpy(data+(rs*y*2),data+(rs*y),temp_canv->cv_bitmap.bm_w);
		memcpy(data+(rs*(y*2+1)),data+(rs*y),temp_canv->cv_bitmap.bm_w);
	}

	gr_set_current_canvas(save_canv);

	return temp_canv;
}

//print to buffer, double heights, and blit bitmap to screen
modex_printf(int x,int y,char *s,int fontnum)
{
	grs_canvas *temp_canv;

	temp_canv = print_to_canvas(s,Gamefonts[fontnum], BM_XRGB(20,20,20), -1);

	gr_bitmapm(x,y,&temp_canv->cv_bitmap);

	gr_free_canvas(temp_canv);
}

create_name_canv()
{
	char	name_level[128];

	if (Current_level_num > 0)
		sprintf(name_level, "%s %i: ",TXT_LEVEL, Current_level_num);
	else
		name_level[0] = 0;

	strcat(name_level, Current_level_name);

	gr_set_fontcolor(BM_XRGB(0,31,0),-1);
	name_canv = print_to_canvas(name_level,Gamefonts[GFONT_SMALL], BM_XRGB(0,31,0), -1);

}

void modex_print_message(int x, int y, char *str)
{
	int	i;

	for (i=0; i<2; i++ )	{
		gr_set_current_canvas(&Pages[i]);
		modex_printf(x, y, str, GFONT_MEDIUM_1);
	}

	gr_set_current_canvas(&DrawingPages[current_page]);
}

extern void GameLoop(int, int );
extern int set_segment_depths(int start_seg, ubyte *segbuf);

void do_automap( int key_code )	{
	int done=0;
	vms_matrix	tempm;
	vms_angvec	tangles;
	int leave_mode=0;
	int first_time=1;
	int pcx_error;
	int i;
	int c;
	char filename[] = "MAP.PCX";
	fix entry_time;
	int pause_game=1;		// Set to 1 if everything is paused during automap...No pause during net.
	fix t1, t2;
	control_info saved_control_info;
	grs_bitmap Automap_background;
	int Max_segments_away = 0;
	int SegmentLimit = 1;

	key_code = key_code;	// disable warning...

	if ((Game_mode & GM_MULTI) && (Function_mode == FMODE_GAME) && (!Endlevel_sequence))
		pause_game = 0;

	if (pause_game)
		stop_time();

	create_name_canv();

	Max_edges = min(MAX_EDGES_FROM_VERTS(Num_vertices),MAX_EDGES);			//make maybe smaller than max
	//Edges	= malloc( sizeof(Edge_info)*Max_edges);
	//if ( Edges == NULL )	{
	//	mprintf((0, "Couldn't get %dK for automap!", sizeof(Edge_info)*Max_edges/1024));
	//	return;
	//}
	//DrawingListBright = malloc( sizeof(short)*Max_edges);
	//if ( DrawingListBright == NULL )	{
	//	mprintf((0, "Couldn't get %dK for automap!", sizeof(short)*Max_edges/1024));
	//	return;
	//}

	mprintf( (0, "Num_vertices=%d, Max_edges=%d, (MAX:%d)\n", Num_vertices, Max_edges, MAX_EDGES ));
	mprintf( (0, "Allocated %d K for automap edge list\n", (sizeof(Edge_info)+sizeof(short))*Max_edges/1024 ));

	gr_set_mode( SM_320x400U );
	gr_palette_clear();

	gr_init_sub_canvas(&Pages[0],grd_curcanv,0,0,320,400);
	gr_init_sub_canvas(&Pages[1],grd_curcanv,0,401,320,400);
	gr_init_sub_canvas(&DrawingPages[0],&Pages[0],16,69,288,272);
	gr_init_sub_canvas(&DrawingPages[1],&Pages[1],16,69,288,272);

	Automap_background.bm_data = NULL;
	pcx_error = pcx_read_bitmap(filename,&Automap_background,BM_LINEAR,NULL);
	if ( pcx_error != PCX_ERROR_NONE )	{
		printf("File %s - PCX error: %s",filename,pcx_errormsg(pcx_error));
		Error("File %s - PCX error: %s",filename,pcx_errormsg(pcx_error));
		return;
	}

	for (i=0; i<2; i++ )	{
		gr_set_current_canvas(&Pages[i]);
		gr_bitmap( 0, 0, &Automap_background );
		modex_printf( 40, 22,TXT_AUTOMAP,GFONT_BIG_1);
		modex_printf( 70,353,TXT_TURN_SHIP,GFONT_SMALL);
		modex_printf( 70,369,TXT_SLIDE_UPDOWN,GFONT_SMALL);
		modex_printf( 70,385,TXT_VIEWING_DISTANCE,GFONT_SMALL);
	}
	if ( Automap_background.bm_data )
		free( Automap_background.bm_data );
	Automap_background.bm_data = NULL;

	gr_set_current_canvas(&DrawingPages[current_page]);

	automap_build_edge_list();

	if ( ViewDist==0 ) 
		ViewDist = ZOOM_DEFAULT;
	ViewMatrix = Objects[Players[Player_num].objnum].orient;

	tangles.p = PITCH_DEFAULT;
	tangles.h  = 0;
	tangles.b  = 0;

	done = 0;

	view_target = Objects[Players[Player_num].objnum].pos;

	t1 = entry_time = timer_get_fixed_seconds();
	t2 = t1;

	//Fill in Automap_visited from Objects[Players[Player_num].objnum].segnum
	Max_segments_away = set_segment_depths(Objects[Players[Player_num].objnum].segnum, Automap_visited);
	SegmentLimit = Max_segments_away;

	adjust_segment_limit(SegmentLimit);

	while(!done)	{
		if ( leave_mode==0 && Controls.automap_state && (timer_get_fixed_seconds()-entry_time)>LEAVE_TIME)
			leave_mode = 1;

		if ( !Controls.automap_state && (leave_mode==1) )
			done=1;

		if (!pause_game)	{
			ushort old_wiggle;
			saved_control_info = Controls;				// Save controls so we can zero them
			memset(&Controls,0,sizeof(control_info));	// Clear everything...
			old_wiggle = ConsoleObject->mtype.phys_info.flags & PF_WIGGLE;	// Save old wiggle
			ConsoleObject->mtype.phys_info.flags &= ~PF_WIGGLE;		// Turn off wiggle
			#ifdef NETWORK
			if (multi_menu_poll())
				done = 1;
			#endif
//			GameLoop( 0, 0 );		// Do game loop with no rendering and no reading controls.
			ConsoleObject->mtype.phys_info.flags |= old_wiggle;	// Restore wiggle
			Controls = saved_control_info;
		} 

		controls_read_all();		
		if ( Controls.automap_down_count )	{
			if (leave_mode==0)
				done = 1;
			c = 0;
		}

		while( (c=key_inkey()) )	{
			switch( c ) {
			#ifndef NDEBUG
			case KEY_BACKSP: Int3(); break;
			#endif
	
			case KEY_PRINT_SCREEN: save_screen_shot(1); break;
	
			case KEY_ESC:
				if (leave_mode==0)
					done = 1;
				 break;
			case KEY_ALTED+KEY_F:		// Alt+F shows full map, if cheats enabled
				if (Cheats_enabled) 
				{
					uint t;	
					t = Players[Player_num].flags;
					Players[Player_num].flags |= PLAYER_FLAGS_MAP_ALL_CHEAT;
					automap_build_edge_list();
					Players[Player_num].flags=t;
			 	}
				break;
	
	#ifndef NDEBUG
		  	case KEY_DEBUGGED+KEY_F: 	{
				for (i=0; i<=Highest_segment_index; i++ )
					Automap_visited[i] = 1;
				automap_build_edge_list();
				Max_segments_away = set_segment_depths(Objects[Players[Player_num].objnum].segnum, Automap_visited);
				SegmentLimit = Max_segments_away;
				adjust_segment_limit(SegmentLimit);
				}
				break;
	#endif
			case KEY_MINUS:
				if (SegmentLimit > 1) 		{
					SegmentLimit--;
					adjust_segment_limit(SegmentLimit);
				}
				break;
			case KEY_EQUAL:
				if (SegmentLimit < Max_segments_away) 	{
					SegmentLimit++;
					adjust_segment_limit(SegmentLimit);
				}
				break;
			}
		}

		if ( Controls.fire_primary_down_count )	{
			// Reset orientation
			ViewDist = ZOOM_DEFAULT;
			tangles.p = PITCH_DEFAULT;
			tangles.h  = 0;
			tangles.b  = 0;
			view_target = Objects[Players[Player_num].objnum].pos;
		}

		ViewDist -= Controls.forward_thrust_time*ZOOM_SPEED_FACTOR;

		tangles.p += fixdiv( Controls.pitch_time, ROT_SPEED_DIVISOR );
		tangles.h  += fixdiv( Controls.heading_time, ROT_SPEED_DIVISOR );
		tangles.b  += fixdiv( Controls.bank_time, ROT_SPEED_DIVISOR*2 );
		
		if ( Controls.vertical_thrust_time || Controls.sideways_thrust_time )	{
			vms_angvec	tangles1;
			vms_vector	old_vt;
			old_vt = view_target;
			tangles1 = tangles;
			vm_angles_2_matrix(&tempm,&tangles1);
			vm_matrix_x_matrix(&ViewMatrix,&Objects[Players[Player_num].objnum].orient,&tempm);
			vm_vec_scale_add2( &view_target, &ViewMatrix.uvec, Controls.vertical_thrust_time*SLIDE_SPEED );
			vm_vec_scale_add2( &view_target, &ViewMatrix.rvec, Controls.sideways_thrust_time*SLIDE_SPEED );
			if ( vm_vec_dist_quick( &view_target, &Objects[Players[Player_num].objnum].pos) > i2f(1000) )	{
				view_target = old_vt;
			}
		} 

		vm_angles_2_matrix(&tempm,&tangles);
		vm_matrix_x_matrix(&ViewMatrix,&Objects[Players[Player_num].objnum].orient,&tempm);

		if ( ViewDist < ZOOM_MIN_VALUE ) ViewDist = ZOOM_MIN_VALUE;
		if ( ViewDist > ZOOM_MAX_VALUE ) ViewDist = ZOOM_MAX_VALUE;

		draw_automap();

		if ( first_time )	{
			first_time = 0;
			gr_palette_load( gr_palette );
		}

		t2 = timer_get_fixed_seconds();
		if (pause_game)
			FrameTime=t2-t1;
		t1 = t2;
	}

	//free(Edges);
	//free(DrawingListBright);
	gr_free_canvas(name_canv);  name_canv=NULL;

	mprintf( (0, "Automap memory freed\n" ));

	game_flush_inputs();

	if (pause_game)
		start_time();
}

void adjust_segment_limit(int SegmentLimit)
{
	int i,e1;
	Edge_info * e;

	mprintf(( 0, "Seglimit: %d\n", SegmentLimit ));
	
	for (i=0; i<=Highest_edge_index; i++ )	{
		e = &Edges[i];
		e->flags |= EF_TOO_FAR;
		for (e1=0; e1<e->num_faces; e1++ )	{
			if ( Automap_visited[e->segnum[e1]] <= SegmentLimit )	{
				e->flags &= (~EF_TOO_FAR);
				break;
			}
		}
	}
	
}

void draw_all_edges()	
{
	g3s_codes cc;
	int i,j,nbright;
	ubyte nfacing,nnfacing;
	Edge_info *e;
	vms_vector *tv1;
	fix distance;
	fix min_distance = 0x7fffffff;
	g3s_point *p1, *p2;
	
	
	nbright=0;

	for (i=0; i<=Highest_edge_index; i++ )	{
		//e = &Edges[Edge_used_list[i]];
		e = &Edges[i];
		if (!(e->flags & EF_USED)) continue;

		if ( e->flags & EF_TOO_FAR) continue;

		if (e->flags&EF_FRONTIER)	{						// A line that is between what we have seen and what we haven't
			if ( (!(e->flags&EF_SECRET))&&(e->color==WALL_NORMAL_COLOR))
				continue;		// If a line isn't secret and is normal color, then don't draw it
		}

		cc=rotate_list(2,e->verts);
		distance = Segment_points[e->verts[1]].z;

		if (min_distance>distance )
			min_distance = distance;

		if (!cc.and) 	{	//all off screen?
			nfacing = nnfacing = 0;
			tv1 = &Vertices[e->verts[0]];
			j = 0;
			while( j<e->num_faces && (nfacing==0 || nnfacing==0) )	{
				#ifdef COMPACT_SEGS
				vms_vector temp_v;
				get_side_normal(&Segments[e->segnum[j]], e->sides[j], 0, &temp_v );
				if (!g3_check_normal_facing( tv1, &temp_v ) )
				#else
				if (!g3_check_normal_facing( tv1, &Segments[e->segnum[j]].sides[e->sides[j]].normals[0] ) )
				#endif
					nfacing++;
				else
					nnfacing++;
				j++;
			}

			if ( nfacing && nnfacing )	{
				// a contour line
				DrawingListBright[nbright++] = e-Edges;
			} else if ( e->flags&(EF_DEFINING|EF_GRATE) )	{
				if ( nfacing == 0 )	{
					if ( e->flags & EF_NO_FADE )
						gr_setcolor( e->color );
					else
						gr_setcolor( gr_fade_table[e->color+256*8] );
					g3_draw_line( &Segment_points[e->verts[0]], &Segment_points[e->verts[1]] );
				} 	else {
					DrawingListBright[nbright++] = e-Edges;
				}
			}
		}
	}
		
///	mprintf( (0, "Min distance=%.2f, ViewDist=%.2f, Delta=%.2f\n", f2fl(min_distance), f2fl(ViewDist), f2fl(min_distance)- f2fl(ViewDist) ));

	if ( min_distance < 0 ) min_distance = 0;

	// Sort the bright ones using a shell sort
	{
		int t;
		int i, j, incr, v1, v2;
	
		incr = nbright / 2;
		while( incr > 0 )	{
			for (i=incr; i<nbright; i++ )	{
				j = i - incr;
				while (j>=0 )	{
					// compare element j and j+incr
					v1 = Edges[DrawingListBright[j]].verts[0];
					v2 = Edges[DrawingListBright[j+incr]].verts[0];

					if (Segment_points[v1].z < Segment_points[v2].z) {
						// If not in correct order, them swap 'em
						t=DrawingListBright[j+incr];
						DrawingListBright[j+incr]=DrawingListBright[j];
						DrawingListBright[j]=t;
						j -= incr;
					}
					else
						break;
				}
			}
			incr = incr / 2;
		}
	}
					
	// Draw the bright ones
	for (i=0; i<nbright; i++ )	{
		int color;
		fix dist;
		e = &Edges[DrawingListBright[i]];
		p1 = &Segment_points[e->verts[0]];
		p2 = &Segment_points[e->verts[1]];
		dist = p1->z - min_distance;
		// Make distance be 1.0 to 0.0, where 0.0 is 10 segments away;
		if ( dist < 0 ) dist=0;
		if ( dist >= Automap_farthest_dist ) continue;

		if ( e->flags & EF_NO_FADE )	{
			gr_setcolor( e->color );
		} else {
			dist = F1_0 - fixdiv( dist, Automap_farthest_dist );
			color = f2i( dist*31 );
			gr_setcolor( gr_fade_table[e->color+color*256] );	
		}
		g3_draw_line( p1, p2 );
	}

}


//==================================================================
//
// All routines below here are used to build the Edge list
//
//==================================================================


//finds edge, filling in edge_ptr. if found old edge, returns index, else return -1
static int automap_find_edge(int v0,int v1,Edge_info **edge_ptr)
{
	long vv;
	short hash,oldhash;
	int ret;

	vv = (v1<<16) + v0;

	oldhash = hash = ((v0*5+v1) % Max_edges);

	ret = -1;

	while (ret==-1) {
		if (Edges[hash].num_faces == 0 ) ret=0;
		else if (Edges[hash].vv == vv) ret=1;
		else {
			if (++hash==Max_edges) hash=0;
			if (hash==oldhash) Error("Edge list full!");
		}
	}

	*edge_ptr = &Edges[hash];

	if (ret == 0)
		return -1;
	else
		return hash;

}


void add_one_edge( short va, short vb, ubyte color, ubyte side, short segnum, int hidden, int grate, int no_fade )	{
	int found;
	Edge_info *e;
	short tmp;

	if ( Num_edges >= Max_edges)	{
		// GET JOHN! (And tell him that his 
		// MAX_EDGES_FROM_VERTS formula is hosed.)
		// If he's not around, save the mine, 
		// and send him  mail so he can look 
		// at the mine later. Don't modify it.
		// This is important if this happens.
		Int3();		// LOOK ABOVE!!!!!!
		return;
	}

	if ( va > vb )	{
		tmp = va;
		va = vb;
		vb = tmp;
	}

	found = automap_find_edge(va,vb,&e);
		
	if (found == -1) {
		e->verts[0] = va;
		e->verts[1] = vb;
		e->color = color;
		e->num_faces = 1;
		e->flags = EF_USED | EF_DEFINING;			// Assume a normal line
		e->sides[0] = side;
		e->segnum[0] = segnum;
		//Edge_used_list[Num_edges] = e-Edges;
		if ( (e-Edges) > Highest_edge_index )
			Highest_edge_index = e - Edges;
		Num_edges++;
	} else {
		//Assert(e->num_faces < 8 );

		if ( color != WALL_NORMAL_COLOR )
			e->color = color;
		if ( e->num_faces < 4 ) {
			e->sides[e->num_faces] = side;					
			e->segnum[e->num_faces] = segnum;
			e->num_faces++;
		}
	}

	if ( grate )
		e->flags |= EF_GRATE;

	if ( hidden )
		e->flags|=EF_SECRET;		// Mark this as a hidden edge
	if ( no_fade )
		e->flags |= EF_NO_FADE;
}

void add_one_unknown_edge( short va, short vb )	{
	int found;
	Edge_info *e;
	short tmp;

	if ( va > vb )	{
		tmp = va;
		va = vb;
		vb = tmp;
	}

	found = automap_find_edge(va,vb,&e);
	if (found != -1) 	
		e->flags|=EF_FRONTIER;		// Mark as a border edge
}

extern obj_position Player_init[];

void add_segment_edges(segment *seg)
{
	int 	is_grate, no_fade;
	ubyte	color;
	int	sn;
	int	segnum = seg-Segments;
	int	hidden_flag;
	
	for (sn=0;sn<MAX_SIDES_PER_SEGMENT;sn++) {
		short	vertex_list[4];

		hidden_flag = 0;

		is_grate = 0;
		no_fade = 0;

		color = 255;
		if (seg->children[sn] == -1) {
			color = WALL_NORMAL_COLOR;
		}

		switch( seg->special )	{
		case SEGMENT_IS_FUELCEN:
			color = BM_XRGB( 29, 27, 13 );
			break;
		case SEGMENT_IS_CONTROLCEN:
			color = BM_XRGB( 29, 0, 0 );
			break;
		case SEGMENT_IS_ROBOTMAKER:
			color = BM_XRGB( 29, 0, 31 );
			break;
		}

		if (seg->sides[sn].wall_num > -1)	{

			switch( Walls[seg->sides[sn].wall_num].type )	{
			case WALL_DOOR:
				if (Walls[seg->sides[sn].wall_num].keys == KEY_BLUE) {
					no_fade = 1;
					color = WALL_DOOR_BLUE;
					//mprintf((0, "Seg %i, side %i has BLUE wall\n", segnum, sn));
				} else if (Walls[seg->sides[sn].wall_num].keys == KEY_GOLD) {
					no_fade = 1;
					color = WALL_DOOR_GOLD;
					//mprintf((0, "Seg %i, side %i has GOLD wall\n", segnum, sn));
				} else if (Walls[seg->sides[sn].wall_num].keys == KEY_RED) {
					no_fade = 1;
					color = WALL_DOOR_RED;
					//mprintf((0, "Seg %i, side %i has RED wall\n", segnum, sn));
				} else if (!(WallAnims[Walls[seg->sides[sn].wall_num].clip_num].flags & WCF_HIDDEN)) {
					int	connected_seg = seg->children[sn];
					if (connected_seg != -1) {
						int connected_side = find_connect_side(seg, &Segments[connected_seg]);
						int	keytype = Walls[Segments[connected_seg].sides[connected_side].wall_num].keys;
						if ((keytype != KEY_BLUE) && (keytype != KEY_GOLD) && (keytype != KEY_RED))
							color = WALL_DOOR_COLOR;
						else {
							switch (Walls[Segments[connected_seg].sides[connected_side].wall_num].keys) {
								case KEY_BLUE:	color = WALL_DOOR_BLUE;	no_fade = 1; break;
								case KEY_GOLD:	color = WALL_DOOR_GOLD;	no_fade = 1; break;
								case KEY_RED:	color = WALL_DOOR_RED;	no_fade = 1; break;
								default:	Error("Inconsistent data.  Supposed to be a colored wall, but not blue, gold or red.\n");
							}
							//mprintf((0, "Seg %i, side %i has a colored door on the other side.\n", segnum, sn));
						}
					}
				} else {
					color = WALL_NORMAL_COLOR;
					hidden_flag = 1;
					//mprintf((0, "Wall at seg:side %i:%i is hidden.\n", seg-Segments, sn));
				}
				break;
			case WALL_CLOSED:
				// Make grates draw properly
				color = WALL_NORMAL_COLOR;
				is_grate = 1;
				break;
			case WALL_BLASTABLE:
				// Hostage doors
				color = WALL_DOOR_COLOR;	
				break;
			}
		}
	
		if (segnum==Player_init[Player_num].segnum)
			color = BM_XRGB(31,0,31);

		if ( color != 255 )	{
			// If they have a map powerup, draw unvisited areas in dark blue.
			if (Players[Player_num].flags & PLAYER_FLAGS_MAP_ALL && (!Automap_visited[segnum]))	
				color = BM_XRGB( 0, 0, 25 );

			get_side_verts(vertex_list,segnum,sn);
			add_one_edge( vertex_list[0], vertex_list[1], color, sn, segnum, hidden_flag, 0, no_fade );
			add_one_edge( vertex_list[1], vertex_list[2], color, sn, segnum, hidden_flag, 0, no_fade );
			add_one_edge( vertex_list[2], vertex_list[3], color, sn, segnum, hidden_flag, 0, no_fade );
			add_one_edge( vertex_list[3], vertex_list[0], color, sn, segnum, hidden_flag, 0, no_fade );

			if ( is_grate )	{
				add_one_edge( vertex_list[0], vertex_list[2], color, sn, segnum, hidden_flag, 1, no_fade );
				add_one_edge( vertex_list[1], vertex_list[3], color, sn, segnum, hidden_flag, 1, no_fade );
			}
		}
	}

}


// Adds all the edges from a segment we haven't visited yet.

void add_unknown_segment_edges(segment *seg)
{
	int sn;
	int segnum = seg-Segments;
	
	for (sn=0;sn<MAX_SIDES_PER_SEGMENT;sn++) {
		short	vertex_list[4];

		// Only add edges that have no children
		if (seg->children[sn] == -1) {
			get_side_verts(vertex_list,segnum,sn);
	
			add_one_unknown_edge( vertex_list[0], vertex_list[1] );
			add_one_unknown_edge( vertex_list[1], vertex_list[2] );
			add_one_unknown_edge( vertex_list[2], vertex_list[3] );
			add_one_unknown_edge( vertex_list[3], vertex_list[0] );
		}


	}

}

void automap_build_edge_list()
{	
	int	i,e1,e2,s;
	Edge_info * e;

	Automap_cheat = 0;

	if ( Players[Player_num].flags & PLAYER_FLAGS_MAP_ALL_CHEAT )
		Automap_cheat = 1;		// Damn cheaters...

	// clear edge list
	for (i=0; i<Max_edges; i++) {
		Edges[i].num_faces = 0;
		Edges[i].flags = 0;
	}
	Num_edges = 0;
	Highest_edge_index = -1;

	if (Automap_cheat || (Players[Player_num].flags & PLAYER_FLAGS_MAP_ALL) )	{
		// Cheating, add all edges as visited
		for (s=0; s<=Highest_segment_index; s++)
			#ifdef EDITOR
			if (Segments[s].segnum != -1)
			#endif
			{
				add_segment_edges(&Segments[s]);
			}
	} else {
		// Not cheating, add visited edges, and then unvisited edges
		for (s=0; s<=Highest_segment_index; s++)
			#ifdef EDITOR
			if (Segments[s].segnum != -1)
			#endif
				if (Automap_visited[s]) {
					add_segment_edges(&Segments[s]);
				}
	
		for (s=0; s<=Highest_segment_index; s++)
			#ifdef EDITOR
			if (Segments[s].segnum != -1)
			#endif
				if (!Automap_visited[s]) {
					add_unknown_segment_edges(&Segments[s]);
				}
	}

	// Find unnecessary lines (These are lines that don't have to be drawn because they have small curvature)
	for (i=0; i<=Highest_edge_index; i++ )	{
		e = &Edges[i];
		if (!(e->flags&EF_USED)) continue;

		for (e1=0; e1<e->num_faces; e1++ )	{
			for (e2=1; e2<e->num_faces; e2++ )	{
				if ( (e1 != e2) && (e->segnum[e1] != e->segnum[e2]) )	{
					#ifdef COMPACT_SEGS
					vms_vector v1, v2;
					get_side_normal(&Segments[e->segnum[e1]], e->sides[e1], 0, &v1 );
					get_side_normal(&Segments[e->segnum[e2]], e->sides[e2], 0, &v2 );
					if ( vm_vec_dot(&v1,&v2) > (F1_0-(F1_0/10))  )	{
					#else
					if ( vm_vec_dot( &Segments[e->segnum[e1]].sides[e->sides[e1]].normals[0], &Segments[e->segnum[e2]].sides[e->sides[e2]].normals[0] ) > (F1_0-(F1_0/10))  )	{
					#endif
						e->flags &= (~EF_DEFINING);
						break;
					}
				}
			}
			if (!(e->flags & EF_DEFINING))
				break;
		}
	}	

	mprintf( (0, "Automap used %d / %d edges\n", Num_edges, Max_edges  ));

}

