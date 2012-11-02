#
# Master makefile for Miner source
#
# Makes all the executable by visiting subdirs and making there
#

CLEAN_TARGS = $(OBJDIR)\*.obj

PROGS = xcolor

XCOLOR_SRCS = xcolor.c 

XCOLOR_LIBS = gr.lib misc.lib mem.lib iff.lib readfile.lib ui.lib io.lib func.lib cflib.lib

XCOLOR_LFLAGS = option stack=150k

xcolor.exe: xcolor.obj
	wl386 xcolor.c 
xcolor.obj: xcolor.c
	wcc386 xcolor.c
