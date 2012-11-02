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

void do_object_physics( object * obj )
{
	vms_angvec rotang;
	vms_vector frame_vec;	//movement in this frame
	vms_vector new_pos,ipos;		//position after this frame
	int iseg;
	int hit;
	vms_matrix rotmat,new_pm;
	int count=0;
	short joy_x,joy_y,btns;
	int joyx_moved,joyy_moved;
	fix speed;
	vms_vector *desired_upvec;
	fixang delta_ang,roll_ang;
	vms_vector forvec = {0,0,f1_0};
	vms_matrix temp_matrix;

	//check keys

	rotang.pitch = ROT_SPEED * (key_down_time(KEY_UP) - key_down_time(KEY_DOWN));
	rotang.head  = ROT_SPEED * (key_down_time(KEY_RIGHT) - key_down_time(KEY_LEFT));
	rotang.bank = 0;

	//check for joystick movement

	joy_get_pos(&joy_x,&joy_y);
	btns=joy_get_btns();

	joyx_moved = (abs(joy_x - _old_joy_x)>JOY_NULL);
	joyy_moved = (abs(joy_y - _old_joy_y)>JOY_NULL);

	if (abs(joy_x) < JOY_NULL) joy_x = 0;
	if (abs(joy_y) < JOY_NULL) joy_y = 0;

	if (!rotang.pitch) rotang.pitch = fixmul(-joy_y * 128,FrameTime);
	if (!rotang.head) rotang.head = fixmul(joy_x * 128,FrameTime);
	
	if (joyx_moved) _old_joy_x = joy_x;
	if (joyy_moved) _old_joy_y = joy_y;

	speed = ((btns&2) || keyd_pressed[KEY_A])?SLOW_SPEED*3:(keyd_pressed[KEY_Z]?SLOW_SPEED/2:SLOW_SPEED);

	//now build matrices, do rotations, etc., etc.

	vm_angles_2_matrix(&rotmat,&rotang);
	vm_matrix_x_matrix(&new_pm,&obj->orient,&rotmat);
	obj->orient = new_pm;

	//move player

	vm_vec_copy_scale(&obj->velocity,&obj->orient.fvec,speed);
	vm_vec_copy_scale(&frame_vec,&obj->velocity,FrameTime);

	do {
		fix wall_part;
		vms_vector tvec;

		count++;

		vm_vec_add(&new_pos,&obj->pos,&frame_vec);

		hit = find_vector_intersection(&ipos,&iseg,&obj->pos,obj->seg_id,&new_pos,obj->size,-1);

		obj->seg_id = iseg;
		obj->pos = ipos;

		//-FIXJOHN-if (hit==HIT_OBJECT) ExplodeObject(hit_objnum);

		if (hit==HIT_WALL) {
			vm_vec_sub(&frame_vec,&new_pos,&obj->pos);	//part through wall
			wall_part = vm_vec_dot(wall_norm,&frame_vec);
			vm_vec_copy_scale(&tvec,wall_norm,wall_part);
			if ((wall_part == 0) || (vm_vec_mag(&tvec) < 5)) Int3();
			vm_vec_sub2(&frame_vec,&tvec);
		}

	} while (hit == HIT_WALL);

	Assert(check_point_in_seg(&obj->pos,obj->seg_id,0).centermask==0);

	//now bank player according to segment orientation

	desired_upvec = &Segments[obj->seg_id].sides[3].faces[0].normal;

	if (labs(vm_vec_dot(desired_upvec,&obj->orient.fvec)) < f1_0/2) {

		vm_vector_2_matrix(&temp_matrix,&obj->orient.fvec,desired_upvec,NULL);

		delta_ang = vm_vec_delta_ang(&obj->orient.uvec,&temp_matrix.uvec,&obj->orient.fvec);

		if (rotang.head) delta_ang += (rotang.head<0)?TURNROLL_ANG:-TURNROLL_ANG;

		if (abs(delta_ang) > DAMP_ANG) {

			roll_ang = fixmul(FrameTime,ROLL_RATE);

			if (abs(delta_ang) < roll_ang) roll_ang = delta_ang;
			else if (delta_ang<0) roll_ang = -roll_ang;

			vm_vec_ang_2_matrix(&rotmat,&forvec,roll_ang);

			vm_matrix_x_matrix(&new_pm,&obj->orient,&rotmat);
			obj->orient = new_pm;
		}
	}

}









