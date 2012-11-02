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
 * $Source: f:/miner/source/2d/rcs/vesalib.c $
 * $Revision: 1.3 $
 * $Author: john $
 * $Date: 1994/11/07 12:07:42 $
 * 
 * Routines to access VESA video modes.
 * 
 * $Log: vesalib.c $
 * Revision 1.3  1994/11/07  12:07:42  john
 * Made save/restore cursor work a bit better.
 * 
 * Revision 1.2  1994/09/29  10:09:26  john
 * Initial version, but not used.
 * 
 * Revision 1.1  1994/09/19  11:54:45  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: vesalib.c 1.3 1994/11/07 12:07:42 john Exp $";
#pragma on (unreferenced)


#include "dpmi.h"

typedef struct VgaInfoBlock {
	char				VESASignature[4];		// 4 signature bytes
	ushort			VESAVersion;			// VESA version number
	ushort			OEMStringPtrOffset;	// Pointer to OEM string
	ushort			OEMStringPtrSegment;	// Pointer to OEM string
	uint				Capabilities;			// Capabilities of the video environment
	ushort			VideoModePtrOffset;	// Pointer to supported Super VGA modes
	ushort			VideoModePtrSegment;	// Pointer to supported Super VGA modes
	ushort			TotalMemory;			// Number of 64K memory blocks on board
	ubyte				Reserved[242];			//	Reserved for future use.
} VgaInfoBlock;

typedef struct ModeInfoBlock {
	ushort			ModeAttributes;
	ubyte				WinAAttributes;
	ubyte				WinBAttributes;
	ushort			WinGranularity;
	ushort			WinSize;
	ushort			WinASegment;
	ushort			WinBSegment;
	ushort			WinFuncPtrOffset;
	ushort			WinFuncPtrSegment;
	ushort			BytesPerScanLine;

	ushort			Xresolution;
	ushort			Yresolution;
	ubyte				Xcharsize;
	ubyte				Ycharsize;
	ubyte				NumberOfPlanes;
	ubyte				BitsPerPixel;
	ubyte				NumberOfBanks;
	ubyte				MemoryModel;
	ubyte				BankSize;
	ubyte				dummy[227];
} ModeInfoBlock;

//  0=Mode set OK
//  1=No VGA adapter installed
//  2=Program doesn't support this VESA granularity
//  3=Monitor doesn't support that VESA mode.:
//  4=Video card doesn't support that VESA mode.
//  5=No VESA driver found.
//  6=Bad Status after VESA call/
//  7=Not enough DOS memory to call VESA functions.
//  8=Error using DPMI.
//  9=Error setting logical line width.
// 10=Error allocating selector for A0000h
// 11=Not a valid mode support by gr.lib
// Returns one of the above without setting mode

void *pDosBuffer=NULL;
VgaInfoBlock vesa_info_block;
ModeInfoBlock vesa_mode_info_block;
ushort RealSelector;
ushort * SupportedModes=NULL;

short gr_vesa_check_mode(short mode)
{
	dpmi_real_regs	rr;

	if(pDosBuffer==NULL)	{
		pDosBuffer = dpmi_real_malloc( 256, &RealSelector );
		if (!pDosBuffer)
			Error( "Error allocating DOS buffer" );
	}
	memset( &rr, 0, sizeof(dpmi_real_regs);		// Clear real registers...
	rr.eax = 0x4f00;	// Return SuperVGA information
	rr.es = DPMI_real_segment(pDosBuffer);
	rr.edi = DPMI_real_offset(pDosBuffer);
	
	dpmi_real_int386( 0x10, &rr );
									
	if ((rr.eax & 0xffff) != 0x4f )	{
		return 5;		// No VESA driver found										
	}
		
	memcpy( &vesa_info_block, pDosBuffer, sizeof(VgaInfoBlock) );
					
	// Make sure there is a VESA signature
	if (!memcmp( vesa_info_block.VESASignature, "VESA" ))
		return 5;			// No VESA driver found

	// We now have a valid VESA driver loaded, so search through the
	// list of supported modes, searching for the mode we want.
	SupportedModes	= (ushort *)((vesa_info_block.VideoModePtrSegment*16)+vesa_info_block.VideoModePtrOffset);
		
	while( *SupportedModes != 0xffff )	{
		if (*SupportedModes==mode)	{
			break;
		}	
		SupportedModes++;
	}
	if (*SupportedModes!=mode)	{
		return 4;		// Video card doesn't support that VESA mode.
	}
			
	memset( &rr, 0, sizeof(dpmi_real_regs);		// Clear real registers...
	rr.eax = 0x4f01;	// Return SuperVGA mode information
	rr.ecx = mode;		// Super VGA video mode
	rr.es = DPMI_real_segment(pDosBuffer);
	rr.edi = DPMI_real_offset(pDosBuffer);
	
	dpmi_real_int386x( 0x10, &rr );
							
	if ((rr.eax & 0xffff) != 0x4f )	{
		return 5;		// No VESA driver found										
	}

	memcpy( &vesa_mode_info_block, pDosBuffer, sizeof(ModeInfoBlock) );
	
	if (!(vesa_mode_info_block.ModeAttributes&1) )	{
		return 3;		// Monitor doesn't support this mode.
	}

	switch( vesa_mode_info_block.WinGranularity )	{
	case 64:	PageSizeShift = 0; break;
	case 32:	PageSizeShift = 1; break;
	case 16:	PageSizeShift = 2; break;
	case 8:	PageSizeShift = 3; break;
	case 4:	PageSizeShift = 4; break;
	case 2:	PageSizeShift = 5; break;
	case 1:	PageSizeShift = 6; break;
	default:	
		return 2;		// Granularity not supported.
	}
	
	return 0;
}

typedef struct VgaInfoBlock {
	char				VESASignature[4];		// 4 signature bytes
	ushort			VESAVersion;			// VESA version number
	ushort			OEMStringPtrOffset;	// Pointer to OEM string
	ushort			OEMStringPtrSegment;	// Pointer to OEM string
	uint				Capabilities;			// Capabilities of the video environment
	ushort			VideoModePtrOffset;	// Pointer to supported Super VGA modes
	ushort			VideoModePtrSegment;	// Pointer to supported Super VGA modes
	ushort			TotalMemory;			// Number of 64K memory blocks on board
	ubyte				Reserved[242];			//	Reserved for future use.
} VgaInfoBlock;

void * gr_save_mode_vesa()
{
	void * pDosBuffer;
	ushort DosSelector;
	dpmi_real_regs	rr;
	VgaInfoBlock vesa_info_block;
	int save_size;
	
	pDosBuffer = dpmi_real_malloc(sizeof(VgaInfoBlock), &DosSelector);
	if (!pDosBuffer)
		return NULL;

	memset( &rr, 0, sizeof(dpmi_real_regs));		// Clear real registers...
	rr.eax = 0x4f00;	// Return SuperVGA information
	rr.es = DPMI_real_segment(pDosBuffer);
	rr.edi = DPMI_real_offset(pDosBuffer);
	dpmi_real_int386x( 0x10, &rr );

	memcpy( &vesa_info_block, pDosBuffer, sizeof(VgaInfoBlock) );
	dpmi_real_free(DosSelector);
	if ((rr.eax & 0xffff) != 0x4f )	{
		return NULL;		// No VESA driver found										
	}
	// Make sure there is a VESA signature
	if (!memcmp( vesa_info_block.VESASignature, "VESA", 4 ))
		return NULL;			// No VESA driver found
	
	memset( &rr, 0, sizeof(dpmi_real_regs));		// Clear real registers...
	rr.eax = 0x4f04;	// Save/Restore SuperVGA video state
	rr.edx = 00;		// Return save/restore buffer size
	rr.ecx = 0xf;		// Save all states
	dpmi_real_int386x( 0x10, &rr );
	if ((rr.eax & 0xffff) != 0x4f )	{
		return NULL;		// No VESA driver found										
	}
	save_size = (rr.ebx & 0xffff)*64;		// How many bytes save buffer requires...

	pDosBuffer = dpmi_real_malloc(save_size, &DosSelector);
	if (!pDosBuffer)
		return NULL;

	memset( &rr, 0, sizeof(dpmi_real_regs));		// Clear real registers...
	rr.eax = 0x4f04;	// Save/Restore SuperVGA video state
	rr.edx = 1;			// Save video state
	rr.ecx = 0xf;		// Save all states

	rr.es = DPMI_real_segment(pDosBuffer);
	rr.ebx = DPMI_real_offset(pDosBuffer);
	dpmi_real_int386x( 0x10, &rr );
	if ((rr.eax & 0xffff) != 0x4f )	{
		dpmi_real_free(DosSelector);
		return NULL;		// No VESA driver found										
	}
	gr_restore_mode_vesa( pDosBuffer );
	return pDosBuffer;
}

void gr_restore_mode_vesa(void * buffer)
{
	void * pDosBuffer;
	ushort DosSelector;
	dpmi_real_regs	rr;
	VgaInfoBlock vesa_info_block;
	
	pDosBuffer = dpmi_real_malloc(sizeof(VgaInfoBlock), &DosSelector);
	if (!pDosBuffer)
		return;

	memset( &rr, 0, sizeof(dpmi_real_regs));		// Clear real registers...
	rr.eax = 0x4f00;	// Return SuperVGA information
	rr.es = DPMI_real_segment(pDosBuffer);
	rr.edi = DPMI_real_offset(pDosBuffer);
	dpmi_real_int386x( 0x10, &rr );

	memcpy( &vesa_info_block, pDosBuffer, sizeof(VgaInfoBlock) );
	dpmi_real_free(DosSelector);
	if ((rr.eax & 0xffff) != 0x4f )	{
		return;		// No VESA driver found										
	}
	// Make sure there is a VESA signature
	if (!memcmp( vesa_info_block.VESASignature, "VESA", 4 ))
		return;			// No VESA driver found
	
	memset( &rr, 0, sizeof(dpmi_real_regs));		// Clear real registers...
	rr.eax = 0x4f04;	// Save/Restore SuperVGA video state
	rr.edx = 2;			// Restore video state
	rr.ecx = 0xf;		// Save all states
	rr.es = DPMI_real_segment(buffer);
	rr.ebx = DPMI_real_offset(buffer);
	dpmi_real_int386x( 0x10, &rr );
	if ((rr.eax & 0xffff) != 0x4f )	{
		return;		// No VESA driver found										
	}
	return;
}

