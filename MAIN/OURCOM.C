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
 * $Source: f:/miner/source/main/rcs/ourcom.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:27:36 $
 * 
 * Modem support code
 * 
 * $Log: ourcom.c $
 * Revision 2.0  1995/02/27  11:27:36  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.1  1994/10/09  22:29:00  rob
 * Initial revision
 * 
 * Revision 1.7  1994/09/27  15:05:54  rob
 * Last checkin had error.
 * 
 * Revision 1.6  1994/09/27  15:03:12  rob
 * Null modem basic routines working.  Sending DEAD messages and
 * missiles still need to be done.
 * 
 * Revision 1.5  1994/09/24  16:52:20  rob
 * Added stubbed funcs for startup and stop of serial games.
 * 
 * Revision 1.4  1994/09/24  15:10:21  rob
 * Removed some compiler warnings.
 * 
 * Revision 1.3  1994/09/24  14:40:34  rob
 * Sending shortpos structures.  9600 baud max.
 * 
 * Revision 1.2  1994/09/22  17:53:11  rob
 * First revision.  Not functional yet.
 * 
 * Revision 1.1  1994/09/21  13:23:25  rob
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: ourcom.c 2.0 1995/02/27 11:27:36 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "modem.h"
#include "object.h"
#include "player.h"
#include "laser.h"
#include "error.h"
#include "network.h"
#include "mono.h"
#include "gauges.h"
#include "newmenu.h"
#include "gamesave.h"
#include "netmisc.h"
#include "dpmi.h"
#include "commlib.h"

/*
	Code to support modem/null-modem play
 */

extern int Spreadfire_toggle;

static long com_port;
long serial_active;
long com_port_num;
long com_speed;
long sync_time;
long com_open = 0;
static long com_vect;
static long com_type; /* What type of UART is available */
static char syncbuffer[MAX_SER_MSG_LEN+1];
static long synccnt;
static long synclen;
volatile char inbuf[SERIAL_BUFFER_SIZE];  // Serial input buffer
volatile char outbuf[SERIAL_BUFFER_SIZE]; // Serial output buffer
volatile long inbufplc, inbufend, outbufplc, outbufend;
volatile char line_stat, modem_stat;
volatile char modem_alert;
long comi;  // counter used in ISR
int Other_player; // Player num for modem opponent

long MyObjnum, OtherObjnum;

void (__interrupt __far *oldcomhandler)();
void __interrupt __far comhandler(void);
																			 

void reset_player_object(void); // In object.c but not in object.h

/* Function prototypes for functions not exported through modem.h */

void serial_param_setup(void);
void serial_link_start(void);
void modem_dialout(void);
void modem_answer(void);
long serial_get_sync_packet(void);
void com_do_sync(int nitem, newmenu_item *menus, int *key, int citem);

	
com_enable () // Detect and enable the COM port selected by the user
{
	// com_port_num and com_speed should be set before calling this func

	long divisor;
	short *ptr;

	if (com_open)
		return 0;

	/* Set the variables for the selected com port */
	if ( (com_port_num < 1) || (com_port_num > 4) )
		return -1;

	com_vect = 0xb+(com_port_num & 1); // Interrupt vector

	ptr = (short *) (0x400L+(long)((com_port_num-1)<<1));
	com_port = (long)(*ptr); // COM I/O Port Addr
	if (com_port == 0) return -1;
 
	/* Set the speed of the COM port UART, saving the old value */
	if ( (com_speed <= 0) || (com_speed > 115200) )
 		return -1;
	
	// Enable the interrupts for the COM port 

	_disable();

	outp(com_port+3, 0x080); /* Enable access to the baud divider regs */
	divisor = 115200/com_speed;
	outp(com_port+0, divisor&255); /* Plug in our new baud rate */
	outp(com_port+1, divisor>>8);
	outp(com_port+3, 0x03);   /* 8,N,1, also turns off divisor access */

	// Check UART type, enable FIFO if 16550

	outp(com_port+2, 0x81);  /* Enable FIFO with trigger level of 8 */
	if ((inp(com_port+2) & 0xf8) == 0xc0)
	{
		com_type = 16550;
		mprintf(0,"16550 uart found.\n"); // debug
	}	
	else 
	{
		com_type = 16450;
		mprintf(0,"16450 uart found.\n"); // debug
	}
	
	/* register new com interrupt handler */

	oldcomhandler = _dos_getvect(com_vect);
	_dos_setvect(com_vect, serial_isr);

	/* Enable UART generation of interrupts, but select none for now */
	
	outp(com_port+4, 0x0b);
	outp(com_port+1, 0);	   
	
	
	/* Unmask vector in PIC */
	outp(0x21, inp(0x21)&(255-(1 << (com_vect&7))));

	line_stat = inp(com_port+5);
	modem_stat = inp(com_port+6);

	inp(com_port+0); /* eat any pending char */
	inp(com_port+2); /* ignore any existing interrupts */
	
	/* Enable specific UART interrupts */
	
	outp(com_port+1, 0x03); /* RxRDY and TBE are enabled */

	atexit(com_disable);

	_enable();

	mprintf(0, "Com port opened.\n");
	com_open = 1;

	return 0;
}

void
com_disable()
{
	if (!com_open) 
		return;

	if ((inp(com_port+5)&0x20) > 0)
	{
		if (outbufplc != outbufend)
		{
			outp(com_port, outbuf[outbufplc]);
			outbufplc = ((outbufplc+1)&(SERIAL_BUFFER_SIZE-1));
		}
	}
		
	while (outbufplc != outbufend) ; // wait for buffer to drain

	_disable();
	outp(0x21,inp(0x21)|(1<<(com_vect&7)));          //Mask vector
	outp(com_port+1,0);
	outp(com_port+4,0);
	_dos_setvect(com_vect, oldcomhandler);
	_enable();
	mprintf(0, "Com port released.");
	com_open = 0;
}

com_reset_buffers(void)
{
	inbufend = inbufplc = 0;
}


com_send(long dat, long numbytes)
{
	long i;

	i = 0;
	while (numbytes > 0)
	{
		outbuf[outbufend] = (char)((dat>>i)&255);
		outbufend = ((outbufend+1)&(SERIAL_BUFFER_SIZE-1));

		numbytes--;
		i += 8;
	}

	if ((inp(com_port+5)&0x20) > 0)
	{
		if (outbufplc != outbufend)
		{
			outp(com_port,outbuf[outbufplc]);
			outbufplc = ((outbufplc+1)&(SERIAL_BUFFER_SIZE-1));
		}
	}
	return ;
}

com_send_ptr(char *ptr, long len)
{
	long i = 0;

	while (i < len)
	{
		outbuf[outbufend] = (char)ptr[i];
		outbufend = ((outbufend+1)&(SERIAL_BUFFER_SIZE-1));
		if (outbufend == outbufplc)
			mprintf(0, "possible buffer wraparound\n");
		i++;
	}

	if ((inp(com_port+5)&0x20) > 0)
	{
		if (outbufplc != outbufend)
		{
			outp(com_port, outbuf[outbufplc]);
			outbufplc = ((outbufplc+1)&(SERIAL_BUFFER_SIZE-1));
		}
	}
	return ;
}

		
			  	
com_send_string(char *string, int len)
{
	long i;

	i = 0;
	while (i < len)
	{
		outbuf[outbufend] = string[i];
		outbufend = ((outbufend+1)&(SERIAL_BUFFER_SIZE-1));
		i++;
	}
	if ((inp(com_port+5)&0x20) > 0)
	{
		if (outbufplc != outbufend)
		{
			outp(com_port,outbuf[outbufplc]);
			outbufplc = ((outbufplc+1)&(SERIAL_BUFFER_SIZE-1));
		}
	}
	return ;
}

comgetchar()
{
	short i;

	i = -1;
	if (inbufplc != inbufend)
	{
		i = (short)inbuf[inbufplc];
		inbufplc = ((inbufplc+1)&(SERIAL_BUFFER_SIZE-1));
	}
	return(i);
}

comgetchar_timed(long count)
{
	short i;

	i = -1;

	Assert(count > -1);

	while ((inbufplc == inbufend) && count--) ;

	if (inbufplc != inbufend)
	{
		i = (short)inbuf[inbufplc];
		inbufplc = ((inbufplc+1)&(SERIAL_BUFFER_SIZE-1));
	}
	return(i);
}

comwaitfor(char string[81]) // Wait for reply on serial (smartmodem control)
{
	short dat;
	long i;

	i = 0;
	while ((string[i] != 0) && (kbhit() == 0))
	{
		if ((dat = comgetchar()) >= 0)
		{
			printf("%c",dat);
			if (((char)dat) == string[i])
				i++;
		}
	}
	return(0);
}

void __interrupt __far serial_isr(void)
{
	while (1)
	{
		switch(inp(com_port+2)&7)
		{
			case 4: // receive empty
			 do
			 {
				 inbuf[inbufend] = inp(com_port);
				 inbufend = ((inbufend+1)&(SERIAL_BUFFER_SIZE-1));
			 } while (com_type==16550 && (inp(com_port+5)&1));
			 break;
			case 2: /* X-mit buffer empty */
			 if (outbufplc != outbufend)
			 {
				 if (com_type == 16550)
					 comi = 16;
				 else
					 comi = 1;
				 do
			    {
					 outp(com_port,outbuf[outbufplc]);
					 outbufplc = ((outbufplc+1)&(SERIAL_BUFFER_SIZE-1));
			    } while ((--comi) && (outbufplc != outbufend));
			 }
			 break;
			case 6:
			 line_stat = inp(com_port+5);
			 modem_alert = 1;
			 break;
			case 0: /* RS-232 status change */
			 modem_stat = inp(com_port+6); 
			 modem_alert = 1;
			 break;
			default:
			 outp(0x20, 0x20); // Clear interrupt
			 return;
		}			
	}
}

com_get_fire(void)
{
	ushort check;

	mprintf(0, "Entered com_get_fire.\n");
 
	// Check checksum
	check = netmisc_calc_checksum(syncbuffer, 5);
	if (check != *(ushort *)(syncbuffer+5))
	{
		// Checksum failure
		synccnt = 0;
		mprintf(0, "Bad checksum on fire.\n");
		return;
	}

	// Act out the actual shooting
	
	do_laser_firing(OtherObjnum, (int)syncbuffer[2], (int)syncbuffer[3], (int)syncbuffer[4]);
}

void
com_get_ppos(void)
{		
	ushort check;

	// Check checksum
	check = netmisc_calc_checksum(syncbuffer, 18);
	if (check != *(ushort *)(syncbuffer+18))
	{
		// Checksum failure
		synccnt = 0;
		mprintf(0, "Bad checksum on ppos.\n");
		return;
	}
	
	extract_shortpos(Objects+OtherObjnum, (shortpos *)(syncbuffer+1));
	obj_relink(OtherObjnum, Objects[OtherObjnum].segnum);
}

void
com_process_input(void)
{
	// Read all complete messages from the serial buffer and process
	// the contents.

	short dat;
	char temp_char;

	while ( (dat = comgetchar()) >= 0) // Returns -1 when serial pipe empty
	{
		syncbuffer[synccnt++] = dat;
		if (synccnt == 1)
		{
			switch(dat)
			{
				case SER_PPOS: synclen = 20; break;
				case SER_FIRE: synclen = 7;  break;
				case SER_MESS: synclen = MAX_MESSAGE_LEN+1; break;
				case SER_KILL: synclen = 2;  break;
				case SER_QUIT: synclen = 1;  break; 
				default: mprintf(0,"%c", dat);
					synccnt = 0;
					synclen = 1;
					break;
			}
		}
		if (synccnt == synclen)
		{
			switch(syncbuffer[0])
			{
				case SER_PPOS: // Player position
					com_get_ppos();
					break;
				case SER_FIRE:
					com_get_fire();
					break;
				case SER_MESS:  // Message sending
					HUD_init_message("%s says '%s'", Players[temp_char].callsign,
						(char *)syncbuffer+2);					
					break;
				case SER_KILL:  // 'I got killed' message
					temp_char= (char)syncbuffer[2];     // who killed him
					if (temp_char != Player_num) 
					{  // Not suicide
						Players[temp_char].net_kills_total++;
						Players[temp_char].net_kills_level++;
						HUD_init_message("%s killed %s!",
							(temp_char == Player_num) ? "you" : Players[temp_char].callsign,
							Players[temp_char].callsign);
					}
					else
					{
						HUD_init_message("%s comitted suicide!", Players[temp_char].callsign);
					}
					break;
				case SER_QUIT: // Other player has left the game!
					temp_char = (char)syncbuffer[1];     // size 1, player id
					// More is needed here
					HUD_init_message("%s has left the game!", Players[temp_char].callsign);
					break;
 
			}
			synccnt = 0;
		}
	}

	com_send_status();
	return ;
}


com_send_fire(void)
{
	static char buf[20];

	buf[0] = (char)SER_FIRE;
	buf[1] = (char)Network_laser_fired;
	buf[2] = (char)Network_laser_gun;
	buf[3] = (char)Network_laser_level;
	buf[4] = (char)Network_laser_flags;
	*(ushort *)(buf+5) = netmisc_calc_checksum(buf, 5);
	com_send_ptr(buf, 7);
}

com_send_mypos(void)
{
	char buf[20];

	buf[0] = (char)SER_PPOS;
	create_shortpos((shortpos *)(buf+1), Objects+MyObjnum); // size 17
	*(short *)(buf+18)=netmisc_calc_checksum(buf, 18);
	com_send_ptr(buf, 20);
}

com_send_status(void)
{


	static fix last_comm_time = 0;

	last_comm_time += FrameTime;

	if ((last_comm_time < MIN_COMM_GAP) && !Network_laser_fired)
	 	return;

	last_comm_time = 0;

	// Send the data for my current position through the serial 

	com_send_mypos();

	// If we fired this frame, send FIRE messages
	
	if (Network_laser_fired)
	{
		com_send_fire();
		Network_laser_fired = 0;
	}


	// Send network messages, if any were sent this frame

	if (Network_message_reciever != -1) 
	{
		Network_message_reciever = -1;
		com_send((long)SER_MESS, 1);
		com_send((long)Player_num, 1);
		com_send_string(Network_message, MAX_MESSAGE_LEN);
	}

	// No other message types are sent here.  SER_KILL messages are sent
	// at time of 'termination'.  SER_QUIT messages are sent when leaving.	

	return;
}


// Stubs for future functions

void
modem_stop_serial(void)
{
	
}

void
modem_stop_modem(void)
{

}

#define ADD_ITEM(t,value,key)  do { m[num_options].type=NM_TYPE_MENU; m[num_options].text=t; menu_choice[num_options]=value;num_options++; } while (0)

#define MENU_SERIAL_SETUP 			0
#define MENU_SERIAL_LINK_START 	1
#define MENU_MODEM_CALL				2
#define MENU_MODEM_ANSWER			3
#define MENU_RETURN_TO_MAIN 		4

#define SER_SYNC_LEN 35

void
modem_start_game(void)
{
	newmenu_item m[10];
	int menu_choice[10];
	int num_options = 0;
	int choice;


	ADD_ITEM("Setup serial parameters...", MENU_SERIAL_SETUP, -1);
	ADD_ITEM("Start serial game...", MENU_SERIAL_LINK_START, -1);
	ADD_ITEM("Dial remote modem...", MENU_MODEM_CALL, -1);
	ADD_ITEM("Answer modem call...", MENU_MODEM_ANSWER, -1);

	ADD_ITEM("Return to game...", MENU_RETURN_TO_MAIN, -1);

	choice = newmenu_do("SERIAL OPTIONS", NULL, num_options, m, NULL);

	if (choice > -1) 
	{
		switch (choice)
		{
			case MENU_SERIAL_SETUP:
			 	serial_param_setup();
				break;
			case MENU_SERIAL_LINK_START:
				serial_link_start();
				break;
			case MENU_MODEM_CALL:
				modem_dialout();
				break;
			case MENU_MODEM_ANSWER:
				modem_answer();
				break;
			default: 
				return;
		}
	}
}

void serial_param_setup(void)
{

}

// Handshaking to start a serial game, 2 players only

void com_do_sync(int nitems, newmenu_item *menus, int *key, int citem)
{

	int result;	

	menus = menus;
	nitems = nitems;
	citem = citem;
	
	serial_send_my_sync_packet();
		
	result = serial_get_sync_packet();

	if (result == -1)
		return;

	*key = -2;

	mprintf(0, "Got sync packet.\n");

	serial_send_my_sync_packet();  // One last time for good measure

	com_reset_buffers();

	Game_mode |= GM_SERIAL;

}

void serial_link_start(void)
{
	int i, j;
	int choice;
	newmenu_item m[2];

	if (!serial_active) return;

	sync_time = (long)time(0);

	com_enable(); // Open COM port as configured

	N_players = 2;

	m[0].type=NM_TYPE_MENU;
	m[0].text="Cancel";
	choice = newmenu_do("WAIT", "Waiting for opponent...", 1, m, com_do_sync);
	if (choice !=-2) {
		Game_mode = GM_NORMAL;
		N_players = 0;
		return;
	}

	Assert(Game_mode & GM_SERIAL);

	// Setup the serial game

	load_game(Gamesave_current_filename);

	network_delete_extra_objects(); // Removes monsters from level
	
	j = 0;
	for (i = 0; i < 2; i++)
	{
		while (Objects[j].type != OBJ_PLAYER)
		{
			j++;
			Assert( j < MAX_OBJECTS);
		}
		Players[i].objnum = j;
		mprintf(0,"Player %d is object %d.\n", i, j);
		j++;
	}
		
	MyObjnum = Players[Player_num].objnum;
	OtherObjnum = Players[Other_player].objnum;

	mprintf(0, "I am object number %d.\n", MyObjnum);
	mprintf(0, "Other guy is obj   %d.\n", OtherObjnum);
	mprintf(0, "Other Player num   %d.\n", Other_player);

	init_player_stats_game();

	gameseq_remove_unused_players();

	Viewer = ConsoleObject = &Objects[MyObjnum];

	ConsoleObject->control_type = CT_FLYING;

	reset_player_object();

	Objects[OtherObjnum].control_type = CT_NONE;
	Objects[OtherObjnum].movement_type = MT_PHYSICS;

}


void
serial_send_my_sync_packet(void)
{
	char buf[SER_SYNC_LEN];

	// Build my sync packet

	buf[0] = (char)SER_SYNC;
	*(long *)(buf+1) = (long)sync_time;
	strcpy(buf+5, network_get_callsign());
	strcpy(buf+17, Gamesave_current_filename); // 16 bytes
	// add a checksum, 16-bit CRC
	*(ushort *)(buf+33) = netmisc_calc_checksum(buf, 33);
	com_send_ptr(buf, SER_SYNC_LEN);
}

long
serial_get_sync_packet(void)
{
	char syncbuf[80];
	static long loc_synccnt = 0;
	long other_sync_time;
	ushort checksum;
	short dat;
	
 	while ( (dat = comgetchar_timed(1000000L)) >= 0) // returns -1 when timed out
 	{
 		syncbuf[loc_synccnt++] = dat;
 		if (loc_synccnt == 1) 
 		{
 			if (dat != SER_SYNC)
 			{
 				loc_synccnt = 0;
 					
 			}
 		}
 		if (loc_synccnt == SER_SYNC_LEN)
 		{
			// Check the checksum before we act on this packet
			checksum = netmisc_calc_checksum(syncbuf, 33);
			if (checksum != *(ushort *)(syncbuf+33))
			{
				// Checksum error
				mprintf(0, "Checksum failure on SYNC.  %d!=%d.\n",
					checksum, *(ushort *)(syncbuf+33));

				loc_synccnt = 0;
				continue;
			}

			other_sync_time = *(long *)(syncbuf+1);
			if (other_sync_time > sync_time) 
			{
				// I am the master
				Player_num = 0;
				Other_player = 1;
			}
			else if (other_sync_time < sync_time)
			{									 											  	
				// I am the slave
				Player_num = 1;
				Other_player = 0;
				strcpy(Gamesave_current_filename, (char *)syncbuf+17);
			}
			else 
			{
				// Time times were equal!
				sync_time = (long)time(0);
				loc_synccnt = 0;
				return -1;
			}
			
			// Fill in opponent's Player structure
			strcpy(Players[Other_player].callsign, (char *)syncbuf+5);
			mprintf(0, "Opponent is named %s.\n", Players[Other_player].callsign);

			return (0);			
		}
	}
	return (-1);
}

void modem_dialout(void)
{

}

void modem_answer(void)
{

}


