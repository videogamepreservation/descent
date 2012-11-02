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
 * $Source: f:/miner/source/main/rcs/vfx.h $
 * $Revision: 2.1 $
 * $Author: john $
 * $Date: 1995/03/10 12:58:04 $
 * 
 * Prototypes for vfx.c
 * 
 * $Log: vfx.h $
 * Revision 2.1  1995/03/10  12:58:04  john
 * Added vfx_set_palette_sub function.
 * 
 * Revision 2.0  1995/02/27  11:31:19  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.5  1994/12/28  10:43:02  john
 * More VFX tweaking.
 * 
 * Revision 1.4  1994/12/28  10:26:49  john
 * Fixed some VFX problems.
 * 
 * Revision 1.3  1994/09/20  19:36:01  matt
 * Added seperate function to set palette
 * 
 * Revision 1.2  1994/06/24  17:03:51  john
 * Added VFX support. Also took all game sequencing stuff like
 * EndGame out and put it into gameseq.c
 * 
 * Revision 1.1  1994/06/23  09:14:16  john
 * Initial revision
 * 
 * 
 */



#ifndef _VFX_H
#define _VFX_H

extern int Game_vfx_flag;

extern void vfx_init_graphics();		// Sets vfx to stereo mode
extern void vfx_close_graphics();	// Sets vfx to mono mode
extern void vfx_set_page(ubyte page);		// 0 or 1
extern void vfx_init_positioning();
extern void vfx_close_positioning();
extern void vfx_read_positions(int *helmet_yaw, int *helment_pitch, int *helment_roll, int *puck_pitch, int *puck_roll, int *puck_buttons );
extern void vfx_set_palette();		//download palette into VFX
extern void vfx_set_palette_sub(ubyte * palette);

#endif

