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
 * $Source: f:/miner/source/includes/rcs/types.h $
 * $Revision: 1.2 $
 * $Author: matt $
 * $Date: 1993/09/14 12:12:30 $
 *
 * Common types for use in Miner
 *
 * $Log: types.h $
 * Revision 1.2  1993/09/14  12:12:30  matt
 * Added #define for NULL
 * 
 * Revision 1.1  1993/08/24  12:50:40  matt
 * Initial revision
 * 
 *
 */

#ifndef _TYPES_H
#define _TYPES_H

//define a byte 
typedef signed char byte;

//define unsigned types;
typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

//define a boolean
typedef ubyte bool;

#ifndef NULL
#define NULL 0
#endif

#endif

