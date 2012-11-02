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
 * $Source: f:/miner/source/2d/rcs/circle.c $
 * $Revision: 1.3 $
 * $Author: john $
 * $Date: 1994/11/18 22:51:01 $
 * 
 * .
 * 
 * $Log: circle.c $
 * Revision 1.3  1994/11/18  22:51:01  john
 * Changed a bunch of shorts to ints in calls.
 * 
 * Revision 1.2  1994/05/12  17:33:18  john
 * Added circle code.
 * 
 * Revision 1.1  1994/05/12  17:21:49  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: circle.c 1.3 1994/11/18 22:51:01 john Exp $";
#pragma on (unreferenced)

#include "mem.h"

#include "gr.h"
#include "grdef.h"

int gr_circle(fix xc1,fix yc1,fix r1)
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
		gr_pixel( xc-y, yc-x );
		gr_pixel( xc+y, yc-x );
		gr_pixel( xc-y, yc+x );
		gr_pixel( xc+y, yc+x );

		if (p<0) 
			p=p+(x<<2)+6;
		else	{
			// Draw the second octant
			gr_pixel( xc-x, yc-y );
			gr_pixel( xc+x, yc-y );
			gr_pixel( xc-x, yc+y );
			gr_pixel( xc+x, yc+y );
			p=p+((x-y)<<2)+10;
			y--;
		}
		x++;
	}
	if(x==y)	{
		gr_pixel( xc-x, yc-y );
		gr_pixel( xc+x, yc-y );
		gr_pixel( xc-x, yc+y );
		gr_pixel( xc+x, yc+y );
	}
	return 0;
}

int gr_ucircle(fix xc1,fix yc1,fix r1)
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
		gr_upixel( xc-y, yc-x );
		gr_upixel( xc+y, yc-x );
		gr_upixel( xc-y, yc+x );
		gr_upixel( xc+y, yc+x );

		if (p<0) 
			p=p+(x<<2)+6;
		else	{
			// Draw the second octant
			gr_upixel( xc-x, yc-y );
			gr_upixel( xc+x, yc-y );
			gr_upixel( xc-x, yc+y );
			gr_upixel( xc+x, yc+y );
			p=p+((x-y)<<2)+10;
			y--;
		}
		x++;
	}
	if(x==y)	{
		gr_upixel( xc-x, yc-y );
		gr_upixel( xc+x, yc-y );
		gr_upixel( xc-x, yc+y );
		gr_upixel( xc+x, yc+y );
	}
	return 0;
}

