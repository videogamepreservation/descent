/*
 * $Source: f:/miner/source/main/editor/rcs/medrobot.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:04 $
 * 
 * Header for robot.c
 * 
 * $Log: medrobot.h $
 * Revision 2.0  1995/02/27  11:35:04  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.4  1994/08/02  14:17:56  mike
 * Clean up dialog boxes.
 * 
 * Revision 1.3  1994/07/21  19:34:58  yuan
 * Fixed #include problem
 * 
 * Revision 1.2  1994/04/27  21:01:16  matt
 * Added prototypes for robot functions
 * 
 * Revision 1.1  1994/04/27  20:28:59  matt
 * Initial revision
 * 
 * 
 */



#ifndef _MED_ROBOT_H
#define _MED_ROBOT_H

extern void robot_close_window();
extern void do_robot_window();

extern void object_close_window();
extern void do_object_window();


#endif
