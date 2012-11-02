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

#include "gr.h"
#include "ui.h"
#include "key.h"


extern void gr_pal_setblock(int start, int stop, void * colors );

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;

#define GAMMA  1.3

#define CUBE(x) ((x)*(x)*(x))
#define SQUARE(x) ((x)*(x))

#define BM_LINEAR   0
#define BM_MODEX    1
#define BM_SVGA     2
#define BM_RGB15    3   //5 bits each r,g,b stored at 16 bits

typedef struct  {
	WORD        X, Y;
	WORD        Width;
	WORD        Height;
	WORD        Type;
	WORD        Rowsize;
	DWORD       DataPtr;
	WORD        Data[1];
} BITMAP15;

typedef struct  {
	WORD        X, Y;
	WORD        Width;
	WORD        Height;
	WORD        Type;
	WORD        Rowsize;
	DWORD       DataPtr;
	BYTE        Data[1];
} BITMAP8;

extern BITMAP15 * IFF_To_15BPP(char * ifilename);

typedef struct  {
	BYTE        Red;
	BYTE        Green;
	BYTE        Blue;
} RGB;

typedef struct
{
	int Lo[3];
	int Hi[3];
	int NumElements;
} BOX;


BOX Boxes[256];             // The color boxes that we want to fill
RGB ColRegs[256];           // The palette entries that we fill
int * RGBCube = NULL;         // Used to count the color frequencies in the original image.

void *bmd_fade_table;

BYTE * AverageTable;
BYTE ColorTable[16][256];
BYTE * LookupTable;

int Usage[256];
int NumBoxes;
int TotalPixels;
int TotalPixels1;
int AllowedColors = 256;
BYTE Gamma32[32];
BYTE Gamma64[32];

int WorstError;
BYTE WorstRed, WorstGreen, WorstBlue;

char * Filename[25];
BITMAP15 * Image15[25];
BITMAP8 * Image8[25];
int NumImages = 0;

char temp[200];

int RequestedColors;

void * ReadFile( char * filename, int * length )
{
	void * FileData;
	int handle;

	handle = open( filename, O_RDONLY | O_BINARY );
	if (handle == -1 ) {
		printf( "Unable to open '%s'\n", filename );
		exit(1);
	}

	*length = filelength( handle );

	FileData = (void * )malloc( *length );

	if (FileData == NULL )  {
		close( handle );
		printf( "Not enough memory to read in file '%s'\n", filename );
		exit(1);
	}

	if (read( handle, FileData, *length ) != *length )    {
		printf( "Error reading in '%s' data.\n", filename );
		free( FileData );
		close( handle );
		exit(1);
	}
	close( handle );

	return FileData;

}

void WriteFile( char * filename, void * data, int length )
{
	int handle;

	handle = open( filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
	if (handle == -1 ) {
		printf( "Error opening output file '%s'\n", filename );
		exit(1);
	}
	if (write( handle, data, length )!=length)  {
		printf( "Error writing output file '%s'\n", filename );
		close( handle );
		exit(1);
	}

	close( handle );
}



void InitializeMixer()
{
	int i;

	printf( "Initializing mixer...\n" );

	TotalPixels = 0;


	if (RGBCube==NULL )
	{
		RGBCube = (int *)malloc(32*32*32*sizeof(int));
		if (RGBCube==NULL) exit(1);
	}

	for (i=0; i<32768; i++ )
	{
		RGBCube[i] = 0;
	}

	for (i=0; i<256; i++ )
		Usage[i] = 0;
}


void FillGamma() {
	double intensity;
	int i;
	int sellevels, pallevels;

	sellevels = 31;
	pallevels = 63;

	Gamma64[0] = 0;
	for (i=1; i<=sellevels; i++) {
		intensity = (double)i/(double)sellevels;
		Gamma64[i] = (BYTE)((pow(intensity, 1.0/GAMMA)*pallevels) + 0.5);
	}

	sellevels = 31;
	pallevels = 31;

	Gamma32[0] = 0;
	for (i=1; i<=sellevels; i++) {
		intensity = (double)i/(double)sellevels;
		Gamma32[i] = (BYTE)((pow(intensity, 1.0/GAMMA)*pallevels) + 0.5);
	}
}



void ScanColorFrequencies( BITMAP15 * buf, char * filename )
{
	int i;
	int r, g, b, max;

	printf( "Factoring in %s's colors...\n", filename );

	for (i=0; i< (buf->Width*buf->Height); i++ )
	{

		r = ((buf->Data[i] >> 10) & 31);
		g = ((buf->Data[i] >> 5) & 31);
		b = (buf->Data[i] & 31);



		max = 0;
		if ( r > max) max = r;
		if ( g > max) max = g;
		if ( b > max) max = b;

		if (max > 0) {
			RGBCube[ (r<<10)+(g<<5)+b ]++;
			TotalPixels++;
		}
	}
}

void Shrink( int BoxIndex ) {
  int axis, aax1, aax2;
  int ind[3], flag;

  for (axis=0; axis<3; axis++ ) {
	  switch(axis)    {
		  case 0:
			  aax1=1; aax2=2; break;
		  case 1:
			  aax1=0; aax2=2; break;
		  case 2:
			  aax1=0; aax2=1;
	  }

	  flag=0;
	  for (ind[axis]=Boxes[BoxIndex].Lo[axis]; ind[axis]<=Boxes[BoxIndex].Hi[axis]; ind[axis]++ )    {
		  for (ind[aax1]=Boxes[BoxIndex].Lo[aax1]; ind[aax1]<=Boxes[BoxIndex].Hi[aax1]; ind[aax1]++ )    {
			  for (ind[aax2]=Boxes[BoxIndex].Lo[aax2]; ind[aax2]<=Boxes[BoxIndex].Hi[aax2]; ind[aax2]++ )
				  if (RGBCube[ (ind[0]<<10)+(ind[2]<<5)+ind[1] ] )    {
					  flag = 1;
					  break;
				  }
			  if (flag) break;
		  }
		  if (flag) break;
	  }

	  Boxes[BoxIndex].Lo[axis] = ind[axis];
	  flag=0;
	  for (ind[axis]=Boxes[BoxIndex].Hi[axis]; ind[axis]+1>=Boxes[BoxIndex].Lo[axis]+1; ind[axis]-- )    {
		  for (ind[aax1]=Boxes[BoxIndex].Hi[aax1]; ind[aax1]+1>=Boxes[BoxIndex].Lo[aax1]+1; ind[aax1]-- )    {
			  for (ind[aax2]=Boxes[BoxIndex].Hi[aax2]; ind[aax2]+1>=Boxes[BoxIndex].Lo[aax2]+1; ind[aax2]-- )
				  if (RGBCube[ (ind[0]<<10)+(ind[2]<<5)+ind[1] ] )    {
					  flag = 1;
					  break;
				  }
			  if (flag) break;
		  }
		  if (flag) break;
	  }
	  Boxes[BoxIndex].Hi[axis] = ind[axis];
  }
}


int GetGrey( RGB * reg )
{
	BYTE r, b, g;

	r = reg->Red/2;
	g = reg->Green/2;
	b = reg->Blue/2;
	return (r+g+b)/3;

}

void SortPalette()
{
	BYTE t;
	int i, j, incr, grey1, grey2, t1;

	incr = AllowedColors / 2;
	while( incr > 0 )
	{
		for (i=incr; i<AllowedColors; i++ )
		{
			j = i - incr;
			while (j>=0 )
			{
				grey1 = GetGrey( &(ColRegs[j]) );
				grey2 = GetGrey( &(ColRegs[j+incr]) );

				if (grey1 > grey2 ) {
					t = ColRegs[j+incr].Red;
					ColRegs[j+incr].Red=ColRegs[j].Red;
					ColRegs[j].Red=t;
					t = ColRegs[j+incr].Green;
					ColRegs[j+incr].Green=ColRegs[j].Green;
					ColRegs[j].Green=t;
					t = ColRegs[j+incr].Blue;
					ColRegs[j+incr].Blue=ColRegs[j].Blue;
					ColRegs[j].Blue=t;
					t1 = Usage[j+incr];
					Usage[j+incr]=Usage[j];
					Usage[j]=t1;
					j -= incr;
				}
				else
					break;
			}
		}
		incr = incr / 2;
	}

	t = ColRegs[254].Red;
	ColRegs[254].Red=ColRegs[7].Red;
	ColRegs[7].Red=t;
	t = ColRegs[254].Green;
	ColRegs[254].Green=ColRegs[7].Green;
	ColRegs[7].Green=t;
	t = ColRegs[254].Blue;
	ColRegs[254].Blue=ColRegs[7].Blue;
	ColRegs[7].Blue=t;
	t1 = Usage[254];
	Usage[254]=Usage[7];
	Usage[7]=t1;
}


void CombinePalettes() {
  int SelectedBox, c, ind[3], Max, axis, TargetBox, k, aax1, aax2;
  int LongMax, PlaneSum, ElementSum;
  int Index,r,b,g;
  int rsum,bsum,gsum,tmp, i;

  printf( "Creating a master palette...\n"  );

  Boxes[0].Lo[0] = 0;
  Boxes[0].Hi[0] = 31;
  Boxes[0].Lo[1] = 0;
  Boxes[0].Hi[1] = 31;
  Boxes[0].Lo[2] = 0;
  Boxes[0].Hi[2] = 31;

  Boxes[0].NumElements = TotalPixels;
  NumBoxes = 1;
  Shrink(0);

  while(NumBoxes < AllowedColors )    {
	  LongMax = 0;
	  SelectedBox = 1000;
	  for (c=0; c<NumBoxes; c++ )    {
		  if ((Boxes[c].NumElements > LongMax) && ((Boxes[c].Lo[0] != Boxes[c].Hi[0]) || (Boxes[c].Lo[1] != Boxes[c].Hi[1]) || (Boxes[c].Lo[2] != Boxes[c].Hi[2])) )    {
			  LongMax = Boxes[c].NumElements;
			  SelectedBox = c;
		  }
	  }
	  if (SelectedBox == 1000) break;
	  axis = 0;
	  Max = Boxes[SelectedBox].Hi[axis] - Boxes[SelectedBox].Lo[axis];
	  for (k=1; k<3; k++ )    {
		  if (Max<(c=(Boxes[SelectedBox].Hi[k]-Boxes[SelectedBox].Lo[k])))    {
			  Max = c;
			  axis = k;
		  }
	  }

	  TargetBox = NumBoxes;
	  for (c=0; c<NumBoxes; c++ )    {
		  if (Boxes[c].NumElements == 0 )    {
			  TargetBox = c;
			  break;
		  }
	  }

	  switch(axis)    {
		  case 0:
			  aax1=1; aax2=2; break;
		  case 1:
			  aax1=0; aax2=2; break;
		  case 2:
			  aax1=0; aax2=1;
	  }

	  if (Boxes[SelectedBox].Hi[axis] != Boxes[SelectedBox].Lo[axis])    {
		  ElementSum = 0;
		  for (ind[axis]=Boxes[SelectedBox].Lo[axis]; ind[axis]<=Boxes[SelectedBox].Hi[axis]; ind[axis]++ )    {
			  PlaneSum = 0;
			  for (ind[aax1]=Boxes[SelectedBox].Lo[aax1]; ind[aax1]<=Boxes[SelectedBox].Hi[aax1]; ind[aax1]++ )
				  for (ind[aax2]=Boxes[SelectedBox].Lo[aax2]; ind[aax2]<=Boxes[SelectedBox].Hi[aax2]; ind[aax2]++ )
					  PlaneSum += RGBCube[ (ind[0]<<10)+(ind[2]<<5)+ ind[1] ];
			  ElementSum += PlaneSum;
			  if (ElementSum > Boxes[SelectedBox].NumElements/2)
				  break;
		  }
		  if (ind[axis] == Boxes[SelectedBox].Hi[axis])    {
			  ind[axis]--;
			  ElementSum -= PlaneSum;
		  }

		  for (c=0; c< 3; c++ )    {
			  Boxes[TargetBox].Lo[c] = Boxes[SelectedBox].Lo[c];
			  Boxes[TargetBox].Hi[c] = Boxes[SelectedBox].Hi[c];
		  }
		  Boxes[TargetBox].Lo[axis] = ind[axis]+1;
		  Boxes[TargetBox].NumElements = Boxes[SelectedBox].NumElements - ElementSum;

		  Boxes[SelectedBox].Hi[axis] = ind[axis];
		  Boxes[SelectedBox].NumElements = ElementSum;
		  Shrink(SelectedBox);
		  Shrink(TargetBox);

		  if (TargetBox == NumBoxes )   {
			NumBoxes++;
		  }
	  }
  }

  AllowedColors = NumBoxes;

  for (Index=0; Index < AllowedColors; Index++ )    {
	  rsum = bsum = gsum = 0;
	  for (r=Boxes[Index].Lo[0]; r<=Boxes[Index].Hi[0]; r++ )
		  for (b=Boxes[Index].Lo[1]; b<=Boxes[Index].Hi[1]; b++ )
			  for (g=Boxes[Index].Lo[2]; g<=Boxes[Index].Hi[2]; g++ )    {
				  tmp = RGBCube[ (r<<10)+(g<<5)+b ];
				  rsum += r*tmp;
				  bsum += b*tmp;
				  gsum += g*tmp;
			  }
			  ColRegs[Index+16].Red   = (BYTE)((rsum*2) / Boxes[Index].NumElements);
			  ColRegs[Index+16].Blue  = (BYTE)((bsum*2) / Boxes[Index].NumElements);
			  ColRegs[Index+16].Green = (BYTE)((gsum*2) / Boxes[Index].NumElements);
  }

  AllowedColors += 16;

  ColRegs[0].Red =  0; ColRegs[0].Green = 0; ColRegs[0].Blue = 0;
  ColRegs[1].Red =  0; ColRegs[1].Green = 0; ColRegs[1].Blue =42;
  ColRegs[2].Red =  0; ColRegs[2].Green =42; ColRegs[2].Blue = 0;
  ColRegs[3].Red =  0; ColRegs[3].Green =42; ColRegs[3].Blue =42;
  ColRegs[4].Red = 42; ColRegs[4].Green = 0; ColRegs[4].Blue = 0;
  ColRegs[5].Red = 42; ColRegs[5].Green = 0; ColRegs[5].Blue =42;
  ColRegs[6].Red = 42; ColRegs[6].Green =21; ColRegs[6].Blue = 0;
  ColRegs[7].Red = 42; ColRegs[7].Green =42; ColRegs[7].Blue =42;
  ColRegs[8].Red = 21; ColRegs[8].Green =21; ColRegs[8].Blue =21;
  ColRegs[9].Red = 21; ColRegs[9].Green =21; ColRegs[9].Blue =63;
  ColRegs[10].Red = 21; ColRegs[10].Green =63; ColRegs[10].Blue =21;
  ColRegs[11].Red = 21; ColRegs[11].Green =63; ColRegs[11].Blue =63;
  ColRegs[12].Red = 63; ColRegs[12].Green =21; ColRegs[12].Blue =21;
  ColRegs[13].Red = 63; ColRegs[13].Green =21; ColRegs[13].Blue =63;
  ColRegs[14].Red = 63; ColRegs[14].Green =63; ColRegs[14].Blue =21;
  ColRegs[15].Red = 63; ColRegs[15].Green =63; ColRegs[15].Blue =63;

}


void GammaCorrect()
{
	int i;
	for (i=0; i<AllowedColors; i++ )
	{
		ColRegs[i].Red = Gamma32[ColRegs[i].Red];
		ColRegs[i].Green = Gamma32[ColRegs[i].Green];
		ColRegs[i].Blue = Gamma32[ColRegs[i].Blue];
	}
}

void SortPalette1()
{
	BYTE t;
	int i, j, k;

	printf( "Sorting palette by usage...\n" );

	for (i=1; i<AllowedColors; i++) {
		j=i;
		while (j && (Usage[j] < Usage[j-1])) {
			t = ColRegs[j-1].Red;
			ColRegs[j-1].Red=ColRegs[j].Red;
			ColRegs[j].Red=t;
			t = ColRegs[j-1].Green;
			ColRegs[j-1].Green=ColRegs[j].Green;
			ColRegs[j].Green=t;
			t = ColRegs[j-1].Blue;
			ColRegs[j-1].Blue=ColRegs[j].Blue;
			ColRegs[j].Blue=t;
			k = Usage[j-1];
			Usage[j-1] = Usage[j];
			Usage[j] = k;
			j--;
		}
	}
}

BYTE FindColor( int r, int g, int b )
{
	int i, minerr, e;
	BYTE best;

	//index = (r<<12)+(g<<6)+b;
	//if (Lookup[index] != -1)
	//    return Lookup[index];

	minerr = 64*64*64*64;
	best = 0;

	for (i=0; i<AllowedColors; i++ )
	{

		e =  SQUARE(ColRegs[i].Red - r);
		e += SQUARE(ColRegs[i].Green - g);
		e += SQUARE(ColRegs[i].Blue - b);

		if ( e < minerr  )   {
			minerr = e;
			best = (BYTE)i;
			if (minerr == 0) break;
		}
	}

	return best;
}

BITMAP8 * ReduceTo8bpp( BITMAP15 * in, char * filename )
{
	char new[20];
	char * p;
	int i, j, size, r, g, b;
	BITMAP8 * out;

	char * tmp;

	strcpy(new, filename );
	p = strstr( new, "." );
	if (p!=NULL )
	{
		p[1] = 'B';
		p[2] = 'M';
		p[3] = '1';
		p[4] = 0;
		printf( "Writing %s...\n", new );
		WriteFile( new, in, in->Width*in->Height*2+sizeof(BITMAP15)  );

		p[3] = '2';
		printf( "Writing %s...\n", new );
		tmp = malloc( in->Width*in->Height*4 );
		if (tmp==NULL) exit(1);

		for (i=0; i< in->Width*in->Height; i++ )
		{
			tmp[i*4+0] = (in->Data[i]>>10) & 31;
			tmp[i*4+1] = (in->Data[i]>>5) & 31;
			tmp[i*4+2] = in->Data[i] & 31;
			tmp[i*4+3] = 0;
		}

		WriteFile( new, tmp, in->Width*in->Height*4  );
		free(tmp);
	}


	printf( "Reducing colors in %s...\n", filename );


	size = in->Height * in->Width;

	out = (BITMAP8 *)in;

	out->Rowsize = out->Width;
	out->Type = BM_LINEAR;
	out->DataPtr = 0;

	for (i=0; i< size; i++ )
	{
		if (in->Data[i]==0)
			out->Data[i] = 255;
		else    {
			out->Data[i] = LookupTable[ in->Data[i] ];
			Usage[ out->Data[i] ]++;
		}
	}

	return out;
}

void WritePalette()
{
	printf( "Writing palette...\n" );

	WriteFile( "PALETTE.TBL", ColRegs, 256*3 );

}

BITMAP15 * ReadImage( char * filename )
{
	printf( "Reading in %s...\n", filename );
	return IFF_To_15BPP( filename );

}


void PrintUsage()
{
	printf( "Usage:  TESTP [-c#] files...\n" );
}

void DisplayPalette()
{
	int i, j, k;
	gr_set_current_canvas(NULL);
	for (i=0; i<AllowedColors; i++ )
	{
		for (j=0; j<16; j++ )
		{
			gr_setcolor( ColorTable[j][i] );
			for (k=0; k<10; k++ )
				gr_pixel( i, (j*10)+k );
		}
	}
}


void DisplayImage( BITMAP8 * Image )
{
	int x,y,i,j, l, lt;

	lt = MessageBox( 320, 240, 4, "Animation?", "Add Red", "Add Green", "Add Blue", "Fade out" );

	gr_set_current_canvas(NULL);

	if (lt==4)
	{
		for (j=15; j>=0; j-- )
		{
			i=0;
			for (y=0; y< Image->Height; y++ )
			{
				for (x=0; x< Image->Width; x++ )
				{
					gr_setcolor( ColorTable[j][Image->Data[i++]] );
					gr_rect( x*3, y*3, x*3+3, y*3+3 );
					//gr_pixel(x,y);
				}
			}
			key_getch();
		}
	}
	else
	{
		for (j=15; j>=0; j-- )
		{
			if (lt==1)
				l = LookupTable[((15-j)*2)<<10] << 8;
			if (lt==2)
				l = LookupTable[((15-j)*2)<<5] << 8;
			if (lt==3)
				l = LookupTable[((15-j)*2)] << 8;

			i=0;
			for (y=0; y< Image->Height; y++ )
			{
				for (x=0; x< Image->Width; x++ )
				{
					gr_setcolor( AverageTable[l+Image->Data[i++]] );
					gr_rect( x*3, y*3, x*3+3, y*3+3 );
					//gr_pixel(x,y);
				}
			}
			key_getch();
		}
	}

}


void FactorInShades()
{
	int j, i, r, g, b;

	printf( "Factoring in shades...\n" );

	for (j=1; j<=16; j++ )
	{
		for (i=0; i < AllowedColors; i++ )
		{
				r = (j*ColRegs[i].Red)/(16*2);
				g = (j*ColRegs[i].Green)/(16*2);
				b = (j*ColRegs[i].Blue)/(16*2);

				if ((r+g+b)>0)  {
					RGBCube[ (r<<10)+(g<<5)+b ] += (Boxes[i].NumElements*2)/j;
					TotalPixels+=(Boxes[i].NumElements*2)/j;
				}
		}
	}
}

void FactorInAlternateLights()
{
	int j, i, r, g, b;

	printf( "Factoring in alternate lighting...\n" );

	for (j=1; j<=16; j++ )
	{
		for (i=0; i < AllowedColors; i++ )
		{
				r = (j*ColRegs[i].Red)/(16*2);
				g = (j*ColRegs[i].Green)/(16*2);
				b = (j*ColRegs[i].Blue)/(16*2);

				r += j;
				if (r>31) r=31;

				if ((r+g+b)>0)  {
					RGBCube[ (r<<10)+(g<<5)+b ] += (Boxes[i].NumElements*2)/j;
					TotalPixels+=(Boxes[i].NumElements*2)/j;
				}
		}
	}
	for (j=1; j<=16; j++ )
	{
		for (i=0; i < AllowedColors; i++ )
		{
				r = (j*ColRegs[i].Red)/(16*2);
				g = (j*ColRegs[i].Green)/(16*2);
				b = (j*ColRegs[i].Blue)/(16*2);

				g += j;
				if (g>31) g=31;

				if ((r+g+b)>0)  {
					RGBCube[ (r<<10)+(g<<5)+b ] += (Boxes[i].NumElements*2)/j;
					TotalPixels+=(Boxes[i].NumElements*2)/j;
				}
		}
	}
	for (j=1; j<=16; j++ )
	{
		for (i=0; i < AllowedColors; i++ )
		{
				r = (j*ColRegs[i].Red)/(16*2);
				g = (j*ColRegs[i].Green)/(16*2);
				b = (j*ColRegs[i].Blue)/(16*2);

				b += j;
				if (b>31) b=31;

				if ((r+g+b)>0)  {
					RGBCube[ (r<<10)+(g<<5)+b ] += (Boxes[i].NumElements*2)/j;
					TotalPixels+=(Boxes[i].NumElements*2)/j;
				}
		}
	}

}


void CalculateLookupTable()
{
	int r, g, b, i;

	printf( "Creating inverse lookup table...\n" );

	LookupTable= (BYTE *)malloc(32768);
	if (LookupTable==NULL) exit(1);

	i = 0;
	for (r=0; r<32; r++ )
		for (g=0; g<32; g++ )
			for (b=0; b<32; b++ )
				LookupTable[i++] = FindColor( r*2, g*2, b*2 );

	WriteFile( "IPAL.TBL", LookupTable, 32768 );

}

void CalculateShadeTables()
{
	int j, i, r, g, b;

	printf( "Creating and writing shade lookup tables...\n" );

	for (j=1; j<=16; j++ )
	{
	  ColorTable[j-1][255] = 255;
	  for (i=0; i < AllowedColors; i++ )
	  {
		  r = (j*ColRegs[i].Red)/16;
		  g = (j*ColRegs[i].Green)/16;
		  b = (j*ColRegs[i].Blue)/16;
		  if (j<16)
			ColorTable[j-1][i] = FindColor( r, g, b);
		  else
			ColorTable[15][i] = i;

	  }
	}
	WriteFile( "FADE.TBL", ColorTable, 256*16 );
}

void CreateBlendTable()
{
	int i,j,r,g,b, c;

	printf( "Creating and writing color averaging table...\n" );

	AverageTable= (BYTE *)malloc(256*256);
	if (AverageTable==NULL) exit(1);

	for (i=0; i<256; i++ )
	{
		for (j=i; j<256; j++ )
		{
			r = (ColRegs[i].Red+ColRegs[j].Red);
			g = (ColRegs[i].Green+ColRegs[j].Green);
			b = (ColRegs[i].Blue+ColRegs[j].Blue);
			if (r>63) r=63;
			if (g>63) g=63;
			if (b>63) b=63;
			c = FindColor( r,g,b );
			AverageTable[(i<<8)+j] = c;
			AverageTable[(j<<8)+i] = c;
		}
	}
	WriteFile( "BLEND.TBL", AverageTable, 256*256 );
}

void NormalizePalette()
{
	int i, max;

	max = 0;

	for (i=0; i<AllowedColors; i++ )
	{
		if (ColRegs[i].Red > max ) max = ColRegs[i].Red;
		if (ColRegs[i].Green > max ) max = ColRegs[i].Green;
		if (ColRegs[i].Blue > max ) max = ColRegs[i].Blue;
	}

	//printf( "Max=%d\n", max );
	//getch();

	for (i=0; i<AllowedColors; i++ )
	{
		ColRegs[i].Red = ((int)ColRegs[i].Red*63)/max;
		ColRegs[i].Green = ((int)ColRegs[i].Green*63)/max;
		ColRegs[i].Blue = ((int)ColRegs[i].Blue*63)/max;
	}

}

void DisplayIPAL()
{
	int i,x,y;

	x=y=0;

	gr_set_current_canvas(NULL);

	for (i=0; i<32768; i++ )
	{
		x++;
		if (x > 320) {
			x = 0; y++;
		}
		gr_setcolor( LookupTable[i] );
		gr_upixel( x, y );
	}
}


void DoInterface()
{
	int i;
	int menu1, NumItems1;
	char * MenuItems[256];

	gr_init( SM_640x480V );
	gr_init_font( "/miner/fonts/pc8x16.fnt" );

	ColRegs[255].Red   = 0;
	ColRegs[255].Blue  = 30;
	ColRegs[255].Green = 0;

	gr_set_palette(0, 256, (BYTE *)ColRegs );


	gr_setcolor(255);
	gr_rect(0,0,grd_curscreen->sc_w,grd_curscreen->sc_h);

	ui_init();

	for (NumItems1=0; NumItems1<NumImages; NumItems1++ )
	{
		sprintf(temp,"Fade %s", Filename[NumItems1]);
		MenuItems[NumItems1] = strdup( temp );
	}
	MenuItems[NumItems1++] = strdup( "Show IPAL" );
	MenuItems[NumItems1++] = strdup( "Show palette" );
	MenuItems[NumItems1++] = strdup( "Quit program" );

	gr_pal_fade_in( grd_curscreen->pal );

	menu1 = 2;

	while( menu1 != 1 )
	{
		while ( menu1 != NumItems1 )
		{
			ui_mouse_process();

			menu1 = PopupMenu( NumItems1, MenuItems );

			if (menu1<(NumItems1-2) && (menu1>0) )
			{
				DisplayImage( Image8[menu1-1] );
			}

			if (menu1==(NumItems1-2))
			{
				DisplayIPAL();
			}

			if (menu1==(NumItems1-1))
			{
				DisplayPalette();
			}
		}
		menu1 = MessageBox( -1, -1, 2, "Exit to DOS?", "Yes", "No" );
	}

	ui_close();

	gr_close();



	for (i=0; i< AllowedColors; i++ )
	{
		if ((i%20)==0) getch();
		printf( "%d  %d,%d,%d   %d\n", i, ColRegs[i].Red,ColRegs[i].Green,ColRegs[i].Blue, Usage[i] );
	}


}

void ParseCommandLine( int argc, char * argv[] )
{
	struct find_t find;
	char * cp;
	char * cp1;

	if (argc < 2 )  {
		PrintUsage();
		exit(1);
	}

	RequestedColors = 256;

	argv++; argc--;
	for (;argc--;argv++)
	{
		if ((**argv=='-') || (**argv=='/'))
		{
			cp=(*argv)+1;
			cp1 = cp;
			switch (*cp++)
			{
				case 'c':
					RequestedColors = atoi( cp );
					break;
				case '?':
				case 'h':
					PrintUsage();
					exit(1);
				default:
					printf( "Unrecognized switch '%s'\n", cp1 );
					break;
			}
		}
		else
		{
			if( !_dos_findfirst( *argv, 0xffff, &find ) )
			{
				Filename[NumImages++] = strdup( find.name );
				while( !_dos_findnext( &find ) )
				{
					Filename[NumImages++] = strdup( find.name );
				}
			}
			else
			{
				printf( "No files matching '%s' were found.\n", *argv );
				exit(1);
			}
		}
	}

	if ( NumImages == 0 )   {
		printf( "Error: Input files must be specified.\n\n" );
		PrintUsage();
		exit(1);
	}

	if (RequestedColors < 16) RequestedColors = 16;
	if (RequestedColors > 256) RequestedColors = 256;
	RequestedColors--;

}


void WriteBitmaps()
{
	char new[20];
	char * p;
	int i, j;

	for (j=0; j<NumImages; j++ )
	{
		strcpy(new, Filename[j] );
		p = strstr( new, "." );
		if (p!=NULL )
		{
			p[1] = 'B';
			p[2] = 'M';
			p[3] = 'S';
			p[4] = 0;
			printf( "Writing %s...\n", new );
			WriteFile( new, Image8[j], Image8[j]->Width*Image8[j]->Height+sizeof(BITMAP8)  );
		}
	}
}

void ReduceImages()
{
	int i;

	for (i=0; i<NumImages; i++ )
	{
		Image8[i] = ReduceTo8bpp( Image15[i], Filename[i] );
	}
}

void AnalyzeImages()
{
	int i;

	for (i=0; i<NumImages; i++ )
	{
		printf( "Reading in %s\n", Filename[i] );
		Image15[i] = IFF_To_15BPP( Filename[i] );
		ScanColorFrequencies( Image15[i], Filename[i] );
	}
}

int main(int argc, char * argv[] )
{

	ParseCommandLine( argc, argv );

	FillGamma();

	AllowedColors = 239;

	InitializeMixer();

	AnalyzeImages();

	CombinePalettes();

	//AllowedColors = 255;

	//InitializeMixer();

	//FactorInShades();

	//FactorInAlternateLights();

	//CombinePalettes();

	//SortPalette();

	CalculateLookupTable();

	ReduceImages();

	CalculateShadeTables();

	CreateBlendTable();

	NormalizePalette();

	WritePalette();

	WriteBitmaps();

	DoInterface();

	exit(1);
}





