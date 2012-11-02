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
 * $Source: f:/miner/source/main/rcs/iglasses.c $
 * $Revision: 2.8 $
 * $Author: mike $
 * $Date: 1995/03/30 16:36:31 $
 * 
 * Routines for the i-glasses VR stuff.
 * 
 * $Log: iglasses.c $
 * Revision 2.8  1995/03/30  16:36:31  mike
 * text localization.
 * 
 * Revision 2.7  1995/03/24  13:10:37  john
 * Synced with shareware.
 * 
 * Revision 2.6  1995/03/09  18:07:35  john
 * Fixed bug with iglasses tracking not "centering" right.
 * Made VFX have bright headlight lighting.
 * 
 * Revision 2.5  1995/03/09  15:33:23  john
 * Fixed bug with iglasses timeout too long, and objects
 * disappearing from left eye.
 * 
 * Revision 2.4  1995/03/06  15:23:13  john
 * New screen techniques.
 * 
 * Revision 2.3  1995/03/03  22:38:30  john
 * Tweaked the filtering,.
 * 
 * Revision 2.2  1995/03/03  22:28:22  john
 * Added code to detect invalid serial ports.
 * 
 * Revision 2.1  1995/03/03  22:24:00  john
 * Added code to make iglasses work with Greenleaf.
 * 
 * Revision 1.2  1995/02/09  22:01:01  john
 * Added i-glasses tracking.
 * 
 * Revision 1.1  1995/02/09  15:53:47  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: iglasses.c 2.8 1995/03/30 16:36:31 mike Exp $";
#pragma on (unreferenced)

#define DOS4G		

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#include "error.h"
#include "mono.h"
#include "args.h"
#include "text.h"
#include "iglasses.h"
#include "key.h"
//#include "commlib.h" //nien - These aren't included -KRB
//#include "fast.h"
#include "timer.h"

int iglasses_headset_installed=0;

void iglasses_close_tracking();

//*******************************************

typedef struct  {
	int status;
	int count;
	
} PORT; //I added this it will compile, but I doubt it works. -KRB
//*******************************************


PORT * Iport = NULL;

#define USE_FILTERS 1

#ifdef USE_FILTERS 
#define FILTER_LENGTH 6
typedef struct filter {
  fix history[FILTER_LENGTH];
  fix weights[FILTER_LENGTH];
  long len;
  fix * hCurrent,* hEnd,* hRestart;
} filter;  

void initFIR(filter * f);
fix filterFIR(filter * f,fix newval);
static filter X_filter, Y_filter, Z_filter;
#endif

void iglasses_init_tracking(int serial_port)	
{
	fix t1;
	int c;

	if (iglasses_headset_installed) return;

	if ( (serial_port < 1) || (serial_port > 4) )	{
		Error( TXT_IGLASSES_ERROR_1 );
	}
	printf( "\n\n%s %d\n", TXT_IGLASSES_INIT, serial_port );
	printf( "%s\n", TXT_IGLASSES_ON);
	printf( "%s\n", TXT_PRESS_ESC_TO_ABORT);
	Iport = PortOpenGreenleafFast(serial_port-1, 9600, 'N', 8, 1 );
	if ( !Iport )	{
		printf( "%s\n", TXT_SERIAL_FAILURE, Iport->status );
		return;
	}
	
	SetDtr( Iport, 1 );
	SetRts( Iport, 1 );
	UseRtsCts( Iport, 0 );
	
	iglasses_headset_installed = 1;
	atexit( iglasses_close_tracking );

	while( 1 )	{
		printf( "." );
		t1 = timer_get_fixed_seconds() + F1_0;
		ClearRXBuffer(Iport);
		ClearTXBuffer(Iport);
		WriteBuffer( Iport, "!\r", 2 );
		while ( timer_get_fixed_seconds() < t1 )	{
			if ( key_inkey() == KEY_ESC ) exit(0);
			c = ReadChar( Iport );
			if ( c == 'O' )	{
				goto TrackerOK1;
			} 	
		}
	}

TrackerOK1:

	while( 1 )	{
		printf( "." );
		t1 = timer_get_fixed_seconds() + F1_0;
		ClearRXBuffer(Iport);
		ClearTXBuffer(Iport);
		// M2 = p,b,h
		// M1 = all data
		WriteBuffer( Iport, "!M1,P,B\r", 8 );
		while ( timer_get_fixed_seconds() < t1 )	{
			if ( key_inkey() == KEY_ESC ) exit(0);
			c = ReadChar( Iport );
			if ( c == 'O' )	{
				goto TrackerOK2;
			} 	
		}
	}

TrackerOK2:
	printf( ".\n" );
	ClearRXBuffer(Iport);
	ClearTXBuffer(Iport);

 	WriteChar( Iport, 'S' );

#ifdef USE_FILTERS 
	initFIR( &X_filter );
	initFIR( &Y_filter );
	initFIR( &Z_filter );
#endif

//	{
//		fix y,p,r;
//		while( 1 )	{
//			iglasses_read_headset( &y, &p, &r);
//		 	//printf( "%d\t%d\t%d\n", y, p, r );
//		 	printf( "%8.2f\n", f2fl(y) );
//			if (key_inkey()==KEY_ESC) break;
//		}
//	}

}

void iglasses_close_tracking()	{
	if ( iglasses_headset_installed )	{
		iglasses_headset_installed = 0;
		PortClose(Iport);
		Iport =  NULL;
	}
}

//UNUSED int iglasses_read_headset1( fix *yaw, fix *pitch, fix *roll )
//UNUSED {
//UNUSED 	int y,p,r;
//UNUSED 	static unsigned char buff[8];
//UNUSED 	unsigned char checksum;
//UNUSED 	int i,count;
//UNUSED 
//UNUSED 	ReadBufferTimed(Iport, buff, 8, 1000);
//UNUSED 	checksum = 0;
//UNUSED 	count = Iport->count;
//UNUSED 	for (i=0; i < 7; i++) checksum += buff[i];
//UNUSED 	if ( (count < 8) || (checksum != buff[7]) )	{
//UNUSED 		ClearRXBuffer(Iport);
//UNUSED 		WriteChar( Iport, 'S' );
//UNUSED 		*yaw = *pitch = *roll = 0;
//UNUSED 		return 0;
//UNUSED 	}
//UNUSED 	WriteChar( Iport, 'S' );
//UNUSED 
//UNUSED 	y  =  (short)(buff[1] << 8) | buff[2];
//UNUSED 	p  =  (short)(buff[3] << 8) | buff[4];
//UNUSED 	r  =  (short)(buff[5] << 8) | buff[6];
//UNUSED 
//UNUSED 	*yaw 		= y;
//UNUSED 	*pitch 	= p;
//UNUSED 	*roll 	= r;
//UNUSED 
//UNUSED 	return 1;
//UNUSED }

#define M_PI 3.14159265358979323846264338327950288
#define FBITS 16384.
#define TO_RADIANS (M_PI/FBITS)
#define TO_DEGREES (180./M_PI)
#define TO_FIX (0.5/M_PI)

fix i_yaw = 0;
fix i_pitch = 0;
fix i_roll = 0;

int Num_readings = 0;
fix BasisYaw, BasisPitch, BasisBank;

int iglasses_read_headset( fix *yaw, fix *pitch, fix *roll )
{
	static unsigned char buff[16];
	float radPitch,radRoll,sinPitch,sinRoll,cosPitch,cosRoll;
	float rotx,roty,rotz;
	float fx,fy,fz;
	long x,y,z,p,r;
	unsigned char checksum;
	long i,count;

	ReadBufferTimed(Iport, buff, 12, 10 );			// Wait 1/100 second for timeout.
	checksum = 0;
	count = Iport->count;
	ClearRXBuffer(Iport);
	WriteChar( Iport, 'S' );
	for (i=0; i < 11; i++) checksum += buff[i];
	if ( (count < 12) || (checksum != buff[11]) )	{
		*yaw = i_yaw;
		*pitch = i_pitch;
		*roll = i_roll;
		return 0;
	}

	x  =  (short)(buff[1] << 8) | buff[2];
	y  =  (short)(buff[3] << 8) | buff[4];
	z  =  (short)(buff[5] << 8) | buff[6];
	p  =  (short)(buff[7] << 8) | buff[8];
	r  =  (short)(buff[9] << 8) | buff[10];

	fx = (float)x/FBITS;
	fy = (float)y/FBITS;
	fz = (float)z/FBITS;
	radPitch = (float)p*TO_RADIANS;
	radRoll  = (float)r*TO_RADIANS;
	sinPitch = sin(radPitch);
	cosPitch = cos(radPitch);
	sinRoll  = sin(radRoll);
	cosRoll  = cos(radRoll);

	roty = cosPitch*fy - sinPitch*fz;
	rotz = sinPitch*fy + cosPitch*fz;
	rotx = cosRoll*fx  - sinRoll*roty;

#ifdef USE_FILTERS 
	*yaw   = filterFIR( &X_filter,fl2f(-atan2(rotz,rotx)*M_PI/2.0));
	*pitch = filterFIR( &Y_filter,fl2f(-radPitch*M_PI/2.0));
	*roll  = filterFIR( &Z_filter,fl2f(radRoll*M_PI/2.0));
	if ( Num_readings < 30 )	{
		Num_readings++;
		BasisYaw = *yaw;
		BasisPitch = *pitch;
		BasisBank = *roll;
	}
	*yaw -= BasisYaw;
	*pitch -= BasisPitch;
	*roll -= BasisBank;
#else
	*yaw   = fl2f(-atan2(rotz,rotx)*M_PI/2.0);
	*pitch = fl2f(-radPitch*M_PI/2.0);
	*roll  = fl2f(radRoll*M_PI/2.0);
#endif

	i_yaw = *yaw;
	i_pitch = *pitch;
	i_roll = *roll;

	return 1;
}


#ifdef USE_FILTERS 
void initWeights(filter * f) 
{
	fix sum;
	long i;

	// Generate weights.
	sum = 0;
	for (i=0; i < f->len; i++) {
		//f->weights[i] = i;		// Linear ramp
		//f->weights[i] = i*i;		// Exp ramp
		f->weights[i] = F1_0;		// Average

	}

	// Summate.
	for (i=0; i < f->len; i++) {
		sum += f->weights[i];
	}

	// Normalize and convert to fixed point.
	for (i=0; i < f->len; i++) {
		f->weights[i] = fixdiv(f->weights[i],sum);
	} 
}

void initHistory(filter * f) 
{
	long i;
	for (i=0; i < f->len; i++) {
		f->history[i] = F1_0;
	}
	f->hCurrent = f->history;
	f->hEnd     = &f->history[f->len-1];
	f->hRestart = &f->history[-1];
} 

void initFIR(filter * f) 
{
  f->len = FILTER_LENGTH;
  initWeights(f);
  initHistory(f);
} 

fix filterFIR(filter * f,fix newval) 
{
	fix * currp,* last;
	fix * weightp;
	fix sum;

	currp   = f->hCurrent;
	last    = f->hCurrent;
	weightp = f->weights;

	*(f->hCurrent)++ = newval;
	if (f->hCurrent == &f->history[f->len]) f->hCurrent = f->history;

	// Compute FIR filter
	sum = 0;
	while (1) {
		sum += fixmul( (*currp--), (*weightp++) );
    	if (currp == f->hRestart) currp = f->hEnd;
    	if (currp == last) break;
	} /* while */
	return sum/f->len;
} /* filterFIR */

#endif
