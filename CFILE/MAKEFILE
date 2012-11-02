#
# Master makefile for Miner source
#
# Makes all the executable by visiting subdirs and making there
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

CLEAN_TARGS = $(OBJDIR)\*.obj

INSTALL_TARGS = cfile.lib cfile.h nocfile.h

LIBS = cfile

CFILE_OBJS = cfile.obj

cfile.lib: $(CFILE_OBJS)
	wlib cfile.lib $(CFILE_OBJS)
	copy *.lib $(LIBDIR)
cfile.obj: cfile.h cfile.c
	wcc386 $(CCFLAGS) cfile.c

test.obj: test.c
	wcc386 $(CCFLAGS) test.c

