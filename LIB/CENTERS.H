/*
 * $Source: f:/miner/source/main/editor/rcs/centers.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:02 $
 * 
 * Center header
 * 
 * $Log: centers.h $
 * Revision 2.0  1995/02/27  11:35:02  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.3  1994/08/02  12:16:27  mike
 * Prototype Center_names.
 * 
 * Revision 1.2  1994/07/21  19:35:20  yuan
 * Fixed #include problem
 * 
 * Revision 1.1  1994/07/21  19:11:36  yuan
 * Initial revision
 * 
 * 
 */



#ifndef _CENTERS_H
#define _CENTERS_H

#define	CENTER_STRING_LENGTH	12

#include	"fuelcen.h"

extern	char	Center_names[MAX_CENTER_TYPES][CENTER_STRING_LENGTH];

void close_centers_window();
void do_centers_window();

#endif
