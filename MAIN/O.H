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
 * $Source: f:/miner/source/main/rcs/object.h $
 * $Revision: 1.75 $
 * $Author: matt $
 * $Date: 1994/08/03 21:06:19 $
 * 
 * object system definitions
 * 
 * $Log: object.h $
 * Revision 1.75  1994/08/03  21:06:19  matt
 * Added prototype for fix_object_segs(), and renamed now-unused spawn_pos
 * 
 * Revision 1.74  1994/08/02  12:30:27  matt
 * Added support for spinning objects
 * 
 * Revision 1.73  1994/07/27  20:53:25  matt
 * Added rotational drag & thrust, so turning now has momemtum like moving
 * 
 * Revision 1.72  1994/07/27  19:44:21  mike
 * Objects containing objects.
 * 
 * Revision 1.71  1994/07/22  20:43:29  matt
 * Fixed flares, by adding a physics flag that makes them stick to walls.
 * 
 * Revision 1.70  1994/07/21  12:42:10  mike
 * Prototype new find_object_seg and update_object_seg.
 * 
 * Revision 1.69  1994/07/19  15:26:39  mike
 * New ai_static structure.
 * 
 * Revision 1.68  1994/07/13  00:15:06  matt
 * Moved all (or nearly all) of the values that affect player movement to 
 * bitmaps.tbl
 * 
 * Revision 1.67  1994/07/12  12:40:12  matt
 * Revamped physics system
 * 
 * Revision 1.66  1994/07/06  15:26:23  yuan
 * Added chase mode.
 * 
 * Revision 1.65  1994/06/29  19:43:31  matt
 * Made powerup animation not happen in render routine
 * 
 * Revision 1.64  1994/06/27  18:31:09  mike
 * Add flares.
 * 
 * Revision 1.63  1994/06/14  21:15:21  matt
 * Made rod objects draw lighted or not depending on a parameter, so the
 * materialization effect no longer darkens.
 * 
 * Revision 1.62  1994/06/14  16:55:00  matt
 * Got rid of physics_object speed field
 * 
 * Revision 1.61  1994/06/09  16:24:58  matt
 * Fixed confusion with two constants, MAX_SUBOBJS & MAX_SUBMODELS, which
 * were used for the same things, but had different values.
 * 
 * Revision 1.60  1994/06/08  10:56:44  matt
 * Improved debris: now get submodel size from new POF files; debris now has
 * limited life; debris can now be blown up.
 * 
 * Revision 1.59  1994/06/02  19:30:49  matt
 * Moved texture-mapped rod drawing stuff (used for hostage & now for the
 * materialization center) to object.c
 * 
 * Revision 1.58  1994/05/30  22:50:20  matt
 * Added morph effect for robots
 * 
 * Revision 1.57  1994/05/30  19:46:47  mike
 * Move MAX_SUBOBJS from here to inferno.h
 * 
 * Revision 1.56  1994/05/30  00:03:19  matt
 * Got rid of robot render type, and generally cleaned up polygon model
 * render objects.
 * 
 * Revision 1.55  1994/05/26  21:08:44  matt
 * Moved robot stuff out of polygon model and into robot_info struct
 * Made new file, robot.c, to deal with robots
 * 
 * Revision 1.54  1994/05/20  11:56:59  matt
 * Cleaned up find_vector_intersection() interface
 * Killed check_point_in_seg(), check_player_seg(), check_object_seg()
 * 
 * Revision 1.53  1994/05/19  18:53:18  yuan
 * Changing player structure...
 * 
 * Revision 1.52  1994/05/17  12:36:16  matt
 * Changed parms to obj_create() and cleaned up object creation code
 * 
 * Revision 1.51  1994/05/17  11:53:45  yuan
 * Adding Player_init, which saves position of player at beginning of level.
 * 
 * Revision 1.50  1994/05/17  10:37:08  matt
 * Added new parm to reset_objects(), made Num_objects not public, added
 * Highest_object_index().
 * 
 * Revision 1.49  1994/05/16  16:48:14  yuan
 * Added symbolic constant OBJECT_INITIAL_SHIELDS
 * 
 * Revision 1.48  1994/05/16  16:17:47  john
 * Bunch of stuff on my Inferno Task list May16-23
 * 
 * Revision 1.47  1994/05/16  10:12:12  matt
 * Added prototype for init_player_object()
 * 
 * Revision 1.46  1994/05/14  17:15:06  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.45  1994/05/13  20:27:37  john
 * Version II of John's new object code.
 * 
 * Revision 1.44  1994/05/12  14:44:39  mike
 * Add ai_info to object structure.
 * 
 * Revision 1.43  1994/05/11  23:33:08  mike
 * Make RESULT_KILLS directional with RESULT_KILLS_1_2 and RESULT_KILLS_2_1.
 * 
 * Revision 1.42  1994/05/10  18:02:16  mike
 * Increase MAX_OBJECTS from 100 to 200
 * 
 * Revision 1.41  1994/05/07  16:42:07  matt
 * Changed name of turnbank to turnroll
 * 
 * Revision 1.40  1994/05/06  10:12:39  matt
 * Added new function, obj_create_copy()
 * 
 * Revision 1.39  1994/05/05  20:39:33  matt
 * Made physics code more general-purpose; moved code from object.c to
 * physics.c; renamed routines; made behavior such as levelling and turn
 * banking controllable by flags.
 * 
 * Revision 1.38  1994/05/05  16:41:12  matt
 * Cleaned up repair center code, and moved some from object.c to fuelcen.c
 * 
 * Revision 1.37  1994/05/03  19:33:40  matt
 * Cleaned up usage of rotvel field from physics_info structure
 * 
 * Revision 1.36  1994/05/02  18:34:14  matt
 * Changed parms to obj_link(), added a little more error checking, changed
 * a few fields in object physics_info, and probably a few other little things.
 * 
 * Revision 1.35  1994/05/02  16:30:31  john
 * Added object flag OF_SHOULD_BE_DEAD which fixed the bug
 * that caused very close robots to die 10 seconds after they
 * actually got shot.
 * 
 * Revision 1.34  1994/04/29  09:18:08  matt
 * Added support for multiple-piece explosions
 * 
 * Revision 1.33  1994/04/07  16:28:07  yuan
 * Added SUPERPIG the robot with 200 hit points.
 * Now robots can take multiple hits before blowing up.
 * 
 * Revision 1.32  1994/04/07  13:44:13  yuan
 * Added powerup_info to object structure.
 * 
 * Revision 1.31  1994/04/01  13:34:15  matt
 * Cleaned up explosion and laser code; added mutliple-stage explosions.
 * 
 * Revision 1.30  1994/04/01  11:15:07  yuan
 * Added multiple bitmap functionality to all objects...
 * (hostages, powerups, lasers, etc.)  
 * Hostages and powerups are implemented in the object system,
 * just need to finish function call to "affect" player.
 * 
 * Revision 1.29  1994/03/31  09:07:43  matt
 * Made last_pos a field of all objects, not just lasers
 * 
 * Revision 1.28  1994/03/30  21:14:33  yuan
 * Minor name change for object->damage...
 * object->lifeleft now.
 * 
 * Revision 1.27  1994/03/28  20:57:36  yuan
 * *** empty log message ***
 * 
 * Revision 1.26  1994/03/25  17:30:13  yuan
 * Checking in hostage stuff... (start of it)
 * 
 * Revision 1.25  1994/03/25  16:54:55  matt
 * polyobj_info field of object structure now has data for object animation
 * 
 * Revision 1.24  1994/02/17  11:32:50  matt
 * Changes in object system
 * 
 * 
 * 
 */

#ifndef _OBJECT_H
#define _OBJECT_H

#include "types.h"
#include "vecmat.h"
#include "segment.h"
#include "gameseg.h"
#include "aistruct.h"
#include "gr.h"

/*
 *		CONSTANTS
 */

#define MAX_OBJECTS			200		//total number of objects in world

//Object types
#define OBJ_NONE		255	//unused object
#define OBJ_WALL		0		//A wall... not really an object, but used for collisions
#define OBJ_FIREBALL	1		//a fireball, part of an explosion
#define OBJ_ROBOT		2		//an evil enemy
#define OBJ_HOSTAGE	3		//a hostage you need to rescue
#define OBJ_PLAYER	4		//the player on the console
#define OBJ_WEAPON	5		//a laser, missile, etc
#define OBJ_CAMERA	6		//a camera to slew around with
#define OBJ_POWERUP	7		//a powerup you can pick up
#define OBJ_DEBRIS	8		//a piece of robot
#define OBJ_CNTRLCEN	9		//the control center
#define OBJ_FLARE		10		//the control center
#define MAX_OBJECT_TYPES	11

//Result types
#define RESULT_NOTHING	0		//Ignore this collision
#define RESULT_CHECK		1		//Check for this collision

//Control types - what tells this object what do do
#define CT_NONE			0	//doesn't move (or change movement)
#define CT_AI				1	//driven by AI
#define CT_EXPLOSION		2	//explosion sequencer
#define CT_MULTIPLAYER	3	//via serial or network
#define CT_FLYING			4	//the player is flying
#define CT_SLEW			5	//slewing
#define CT_FLYTHROUGH	6	//the flythrough system
#define CT_DEMO			7	//recorded demo playing back
//#define CT_ROBOT_FLYTHROUGH	8	//special flythrough for robots
#define CT_WEAPON			9		//laser, etc.
#define CT_REPAIRCEN		10	//under the control of the repair center
#define CT_MORPH			11	//this object is being morphed
#define CT_DEBRIS			12	//this is a piece of debris
#define CT_POWERUP		13	//animating powerup blob

//Movement types
#define MT_NONE			0	//doesn't move
#define MT_PHYSICS		1	//moves by physics
#define MT_MULTIPLAYER	2	//another player (maybe just interpolation?)
#define MT_SPINNING		3	//this object doesn't move, just sits and spins

//Render types
#define RT_NONE			0	//does not render
#define RT_POLYOBJ		1	//a polygon model
#define RT_FIREBALL		2	//a fireball
#define RT_LASER			3	//a laser
#define RT_HOSTAGE		4	//a hostage
#define RT_POWERUP		5	//a powerup
#define RT_MORPH			6	//a robot being morphed

//misc object flags
#define OF_EXPLODING			1		//this object is exploding
#define OF_SHOULD_BE_DEAD	2		//this object should be dead, so next time we can, we should delete this object.

//Different Weapon ID types...
#define WEAPON_ID_LASER			0
#define WEAPON_ID_MISSLE		1
#define WEAPON_ID_CANNONBALL	2

//Object Initial shields...
#define OBJECT_INITIAL_SHIELDS F1_0/2

//physics flags
#define PF_TURNROLL		1		//roll when turning
#define PF_LEVELLING		2		//level object with closest side
#define PF_BOUNCE			4		//bounce (not slide) when hit will
#define PF_WIGGLE			8		//wiggle while flying
#define PF_STICK			16		//object sticks (stops moving) when hits wall

/*
 *		STRUCTURES
 */

//information for physics sim for an object
typedef struct physics_info {
	vms_vector	velocity;		//velocity vector of this object
	vms_vector	thrust;			//constant force applied to this object
	fix			mass;				//the mass of this object
	fix			drag;				//how fast this slows down
	fix			brakes;			//how much brakes applied
	vms_vector	rotvel;			//rotational velecity (angles)
	vms_vector	rotthrust;		//rotational acceleration
	fixang		turnroll;		//rotation caused by turn banking
	ushort		flags;			//misc physics flags
} physics_info;

//information on remotely-controlled object
typedef struct mplayer_info {
	int dummy;		//what will be here?
} mplayer_info;

//stuctures for different kinds of simulation

typedef struct laser_info {
	fix			lifeleft;		// decreased by FrameTime each frame, if > 0 don't decrease.
	short			parent_type; 	// The type of the parent of this object
	short			parent_num; 	// The object's parent's number
	int			parent_signature;	// The object's parent's signature...
	vms_vector  extra_vec1;		// Used to draw the proper width of laser bolt. (Offset)
} laser_info;

typedef struct explosion_info {
	fix			lifeleft;		// decreased by FrameTime each frame, if > 0 don't decrease.
	vms_vector	unused;			// was spawn_pos
	fix			spawn_time;		// when lifeleft is < this, spawn another
	fix			delete_time;	// when to delete object
	short			delete_objnum;	// and what object to delete
} explosion_info;

typedef struct flythrough_info {
	vms_angvec  angle_step;		// Used in flythrough
	vms_vector  heading;			// Where we want to be heading...
	int			ft_mode;
} flythrough_info;

typedef struct powerup_info {
	fix			frametime;
	byte			framenum;
} powerup_info;

//structures for different kinds of rendering

typedef struct polyobj_info {
	int			model_num;						//which polygon model
	vms_angvec	anim_angles[MAX_SUBMODELS];	//angles for each subobject
	int			subobj_flags;					//specify which subobjs to draw
} polyobj_info;

typedef struct object {
	int			signature;		// Every robot has a unique signature...
	ubyte			type;				// what type of object this is... robot, weapon, hostage, powerup, fireball
	ubyte			id;				// which form of object...which powerup, robot, etc.
	short			next,prev;		// id of next and previous connected object in Objects, -1 = no connection
	ubyte			control_type;  // how this object is controlled
	ubyte			movement_type; // how this object moves
	ubyte			render_type;	//	how this object renders
	ubyte			flags;			// misc flags
	short			segnum;			// segment number containing object
	segmasks		segmasks;		// does obj poke into other segs?
	vms_vector  pos;				// absolute x,y,z coordinate of center of object
	vms_matrix  orient;			// orientation of object in world
	fix			size;				// 3d size of object - for collision detection
	fix			shields; 		// Starts at maximum, when <0, object dies..
	vms_vector  last_pos;		// where object was last frame
	byte			contains_type;	//	Type of object this object contains (eg, spider contains powerup)
	byte			contains_id;	//	ID of object this object contains (eg, id = blue type = key)
	byte			contains_count; // number of objects of type:id this object contains
	byte			dum1;				//	Unused.

	//movement info, determined by MOVEMENT_TYPE
	union {
		physics_info phys_info;			//a physics object
		mplayer_info mplayer_info;		//controled by remote player
		vms_vector	 spin_rate;			//for spinning objects
	};

	//control info, determined by CONTROL_TYPE
	union {								
		laser_info laser_info;
		explosion_info expl_info;		//NOTE: debris uses this also
		ai_static	ai_info;
	};

	//render info, determined by RENDER_TYPE
	union {
		polyobj_info pobj_info;			//polygon model
		powerup_info powerup_info;		//powerup (vclip)
	};

} object;

typedef struct obj_position {
	vms_vector  pos;				// absolute x,y,z coordinate of center of object
	vms_matrix  orient;			// orientation of object in world
	short			segnum;			// segment number containing object
} obj_position;	

/*
 *		VARIABLES
 */

extern int Object_next_signature;		// The next signature for the next newly created object

extern ubyte CollisionResult[MAX_OBJECT_TYPES][MAX_OBJECT_TYPES];
// ie CollisionResult[a][b]==  what happens to a when it collides with b

extern int Player_objnum;

extern object Objects[];
extern int Highest_object_index;		//highest objnum
extern int num_objects;					//not really a public, included for

extern grs_bitmap *robot_bms[];		//all bitmaps for all robots
extern int robot_bm_nums[];			//starting bitmap num for each robot
extern int robot_n_bitmaps[];			//how many bitmaps for each robot
extern char *robot_names[];			//name of each robot

extern int Num_robot_types;

extern short free_obj_list[MAX_OBJECTS];

extern object *Player;			//pointer to the object that is the player
extern object *Viewer;			//which object we are seeing from

extern object Follow;

extern obj_position Player_init;

/*
 *		FUNCTIONS
 */


//do whatever setup needs to be done
void init_objects();

//returns segment number object is in.  Searches out from object's current
//seg, so this shouldn't be called if the object has "jumped" to a new seg
int obj_get_new_seg(object *obj);

//when an object has moved into a new segment, this function unlinks it
//from its old segment, and links it into the new segment
void obj_relink(short objnum,int newsegnum);

//move an object from one segment to another. unlinks & relinks
void obj_set_new_seg(short objnum,int newsegnum);

//links an object into a segment's list of objects.
//takes object number and segment number
void obj_link(int objnum,int segnum);

//unlinks an object from a segment's list of objects
void obj_unlink(int objnum);

//initialize a new object.  adds to the list for the given segment
//returns the object number
short obj_create(ubyte type,ubyte id,short segnum,vms_vector *pos,
				vms_matrix *orient,fix size,ubyte ctype,ubyte mtype,ubyte rtype);

//make a copy of an object. returs num of new object
short obj_create_copy(int objnum, vms_vector *new_pos, int newsegnum);

//remove object from the world
void obj_delete(short objnum);

//called after load.  Takes number of objects,  and objects should be 
//compressed
void reset_objects(int n_objs);

//make object array non-sparse
void compress_objects(void);

//Render an object.  Calls one of several routines based on type
void render_object(object *obj);

//Draw a blob-type object, like a fireball
void draw_object_blob(object *obj,grs_bitmap *bitmap);

//draw an object that is a texture-mapped rod
void draw_object_tmap_rod(object *obj,grs_bitmap *bitmap,int lighted);

//Deletes all objects that have been marked for death.
void obj_delete_all_that_should_be_dead();

// Toggles whether or not lock-boxes draw.
void object_toggle_lock_targets();

//move all objects for the current frame
void object_move_all();		// moves all objects

//set viewer object to next object in array
void object_goto_next_viewer();

//draw target boxes for nearby robots
void object_render_targets(void);

//move an object for the current frame
void object_move_one( object * obj );

//make object0 the player, setting all relevant fields
void init_player_object();

//check if object is in object->segnum.  if not, check the adjacent segs.
//if not any of these, returns false, else sets obj->segnum & returns true
//callers should really use find_vector_intersection()  
//Note: this function is in gameseg.c
int update_object_seg(struct object *obj);


//	Finds what segment *obj is in, returns segment number.
//	If not in any segment, returns -1.
//	Note: This function is defined in gameseg.h, but object.h depends on gameseg.h, and
//	object.h is where object is defined...get it?
extern int find_object_seg(object * obj );

//	Calls find_object_seg and relinks object if segment has changed.
extern int update_object_seg(object * obj );

//go through all objects and make sure they have the correct segment numbers
//used when debugging is on
fix_object_segs();

#endif
