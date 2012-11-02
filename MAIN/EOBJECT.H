/*
 * $Source: f:/miner/source/main/editor/rcs/eobject.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:30 $
 * 
 * Header for eobject.c
 * 
 * $Log: eobject.h $
 * Revision 2.0  1995/02/27  11:35:30  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.5  1994/09/15  22:57:46  matt
 * Made new objects be oriented to their segment
 * Added keypad function to flip an object upside-down
 * 
 * Revision 1.4  1994/08/25  21:57:23  mike
 * Prototype ObjectSelectPrevInMine, and probably wrote it too, though not in this file.
 * 
 * Revision 1.3  1994/08/05  18:17:48  matt
 * Made object rotation have 4x resolution, and SHIFT+rotate do old resolution.
 * 
 * Revision 1.2  1994/05/14  18:00:59  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.1  1994/05/14  17:36:30  matt
 * Initial revision
 * 
 * 
 */



#ifndef _EOBJECT_H
#define _EOBJECT_H

int ObjectSelectNextInMine(void);
int ObjectSelectPrevInMine(void);

int	ObjectDecreaseBankBig(); 
int	ObjectIncreaseBankBig(); 
int	ObjectDecreasePitchBig();
int	ObjectIncreasePitchBig();
int	ObjectDecreaseHeadingBig();
int	ObjectIncreaseHeadingBig();
int 	ObjectFlipObject();

#endif
