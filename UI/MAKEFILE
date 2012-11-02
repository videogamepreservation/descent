#
# $Source: f:/miner/source/ui/rcs/makefile $
# $Revision: 1.8 $
# $Author: matt $
# $Date: 1994/11/27 14:53:50 $
#
# Makefile for user-interface functions
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG


# What to install, and where to install it
INSTALL_TARGS = ui.lib ui.h func.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = ui

# What programs to make
TEST_PROGS = testg

# What object files for the lib
UI_OBJS = ui.obj mouse.obj window.obj uidraw.obj button.obj \
		  gadget.obj listbox.obj message.obj popup.obj scroll.obj \
		  radio.obj checkbox.obj inputbox.obj userbox.obj menu.obj \
		  harderr.obj file.obj keytrap.obj icon.obj keypress.obj \
		  menubar.obj keypad.obj 
#func.obj #Couldn't find func.c - where are the functions??

# What object files and libs for the test programs
TESTG_SRCS = testg.c
TESTG_LIBS = io.lib gr.lib ui.lib misc.lib mem.lib readfile.lib iff.lib func.lib cflib.lib cfile.lib
TESTG_LFLAGS = option stack=50k

ui.lib: $(UI_OBJS)
	wlib ui.lib $(UI_OBJS)
	copy *.lib $(LIBDIR)
barbox.obj: barbox.c
	wcc386 $(CCFLAGS) barbox.c
button.obj: button.c
	wcc386 $(CCFLAGS) button.c
checkbox.obj: checkbox.c
	wcc386 $(CCFLAGS) checkbox.c
file.obj: file.c
	wcc386 $(CCFLAGS) file.c
func.obj: func.c
	wcc386 $(CCFLAGS) func.c
gadget.obj: gadget.c
	wcc386 $(CCFLAGS) gadget.c
harderr.obj: harderr.c
	wcc386 $(CCFLAGS) harderr.c
icon.obj: icon.c
	wcc386 $(CCFLAGS) icon.c
inputbox.obj: inputbox.c
	wcc386 $(CCFLAGS) inputbox.c
keypad.obj: keypad.c
	wcc386 $(CCFLAGS) keypad.c
keypress.obj: keypress.c
	wcc386 $(CCFLAGS) keypress.c
keytrap.obj: keytrap.c
	wcc386 $(CCFLAGS) keytrap.c
listbox.obj: listbox.c
	wcc386 $(CCFLAGS) listbox.c
menu.obj: menu.c
	wcc386 $(CCFLAGS) menu.c
menubar.obj: menubar.c
	wcc386 $(CCFLAGS) menubar.c
message.obj: message.c
	wcc386 $(CCFLAGS) message.c
mouse.obj: mouse.c
	wcc386 $(CCFLAGS) mouse.c
number.obj: number.c
	wcc386 $(CCFLAGS) number.c
popup.obj: popup.c
	wcc386 $(CCFLAGS) popup.c
radio.obj: radio.c
	wcc386 $(CCFLAGS) radio.c
scroll.obj: scroll.c
	wcc386 $(CCFLAGS) scroll.c
ui.obj:	 ui.c
	wcc386 $(CCFLAGS) ui.c
uidraw.obj: uidraw.c
	wcc386 $(CCFLAGS) uidraw.c
userbox.obj: userbox.c
	wcc386 $(CCFLAGS) userbox.c
window.obj: window.c
	wcc386 $(CCFLAGS) window.c
