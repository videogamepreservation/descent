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
 * $Source: f:/miner/source/texmap/rcs/scanline.h $
 * $Revision: 1.2 $
 * $Author: john $
 * $Date: 1995/02/20 18:23:40 $
 * 
 * Prototypes for C versions of texture mapped scanlines.
 * 
 * $Log: scanline.h $
 * Revision 1.2  1995/02/20  18:23:40  john
 * Added new module for C versions of inner loops.
 * 
 * Revision 1.1  1995/02/20  17:44:16  john
 * Initial revision
 * 
 * 
 */



#ifndef _SCANLINE_H
#define _SCANLINE_H

extern void c_tmap_scanline_per();
extern void c_tmap_scanline_per_nolight();
extern void c_tmap_scanline_lin();
extern void c_tmap_scanline_lin_nolight();
extern void c_tmap_scanline_flat();
extern void c_tmap_scanline_shaded();

#endif

