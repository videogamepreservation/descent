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
 * $Source: f:/miner/source/main/rcs/victor.c $
 * $Revision: 2.5 $
 * $Author: mike $
 * $Date: 1995/03/30 16:35:59 $
 * 
 * Routines to access the VictorMaxx helmet.
 * 
 * $Log: victor.c $
 * Revision 2.5  1995/03/30  16:35:59  mike
 * text localization.
 * 
 * Revision 2.4  1995/03/09  15:34:33  john
 * Fixed bug with timeout being too long.
 * 
 * Revision 2.3  1995/03/08  15:42:00  john
 * Took out debug code.
 * 
 * Revision 2.2  1995/03/08  15:32:54  john
 * Made VictorMaxx head tracking use Greenleaf code.
 * 
 * Revision 2.1  1995/03/06  15:24:03  john
 * New screen techniques.
 * 
 * Revision 2.0  1995/02/27  11:31:23  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.13  1994/12/06  16:30:40  yuan
 * Localization
 * 
 * Revision 1.12  1994/11/27  23:15:06  matt
 * Made changes for new mprintf calling convention
 * 
 * Revision 1.11  1994/11/19  15:14:56  mike
 * remove unused code and data
 * 
 * Revision 1.10  1994/11/13  16:32:58  matt
 * Made angle averaging work right when yaw is near 0/360 degrees
 * Made angle read code empty buffer and use last values
 * 
 * Revision 1.9  1994/11/12  13:47:37  john
 * Reenabled the victor screen mode setting code.
 * 
 * Revision 1.8  1994/11/11  18:33:26  john
 * Made victor not set screen mode.
 * 
 * Revision 1.7  1994/08/25  18:43:44  john
 * First revision of new control code.
 * 
 * Revision 1.6  1994/07/25  10:24:04  john
 * Victor stuff.
 * 
 * Revision 1.5  1994/07/22  17:53:32  john
 * Added better victormax support.
 * 
 * Revision 1.4  1994/07/21  21:31:31  john
 * First cheapo version of VictorMaxx tracking.
 * 
 * Revision 1.3  1994/07/20  21:30:34  john
 * Got screen mode change to work.
 * 
 * Revision 1.2  1994/07/20  21:04:31  john
 * Add VictorMax VR helment support.
 * 
 * Revision 1.1  1994/07/20  20:49:01  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: victor.c 2.5 1995/03/30 16:35:59 mike Exp $";
#pragma on (unreferenced)

#define DOS4G		

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "error.h"
#include "mono.h"
#include "args.h"
//#include "serial.h"
#include "victor.h"
#include "text.h"
//#include "commlib.h" //Not included in public release -KRB
//#include "fast.h" //Not included in public release -KRB
#include "key.h"
//This include is just to allow compiling. It doesn't mean it will work. Values in here are only dummy values
#include "nocomlib.h"

int Victor_headset_installed=0;
int Victor_mode=0;

PORT * VictorPort = NULL;

/********************************************************************
 *                                                                  *
 *  Functions for changing low level VGA registers                  *
 *      (c) VictorMaxx Technologies, March 1994                     *
 *                                                                  *
 ********************************************************************/

// The structure containes all atributes for a VGA mode
typedef struct {              // VGA atributes
	unsigned char   MOR,      // Misccellaneous Output register
	FCR,      // Feature Control Register
	ISR0,     // Input Status Register 0
	ISR1,     // Input Status Register 1
	SR[5],
	CRCT[25]; // Catode Ray Tybe Controller registers
} VGAatr;

VGAatr VGA13 = { 0x27, 1, 0x10, 0x31,     // 320x200x256
		3, 9, 0xf, 0, 0xe,
		0x6b, 0x4f, 0x50, 0x8e,
		0x5c, 0x84, 0x4, 0x11,
		0, 0x40, 0, 0, 0, 0, 0,
		0, 0xdf, 0xa2, 0xc7, 0x28,
		0x40, 0xdc, 0xf0, 0xa3, 0xff};

VGAatr VGAi13 = {   0x63, 0x0, 0x10, 0x5,                   // 320x200x256
		0x3, 0x1, 0xf, 0x0, 0xe,
		0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0xbf, 0x1f, 0x0,
		0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9c, 0x8e, 0x8f,
		0x28, 0x40, 0x96, 0xb9, 0xa3, 0xff};
//*******************************************************************

// This function sets the data defined above into proper VGA registers
// in:  VGAatr structure
// out: none
void set_mode_atr(VGAatr *vga)
{
	unsigned char  i;
	
	outp( 0x3c2, vga->MOR );                        

	outp( 0x3c4, 0 ); 	
	outp( 0x3c5, 0 ); /* choose index 0, reset */
	for(i=1; i<=4; i++)     {
		outp( 0x3c4, i );  /* choose index i */
		outp( 0x3c5, vga->SR[i] );  /* choose index i */
	}

	outp( 0x3c4, 0 );
	outp( 0x3c5, 3 ); /* choose index 0 set 11b*/

	/* CRTC Registers */
	outp( 0x3d4, 0x11 ); /* WRITE index 11H */
	i = inp( 0x3d5 );
	outp( 0x3d5, i & 0x7f  ); /* ENABLE ACCESS TO THE REGISTERS */
	
	for(i=0; i<=7; i++)	{
		if(i!=1 && i!=2)        {
			outp( 0x3d4, i );
			outp( 0x3d5, vga->CRCT[i] );
		}
	}

	outp( 0x3d4, 0x11 );
	outp( 0x3d5, 128 ); /* DISABLE ACCESS TO 0-7 */
       
	for(i=9; i<23; i++)     {
		outp( 0x3d4, i );
		outp( 0x3d5, vga->CRCT[i] );
	}

}

void victor_init_graphics()	
{
	if (Victor_mode==0) {
		Victor_mode = 1;
		set_mode_atr(&VGA13);
	} else {
		Victor_mode = 0;
		set_mode_atr(&VGAi13);
	}
}

void victor_init_tracking(int serial_port)	
{
	if (Victor_headset_installed) return;

	printf( "%s %d\n", TXT_INIT_VICTOR, serial_port );

	if ( (serial_port < 1) || (serial_port > 4) )	{
		Error( "Invalid serial port parameter for -maxxtrak!\n" );
	}
	VictorPort = PortOpenGreenleafFast(serial_port-1, 19200, 'N', 8, 1 );
	if ( !VictorPort )	{
		printf( "%s %d\n", TXT_SERIAL_FAILURE, VictorPort->status );
		return;
	}
//	printf( "Port opened\n" );

	SetDtr( VictorPort, 1 );
//	printf( "Step #1\n" );
	SetRts( VictorPort, 1 );
//	printf( "Step #2\n" );
	UseRtsCts( VictorPort, 0 );
//	printf( "Step #3\n" );

	Victor_headset_installed = 1;
	atexit( victor_close_tracking );

 	WriteChar( VictorPort, 'A' );		// Absolute coordinate mode
 	WriteChar( VictorPort, 'C' );		// High accuracy mode
 	WriteChar( VictorPort, 'F' );		// Binary Mode
 	WriteChar( VictorPort, 'G' );		// Sampling upon request

	ClearRXBuffer( VictorPort );

 	WriteChar( VictorPort, 'S' );		// Request a packet

//	{ 
//		int k=0;
//		while ( k!=KEY_ESC )	{ 
//			fix y, p, r;
//			victor_read_headset_filtered( &y, &p, &r );
//			printf( "%d\t%d\t%d\n", y, p, r );
//			k = key_inkey();
//		}
//	}

}

void victor_close_tracking()	{
	if ( Victor_headset_installed )	{
		Victor_headset_installed = 0;
		VictorPort	 =  NULL;
	}
}

fix v_yaw = 0, v_pitch = 0, v_roll = 0;

void victor_read_headset_raw( fix *yaw, fix *pitch, fix *roll )
{
	int y, p, r, count;
	static unsigned char buff[6];
	ReadBufferTimed(VictorPort, buff, 6, 10);

	count = VictorPort->count;
	if ( count < 6 )	{
		ClearRXBuffer(VictorPort);
		WriteChar( VictorPort, 'S' );
		*yaw = v_yaw;
		*pitch = v_pitch;
		*roll = v_roll;
		return;
	}
	WriteChar( VictorPort, 'S' );

	y  =  (short)(buff[0] << 8) | buff[1];
	p  =  (short)(buff[2] << 8) | buff[3];
	r  =  (short)(buff[4] << 8) | buff[5];
	
	v_yaw = *yaw = y;
	v_pitch = *pitch = -1*(p - 16384);
	v_roll = *roll = -1*(r - 16384);
}


void victor_read_headset_filtered( fix *yaw, fix *pitch, fix *roll )
{
	int i;
	fix _yaw, _pitch, _roll;
	fix t_yaw, t_pitch, t_roll;
	fix last_yaw;

	t_yaw=t_pitch=t_roll=0;

	for (i=0; i<2; i++ )	{
		victor_read_headset_raw( &_yaw, &_pitch, &_roll );

		//if (i>0)
		//	if (last_yaw<i2f(90) && _yaw>i2f(270))
		//		_yaw -= i2f(360);
		//	else if (_yaw<i2f(90) && last_yaw>i2f(270))
		//		_yaw += i2f(360);

	 	t_yaw += _yaw; 
		t_pitch += _pitch;
		t_roll += _roll;

		last_yaw = _yaw;
	}
	
	*yaw = t_yaw / 2;
	*pitch = t_pitch / 2;
	*roll = t_roll / 2;

}

