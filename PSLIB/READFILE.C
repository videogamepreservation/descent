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
 * $Source: f:/miner/source/pslib/rcs/readfile.c $
 * $Revision: 1.8 $
 * $Author: yuan $
 * $Date: 1993/10/22 17:50:18 $
 *
 * ReadFileRaw, WriteFile, AppendFile, ReadFileBuf
 * File and buffer i/o routines.
 *
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: readfile.c 1.8 1993/10/22 17:50:18 yuan Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <errno.h>
#include <string.h>

#include "readfile.h"
#include "error.h"
#include "mem.h"
//#include "mem2.h"

#define PACIFIER_COUNT 2047

void *ReadFile( char *filename, int *length )
{
    void *FileData;
    int handle;

	handle = open( filename, O_RDONLY | O_BINARY );
	if (handle == -1 )
        Error("File %s, %s ",filename,strerror(errno)); 

    *length = filelength( handle );

    //FileData = (void * )malloc( *length );
    MALLOC( FileData, ubyte, *length );

    if (FileData == NULL )  {
        close( handle );
        Error("File %s, cannot malloc memory",filename);
   }

    if (read( handle, FileData, *length ) != *length )    {
		free( FileData );
		close( handle );
      Error("File %s, %s ",filename,strerror(errno)); 
	}
	close( handle );

	return FileData;

}

int AppendFile( char *filename, void *data, int length )
{
	int handle;

    handle = open( filename, O_WRONLY | O_CREAT | O_APPEND | O_BINARY , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
	if (handle == -1 ) {
        return ERROR_OPENING_FILE;
	}
    if (write( handle, data, length )!=length)  {
		close( handle );
        return ERROR_WRITING_FILE;
	}

	close( handle );
    return 0;
}

int WriteFile( char *filename, void *data, int length )
{
	int handle;

	handle = open( filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
	if (handle == -1 ) {
        return ERROR_OPENING_FILE;
	}
	if (write( handle, data, length )!=length)  {
		close( handle );
        return ERROR_WRITING_FILE;
	}

	close( handle );
    return 0;
}

int ReadFileBuf( char *filename, byte *buf, int bufsize )
{
    int length;
    int handle;

	handle = open( filename, O_RDONLY | O_BINARY );
	if (handle == -1 ) {
        return ERROR_OPENING_FILE;
	}

    if (length = read( handle, buf, bufsize ) != bufsize )    {
		close( handle );
        return ERROR_READING_DATA;
	}
	close( handle );

    return length;
}
