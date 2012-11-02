/*
 * $Source: f:/miner/source/main/editor/rcs/texpage.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:31 $
 * 
 * Definitions for texpage.c
 * 
 * $Log: texpage.h $
 * Revision 2.0  1995/02/27  11:34:31  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.3  1994/05/14  17:17:53  matt
 * Got rid of externs in source (non-header) files
 * 
 * Revision 1.2  1993/12/16  15:57:54  john
 * moved texture selection stuff to texpage.c
 * 
 * Revision 1.1  1993/12/16  15:22:34  john
 * Initial revision
 * 
 * 
 */



#ifndef _TEXPAGE_H
#define _TEXPAGE_H

#include "ui.h"

extern int TextureLights;
extern int TextureEffects;
extern int TextureMetals;

int texpage_grab_current(int n);
int texpage_goto_first();
void texpage_init( UI_WINDOW * win );
void texpage_close();
void texpage_do();

#endif
