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
#include "timer.h"
#include "fix.h"
#include "mono.h"

extern void mike_test(void);
extern void john_test(void);

int test_mike(void)
{
	int	start, finish;
	float	elapsed_time;

	mprintf(0, "Mike test:...");
	start = timer_get_fixed_seconds();

	mike_test();

	finish = timer_get_fixed_seconds();

	elapsed_time = (float) (finish - start);
	mprintf(0, "Done, time = %7.3f\n", elapsed_time/65536.0);

	return finish - start;

}

int test_john(void)
{
	int	start, finish;
	float	elapsed_time;

	mprintf(0, "John test:...");
	start = timer_get_fixed_seconds();

	john_test();

	finish = timer_get_fixed_seconds();

	elapsed_time = (float) (finish - start);
	mprintf(0, "Done, time = %7.3f\n", elapsed_time/65536.0);
	return finish - start;

}


