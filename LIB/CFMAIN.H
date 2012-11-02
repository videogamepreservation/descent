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
/* CFMAIN.H */
/*=========================================================================
   Compression File Library                                Version 1.6
===========================================================================
The following files contain the source for the compression programs.
ARITH-N.C yields the best compression ratios, but is far too slow.
It looks like LZW is what we will be using.  It uncompressed PROCOMM.EXE
(~160K) in about 3-4 seconds.  If we need to compress sound or graphics
files, there are also algorithms available.  We could name the functions
gfread and sfread or something like that.

Version 1.6 fixes a bug in lzw.c (cfread).

Main Code
---------
CFMAIN.H        The header file which give prototypes and definitions for
                the routines and data structures which CFWRITE.C and
                CFREAD.C expect to find in compression routines.

CFTEST.C        Test program for CFREAD.

BITIO.C         The C file containing the bit oriented I/O routines used.

LZW.C           A fully featured LZW compression module.  This version
                includes variable length codes up to 15 bits, and tree
                flushing.

COMPRESS.C      Contains a driver for CFWRITE that takes an input
                file and writes it to a compressed output.

MAKEFILE        Makes CF.LIB, COMPRESS.EXE, and CFTEST.EXE.
===========================================================================

COMPRESS is used to compress the original datafile.

        Usage:  COMPRESS in-file out-file.

CFTEST checks the decompression using the cfread procedure and outputs
        some benchmarks.

=========================================================================*/
                  
#include <stdio.h>

typedef struct bit_file {
    unsigned char *buf;
    char *name;
    int current_byte;
    unsigned char mask;
    int rack;
    int pacifier_counter;
    int length;
} BIT_FILE;

BIT_FILE     *OpenInputBitFile( char *name );
BIT_FILE     *OpenOutputBitFile( char *name );
void          OutputBit( BIT_FILE *bit_file, int bit );
void          OutputBits( BIT_FILE *bit_file,
                          unsigned int code, int count );
int           InputBit( BIT_FILE *bit_file );
unsigned int  InputBits( BIT_FILE *bit_file, int bit_count );
void          CloseInputBitFile( BIT_FILE *bit_file );
void          CloseOutputBitFile( BIT_FILE *bit_file );
void          FilePrintBinary( FILE *file, unsigned int code, int bits );

unsigned char *cfread( char *filename, int *length );
void cfwrite( char *filename, unsigned char *input, int length );

extern char *Usage;
extern char *CompressionName;

