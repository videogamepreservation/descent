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
 * $Source: f:/miner/source/pslib/rcs/readfile.h $
 * $Revision: 1.5 $
 * $Author: matt $
 * $Date: 1993/10/19 12:38:14 $
 *
 * 
 *
 */

#ifndef _READFILE_H
#define _READFILE_H

#include "types.h"

#define ERROR_OPENING_FILE  21
#define ERROR_WRITING_FILE  22
#define ERROR_READING_DATA -20

void *ReadFile( char *filename, int *length );
// ReadFile reads 'filename' and returns the buffer and passes the length
// in bytes.

int WriteFile( char *filename, void *data, int length );
// WriteFile writes 'length' bytes of 'data' to 'filename'
//  returns an error code != 0 if there is an error.

int AppendFile( char *filename, void *data, int length );
// AppendFile appends 'length' bytes of 'data' to 'filename'
//  returns an error code != 0 if there is an error.

int ReadFileBuf( char *filename, byte *buf, int bufsize );
// ReadFileBuf reads bufize bytes of 'filename' into the address of 'buf'
//  returns an error code < 0 if there is an error.

#endif

