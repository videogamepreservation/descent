#
# Master makefile for Miner source
#
# Makes all the executable by visiting subdirs and making there
#

# The only thing to make is the subdirs
SUBSYSTEMS = misc includes fix cfile 2d bios iff div mem vecmat 3d texmap
#sosdigi sosmidi

# What to make in the subdirs if nothing specified
SUBTARGETS = clean optimize no_mono no_debug linstall
#SUBTARGETS = update linstall

# When making clean here, delete libs
CLEAN_TARGS = 
#lib\*.lib lib\*.h lib\*.inc

#all Miner makefiles should include this
%include $(INIT)\makefile.def
