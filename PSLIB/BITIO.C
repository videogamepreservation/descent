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
 * $Source: f:/miner/source/pslib/rcs/bitio.c $
 * $Revision: 1.9 $
 * $Author: yuan $
 * $Date: 1993/10/22 17:51:09 $
 *
 * Contains all the necessary bit routines.
 * The file handling has been optimized and the pacifier has been removed.
 *
 * $Log: bitio.c $
 * Revision 1.9  1993/10/22  17:51:09  yuan
 * No major revisions
 * 
 * Revision 1.8  1993/10/18  17:59:55  yuan
 * Fixed memory alloc errors
 * 
 * Revision 1.7  1993/09/21  17:22:17  yuan
 * *** empty log message ***
 * 
 * Revision 1.6  1993/09/21  17:16:19  yuan
 * cleaning up
 * 
 * Revision 1.5  1993/09/20  12:25:39  yuan
 * ReadFile, WriteFile, AppendFile removed to readfile.lib
 * 
 * Revision 1.4  1993/09/14  13:06:25  yuan
 * CloseOutputBitFile no longer frees the buffer.
 * 
 * Revision 1.3  1993/09/09  17:42:02  yuan
 * tab added to ERROR messages
 * 
 * Revision 1.2  1993/09/09  12:38:28  yuan
 * WriteFile and AppendFile fixed.
 * 
 * Revision 1.1  1993/09/08  16:14:04  yuan
 * Initial revision
 * 
 * Revision 1.3  1993/07/24  19:04:52  yuan
 * *** empty log message ***
 * 
 * Revision 1.2  1993/07/22  11:25:35  yuan
 * No change
 * 
 * Revision 1.1  1993/07/21  15:28:13  matt
 * Initial revision
 * 
 *
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: bitio.c 1.9 1993/10/22 17:51:09 yuan Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>

#include "library.h"
#include "mem.h"
//#include "mem2.h"

#define PACIFIER_COUNT 2047

BIT_BUF *OpenOutputBitBuf( ) {
    BIT_BUF *bit_buf;

    //MALLOC( bit_buf, BIT_BUF, 1 );//Compile hack again -KRB
	bit_buf = (BIT_BUF *)malloc(1*sizeof(BIT_BUF));
    if ( bit_buf == NULL )
        return( bit_buf );
    bit_buf->current_byte = 0;
    bit_buf->rack = 0;
    bit_buf->mask = 0x80;
    bit_buf->pacifier_counter = 0;
    return( bit_buf );
}

BIT_BUF *OpenInputBitBuf( ubyte *buffer ) {
    BIT_BUF *bit_buf;

    //bit_buf = (BIT_BUF *) calloc( 1, sizeof( BIT_BUF ) );
  //  MALLOC(bit_buf, BIT_BUF, 1);//Compile hack again -KRB
	bit_buf = (BIT_BUF *)malloc(1*sizeof(BIT_BUF));
    if ( bit_buf == NULL )
        return( bit_buf );
    bit_buf->buf = buffer;
    bit_buf->current_byte = 0;
    bit_buf->rack = 0;
    bit_buf->mask = 0x80;
    bit_buf->pacifier_counter = 0;
    return( bit_buf );
}

void CloseOutputBitBuf( BIT_BUF *bit_buf ) {
    if ( bit_buf->mask != 0x80 )
        bit_buf->buf[bit_buf->current_byte++] = bit_buf->rack;
    free( bit_buf );
}

void CloseInputBitBuf( BIT_BUF *bit_buf )
{
    free( bit_buf->buf );
    free( bit_buf );
}

void OutputBit( BIT_BUF *bit_buf, int bit ) {
    if ( bit )
        bit_buf->rack |= bit_buf->mask;
    bit_buf->mask >>= 1;
    if ( bit_buf->mask == 0 ) {
        bit_buf->buf[bit_buf->current_byte++] = bit_buf->rack;
        bit_buf->rack = 0;
        bit_buf->mask = 0x80;
    }
}

void OutputBits( BIT_BUF *bit_buf, unsigned int code, int count ) {
    unsigned int mask;

    mask = 1L << ( count - 1 );
    while ( mask != 0) {
        if ( mask & code )
            bit_buf->rack |= bit_buf->mask;
        bit_buf->mask >>= 1;
        if ( bit_buf->mask == 0 ) {
            bit_buf->buf[bit_buf->current_byte++] = bit_buf->rack;
            bit_buf->rack = 0;
            bit_buf->mask = 0x80;
        }
        mask >>= 1;
    }
}

int InputBit( BIT_BUF *bit_buf ) {
    int value;

    if ( bit_buf->mask == 0x80 ) {
        bit_buf->rack = bit_buf->buf[bit_buf->current_byte++];
        if ( bit_buf->rack == EOF ) {
            printf( "    ERROR : Fatal error in InputBit!\n" );
            exit(4);
        }
    }
    value = bit_buf->rack & bit_buf->mask;
    bit_buf->mask >>= 1;
    if ( bit_buf->mask == 0 )
    bit_buf->mask = 0x80;
    return( value ? 1 : 0 );
}

unsigned int InputBits( BIT_BUF *bit_buf, int bit_count ) {
    unsigned int mask;
    unsigned int return_value;

    mask = 1L << ( bit_count - 1 );
    return_value = 0;
    while ( mask != 0) {
    if ( bit_buf->mask == 0x80 ) {
        bit_buf->rack = bit_buf->buf[bit_buf->current_byte++];
        if ( bit_buf->rack == EOF ) {
            printf( "    ERROR : Fatal error in InputBits!\n" );
            exit(5);
        }
	}
    if ( bit_buf->rack & bit_buf->mask )
            return_value |= mask;
        mask >>= 1;
        bit_buf->mask >>= 1;
        if ( bit_buf->mask == 0 )
            bit_buf->mask = 0x80;
    }
    return( return_value );
}
/*
void FilePrintBinary( FILE *file, unsigned int code, int bits ) {
    unsigned int mask;

    mask = 1 << ( bits - 1 );
    while ( mask != 0 ) {
        if ( code & mask )
            fputc( '1', file );
        else
            fputc( '0', file );
        mask >>= 1;
    }
} */
