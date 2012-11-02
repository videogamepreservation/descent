#
# $Source: f:/miner/source/texmap/rcs/makefile $
# $Revision: 1.23 $
# $Author: john $
# $Date: 1995/02/20 18:23:00 $
#
# Makefile for hack3df testbed
#

LIBDIR = ..\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG


#Uncomment the following 3 lines if you want to build with only C code.
#NASM = 1
#GLOBAL_CFLAGS+=/dNASM
#GLOBAL_AFLAGS+=/DNASM

# What to install
INSTALL_TARGS = texmap.lib texmap.h

# What libs to build
LIBS = texmap

# What objs in the TEXMAP lib
#TEXMAP_OBJS = ntmap.obj tmapsky.obj tmapv.obj tmapflat.obj tmap_per.obj tmap_p2.obj tmap_lin.obj tmap_ll.obj tmap_kl.obj\
#		tmap_ld.obj tmap_flt.obj tmap_sky.obj tmap_skv.obj tmap_fl1.obj tmapz.obj tmap_z.obj tmapfade.obj tmap_mul.obj

TEXMAP_OBJS = 	ntmap.obj tmapflat.obj  

#%if !%defined(NASM)
TEXMAP_OBJS +=	tmap_per.obj tmap_lin.obj tmap_ll.obj tmap_flt.obj tmapfade.obj tmap_fl1.obj
#%else
TEXMAP_OBJS += scanline.obj
#%endif

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What test programs to make
TEST_PROGS = textest

# What object files and libs for the test programs
TEXTEST_SRCS = TEXTEST.obj
TEXTEST_LIBS = TEXMAP.lib

texmap.lib: $(TEXMAP_OBJS)
	wlib texmap.lib $(TEXMAP_OBJS)
	copy *.lib $(LIBDIR)
lightabs.obj: lightabs.asm
	masm lightabs.asm

ntmap.obj: ntmap.c
	wcc386 $(CCFLAGS) ntmap.c
scanline.obj: scanline.c
	wcc386 $(CCFLAGS) scanline.c
tmap_16.obj: tmap_16.asm
	masm tmap_16.asm
tmap_16g.obj: tmap_16g.asm
	masm tmap_16g.asm
tmap_16l.obj: tmap_16l.asm
	masm tmap_16l.asm

tmap_16p.obj: tmap_16p.asm
	masm tmap_16p.asm
	
tmap_16r.obj: tmap_16r.asm
	masm tmap_16r.asm

tmap_aa.obj: tmap_aa.asm
	masm tmap_aa.asm

tmap_fl1.obj: tmap_fl1.asm
	masm tmap_fl1.asm

tmap_flt.obj: tmap_flt.asm
	masm tmap_flt.asm

tmap_kl.obj: tmap_kl.asm
	masm tmap_kl.asm
tmap_ld.obj: tmap_ld.asm
	masm tmap_ld.asm

tmap_lin.obj: tmap_lin.asm
	masm tmap_lin.asm

tmap_ll.obj: tmap_ll.asm
	masm tmap_ll.asm

tmap_mul.obj: tmap_mul.asm
	masm tmap_mul.asm

tmap_p2.obj: tmap_p2.asm
	masm tmap_p2.asm

tmap_per.obj: tmap_per.asm
	masm tmap_per.asm

tmap_rgb.obj: tmap_rgb.asm
	masm tmap_rgb.asm

tmap_skv.obj: tmap_skv.asm
	masm tmap_skv.asm

tmap_sky.obj: tmap_sky.asm
	masm tmap_sky.asm
tmap_z.obj: tmap_z.asm
	masm tmap_z.asm

tmapdith.obj: tmapdith.asm
	masm tmapdith.asm

tmapfade.obj: tmapfade.asm
	masm tmapfade.asm

tmapflat.obj: tmapflat.c
	wcc386 $(CCFLAGS) tmapflat.c

tmapsky.obj: tmapsky.asm
	masm tmapsky.asm

tmapv.obj: tmapv.asm
	masm tmapv.asm
tmapz.obj: tmapz.asm
	masm tmapz.asm

