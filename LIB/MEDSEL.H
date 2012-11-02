/*
 * $Source: f:/miner/source/main/editor/rcs/medsel.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:28 $
 * 
 * rountines stipped from med.c for segment selection
 * 
 * $Log: medsel.h $
 * Revision 2.0  1995/02/27  11:34:28  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.2  1993/12/17  12:18:35  john
 * Moved selection stuff out of med.c
 * 
 * Revision 1.1  1993/12/17  09:29:51  john
 * Initial revision
 * 
 * 
 */



#ifndef _MEDSEL_H
#define _MEDSEL_H

extern void sort_seg_list(int n_segs,short *segnumlist,vms_vector *pos);

#endif
