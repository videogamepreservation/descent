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
 * $Source: f:/miner/source/main/rcs/demo.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:27:03 $
 * 
 * Demo constants.
 * 
 * $Log: demo.h $
 * Revision 2.0  1995/02/27  11:27:03  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.5  1994/05/14  17:16:10  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.4  1994/02/02  09:40:49  mike
 * prototypes, change MAX_DEMO_RECORDS.
 * 
 * Revision 1.3  1994/02/01  15:15:02  mike
 * Self-running demo code.
 * 
 * Revision 1.2  1994/02/01  11:51:49  mike
 * demo functions.
 * 
 * Revision 1.1  1994/01/31  19:15:05  mike
 * Initial revision
 * 
 * 
 */



#ifndef _DEMO_H
#define _DEMO_H

#include "types.h"
#include "vecmat.h"

#define DEMO_INACTIVE 0
#define DEMO_RECORDING 1
#define DEMO_PLAYBACK 2

#define MAX_DEMO_RECS 1000	// a recording is made once/second (search for 65536 in demo.c)

typedef struct _demorec {
	fix		time;
	fix		x,y,z;
	fixang	p,b,h;
	short		segnum;
//	short		specials;
} demorec;


extern int Demo_mode, Auto_demo, demo_loaded;

extern void start_demo_playback(void);
extern void start_demo_recording(void);
extern void record_demo_frame(void);

extern int get_demo_data(fix curtime, vms_vector *pos, vms_angvec *pbh, short *segnum, int *do_fire);

extern void demo_startup(void);

#endif
