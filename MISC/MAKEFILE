#
# $Source: f:/miner/source/misc/rcs/makefile $
# $Revision: 1.4 $
# $Author: matt $
# $Date: 1994/11/27 22:27:23 $
#
# Makefile for misc libaray
#

LIBDIR = ..\main

#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

# What to install
INSTALL_TARGS = misc.lib parsarg.h error.h

# What libs to build
LIBS = misc

# What objs in the lib
MISC_OBJS = parsarg.obj error.obj

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = errtest parstest

# What object files and libs for the test programs
PARSTEST_SRCS = parstest.c
PARSTEST_LIBS = misc.lib

ERRTEST_SRCS = errtest.c
ERRTEST_LIBS = misc.lib

misc.lib: $(MISC_OBJS)
	wlib misc.lib $(MISC_OBJS)
	copy *.lib $(LIBDIR)

error.obj: error.c error.h 
	wcc386 $(CCFLAGS) error.c

errtest.obj: errtest.c error.h 
	wcc386 $(CCFLAGS) errtest.c

parsarg.obj: parsarg.c parsarg.h

	wcc386 $(CCFLAGS) parsarg.c

parstest.obj: parstest.c parsarg.h 

	wcc386 $(CCFLAGS) parstest.c

