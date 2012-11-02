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
	TESTT.C - Timer handler testing routines
*/

#include <stdio.h>

#include "timer.h"
#include "key.h"

extern int timer_interrupted;
extern unsigned short timer_value;

int count=0;

#pragma off (check_stack);			// No stack checking!
void _loadds _far my_handler()
{
	count++;
}
#pragma on (check_stack);			

void main (void)
{
	fix t1, t2, t3, delta, mdelta, mt3;

	printf( "Press any key to start...\n" );

	timer_init();
	timer_set_rate( 9943 );

	t1 = t2 = timer_get_fixed_seconds();

	mdelta = 0;
	mt3 = 0;
	while(!kbhit())	{
		t3 = t2;
		t1 = timer_get_approx_seconds();
		t2 = timer_get_fixed_seconds();
		delta = t2 - t1;
		if ( abs(delta) > mdelta )
			mdelta = abs(delta);

		if ( abs(t2-t3) > mt3 )
			mt3 = abs(t2-t3);
		printf( "%.8f\t%.8f\t%.8f\t%d\t%.8f\t%.8f\n", f2fl(t2), f2fl(t2-t1), f2fl(mdelta), TICKER/19, f2fl(t2-t3), f2fl(mt3) );
//		if ( t2 < t1 )	{
//			printf( "Bad time of %.8f\n", f2fl(t1-t2) );
//		}
	}
}

