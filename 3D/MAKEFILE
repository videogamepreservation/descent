#
# $Source: f:/miner/source/3d/rcs/makefile $
# $Revision: 1.9 $
# $Author: matt $
# $Date: 1994/03/15 21:22:47 $
#
# Makefile for 3d library
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG


# What to install
INSTALL_TARGS = 3d.lib 3d.h 3d.inc

# What libs to build
LIBS = 3d

# What objs in the lib
3D_OBJS = globvars.obj setup.obj matrix.obj points.obj draw.obj clipper.obj \
	horizon.obj instance.obj rod.obj interp.obj

# What test programs to make
TEST_PROGS = 3dtest

# What object files and libs for the test programs
3DTEST_SRCS = 3dtest.c segment.c trakball.c
3DTEST_LIBS = 3d.lib vecmat.lib gr.lib io.lib fix.lib div0.lib texmap.lib misc.lib bm.lib

#rules and targets common to all Miner makefiles

3d.lib: $(3D_OBJS)
	wlib 3d.lib $(3D_OBJS)
	copy *.lib $(LIBDIR)
3dtest.obj: 3dtest.c
	wcc386 $(CCFLAGS) 3dtest.c

clipper.obj: clipper.asm
	masm clipper.asm clipper.obj
draw.obj: draw.asm
	masm draw.asm draw.obj
globvars.obj: globvars.asm
	masm globvars.asm globvars.obj
horizon.obj: horizon.asm
	masm horizon.asm horizon.obj
instance.obj: instance.asm 
	masm instance.asm instance.obj

interp.obj: interp.asm
	masm interp.asm interp.obj
matrix.obj: matrix.asm
	masm matrix.asm matrix.obj

objects.obj: objects.asm
	masm objects.asm objects.obj

out.obj: out.asm
	masm out.asm out.obj

points.obj: points.asm
	masm points.asm points.obj

rod.obj: rod.asm
	masm rod.asm rod.obj

segment.obj: segment.asm
	masm segment.asm segment.obj

setup.obj: setup.asm
	masm setup.asm setup.obj

#trakball.obj: trackball.asm

upvecs.obj: upvecs.asm
	masm upvecs.asm upvecs.obj

upvecs2.obj: fix.h types.h vecmat.h

