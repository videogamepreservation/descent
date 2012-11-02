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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <graph.h>
#include <malloc.h>
#include <math.h>
#include <conio.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

#define BITS 	(5)
#define LEVELS (1<<BITS)
#define MAXVAL (LEVELS-1)

// 0 = Red, 2=Green, 1=Blue

static int BoxRedLo[256];
static int BoxRedHi[256];

static int BoxGreenLo[256];
static int BoxGreenHi[256];

static int BoxBlueLo[256];
static int BoxBlueHi[256];

static int BoxNumElements[256];

static int TotalPixels;
int Frequency[32768];

static unsigned char * VideoMemory = (unsigned char *)0xA0000;

static void Shrink( int BoxIndex ) {
		
	int RedIndex, BlueIndex, GreenIndex, Index;

	int RedLo = BoxRedLo[BoxIndex];
	int RedHi = BoxRedHi[BoxIndex];

	int GreenLo = BoxGreenLo[BoxIndex];
	int GreenHi = BoxGreenHi[BoxIndex];

	int BlueLo = BoxBlueLo[BoxIndex];
	int BlueHi = BoxBlueHi[BoxIndex];
	
	for (RedIndex=RedLo; RedIndex<=RedHi; RedIndex++ )	
		for (BlueIndex=BlueLo; BlueIndex<=BlueHi; BlueIndex++ )	{
			Index = (RedIndex<<(BITS+BITS)) + (GreenLo<<BITS) + BlueIndex;
			for (GreenIndex=GreenLo; GreenIndex<=GreenHi; GreenIndex++, Index+=32 )
				if ( Frequency[ Index ]  )
					goto Next1;
		}
 
Next1:
	  BoxRedLo[BoxIndex] = RedIndex;
	  RedLo = RedIndex;
	  for (RedIndex=RedHi; RedIndex>=RedLo; RedIndex-- )
		  for (BlueIndex=BlueHi; BlueIndex>=BlueLo; BlueIndex-- ) {
			  Index=(RedIndex<<(BITS+BITS)) + (GreenHi<<BITS) + BlueIndex;
			  for (GreenIndex=GreenHi; GreenIndex>=GreenLo; GreenIndex--, Index -= 32 )
				  if ( Frequency[ Index ]  )
				  		goto Next2;
		  }
Next2:

	  BoxRedHi[BoxIndex] = RedIndex;
		RedHi = RedIndex;
	  for (BlueIndex=BlueLo; BlueIndex<=BlueHi; BlueIndex++ ) 
		  for (RedIndex=RedLo; RedIndex<=RedHi; RedIndex++ ) 	{
			  Index = (RedIndex<<(BITS+BITS)) + BlueIndex + (GreenLo<<BITS);
			  for (GreenIndex=GreenLo; GreenIndex<=GreenHi; GreenIndex++, Index += 32 )
				  if ( Frequency[ Index ] )
						goto Next3;

		 }
Next3:

	  BoxBlueLo[BoxIndex] = BlueIndex;
		BlueLo = BlueIndex;
	  for (BlueIndex=BlueHi; BlueIndex>=BlueLo; BlueIndex-- )   
		  for (RedIndex=RedHi; RedIndex>=RedLo; RedIndex-- )    {
			  Index=(RedIndex<<(BITS+BITS)) + (GreenHi<<BITS) + BlueIndex;
			  for (GreenIndex=GreenHi; GreenIndex>=GreenLo; GreenIndex--, Index -= 32 )
				  if ( Frequency[ Index ]  )
						goto Next4;
			}

Next4:
	  BoxBlueHi[BoxIndex] = BlueIndex;
		BlueHi = BlueIndex;
	  for (GreenIndex=GreenLo; GreenIndex<=GreenHi; GreenIndex++ )    
		  for (RedIndex=RedLo; RedIndex<=RedHi; RedIndex++ )    {
			  Index=(RedIndex<<(BITS+BITS)) + (GreenIndex<<BITS) + BlueLo;
			  for (BlueIndex=BlueLo; BlueIndex<=BlueHi; BlueIndex++, Index++ )
				  if ( Frequency[ Index ] )
						goto Next5;
			}

Next5:
	  BoxGreenLo[BoxIndex] = GreenIndex;
		GreenLo = GreenIndex;
	  for (GreenIndex=GreenHi; GreenIndex>=GreenLo; GreenIndex-- )   
		  for (RedIndex=RedHi; RedIndex>=RedLo; RedIndex-- )	{
			  Index=(RedIndex<<(BITS+BITS)) + (GreenIndex<<BITS) + BlueHi;
			  for (BlueIndex=BlueHi; BlueIndex>=BlueLo; BlueIndex--, Index-- )
				  if ( Frequency[ Index ]  )
						goto Next6;
			}
Next6:

	  BoxGreenHi[BoxIndex] = GreenIndex;

		// John's way...
		/*
		NewGreenLo = GreenHi; NewGreenHi = GreenLo;
		NewRedLo = RedHi; NewRedHi = RedLo;
		NewBlueLo = BlueHi; NewBlueHi = BlueLo;

	  for (GreenIndex=GreenHi; GreenIndex>=GreenLo; GreenIndex-- )   
		  for (RedIndex=RedHi; RedIndex>=RedLo; RedIndex-- )	{
			  Index=(RedIndex<<(BITS+BITS)) + (GreenIndex<<BITS) + BlueHi;
			  for (BlueIndex=BlueHi; BlueIndex>=BlueLo; BlueIndex--, Index-- )	{
					f = Frequency[ Index ]					
					
					if ( f )	{
						if ( RedIndex < NewRedLo ) NewRedLo = RedIndex;
						if ( GreenIndex < NewGreenLo ) NewGreenLo = GreenIndex;
						if ( BlueIndex < NewBlueLo ) NewBlueLo = BlueIndex;
					}
	
				}

	*/

}


void MedianClearFrequencies( int * freq_buffer );
#pragma aux MedianClearFrequencies= \
"push es"	\
"push ds" \
"pop  es" \
"xor	eax, eax"			\
"mov	ecx, 32768"			\
"rep	stosd"				\
"pop  es" \
parm [EDI]	\
modify [ EAX ECX ];

void MedianReadFrequencies(WORD * source, int numpixels);
#pragma aux MedianReadFrequencies = \
"xor	eax, eax"	\
"next_one:  mov	ax, word ptr [esi]" \
"add	esi,2"	\
"inc	Frequency[eax*4]" \
"loop	next_one"	\
parm [ESI] [ECX]	\
modify [ EAX ];


void MedianPutImage( WORD * source, unsigned char * dest, int numpixels );
#pragma aux MedianPutImage = \
"xor	eax, eax"	\
"next_pixel:  mov	ax, word ptr [esi]" \
"add	esi, 2"	\
"mov	eax, Frequency[eax*4]" \
"mov	[edi], al"	\
"inc 	edi"	\
"loop	next_pixel"	\
parm [ESI] [EDI] [ECX]	\
modify [ EAX ];


static int FindNextBoxToSplit(int NumBoxes)
{
	int c, SelectedBox, LongMax;

	  LongMax = 0;
	  SelectedBox = -1;

		// Find box with most elements that includes more than 1 color...
		// if none, we're done...

	  for (c=0; c<NumBoxes; c++ )    {
		  if ( (BoxNumElements[c] > LongMax) && 
					(	(BoxRedLo[c] != BoxRedHi[c]) ||
						(BoxBlueLo[c] != BoxBlueHi[c]) || 
						(BoxGreenLo[c] != BoxGreenHi[c]) ))	
		  {
			  LongMax = BoxNumElements[c];
			  SelectedBox = c;
		  }
	  }

	return SelectedBox;
}

static void SplitBoxRed( int SelectedBox, int TargetBox )
{
	int ElementSum, PlaneSum, RedIndex, GreenIndex, BlueIndex;

	ElementSum = 0;
	for (RedIndex=BoxRedLo[SelectedBox]; RedIndex<=BoxRedHi[SelectedBox]; RedIndex++ )    {
		PlaneSum = 0;
		for (BlueIndex=BoxBlueLo[SelectedBox]; BlueIndex<=BoxBlueHi[SelectedBox]; BlueIndex++ )
			for (GreenIndex=BoxGreenLo[SelectedBox]; GreenIndex<=BoxGreenHi[SelectedBox]; GreenIndex++ )
				PlaneSum += Frequency[ RedIndex<<(BITS+BITS) | GreenIndex<<BITS | BlueIndex ];

		ElementSum += PlaneSum;
		if (ElementSum > BoxNumElements[SelectedBox]/2)
			break;
	}

	if (RedIndex == BoxRedHi[SelectedBox] )    {
		RedIndex--;
		ElementSum -= PlaneSum;
	}

	BoxRedLo[TargetBox] = BoxRedLo[SelectedBox];
	BoxRedHi[TargetBox] = BoxRedHi[SelectedBox];

	BoxBlueLo[TargetBox] = BoxBlueLo[SelectedBox];
	BoxBlueHi[TargetBox] = BoxBlueHi[SelectedBox];

	BoxGreenLo[TargetBox] = BoxGreenLo[SelectedBox];
	BoxGreenHi[TargetBox] = BoxGreenHi[SelectedBox];

	BoxRedLo[TargetBox] = RedIndex+1;
	BoxNumElements[TargetBox] = BoxNumElements[SelectedBox] - ElementSum;

	BoxRedHi[SelectedBox] = RedIndex;
	BoxNumElements[SelectedBox] = ElementSum;
}

static void SplitBoxBlue( int SelectedBox, int TargetBox )
{
	int ElementSum, PlaneSum, RedIndex, GreenIndex, BlueIndex;

	ElementSum = 0;
	for (BlueIndex=BoxBlueLo[SelectedBox]; BlueIndex<=BoxBlueHi[SelectedBox]; BlueIndex++ )    {
		PlaneSum = 0;
		for (RedIndex=BoxRedLo[SelectedBox]; RedIndex<=BoxRedHi[SelectedBox]; RedIndex++ )
			for (GreenIndex=BoxGreenLo[SelectedBox]; GreenIndex<=BoxGreenHi[SelectedBox]; GreenIndex++ )
				PlaneSum += Frequency[ RedIndex<<(BITS+BITS) | GreenIndex<<BITS | BlueIndex ];

		ElementSum += PlaneSum;
		if (ElementSum > BoxNumElements[SelectedBox]/2)
			break;
	}
	if (BlueIndex == BoxBlueHi[SelectedBox])    {
		BlueIndex--;
		ElementSum -= PlaneSum;
	}
	BoxRedLo[TargetBox] = BoxRedLo[SelectedBox];
	BoxRedHi[TargetBox] = BoxRedHi[SelectedBox];
	BoxBlueLo[TargetBox] = BoxBlueLo[SelectedBox];
	BoxBlueHi[TargetBox] = BoxBlueHi[SelectedBox];
	BoxGreenLo[TargetBox] = BoxGreenLo[SelectedBox];
	BoxGreenHi[TargetBox] = BoxGreenHi[SelectedBox];

	BoxBlueLo[TargetBox] = BlueIndex+1;
	BoxNumElements[TargetBox] = BoxNumElements[SelectedBox] - ElementSum;

	BoxBlueHi[SelectedBox] = BlueIndex;
	BoxNumElements[SelectedBox] = ElementSum;

}

static void SplitBoxGreen( int SelectedBox, int TargetBox )
{
	int ElementSum, PlaneSum, RedIndex, GreenIndex, BlueIndex;

	ElementSum = 0;
	for (GreenIndex=BoxGreenLo[SelectedBox]; GreenIndex<=BoxGreenHi[SelectedBox]; GreenIndex++ )    {
		PlaneSum = 0;
		for (RedIndex=BoxRedLo[SelectedBox]; RedIndex<=BoxRedHi[SelectedBox]; RedIndex++ )
			for (BlueIndex=BoxBlueLo[SelectedBox]; BlueIndex<=BoxBlueHi[SelectedBox]; BlueIndex++ )
				PlaneSum += Frequency[ RedIndex<<(BITS+BITS) | GreenIndex<<BITS | BlueIndex ];

		ElementSum += PlaneSum;
		if (ElementSum > BoxNumElements[SelectedBox]/2)
			break;
	}
	if (GreenIndex == BoxGreenHi[SelectedBox])    {
		GreenIndex--;
		ElementSum -= PlaneSum;
	}

	BoxRedLo[TargetBox] = BoxRedLo[SelectedBox];
	BoxRedHi[TargetBox] = BoxRedHi[SelectedBox];

	BoxBlueLo[TargetBox] = BoxBlueLo[SelectedBox];
	BoxBlueHi[TargetBox] = BoxBlueHi[SelectedBox];

	BoxGreenLo[TargetBox] = BoxGreenLo[SelectedBox];
	BoxGreenHi[TargetBox] = BoxGreenHi[SelectedBox];

	BoxGreenLo[TargetBox] = GreenIndex+1;
	BoxNumElements[TargetBox] = BoxNumElements[SelectedBox] - ElementSum;

	BoxGreenHi[SelectedBox] = GreenIndex;
	BoxNumElements[SelectedBox] = ElementSum;

}

static int FindAxisToSplit(int box )
{
	int c, Max, axis;

	Max = BoxRedHi[box] - BoxRedLo[box];
	axis = 0;

	if (Max<(c=(BoxBlueHi[box]-BoxBlueLo[box])))    {
		Max = c;
		axis = 1;
	}

	if (Max<(c=(BoxGreenHi[box]-BoxGreenLo[box])))    {
		Max = c;
		axis = 2;
	}

	return axis;
}


static int FindTargetBox( int NumBoxes )
{
	int c;

	for (c=0; c<NumBoxes; c++ ) 
		if (BoxNumElements[c] == 0 )  
			return c;
	
	return NumBoxes;
}

static void MedianSetPalette( int NumBoxes, char * palette )
{
	int Index,r,b,g;
	int rsum,bsum,gsum,tmp;
	int n=0;

//	outp( 0x3c8, 0 );

  	for (Index=0; Index < NumBoxes; Index++ )    {
		rsum = bsum = gsum = 0;
		for (r=BoxRedLo[Index]; r<=BoxRedHi[Index]; r++ )
			for (b=BoxBlueLo[Index]; b<=BoxBlueHi[Index]; b++ )
				for (g=BoxGreenLo[Index]; g<=BoxGreenHi[Index]; g++ )    {
					tmp = Frequency[ r<<(BITS+BITS) | g<<BITS | b ];
					Frequency[r<<(BITS+BITS) | g<<BITS | b] = Index;
					rsum += r*tmp;
					bsum += b*tmp;
					gsum += g*tmp;
					n++;
				}

		r = ((rsum)*2)/BoxNumElements[Index];
		g = ((gsum)*2)/BoxNumElements[Index];
		b = ((bsum)*2)/BoxNumElements[Index];

		*palette++ = r;
		*palette++ = g;
		*palette++ = b;

		//outp( 0x3c9, r );
		//outp( 0x3c9, g );
		//outp( 0x3c9, b );

 	}
	fprintf( stderr, "Inv table has %d entries\n", n );
	fprintf( stderr, "NumBoxes are %d\n", NumBoxes );
}

void mediancut( WORD * data, int num_pixels, int num_colors, void * dest_bitmap, char * palette )
{
  unsigned char r,g,b;
  int i, axis, SelectedBox, TargetBox;
	int TargetColors;
  int NumBoxes;

  TotalPixels = num_pixels;
  TargetColors = num_colors;

  //for ( i=0; i< 32768; i++ )
  //		Frequency[i] = 0;
  //MedianClearFrequencies(Frequency);

  BoxRedLo[0] = 0;
  BoxRedHi[0] = MAXVAL;
  BoxBlueLo[0] = 0;
  BoxBlueHi[0] = MAXVAL;
  BoxGreenLo[0] = 0;
  BoxGreenHi[0] = MAXVAL;

/*  for ( i=0; i< TotalPixels; i++ )	{
		if ((Frequency[ data[i] ]++)==0)	{
			r = (data[i]>>10)&31;
			g = (data[i]>>5)&31;
			b = (data[i]>>0)&31;
			if ( r < BoxRedLo[0] )
				BoxRedLo[0] = r;
			else if( r > BoxRedHi[0] )
				BoxRedHi[0] = r;
	
			if ( g < BoxGreenLo[0] )
				BoxGreenLo[0] = g;
			else if( g > BoxGreenHi[0] )
				BoxGreenHi[0] = g;
	
			if ( b < BoxBlueLo[0] )
				BoxBlueLo[0] = b;
			else if( b > BoxBlueHi[0] )
				BoxBlueHi[0] = b;
		}
	}
*/

  BoxNumElements[0] = TotalPixels;
  NumBoxes = 1;
	
  //for ( i=0; i< TotalPixels; i++ )	
  //	Frequency[ data[i] ]++;
  //MedianReadFrequencies( data,TotalPixels );
  Shrink(0);

  while(NumBoxes < TargetColors )    {
							 
		SelectedBox = FindNextBoxToSplit(NumBoxes);
		if (SelectedBox == -1 ) break;
		
		TargetBox	= FindTargetBox( NumBoxes );
		axis			= FindAxisToSplit( SelectedBox );

		switch(axis)	{
		case 0:	SplitBoxRed( SelectedBox, TargetBox );
					break;
		case 1:	SplitBoxBlue( SelectedBox, TargetBox );
					break;
		case 2:	SplitBoxGreen( SelectedBox, TargetBox );
					break;
		}

		Shrink(SelectedBox);
		Shrink(TargetBox);
	
		if (TargetBox == NumBoxes )  NumBoxes++;

  }

  //for ( i=0; i< TotalPixels; i++ )
  //		VideoMemory[i] = Frequency[ bmp->Bits[i] ];

 	MedianSetPalette( NumBoxes, palette );
//	MedianPutImage( data, dest_bitmap, TotalPixels );
}


