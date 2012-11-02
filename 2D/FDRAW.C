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
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#include "gr.h"
#include "key.h"
#include "timer.h"
#include "grdef.h"
#include "mono.h"
#include "fix.h"
#include "iff.h"
#include "palette.h"
#include "rle.h"
#include "pcx.h"

main(int argc, char * argv[] )
{
	int y;
	FILE * fp;
	char line[200];
	grs_font * font;
	
	if ( argc < 2 )	{
		printf( "Usage: fdraw fontfile textfile outfile.pcx\n" );
		printf( " example: fdraw font3-1.fnt config.sys config.pcx\n" );
		printf( "Requires PALETTE.256 to be in current directory\n" );
		exit(1);
	}

	fp = fopen( argv[2], "rt" );
	if ( fp == NULL )	{
		printf( "Error opening text file '%s'\n", argv[2] );
		exit(1);
	}

	gr_init( SM_320x200C );
   gr_use_palette_table( "PALETTE.256" );
	gr_palette_load( gr_palette );

	font = gr_init_font( argv[1] );
	gr_set_fontcolor( gr_getcolor( 0,63,0), gr_getcolor(0,0,0) );
	if ( font == NULL )	{
		printf( "Error opening font file '%s'\n", argv[1] );
		exit(1);
	}

	y = 0;
	while (fgets(line, 200, fp)) {
		gr_string( 0, y, line );
		y += font->ft_h+1;
	}

	getch();

	pcx_write_bitmap( argv[3], &grd_curcanv->cv_bitmap, gr_palette );
}



