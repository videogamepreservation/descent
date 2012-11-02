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
 * $Source: f:/miner/source/pslib/rcs/cflib.h $
 * $Revision: 1.13 $
 * $Author: john $
 * $Date: 1994/02/15 12:53:06 $
 *
 * Sample setup for RCS header
 *
 * $Log: cflib.h $
 * Revision 1.13  1994/02/15  12:53:06  john
 * Made numfiles stored in library be a short instead of char.
 * 
 * Revision 1.12  1993/12/08  17:06:34  yuan
 * Added CheckFile.
 * 
 * Revision 1.11  1993/10/28  15:16:54  yuan
 * Added WriteFile and AppendFile prototypes.
 * 
 * Revision 1.10  1993/10/28  14:41:13  yuan
 * New cfread functions added to cflib.
 * 
 * Revision 1.9  1993/10/27  12:41:58  yuan
 * Added prototype for ReadFileBuf.
 * 
 * Revision 1.8  1993/10/19  12:39:13  matt
 * Added '#ifndef' around body of file
 * Added new error codes
 * Included readfile.h
 * 
 * Revision 1.7  1993/09/29  17:53:38  yuan
 * lzw_compression and lzw_expand documented more clearly (hopefully)
 * some error flags added.
 * 
 * Revision 1.6  1993/09/27  17:12:21  yuan
 * Documentation for using lib_init(), lib_close(), and LibReadFile() added.
 * 
 * Revision 1.5  1993/09/21  17:22:22  yuan
 * *** empty log message ***
 * 
 * Revision 1.4  1993/09/21  17:16:56  yuan
 * cleaning up
 * 
 * Revision 1.3  1993/09/21  17:07:01  yuan
 * broken and unbroken
 * 
 * Revision 1.2  1993/09/20  12:26:25  yuan
 * ReadFile, WriteFile, AppendFile removed to readfile.h
 * 
 * Revision 1.1  1993/09/14  13:15:30  yuan
 * Initial revision
 * 
 * Revision 1.3  1993/09/09  15:41:22  yuan
 * pslib.exe docs removed.
 * 
 * Revision 1.2  1993/09/09  12:40:03  yuan
 * compression routine prototypes from library.h added.
 * 
 * Revision 1.1  1993/09/08  16:16:57  yuan
 * Initial revision
 * 
 *

        PSLIB is a system for librarying files.  We can combine data
        files into one library, saving disk space and simplifying
        distribution of demos, etc.

        The system consists of two parts, a program to create libraries
        and a routine to read from the libraries.


        The old way:
        -----------

        In order to extract the file from the library, the extract function is
        called.  The format is as follows:

        extract ( <name of library to extract files from>,
                  <filename to extract>,
                  <buffer to extract to> );

        The improved and current method:
        -------------------------------

        In order to extract a file from the library, we first initialize
        our library file with lib_init(filename);

        This opens our library file, verifies that it is a PSLB file,
        and reads the headers into a global array.

        LibReadFile() reads the desired filename from the library and returns
        the data in a buffer.  This is identical in format to the
        function ReadFile() in readfile.h.

        Once the library is opened, we can call LibReadFile() multiple
        times.

        After all the data files desired are read lib_close() is called
        to close our library file.

*/

#ifndef _CFLIB_H
#define _CFLIB_H

#include "types.h"

//#include "readfile.h"   //get prototypes

// error codes for lib_init()

#define LI_NO_ERROR		0		//opened just fine
#define LI_NOT_PSLIB		1		//the file isn't one of our libraries
#define LI_NO_FILE		2		//can't find requested file
#define LI_NO_MEM			3		//couldn't allocate memory for file index

int lib_init( char *lib_name );

void lib_close();

ubyte *ReadFile( char *filename, int *length );

int ReadFileBuf( char *filename, byte *buf, int bufsize );
// ReadFileBuf reads bufize bytes of 'filename' into the address of 'buf'
//  returns an error code < 0 if there is an error.

ubyte *extract( char *library, char *filename );

// lzw function prototypes

ubyte *lzw_expand( ubyte *inputbuf, ubyte *outputbuf, int length );
// lzw_expand can be called in one of two ways :
// 1) If the address of a buffer is passed to outputbuf, then the expanded
//      data of the input is passed thru outputbuf.
// 2) Otherwise, if NULL is passed to outputbuf, then a buffer is malloc'd and
//      returned.

ubyte *lzw_compress( ubyte *inputbuf, ubyte *outputbuf, int input_size, int *output_size );
// lzw_compress can be called in one of two ways :
// 1) If the address of a buffer is passed to outputbuf, then the compressed
//      data of the input is passed thru outputbuf.
// 2) Otherwise, if NULL is passed to outputbuf, then a buffer is malloc'd and
//      returned.

ubyte *cfreadfile( char *filename, int *size );
// cfreadfile reads a "cfwrite" file and returns the lzw expanded buffer.

int cfwritefile( char *filename, ubyte *buffer, int length );
// cfwrite takes a buffer 'buffer', lzw compresses it, and writes it to a "cfwrite" file.

int WriteFile( char *filename, void *data, int length );
// WriteFile writes 'length' bytes of 'data' to 'filename'
//  returns an error code != 0 if there is an error.

int AppendFile( char *filename, void *data, int length );
// AppendFile appends 'length' bytes of 'data' to 'filename'
//  returns an error code != 0 if there is an error.

int CheckFile( char *filename );
// Returns status of a file
//		0 - not found.
//		1 - In local directory.
//		2 - In opened library file

#endif


