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
 * $Source: f:/miner/source/main/rcs/demo.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:30:55 $
 * 
 * Demo playback and recording.
 * 
 * $Log: demo.c $
 * Revision 2.0  1995/02/27  11:30:55  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.19  1994/12/15  13:04:11  mike
 * Replace Players[Player_num].time_total references with GameTime.
 * 
 * Revision 1.18  1994/07/02  13:49:49  matt
 * Cleaned up includes
 * 
 * Revision 1.17  1994/06/24  17:01:37  john
 * Add VFX support; Took Game Sequencing, like EndGame and stuff and
 * took it out of game.c and into gameseq.c
 * 
 * Revision 1.16  1994/06/20  12:02:10  john
 * Made demo only start if the -autostart switch is on ...
 * 
 * 
 * Revision 1.15  1994/05/19  18:53:21  yuan
 * Changing player structure...
 * 
 * Revision 1.14  1994/05/14  17:16:08  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.13  1994/02/17  11:32:33  matt
 * Changes in object system
 * 
 * Revision 1.12  1994/02/11  21:51:12  matt
 * Made auto_demo off by default
 * 
 * Revision 1.11  1994/02/10  17:45:38  yuan
 * Integrated some hacks which still need to be fixed.
 * 
 * Revision 1.10  1994/02/09  13:42:31  john
 * Fix demo playback bug with Slew_object->pos and
 * Slew_object->seg_id not being in sync by call
 * ing check_object_seg after the demo code computes
 * x,y,z and segment each frame.
 * 
 * Revision 1.9  1994/02/08  12:38:29  yuan
 * fixed demo_loaded variable, so it must be 1 (first time)
 * to run demo.
 * 
 * Revision 1.8  1994/02/07  17:25:01  yuan
 * Fixed hack, reset time.
 * 
 * Revision 1.7  1994/02/02  09:41:51  mike
 * much auto-demo stuff.
 * 
 * Revision 1.6  1994/02/01  18:04:19  yuan
 * Tweaked gauges
 * 
 * Revision 1.5  1994/02/01  16:26:51  yuan
 * Removed mprintf debugs.
 * 
 * Revision 1.4  1994/02/01  16:21:03  yuan
 * Checked in.
 * 
 * Revision 1.3  1994/02/01  15:14:53  mike
 * Self-running demo code.
 * 
 * Revision 1.2  1994/02/01  11:49:46  mike
 * Demo system.
 * 
 * Revision 1.1  1994/01/31  18:10:23  mike
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: demo.c 2.0 1995/02/27 11:30:55 john Exp $";
#pragma on (unreferenced)


#include <stdlib.h>
#include <stdio.h>

#include "inferno.h"
#include "game.h"
#include "key.h"
#include "object.h"
#include "error.h"
#include "mono.h"
#include "timer.h"
#include "render.h"
#include "screens.h"
#include "vecmat.h"
#include "demo.h"
#include "slew.h"
#include "player.h"

#include "demorecs.c"

int	Num_demo_recs=0, Demo_record_index, Demo_last_time;
fix	Demo_start_time;

int	Demo_mode, Auto_demo = 0;

demorec Demo_records[MAX_DEMO_RECS];

void print_demo_message(void)
{
//	gr_set_curfont( game_font );
//	gr_set_fontcolor(gr_getcolor(0,55,0), gr_getcolor(0,0,0));
//	gr_printf(24, 20, "AUTO DEMO    ");
//	gr_printf(24, 28, "ESC TO END   ");
}

void start_demo_playback(void)
{
//	print_demo_message();
//
//	Player_stats.time_total = 0;
//	Demo_start_time = Player_stats.time_total; // = 0;
}

//	For curtime, return location and pbh
//	Return 1 if demo playback over.
//	Curtime should be current absolute time.  Internally, Demo_start_time is subtracted.
int get_demo_data(fix curtime, vms_vector *pos, vms_angvec *pbh, short *segnum, int *do_fire)
{
	int		rec;
	demorec	*demo_ptr, *demo_ptr2;
	fix		base_x, base_y, base_z;
	fixang	base_p, base_b, base_h;

	fix		delta_x, delta_y, delta_z;
	fixang	delta_p, delta_b, delta_h;
	fix		delta_time, scale_time, extra_time;

	rec = 0;

	curtime -= Demo_start_time;

	while ((curtime > Demo_records[rec].time) && (rec < Num_demo_recs-1))
		rec++;

	if (rec > 0)
		rec--;		// we overshot

//mprintf(0,"Record = %3i ", rec);

	if (rec >= Num_demo_recs-2) {
		// mprintf(0, "DONE!\n");
		start_demo_playback();
		return 1;		// done doing demo playback
	}

	demo_ptr = &Demo_records[rec];
	demo_ptr2 = &Demo_records[rec+1];

	extra_time = curtime - demo_ptr->time;

	base_x = demo_ptr->x;
	base_y = demo_ptr->y;
	base_z = demo_ptr->z;
	base_p = demo_ptr->p;
	base_b = demo_ptr->b;
	base_h = demo_ptr->h;

	delta_x = demo_ptr2->x - base_x;
	delta_y = demo_ptr2->y - base_y;
	delta_z = demo_ptr2->z - base_z;
	delta_p = demo_ptr2->p - base_p;
	delta_b = demo_ptr2->b - base_b;
	delta_h = demo_ptr2->h - base_h;

	delta_time = demo_ptr2->time - demo_ptr->time;

	if (curtime != 0) {
		scale_time = fixdiv(extra_time, delta_time);
		base_x += fixmul(delta_x, scale_time);
		base_y += fixmul(delta_y, scale_time);
		base_z += fixmul(delta_z, scale_time);
		base_p += fixmul(delta_p, scale_time);
		base_b += fixmul(delta_b, scale_time);
		base_h += fixmul(delta_h, scale_time);
	}

//mprintf(0, "%8x %8x %8x %4x %4x %4x\n", base_x, base_y, base_z, base_p, base_b, base_h);

	pos->x = base_x;
	pos->y = base_y;
	pos->z = base_z;

	pbh->p = base_p;
	pbh->b = base_b;
	pbh->h = base_h;

	*segnum = demo_ptr->segnum;

//	if (demo_ptr->specials)
//		*do_fire = 1;
//	else
		*do_fire = 0;
//	if (demo_ptr->specials)
//		switch (Demo_frame_count %10) {
//			case 0:
//			case 1:
//			case 3:
//			case 5:
//			case 8:
//				*do_fire = 1;
//				break;
//			default:
//				*do_fire = 0;
//		}
	return 0;

}

void start_demo_recording(void)
{
//	Demo_record_index = 0;
//	Demo_start_time = Player_stats.time_total;
//	Demo_last_time = Demo_start_time - 65536;

}

void matrix_compare(vms_matrix *mat1, vms_matrix *mat2)
{
	int	totdiff;

	totdiff = 0;
	totdiff += abs(mat1->m1 - mat2->m1);
	totdiff += abs(mat1->m2 - mat2->m2);
	totdiff += abs(mat1->m3 - mat2->m3);
	totdiff += abs(mat1->m4 - mat2->m4);
	totdiff += abs(mat1->m5 - mat2->m5);
	totdiff += abs(mat1->m6 - mat2->m6);
	totdiff += abs(mat1->m7 - mat2->m7);
	totdiff += abs(mat1->m8 - mat2->m8);
	totdiff += abs(mat1->m9 - mat2->m9);

	//mprintf(0, "Matt's matrix error = %i\n", totdiff);
}

void record_demo_frame(void)
{
	vms_angvec	pbh;

//mprintf(0, "Record start...");

mprintf(0, "Curtime = %6i, Last time = %6i\n", Player_stats.time_total, Demo_last_time);

  if (GameTime - Demo_last_time >= 65536) {
	Demo_last_time = GameTime;
	if (Demo_record_index < MAX_DEMO_RECS) {
		demorec *demo_ptr = &Demo_records[Demo_record_index];
		vms_matrix tempmat;

		demo_ptr->time = GameTime - Demo_start_time;

		demo_ptr->x = Player->pos.x;
		demo_ptr->y = Player->pos.y;
		demo_ptr->z = Player->pos.z;

		vm_extract_angles_matrix(&pbh, &Player->orient);
		vm_angles_2_matrix(&tempmat, &pbh);
		matrix_compare(&tempmat, &Player->orient);
		demo_ptr->p = pbh.p;
		demo_ptr->b = pbh.b;
		demo_ptr->h = pbh.h;

		demo_ptr->segnum = Player->segnum;

		Demo_record_index++;
		Num_demo_recs = Demo_record_index;

//		if (firing)
//			demo_ptr->specials = 1;
//		else
//			demo_ptr->specials = 0;
	}
  }
//mprintf(0, "Record end\n");
}

void setup_predefined_demo(void)
{
	int	i;

	Num_demo_recs = Num_predefined_demo_recs;

	for (i=0; i<Num_predefined_demo_recs; i++)
		Demo_records[i] = Predefined_demo_records[i];

	slew_stop( Player );
	start_demo_playback();
	Demo_mode = DEMO_PLAYBACK;
	//??Player->mode = CAMERA_MODE_SLEW;

}

//	This function should be called when the game is loaded.
//	The first time it is called, if the variable Auto_demo is set, then it will run the auto-demo.
//	If the variable is not set, it just returns.
void demo_startup(void)
{
	//	Warning!  Danger!  Shameful hack!  Don't know how to tell if demo1.min is loaded, so
	//	just make sure there are a bunch of segments.
	
	// Fixed hack to check if Demo mine is loaded.

//	mprintf(0, "demo_loaded for startup %d\n", demo_loaded); 
//	if (Auto_demo && (demo_loaded==1)) {
//		setup_predefined_demo();
//	}
//	Auto_demo = 0;
}

void write_demo_as_source(void)
{
	int	i;
	FILE	*fp;

	fp = fopen( "demorecs.c", "wt" );

	fprintf(fp, "int Num_predefined_demo_recs = %i;\n", Num_demo_recs);

	fprintf(fp, "demorec Predefined_demo_records[] = {\n");
	for (i=0; i<Num_demo_recs; i++) {
		demorec *dp = &Demo_records[i];
		fprintf(fp, " { %li, %li, %li, %li, %li, %li, %li, %li },\n", dp->time, dp->x, dp->y, dp->z, dp->p, dp->b, dp->h, dp->segnum); //, dp->specials);
	}
	fprintf(fp, "};\n");

	fclose(fp);
}



