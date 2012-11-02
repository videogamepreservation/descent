/*
 * $Source: f:/miner/source/main/editor/rcs/seguvs.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:58 $
 * 
 * Header for seguvs.c
 * 
 * $Log: seguvs.h $
 * Revision 2.0  1995/02/27  11:34:58  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.3  1994/08/03  10:32:28  mike
 * Add stretch_uvs_from_curedge.
 * 
 * Revision 1.2  1994/05/14  18:00:58  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.1  1994/05/14  17:27:26  matt
 * Initial revision
 * 
 * 
 */



#ifndef _SEGUVS_H
#define _SEGUVS_H

extern void assign_light_to_side(segment *sp, int sidenum);
extern void assign_default_lighting_all(void);
extern void stretch_uvs_from_curedge(segment *segp, int side);

#endif
