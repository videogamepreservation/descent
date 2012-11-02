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
 * $Source: f:/miner/source/2d/rcs/disc.c $
 * $Revision: 1.5 $
 * $Author: john $
 * $Date: 1994/11/18 22:50:16 $
 *
 * Graphical routines for drawing a disk.
 *
 * $Log: disc.c $
 * Revision 1.5  1994/11/18  22:50:16  john
 * Changed shorts to ints in parameters.
 * 
 * Revision 1.4  1994/05/06  12:50:12  john
 * Added supertransparency; neatend things up; took out warnings.
 * 
 * Revision 1.3  1994/04/22  11:16:04  john
 * *** empty log message ***
 * 
 * Revision 1.2  1993/10/15  16:22:24  john
 * *** empty log message ***
 * 
 * Revision 1.1  1993/09/08  11:43:24  john
 * Initial revision
 * 
 *
 */

#include "mem.h"

#include "gr.h"
#include "grdef.h"

int gr_disk(fix xc1,fix yc1,fix r1)
{
	int p,x, y, xc, yc, r;

	r = f2i(r1);
	xc = f2i(xc1);
	yc = f2i(yc1);
	p=3-(r*2);
	x=0;
	y=r;

	// Big clip
	if ( (xc+r) < 0 ) return 1;
	if ( (xc-r) > WIDTH ) return 1;
	if ( (yc+r) < 0 ) return 1;
	if ( (yc-r) > HEIGHT ) return 1;

	while(x<y)
	{
		// Draw the first octant
		gr_scanline( xc-y, xc+y, yc-x );
		gr_scanline( xc-y, xc+y, yc+x );

		if (p<0) 
			p=p+(x<<2)+6;
		else	{
			// Draw the second octant
			gr_scanline( xc-x, xc+x, yc-y );
			gr_scanline( xc-x, xc+x, yc+y );
			p=p+((x-y)<<2)+10;
			y--;
		}
		x++;
	}
	if(x==y)	{
		gr_scanline( xc-x, xc+x, yc-y );
		gr_scanline( xc-x, xc+x, yc+y );
	}
	return 0;
}

int gr_udisk(fix xc1,fix yc1,fix r1)
{
	int p,x, y, xc, yc, r;

	r = f2i(r1);
	xc = f2i(xc1);
	yc = f2i(yc1);
	p=3-(r*2);
	x=0;
	y=r;

	while(x<y)
	{
		// Draw the first octant
		gr_uscanline( xc-y, xc+y, yc-x );
		gr_uscanline( xc-y, xc+y, yc+x );

		if (p<0) 
			p=p+(x<<2)+6;
		else	{
			// Draw the second octant
			gr_uscanline( xc-x, xc+x, yc-y );
			gr_uscanline( xc-x, xc+x, yc+y );
			p=p+((x-y)<<2)+10;
			y--;
		}
		x++;
	}
	if(x==y)	{
		gr_uscanline( xc-x, xc+x, yc-y );
		gr_uscanline( xc-x, xc+x, yc+y );
	}
	return 0;
}

