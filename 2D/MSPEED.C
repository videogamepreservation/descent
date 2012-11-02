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
 * $Source: f:/miner/source/2d/rcs/mspeed.c $
 * $Revision: 1.2 $
 * $Author: john $
 * $Date: 1993/10/15 16:23:32 $
 *
 * Program for testing video speed.
 *
 * $Log: mspeed.c $
 * Revision 1.2  1993/10/15  16:23:32  john
 * y
 * 
 * Revision 1.1  1993/09/08  11:43:59  john
 * Initial revision
 * 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "gr.h"
#include "grdef.h"
#include "timer.h"


#define RelTime(x)  (((float)(x*100/fastest))/100.0)

unsigned int MyPoly[100];

char TempBufferX[320*200+16];
char TempBuffer2[320*200+16];

char * TempBuffer1;

char * Video1;
char * Video2;
char * Video4;

char * System1;
char * System2;
char * System4;


main()
{
	unsigned int t1, t2, fastest;
	unsigned int timeb1v, timeb2v, timeb4v, timeb1s, timeb2s, timeb4s;
	unsigned int timew1v, timew2v, timew4v, timew1s, timew2s, timew4s;
	unsigned int timed1v, timed2v, timed4v, timed1s, timed2s, timed4s;

	gr_init( 0 );

	timer_init( 0, NULL );


	TempBuffer1 = (unsigned char *)(((unsigned int)TempBufferX+0xF) & 0xFFFFFFF0);

	System4  = (unsigned char *)(((unsigned int)TempBuffer2+0xF) & 0xFFFFFFF0);
	System2  = (unsigned char *)((unsigned int)System4 + 2);
	System1  = (unsigned char *)((unsigned int)System4 + 1);



	Video4 = (unsigned char *)0xA0000;
	Video2 = (unsigned char *)0xA0002;
	Video1 = (unsigned char *)0xA0001;


	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, Video1, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, Video1, 320*200);
	t2 = timer_get_microseconds();
	fastest = t2-t1;
	if ((t2-t1) < fastest) fastest = t2-t1;
	timeb1v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, Video2, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, Video2, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timeb2v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, Video4, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, Video4, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timeb4v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, System1, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, System1, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timeb1s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, System2, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, System2, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timeb2s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, System4, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsb(TempBuffer1, System4, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timeb4s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, Video1, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, Video1, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timew1v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, Video2, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, Video2, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timew2v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, Video4, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, Video4, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timew4v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, System1, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, System1, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timew1s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, System2, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, System2, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timew2s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, System4, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsw(TempBuffer1, System4, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timew4s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, Video1, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, Video1, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timed1v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, Video2, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, Video2, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timed2v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, Video4, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, Video4, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timed4v = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, System1, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, System1, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timed1s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, System2, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, System2, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timed2s = t2 - t1;

	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, System4, 320*200);
	t2 = timer_get_microseconds();
	t1 = timer_get_microseconds();
	gr_linear_movsd(TempBuffer1, System4, 320*200);
	t2 = timer_get_microseconds();
	if ((t2-t1) < fastest) fastest = t2-t1;
	timed4s = t2 - t1;

	timer_close();
	gr_close();

	printf( "Relative memory move speeds: \n\n" );
	printf( "           Vid1  Vid2  Vid4  Sys1  Sys2  Sys4\n" );
	printf( "REP MOVSB: %#2.2f  %#2.2f  %#2.2f  %#2.2f  %#2.2f  %#2.2f\n", RelTime(timeb1v),RelTime(timeb2v),RelTime(timeb4v), RelTime(timeb1s),RelTime(timeb2s),RelTime(timeb4s) );
	printf( "REP MOVSW: %#2.2f  %#2.2f  %#2.2f  %#2.2f  %#2.2f  %#2.2f\n", RelTime(timew1v),RelTime(timew2v),RelTime(timew4v), RelTime(timew1s),RelTime(timew2s),RelTime(timew4s) );
	printf( "REP MOVSD: %#2.2f  %#2.2f  %#2.2f  %#2.2f  %#2.2f  %#2.2f\n", RelTime(timed1v),RelTime(timed2v),RelTime(timed4v), RelTime(timed1s),RelTime(timed2s),RelTime(timed4s) );

	printf( "\nA 1.00 corresponds to %d microseconds to move 320x200 unsigned chars.\n", fastest );

	return;
}
