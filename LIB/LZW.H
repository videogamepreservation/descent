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
 * $Source: f:/miner/source/lzw/rcs/lzw.h $
 * $Revision: 1.2 $
 * $Author: john $
 * $Date: 1994/08/05 16:10:23 $
 * 
 * Routines for lzw compressing some bits.
 * 
 * $Log: lzw.h $
 * Revision 1.2  1994/08/05  16:10:23  john
 * First version.
 * 
 * Revision 1.1  1994/07/29  11:02:14  john
 * Initial revision
 * 
 * 
 */



#ifndef _LZW_H
#define _LZW_H

// Compresses inputbuf into output buf... Returns size of output buffer.
int new_lzw_compress( ubyte *outputbuf, ubyte *inputbuf, int input_size );

// Decompresses inputbuf into output buf... Returns size of output buiffer.
int new_lzw_expand( ubyte *outputbuf, ubyte *inputbuf );

#endif

