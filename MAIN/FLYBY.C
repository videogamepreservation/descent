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
 * $Source: f:/miner/source/main/rcs/flyby.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:33:03 $
 * 
 * Routines to fly though a mine (to show it off)
 * 
 * $Log: flyby.c $
 * Revision 2.0  1995/02/27  11:33:03  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.20  1994/11/19  15:17:49  mike
 * rip out unused code and data
 * 
 * Revision 1.19  1994/11/17  14:58:40  mike
 * move segment validation functions from editor to main.
 * 
 * Revision 1.18  1994/08/19  20:09:39  matt
 * Added end-of-level cut scene with external scene
 * 
 * Revision 1.17  1994/05/20  11:56:44  matt
 * Cleaned up find_vector_intersection() interface
 * Killed check_point_in_seg(), check_player_seg(), check_object_seg()
 * 
 * Revision 1.16  1994/05/19  09:51:06  matt
 * Use new function vm_vec_normalized_dir()
 * 
 * Revision 1.15  1994/05/18  22:29:25  matt
 * Use new function vm_extract_angles_vector()
 * 
 * Revision 1.14  1994/05/12  10:26:33  matt
 * Took out references to unused structure fields.
 * 
 * Revision 1.13  1994/03/30  16:44:13  matt
 * Use great new vm_vec_scale_add2() function
 * 
 * Revision 1.12  1994/02/17  11:32:37  matt
 * Changes in object system
 * 
 * Revision 1.11  1994/01/27  18:18:10  john
 * Added new module, objfly, and made robots
 * fly using this code.
 * 
 * Revision 1.10  1994/01/05  14:26:01  john
 * Made Slew_object be able to slew, fly, and auto-fly.
 * 
 * Revision 1.9  1994/01/05  10:53:52  john
 * New object code by John.  
 * 
 * Revision 1.8  1993/12/21  14:30:32  matt
 * Explicitely set some variables in init, so flyby is the same each time.
 * 
 * Revision 1.7  1993/12/01  11:44:15  matt
 * Chagned Frfract to FrameTime
 * 
 * Revision 1.6  1993/11/04  15:47:29  matt
 * Added check to hopefully prevent divide overflow
 * 
 * Revision 1.5  1993/10/29  22:54:38  matt
 * Made changes for new matrix ordering
 * 
 * Revision 1.4  1993/10/26  11:23:25  matt
 * Doesn't get stuck now in some cases with very small segments
 * 
 * Revision 1.3  1993/10/25  14:34:42  matt
 * Added direction choice keys to flythrough
 * 
 * Revision 1.2  1993/10/14  18:04:51  mike
 * Change CONNECTIVITY to MAX_SIDES_PER_SEGMENT
 * 
 * Revision 1.1  1993/10/14  11:03:47  matt
 * Initial revision
 * 
 * 
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: flyby.c 2.0 1995/02/27 11:33:03 john Exp $";
#pragma on (unreferenced)


#include <stdlib.h>

#include "inferno.h"
#include "game.h"
#include "editor\editor.h"
#include "error.h"


//variables for fly-through system

static vms_angvec player_angles;	//player's orientation in angles

vms_vector player_step;		//how far in a second
vms_angvec player_angstep;	//rotation per second

fix player_speed=0x100000;	//how fast the player is moving (per second)

//which way the user wants to turn
int ft_preference = FP_FORWARD;

vms_vector headvec;			//where we want to be pointing

vms_angvec *angvec_add2_scale(vms_angvec *dest,vms_angvec *src,fix s)
{
	dest->pitch += fixmul(src->pitch,s);
	dest->bank  += fixmul(src->bank,s);
	dest->head  += fixmul(src->head,s);

	return dest;
}

//return though which side of seg0 is seg1
int matt_find_connect_side(int seg0,int seg1)
{
	segment *Seg=&Segments[seg0];
	int i;

	for (i=MAX_SIDES_PER_SEGMENT;i--;) if (Seg->children[i]==seg1) return i;

	return -1;
}

//#include <math.h>
//
//#define PI 3.14159
//
////extract 3 angles from forward vector, assuming bank==0
//angles_from_vector(vms_angvec *dest,vms_vector *vec)
//{
//	vms_vector tempv;
//	double x,y,z;
//	double p,b,h;
//
//	tempv = *vec;
//	vm_vec_normalize(&tempv);
//
//	x = f2fl(tempv.x); y = f2fl(tempv.y); z = f2fl(tempv.z);
//
//	b = 0;
//	p = asin(-y);
//
//	if (x==0 && z==0) h=0;
//	else
//		h = atan2(x,z);
//
//	dest->p = fl2f(p/(PI*2));
//	dest->b = fl2f(b/(PI*2));
//	dest->h = fl2f(h/(PI*2));
//}

void compute_segment_center(vms_vector *vp,segment *sp);
void compute_center_point_on_side(vms_vector *vp,segment *sp,int side);

//find delta between two angles
fixang delta_ang(fixang a,fixang b)
{
	fixang delta0,delta1;

	return (abs(delta0 = a - b) < abs(delta1 = b - a)) ? delta0 : delta1;

}

//--unused-- fixang interp_angle(fixang dest,fixang src,fixang step)
//--unused-- {
//--unused-- 	fixang delta;
//--unused-- 
//--unused-- 	delta = delta_ang(dest,src);
//--unused-- 
//--unused-- 	if (delta > step) delta = step;
//--unused-- 	else if (delta < -step) delta = -step;
//--unused-- 
//--unused-- 	return dest + delta;
//--unused-- 
//--unused-- }

#define MIN_D 0x100

do_flythrough(object *obj,int first_time)		//set true if init
{
	segment *pseg;
	int old_player_seg = obj->segnum;

	if (first_time) {
		//vms_vector zero_vector = {0,0,0};

		obj->control_type = CT_FLYTHROUGH;

		//obj->fly_info.angle_step.p = 0;
		//obj->fly_info.angle_step.b = 0;
		//obj->fly_info.angle_step.h = 0;
		//obj->fly_info.heading = zero_vector;

	}
	
	//move the player for this frame

	if (!first_time) {
		//vms_vector tempv;
		//fix rot_step;

		vm_vec_scale_add2(&obj->pos,&player_step,FrameTime);
		angvec_add2_scale(&player_angles,&player_angstep,FrameTime);

		vm_angles_2_matrix(&obj->orient,&player_angles);
	}

	//check new player seg

	update_object_seg(obj);
	pseg = &Segments[obj->segnum];

	if (first_time || obj->segnum != old_player_seg) {		//moved into new seg
		vms_vector curcenter,nextcenter;
		fix step_size,seg_time;
		short entry_side,exit_side;	//what sides we entry and leave through
		vms_vector dest_point;		//where we are heading (center of exit_side)
		vms_angvec dest_angles;		//where we want to be pointing

		//find new exit side

		if (!first_time) {

			entry_side = matt_find_connect_side(obj->segnum,old_player_seg);
			exit_side = Side_opposite[entry_side];
		}

		if (first_time) ft_preference = FP_FORWARD;

		if (first_time || entry_side==-1 || pseg->children[exit_side]==-1 || ft_preference!=FP_FORWARD) {
			int i;
			vms_vector prefvec,segcenter,sidevec;
			fix best_val=-f2_0;
			int best_side;

			//find exit side

			if (ft_preference == FP_FORWARD) {
				if (first_time) prefvec = obj->orient.fvec;
				else prefvec = headvec;
				vm_vec_normalize(&prefvec);
			}
			else
				prefvec = obj->orient.vecs[ft_preference%3];

			if (ft_preference >= 3) {prefvec.x = -prefvec.x; prefvec.y = -prefvec.y; prefvec.z = -prefvec.z;}

			compute_segment_center(&segcenter,pseg);

			best_side=-1;
			for (i=MAX_SIDES_PER_SEGMENT;--i >= 0;) {
				fix d;

				if (pseg->children[i]!=-1) {

					compute_center_point_on_side(&sidevec,pseg,i);
					//vm_vec_sub2(&sidevec,&segcenter);
					//vm_vec_normalize(&sidevec);
					vm_vec_normalized_dir(&sidevec,&sidevec,&segcenter);
					d = vm_vec_dotprod(&sidevec,&prefvec);

					if (labs(d) < MIN_D) d=0;

					if (d > best_val || (d==best_val && i==exit_side)) {best_val=d; best_side=i;}

				}
			}

			if (best_val > 0) ft_preference = FP_FORWARD;

			Assert(best_side!=-1);

			exit_side = best_side;
		}

		//update target point & angles

		compute_center_point_on_side(&dest_point,pseg,exit_side);

		//update target point and movement points

		vm_vec_sub(&player_step,&dest_point,&obj->pos);
		step_size = vm_vec_normalize(&player_step);
		vm_vec_scale(&player_step,player_speed);

		compute_segment_center(&curcenter,pseg);
		compute_segment_center(&nextcenter,&Segments[pseg->children[exit_side]]);
		vm_vec_sub(&headvec,&nextcenter,&curcenter);

		//angles_from_vector(&dest_angles,&headvec);	//extract angles
		vm_extract_angles_vector(&dest_angles,&headvec);	//extract angles

		if (first_time)
			//angles_from_vector(&player_angles,&obj->orient.fvec);
			vm_extract_angles_vector(&player_angles,&obj->orient.fvec);

		seg_time = fixdiv(step_size,player_speed);	//how long through seg

		if (seg_time) {
			player_angstep.p = fixdiv(delta_ang(player_angles.p,dest_angles.p),seg_time);
			player_angstep.b = fixdiv(delta_ang(player_angles.b,dest_angles.b),seg_time);
			player_angstep.h = fixdiv(delta_ang(player_angles.h,dest_angles.h),seg_time);
		}
		else {
			player_angles = dest_angles;
			player_angstep.p = player_angstep.b = player_angstep.h = 0;
		}
	}
}

