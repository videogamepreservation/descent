/*
 * $Source: f:/miner/source/main/editor/rcs/einit.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:24 $
 *
 * Initialization routines for editor.
 *
 * $Log: einit.c $
 * Revision 2.0  1995/02/27  11:34:24  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.9  1993/09/23  15:01:38  mike
 * *** empty log message ***
 * 
 * Revision 1.8  1993/09/22  10:52:36  mike
 * Move create_new_mine to segment.c, thereby making this a rather unimportant file.
 * 
 * Revision 1.7  1993/09/22  09:41:33  mike
 * Change constant and variable names to conform to coding standards.
 * 
 * Revision 1.6  1993/09/21  15:09:25  mike
 * Make create_new_mine create a multi-segment mine.
 * 
 * Revision 1.5  1993/09/17  16:44:31  mike
 * new create_new_mine
 * 
 * Revision 1.4  1993/09/16  12:50:00  mike
 * Create file.
 * 
 * Revision 1.3  1993/09/15  19:11:20  matt
 * Hacked this to make it work
 * 
 * Revision 1.2  1993/09/15  18:10:54  mike
 * no changes
 * 
 * Revision 1.1  1993/09/15  18:01:39  mike
 * Initial revision
 * 
 *
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: einit.c 2.0 1995/02/27 11:34:24 john Exp $";
#pragma on (unreferenced)

#include	<stdio.h>

#include "inferno.h"
#include "mono.h"
#include "fix.h"
#include "segment.h"
#include	"editor.h"

