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
 * $Source: f:/miner/source/pslib/rcs/library.h $
 * $Revision: 1.12 $
 * $Author: john $
 * $Date: 1994/02/15 12:53:23 $
 *
 * PSLIB and library generation tool header file.
 *
 * $Log: library.h $
 * Revision 1.12  1994/02/15  12:53:23  john
 * Made numfiles stored in library be a short instead of char.
 * 
 * Revision 1.11  1993/12/08  16:07:37  yuan
 * Changed MAX_FILES from 100 to 250. 
 * Incorporated the constant into FileList[MAX_FILES]
 * 
 * Revision 1.10  1993/10/27  12:42:50  yuan
 * Added prototype for ReadFileBuf.
 * 
 * Revision 1.9  1993/09/29  17:54:09  yuan
 * ReadFileRaw, etc. prototypes imported.
 * 
 * Revision 1.8  1993/09/27  17:12:48  yuan
 * Cleaned up... function prototypes internal to pslib.c removed.
 * 
 * Revision 1.7  1993/09/21  17:22:14  yuan
 * *** empty log message ***
 * 
 * Revision 1.6  1993/09/21  17:16:50  yuan
 * cleaning up
 * 
 * Revision 1.5  1993/09/21  17:07:11  yuan
 * broken and unbroken
 * 
 * Revision 1.4  1993/09/14  14:19:08  yuan
 * header fixed.
 * 
 * Revision 1.3  1993/09/14  13:16:37  yuan
 * Minor function prototype modifications were made.
 * 
 * Revision 1.2  1993/09/09  12:39:19  yuan
 * compression routine prototypes moved to pslib.h
 * 
 * Revision 1.1  1993/09/08  16:16:07  yuan
 * Initial revision
 * 
 *
 */

#include "cflib.h"
#include "time.h"
#include "types.h"

#define TICKER (*(volatile int *)0x46C)
#define USECS_PER_READING( start, stop, frames ) (((stop-start)*54945)/frames)
#define MSECS_TOTAL_TIME( start, stop ) ((stop - start)*55)

#define MAX_FILES 500

#define ERROR_OPENING_FILE  21
#define ERROR_WRITING_FILE  22
#define ERROR_READING_DATA -20

typedef struct bit_file {
    ubyte *buf;
    int current_byte;
    ubyte mask;
    int rack;
    int pacifier_counter;
    int length;
} BIT_BUF;

typedef struct lib_header {
   char id[4];      // set to 'PSLB'
   short nfiles;    // how many files in this library
} lib_header;

typedef struct file_header {
   char name[13];       // 8 chars, dot, extension, null
   byte compression;    // compression method
   int offset,          // where in the lib file
       length,          // how much space in lib taken by this file
       original_size;   // how long the actual (uncompressed) data is
//   time_t time;         // the date & time, from the time() function
   ushort date;         // the date
   ushort time;         // the time
   short ratio;         // this makes the structure 32 bytes
} file_header;

// Date and time macros

#define YEAR(t)    (((t & 0xFE00) >> 9) + 1980)
#define MONTH(t)   ((t & 0x01E0) >> 5)
#define DAY(t)     (t & 0x001F)
#define HOUR(t)    ((t & 0xF800) >> 11)
#define MINUTE(t)  ((t & 0x07E0) >> 5)
#define SECOND(t)  ((t & 0x001F) << 1)

// bitio function prototypes

BIT_BUF *OpenInputBitBuf( ubyte *buffer );
BIT_BUF *OpenOutputBitBuf();
void OutputBit( BIT_BUF *bit_file, int bit );
void OutputBits( BIT_BUF *bit_file, unsigned int code, int count );
int InputBit( BIT_BUF *bit_file );
unsigned int InputBits( BIT_BUF *bit_file, int bit_count );
void CloseInputBitBuf( BIT_BUF *bit_file );
void CloseOutputBitBuf( BIT_BUF *bit_file );
void FilePrintBinary( FILE *file, unsigned int code, int bits );

#define LISTING 1       // listing the library
#define BUILDING 1      // building the library
#define LF_LZW 1        // this file has LZW compression

#define MAX_FILE_SIZE 1024 * 100

// pslib function prototypes

int file_size( char *name );
void header_count( char *argv );
int read_data( FILE *fp, struct file_header *p );
void init_library( char *filename, int numfiles );
void write_file_header( char *filename, file_header Header );

//void print_usage( void );
//void check_list( char *argv );
//void list_files( void );
//void cfr_test( char *input, char *output );
//void cfw_test( char *input, char *output );
//void extract_test( char *extractname, char *extractout );
//void lib_read_test( char *extractname, char *extractout );
//void process_arg( char *argv );


extern char *Usage;
extern char *CompressionName;

extern int lib_flag;           // library flag
extern int b_flag;             // building flag
extern int c_flag;             // compression flag
extern int l_flag;             // listing flag
extern int lib_flag;           // library flag
extern FILE *InputLibFile;     // file to read from
extern FILE *OutputLibFile;    // file to write to
extern char *lib_name;         // name of the library
extern int file_count;         // number of files processed
extern int headers;            // number of header spaces allocated
extern file_header Header;     // Holds header info of file being processed
extern char *FileList[MAX_FILES];    // Contains the list of files being processed
extern file_header *LibHeaderList;
extern FILE *InputLibInitFile; // file to read from
extern short init_numfiles;    // number of files in the library

void *ReadFileRaw( char *filename, int *length );
// ReadFileRaw reads 'filename' and returns the buffer and passes the length
// in bytes.

int WriteFile( char *filename, void *data, int length );
// WriteFile writes 'length' bytes of 'data' to 'filename'
//  returns an error code != 0 if there is an error.

int AppendFile( char *filename, void *data, int length );
// AppendFile appends 'length' bytes of 'data' to 'filename'
//  returns an error code != 0 if there is an error.



