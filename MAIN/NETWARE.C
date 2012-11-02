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
 * $Source: f:/miner/source/main/rcs/netware.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:32:29 $
 * 
 * Code to get the user's login name from netware
 * 
 * $Log: netware.c $
 * Revision 2.0  1995/02/27  11:32:29  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.2  1994/07/23  23:53:56  matt
 * Code to get the user's name, not usable until we find a way to check
 * that the user is attached and logged into a Novell network.
 * 
 * Revision 1.1  1994/07/23  19:35:16  matt
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: netware.c 2.0 1995/02/27 11:32:29 john Exp $";
#pragma on (unreferenced)

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mono.h"

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   LONG;
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   char   SINT8;
typedef signed   short  SINT16;
typedef signed   long   SINT32;

#define MAXOBJNAME  48
#define BINDFUNC    227
#define CONNFUNC    220

/*----------------------------------------------------------------------*/
/*-- These structures are used by GetBinderyObjectName -----------------*/
/*----------------------------------------------------------------------*/

typedef struct {
        UINT16      length;
        UINT8       subfunc;
        UINT32      objID;
}NWGBONS;

typedef struct {
        UINT16      length;
        UINT32      objID;
        UINT16      objType;
        BYTE        objName[MAXOBJNAME];
}NWGBONR;

extern  SINT16  NWGetBinderyObjectName(UINT32, BYTE *, UINT16 *);

/*----------------------------------------------------------------------*/
/*-- These structures are used by GetBinderyObjectID -------------------*/
/*----------------------------------------------------------------------*/

typedef struct {
        UINT16      length;
        UINT8       subfunc;
        UINT16      objType;
        UINT8       objNLen;
        BYTE        objName[47];
}NWGBIDS;

typedef struct {
        UINT16      length;
        UINT32      objID;
        UINT16      objType;
        BYTE        objName[MAXOBJNAME];
}NWGBIDR;

extern  SINT16  NWGetBinderyObjectID(BYTE *, UINT16, UINT32 *);

/*----------------------------------------------------------------------*/
/*-- Functions in GETCONN.C --------------------------------------------*/
/*----------------------------------------------------------------------*/

extern  SINT16  NWGetConnectionNumber(void);

/*----------------------------------------------------------------------*/
/*-- These structures are used by NWGetConnectionInformation -----------*/
/*----------------------------------------------------------------------*/

typedef struct {
        UINT16      length;
        UINT8       subfunc;
        UINT8       connNum;
}NWGCIS;

typedef struct {
        UINT16      length;
        UINT32      objID;
        UINT16      objType;
        BYTE        objName[MAXOBJNAME];
        BYTE        loginTm[7];
        BYTE        reserved;
}NWGCIR;

SINT16  NWGetConnectionInformation(UINT16,BYTE *,UINT16 *,UINT32 *,BYTE *);

/*----------------------------------------------------------------------*/
/*-- This information is for the protected to real mode call -----------*/
/*----------------------------------------------------------------------*/

//
//  The following structure is passed to the DPMI function which will
//  execute the real-mode interrupt.  It will load these values into
//  the registers and call Btrieve.
//

typedef struct REAL_MODE_REGS{
        LONG        edi;            // Real Mode Registers ...
        LONG        esi;            // used for DPMI call 0x300
        LONG        ebp;
        LONG        reserved;
        LONG        ebx;
        LONG        edx;
        LONG        ecx;
        LONG        eax;
        WORD        flags;
        WORD        es;
        WORD        ds;
        WORD        fs;
        WORD        gs;
        WORD        ip;
        WORD        cs;
        WORD        sp;
        WORD        ss;
}RMREG;

extern  SINT16  NWRequest(BYTE function, void *request, void *reply);
extern  void    *NWGetRequestBuffer(void);
extern  void    *NWGetReplyBuffer(void);
extern  SINT16  NWInitializeXfaceForDPMICalls(void);

/*----------------------------------------------------------------------*/
/*-- These are helper functions ----------------------------------------*/
/*----------------------------------------------------------------------*/

UINT32  DWordSwap( UINT32 );
#pragma aux DWordSwap =                                 \
        0x8b 0xd0           /* mov  edx,eax      */     \
        0x86 0xe0           /* xchg ah,al        */     \
        0xb1 0x10           /* mov  cl,16        */     \
        0xd3 0xe0           /* shl  eax,cl       */     \
        0xd3 0xea           /* shr  edx,cl       */     \
        0x86 0xf2           /* xchg dh,dl        */     \
        0x0b 0xc2           /* or   eax,edx      */     \
        parm [eax] value [eax] modify [ecx edx];

UINT16  WordSwap( UINT16 );
#pragma aux WordSwap =                              \
        0x86 0xC4           /* xchg AL,AH   */      \
        parm [eax] value [ax];

/*----------------------------------------------------------------------*/

#define DEFPARA         64                      // 1K of RAM needed
#define DOS_INT         0x21                    // Shell Interrupt
#define TRUE            1
#define FALSE           0
#define DPMI            0x31    /* DPMI Interrupt             */
#define NULL            0

static  BYTE    *request;
static  BYTE    *reply;
static  WORD    DOSBuffSeg;             // DOS Buffer Segment
static  WORD    DOSBuffDes;             // DOS Buffer Selector
static  BYTE    IsInitialized;          // have we already setup
static  RMREG   rmreg;                  // the real mode register struct

void    *NWGetRequestBuffer()
{
        return request;
}

void    *NWGetReplyBuffer()
{
        return reply;
}

SINT16  NWInitializeXfaceForDPMICalls()
{
        union   REGS    regs;
        LONG    DOSBuffer;
        //
        //  First, get some DOS memory.  That is, memory that exists in
        //  the first meg of RAM.  This is where we'll store the request
        //  and reply buffers for the shell.  This is necessary since it
        //  can't see extended memory.
        //
        regs.x.eax = 0x100;
        regs.w.bx  = DEFPARA;
        int386(DPMI,&regs,&regs);
        if( regs.x.cflag ) return -1;
        //
        //  Save the DOS Buffer selector and the segment.
        //
        DOSBuffDes = regs.w.dx;
        DOSBuffSeg = regs.w.ax;
        //
        //  Convert the selector to a linear address so we can use it
        //  for copying data to/from the real mode buffer.
        //
        regs.x.eax = 0x6;
        regs.w.bx  = DOSBuffDes;
        int386(DPMI,&regs,&regs);
        if( regs.x.cflag ) return -1;
        //
        //  Setup our two pointers so they can be used by the protected mode
        //  application to copy data directly into the request and reply bufs
        //
        request = (BYTE *)((regs.w.cx << 16) | regs.w.dx);
        reply   = request + 512;
        //
        //  Show we have initialized successfully and return.
        //
        IsInitialized = TRUE;
        return 0;
}

SINT16  NWRequest(BYTE function, void *request, void *reply)
{
        union   REGS    regs;
        struct  SREGS   sregs;

        if( !IsInitialized ) return -1;
        //
        //  Get prepared to do the real mode interrupt.  Load the values
        //  we want loaded into the registers when the interrupt is done.
        //
        rmreg.eax = function << 8;
        rmreg.esi = NULL;                       // always at start of segment
        rmreg.edi = 512;                        // always at 512 from start
        rmreg.sp  = rmreg.ss  = NULL;           // use DPMI stack
        rmreg.ds  = rmreg.es  = DOSBuffSeg;     // our real mode segment
        //
        //  Now do the DPMI request to execute a real mode interrupt.
        //
        regs.w.ax  = 0x300;
        regs.h.bh  = 0;
        regs.h.bl  = DOS_INT;
        regs.w.cx  = NULL;
        regs.x.edi = (UINT32)&rmreg;
        segread(&sregs);
        int386x(DPMI,&regs,&regs,&sregs);
        //
        //  If carry set, DPMI error occurred, so return -1.
        //
        if( regs.x.cflag ) return -1;
        //
        //  Return whatever is in register AL.
        //
        return (SINT16)rmreg.eax & 255;
}

SINT16  NWGetConnectionNumber()
{
        return NWRequest( CONNFUNC, 0, 0 );
}

SINT16  NWGetConnectionInformation(UINT16 connNum,
                                   BYTE   *objName,
                                   UINT16 *objType,
                                   UINT32 *objID,
                                   BYTE   *loginTm)
{
        NWGCIS      *rq = (NWGCIS *)NWGetRequestBuffer();   // request packet
        NWGCIR      *rp = (NWGCIR *)NWGetReplyBuffer();     // reply packet
        SINT16      cc;                                     // completion code

        rq->subfunc = 22;
        rq->connNum = (UINT8)connNum;
        rq->length  = 2;
        rp->length  = 62;

        if( (cc = NWRequest(BINDFUNC, rq, rp)) ) return cc;

        if( objID )   *objID   = DWordSwap(rp->objID);
        if( objType ) *objType = WordSwap(rp->objType);
        if( objName ) strcpy(objName,rp->objName);
        if( objName ) memmove(loginTm,rp->loginTm,7);

        return cc;
}

SINT16  NWGetBinderyObjectName(UINT32 objID, BYTE *objName, UINT16 *objType)
{
        NWGBONS     *rq = (NWGBONS *)NWGetRequestBuffer();  // request packet
        NWGBONR     *rp = (NWGBONR *)NWGetReplyBuffer();    // reply packet
        SINT16      cc;                                     // completion code

        rq->subfunc = 54;
        rq->objID   = DWordSwap(objID);
        rq->length  = 5;
        rp->length  = 54;

        if( (cc = NWRequest(BINDFUNC, rq, rp)) ) return cc;

        if( objType ) *objType = WordSwap(rp->objType);
        if( objName ) strcpy(objName,rp->objName);

        return cc;
}


//returns pointer to user's name, or NULL if unavailable
//NOTE: This code isn't really usable until I find a way to first check
//if you are attached and logged into a Novell network.
char *NW_get_user_name()
{
	int     cc;
	static	char objName[48];
	WORD    objType;
	LONG    objID;
	BYTE    loginTm[7];
	int	conn_num;

	if( !IsInitialized )
		if( NWInitializeXfaceForDPMICalls() )
			return NULL;

	conn_num = NWGetConnectionNumber();

	if( NWGetConnectionInformation(conn_num,objName,&objType,&objID,loginTm) )
		return NULL;

	return objName;

}

