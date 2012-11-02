#
# $Source: f:/miner/source/vecmat/rcs/makefile $
# $Revision: 1.2 $
# $Author: matt $
# $Date: 1993/09/24 21:17:56 $
#
# Makefile for vector/matrix library
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

# What to install
INSTALL_TARGS = vecmat.lib vecmat.h vecmat.inc

# What libs to build
LIBS = vecmat

# What objs in the lib
VECMAT_OBJS = vecmat.obj

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = vectest 

# What object files and libs for the test programs
VECTEST_SRCS = vectest.c
VECTEST_LIBS = vecmat.lib io.lib fix.lib
vecmat.lib: $(VECMAT_OBJS)
	wlib vecmat.lib $(VECMAT_OBJS)
	copy *.lib $(LIBDIR)
vecmat.obj: vecmat.inc vecmat.asm
	masm vecmat.asm;

vectest.obj: fix.h stdio.h types.h vecmat.h
