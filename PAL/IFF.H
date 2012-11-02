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
		Constants and structures for IFF routines
*/

#define IFF_NO_ERROR			0
#define IFF_NO_MEM			1
#define IFF_UNKNOWN_FORM	2
#define IFF_NOT_IFF			3

#define PBM_TYPE				1
#define ILBM_TYPE				2

#define cmpNone	0
#define cmpByteRun1	1

#define mskNone	0
#define mskHasMask	1
#define mskHasTransparentColor 2

#define byte BYTE

/* structures used by IFF parser */

typedef unsigned char BYTE;
typedef char SBYTE;
typedef unsigned short WORD;
typedef short SWORD;
typedef unsigned int DWORD;
typedef int SDWORD;


struct pal_entry {BYTE r,g,b;};

// w = width, must be set
// h = height, must be set
// x,y ignored
// transparentcolor, set this to 0
// pagewidth = width of source screen, set to 320
// pageheight = height of source screen, set to 200
// nplanes = 8
// masking = set according to constants above, set to 0
// compression = 0 (cmpNone)
// xaspect = 5
// yaspect = 6
// palette = i must set this, values get shifted up by 4x
// raw_data = poSWORD to my data
struct bitmap_header {
	SWORD w,h,x,y;
	SWORD type;
	SWORD transparentcolor,pagewidth,pageheight;
	SBYTE nplanes,masking,compression;
	SBYTE xaspect,yaspect;
	struct pal_entry palette[256]; 
	BYTE  *raw_data;
};

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
