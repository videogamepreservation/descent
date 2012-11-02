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
 * $Source: f:/miner/source/main/rcs/nfile.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:31:27 $
 * 
 * Routines to read from a big file.
 * 
 * $Log: nfile.c $
 * Revision 2.0  1995/02/27  11:31:27  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.2  1994/12/09  19:20:49  john
 * *** empty log message ***
 * 
 * Revision 1.1  1994/12/07  20:54:27  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: nfile.c 2.0 1995/02/27 11:31:27 john Exp $";
#pragma on (unreferenced)

#include <stdlib.h>

static nfile_initialized = 0;

void nfile_close()
{
	if ( !nfile_initialized ) return;
	nfile_initialized = 0;
}

void nfile_init()
{
	if ( nfile_initialized ) return;
	nfile_initialized = 1;
	
	atexit( nfile_close );

	// Init....
}

FILE * nfopen( char * name, char * mode )
{
	FILE * fp;
	fp = fopen( name, mode );
	return fp;
}

int nfilelength( char * name )
{

}



