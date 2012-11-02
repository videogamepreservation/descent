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
 * $Source: f:/miner/source/main/rcs/objfly.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:31:00 $
 * 
 * Code for making objects do a flythrough.
 * 
 * $Log: objfly.c $
 * Revision 2.0  1995/02/27  11:31:00  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.4  1994/11/17  14:58:11  mike
 * moved segment validation functions from editor to main.
 * 
 * Revision 1.3  1994/02/17  11:32:36  matt
 * Changes in object system
 * 
 * Revision 1.2  1994/01/27  18:46:28  john
 * Initial version
 * 
 * Revision 1.1  1994/01/27  15:11:07  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: objfly.c 2.0 1995/02/27 11:31:00 john Exp $";
#pragma on (unreferenced)

#include <stdlib.h>

#include "inferno.h"
#include "game.h"
#include "editor\editor.h"
#include "error.h"

#define MIN_D 0x100

// In flyby.c
extern int matt_find_connect_side(int seg0,int seg1);
extern vms_angvec *angvec_add2_scale(vms_angvec *dest,vms_angvec *src,fix s);
extern angles_from_vector(vms_angvec *dest,vms_vector *vec);
extern fixang delta_ang(fixang a,fixang b);

// In physics.c
int check_object_seg(object * obj );


void objfly_move_to_new_segment( object * obj, short newseg, int first_time )
{
	segment *pseg;
	int old_object_seg = obj->segnum;

	if ( newseg != obj->segnum )
		obj_relink(obj-Objects, newseg );

	pseg = &Segments[obj->segnum];

	if ( first_time || obj->segnum != old_object_seg) {		//moved into new seg
		vms_vector curcenter,nextcenter;
		fix step_size,seg_time;
		short entry_side,exit_side;	//what sides we entry and leave through
		vms_vector dest_point;		//where we are heading (center of exit_side)
		vms_angvec dest_angles;		//where we want to be pointing

		//find new exit side

		if ( !first_time ) {

			entry_side = matt_find_connect_side(obj->segnum,old_object_seg);
			exit_side = Side_opposite[entry_side];
		}

		//if (first_time) obj->fly_info.ft_mode = FP_FORWARD;

		if (first_time || entry_side==-1 || (pseg->children[exit_side]==-1) || (obj->fly_info.ft_mode!=FP_FORWARD) ) {
			int i;
			vms_vector prefvec,segcenter,sidevec;
			fix best_val=-f2_0;
			int best_side;

			//find exit side

			if (obj->fly_info.ft_mode == FP_FORWARD) {
				if (first_time) 
					prefvec = obj->orient.fvec;
				else
					prefvec = obj->fly_info.heading;
				vm_vec_normalize(&prefvec);
			}
			else
				prefvec = obj->orient.vecs[obj->fly_info.ft_mode%3];

			if (obj->fly_info.ft_mode >= 3) {prefvec.x = -prefvec.x; prefvec.y = -prefvec.y; prefvec.z = -prefvec.z;}

			compute_segment_center(&segcenter,pseg);

			best_side=-1;
			for (i=MAX_SIDES_PER_SEGMENT;--i >= 0;) {
				fix d;

				if (pseg->children[i]!=-1) {

					compute_center_point_on_side(&sidevec,pseg,i);
					vm_vec_sub2(&sidevec,&segcenter);
					vm_vec_normalize(&sidevec);
					d = vm_vec_dotprod(&sidevec,&prefvec);

					if (labs(d) < MIN_D) d=0;

					if (d > best_val || (d==best_val && i==exit_side)) {best_val=d; best_side=i;}

				}
			}

			if (best_val > 0) obj->fly_info.ft_mode = FP_FORWARD;

			Assert(best_side!=-1);

			exit_side = best_side;
		}

		//update target point & angles

		compute_center_point_on_side(&dest_point,pseg,exit_side);

		//update target point and movement points

		vm_vec_sub(&obj->phys_info.velocity,&dest_point,&obj->pos);
		step_size = vm_vec_normalize(&obj->phys_info.velocity);
		vm_vec_scale(&obj->phys_info.velocity, obj->phys_info.speed);

		compute_segment_center(&curcenter,pseg);
		compute_segment_center(&nextcenter,&Segments[pseg->children[exit_side]]);
		vm_vec_sub(&obj->fly_info.heading,&nextcenter,&curcenter);

		angles_from_vector(&dest_angles,&obj->fly_info.heading);	//extract angles

		if (first_time) 
			angles_from_vector(&obj->phys_info.rotvel,&obj->orient.fvec);

		seg_time = fixdiv(step_size,obj->phys_info.speed);	//how long through seg

		if (seg_time) {
			obj->fly_info.angle_step.p = fixdiv(delta_ang(obj->phys_info.rotvel.p,dest_angles.p),seg_time);
			obj->fly_info.angle_step.b = fixdiv(delta_ang(obj->phys_info.rotvel.b,dest_angles.b),seg_time);
			obj->fly_info.angle_step.h = fixdiv(delta_ang(obj->phys_info.rotvel.h,dest_angles.h),seg_time);
		}
		else {
			obj->phys_info.rotvel = dest_angles;
			obj->fly_info.angle_step.p = obj->fly_info.angle_step.b = obj->fly_info.angle_step.h = 0;
		}
	}
}

void do_object_flythrough(object * obj )		//set true if init
{
	if ( obj->fly_info.ft_mode == FP_FIRST_TIME )	{
	 	obj->fly_info.ft_mode = FP_FORWARD;
		objfly_move_to_new_segment( obj, obj->segnum, 1 );
	} else {
		//move the object for this frame
		angvec_add2_scale(&obj->phys_info.rotvel,&obj->fly_info.angle_step,FrameTime);
		vm_angles_2_matrix(&obj->orient,&obj->phys_info.rotvel);
	}
}
