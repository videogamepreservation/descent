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
	TESTMS.C - Mouse testing routines
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "mouse.h"
#include "key.h"
#include "timer.h"

void main (void)
{
	int x, y, dx, dy,  b;
	fix dt1, dt2, dt3;
	fix t;

	printf( "This tests the mouse interface. ESC exits.\n" );

	dpmi_init();
	key_init();
	timer_init();
	b = mouse_init();
	if (!b)  {
		printf( "No mouse installed.\n" );
		exit(1);
	}
	printf( "Mouse has %d buttons...\n", b );
	key_getch();

	t = timer_get_fixed_seconds();

	while( !keyd_pressed[KEY_ESC])
	{
		mouse_get_pos( &x, &y );
		mouse_get_delta( &dx, &dy );
		b = mouse_get_btns();
		dt1 = mouse_button_down_time(MB_LEFT);
		dt2 = mouse_button_down_time(MB_HEAD_LEFT);
		dt3 = mouse_button_down_time(MB_HEAD_RIGHT);

		printf( "POS:(%d,%d)\tDELTA:(%d,%d)\tBUTTONS:%d\tDOWNTIME:%12.5f %12.5f %12.5f\n", x, y, dx, dy, b, f2fl(dt1), f2fl(dt2),f2fl(dt3) );

		while( (t+F1_0/20) < timer_get_fixed_seconds() )
			;
		t = timer_get_fixed_seconds();
	}

	mouse_close();
	key_close();
}

