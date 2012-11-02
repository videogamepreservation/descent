/*
 * $Source: f:/miner/source/main/editor/rcs/meddraw.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:12 $
 * 
 * Defnts for med drawing stuff
 * 
 * $Log: meddraw.h $
 * Revision 2.0  1995/02/27  11:35:12  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.3  1994/07/06  16:36:54  mike
 * Prototype for draw_mine_all.
 * 
 * Revision 1.2  1993/12/17  12:05:09  john
 * Took stuff out of med.c; moved into medsel.c, meddraw.c, medmisc.c
 * 
 * Revision 1.1  1993/12/17  08:55:14  john
 * Initial revision
 * 
 * 
 */



#ifndef _MEDDRAW_H
#define _MEDDRAW_H


void meddraw_init_views( grs_canvas * canvas);
void draw_world(grs_canvas *screen_canvas,editor_view *v,segment *mine_ptr,int depth);
void find_segments(short x,short y,grs_canvas *screen_canvas,editor_view *v,segment *mine_ptr,int depth);

//	segp = pointer to segments array, probably always Segments.
//	automap_flag = 1 if this render is for the automap, else 0 (for editor)
extern void draw_mine_all(segment *segp, int automap_flag);

#endif
