#
# $Source: f:/miner/source/2d/rcs/makefile $
# $Revision: 1.35 $
# $Author: john $
# $Date: 1995/01/29 17:08:57 $
#
# Makefile for bios functions
#

LIBDIR = ..\main

#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

# What to install, and where to install it
INSTALL_TARGS = effect2d.h gr.h gr.lib grdef.h gr.inc palette.h ibitblt.h rle.h pcx.h

# What to clean
CLEAN_TARGS = $(OBJDIR)\*.obj

# What libs to make
LIBS = gr

# What test programs to make
#TEST_PROGS = testg testp vesatest
PROGS = testg

# What object files for the lib
GR_OBJS = linear.obj vesa.obj modex.obj canvas.obj bitmap.obj gr.obj font.obj \
	bitblt.obj scanline.obj pixel.obj poly.obj disc.obj rect.obj box.obj  \
	gpixel.obj palette.obj line.obj lbitblt.obj        \
	scale.obj scalea.obj circle.obj ibitblt.obj rle.obj pcx.obj
#effects.obj roller.obj 

# What object files and libs for the test programs
TESTG_SRCS = testg.c tmapbits.c
TESTG_LIBS = io.lib gr.lib misc.lib mem.lib iff.lib  cfile.lib

#TESTP_OBJS = testp.obj
#TESTP_LIBS = io.lib gr.lib misc.lib mem.lib cflib.lib 

#VESATEST_OBJS = vesatest.obj
#VESATEST_LIBS = io.lib gr.lib misc.lib mem.lib cflib.lib 

gr.lib: linear.obj vesa.obj modex.obj canvas.obj bitmap.obj gr.obj font.obj \
	bitblt.obj scanline.obj pixel.obj poly.obj disc.obj rect.obj box.obj  \
	gpixel.obj palette.obj line.obj lbitblt.obj        \
	scale.obj scalea.obj circle.obj ibitblt.obj rle.obj pcx.obj

	wlib gr.lib linear.obj vesa.obj modex.obj canvas.obj bitmap.obj gr.obj font.obj \
		bitblt.obj scanline.obj pixel.obj poly.obj disc.obj rect.obj box.obj  \
		gpixel.obj palette.obj line.obj lbitblt.obj        \
		scale.obj scalea.obj circle.obj ibitblt.obj rle.obj pcx.obj
	copy *.lib $(LIBDIR)

bitblt.obj: bitblt.c
	wcc386 $(CCFLAGS) bitblt.c
bitmap.obj: bitmap.c
	wcc386 $(CCFLAGS) bitmap
box.obj: box.c
	wcc386 $(CCFLAGS) box
canvas.obj: canvas.c
	wcc386 $(CCFLAGS) canvas.c
circle.obj: circle.c
	wcc386 $(CCFLAGS) circle.c
disc.obj: disc.c
	wcc386 $(CCFLAGS) disc.c
effects.obj: effects.c
	wcc386 $(CCFLAGS) effects.c
fdraw.obj: fdraw.c
	wcc386 $(CCFLAGS) fdraw.c
font.obj: font.c
	wcc386 $(CCFLAGS) font.c
gpixel.obj: gpixel.c
	wcc386 $(CCFLAGS) gpixel.c
gr.obj:  gr.c
	wcc386 $(CCFLAGS) gr.c
ibitblt.obj: ibitblt.c
	wcc386 $(CCFLAGS) ibitblt.c
inv_cmap.obj: inv_cmap.c
	wcc386 $(CCFLAGS) inv_cmap.c
l.obj:   l.obj
	wcc386 $(CCFLAGS) l.c
lbitblt.obj: lbitblt.asm
	masm lbitblt.asm
line.obj: line.c
	wcc386 $(CCFLAGS) line.c
linear.obj: linear.asm
	masm linear.asm linear.obj
median.obj: median.c
	wcc386 $(CCFLAGS) median.c
mspeed.obj: mspeed.c
	wcc386 $(CCFLAGS) mspeed.c
modex.obj: modex.asm
	masm modex.asm
palette.obj: palette.c
	wcc386 $(CCFLAGS) palette.c
pcx.obj: pcx.c
	wcc386 $(CCFLAGS) pcx.c
pixel.obj: pixel.c
	wcc386 $(CCFLAGS) pixel.c
poly.obj: poly.c
	wcc386 $(CCFLAGS) poly.c
rect.obj: rect.c
	wcc386 $(CCFLAGS) rect.c
rle.obj: rle.c
	wcc386 $(CCFLAGS) rle.c
roller.obj: roller.c
	wcc386 $(CCFLAGS) roller.c
scale.obj: scale.c
	wcc386 $(CCFLAGS) scale.c
scalea.obj: scalea.asm
	masm scalea.asm scalea.obj 
scanline.obj: scanline.c
	wcc386 $(CCFLAGS) scanline.c
testg.obj: testg.c
	wcc386 $(CCFLAGS) testg.c
vesa.obj: vesa.asm
	masm vesa.asm
vesalib.obj: vesalib.c
	wcc386 $(CCFLAGS) vesalib.c
vesatest.obj: vesatest.c
	wcc386 $(CCFLAGS) vesatest.c
