#
# $Source: f:/miner/source/bios/rcs/makefile $
# $Revision: 1.3 $
# $Author: matt $
# $Date: 1993/07/21 20:12:19 $
#
# Makefile for bios functions
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG


# What to install, and where to install it
INSTALL_TARGS =

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS =

# What test programs to make
TEST_PROGS = testp

# What object files and libs for the test programs
TESTP_OBJS = testp.obj iff.obj iff15bpp.obj
TESTP_LIBS = io.lib gr.lib ui.lib misc.lib mem.lib bm.lib iff.lib readfile.lib
iff.exe: iff.obj
	wcl386 /l=dos4g iff.c
iff.obj: iff.c
	wcc386 /l=dos4g $(CCFLAGS) iff.c
iff15bpp.obj: 

testp.obj: 

tga.obj: 
