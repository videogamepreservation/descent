/*
 * $Source: f:/miner/source/main/editor/rcs/eswitch.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:40 $
 * 
 * Headers for switch adding functions
 * 
 * $Log: eswitch.h $
 * Revision 2.0  1995/02/27  11:35:40  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.6  1994/05/30  20:22:35  yuan
 * New triggers.
 * 
 * Revision 1.5  1994/05/27  10:34:35  yuan
 * Added new Dialog boxes for Walls and Triggers.
 * 
 * Revision 1.4  1994/05/25  18:08:39  yuan
 * Revamping walls and triggers interface.
 * Wall interface complete, but triggers are still in progress.
 * 
 * Revision 1.3  1994/04/28  23:46:56  yuan
 * Added prototype for remove_trigger.
 * 
 * Revision 1.2  1994/03/15  16:34:20  yuan
 * Fixed bm loader (might have some changes in walls and switches)
 * 
 * Revision 1.1  1994/03/10  14:49:03  yuan
 * Initial revision
 * 
 * 
 */

#ifndef _ESWITCH_H
#define _ESWITCH_H

#include "inferno.h"
#include "segment.h"
#include "switch.h"

extern int bind_wall_to_trigger();

extern int trigger_remove();

extern int remove_trigger(segment *seg, short side);

extern void close_trigger_window();

extern void do_trigger_window();

#endif

