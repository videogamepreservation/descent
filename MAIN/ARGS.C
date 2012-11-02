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
 * $Source: f:/miner/source/main/rcs/args.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:31:22 $
 * 
 * Functions for accessing arguments.
 * 
 * $Log: args.c $
 * Revision 2.0  1995/02/27  11:31:22  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.9  1994/11/29  01:07:57  john
 * Took out some unused vars.
 * 
 * Revision 1.8  1994/11/29  01:04:30  john
 * Took out descent.ini stuff.
 * 
 * Revision 1.7  1994/09/20  19:29:15  matt
 * Made args require exact (not substring), though still case insensitive.
 * 
 * Revision 1.6  1994/07/25  12:33:11  john
 * Network "pinging" in.
 * 
 * Revision 1.5  1994/06/17  18:07:50  matt
 * Took out printf
 * 
 * Revision 1.4  1994/05/11  19:45:33  john
 * *** empty log message ***
 * 
 * Revision 1.3  1994/05/11  18:42:11  john
 * Added Descent.ini config file.
 * 
 * Revision 1.2  1994/05/09  17:03:30  john
 * Split command line parameters into arg.c and arg.h.
 * Also added /dma, /port, /irq to digi.c
 * 
 * Revision 1.1  1994/05/09  16:49:11  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: args.c 2.0 1995/02/27 11:31:22 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Num_args=0;
char * Args[100];

int FindArg( char * s )	{
	int i;

	for (i=0; i<Num_args; i++ )
		if (! stricmp( Args[i], s))
			return i;

	return 0;
}

void InitArgs( int argc,char **argv )	{
	int i;
//NO_DESCENT_INI	FILE * fp;
//NO_DESCENT_INI	char line[100];

	Num_args=0;

	for (i=0; i<argc; i++ )	{
		Args[Num_args++] = strdup( argv[i] );
	}

//NO_DESCENT_INI	fp = fopen( "descent.ini", "rt" );		
//NO_DESCENT_INI	if (fp!=NULL) {
//NO_DESCENT_INI		while( fgets( line, 100, fp ) != NULL )	{
//NO_DESCENT_INI			char *p;
//NO_DESCENT_INI			p = strchr(line,';');
//NO_DESCENT_INI			if (p) *p = '\0';		// Remove comments
//NO_DESCENT_INI			p = strchr(line,10);
//NO_DESCENT_INI			if (p) *p = '\0';		// Remove EOL
//NO_DESCENT_INI			p = strchr(line,13);
//NO_DESCENT_INI			if (p) *p = '\0';		// Remove CR
//NO_DESCENT_INI			p = strtok( line, " \t" );
//NO_DESCENT_INI			while( p )	{
//NO_DESCENT_INI				Args[Num_args++] = strdup( p );
//NO_DESCENT_INI				p = strtok( NULL, " \t" );
//NO_DESCENT_INI			}
//NO_DESCENT_INI		}
//NO_DESCENT_INI		fclose(fp);
//NO_DESCENT_INI	}

	for (i=0; i< Num_args; i++ )	{
		if ( Args[i][0] == '/' )  
			Args[i][0] = '-';

		if ( Args[i][0] == '-' )
			strlwr( Args[i]  );		// Convert all args to lowercase

		//printf( "Args[%d] = '%s'\n", i, Args[i] );
	}

}

