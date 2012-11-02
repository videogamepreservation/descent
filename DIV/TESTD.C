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
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "div0.h"

extern void correct();
extern int testdivide();
extern void div0_my_add();
extern void setup_correct();

void correct_c()
{
	printf( "Divide by zero error, man!\n" );
}


main()
{   int x,y,z;

	printf( "\n\n\n\n" );

	if (!div0_init( DM_SATURATE )) {
		printf( "Error initializing Divide By Zero Exception.\n" );
		exit(1);
	}

	z = 0;
	printf( "The following should saturate on divide by zero.\n" );
	printf( "A result of 3 (ie... 6 2) will continue...\n" );

	while(z != 3)
	{
		printf( "Enter two integers: " );
		scanf( "%d %d", &x, &y );
		z = x / y;
		printf( "%d / %d = %d\n", x, y, z );
	}


	//div0_set_handler( div0_my_add, correct );
	setup_correct();

	z = testdivide();

	printf( ".\n.\n.This should equal 15 (from TESTDA.ASM): %d\n", z );


	printf( "\nPress any key...\n" );
	getch();


	div0_set_mode( DM_ERROR );

	printf( "\nNow, you should see the Watcom runtime error message...\n" );

	printf( "\nPress any key...\n" );
	getch();

	x = y = 0;
	z = x / y;

	printf( "Z=%d\n", z );

	div0_close();


}


