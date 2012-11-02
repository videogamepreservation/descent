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
 * $Source: h:/miner/source/compress/RCS/test.c $
 * $Revision: 1.1 $
 * $Author: john $
 * $Date: 1994/02/10 15:54:38 $
 * 
 * Test program...
 * 
 * $Log: test.c $
 * Revision 1.1  1994/02/10  15:54:38  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: test.c 1.1 1994/02/10 15:54:38 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "cfile.h"
#include "cflib.h"

int main(int argc, char * argv[] )
{	
	int size, size1;
	CFILE * cfile;
	void * buffer;

	lib_init( "JOHN.LIB" );

	if (strcmpi( argv[1], "x" )==0)	{
		// extract argv[2] into argv[3];
		printf( "Uncompressing %s into %s\n", argv[2], argv[3] );

		cfile = cfopen(argv[2], "rc" );
		size = cfilelength( cfile );
		buffer = malloc(size);
		size1 = cfread( buffer, 1, size, cfile );
		printf( "File expanded to %d bytes (Orginal was %d bytes)\n", size1, size );
		cfclose(cfile);

		cfile = cfopen( argv[3], "wb" );
		cfwrite( buffer, 1, size, cfile );
		cfclose( cfile );
		free(buffer);

	} else if (strcmpi( argv[1], "c" )==0)	{

		// compress argv[2] into argv[3];
		printf( "Compressing %s into %s\n", argv[2], argv[3] );
		cfile = cfopen( argv[2], "rb" );
		size = cfilelength( cfile );
		printf( "File is %d bytes big\n", size );
		buffer = malloc(size);
		size1 = cfread( buffer, 1, size, cfile );
		printf( "Read in %d bytes\n", size1 );
		cfclose(cfile);

		cfile = cfopen( argv[3], "wc" );
		if ((size1=cfwrite( buffer, 1, size, cfile ))!=size)
			printf( "Error! Only wrote %d bytes\n", size1 );
		cfclose( cfile );
		free(buffer);
	}
}

