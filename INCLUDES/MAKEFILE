#
# $Source: f:/miner/source/includes/rcs/makefile $
# $Revision: 1.1 $
# $Author: matt $
# $Date: 1993/11/04 10:24:14 $
#
# Makefile for macro and header files
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

# What to install
INSTALL_TARGS = types.h types.inc psmacros.inc

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = test

# What object files and libs for the test programs
TEST_SRCS = test.c mactest.c
TEST_LIBS = io.lib


mactest.obj: psmacros.inc

test.obj: mono.h
