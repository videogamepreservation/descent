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
 * $Source: f:/miner/source/texmap/rcs/tmap.c $
 * $Revision: 1.19 $
 * $Author: mike $
 * $Date: 1994/03/14 15:44:27 $
 *
 * Texture mapper
 *
 * $Log: tmap.c $
 * Revision 1.19  1994/03/14  15:44:27  mike
 * Streamline code, rendering this file obsolete, replaced by ntmap.c
 * 
 * Revision 1.18  1994/02/10  18:35:15  matt
 * Got rid of DEBUG_ON define, now replaced by NDEBUG system
 * 
 * Revision 1.17  1994/02/08  15:21:28  mike
 * sc2000 stuff.
 * 
 * Revision 1.16  1994/01/31  15:43:01  mike
 * who knows, probably a lot of stuff...
 * 
 * Revision 1.15  1994/01/26  11:50:28  mike
 * Don't make dark textures not plot.
 * 
 * Revision 1.14  1994/01/21  21:12:53  mike
 * sky stuff.
 * 
 * Revision 1.13  1994/01/21  11:58:47  john
 * Changed lighting.
 * 
 * Revision 1.12  1994/01/18  10:51:11  mike
 * Interface to sky texture mapper.
 * 
 * Revision 1.11  1994/01/14  14:02:43  mike
 * *** empty log message ***
 * 
 * Revision 1.10  1993/12/20  13:56:45  mike
 * Remove "negative lighting value" message.
 * 
 * Revision 1.9  1993/12/17  19:59:58  mike
 * Change lighting.
 * 
 * Revision 1.8  1993/12/07  12:28:01  john
 * Moved bmd_palette to gr_palette
 * 
 * Revision 1.7  1993/12/06  13:28:11  mike
 * Prevent divide overflow.
 * 
 * Revision 1.6  1993/11/30  17:17:11  john
 * *** empty log message ***
 * 
 * Revision 1.5  1993/11/23  18:08:13  mike
 * Put in check for under/over flowing lighting values.
 * 
 * Revision 1.4  1993/11/23  15:09:19  mike
 * Added lighting to perspective tmapper
 * 
 * Revision 1.3  1993/11/22  10:24:04  mike
 * *** empty log message ***
 * 
 * Revision 1.2  1993/10/06  12:41:45  mike
 * Change draw_tmap to conform to Matt's new interface.
 * 
 * Revision 1.1  1993/09/08  17:29:09  mike
 * Initial revision
 * 
 *
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: tmap.c 1.19 1994/03/14 15:44:27 mike Exp $";
#pragma on (unreferenced)



