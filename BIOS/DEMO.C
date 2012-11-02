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
#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>

#include "types.h"
#include "timer.h"
#include "mono.h"
#include "ipx.h"
#include "error.h"
#include "dpmi.h"
#include "key.h"

char temp1[512];


void stuff_key( ushort code )
{
	ushort * buffer = (ushort *)0x41e;
	ushort * ptr = (ushort *)0x41c;
	buffer[*ptr] = code << 8;
	*ptr = *ptr + 1;
	if ( *ptr > 15 ) *ptr = 0;
}

void _far my_timer()
{
	char c;
	int k, size = ipx_get_packet_data( temp1 );

	if ( size > 0 )	{
		stuff_key( temp1[0] );
	}
	
	k = key_inkey();
	c = key_to_ascii(k);

	if ( c < 255 )	{
		stuff_key( c );
		temp1[0] = c;
		ipx_send_broadcast_packet_data( temp1, 1 );
	}

}

void main()
{
	char c;
	int i, k,size;
	char temp[100];
	char temp1[512];
	
	setbuf(stdout, NULL);	// unbuffered output via printf
	dpmi_init(0);
	minit();
	mopen( 0, 9, 1, 78, 15, "Debug Spew");
	mopen( 1, 2, 1, 78,  5, "Errors & Serious Warnings");
	key_init();
	timer_init();

	ipx_init( IPX_DEFAULT_SOCKET-10, 1 );

	ipx_read_user_file( "descent.usr" );
	ipx_read_network_file( "descent.net" );

	timer_set_function( my_timer );

	printf( "\nType something and then hit Enter to send (Esc Quits)...\n" );

	system( "4dos" );
}


