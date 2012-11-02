#
# Makefile for DESCENT execuable
#

LIBDIR = c:\dev\d1src\main
#

CCFLAGS = /dNETWORK /dRELEASE /dNDEBUG

#


DESCENT_LIBS =	3d.lib gr.lib fix.lib io.lib iff.lib vecmat.lib \
        	 	cfile.lib \
        	 	mem.lib \
        	 	ui.lib \
				div0.lib misc.lib texmap.lib editor.lib

#		sosdigi.lib #sosmidi.lib \
		#iglasses.lib gclfr32.lib loadpats.lib
		#lzw.lib sosdwxcr.lib sosmwxcr.lib 

DESCENT_OBJS = inferno.obj game.obj gauges.obj mglobal.obj \
			 scores.obj object.obj laser.obj physics.obj bm.obj \
 			 menu.obj gamesave.obj switch.obj effects.obj texmerge.obj polyobj.obj \
			 gamemine.obj fireball.obj ai.obj aipath.obj hostage.obj \
			 powerup.obj fuelcen.obj digi.obj render.obj hash.obj piggy.obj args.obj \
			 gameseg.obj wall.obj collide.obj lighting.obj \
			 robot.obj morph.obj vclip.obj weapon.obj fvi.obj newdemo.obj titles.obj \
			 vfx.obj gameseq.obj controls.obj automap.obj text.obj \
			 network.obj newmenu.obj netmisc.obj victor.obj\
			 gamefont.obj joydefs.obj hud.obj playsave.obj \
			 endlevel.obj terrain.obj kconfig.obj modem.obj  \
			 multi.obj cntrlcen.obj credits.obj config.obj soscodec.obj kmatrix.obj	\
			 paging.obj mission.obj iglasses.obj songs.obj bmread.obj state.obj multibot.obj nocomlib.obj slew.obj dumpmine.obj 
#demo.obj garage.obj radar.obj coindev.obj serial.obj vfxread.obj victor.obj


# Files to link for Descent

DESCENTR.EXE: 	$(DESCENT_OBJS)
	wcl386 /fe=descentr.exe /l=dos4g /k50000 $(DESCENT_OBJS) $(DESCENT_LIBS)

ai.obj:	 ai.c ai.h
	wcc386 $(CCFLAGS) ai.c
aipath.obj: aipath.c
	wcc386 $(CCFLAGS) aipath.c
arcade.obj: arcade.c 
	wcc386 $(CCFLAGS) arcade.c
args.obj: args.c args.h
	wcc386 $(CCFLAGS) args.c
automap.obj: automap.c 
	wcc386 $(CCFLAGS) automap.c
	
autosave.obj: autosave.c 
	wcc386 $(CCFLAGS) autosave.c
	
bm.obj:	 bm.c 
	wcc386 $(CCFLAGS) bm.c
bmread.obj: bmread.c 
	wcc386 $(CCFLAGS) bmread.c
	
centers.obj: centers.c 
	wcc386 $(CCFLAGS) centers.c
	
cntrlcen.obj: cntrlcen.c 
	wcc386 $(CCFLAGS) cntrlcen.c
cockpit.obj: cockpit.asm
	masm /i$INCLUDE cockpit.asm cockpit.obj
#cockpit.inc psmacros.inc types.inc

coindev.obj: coindev.c coindev.h 
	wcc386 $(CCFLAGS) coindev.c
collide.obj: collide.c collide.h
	wcc386 $(CCFLAGS) collide.c
config.obj: config.c 
	wcc386 $(CCFLAGS) config.c
controls.obj: controls.c 
	wcc386 $(CCFLAGS) controls.c
credits.obj: credits.c 
	wcc386 $(CCFLAGS) credits.c
curves.obj: curves.c 
	wcc386 $(CCFLAGS) curves.c
demo.obj: demo.c 
	wcc386 $(CCFLAGS) demo.c
digi.obj: digi.c 
	wcc386 $(CCFLAGS) digi.c
dumpmine.obj: dumpmine.c 
	wcc386 $(CCFLAGS) dumpmine.c
effects.obj: effects.c 
	wcc386 $(CCFLAGS) effects.c
eglobal.obj: eglobal.c 
	wcc386 $(CCFLAGS) eglobal.c
ehostage.obj: ehostage.c 
	wcc386 $(CCFLAGS) ehostage.c
einit.obj: einit.c 
	wcc386 $(CCFLAGS) einit.c
elight.obj: elight.c 
	wcc386 $(CCFLAGS) elight.c
endlevel.obj: endlevel.c 
	wcc386 $(CCFLAGS) endlevel.c
eobject.obj: eobject.c 
	wcc386 $(CCFLAGS) eobject.c
eswitch.obj: eswitch.c 
	wcc386 $(CCFLAGS) eswitch.c
fireball.obj: fireball.c 
	wcc386 $(CCFLAGS) fireball.c
fixseg.obj: fixseg.c 
	wcc386 $(CCFLAGS) fixseg.c
flyby.obj: flyby.c 
	wcc386 $(CCFLAGS) flyby.c
fuelcen.obj: fuelcen.c 
	wcc386 $(CCFLAGS) fuelcen.c
func.obj: func.c 
	wcc386 $(CCFLAGS) func.c
fvi.obj: fvi.c 
	wcc386 $(CCFLAGS) fvi.c
game.obj: game.c 
	wcc386 $(CCFLAGS) game.c
gamefont.obj: gamefont.c
	wcc386 $(CCFLAGS) gamefont.c
gamemine.obj: gamemine.c 
	wcc386 $(CCFLAGS) gamemine.c
gamesave.obj: gamesave.c 
	wcc386 $(CCFLAGS) gamesave.c
gameseg.obj: gameseg.c 
	wcc386 $(CCFLAGS) gameseg.c
gameseq.obj: gameseq.c
	wcc386 $(CCFLAGS) gameseq.c
garage.obj: garage.c 
	wcc386 $(CCFLAGS) garage.c
gauges.obj: gauges.c
	wcc386 $(CCFLAGS) gauges.c
group.obj: group.c
	wcc386 $(CCFLAGS) group.c
hash.obj: hash.c
	wcc386 $(CCFLAGS) hash.c
hostage.obj: hostage.c
	wcc386 $(CCFLAGS) hostage.c
hud.obj: hud.c
	wcc386 $(CCFLAGS) hud.c
iglasses.obj: iglasses.c
	wcc386 $(CCFLAGS) iglasses.c
inferno.obj: inferno.c
	wcc386 $(CCFLAGS) inferno.c
info.obj: info.c
	wcc386 $(CCFLAGS) info.c
int3hand.obj: psmacros.inc types.inc
	masm int3hand.asm int3hand.obj;
ipx_esr.obj: psmacros.inc types.inc
	masm ipx_esr.asm ipx_esr.obj;
joydefs.obj: joydefs.c
	wcc386 $(CCFLAGS) joydefs.c
kbuild.obj: 
	wcc386 $(CCFLAGS) kbuild.c
kconfig.obj: 
	wcc386 $(CCFLAGS) kconfig.c
kcurve.obj: 
	wcc386 $(CCFLAGS) kcurve.c
kfuncs.obj: 
	wcc386 $(CCFLAGS) kfuncs.c
kgame.obj: 
	wcc386 $(CCFLAGS) kgame.c
khelp.obj: 
	wcc386 $(CCFLAGS) khelp.c
kmatrix.obj: 
	wcc386 $(CCFLAGS) kmatrix.c
kmine.obj:
	wcc386 $(CCFLAGS) kmine.c
ksegmove.obj:
	wcc386 $(CCFLAGS) ksegmove.c
ksegsize.obj:
	wcc386 $(CCFLAGS) ksegsize.c
ktmap.obj:
	wcc386 $(CCFLAGS) ktmap.c
kview.obj:
	wcc386 $(CCFLAGS) kview.c
laser.obj:
	wcc386 $(CCFLAGS) laser.c
lighting.obj:
	wcc386 $(CCFLAGS) lighting.c
macro.obj: 
	wcc386 $(CCFLAGS) macro.c
mcockpit.obj: 
	wcc386 $(CCFLAGS) mcockpit.c
med.obj: 
	wcc386 $(CCFLAGS) med.c
meddraw.obj: 
	wcc386 $(CCFLAGS) meddraw.c
medmisc.obj: 
	wcc386 $(CCFLAGS) medmisc.c
medrobot.obj: 
	wcc386 $(CCFLAGS) medrobot.c
medsel.obj: 
	wcc386 $(CCFLAGS) medsel.c
medwall.obj: 
	wcc386 $(CCFLAGS) medwall.c
menu.obj: 
	wcc386 $(CCFLAGS) menu.c
mglobal.obj: 
	wcc386 $(CCFLAGS) mglobal.c
mine.obj: 
	wcc386 $(CCFLAGS) mine.c
mission.obj: 
	wcc386 $(CCFLAGS) mission.c
modem.obj: 
	wcc386 $(CCFLAGS) modem.c
morph.obj: 
	wcc386 $(CCFLAGS) morph.c
multi.obj: 
	wcc386 $(CCFLAGS) multi.c
multibot.obj:
	wcc386 $(CCFLAGS) multibot.c
netmisc.obj: 
	wcc386 $(CCFLAGS) netmisc.c
netware.obj: 
	wcc386 $(CCFLAGS) netware.c
network.obj: 
	wcc386 $(CCFLAGS) network.c
newdemo.obj: 
	wcc386 $(CCFLAGS) newdemo.c
newdemo1.obj:
	wcc386 $(CCFLAGS) newdemo1.c
newmenu.obj: 
	wcc386 $(CCFLAGS) newmenu.c
object.obj: 
	wcc386 $(CCFLAGS) object.c
objfly.obj: 
	wcc386 $(CCFLAGS) objfly.c
objpage.obj: 
	wcc386 $(CCFLAGS) objpage.c
ourcom.obj: 
	wcc386 $(CCFLAGS) ourcom.c
paging.obj: 
	wcc386 $(CCFLAGS) paging.c
photos.obj: 
	wcc386 $(CCFLAGS) photos.c
physics.obj:
	wcc386 $(CCFLAGS) physics.c
piggy.obj: 
	wcc386 $(CCFLAGS) piggy.c
playsave.obj: 
	wcc386 $(CCFLAGS) playsave.c
polyobj.obj: 
	wcc386 $(CCFLAGS) polyobj.c
powerup.obj: 
	wcc386 $(CCFLAGS) powerup.c
radar.obj: 
	wcc386 $(CCFLAGS) radar.c
render.obj: 
	wcc386 $(CCFLAGS) render.c
robot.obj: 
	wcc386 $(CCFLAGS) robot.c
scores.obj: 
	wcc386 $(CCFLAGS) scores.c
segment.obj: 
	wcc386 $(CCFLAGS) segment.c
seguvs.obj: 
	wcc386 $(CCFLAGS) seguvs.c
serial.obj: 
	wcc386 $(CCFLAGS) serial.c
ship.obj: 
	wcc386 $(CCFLAGS) ship.c
slew.obj: 
	wcc386 $(CCFLAGS) slew.c
songs.obj: 
	wcc386 $(CCFLAGS) songs.c
soscodec.obj: soscodec.asm
	masm soscodec.asm soscodec.obj;
state.obj: 
	wcc386 $(CCFLAGS) state.c
switch.obj:
	wcc386 $(CCFLAGS) switch.c
tempgame.obj: 
	wcc386 $(CCFLAGS) tempgame.c
terrain.obj: 
	wcc386 $(CCFLAGS) terrain.c
test1.obj:
	wcc386 $(CCFLAGS) test1.c
testgaug.obj:
	wcc386 $(CCFLAGS) testgaug.c
texmerge.obj:
	wcc386 $(CCFLAGS) texmerge.c
texpage.obj: 
	wcc386 $(CCFLAGS) texpage.c
text.obj: 
	wcc386 $(CCFLAGS) text.c
texture.obj: 
	wcc386 $(CCFLAGS) texture.c
titles.obj: 
	wcc386 $(CCFLAGS) titles.c
vclip.obj: 
	wcc386 $(CCFLAGS) vclip.c
vfx.obj: 
	wcc386 $(CCFLAGS) vfx.c
#vfxread.obj: 
#	wcc386 $(CCFLAGS) vfxread.c
victor.obj: 
	wcc386 $(CCFLAGS) victor.c
wall.obj: 
	wcc386 $(CCFLAGS) wall.c
weapon.obj: 
	wcc386 $(CCFLAGS) weapon.c
nocomlib.obj: nocomlib.c
	wcc386 $(CCFLAGS) nocomlib.c

