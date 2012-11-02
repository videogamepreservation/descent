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
 * TESTK.C - Keyboard handler testing routines
 */

#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include "key.h"
#include "timer.h"

char * vidmem = (char *)0xb8000;

void main (void)
{
	char ascii;
	int c;

	setbuf( stdout, NULL );

	key_init();
	//timer_init( 0, NULL );
	//keyd_buffer_type = 2;
	keyd_repeat = 0;

	printf( "\n\n\n\nThis tests the keyboard library.\n\n" );
	printf( "Press any key to start...\n" );
	printf( "You pressed: %c\n\n", key_getch() );
	printf( "Press F1 to turn off buffering.\n" );
	printf( "      F2 to turn on buffering.\n" );
	printf( "      F4 to flush keyboard.\n" );
	printf( "      F5 to turn repeat off.\n");
	printf( "      F6 to turn repeat on.\n");
	printf( "      F7 to do an INT 3.\n" );
	printf( "      F10 to display some boxes.\n" );
	printf( "      The arrows to see fast multiple keystrokes.\n");
	printf( "      ESC to exit.\n\n" );


	while( !keyd_pressed[KEY_ESC]  ) {
		int i,j;

		for (i=0; i<256; i++ )	{
			if (keyd_pressed[i])		{
				j = key_to_ascii(i);
				if (j==255)
					vidmem[i*2] = 219;
				else
					vidmem[i*2] = j;
			} else 
				vidmem[i*2] = 32;
		}

		if (keyd_pressed[KEY_F1])
			keyd_buffer_type = 0;

		if (keyd_pressed[KEY_F2])
			keyd_buffer_type = 1;

		if (keyd_pressed[KEY_F4])
			key_flush();

		if (keyd_pressed[KEY_F5])	{
			keyd_repeat = 0;
			printf( "Repeat off" );
		}

		if (keyd_pressed[KEY_F6])	{
			keyd_repeat = 1;
			printf( "Repeat on" );
		}

//		if (keyd_pressed[KEY_F7] )
//			key_debug();

		if (keyd_pressed[KEY_PAUSE])
			putch( 254 );

		if (key_checkch())   {
			c = key_getch();

			ascii=key_to_ascii(c);
			if ( ascii==255 )
			{
				printf("[%4X] ", c );
				fflush( stdout );
			}
			else
				putch( ascii );

			if (c==1) break;
		}

		delay(100);

	}

	key_close();
	//timer_close();
}

