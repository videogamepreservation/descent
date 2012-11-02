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
 * $Source: f:/miner/source/2d/rcs/ibitblt.c $
 * $Revision: 1.6 $
 * $Author: john $
 * $Date: 1994/11/28 17:07:29 $
 * 
 * Rountines to copy a bitmap on top of another bitmap, but
 * only copying to pixels that are transparent.
 * 
 * $Log: ibitblt.c $
 * Revision 1.6  1994/11/28  17:07:29  john
 * Took out some unused functions in linear.asm, moved
 * gr_linear_movsd from linear.asm to bitblt.c, made sure that
 * the code in ibiblt.c sets the direction flags before rep movsing.
 * 
 * Revision 1.5  1994/11/18  22:50:22  john
 * Changed shorts to ints in parameters.
 * 
 * Revision 1.4  1994/11/09  16:35:16  john
 * First version with working RLE bitmaps.
 * 
 * Revision 1.3  1994/10/03  17:18:05  john
 * Fixed bug with edi not getting intialized to zero
 * in create_mask.
 * 
 * Revision 1.2  1994/05/31  11:10:55  john
 * *** empty log message ***
 * 
 * Revision 1.1  1994/05/30  16:08:27  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: ibitblt.c 1.6 1994/11/28 17:07:29 john Exp $";
#pragma on (unreferenced)

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "gr.h"
#include "mem.h"
#include "error.h"
#include "ibitblt.h"

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

int gr_ibitblt_find_code_size( grs_bitmap * mask_bmp, int sx, int sy, int sw, int sh, int srowsize )
{
	int x,y;
	ubyte pixel;
	int draw_mode = MODE_NONE;
	int source_offset = 0;
	int dest_offset = 0;
	int num_to_draw, draw_start_source, draw_start_dest;
	int esi, edi;

	Assert( (!(mask_bmp->bm_flags&BM_FLAG_RLE)) );

	Code_counter = 0;

	esi = source_offset = 0;
	edi = dest_offset = 0;
	draw_start_source = draw_start_dest = 0;

	for ( y=sy; y<sy+sh; y++ )	{
		for ( x=sx; x<sx+sw; x++ )	{
			dest_offset = y*mask_bmp->bm_rowsize+x;
			pixel = mask_bmp->bm_data[dest_offset];
			if ( pixel!=255 )	{
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

	Code_counter += 16;	// for safety

	return Code_counter;
}


//-----------------------------------------------------------------------------------------
// Given bitmap, bmp, create code that transfers a bitmap of size sw*sh to position
// (sx,sy) on top of bmp, only overwritting transparent pixels of the bitmap.

ubyte	*gr_ibitblt_create_mask( grs_bitmap * mask_bmp, int sx, int sy, int sw, int sh, int srowsize )
{
	int x,y;
	ubyte pixel;
	int draw_mode = MODE_NONE;
	int source_offset = 0;
	int dest_offset = 0;
	int num_to_draw, draw_start_source, draw_start_dest;
	int esi, edi;
	int code_size;
	ubyte *code;

	Assert( (!(mask_bmp->bm_flags&BM_FLAG_RLE)) );

	code_size = gr_ibitblt_find_code_size( mask_bmp, sx, sy, sw, sh, srowsize );

	code = malloc( code_size );
	if ( code == NULL )
		return NULL;

	Code_pointer = code;

	esi = source_offset = 0;
	edi = dest_offset = 0;
	draw_start_source = draw_start_dest = 0;

	for ( y=sy; y<sy+sh; y++ )	{
		for ( x=sx; x<sx+sw; x++ )	{
			dest_offset = y*mask_bmp->bm_rowsize+x;
			pixel = mask_bmp->bm_data[dest_offset];
			if ( pixel!=255 )	{
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

	if ( Code_pointer >= &code[code_size-1] )	
		Error( "ibitblt overwrote allocated code block\n" );

	//printf( "Code is %d bytes\n", Code_pointer - code );

	return code;
}


void gr_ibitblt_do_asm(char *start_si, char *start_di, ubyte * code);
#pragma aux gr_ibitblt_do_asm parm [esi] [edi] [eax] modify [ecx edi esi eax] = \
	"pusha"	\
	"cld"		\
	"call	eax"	\
	"popa"


void gr_ibitblt(grs_bitmap * source_bmp, grs_bitmap * dest_bmp, ubyte * mask )
{
	if (mask != NULL )
		gr_ibitblt_do_asm( source_bmp->bm_data, dest_bmp->bm_data, mask );
}


void	gr_ibitblt_find_hole_size( grs_bitmap * mask_bmp, int *minx, int *miny, int *maxx, int *maxy )
{
	ubyte c;
	int x, y, count=0;

	Assert( (!(mask_bmp->bm_flags&BM_FLAG_RLE)) );

	*minx = mask_bmp->bm_w-1;
 	*maxx = 0;
	*miny = mask_bmp->bm_h-1;
	*maxy = 0;

	for ( y=0; y<mask_bmp->bm_h; y++ )
		for ( x=0; x<mask_bmp->bm_w; x++ )	{
			c = mask_bmp->bm_data[mask_bmp->bm_rowsize*y+x];
			if (c == 255 )	{
				if ( x < *minx ) *minx = x;
				if ( y < *miny ) *miny = y;
				if ( x > *maxx ) *maxx = x;
				if ( y > *maxy ) *maxy = y;
				count++;
			}
		}

	if ( count == 0 )	{
		Error( "Bitmap for ibitblt doesn't have transparency!\n" );
	}
}

