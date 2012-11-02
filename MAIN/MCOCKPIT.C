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
 * $Source: f:/miner/source/main/rcs/mcockpit.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:30:20 $
 * 
 * File to make the cockpit.inc source code file.
 * 
 * $Log: mcockpit.c $
 * Revision 2.0  1995/02/27  11:30:20  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.8  1994/05/30  19:27:57  john
 * Moved functionality of mcockpit into 2d, ibitblt.
 * 
 * Revision 1.7  1994/05/11  11:36:08  john
 * Neatend up; took out debugging lines.
 * 
 * Revision 1.6  1994/05/11  11:19:19  john
 * Made cockpit code be created at runtime.
 * 
 * Revision 1.5  1994/01/28  17:40:22  john
 * *** empty log message ***
 * 
 * Revision 1.4  1994/01/26  18:14:10  john
 * Printed out x,y,w,h;
 * 
 * Revision 1.3  1994/01/25  11:45:00  john
 * Took out graphics.
 * 
 * Revision 1.2  1994/01/24  18:55:40  john
 * initial version.
 * 
 * Revision 1.1  1994/01/24  18:31:21  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: mcockpit.c 2.0 1995/02/27 11:30:20 john Exp $";
#pragma on (unreferenced)

/*
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "gr.h"
#include "iff.h"
#include "mem.h"
#include "screens.h"
#include "mcockpit.h"

#define MODE_NONE		0
#define MODE_SKIP		1
#define MODE_DRAW		2

#define OPCODE_ADD 			0x81			
#define OPCODE_ESI			0xC6			// Followed by a dword	(add esi, ????)
#define OPCODE_EDI		 	0xC7			// Followed by a dword  (add edi, ????)
#define OPCODE_MOV_ECX		0xB9			// Followed by a dword  (mov ecx,????)
#define OPCODE_MOVSB   		0xA4			// movsb
#define OPCODE_16BIT 		0x66			// movsw
#define OPCODE_MOVSD 		0xA5			// movsd
#define OPCODE_REP			0xF3			// rep
#define OPCODE_RET			0xC3			// ret

ubyte *Code_pointer = NULL;
int Code_counter = 0;

void move_and_count( int dsource, int ddest, int ecx )
{
	int blocks;
	if ( ecx <= 0 )	
		return;

	if ( dsource > 0 )
		Code_counter+=6;		// ADD ESI, dsource

	if ( ddest > 0 )
		Code_counter+=6;		// ADD EDI, ddest

	while ( ecx > 0 )	{
		switch(ecx)	{
		case 1: Code_counter++; ecx = 0; break;	// MOVSB
		case 2: Code_counter+=2; ecx = 0; break;	// MOVSW
		case 3: Code_counter+=3; ecx = 0; break;	// MOVSW, MOVSB
		case 4: Code_counter++; ecx = 0; break; 	// MOVSD
		default:
			blocks = ecx / 4;
			if ( blocks == 1 )
				Code_counter++;	// MOVSD
			else 
				Code_counter+=7;
			ecx -= blocks*4;
		}
	}
}


void move_and_draw( int dsource, int ddest, int ecx )
{
	int blocks;
	int * iptr;

	if ( ecx <= 0 )
		return;

	if ( dsource > 0 )	{
		// ADD ESI, dsource
		*Code_pointer++ = OPCODE_ADD;
		*Code_pointer++ = OPCODE_ESI;
		iptr = (int *)Code_pointer;
		*iptr++ = dsource;
		Code_pointer = (ubyte *)iptr;						
	}
	if ( ddest > 0 )	{
		// ADD EDI, ddest
		*Code_pointer++ = OPCODE_ADD;
		*Code_pointer++ = OPCODE_EDI;
		iptr = (int *)Code_pointer;
		*iptr++ = ddest;
		Code_pointer = (ubyte *)iptr;						
	}

	while ( ecx > 0 )	{
		switch( ecx )	{
		case 1:
			// MOVSB
			*Code_pointer++ = OPCODE_MOVSB;
			ecx = 0;
			break;
		case 2:
			// MOVSW
			*Code_pointer++ = OPCODE_16BIT;
			*Code_pointer++ = OPCODE_MOVSD;
			ecx = 0;
			break;
		case 3:
			// MOVSW, MOVSB
			*Code_pointer++ = OPCODE_16BIT;
			*Code_pointer++ = OPCODE_MOVSD;
			*Code_pointer++ = OPCODE_MOVSB;
			ecx = 0;
			break;
		case 4:
			// MOVSD
			*Code_pointer++ = OPCODE_MOVSD;
			ecx = 0;
			break;
		default:
			blocks = ecx / 4;

			if ( blocks == 1 )	{	
				// MOVSD
				*Code_pointer++ = OPCODE_MOVSD;
			} else {
				// MOV ECX, blocks
				*Code_pointer++ = OPCODE_MOV_ECX;
				iptr = (int *)Code_pointer;
				*iptr++ = blocks;
				Code_pointer = (ubyte *)iptr;						
				// REP MOVSD
				*Code_pointer++ = OPCODE_REP;
				*Code_pointer++ = OPCODE_MOVSD;
			}
			ecx -= blocks*4;
		}
	}
}

//-----------------------------------------------------------------------------------------
// Given bitmap, bmp, finds the size of the code 

int mcockpit_find_inverted_bitblit_code_size( grs_bitmap * bmp, int sx, int sy, int sw, int sh, int srowsize, ubyte transparent_color )
{
	int x,y;
	ubyte pixel;
	int draw_mode = MODE_NONE;
	int source_offset = 0;
	int dest_offset = 0;
	int num_to_draw, draw_start_source, draw_start_dest;
	int esi, edi;

	Code_counter = 0;

	esi = source_offset = 0;

	for ( y=sy; y<sy+sh; y++ )	{
		for ( x=sx; x<sx+sw; x++ )	{
			dest_offset = y*bmp->bm_rowsize+x;
			pixel = bmp->bm_data[dest_offset];
			if ( pixel!=transparent_color )	{
				switch ( draw_mode)	{
				case MODE_DRAW:
					move_and_count( draw_start_source-esi, draw_start_dest-edi, num_to_draw );
					esi = draw_start_source + num_to_draw;
					edi = draw_start_dest + num_to_draw;
					// fall through!!!
				case MODE_NONE:
				case MODE_SKIP:
					break;
				}
				draw_mode = MODE_SKIP;
			} else {
				switch ( draw_mode)	{
				case MODE_SKIP:
				case MODE_NONE:
					draw_start_source = source_offset;
					draw_start_dest = dest_offset;
					num_to_draw = 0;
					// fall through
				case MODE_DRAW:
					num_to_draw++;
					break;
				}
				draw_mode = MODE_DRAW;
			}
			source_offset++;
		}
		if ( draw_mode == MODE_DRAW )	{
			move_and_count( draw_start_source-esi, draw_start_dest-edi, num_to_draw );
			esi = draw_start_source + num_to_draw;
			edi = draw_start_dest + num_to_draw;
		}
		draw_mode = MODE_NONE;
		source_offset += (srowsize - sw);
	}
	Code_counter++;		// for return

	//printf( "Code will be %d bytes\n", Code_counter );

	return Code_counter;
}


//-----------------------------------------------------------------------------------------
// Given bitmap, bmp, create code that transfers a bitmap of size sw*sh to position
// (sx,sy) on top of bmp, only overwritting transparent pixels of the bitmap.

int mcockpit_create_inverted_bitblit( grs_bitmap * bmp, int sx, int sy, int sw, int sh, int srowsize, ubyte transparent_color, ubyte * code )
{
	int x,y;
	ubyte pixel;
	int draw_mode = MODE_NONE;
	int source_offset = 0;
	int dest_offset = 0;
	int num_to_draw, draw_start_source, draw_start_dest;
	int esi, edi;

	Code_pointer = code;

	esi = source_offset = 0;

	for ( y=sy; y<sy+sh; y++ )	{
		for ( x=sx; x<sx+sw; x++ )	{
			dest_offset = y*bmp->bm_rowsize+x;
			pixel = bmp->bm_data[dest_offset];
			if ( pixel!=transparent_color )	{
				switch ( draw_mode)	{
				case MODE_DRAW:
					move_and_draw( draw_start_source-esi, draw_start_dest-edi, num_to_draw );
					esi = draw_start_source + num_to_draw;
					edi = draw_start_dest + num_to_draw;
					// fall through!!!
				case MODE_NONE:
				case MODE_SKIP:
					break;
				}
				draw_mode = MODE_SKIP;
			} else {
				switch ( draw_mode)	{
				case MODE_SKIP:
				case MODE_NONE:
					draw_start_source = source_offset;
					draw_start_dest = dest_offset;
					num_to_draw = 0;
					// fall through
				case MODE_DRAW:
					num_to_draw++;
					break;
				}
				draw_mode = MODE_DRAW;
			}
			source_offset++;
		}
		if ( draw_mode == MODE_DRAW )	{
			move_and_draw( draw_start_source-esi, draw_start_dest-edi, num_to_draw );
			esi = draw_start_source + num_to_draw;
			edi = draw_start_dest + num_to_draw;
		}
		draw_mode = MODE_NONE;
		source_offset += (srowsize - sw);
	}
	*Code_pointer++ = OPCODE_RET;

	//printf( "Code is %d bytes\n", Code_pointer - code );

	return 0;
}


void mcockpit_do_inverted_bitblit_asm(char *start_si, char *start_di, ubyte * code);
#pragma aux mcockpit_do_inverted_bitblit_asm parm [esi] [edi] [eax] modify [ecx edi esi eax] = \
	"pusha"	\
	"call	eax"	\
	"popa"

void mcockpit_do_inverted_bitblit(char *start_si, char *start_di, ubyte * code)
{
	if (code != NULL )
		mcockpit_do_inverted_bitblit_asm( start_si, start_di, code );
}


void mcockpit_find_extents( grs_bitmap * bmp, int *minx, int *miny, int *maxx, int *maxy )
{
	ubyte c;
	int x, y;

	*minx = 0;
	*maxx = bmp->bm_w;
	*miny = 0;
	*maxy = bmp->bm_h;

	for ( y=0; y<bmp->bm_h; y++ )
		for ( x=0; x<bmp->bm_w; x++ )	{
			c = bmp->bm_data[bmp->bm_rowsize*y+x];
			if (c == iff_transparent_color )	{
				if ( x < *minx ) *minx = x;
				if ( y < *miny ) *miny = y;
				if ( x > *maxx ) *maxx = x;
				if ( y > *maxy ) *maxy = y;
			}
		}
}

*/

