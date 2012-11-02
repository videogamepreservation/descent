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
#include <stdio.h>
#include <stdlib.h>

#include "dpmi.h"

typedef struct {
	char unit;
	short dev_offset;
	short dev_segment;
} dev_list;

typedef struct _Dev_Hdr {
	struct _Dev_Hdr far *dev_next;
	unsigned short dev_att;
	ushort dev_stat;
	ushort dev_int;
	char dev_name[8];
	short dev_resv;
	char dev_letr;
	char dev_units;
} dev_header;
	

void find_cdrom()
{
	dpmi_real_regs rregs;
		
	// Get dos memory for call...
	dev_list * buf;
	dev_header *device;
	int num_drives, i;
	unsigned cur_drive, cdrom_drive;
	FILE *fp;


	memset(&rregs,0,sizeof(dpmi_real_regs));
	rregs.eax = 0x1500;
	rregs.ebx = 0;
	dpmi_real_int386x( 0x2f, &rregs );
	if ((rregs.ebx & 0xffff) == 0) {
		printf ("Error: no cd-rom driver detected.");
		exit(1);
	}
	num_drives = rregs.ebx;

	buf = (dev_list *)dpmi_get_temp_low_buffer( sizeof(dev_list)*26 );	

	rregs.es = DPMI_real_segment(buf);
	rregs.ebx = DPMI_real_offset(buf);
	rregs.eax = 0x1501;
	dpmi_real_int386x( 0x2f, &rregs );
	cdrom_drive = 0;
	for (i = 0; i < num_drives; i++) {
		device = (dev_header *)((buf[i].dev_segment<<4)+ buf[i].dev_offset);
		_dos_setdrive(device->dev_letr);
		_dos_getdrive(&cur_drive);
		if (cur_drive == device->dev_letr) {
			//if (!chdir("\\descent")) {
			//if (pig_found)
			cdrom_drive = device->dev_letr;
			printf( "Found cd-rom drive %c:\n", cdrom_drive + 'A' -1 );
		}				
	}
	if (cdrom_drive == 0)
		printf("Descent CD not detected in any connected CDROM device\n");
}

void main()
{
	find_cdrom();
}

