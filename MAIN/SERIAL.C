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
 * $Source: f:/miner/source/main/rcs/serial.c $
 * $Revision: 2.1 $
 * $Author: mike $
 * $Date: 1995/03/30 16:36:21 $
 * 
 * .
 * 
 * $Log: serial.c $
 * Revision 2.1  1995/03/30  16:36:21  mike
 * text localization.
 * 
 * Revision 2.0  1995/02/27  11:27:38  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.7  1994/11/19  15:15:11  mike
 * remove unused code and data
 * 
 * Revision 1.6  1994/11/12  19:58:17  john
 * Turned off stack checking around interrupt.
 * 
 * Revision 1.5  1994/11/12  19:47:20  matt
 * Added paren
 * 
 * Revision 1.4  1994/11/12  19:46:16  john
 * Fixed buug that didn't lock down isr data.
 * 
 * Revision 1.3  1994/11/12  17:59:54  john
 * Added code to lock down interrupt code and data for serial isr.
 * 
 * Revision 1.2  1994/07/21  21:31:32  john
 * First cheapo version of VictorMaxx tracking.
 * 
 * Revision 1.1  1994/07/21  18:40:38  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: serial.c 2.1 1995/03/30 16:36:21 mike Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include "serial.h"
#include "dpmi.h"

typedef struct  {
	char buffer[256];
	unsigned char write_index;
	unsigned char read_index;
} BUFFER;

typedef struct  {
	void (interrupt far * old_vector)();
	int uart_base;
	int irq_mask;
	int interrupt_number;
	BUFFER in;
	BUFFER out;
} PORT;


#define RBR                 0   /* Receive buffer register */
#define THR                 0   /* Trasmit holding register */
#define IER                 1   /* Interrupt Enable reg. */
#define IER_RX_DATA         1   /* Enable RX interrupt bit */
#define IER_THRE            2   /* Enable TX interrupt bit */
#define IIR                 2   /* interrupt ID register */
#define IIR_MODEM_STATUS    0   /* Modem stat. interrupt ID */
#define IIR_TRANSMIT        2   /* Transmit interrupt ID */
#define IIR_RECEIVE         4   /* Receive interrupt ID */
#define IIR_LINE_STATUS     6   /* Line stat. interrupt ID */
#define LCR                 3   /* Line control register */
#define LCR_DLAB            0x80 /* Divisor access bit */
#define LCR_EVEN_PARITY     0x18 /* Set parity 'E' bits */
#define LCR_ODD_PARITY      0x8 /* Set parity 'O' bits */
#define LCR_NO_PARITY       0    /* Set parity 'N' bits */
#define LCR_1_STOP_BIT      0   /* Bits to set 1 stop bit */
#define LCR_2_STOP_BITS     4   /* Bits to set 2 stop bits */
#define LCR_5_DATA_BITS     0   /* Bits to set 5 data bits */
#define LCR_6_DATA_BITS     1   /* Bits to set 6 data bits */
#define LCR_7_DATA_BITS     2   /* Bits to set 7 data bits */
#define LCR_8_DATA_BITS     3   /* Bits to set 8 data bits */
#define MCR                 4   /* Modem Control Register */
#define MCR_DTR             1   /* Bit to turn on DTR */
#define MCR_RTS             2   /* Bit to turn on RTS */
#define MCR_OUT1            4   /* Bit to turn on OUT1 */
#define MCR_OUT2            8   /* Bit to turn on OUT2 */
#define LSR                 5   /* Line status register */
#define LSR_TEMT            32  /* transmitter holding status empty */
#define MSR                 6   /* Modem status register */
#define DLL                 0   /* Divisor latch LSB */
#define DLM                 1   /* Divisor latch MSB */

#define INT_CONTROLLER      0x20    /* Address of 8259 */
#define EOI                 0x20    /* the end of int command */
#define BREAK_VECTOR        0x23    /* the CTRL-BREAK vector */

#define COM1_UART       0x3f8
#define COM1_INTERRUPT  12
#define COM2_UART       0x2f8
#define COM2_INTERRUPT  11
#define COM3_UART       0x2e8
#define COM3_INTERRUPT  11
#define COM4_UART       0x2d8
#define COM4_INTERRUPT  12

static PORT port;

#pragma off (check_stack)
void __interrupt __far serial_isr()   
{
	unsigned char c;

	_enable();
	while(1)    {
		switch( inp( port.uart_base + IIR ) )   {
		case IIR_MODEM_STATUS :
			inp( port.uart_base + MSR );
			break;
		case IIR_TRANSMIT :
			if ( port.out.read_index == port.out.write_index )
				outp( port.uart_base + IER, IER_RX_DATA );
			else    {
				c = port.out.buffer[ port.out.read_index++ ];
				outp( port.uart_base + THR, c );
			}
			break;
		case IIR_RECEIVE :
			c = (unsigned char) inp( port.uart_base+RBR );
			if ((port.in.write_index+1 ) != port.in.read_index)
				port.in.buffer[ port.in.write_index++ ] = c;
			break;
		case IIR_LINE_STATUS :
			inp( port.uart_base + LSR );
			break;
		default:
			outp( INT_CONTROLLER, EOI );
			return;
		}
	}
}

void serial_isr_end()		// Used to mark the end of interrupt
{
}
#pragma on (check_stack)


void serial_open(int port_number, long speed, char parity, int data, int stopbits )
{
	int address, int_number;
	unsigned char temp;
	unsigned char lcr_out;
	unsigned char mcr_out;
	unsigned char low_divisor;
	unsigned char high_divisor;

	if (!dpmi_lock_region ((void near *)serial_isr, (char *)serial_isr_end - (char near *)serial_isr))	{
		printf( "%s\n", TXT_ERROR_SERIAL_LOCK );
		exit(1);
	}

	if (!dpmi_lock_region (&port, sizeof(PORT) ) )	{
		printf( "%s\n", TXT_ERROR_SERIAL_LOCK_2 );
		exit(1);
	}
	
	switch( port_number )   {
	case 1:
		address = COM1_UART;
		int_number = COM1_INTERRUPT;
		break;
	case 2:
		address = COM2_UART;
		int_number = COM2_INTERRUPT;
		break;
	case 3:
		address = COM3_UART;
		int_number = COM3_INTERRUPT;
		break;
	case 4:
		address = COM4_UART;
		int_number = COM4_INTERRUPT;
		break;
	default:
		address = COM1_UART;
		int_number = COM1_INTERRUPT;
	}

	port.in.write_index = port.in.read_index = 0;
	port.out.write_index = port.out.read_index = 0;
	port.uart_base = address;
	port.irq_mask = (char) 1 << (int_number % 8 );
	port.interrupt_number = int_number;
	port.old_vector = _dos_getvect( int_number );
	_dos_setvect( int_number, serial_isr );
	temp = (char) inp( INT_CONTROLLER + 1 );
	outp( INT_CONTROLLER + 1, ~port.irq_mask & temp );

	/* Disable all interrupts from the port */
	outp( port.uart_base + IER, 0 );

	/* read RBR just in case there is a char sitting there */
	inp( port.uart_base );

	/* write the baud rate */
	low_divisor = (char)(115200L / speed ) & 0xff;
	high_divisor = (char)((115200L / speed ) >> 8 );
	outp( port.uart_base + LCR, LCR_DLAB );
	outp( port.uart_base + DLL, low_divisor );
	outp( port.uart_base + DLM, high_divisor );
	outp( port.uart_base + LCR, 0 );

	/* set up parity, stop bits, word length */
	if ( parity == 'E' )
		lcr_out = LCR_EVEN_PARITY;
	else if ( parity == 'O' )
		lcr_out = LCR_ODD_PARITY;
	else
		lcr_out = LCR_NO_PARITY;

	if ( stopbits == 2 )
		lcr_out |= LCR_2_STOP_BITS;

	if ( data == 6 )
		lcr_out |= LCR_6_DATA_BITS;
	else if ( data == 7 )
		lcr_out |= LCR_7_DATA_BITS;
	else if ( data == 8 )
		lcr_out |= LCR_8_DATA_BITS;

	outp( port.uart_base + LCR, lcr_out );

	/* Turn on RTS, DTR, OUT2 */
	mcr_out = MCR_RTS | MCR_DTR | MCR_OUT2;
	outp( port.uart_base + MCR, mcr_out );

	/* restart reciever interrupts and exit */
	outp( port.uart_base + IER, IER_RX_DATA );
}

void serial_close( )
{
	unsigned char temp;

	outp( port.uart_base + IER, 0 );
	temp = (unsigned char) inp( INT_CONTROLLER + 1 );
	outp( INT_CONTROLLER + 1, port.irq_mask | temp );
	_dos_setvect( port.interrupt_number, port.old_vector );
	outp( port.uart_base + MCR, 0 );
}

//--unused-- int serial_putc( unsigned char c )    {
//--unused-- 	if ((port.out.write_index+1 ) == port.out.read_index )
//--unused-- 		return(-1);
//--unused-- 	port.out.buffer[ port.out.write_index ] = c;
//--unused-- 	port.out.write_index += 1;
//--unused-- 	if ((inp( port.uart_base + IER ) & IER_THRE) == 0 )
//--unused-- 		outp( port.uart_base + IER, IER_THRE | IER_RX_DATA );
//--unused-- 	return( c );
//--unused-- }
//--unused-- 

int serial_getc()  {
	if (port.in.write_index == port.in.read_index )
		return(-1);
	else
		return( port.in.buffer[ port.in.read_index++ ] );
}

void serial_test()
{

}

