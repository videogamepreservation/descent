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
 * $Source: f:/miner/source/2d/rcs/effects.h $
 * $Revision: 1.2 $
 * $Author: john $
 * $Date: 1993/10/26 13:17:58 $
 * 
 * Special effects stuff.
 * 
 * $Log: effects.h $
 * Revision 1.2  1993/10/26  13:17:58  john
 * *** empty log message ***
 * 
 * Revision 1.1  1993/10/25  14:56:02  john
 * Initial revision
 * 
 * 
 */


extern void gr_fade_canvas(int fade);
extern void gr_deaccent_canvas();
extern void gr_grey_canvas();
extern void effects_headlight_copy( grs_bitmap * src_bmp, grs_bitmap * dest_bmp, grs_bitmap * mask_bmp );


