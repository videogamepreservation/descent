/*
 * $Source: f:/miner/source/main/editor/rcs/medmisc.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:40 $
 * 
 * Defn'tns for medmisc.c
 * 
 * $Log: medmisc.h $
 * Revision 2.0  1995/02/27  11:34:40  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.3  1994/07/21  17:25:28  matt
 * Took out unused func medlisp_create_new_mine() and its prototype
 * 
 * Revision 1.2  1993/12/17  12:05:04  john
 * Took stuff out of med.c; moved into medsel.c, meddraw.c, medmisc.c
 * 
 * Revision 1.1  1993/12/17  08:45:23  john
 * Initial revision
 * 
 * 
 */



#ifndef _MEDMISC_H
#define _MEDMISC_H

void GetMouseRotation( int idx, int idy, vms_matrix * RotMat );
extern int Gameview_lockstep;				//In medmisc.c
int ToggleLockstep();
int medlisp_delete_segment(void);
int medlisp_scale_segment(void);
int medlisp_rotate_segment(void);
int ToggleLockViewToCursegp(void);
int ToggleDrawAllSegments();
int IncreaseDrawDepth(void);
int DecreaseDrawDepth(void);
int ToggleCoordAxes();
extern int	Big_depth;
void set_chase_matrix(segment *sp);
void set_view_target_from_segment(segment *sp);


#endif
