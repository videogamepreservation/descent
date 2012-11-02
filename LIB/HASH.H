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
 * $Source: f:/miner/source/main/rcs/hash.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:26:46 $
 * 
 * .
 * 
 * $Log: hash.h $
 * Revision 2.0  1995/02/27  11:26:46  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.2  1994/05/03  16:45:24  john
 * Added hash table lookup to speed up loading.
 * 
 * Revision 1.1  1994/05/03  11:35:16  john
 * Initial revision
 * 
 * 
 */



#ifndef _HASH_H
#define _HASH_H

typedef struct hashtable {
	int 				bitsize;
	int				and_mask;
	int				size;
	int				nitems;
	char				**key;
	int				*value;
} hashtable;

int hashtable_init( hashtable *ht, int size );
void hashtable_free( hashtable *ht );
int hashtable_search( hashtable *ht, char *key );
void hashtable_insert( hashtable *ht, char *key, int value );

#endif

