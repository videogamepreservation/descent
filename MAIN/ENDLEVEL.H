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
 * $Source: f:/miner/source/main/rcs/endlevel.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:31:37 $
 * 
 * Header for newfile.c
 * 
 * $Log: endlevel.h $
 * Revision 2.0  1995/02/27  11:31:37  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.5  1994/12/06  13:24:55  matt
 * Made exit model come out of bitmaps.tbl
 * 
 * Revision 1.4  1994/11/19  12:41:35  matt
 * Added system to read endlevel data from file, and to make it work
 * with any exit tunnel.
 * 
 * Revision 1.3  1994/10/30  20:09:20  matt
 * For endlevel: added big explosion at tunnel exit; made lights in tunnel 
 * go out; made more explosions on walls.
 * 
 * Revision 1.2  1994/08/19  20:09:38  matt
 * Added end-of-level cut scene with external scene
 * 
 * Revision 1.1  1994/08/15  19:18:47  matt
 * Initial revision
 * 
 * 
 */



#ifndef _OUTSIDE_H
#define _OUTSIDE_H

#include "object.h"

extern int Endlevel_sequence;

start_endlevel_sequence();
render_external_scene();
void render_endlevel_frame(fix eye_offset);
do_endlevel_frame();
draw_exit_model();
init_endlevel();
stop_endlevel_sequence();


extern vms_vector mine_exit_point;
extern int exit_segnum;
extern grs_bitmap *satellite_bitmap,*station_bitmap,*exit_bitmap,*terrain_bitmap;

extern object external_explosion;
extern int ext_expl_playing;

//called for each level to load & setup the exit sequence
load_endlevel_data(int level_num);

extern int exit_modelnum,destroyed_exit_modelnum;

#endif
