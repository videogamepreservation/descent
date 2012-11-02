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
 * $Source: f:/miner/source/2d/rcs/box.c $
 * $Revision: 1.3 $
 * $Author: john $
 * $Date: 1994/11/18 22:50:19 $
 *
 * Graphical routines for drawing boxes.
 *
 * $Log: box.c $
 * Revision 1.3  1994/11/18  22:50:19  john
 * Changed shorts to ints in parameters.
 * 
 * Revision 1.2  1993/10/15  16:23:31  john
 * y
 * 
 * Revision 1.1  1993/09/08  11:43:11  john
 * Initial revision
 * 
 *
 */

#include "mem.h"


#include "gr.h"
#include "grdef.h"

void gr_ubox0(int left,int top,int right,int bot)
{
	int i, d;

	unsigned char * ptr1;
	unsigned char * ptr2;

	ptr1 = DATA + ROWSIZE *top+left;

	ptr2 = ptr1;
	d = right - left;

	for (i=top; i<=bot; i++ )
	{
		ptr2[0] = COLOR;
		ptr2[d] = COLOR;
		ptr2 += ROWSIZE;
	}

	ptr2 = ptr1;
	d = (bot - top)*ROWSIZE;

	for (i=1; i<(right-left); i++ )
	{
		ptr2[i+0] = COLOR;
		ptr2[i+d] = COLOR;
	}
}

void gr_box0(int left,int top,int right,int bot)
{
	if (top > MAXY ) return;
    if (bot < MINY ) return;
    if (left > MAXX ) return;
    if (right < MINX ) return;
    
	if (top < MINY) top = MINY;
    if (bot > MAXY ) bot = MAXY;
	if (left < MINX) left = MINX;
    if (right > MAXX ) right = MAXX;

	gr_ubox0(left,top,right,bot);

}


void gr_ubox12(int left,int top,int right,int bot)
{
	int i;

	for (i=top; i<=bot; i++ )
	{
		gr_upixel( left, i );
		gr_upixel( right, i );
	}

	gr_uscanline( left, right, top );

	gr_uscanline( left, right, bot );
}

void gr_box12(int left,int top,int right,int bot)
{
    if (top > MAXY ) return;
    if (bot < MINY ) return;
    if (left > MAXX ) return;
    if (right < MINX ) return;
    
	if (top < MINY) top = MINY;
    if (bot > MAXY ) bot = MAXY;
	if (left < MINX) left = MINX;
    if (right > MAXX ) right = MAXX;
        
	gr_ubox12(left, top, right, bot );
    
}

void gr_ubox(int left,int top,int right,int bot)
{
	if (TYPE==BM_LINEAR)
		gr_ubox0( left, top, right, bot );

	else if ( TYPE == BM_MODEX )
		gr_ubox12( left, top, right, bot );

    else
		gr_ubox12( left, top, right, bot );
}

void gr_box(int left,int top,int right,int bot)
{
	if (TYPE==BM_LINEAR)
		gr_box0( left, top, right, bot );

	else if ( TYPE == BM_MODEX )
		gr_box12( left, top, right, bot );
    
	else
		gr_ubox12( left, top, right, bot );
}

