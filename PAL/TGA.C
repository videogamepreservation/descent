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
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <malloc.h>
#include <ctype.h>

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;

typedef struct
{
	 BYTE  Field1;
	 BYTE  Field2;
	 BYTE  DataType ;
	 BYTE  Field41H ;
	 BYTE  Field41L ;
	 BYTE  Field42H ;
	 BYTE  Field42L ;
	 BYTE  Field43  ;
	 WORD  XOrigin  ;
	 WORD  YOrigin  ;
	 WORD  Width    ;
	 WORD  Height   ;
	 BYTE  BitsPerPixel;
	 BYTE  Field56  ;
} TGA_FILE_HEADER;

typedef struct  {
	WORD        Width;
	WORD        Height;
	WORD        Data[1];
} BITMAP15;

BITMAP15 * ReadInTGA(char * filename )
{
	BITMAP15 * NewBitmap;
	short row_b,col_b,row_e,col_e;
	char *buffer;
	TGA_FILE_HEADER imghead;
	int infile;
	int numread;
	short len ;
	short row,dcol,col_now,drow;
	char quitflag ;
	WORD bufindex, value ;
	BYTE  count;
	short tempcnt;
	short BufSize, RowsPerRead, BytesPerRow;
	short i;
	WORD tmpbuf[320];

	// Try to open file

	infile = open(filename,(O_BINARY|O_RDONLY) ) ;
	if (infile==-1) {
		printf( "TGAto15BPP: '%s' not found.\n", filename );
		return NULL;
	}

	// Read the image header
	numread = read(infile,(char*)&imghead, sizeof(TGA_FILE_HEADER) ) ;
	if ( numread != sizeof(TGA_FILE_HEADER) )     {
		printf( "TGAto15BPP: Error reading '%s' header.\n", filename );
		close(infile);
		return NULL;
	}

	//  get the data in header
	dcol  = imghead.Width;
	drow  = imghead.Height;

	// Assign column,row begin , displacement to input Origion
	col_b = imghead.XOrigin ;
	row_b = imghead.YOrigin ;
	col_b = 0 ;
	row_b = 0 ;

	// check if orgion overflow
	if ( (row_b>320) || (col_b>480) )   {
		printf( "TGAto15BPP: Origin overflow reading '%s'.", filename );
		close(infile);
		return NULL;
	}

	// Bound the image in maxrow, maxcal
	col_e = dcol + col_b - 1 ;
	row_e = drow + row_b - 1 ;
	if (col_e>320)
		col_e = 320 ;
	if (row_e>480)
		row_e = 480 ;

	// check if 16 bits per pixel, now support VIGA16 file only
	if (imghead.BitsPerPixel != 16 )    {
		printf( "TGAto15BPP: '%s' is not a valid 16bpp TGA file.\n", filename );
		close(infile);
		return NULL;
	}

	NewBitmap = (BITMAP15 *)malloc( sizeof(BITMAP15)+((col_e-col_b+1)*(row_e-row_b+1)*3) );
	if (NewBitmap == NULL ) {
		printf( "TGAto15bpp: Error allocating bitmap memory for '%s'.\n", filename );
		return NULL;
	}
	NewBitmap->Height = row_e - row_b + 1;
	NewBitmap->Width =  col_e - col_b + 1;

	// allocate a buffer as large as possible
	BufSize =  1024;
	while ( ((buffer=(char *)malloc((size_t)BufSize))==NULL) && (BufSize>=1024) )   {
		BufSize -= 1024 ;
	}

	// check if buffer enough, if not return MEMORY FULL
	if (BufSize<1024)   {
		printf( "TGAto15BPP: Error allocating buffer for '%s'.\n", filename );
		close(infile);
		free( buffer );
		free( NewBitmap);
		return NULL;
	}

	BytesPerRow = dcol * 2 ;
	RowsPerRead = BufSize / BytesPerRow  ;

	// Load image file routines...

	if (imghead.DataType == 2)   {
		// This image file is not compressed

		row = row_b ;
		while (row <= row_e)    {
			if ( (row+RowsPerRead) > row_e)
				RowsPerRead = row_e - row + 1;
			read(infile,buffer,BytesPerRow*RowsPerRead);
			bufindex = 0 ;
			for (value=0; value<RowsPerRead; value++)   {
				for (i=col_b; i<=col_e; i++ )
					memcpy( &(NewBitmap->Data[ NewBitmap->Width*(NewBitmap->Height-row-1)+i ]), &(buffer[bufindex+2*(i-col_b)]), 2 );
					//NewBitmap->Data[ NewBitmap->Width*(NewBitmap->Height-row-1)+i ] = buffer[bufindex+i-col_b];
				row++;
				bufindex += BytesPerRow ;
			} // for
		} // while
		// free the allocated buffer, close opened file
		free(buffer) ;
		close(infile);
		return NewBitmap;
	}

	if (imghead.DataType != 10)   {
		// Not a valid TGA file. Free the allocated buffer, close opened file
		printf( "TGAto15BPP: '%s' compression type is invalid.\n", filename );
		free(buffer) ;
		close(infile);
		free( NewBitmap);
		return  NULL;
	}

	// init col_now, quitflag to 0,init row to the start row
	col_now = 0 ;
	row = row_b ;
	quitflag = 0 ;

	// read a block of data to buffer
	numread = read(infile,buffer,BufSize);

	while ( (numread>0) && (quitflag==0) )   {

		// init bufindex, while loop to decode data
		bufindex = 0 ;

		while ( (bufindex<numread) && (quitflag==0) )  {
			// get the first data in buffer, inc bufindex

			count =  *(buffer+bufindex) ;
			bufindex++;

			// check if count a data or a counter

			if ( count >= 128 ) {
				// if end of buffer, read next block, and continue to decode them
				if (bufindex>=numread)  {
					numread = read(infile,buffer,BufSize) ;
					if (numread>0)  {
						bufindex = 0 ;
						memcpy(&value,(buffer+bufindex),2);
						bufindex += 2 ;
					}
					else
						quitflag = 1 ;
				}
				else
					if ( (bufindex+1) >= numread )  {
						value = *(buffer+bufindex) ;
						numread = read(infile,buffer,BufSize) ;
						if (numread>0)  {
							bufindex = 0 ;
							value += *(buffer+bufindex) << 8 ;
							bufindex++;
						}
						else
							quitflag = 1 ;
					}
					else    {
						memcpy(&value,(buffer+bufindex),2);
						bufindex += 2 ;
					}

				// dec count by 127, store data to
				// tmpbuf, adjust col_now

				count -= 127 ;    // 127
				while (count>0)   {
					tmpbuf[col_now] = value ;
					col_now++;
					count--;
				} // while
			} // if count
			else    {
				// not counter, inc count, cal tempcnt
				count++;
				tempcnt = 2*count ;

				// check if end of buffer, if yes read next
				// block to buffer, continue to decode
				if ( (bufindex+tempcnt) >= numread )    {
					while ( (bufindex+2)<=numread )   {
						memcpy(&tmpbuf[col_now],(buffer+bufindex), 2);
						col_now++;
						tempcnt -= 2 ;
						bufindex += 2 ;
					}
					if ( bufindex<numread ) {
						value = *(buffer+bufindex) ;
						numread = read(infile,buffer,BufSize) ;
						if (numread>0)  {
							bufindex = 0 ;
							value += *(buffer+bufindex) << 8 ;
							tmpbuf[col_now] = value ;
							col_now++;
							bufindex++;
							tempcnt -= 2 ;
						}
						else
							quitflag = 1 ;
					}
					else
					{
						numread = read(infile,buffer,BufSize) ;
						if (numread>0)
							bufindex = 0 ;
						else
							quitflag = 1 ;
					}
					if (tempcnt>0)
					{
						memcpy(&tmpbuf[col_now],(buffer+bufindex), tempcnt) ;
						col_now += ( tempcnt / 2) ;
						bufindex += tempcnt ;
					} // if tempcnt
				}  // if buffindex
				else
				{
					memcpy(&tmpbuf[col_now],(buffer+bufindex),tempcnt);
					bufindex += tempcnt ;
					col_now += count;
				} // else buffindex
			} // else count

			// check if tmpbuf enough to be a row, if yes
			// output the row to board, inc row
			if ( col_now>=dcol )
			{
				for (i=col_b; i<=col_e; i++ )
					NewBitmap->Data[ NewBitmap->Width*(NewBitmap->Height-row-1)+i ] = tmpbuf[i-col_b];
				row++;
				if (row>row_e)
					quitflag = 1 ;
				len = col_now - dcol ;
				if (len>0)
					memcpy(&tmpbuf[0],&tmpbuf[dcol], (2*len) ) ;
				col_now = len;
			} // if col_now >dcol
		} // while bufindex

		// read another block to be decoded
		numread = read(infile,buffer,BufSize) ;
	} // while numread

	// if there were some data in tmpbuf, output it
	if (col_now>0)  {
		for (i=col_b; i<=(col_e+col_now); i++ )
			NewBitmap->Data[ NewBitmap->Width*(NewBitmap->Height-row-1)+i ] = tmpbuf[i-col_b];
	}

	// free the allocated buffer, close opened file
	free(buffer) ;
	close(infile);
	return NewBitmap;

}


