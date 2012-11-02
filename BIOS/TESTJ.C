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
	TESTJ.C - joystick testing routines
*/

#include <stdio.h>
#include <conio.h>

#include "types.h"
#include "joy.h"
#include "key.h"
#include "mouse.h"
#include "swift.h"
#include "fix.h"
#include "timer.h"

void main (void)
{
	int k;
	int x,y, e, i,j,t1,t2,t3;
	int axis[4];
	fix xt1, xt2, bt;
	int axis_min[4], axis_center[4], axis_max[4], c1, c2, c3;

	setbuf( stdout, NULL );

	minit();
	dpmi_init();
	timer_init();
	joy_init();
	joy_set_slow_reading(JOY_BIOS_READINGS);
	key_init();
//	mouse_init();

	timer_set_rate( 9943 );

	joy_flush();

	t1 = timer_get_fixed_seconds();
	
	t1 = t2 = 0;
	k = JOY_ALL_AXIS;
	while (!key_inkey())	{
		//k = joy_get_btns();
		delay(100);
		t1 = timer_get_fixed_seconds();
		t3 = t1 - t2;
		t2  = t1;
		k = joy_get_button_down_time(0);
		printf( "%f\t%f\n", f2fl(k), f2fl(t3) );
		//k = joystick_read_raw_axis( JOY_ALL_AXIS, axis );
		//printf( "X1:%d\tY1:%d\tX2:%d\tY2:%d\n", axis[0], axis[1], axis[2], axis[3] );
	}
	exit(0);
}

/*
Normal joystick
	-2 buttons					
	-2 analog channels

Dual joysticks
	-4 buttons
	-4 analog channels

Thrustmaster (Pro) Flight Control System Mark I
	-4 buttons
	-If not used with WCS Mark II, then has a hat switch on channel BY
    that returns 4 digital values.
	
Thrustmaster Weapons Control System Mark II
	-adds analog throttle on BY, but makes the FCS hat switch return
	 
CH Flightstick Pro
	-4 buttons + hat switch (buttons use bit combinations)
	-Analog throttle on BY.

Gravis Gamepad
	-4 buttons
	-XY axis returns digital values, needs sensitivity adjustment like keyboard

Logitech Cyberman
	-3 digital buttons
	-Pitch, bank, heading, up/down digital values, but in future might be analog
	-Analog XY

Mouse
	-3 digital buttons
	-2 analog channels

analog throttle		   {analog axis}
analog turn up/down     {analog axis}
analog turn left/right  {analog axis}
analog slide up/down	   [modifier]{analog axis}
analog slide left/right [modifier]{analog axis}


	


*/


