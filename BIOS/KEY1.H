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
 * $Source: f:/miner/source/bios/rcs/key.h $
 * $Revision: 1.1 $
 * $Author: matt $
 * $Date: 1993/07/10 13:10:39 $
 *
 * Header for keyboard functions
 *
 * $Log: key.h $
 * Revision 1.1  1993/07/10  13:10:39  matt
 * Initial revision
 * 
 *
 */


//==========================================================================
// This installs the int9 vector and initializes the keyboard in buffered
// ASCII mode. key_close simply undoes that.
extern void key_init();
extern void key_close();

//==========================================================================
// These are configuration parameters to setup how the buffer works.
// set keyd_buffer_type to 0 for no key buffering.
// set it to 1 and it will buffer scancodes.
extern char keyd_buffer_type;
extern char keyd_repeat;        // 1=allow repeating, 0=dont allow repeat


//==========================================================================
// These are the "buffered" keypress routines.  Use them by setting the
// "keyd_buffer_type" variable.

extern void key_flush();    // Clears the 256 char buffer
extern int key_checkch();   // Returns 1 if a char is waiting
extern int key_getch();     // Gets key if one waiting other waits for one.
extern int key_inkey();     // Gets key if one, other returns 0.

extern char key_to_ascii(int keycode );

extern void key_debug();    // Does an INT3

//==========================================================================
// These are the unbuffered routines. Index by the keyboard scancode.

// Set to 1 if the key is currently down, else 0
extern char keyd_pressed[128];

// Returns the number of milliseconds this key has been down since last call.
extern unsigned int key_down_time(int scancode);

// Returns number of times key has went from up to down since last call.
extern unsigned int key_down_count(int scancode);

// Returns number of times key has went from down to up since last call.
extern unsigned int key_up_count(int scancode);

#define KEY_SHIFTED  0x100
#define KEY_ALTED    0x200
#define KEY_CTRLED   0x400

#define KEY_ESC      0x01
#define KEY_1        0x02
#define KEY_2        0x03
#define KEY_3        0x04
#define KEY_4        0x05
#define KEY_5        0x06
#define KEY_6        0x07
#define KEY_7        0x08
#define KEY_8        0x09
#define KEY_9        0x0A
#define KEY_0        0x0B
#define KEY_MINUS    0x0C
#define KEY_EQUAL    0x0D
#define KEY_BACKSP   0x0E
#define KEY_TAB      0x0F
#define KEY_Q        0x10
#define KEY_W        0x11
#define KEY_E        0x12
#define KEY_R        0x13
#define KEY_T        0x14
#define KEY_Y        0x15
#define KEY_U        0x16
#define KEY_I        0x17
#define KEY_O        0x18
#define KEY_P        0x19
#define KEY_LBRACKET 0x1A
#define KEY_RBRACKET 0x1B
#define KEY_ENTER    0x1C
#define KEY_CTRL     0x1D
#define KEY_A        0x1E
#define KEY_S        0x1F
#define KEY_D        0x20
#define KEY_F        0x21
#define KEY_G        0x22
#define KEY_H        0x23
#define KEY_J        0x24
#define KEY_K        0x25
#define KEY_L        0x26
#define KEY_SEMICOL  0x27
#define KEY_RAPOSTRO 0x28
#define KEY_LAPOSTRO 0x29
#define KEY_LSHIFT   0x2A
#define KEY_SLASH    0x2B
#define KEY_Z        0x2C
#define KEY_X        0x2D
#define KEY_C        0x2E
#define KEY_V        0x2F
#define KEY_B        0x30
#define KEY_N        0x31
#define KEY_M        0x32
#define KEY_COMMA    0x33
#define KEY_PERIOD   0x34
#define KEY_DIVIDE   0x35
#define KEY_RSHIFT   0x36
#define KEY_ASTERIK  0x37
#define KEY_ALT      0x38
#define KEY_SPACEBAR 0x39
#define KEY_CAPSLOCK 0x3A
#define KEY_F1       0x3B
#define KEY_F2       0x3C
#define KEY_F3       0x3D
#define KEY_F4       0x3E
#define KEY_F5       0x3F
#define KEY_F6       0x40
#define KEY_F7       0x41
#define KEY_F8       0x42
#define KEY_F9       0x43
#define KEY_F10      0x44
#define KEY_NUMLOCK  0x45
#define KEY_SCROLLCK 0x46
#define KEY_HOME     0x47
#define KEY_UP       0x48
#define KEY_PGUP     0x49
#define KEY_PADMINUS 0x4A
#define KEY_LEFT     0x4B
#define KEY_PAD5     0x4C
#define KEY_RIGHT    0x4D
#define KEY_PADPLUS  0x4E
#define KEY_END      0x4F
#define KEY_DOWN     0x50
#define KEY_PGDN     0x51
#define KEY_INS      0x52
#define KEY_DEL      0x53
#define KEY_F12      0x58
#define KEY_F11      0x57


