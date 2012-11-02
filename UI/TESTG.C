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
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <string.h>
#include <direct.h>

#include "types.h"
#include "gr.h"
#include "key.h"
#include "mono.h"
#include "ui.h"
#include "mem.h"
#include "cflib.h"

char TempFilename[100];

extern int ui_button_any_drawn;

UI_WINDOW * MainWindow;
UI_GADGET_BUTTON * ColorButton[256];
UI_GADGET_BUTTON * FadeButton[16];
UI_GADGET_BUTTON * OrgFadeButton[16];
UI_GADGET_BUTTON * QuitButton;
UI_GADGET_BUTTON * SaveButton;
UI_GADGET_BUTTON * CurrentColorButton;
UI_GADGET_BUTTON * RestoreButton;
UI_GADGET_BUTTON * RestoreAllButton;
UI_GADGET_BUTTON * HelpButton;

ubyte SavedFadeTable[256*16];

int Changed=0;

int CurrentFader = 0;
int Mode = 0;
int CurrentColor = 0;

char MainHelpText[] =							\
"\nHELP\n"											\
"\nClick on any color to make it the\n"	\
"current color.  Shift+click on the\n"		\
"fade bar assigns the current color\n"		\
"to the fade level you clicked on.\n"		\
"Shift+click on one of the 256 colors\n"	\
"at the bottom selects that color to\n"	\
"be the one on the fade bar at top."		\
"";

void init_color_buttons()
{
	int i,x,y,w,h;

	w = 320/16; h = 100/16;

	x = 0;
	y = 100;

	for (i=0; i<256; i++ )	{
		ColorButton[i]=ui_add_gadget_button( MainWindow, x, y, w, h, NULL, NULL );
		ColorButton[i]->canvas->cv_color = i;
		ColorButton[i]->status = 1;
		y+=h;
		if ( y > (199-h) )	{
			y = 100;
			x += w;
		}
	}

	x = 0; y = 0;
	for (i=0; i<16; i++ )	{
		OrgFadeButton[i]=ui_add_gadget_button( MainWindow, x, y, w, h, NULL, NULL );
		OrgFadeButton[i]->canvas->cv_color = i;
		OrgFadeButton[i]->status = 1;
		FadeButton[i]=ui_add_gadget_button( MainWindow, x+w, y, w, h, NULL, NULL );
		FadeButton[i]->canvas->cv_color = i;
		FadeButton[i]->status = 1;
		y += h;
		if ( y > (199-h) )	{
			y = 100;
			x += w;
		}
	}
}

void set_fade_buttons(int color)
{
	int i;

	if (color < 0 ) color += 256;
	if (color > 255 ) color -= 256;

	CurrentFader = color;

	for (i=0; i<16; i++ )	{
		FadeButton[i]->canvas->cv_color = gr_fade_table[i*256+color];
		FadeButton[i]->status = 1;

		OrgFadeButton[i]->canvas->cv_color = SavedFadeTable[i*256+color];
		OrgFadeButton[i]->status = 1;

	}	
}

void set_fade_color_to_current(int fade_level)	{
	gr_fade_table[fade_level*256+CurrentFader ] = CurrentColor ;
	Changed = 1;
	FadeButton[fade_level]->canvas->cv_color = CurrentColor ;
	FadeButton[fade_level]->status = 1;
}

void outline_current_fader()
{
	int color;
	UI_GADGET_BUTTON * button;

	ui_mouse_hide();
	button = ColorButton[CurrentFader];
	color = button->canvas->cv_color;
	gr_set_current_canvas( button->canvas );
	gr_setcolor( CBRIGHT );
	gr_box( 0, 0, button->width, button->height );
	button->canvas->cv_color = color;
	ui_mouse_show();
}

void outline_current_color()
{
	int color;
	UI_GADGET_BUTTON * button;

	ui_mouse_hide();
	button = ColorButton[CurrentColor];
	color = button->canvas->cv_color;
	gr_set_current_canvas( button->canvas );
	gr_setcolor( CRED );
	gr_box( 0, 0, button->width, button->height );
	button->canvas->cv_color = color;
	ui_mouse_show();
}


void set_current_color( int color )	
{
	CurrentColor = color;
	CurrentColorButton->canvas->cv_color = CurrentColor;
	CurrentColorButton->status = 1;
}

void RestoreAll()
{
	int i, j;

	for (i=0; i<256; i++ )	{
		for (j=0; j<16; j++ )	{
			gr_fade_table[256*j+i]=SavedFadeTable[256*j+i];
			if (i==CurrentFader)	{
				FadeButton[j]->canvas->cv_color = gr_fade_table[256*j+i];
				FadeButton[j]->status = 1;
			}
		}
	}

}

void RestoreCurrent()
{
	int j;

	for (j=0; j<16; j++ )	{
		gr_fade_table[256*j+CurrentFader]=SavedFadeTable[256*j+CurrentFader];
		FadeButton[j]->canvas->cv_color = gr_fade_table[256*j+CurrentFader];
		FadeButton[j]->status = 1;
	}

}

void SaveTables( )
{
	WriteFile( "PALETTE.256", gr_palette, 256*3 );
	AppendFile( "PALETTE.256", gr_inverse_table, 32*32*32 );
	AppendFile( "PALETTE.256", gr_blend_table, 256*256 );
	AppendFile( "PALETTE.256", gr_fade_table, 256*16 );
	MessageBox( -2, -2, 1, "Palette saved successfully.", "Ok" );
	Changed = 0;
}

main()
{	int i,j, last_fader, last_color;
	grs_font * my_font;
	
	minit();
	gr_init( SM_320x200C );
	gr_use_palette_table( "PALETTE.256");
	for (i=0; i<256; i++ )	{
		gr_fade_table[15*256+i] = i;	// Make all fades start at the right color
		for (j=0; j<16; j++ )
			SavedFadeTable[256*j+i] = gr_fade_table[256*j+i];
	}

	my_font = gr_init_font( "xm4x5.fnt" );
	ui_init();

	MainWindow = ui_open_window( 0 , 0, 320, 200, 0 );
	QuitButton = ui_add_gadget_button( MainWindow, 270, 0, 50, 10, "Exit", NULL );
	SaveButton = ui_add_gadget_button( MainWindow, 270, 10, 50, 10, "Save", NULL );
	RestoreAllButton = ui_add_gadget_button( MainWindow, 270, 20, 50, 10, "RestoreAll", NULL );
	HelpButton = ui_add_gadget_button( MainWindow, 270, 30, 50, 10, "Help", NULL ) ;
	CurrentColorButton = ui_add_gadget_button( MainWindow, 40, 0, 320/16, (100/16)*16, NULL, NULL );
	RestoreButton = ui_add_gadget_button( MainWindow, 70, 80, 40, 10, "Restore", NULL );
	init_color_buttons();
	set_fade_buttons(0);
	
	last_fader = last_color = -1;

	while ( 1 )
	{
		ui_button_any_drawn = 0;
		ui_mega_process();
		ui_window_do_gadgets(MainWindow);

		switch(last_keypress)	{
		case KEY_UP:
			set_fade_buttons(CurrentFader-1);
			break;
		case KEY_DOWN:
			set_fade_buttons(CurrentFader+1);
			break;
		case KEY_LEFT:
			set_fade_buttons(CurrentFader-16);
			break;
		case KEY_RIGHT:
			set_fade_buttons(CurrentFader+16);
			break;
		case KEY_HOME:
			set_fade_buttons(0);
			break;
		case KEY_END:
			set_fade_buttons(255);
			break;
		}

		if (keyd_pressed[KEY_LSHIFT])	{
			for (i=0; i<256; i++ )	{
				if (ColorButton[i]->position>0 )	{
				 	set_fade_buttons(i);
					Mode = 0;
				}
			}
			for (i=0; i<15; i++ )	{
				if (FadeButton[i]->position>0 )	{
					set_fade_color_to_current(i);
				}
			}
		} else {
			for (i=0; i<256; i++ )	{
				if (ColorButton[i]->position>0)	{
					set_current_color(i);
				}
			}
			for (i=0; i<16; i++ )	{
				if (FadeButton[i]->position>0 )	{
					set_current_color( FadeButton[i]->canvas->cv_color );
				}
			}
		}
	
		if (last_fader != CurrentFader) {
			if (last_fader >= 0 )	{
				ColorButton[last_fader]->status = 1;
			}
			outline_current_fader();
			last_fader = CurrentFader;
		}	

		if (last_color != CurrentColor) {
			if (last_color >= 0 )	{
				ColorButton[last_color]->status = 1;
			}
			outline_current_color();
			last_color = CurrentColor;
		}	

		if (ui_button_any_drawn)	{
			outline_current_fader();
			outline_current_color();
		}	

		if (RestoreButton->pressed )
			RestoreCurrent();

		if (RestoreAllButton->pressed )
			RestoreAll();

		if (SaveButton->pressed )
			SaveTables();

		if (QuitButton->pressed ) {
			if ( Changed == 1 )	{
				int choice;
				choice = MessageBox(-2,-2, 3, "Save palette?", "Yes", "No", "Cancel" );
				if (choice==2) break;
				if (choice==1)	{
					SaveTables();
					break;
				}								
			} else {
				break;
			}
		}

		if (HelpButton->pressed)	{
       	MessageBox( -2, -2, 1, MainHelpText, "Ok" );
		}
	}

	ui_close_window( MainWindow );

	return 0;

}


