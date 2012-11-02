/*
 * $Source: f:/miner/source/2d/rcs/pcx.h $
 * $Revision: 1.4 $
 * $Author: john $
 * $Date: 1995/01/21 17:07:34 $
 * 
 * Routines to read/write pcx images.
 * 
 * $Log: pcx.h $
 * Revision 1.4  1995/01/21  17:07:34  john
 * Added out of memory error.
 * 
 * Revision 1.3  1994/11/29  02:53:10  john
 * Added error messages; made call be more similiar to iff.
 * 
 * Revision 1.2  1994/11/28  20:03:48  john
 * Added PCX functions.
 * 
 * Revision 1.1  1994/11/28  19:57:45  john
 * Initial revision
 * 
 * 
 */



#ifndef _PCX_H
#define _PCX_H

#define PCX_ERROR_NONE 				0
#define PCX_ERROR_OPENING			1
#define PCX_ERROR_NO_HEADER		2
#define PCX_ERROR_WRONG_VERSION	3
#define PCX_ERROR_READING			4
#define PCX_ERROR_NO_PALETTE		5
#define PCX_ERROR_WRITING			6
#define PCX_ERROR_MEMORY			7

// Reads filename into bitmap bmp, and fills in palette.  If bmp->bm_data==NULL, 
// then bmp->bm_data is allocated and the w,h are filled.  
// If palette==NULL the palette isn't read in.  Returns error code.

extern int pcx_read_bitmap( char * filename, grs_bitmap * bmp, int bitmap_type, ubyte * palette );

// Writes the bitmap bmp to filename, using palette. Returns error code.

extern int pcx_write_bitmap( char * filename, grs_bitmap * bmp, ubyte * palette );

extern char *pcx_errormsg(int error_number);

#endif
