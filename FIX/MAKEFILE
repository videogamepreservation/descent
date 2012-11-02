#
# $Source: f:/miner/source/fix/rcs/makefile $
# $Revision: 1.5 $
# $Author: matt $
# $Date: 1994/11/27 23:21:18 $
#
# Makefile for fixed-point library
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

# What to install, and where to install it
INSTALL_TARGS = fix.lib fix.h fix.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = fix

# What test programs to make
TEST_PROGS = fixtest

# What object files for the lib
FIX_OBJS = fix.obj

# What object files and libs for the test programs
FIXTEST_SRCS = fixtest.c
FIXTEST_LIBS = fix.lib io.lib

fix.lib: $(FIX_OBJS)
	wlib fix.lib $(FIX_OBJS)
	copy *.lib $(LIBDIR)
asingen.obj: math.h stdio.h

fix.obj: fix.inc fix.asm
	masm fix.asm

fixtest.obj: conio.h fix.h graph.h stdio.h stdlib.h types.h

singen.obj: math.h

test.obj: psmacros.inc
