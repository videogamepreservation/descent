#
# $Source: f:/miner/source/bios/rcs/makefile $
# $Revision: 1.18 $
# $Author: john $
# $Date: 1995/03/23 18:27:04 $
#
# Makefile for bios functions
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

# What to install, and where to install it
INSTALL_TARGS = io.lib joy.h key.h mono.h mouse.h timer.h swift.h dpmi.h ipx.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = io

# What test programs to make
TEST_PROGS = demo
# testj testk testm testms

# What object files for the lib
IO_OBJS = key.obj joy.obj mouse.obj timer.obj mono.obj joyc.obj dpmi.obj ipx.obj

# What object files and libs for the test programs
DEMO_SRCS = demo.c
DEMO_LIBS = io.lib 

io.lib: $(IO_OBJS)
	wlib io.lib $(IO_OBJS)
	copy *.lib $(LIBDIR)
cdrom.obj: cdrom.c
	wcc386 $(CCFLAGS) cdrom.c

demo.obj: demo.c
	wcc386 $(CCFLAGS) demo.c
dpmi.obj: dpmi.c
	wcc386 $(CCFLAGS) dpmi.c
ipx.obj: ipx.c
	wcc386 $(CCFLAGS) ipx.c
joyc.obj: joyc.c
	wcc386 $(CCFLAGS) joyc.c
joy.obj: joy.asm
	masm joy.asm
key.obj: key.c
	wcc386 $(CCFLAGS) key.c
mono.obj: mono.c
	wcc386 $(CCFLAGS) mono.c
mouse.obj: mouse.c
	wcc386 $(CCFLAGS) mouse.c
testj.obj: testj.c
	wcc386 $(CCFLAGS) testj.c
testk.obj: testk.c
	wcc386 $(CCFLAGS) testk.c
testm.obj: testm.c
	wcc386 $(CCFLAGS) testm.c
testmem.obj: testmem.c
	wcc386 $(CCFLAGS) testmem.c
testms.obj: testms.c
	wcc386 $(CCFLAGS) testms.c
testt.obj: testt.c
	wcc386 $(CCFLAGS) testt.c
timer.obj: timer.asm
	masm timer.asm

