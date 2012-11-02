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
//========================================================
// Views a PCX file 
// This is a 16-bit program.  
// Compiles using Turbo C.
// All self-contained in this one module. 
// Uses 320x200 or 320x400 ModeX.
//========================================================

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include <dir.h>
#include <dos.h>

typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char ubyte;

#ifndef MK_FP
#define MK_FP(seg,off)  ((void far *)(((dword)(seg)<<16)+(dword)(off)))
#endif

ubyte far * video_ram;

/* PCX Header data type */
typedef struct pcx_header	{
	ubyte		Manufacturer;
	ubyte		Version;
	ubyte		Encoding;
	ubyte		BitsPerPixel;
	short		Xmin;
	short		Ymin;
	short		Xmax;
	short		Ymax;
	short		Hdpi;
	short		Vdpi;
	ubyte		ColorMap[16][3];
	ubyte		Reserved;
	ubyte		Nplanes;
	short		BytesPerLine;
	ubyte		filler[60];
} pcx_header;

#define PCX_COUNT_FLAG 0xC0

#define PCX_ERROR_NONE				0
#define PCX_ERROR_OPENING			1
#define PCX_ERROR_NO_HEADER 		2
#define PCX_ERROR_WRONG_VERSION     3
#define PCX_ERROR_READING			4
#define PCX_ERROR_NO_PALETTE        5
#define PCX_TOO_LARGE               6

void set_cellheight( ubyte height )
{
	unsigned char temp;

   outportb( 0x3d4, 9 );
	temp = inportb( 0x3d5 );
   temp &= 0xE0;
	temp |= height;
	outportb( 0x3d5, temp );
}

void clear_palette()
{
	int i;

	outportb( 0x3c8, 0 );
	for (i=0; i<768; i++ )
		outportb( 0x3c9, 0 );
}

int modex_vres = 0;
int modex_drawing_start = 0;

void set_drawing_page(int page)
{
	if (page)
		modex_drawing_start = modex_vres;	// Page 1
	else
		modex_drawing_start = 0;				// Page 0
}

void set_viewing_page(int page)
{
	int hi, lo;
	int offset;

	if (page)
		offset = modex_vres*80;
	else
		offset = 0;

	hi = offset>>8;
	lo = offset&0xff;
	hi = (hi<<8) | 0xC;
	lo = (lo<<8) | 0xD;

	disable();
	
	while( inportb(0x3DA) & 0x1 )			// Wait for display enable
		;		

	outport( 0x3d4, hi );		// Set Start address high bits
	outport( 0x3d4, lo );		// Set Start address lo bits
	enable();

	while( !(inportb(0x3DA) & 0x8) )			// Wait for not vertical retrace
		;		
}

void set_video_mode(int mode, int use_400_lines )
{
	union REGS regs;

	regs.x.ax = mode;
	int86( 0x10, &regs, &regs );

	if ( mode == 0x13 )	{
		clear_palette();

		// Set ModeX
		outport( 0x3c4, 0x0604 );		  // disable chain4 mode
		outport( 0x3d4, 0x0014 );		  // turn on dword mode
		outport( 0x3d4, 0xe317 );		  // turn off byte mode

		modex_drawing_start = 0;			// Draw to page 0

		if ( use_400_lines )	{
			modex_vres = 400;
			set_cellheight(0);				// Set 400 line mode
		} else {
			modex_vres = 200;
		}
		set_drawing_page(0);					// Draw to page 0
	}
}


void set_pixel(int x, int y, ubyte color )
{
	unsigned int address;
	// Select write plane
	outport( 0x3c4, (0x100<<(x&3))|2 );

	address = (y+modex_drawing_start)*80;
	address += x/4;
	video_ram[address] = color;
}



void set_palette(ubyte * palette)
{
	int i;

	outportb( 0x3c8, 0 );
	for (i=0; i<768; i++ )
		outportb( 0x3c9, *palette++ );
}


int read_pcx_file( char * filename, ubyte * palette )
{
	FILE * PCXfile;
	pcx_header header;
	int i, row, col, count, xsize, ysize;
	ubyte data;

	PCXfile = fopen( filename , "rb" );
	if ( !PCXfile )
		return PCX_ERROR_OPENING;

	// read 128 char PCX header
	if (fread( &header, sizeof(pcx_header), 1, PCXfile )!=1) {
		fclose( PCXfile );
		return PCX_ERROR_NO_HEADER;
	}

	// Is it a 256 color PCX file?
	if ((header.Manufacturer != 10)||(header.Encoding != 1)||(header.Nplanes != 1)||(header.BitsPerPixel != 8)||(header.Version != 5))	{
		fclose( PCXfile );
		return PCX_ERROR_WRONG_VERSION;
	}

	// Find the size of the image
	xsize = header.Xmax - header.Xmin + 1;
	ysize = header.Ymax - header.Ymin + 1;

	if ( (xsize > 320) || (ysize > 400) )  {
		fclose( PCXfile );
		return PCX_TOO_LARGE;
	}

	if ( ysize > 200 )
		set_video_mode( 0x13, 1 );         // 320x200, 256 color
	else
		set_video_mode( 0x13, 0 );         // 320x200, 256 color

	set_drawing_page(1);						// Draw to page 1

	for (row=0; row< ysize ; row++)      {
		for (col=0; col< xsize ; )      {
			if (fread( &data, 1, 1, PCXfile )!=1 )	{
				fclose( PCXfile );	
				return PCX_ERROR_READING;
			}
			if ((data & PCX_COUNT_FLAG) == PCX_COUNT_FLAG)     {
				count =  data & (~PCX_COUNT_FLAG);
				if (fread( &data, 1, 1, PCXfile )!=1 )	{
					fclose( PCXfile );	
					return PCX_ERROR_READING;
				}
				for (i=0;i<count;i++)
					set_pixel( col++, row, data );
			} else {
				set_pixel( col++, row, data );
			}
		}
	}

	// Read the extended palette at the end of PCX file
	// Read in a character which should be 12 to be extended palette file
	if (fread( &data, 1, 1, PCXfile )==1)	{
		if ( data == 12 )	{
			if (fread(palette,768, 1, PCXfile)!=1)	{
				fclose( PCXfile );
				return PCX_ERROR_READING;
			}
			for (i=0; i<768; i++ )	{
				palette[i] /= 4;
			}
		}
	} else {
		fclose( PCXfile );	
		return PCX_ERROR_NO_PALETTE;
	}
	fclose(PCXfile);
	return PCX_ERROR_NONE;
}

int main( int argc, char * argv[] )
{
	struct ffblk ffblk;
	ubyte palette[768];
	int pcx_error, done;

	if ( argc < 2 ) {
		printf( "You must specify a pcx file to view!\n" );
		exit(1);
	}

	video_ram = MK_FP( 0xA000, 0 );

	done = findfirst( argv[1], &ffblk, 0 );
	while (!done) {
		pcx_error = read_pcx_file( ffblk.ff_name, palette );
		if ( pcx_error == PCX_ERROR_NONE )  {
			set_palette(palette);
			set_viewing_page(1);		// Show the completed page 1.
			getch();
		} else {
			set_video_mode( 0x3, 0 );
			if ( pcx_error != PCX_ERROR_NONE )
				printf( "Error %d reading '%s'\n", pcx_error, ffblk.ff_name );
			return pcx_error;
		}
		done = findnext( &ffblk );
	}
	set_video_mode( 0x3, 0 );
	return 0;		
}


