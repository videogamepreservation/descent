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
 * $Source: f:/miner/source/main/rcs/radar.c $
 * $Revision: 1.10 $
 * $Author: john $
 * $Date: 1995/02/27 12:31:15 $
 * 
 * Routines for drawing the radar.
 * . 
 * 
 * $Log: radar.c $
 * Revision 1.10  1995/02/27  12:31:15  john
 * Version 2.0.
 * 
 * Revision 1.9  1995/02/01  21:03:36  john
 * Lintified.
 * 
 * Revision 1.8  1994/08/12  22:41:28  john
 * Took away Player_stats; add Players array.
 * 
 * Revision 1.7  1994/07/15  09:38:00  john
 * Moved in radar_farthest_dist.
 * 
 * Revision 1.6  1994/07/14  22:05:57  john
 * Made radar display not conflict with hostage
 * vclip talking.
 * 
 * Revision 1.5  1994/07/12  18:41:51  yuan
 * Tweaked location of radar and hostage screen... 
 * Still needs work.
 * 
 * 
 * Revision 1.4  1994/07/07  14:59:00  john
 * Made radar powerups.
 * 
 * 
 * Revision 1.3  1994/07/07  10:05:36  john
 * Pegged objects in radar to edges.
 * 
 * Revision 1.2  1994/07/06  19:36:33  john
 * Initial version of radar.
 * 
 * Revision 1.1  1994/07/06  17:22:07  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: radar.c 1.10 1995/02/27 12:31:15 john Exp $";
#pragma on (unreferenced)


#include <stdlib.h>

#include "error.h"
#include "3d.h"
#include "inferno.h"
#include "object.h"
#include "vclip.h"
#include "game.h"
#include "mono.h"
#include "polyobj.h"
#include "sounds.h"
#include "player.h"
#include "bm.h"
#include "hostage.h"
#include "multi.h"
#include "network.h"
#include "gauges.h"

static short 		Hostage_monitor_x = 160-(55/2);			// X location of monitor where hostage face appears
static short 		Hostage_monitor_y = 100-50; 			// Y
static short 		Hostage_monitor_w = 55;				// Width of monitor
static short 		Hostage_monitor_h = 41;				// Height of monitor

static fix radx, rady, rox, roy, cenx, ceny;

typedef struct blip	{
	short x, y;
	ubyte c;
} blip;

#define MAX_BLIPS 1000

blip Blips[MAX_BLIPS];
int N_blips = 0;

fix Radar_farthest_dist = (F1_0 * 20 * 15);		// 15 segments away

void radar_plot_object( object * objp, int hue )	
{
	ubyte flags;
	g3s_point pos;
	int color;
	fix dist, rscale, zdist, distance;
	int xpos, ypos;

	flags = g3_rotate_point(&pos,&objp->pos);
	dist = vm_vec_mag_quick(&pos.p3_vec);

	// Make distance be 1.0 to 0.0, where 0.0 is 10 segments away;
	if ( dist >= Radar_farthest_dist ) return;
	distance = F1_0 - fixdiv( dist, Radar_farthest_dist );
	color = f2i( distance*31 );

	zdist = fix_sqrt( fixmul(pos.x,pos.x)+fixmul(pos.y,pos.y) );
	if (zdist < 100 ) return;		// Watch for divide overflow

	rscale = fix_acos( fixdiv(pos.z,dist) )/2;	 

	xpos = f2i(fixmul( rox+fixmul(fixdiv(pos.x,zdist),rscale), radx));
	ypos = f2i(fixmul( roy-fixmul(fixdiv(pos.y,zdist),rscale), rady));

	if ( xpos < Hostage_monitor_x ) xpos = Hostage_monitor_x;
	if ( ypos < Hostage_monitor_y ) ypos = Hostage_monitor_y;
	if ( xpos > Hostage_monitor_x+Hostage_monitor_w-1 ) xpos = Hostage_monitor_x+Hostage_monitor_w-1;
	if ( ypos > Hostage_monitor_y+Hostage_monitor_h-1 ) ypos = Hostage_monitor_y+Hostage_monitor_h-1;

	Blips[N_blips].c = gr_fade_table[hue+color*256];
	Blips[N_blips].x = xpos; Blips[N_blips].y = ypos;
	N_blips++;
}

void radar_render_frame()	
{
	int i,color;
	object * objp;

	if (hostage_is_vclip_playing())
		return;

	gr_set_current_canvas(NULL);

	gr_setcolor( BM_XRGB( 0, 31, 0 ) );
	
	gr_ucircle( i2f(Hostage_monitor_x+Hostage_monitor_w/2), i2f(Hostage_monitor_y+Hostage_monitor_h/2),	i2f(Hostage_monitor_w)/2);

	// Erase old blips
	for (i=0; i<N_blips; i++ )	{
		gr_setcolor(gr_gpixel( cockpit_bitmap[0], Blips[i].x, Blips[i].y ));
		gr_upixel( Blips[i].x, Blips[i].y );
	}
	N_blips = 0;

//	if ( !(Players[Player_num].flags & (PLAYER_FLAGS_RADAR_ENEMIES | PLAYER_FLAGS_RADAR_POWERUPS  )) ) return;

	radx = i2f(Hostage_monitor_w*4)/2;
	rady = i2f(Hostage_monitor_h*4)/2;
	cenx = i2f(Hostage_monitor_x)+i2f(Hostage_monitor_w)/2;
	ceny = i2f(Hostage_monitor_y)+i2f(Hostage_monitor_h)/2;

	rox = fixdiv( cenx, radx );
	roy = fixdiv( ceny, rady );

	objp = Objects;
	for (i=0;i<=Highest_object_index;i++) {
		switch( objp->type )	{
		case OBJ_PLAYER:
			if ( i != Players[Player_num].objnum )	{
				if (Game_mode & GM_TEAM)
					color = get_team(i);
				else
					color = i;
				radar_plot_object( objp, gr_getcolor(player_rgb[color].r,player_rgb[color].g,player_rgb[color].b) );
			}
			break;
//		case OBJ_HOSTAGE:
//			radar_plot_object( objp, BM_XRGB(0,31,0) );
//			break;
//		case OBJ_POWERUP:
//			//if ( Players[Player_num].flags & PLAYER_FLAGS_RADAR_POWERUPS )
//			radar_plot_object( objp, BM_XRGB(0,0,31) );
//			break;
//		case OBJ_ROBOT:
//			//if ( Players[Player_num].flags & PLAYER_FLAGS_RADAR_ENEMIES )
//			radar_plot_object( objp, BM_XRGB(31,0,0) );
//			break;
		default:
			break;
		}
		objp++;
	}

	// Draw new blips...
	for (i=0; i<N_blips; i++ )	{
		gr_setcolor( Blips[i].c );
		gr_upixel( Blips[i].x, Blips[i].y );
	}
}

