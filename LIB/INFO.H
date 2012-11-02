/*
 * $Source: f:/miner/source/main/editor/rcs/info.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:32 $
 * 
 * Header for info.c
 * 
 * $Log: info.h $
 * Revision 2.0  1995/02/27  11:34:32  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.2  1994/05/14  17:18:17  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.1  1994/05/14  16:30:39  matt
 * Initial revision
 * 
 * 
 */



#ifndef _INFO_H
#define _INFO_H

void info_display_all( UI_WINDOW * wnd );

extern int init_info;

#endif
