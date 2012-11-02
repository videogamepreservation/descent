/*
 * $Source: f:/miner/source/main/editor/rcs/medwall.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:10 $
 * 
 * Created from version 1.6 of main\wall.h
 * 
 * $Log: medwall.h $
 * Revision 2.0  1995/02/27  11:35:10  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.9  1994/09/28  17:31:51  mike
 * Prototype copy_group_walls().
 * 
 * Revision 1.8  1994/08/05  21:18:16  matt
 * Allow two doors to be linked together
 * 
 * Revision 1.7  1994/06/20  22:30:10  yuan
 * Fixed crazy runaway trigger bug that Adam found
 * 
 * Revision 1.6  1994/05/30  20:22:58  yuan
 * New triggers.
 * 
 * Revision 1.5  1994/05/25  18:08:37  yuan
 * Revamping walls and triggers interface.
 * Wall interface complete, but triggers are still in progress.
 * 
 * Revision 1.4  1994/05/18  18:22:04  yuan
 * Fixed delete segment and walls bug.
 * 
 * Revision 1.3  1994/03/17  18:08:41  yuan
 * New wall stuff... Cut out switches....
 * 
 * Revision 1.2  1994/03/15  16:34:10  yuan
 * Fixed bm loader (might have some changes in walls and switches)
 * 
 * Revision 1.1  1994/02/10  17:52:01  matt
 * Initial revision
 * 
 * 
 * 
 */

#ifndef _MEDWALL_H
#define _MEDWALL_H

#include "wall.h"
#include "inferno.h"
#include "segment.h"

extern int wall_add_removable(); 

// Restores all the walls to original status
extern int wall_restore_all();

// Reset a wall.
extern void wall_reset(segment *seg, short side);

// Adds a removable wall (medwall.c)
extern int wall_add_removable();

// Adds a door (medwall.c)
extern int wall_add_door();

// Adds an illusory wall (medwall.c)
extern int wall_add_illusion();

// Removes a removable wall (medwall.c) 
extern int wall_remove_blastable(); 

// Adds a wall. (visually)
extern int wall_add_to_curside();
extern int wall_add_to_markedside();
 
// Removes a wall. (visually)
extern int wall_remove();

// Removes a specific side.
int wall_remove_side(segment *seg, short side);

extern int bind_wall_to_control_center();

extern void close_wall_window();

extern void do_wall_window();

extern wall_link_doors();
extern wall_unlink_door();
extern void copy_group_walls(int old_group, int new_group);

#endif

