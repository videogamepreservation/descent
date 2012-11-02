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
 * $Source: $
 * $Revision: $
 * $Author: $
 * $Date: $
 * 
 * Files for debugging memory allocator
 * 
 * $Log: $
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: $";
#pragma on (unreferenced)


// Warning( "MEM: Too many malloc's!" );
// Warning( "MEM: Malloc returnd an already alloced block!" );
// Warning( "MEM: Malloc Failed!" );
// Warning( "MEM: Freeing the NULL pointer!" );
// Warning( "MEM: Freeing a non-malloced pointer!" );
// Warning( "MEM: %d/%d check bytes were overwritten at the end of %8x", ec, CHECKSIZE, buffer  );
// Warning( "MEM: %d blocks were left allocated!", numleft );

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <dos.h>

#include "mono.h"
#include "error.h"

#define CHECKSIZE 16

static unsigned int MallocBase[10000];
static unsigned int MallocSize[10000];
static unsigned char Present[10000];
static char * Filename[10000];
static char * Varname[10000];
static int Line[10000];
static int BytesMalloced = 0;

static int Initialized = 0;

void mem_display_blocks();

void mem_init()
{
	int i;

	Initialized = 1;

	for (i=0; i<10000; i++ )
	{
		MallocBase[i] = 0;
		MallocSize[i] = 0;
		Present[i] = 0;
		Filename[i] = NULL;
		Varname[i] = NULL;
		Line[i] = 0;
	}

	atexit(mem_display_blocks);

}

void * mem_malloc( unsigned int size, char * var, char * filename, int line )
{
	int i,j;
	void * ptr;
	char * pc;

	if (Initialized==0)
		mem_init();

	for (i=0; i<10000; i++ )
		if (Present[i]==0) break;

	if (i==10000)
	{
		Warning( "MEM: Too many malloc's!" );
		Int3();
	}

	ptr = malloc( size+CHECKSIZE );

	for (j=0; j<10000; j++ )
	{
		if (Present[j] && MallocBase[j] == (unsigned int)ptr )
		{
			Warning( "MEM: Malloc returnd an already alloced block!" );
			Int3();
		}
	}


	if (ptr==NULL)
	{
		Error( "MEM: Malloc Failed! (Var: %s, File: %s, Line: %d Size: %d bytes)",var,filename,line,size );
		Int3();
	}

	MallocBase[i] = (unsigned int)ptr;
	MallocSize[i] = (unsigned int)size;
	Varname[i] = var;
	Filename[i] = filename;
	Line[i] = line;
	Present[i]    = 1;

	pc = (char *)ptr;

	BytesMalloced += size;

	for (i=0; i<CHECKSIZE; i++ )
		pc[size+i] = 0xC;

	return ptr;

}

void mem_free( void * buffer )
{
	int i;
	char * pc;
	int n, ec;

	if (Initialized==0)
		mem_init();

	// mprintf( 0, "Freeing pointer: %8x\n", buffer );

	if (buffer==NULL )
	{
		Warning( "MEM: Freeing the NULL pointer!" );
		Int3();
	}

	for (i=0; i<10000; i++ )
	{
		if (Present[i]==1)
		{
			if (MallocBase[i] == (unsigned int)buffer )
				break;
		}
	}

	if (i==10000)
	{
		Warning( "MEM: Freeing a non-malloced pointer!" );
		Int3();
	}
	else {

		n = i;
		pc = (char *)buffer;
		ec = 0;
	
		for (i=0; i<CHECKSIZE; i++ )
		{
			if (pc[MallocSize[n]+i] != 0xC ) ec++;
		}
	
		if (ec > 0)
		{
			Warning( "MEM: %d/%d check bytes were overwritten at the end of %8x", ec, CHECKSIZE, buffer  );
			Int3();
		}
	
		BytesMalloced -= MallocSize[n];
	
		free( buffer );
	
		Present[n] = 0;
		MallocBase[n] = 0;
		MallocSize[n] = 0;
	}
}

void mem_display_blocks()
{
	int i, numleft;

	if (Initialized==0) return;

	numleft = 0;
	for (i=0; i<10000; i++ )
	{
		if (Present[i]==1)
		{
			numleft++;
			printf( "Block: %d, Var:%s = %x File: %s, Line: %d Size: %d bytes.\n", i, Varname[i], MallocBase[i], Filename[i], Line[i], MallocSize[i] );
			mem_free( (void *)MallocBase[i] );
		}
	}

	if (numleft )
	{
		Warning( "MEM: %d blocks were left allocated!", numleft );
	}
}

