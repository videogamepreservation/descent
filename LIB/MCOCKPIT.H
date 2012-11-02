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
 * $Source: f:/miner/source/main/rcs/mcockpit.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:30:24 $
 * 
 * .
 * 
 * $Log: mcockpit.h $
 * Revision 2.0  1995/02/27  11:30:24  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.3  1994/05/30  19:28:16  john
 * Moved functionality of mcockpit into 2d, ibitblt.
 * 
 * Revision 1.2  1994/05/11  11:19:21  john
 * Made cockpit code be created at runtime.
 * 
 * Revision 1.1  1994/05/11  10:38:56  john
 * Initial revision
 * 
 * 
 */



#ifndef _MCOCKPIT_H
#define _MCOCKPIT_H

/*
int mcockpit_find_inverted_bitblit_code_size( grs_bitmap * bmp, int sx, int sy, int sw, int sh, int srowsize, ubyte transparent_color );
int mcockpit_create_inverted_bitblit( grs_bitmap * bmp, int sx, int sy, int sw, int sh, int srowsize, ubyte transparent_color, ubyte * code );
void mcockpit_do_inverted_bitblit(char *start_si, char *start_di, ubyte * code);
void mcockpit_find_extents( grs_bitmap * bmp, int *minx, int *miny, int *maxx, int *maxy );
*/

#endif

