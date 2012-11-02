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
 * $Source: f:/miner/source/main/rcs/ipx.h $
 * $Revision: 1.3 $
 * $Author: john $
 * $Date: 1994/07/25 12:33:22 $
 * 
 * Prototype for IPX communications.
 * 
 * $Log: ipx.h $
 * Revision 1.3  1994/07/25  12:33:22  john
 * Network "pinging" in.
 * 
 * Revision 1.2  1994/07/20  15:58:29  john
 * First installment of ipx stuff.
 * 
 * Revision 1.1  1994/07/19  15:43:05  john
 * Initial revision
 * 
 * 
 */

#ifndef _IPX_H
#define _IPX_H

// The default socket to use.
#define IPX_DEFAULT_SOCKET 0x869d

//---------------------------------------------------------------
// Initializes all IPX internals. 
// If socket_number==0, then opens next available socket.
// Returns:	0  if successful.
//				-1 if socket already open.
//				-2	if socket table full.
//				-3 if IPX not installed.
//				-4 if couldn't allocate low dos memory
//				-5 if error with getting internetwork address
extern int ipx_init( int socket_number );

// Returns a pointer to 6-byte address
extern ubyte * ipx_get_my_local_address();

// If any packets waiting to be read in, this fills data in with the packet data and returns
// the number of bytes read.  Else returns 0 if no packets waiting.
extern int ipx_get_packet_data( ubyte * data );

// Sends a broadcast packet to everyone on this socket.
extern void ipx_send_broadcast_packet_data( ubyte * data, int datasize );

// Sends a packet to a certain address
extern void ipx_send_packet_data( ubyte * data, int datasize, ubyte *address );

#define IPX_MAX_DATA_SIZE (534-4)

#endif

