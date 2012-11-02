/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/
/* $Source: f:/miner/source/main/rcs/inferno.c $
 * $Revision: 2.36 $
 * $Author: john $
 * $Date: 1996/01/05 16:52:16 $
 *
 * main() for Inferno  
 *
 * $Log: inferno.c $
 * Revision 2.36  1996/01/05  16:52:16  john
 * Improved 3d stuff.
 * 
 * Revision 2.35  1995/10/07  13:20:06  john
 * Added new modes for LCDBIOS, also added support for -JoyNice,
 * and added Shift+F1-F4 to controls various stereoscopic params.
 * 
 * Revision 2.34  1995/06/26  11:30:57  john
 * Made registration/copyright screen go away after 5 minutes.
 * 
 * Revision 2.33  1995/05/31  14:26:55  unknown
 * Fixed ugly spacing.
 * 
 * Revision 2.32  1995/05/26  16:15:28  john
 * Split SATURN into define's for requiring cd, using cd, etc.
 * Also started adding all the Rockwell stuff.
 * 
 * Revision 2.31  1995/05/11  13:30:01  john
 * Changed 3dbios detection to work like Didde Kim wanted it to.
 * 
 * Revision 2.30  1995/05/08  13:53:50  john
 * Added code to read vipport environemnt variable.
 * 
 * Revision 2.29  1995/05/08  11:26:18  john
 * Reversed eyes in 3dmax mode.
 * 
 * Revision 2.28  1995/05/08  11:24:06  john
 * Made 3dmax work like Kasan wants it to.
 * 
 * Revision 2.27  1995/04/23  16:06:25  john
 * Moved rinvul into modem/null modem menu.
 * 
 * Revision 2.26  1995/04/12  13:39:26  john
 * Fixed bug with -lowmem not working.
 * 
 * Revision 2.25  1995/04/09  14:43:00  john
 * Made Dynamic sockets not print Msockets for help.
 * 
 * Revision 2.24  1995/04/07  16:11:33  john
 * Fixed problem with VFX display when using setup.
 * 
 * Revision 2.23  1995/04/06  15:40:51  john
 * Synced VFX with setup #'s.
 * 
 * Revision 2.22  1995/04/06  12:12:53  john
 * Fixed some bugs with 3dmax.
 * 
 * Revision 2.21  1995/03/30  16:36:51  mike
 * text localization.
 * 
 * Revision 2.20  1995/03/29  15:33:52  john
 * Added code to parse descent.net file.
 * 
 * Revision 2.19  1995/03/28  20:08:21  john
 * Took away alternate server thing.
 * 
 * Revision 2.18  1995/03/27  09:43:08  john
 * Added VR Settings in config file.
 * 
 * Revision 2.17  1995/03/23  19:02:21  john
 * Added descent.net file use.
 * 
 * Revision 2.16  1995/03/23  12:25:11  john
 * Moved IPX stuff into BIOS lib.
 * 
 * Revision 2.15  1995/03/21  16:52:34  john
 * Added 320x100.
 * 
 * Revision 2.14  1995/03/21  14:40:33  john
 * Ifdef'd out the NETWORK code.
 * 
 * Revision 2.13  1995/03/16  23:13:35  john
 * Fixed bug with piggy paging in bitmap not checking for disk
 * error, hence bogifying textures if you pull the CD out.
 * 
 * Revision 2.12  1995/03/16  21:45:22  john
 * Made all paged modes have incompatible menus!
 * 
 * Revision 2.11  1995/03/15  15:19:34  john
 * Took out code that changes to exe dir.
 * 
 * Revision 2.10  1995/03/15  14:33:37  john
 * Added code to force the Descent CD-rom in the drive.
 * 
 * Revision 2.9  1995/03/15  11:41:27  john
 * Better Saturn CD-ROM support.
 * 
 * Revision 2.8  1995/03/14  18:24:46  john
 * Force Destination Saturn to use CD-ROM drive.
 * 
 * Revision 2.7  1995/03/14  16:22:35  john
 * Added cdrom alternate directory stuff.
 * 
 * Revision 2.6  1995/03/13  15:17:19  john
 * Added alternate hogfile directory.
 * 
 * Revision 2.5  1995/03/10  13:05:35  john
 * Added code so that palette is correct for VFX1 helmets.
 * 
 * Revision 2.4  1995/03/07  15:12:43  john
 * Fixed VFX,3dmax support.
 * 
 * Revision 2.3  1995/03/07  14:19:35  mike
 * More destination saturn stuff.
 * 
 * Revision 2.2  1995/03/06  16:47:34  mike
 * destination saturn
 * 
 * Revision 2.1  1995/03/06  15:24:06  john
 * New screen techniques.
 * 
 * Revision 2.0  1995/02/27  11:31:29  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.295  1995/02/23  12:02:14  john
 * Made mono  windows smaller.
 * 
 * Revision 1.294  1995/02/16  17:35:00  john
 * Added code to allow dynamic socket changing.
 * 
 * Revision 1.293  1995/02/14  19:29:29  john
 * Locked down critical error handler.
 * 
 * Revision 1.292  1995/02/14  15:29:20  john
 * Added CR-LF to last line of menu help text.
 * 
 * Revision 1.291  1995/02/14  11:39:01  john
 * Added polled/bios joystick readers.
 * 
 * Revision 1.290  1995/02/13  20:35:03  john
 * Lintized
 * 
 * Revision 1.289  1995/02/11  16:20:02  john
 * Added code to make the default mission be the one last played.
 * 
 * Revision 1.288  1995/02/11  15:54:13  rob
 * changed cinvul to rinvul.
 * 
 * Revision 1.287  1995/02/11  14:48:43  rob
 * Added max of 314 seconds to control invul. times
 * 
 * Revision 1.286  1995/02/11  12:42:01  john
 * Added new song method, with FM bank switching..
 * 
 * Revision 1.285  1995/02/11  11:36:11  rob
 * Added cinvul option.
 * 
 * Revision 1.284  1995/02/10  16:07:45  matt
 * Took 'registered' out of printed info at startup
 * 
 * Revision 1.283  1995/02/09  22:00:59  john
 * Added i-glasses tracking.
 * 
 * Revision 1.282  1995/02/02  11:11:27  john
 * Added -nocyberman switch.
 * 
 * Revision 1.281  1995/02/01  16:35:14  john
 * Linted.
 * 
 * Revision 1.280  1995/01/31  02:04:25  matt
 * Fixed up cmdline help
 * 
 * Revision 1.279  1995/01/30  16:25:55  john
 * Put back in graphical screen at program end.
 * 
 * Revision 1.278  1995/01/28  17:05:50  matt
 * Changed imbedded copyright to use comma instead of hyphen
 * 
 * Revision 1.277  1995/01/28  15:57:26  john
 * Made joystick calibration be only when wrong detected in
 * menu or joystick axis changed.
 * 
 * Revision 1.276  1995/01/25  14:37:49  john
 * Made joystick only prompt for calibration once...
 * 
 * Revision 1.275  1995/01/24  18:21:00  john
 * Added Adam's text warning.
 * 
 * Revision 1.274  1995/01/22  15:57:20  john
 * Took out code that printed warning out as game exited.
 * 
 * Revision 1.273  1995/01/22  13:31:35  matt
 * Added load of mission 0, so there's always a default mission
 * 
 * Revision 1.272  1995/01/19  17:00:41  john
 * Made save game work between levels.
 * 
 * Revision 1.271  1995/01/18  11:47:57  adam
 * changed copyright notice
 * 
 * Revision 1.270  1995/01/15  13:42:42  john
 * Moved low_mem cutoff higher.
 * 
 * Revision 1.269  1995/01/12  18:53:50  john
 * Put ifdef EDITOR around the code that checked for 
 * a 800x600 mode, because this trashed some people's
 * computers (maybe) causing the mem allocation error in
 * mouse.c that many users reported.
 * 
 * Revision 1.268  1995/01/12  11:41:42  john
 * Added external control reading.
 * 
 * Revision 1.267  1995/01/06  10:26:55  john
 * Added -nodoscheck command line switch.
 * 
 * Revision 1.266  1995/01/05  16:59:30  yuan
 * Don't show orderform in editor version.
 * 
 * Revision 1.265  1994/12/28  15:33:51  john
 * Added -slowjoy option.
 * 
 * Revision 1.264  1994/12/15  16:44:15  matt
 * Added trademark notice
 * 
 * Revision 1.263  1994/12/14  20:13:59  john
 * Reduced physical mem requments to 2 MB.
 * 
 * Revision 1.262  1994/12/14  19:06:17  john
 * Lowered physical memory requments to 2 MB.
 * 
 * Revision 1.261  1994/12/14  09:41:29  allender
 * change to drive and directory (if needed) of command line invocation
 * so descent can be started from anywhere
 * 
 * Revision 1.260  1994/12/13  19:08:59  john
 * Updated memory requirements.
 * 
 * Revision 1.259  1994/12/13  17:30:33  john
 * Made the timer rate be changed right after initializing it.
 * 
 * Revision 1.258  1994/12/13  02:46:25  matt
 * Added imbedded copyright
 * 
 * Revision 1.257  1994/12/13  02:06:46  john
 * Added code to check stack used by descent... initial
 * check showed stack used 35k/50k, so we decided it wasn't
 * worth pursuing any more.
 * 
 * Revision 1.256  1994/12/11  23:17:54  john
 * Added -nomusic.
 * Added RealFrameTime.
 * Put in a pause when sound initialization error.
 * Made controlcen countdown and framerate use RealFrameTime.
 * 
 * Revision 1.255  1994/12/10  00:56:51  matt
 * Added -nomusic to command-line help
 * 
 * Revision 1.254  1994/12/08  11:55:11  john
 * Took out low memory print.
 * 
 * Revision 1.253  1994/12/08  11:51:00  john
 * Made strcpy only copy corect number of chars,.
 * 
 * Revision 1.252  1994/12/08  00:38:29  matt
 * Cleaned up banner messages
 * 
 * Revision 1.251  1994/12/07  19:14:52  matt
 * Cleaned up command-line options and command-line help message
 * 
 * Revision 1.250  1994/12/06  19:33:28  john
 * Fixed text of message to make more sense.
 * 
 * Revision 1.249  1994/12/06  16:30:55  john
 * Neatend mem message,..
 * 
 * Revision 1.248  1994/12/06  16:17:35  john
 * Added better mem checking/printing.
 * 
 * Revision 1.247  1994/12/06  14:14:37  john
 * Added code to set low mem based on memory.
 * 
 * Revision 1.246  1994/12/05  12:29:09  allender
 * removed ifdefs around -norun option
 * 
 * Revision 1.245  1994/12/05  00:03:30  matt
 * Added -norun option to exit after writing pig
 * 
 * Revision 1.244  1994/12/04  14:47:01  john
 * MAde the intro and menu be the same song.
 * 
 * Revision 1.243  1994/12/04  14:36:42  john
 * Added menu music.
 * 
 * Revision 1.242  1994/12/02  13:50:17  yuan
 * Localization.
 * 
 * Revision 1.241  1994/12/01  17:28:30  adam
 * added end-shareware stuff
 * 
 * Revision 1.240  1994/11/30  12:10:57  adam
 * added support for PCX titles/brief screens
 * 
 * Revision 1.239  1994/11/29  15:47:33  matt
 * Moved error_init to start of game, so error message prints last
 * 
 * Revision 1.238  1994/11/29  14:19:22  jasen
 * reduced dos mem requirments.
 * 
 * Revision 1.237  1994/11/29  03:46:35  john
 * Added joystick sensitivity; Added sound channels to detail menu.  Removed -maxchannels
 * command line arg.
 * 
 * Revision 1.236  1994/11/29  02:50:18  john
 * Increased the amount a joystick has to be off before
 * asking if they want to recalibrate their joystick.
 * 
 * Revision 1.235  1994/11/29  02:01:29  john
 * Corrected some of the Descent command line help items.
 * 
 * Revision 1.234  1994/11/29  01:39:56  john
 * Fixed minor bug with vfx_light help not wrapping correctly.
 * 
 * Revision 1.233  1994/11/28  21:34:17  john
 * Reduced dos mem rqment to 70k.
 * 
 * Revision 1.232  1994/11/28  21:20:38  john
 * First version with memory checking.
 * 
 * Revision 1.231  1994/11/28  20:06:21  rob
 * Removed old serial param command line options.
 * Added -noserial and -nonetwork to help listing.
 * 
 * Revision 1.230  1994/11/27  23:15:24  matt
 * Made changes for new mprintf calling convention
 * 
 * Revision 1.229  1994/11/27  20:50:51  matt
 * Don't set mem stuff if no debug
 * 
 * Revision 1.228  1994/11/27  18:46:21  matt
 * Cleaned up command-line switches a little
 * 
 * Revision 1.227  1994/11/21  17:48:00  matt
 * Added text to specifiy whether shareware or registered version
 * 
 * Revision 1.226  1994/11/21  14:44:20  john
 * Fixed some bugs with setting volumes even when -nosound was used. Duh!
 * 
 * Revision 1.225  1994/11/21  13:53:42  matt
 * Took out dos extender copyright
 * 
 * Revision 1.224  1994/11/21  09:46:54  john
 * Added -showmeminfo parameter.
 * 
 * Revision 1.223  1994/11/20  22:12:05  mike
 * Make some stuff dependent on SHAREWARE.
 * 
 * Revision 1.222  1994/11/20  21:14:09  john
 * Changed -serial to -noserial.  MAde a 1 sec delay 
 * before leaving title screen.  Clear keyboard buffer
 * before asking for player name.
 * 
 * Revision 1.221  1994/11/19  15:20:20  mike
 * rip out unused code and data
 * 
 * Revision 1.220  1994/11/17  19:14:29  adam
 * prevented order screen from coming up when -notitles is used
 * 
 * Revision 1.219  1994/11/16  11:34:39  john
 * Added -nottitle switch.
 * 
 * Revision 1.218  1994/11/16  10:05:53  john
 * Added verbose messages.
 * 
 * Revision 1.217  1994/11/15  20:12:34  john
 * Added back in inferno and parallax screens.
 * 
 * Revision 1.216  1994/11/15  18:35:30  john
 * Added verbose setting.
 * 
 * Revision 1.215  1994/11/15  17:47:44  john
 * Added ordering info screen.
 * 
 * Revision 1.214  1994/11/15  08:57:44  john
 * Added MS-DOS version checking and -nonetwork option.
 * 
 * Revision 1.213  1994/11/15  08:34:32  john
 * Added better error messages for IPX init.
 * 
 * Revision 1.212  1994/11/14  20:14:18  john
 * Fixed some warnings.
 * 
 * Revision 1.211  1994/11/14  19:50:49  john
 * Added joystick cal values to descent.cfg.
 * 
 * Revision 1.210  1994/11/14  17:56:44  allender
 * make call to ReadConfigFile at startup
 * 
 * Revision 1.209  1994/11/14  11:41:55  john
 * Fixed bug with editor/game sequencing.
 * 
 * Revision 1.208  1994/11/13  17:05:11  john
 * Made the callsign entry be a list box and gave the ability
 * to delete players.
 * 
 * Revision 1.207  1994/11/13  15:39:22  john
 * Added critical error handler to game.  Took out -editor command line
 * option because it didn't work anymore and wasn't worth fixing.  Made scores
 * not use MINER enviroment variable on release version, and made scores
 * not print an error if there is no descent.hi.
 * 
 * Revision 1.206  1994/11/10  20:53:29  john
 * Used new sound install parameters.
 * 
 * Revision 1.205  1994/11/10  11:07:52  mike
 * Set default detail level.
 * 
 * Revision 1.204  1994/11/09  13:45:43  matt
 * Made -? work again for help
 * 
 * Revision 1.203  1994/11/09  10:55:58  matt
 * Cleaned up initialization for editor -> game transitions
 * 
 * Revision 1.202  1994/11/07  21:35:47  matt
 * Use new function iff_read_into_bitmap()
 * 
 * Revision 1.201  1994/11/05  17:22:16  john
 * Fixed lots of sequencing problems with newdemo stuff.
 * 
 * Revision 1.200  1994/11/05  14:05:44  john
 * Fixed fade transitions between all screens by making gr_palette_fade_in and out keep
 * track of whether the palette is faded in or not.  Then, wherever the code needs to fade out,
 * it just calls gr_palette_fade_out and it will fade out if it isn't already.  The same with fade_in.
 * This eliminates the need for all the flags like Menu_fade_out, game_fade_in palette, etc.
 * 
 * Revision 1.199  1994/11/04  14:36:30  allender
 * change Auto_demo meaning to mean autostart from menu only.  Use
 * FindArgs when searching for AutoDemo from command line.  also,
 * set N_Players to 1 when starting in editor mode.
 * 
 * Revision 1.198  1994/11/02  11:59:49  john
 * Moved menu out of game into inferno main loop.
 * 
 * Revision 1.197  1994/11/01  17:57:39  mike
 * -noscreens option to bypass all screens.
 * 
 * Revision 1.196  1994/10/28  15:42:34  allender
 * don't register player if Autodemo is on
 * 
 * Revision 1.195  1994/10/28  10:58:01  matt
 * Added copyright notice for DOS4GW
 * 
 * Revision 1.194  1994/10/20  21:26:48  matt
 * Took out old serial name/number code, and put up message if this
 * is a marked version.
 * 
 * Revision 1.193  1994/10/19  09:52:14  allender
 * Print out who descent.exe belongs to if descent.exe is stamped.
 * 
 * Revision 1.192  1994/10/18  16:43:05  allender
 * Added check for identifier stamp and time after which descent will
 * no longer run.
 * 
 * Revision 1.191  1994/10/17  13:07:17  john
 * Moved the descent.cfg info into the player config file.
 * 
 * Revision 1.190  1994/10/04  10:26:31  matt
 * Support new menu fade in
 * 
 * Revision 1.189  1994/10/03  22:58:46  matt
 * Changed some values of game_mode
 * 
 * Revision 1.188  1994/10/03  18:55:39  rob
 * Changed defaults for com port settings.
 * 
 * Revision 1.187  1994/10/03  13:34:47  matt
 * Added new (and hopefully better) game sequencing functions
 * 
 * Revision 1.186  1994/09/30  12:37:28  john
 * Added midi,digi volume to configuration.
 * 
 * Revision 1.185  1994/09/30  10:08:48  john
 * Changed sound stuff... made it so the reseting card doesn't hang, 
 * made volume change only if sound is installed.
 * 
 * Revision 1.184  1994/09/28  17:25:00  matt
 * Added first draft of game save/load system
 * 
 * Revision 1.183  1994/09/28  16:18:23  john
 * Added capability to play midi song.
 * 
 * Revision 1.182  1994/09/28  11:31:18  john
 * Made text output unbuffered.
 * 
 * Revision 1.181  1994/09/27  19:23:44  john
 * Added -nojoystick and -nomouse
 * 
 * Revision 1.180  1994/09/24  16:55:29  rob
 * No longer open COM port immediately upon program start.
 * No longer set Network_active is serial_active is set.
 * 
 * Revision 1.179  1994/09/24  14:16:30  mike
 * Support new game mode constants.
 * 
 * Revision 1.178  1994/09/22  17:52:31  rob
 * Added Findargs hooks for -serial, -speed, and -com.
 * 
 * Revision 1.177  1994/09/22  16:14:11  john
 * Redid intro sequecing.
 * 
 * Revision 1.176  1994/09/21  16:32:58  john
 * Made mouse and keyboard init after bm_init. Why?
 * Because it seems to work better under virtual 
 * memory.
 * 
 * Revision 1.175  1994/09/21  16:27:52  john
 * Added mouse_init
 * 
 * Revision 1.174  1994/09/20  15:14:10  matt
 * New message for new VFX switches
 * 
 * Revision 1.173  1994/09/16  16:14:27  john
 * Added acrade sequencing.
 * 
 * Revision 1.172  1994/09/16  11:49:52  john
 * Added first version of arcade joystick support;
 * Also fixed some bugs in kconfig.c, such as reading non-present
 * joysticks, which killed frame rate, and not reading key_down_time
 * when in slide mode or bank mode.
 * 
 * Revision 1.171  1994/09/15  16:11:35  john
 * Added support for VFX1 head tracking. Fixed bug with memory over-
 * write when using stereo mode.
 * 
 * Revision 1.170  1994/09/12  19:38:23  john
 * Made some stuff that prints to the DOS screen go to the
 * mono instead, since it really is debugging info.
 * 
 * Revision 1.169  1994/08/29  21:18:28  john
 * First version of new keyboard/oystick remapping stuff.
 * 
 * Revision 1.168  1994/08/26  13:02:00  john
 * Put high score system in.
 * 
 * Revision 1.167  1994/08/24  19:00:23  john
 * Changed key_down_time to return fixed seconds instead of
 * milliseconds.
 * 
 * Revision 1.166  1994/08/18  16:24:20  john
 * changed socket to channel in text.
 * 
 * Revision 1.165  1994/08/18  16:16:51  john
 * Added support for different sockets.
 * 
 * Revision 1.164  1994/08/18  10:47:53  john
 * *** empty log message ***
 * 
 * Revision 1.163  1994/08/12  09:15:54  john
 * *** empty log message ***
 * 
 * Revision 1.162  1994/08/12  03:11:19  john
 * Made network be default off; Moved network options into
 * main menu.  Made starting net game check that mines are the
 * same.
 * 
 * Revision 1.161  1994/08/10  19:57:05  john
 * Changed font stuff; Took out old menu; messed up lots of
 * other stuff like game sequencing messages, etc.
 * 
 * Revision 1.160  1994/08/05  16:30:23  john
 * Added capability to turn off network.
 * 
 * Revision 1.159  1994/08/04  19:42:51  matt
 * Moved serial number & name (and version name) from inferno.c to inferno.ini
 * 
 * Revision 1.158  1994/08/03  10:30:23  matt
 * Change cybermaxx switches, updated command-line help, and added serial number system
 * 
 * Revision 1.157  1994/07/29  18:30:10  matt
 * New parms (lack of parms, actually) for g3_init()
 * 
 * Revision 1.156  1994/07/24  00:39:25  matt
 * Added more text to TEX file; make NewGame() take a start level; made game
 * load/save menus use open/close window funcs.
 * 
 * Revision 1.155  1994/07/21  21:31:27  john
 * First cheapo version of VictorMaxx tracking.
 * 
 * Revision 1.154  1994/07/21  18:15:34  matt
 * Ripped out a bunch of unused stuff
 * 
 * Revision 1.153  1994/07/21  17:59:10  matt
 * Cleaned up initial mode game/editor code
 * 
 * Revision 1.152  1994/07/21  13:11:19  matt
 * Ripped out remants of old demo system, and added demo only system that
 * disables object movement and game options from menu.
 * 
 * Revision 1.151  1994/07/20  15:58:27  john
 * First installment of ipx stuff.
 * 
 * Revision 1.150  1994/07/15  16:04:24  matt
 * Changed comment for milestone 3 version
 * 
 * Revision 1.149  1994/07/15  13:59:24  matt
 * Fixed stupid mistake I make in the last revision
 * 
 * Revision 1.148  1994/07/15  13:20:15  matt
 * Updated comand-line help
 * 
 * Revision 1.147  1994/07/14  23:29:43  matt
 * Open two mono debug messages, one for errors & one for spew
 * 
 * Revision 1.146  1994/07/09  22:48:05  matt
 * Added localizable text
 * 
 * Revision 1.145  1994/07/02  13:49:47  matt
 * Cleaned up includes
 * 
 * Revision 1.144  1994/06/30  20:04:43  john
 * Added -joydef support.
 * 
 * Revision 1.143  1994/06/24  17:01:44  john
 * Add VFX support; Took Game Sequencing, like EndGame and stuff and
 * took it out of game.c and into gameseq.c
 * 
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: inferno.c 2.36 1996/01/05 16:52:16 john Exp $";
static char copyright[] = "DESCENT   COPYRIGHT (C) 1994,1995 PARALLAX SOFTWARE CORPORATION";
#pragma on (unreferenced)

#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <direct.h>

#include "gr.h"
#include "ui.h"
#include "mono.h"
#include "key.h"
#include "timer.h"
#include "3d.h"
#include "bm.h"
#include "inferno.h"
#include "error.h"
#include "cflib.h"
#include "div0.h"
#include "game.h"
#include "segment.h"		//for Side_to_verts
#include "mem.h"
#include "textures.h"
#include "segpoint.h"
#include "screens.h"
#include "texmap.h"
#include "texmerge.h"
#include "menu.h"
#include "wall.h"
#include "switch.h"
#include "polyobj.h"
#include "effects.h"
#include "digi.h"
#include "iff.h"
#include "pcx.h"
#include "palette.h"
#include "args.h"
#include "sounds.h"
#include "titles.h"
#include "player.h"
#include "text.h"
#include "ipx.h"
#include "newdemo.h"
#include "victor.h"
#include "network.h"
#include "modem.h"
#include "gamefont.h"
#include "kconfig.h"
#include "arcade.h"
#include "coindev.h"
#include "mouse.h"
#include "joy.h"
#include "dpmi.h"
#include "newmenu.h"
#include "desc_id.h"
#include "config.h"
#include "joydefs.h"
#include "multi.h"
#include "iglasses.h"
#include "songs.h"
#include "cfile.h"
#include "cdrom.h"
#include "gameseq.h"

#ifdef EDITOR
#include "editor\editor.h"
#include "editor\kdefs.h"
#endif

#include "vers_id.h"

extern int Game_simuleyes_flag;

static const char desc_id_checksum_str[] = DESC_ID_CHKSUM;
char desc_id_exit_num = 0;

int Function_mode=FMODE_MENU;		//game or editor?
int Screen_mode=-1;					//game screen or editor screen?

//--unused-- grs_bitmap Inferno_bitmap_title;

int WVIDEO_running=0;		//debugger can set to 1 if running

#ifdef EDITOR
int Inferno_is_800x600_available = 0;
#endif

//--unused-- int Cyberman_installed=0;			// SWIFT device present

void install_int3_handler(void);

int __far descent_critical_error_handler( unsigned deverr, unsigned errcode, unsigned far * devhdr );

#ifndef NDEBUG
do_heap_check()
{
	int heap_status;

	heap_status = _heapset( 0xFF );
	switch( heap_status )
	{
	case _HEAPBADBEGIN:
		mprintf((1, "ERROR - heap is damaged\n"));
		Int3();
		break;
	case _HEAPBADNODE:
		mprintf((1, "ERROR - bad node in heap\n" ));
		Int3();
		break;
	}
}
#endif

int registered_copy=0;
char name_copy[sizeof(DESC_ID_STR)];

void
check_id_checksum_and_date()
{
	const char name[] = DESC_ID_STR;
	char time_str[] = DESC_DEAD_TIME;
	int i, found;
	unsigned long *checksum, test_checksum;
	time_t current_time, saved_time;

	saved_time = (time_t)strtol(&(time_str[strlen(time_str) - 10]), NULL, 16);
	if (saved_time == (time_t)0)
		return;

	strcpy(name_copy,name);
	registered_copy = 1;

	current_time = time(NULL);
	if (current_time >= saved_time)
		desc_id_exit_num = 1;

	test_checksum = 0;
	for (i = 0; i < strlen(name); i++) {
		found = 0;	  
		test_checksum += name[i];
		if (((test_checksum / 2) * 2) != test_checksum)
			found = 1;
		test_checksum = test_checksum >> 1;
		if (found)
			test_checksum |= 0x80000000;
	}
	checksum = (unsigned long *)&(desc_id_checksum_str[0]);
	if (test_checksum != *checksum)
		desc_id_exit_num = 2;

	printf ("%s %s\n", TXT_REGISTRATION, name);
}

int is_3dbios_installed()
{
	dpmi_real_regs rregs;
	memset(&rregs,0,sizeof(dpmi_real_regs));
	rregs.eax = 0x4ed0;
	//rregs.ebx = 0x3d10;	
	dpmi_real_int386x( 0x10, &rregs );
	if ( (rregs.edx & 0xFFFF) != 0x3344 )
		return 0;
	else
		return 1;

}


int init_graphics()
{
	int result;

	result=gr_check_mode(SM_320x200C);
#ifdef EDITOR
	if ( result==0 )	
		result=gr_check_mode(SM_800x600V);
#endif

	switch( result )	{
		case  0:		//Mode set OK
#ifdef EDITOR
						Inferno_is_800x600_available = 1;
#endif
						break;
		case  1:		//No VGA adapter installed
						printf("%s\n", TXT_REQUIRES_VGA );
						return 1;
		case 10:		//Error allocating selector for A0000h
						printf( "%s\n",TXT_ERROR_SELECTOR );
						return 1;
		case 11:		//Not a valid mode support by gr.lib
						printf( "%s\n", TXT_ERROR_GRAPHICS );
						return 1;
#ifdef EDITOR
		case  3:		//Monitor doesn't support that VESA mode.
		case  4:		//Video card doesn't support that VESA mode.

						printf( "Your VESA driver or video hardware doesn't support 800x600 256-color mode.\n" );
						break;
		case  5:		//No VESA driver found.
						printf( "No VESA driver detected.\n" );
						break;
		case  2:		//Program doesn't support this VESA granularity
		case  6:		//Bad Status after VESA call/
		case  7:		//Not enough DOS memory to call VESA functions.
		case  8:		//Error using DPMI.
		case  9:		//Error setting logical line width.
		default:
						printf( "Error %d using 800x600 256-color VESA mode.\n", result );
						break;
#endif
	}

	return 0;
}

extern fix fixed_frametime;

// Returns 1 if ok, 0 if failed...
int init_gameport()
{
	union REGS regs;

	memset(&regs,0,sizeof(regs));
	regs.x.eax = 0x8400;
	regs.x.edx = 0xF0;
   int386( 0x15, &regs, &regs );
	if ( ( regs.x.eax & 0xFFFF ) == 'SG' )
		return 1;
	else
		return 0;
}

void check_dos_version()
{
	int major, minor;
	union REGS regs;

	memset(&regs,0,sizeof(regs));
	regs.x.eax = 0x3000;							// Get MS-DOS Version Number
   int386( 0x21, &regs, &regs );

	major = regs.h.al;
	minor = regs.h.ah;
	
	if ( major < 5 )	{
		printf( "%s %d.d\n%s", TXT_DOS_VERSION_1, major, minor, TXT_DOS_VERSION_2);
		exit(1);
	}
	//printf( "\nUsing MS-DOS %d.%d...\n", major, minor );
}

void change_to_dir(char *cmd_line)
{
	char drive[_MAX_DRIVE], dir[_MAX_DIR], curdir[_MAX_DIR];
	unsigned total, cur_drive;

	_splitpath(cmd_line, drive, dir, NULL, NULL);
	dir[strlen(dir) - 1] = '\0';
	if (drive[0] != '\0') {
		_dos_getdrive(&cur_drive);
		if (cur_drive != (drive[0] - 'A' + 1))
			_dos_setdrive(drive[0] - 'A' + 1, &total);
	}
	getcwd(curdir, _MAX_DIR);
	if (stricmp(&(curdir[2]), dir))
		chdir(dir);
}

void dos_check_file_handles(int num_required)
{
	int i, n;
	FILE * fp[16];

	if ( num_required > 16 )
		num_required = 16;

	n = 0;	
	for (i=0; i<16; i++ )
		fp[i] = NULL;
	for (i=0; i<16; i++ )	{
		fp[i] = fopen( "nul", "wb" );
		if ( !fp[i] ) break;
	}
	n = i;
	for (i=0; i<16; i++ )	{
		if (fp[i])
			fclose(fp[i]);
	}
	if ( n < num_required )	{
		printf( "\n%s\n", TXT_NOT_ENOUGH_HANDLES );
		printf( "------------------------\n" );
		printf( "%d/%d %s\n", n, num_required, TXT_HANDLES_1 );
		printf( "%s\n", TXT_HANDLES_2);
		printf( "%s\n", TXT_HANDLES_3);
		exit(1);
	}
}

#define NEEDED_DOS_MEMORY   			( 300*1024)		// 300 K
#define NEEDED_LINEAR_MEMORY 			(7680*1024)		// 7.5 MB
#define LOW_PHYSICAL_MEMORY_CUTOFF	(5*1024*1024)	// 5.0 MB
#define NEEDED_PHYSICAL_MEMORY		(2000*1024)		// 2000 KB

extern int piggy_low_memory;

void mem_int_to_string( int number, char *dest )
{
	int i,l,c;
	char buffer[20],*p;

	sprintf( buffer, "%d", number );

	l = strlen(buffer);
	if (l<=3) {
		// Don't bother with less than 3 digits
		sprintf( dest, "%d", number );
		return;
	}

	c = 0;
	p=dest;
	for (i=l-1; i>=0; i-- )	{
		if (c==3) {
			*p++=',';
			c = 0;
		}
		c++;
		*p++ = buffer[i];
	}
	*p++ = '\0';
	strrev(dest);
}

void check_memory()
{
	char text[32];

	printf( "\n%s\n", TXT_AVAILABLE_MEMORY);
	printf( "----------------\n" );
	mem_int_to_string( dpmi_dos_memory/1024, text );
	printf( "Conventional: %7s KB\n", text );
	mem_int_to_string( dpmi_physical_memory/1024, text );
	printf( "Extended:     %7s KB\n", text );
	if ( dpmi_available_memory > dpmi_physical_memory )	{
		mem_int_to_string( (dpmi_available_memory-dpmi_physical_memory)/1024, text );
	} else {
		mem_int_to_string( 0, text );
	}
	printf( "Virtual:      %7s KB\n", text );
	printf( "\n" );

	if ( dpmi_dos_memory < NEEDED_DOS_MEMORY )	{
		printf( "%d %s\n", NEEDED_DOS_MEMORY - dpmi_dos_memory, TXT_MEMORY_CONFIG );
		exit(1);
	}

	if ( dpmi_available_memory < NEEDED_LINEAR_MEMORY )	{
		if ( dpmi_virtual_memory )	{
			printf( "%d %s\n", NEEDED_LINEAR_MEMORY - dpmi_available_memory, TXT_RECONFIGURE_VMM );
		} else {
			printf( "%d %s\n", NEEDED_LINEAR_MEMORY - dpmi_available_memory, TXT_MORE_MEMORY );
			printf( "%s\n", TXT_MORE_MEMORY_2);
		}
		exit(1);
	}

	if ( dpmi_physical_memory < NEEDED_PHYSICAL_MEMORY )	{
		printf( "%d %s\n", NEEDED_PHYSICAL_MEMORY - dpmi_physical_memory, TXT_PHYSICAL_MEMORY );
		if ( dpmi_virtual_memory )	{	
			printf( "%s\n", TXT_PHYSICAL_MEMORY_2);
		}
		exit(1);
	}

	if ( dpmi_physical_memory < LOW_PHYSICAL_MEMORY_CUTOFF )	{
		piggy_low_memory = 1;
	}

}


int Inferno_verbose = 0;

//NO_STACK_SIZE_CHECK uint * stack, *stack_ptr;
//NO_STACK_SIZE_CHECK int stack_size, unused_stack_space;
//NO_STACK_SIZE_CHECK int sil;
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK int main(int argc,char **argv)
//NO_STACK_SIZE_CHECK {
//NO_STACK_SIZE_CHECK 	uint ret_value;
//NO_STACK_SIZE_CHECK 	
//NO_STACK_SIZE_CHECK 	unused_stack_space = 0;
//NO_STACK_SIZE_CHECK 	stack = &ret_value;
//NO_STACK_SIZE_CHECK 	stack_size = stackavail()/4;
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	for ( sil=0; sil<stack_size; sil++ )	{
//NO_STACK_SIZE_CHECK 		stack--;
//NO_STACK_SIZE_CHECK 		*stack = 0xface0123;
//NO_STACK_SIZE_CHECK 	}
//NO_STACK_SIZE_CHECK
//NO_STACK_SIZE_CHECK 	ret_value = descent_main( argc, argv );		// Rename main to be descent_main
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	for ( sil=0; sil<stack_size; sil++ )	{
//NO_STACK_SIZE_CHECK 		if ( *stack == 0xface0123 )	
//NO_STACK_SIZE_CHECK 			unused_stack_space++;
//NO_STACK_SIZE_CHECK 		stack++;
//NO_STACK_SIZE_CHECK 	}
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	mprintf(( 0, "Program used %d/%d stack space\n", (stack_size - unused_stack_space)*4, stack_size*4 ));
//NO_STACK_SIZE_CHECK 	key_getch();
//NO_STACK_SIZE_CHECK 
//NO_STACK_SIZE_CHECK 	return ret_value;
//NO_STACK_SIZE_CHECK }

extern int digi_timer_rate;

int descent_critical_error = 0;
unsigned descent_critical_deverror = 0;
unsigned descent_critical_errcode = 0;



#pragma off (check_stack)
int __far descent_critical_error_handler(unsigned deverror, unsigned errcode, unsigned __far * devhdr )
{
	devhdr = devhdr;
	descent_critical_error++;
	descent_critical_deverror = deverror;
	descent_critical_errcode = errcode;
	return _HARDERR_FAIL;
}
void chandler_end (void)  // dummy functions
{
}
#pragma on (check_stack)

extern int Network_allow_socket_changes;

extern void vfx_set_palette_sub(ubyte *);

extern int Game_vfx_flag;
extern int Game_victor_flag;
extern int Game_vio_flag;
extern int Game_3dmax_flag;
extern int VR_low_res;
extern void vfx_init();

#ifdef USE_CD
char destsat_cdpath[128] = "";
int find_descent_cd();
#endif

extern int Config_vr_type;
extern int Config_vr_tracking;

int main(int argc,char **argv)
{
	int i,t;
	ubyte title_pal[768];

	error_init(NULL);

	setbuf(stdout, NULL);	// unbuffered output via printf
		
	InitArgs( argc,argv );

	if ( FindArg( "-verbose" ) )
		Inferno_verbose = 1;

	//change_to_dir(argv[0]);

	// Initialize DPMI before anything else!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// (To check memory size and availbabitliy and allocate some low DOS memory)
	if (Inferno_verbose) printf( "%s... ", TXT_INITIALIZING_DPMI);
	dpmi_init(Inferno_verbose);		// Before anything
	if (Inferno_verbose) printf( "\n" );

	if (Inferno_verbose) printf( "\n%s...", TXT_INITIALIZING_CRIT);
	if (!dpmi_lock_region((void near *)descent_critical_error_handler,(char *)chandler_end - (char near *)descent_critical_error_handler))	{
		Error( "Unable to lock critial error handler" );
	}
	if (!dpmi_lock_region(&descent_critical_error,sizeof(int)))	{
		Error( "Unable to lock critial error handler" );
	}
	if (!dpmi_lock_region(&descent_critical_deverror,sizeof(unsigned)))	{
		Error( "Unable to lock critial error handler" );
	}
	if (!dpmi_lock_region(&descent_critical_errcode,sizeof(unsigned)))	{
		Error( "Unable to lock critial error handler" );
	}
	_harderr((void *) descent_critical_error_handler );
	//Above line modified by KRB, added (void *) cast
	//for the compiler.

#ifdef USE_CD
	i=find_descent_cd();
	if ( i>0 )		{
		sprintf( destsat_cdpath, "%c:\\descent\\", i +'a' - 1  );
		cfile_use_alternate_hogdir( destsat_cdpath );
	} 
#ifdef REQUIRE_CD
	else {		// NOTE ABOVE LINK!!!!!!!!!!!!!!!!!!
		printf( "\n\n" );
#ifdef DEST_SAT
		printf("Couldn't find the 'Descent: Destination Saturn' CD-ROM.\n" );
#else
		printf("Couldn't find the Descent CD-ROM.\n" );
#endif
		printf("Please make sure that it is in your CD-ROM drive and\n" );
		printf("that your CD-ROM drivers are loaded correctly.\n" );
		exit(1);
	}
#endif
#endif

	load_text();

//	set_exit_message("\n\n%s", TXT_THANKS);

	printf("\nDESCENT   %s\n", VERSION_NAME);
	printf("%s\n%s\n",TXT_COPYRIGHT,TXT_TRADEMARK);	

	check_id_checksum_and_date();

	if (FindArg( "-?" ) || FindArg( "-help" ) || FindArg( "?" ) )	{

		printf( "%s\n", TXT_COMMAND_LINE_0 );

		printf("  -SimulEyes     %s\n",
				"Enables StereoGraphics SimulEyes VR stereo display" );

		printf("  -Iglasses      %s\n", TXT_IGLASSES );
		printf("  -VioTrack <n>  %s n\n",TXT_VIOTRACK );
		printf("  -3dmaxLo       %s\n",TXT_KASAN );
		printf("                 %s\n",TXT_KASAN_2 );
		printf("  -3dmaxHi       %s\n",TXT_3DMAX );
		printf( "%s\n", TXT_COMMAND_LINE_1 );
		printf( "%s\n", TXT_COMMAND_LINE_2 );
		printf( "%s\n", TXT_COMMAND_LINE_3 );
		printf( "%s\n", TXT_COMMAND_LINE_4 );
		printf( "%s\n", TXT_COMMAND_LINE_5 );
//		printf( "\n");
		printf( "%s\n", TXT_COMMAND_LINE_6 );
		printf( "%s\n", TXT_COMMAND_LINE_7 );
		printf( "%s\n", TXT_COMMAND_LINE_8 );
//		printf( "\n");
		printf( "\n%s\n",TXT_PRESS_ANY_KEY3);
		getch();
		printf( "\n" );
		printf( "%s\n", TXT_COMMAND_LINE_9);
		printf( "%s\n", TXT_COMMAND_LINE_10);
		printf( "%s\n", TXT_COMMAND_LINE_11);
		printf( "%s\n", TXT_COMMAND_LINE_12);
		printf( "%s\n", TXT_COMMAND_LINE_13);
		printf( "%s\n", TXT_COMMAND_LINE_14);
		printf( "%s\n", TXT_COMMAND_LINE_15);
		printf( "%s\n", TXT_COMMAND_LINE_16);
		printf( "%s\n", TXT_COMMAND_LINE_17);
		printf( "%s\n", TXT_COMMAND_LINE_18);
      printf( "  -DynamicSockets %s\n", TXT_SOCKET);
      printf( "  -NoFileCheck    %s\n", TXT_NOFILECHECK);
      printf( "  -GamePort       %s\n", "Use Colorado Spectrum's Notebook Gameport" );
		printf( "  -NoDoubleBuffer %s\n", "Use only one page of video memory" );
		printf( "  -LCDBios        %s\n", "Enables LCDBIOS for using LCD shutter glasses" );
		printf( "  -JoyNice        %s\n", "Joystick poller allows interrupts to occur" );
		set_exit_message("");
		return(0);
	}

	printf("\n%s\n", TXT_HELP);	

	#ifdef PASSWORD
	if ((t = FindArg("-pswd")) != 0) {
		int	n;
		byte	*pp = Side_to_verts;
		int ch;
		for (n=0; n<6; n++)
			for (ch=0; ch<strlen(Args[t+1]); ch++)
				*pp++ ^= Args[t+1][ch];
	}
	else 
		Error("Invalid processor");		//missing password
	#endif

	if ( FindArg( "-autodemo" ))
		Auto_demo = 1;

	#ifndef RELEASE
	if ( FindArg( "-noscreens" ) )
		Skip_briefing_screens = 1;
	#endif

	Lighting_on = 1;

	if ( !FindArg( "-nodoscheck" ))
		check_dos_version();
	
	if ( !FindArg( "-nofilecheck" ))
		dos_check_file_handles(5);

	if ( !FindArg( "-nomemcheck" ))
		check_memory();

	strcpy(Menu_pcx_name, "menu.pcx");	//	Used to be menu2.pcx.

	if (init_graphics()) return 1;

	#ifdef EDITOR
	if (!Inferno_is_800x600_available)	{
		printf( "The editor will not be available, press any key to start game...\n" );
		Function_mode = FMODE_MENU;
		getch();
	}
	#endif

	#ifndef NDEBUG
		minit();
		mopen( 0, 9, 1, 78, 15, "Debug Spew");
		mopen( 1, 2, 1, 78,  5, "Errors & Serious Warnings");
	#endif

	if (!WVIDEO_running)
		mprintf((0,"WVIDEO_running = %d\n",WVIDEO_running));

	//if (!WVIDEO_running) install_int3_handler();

	//lib_init("INFERNO.DAT");

	if (Inferno_verbose) printf ("%s", TXT_VERBOSE_1);
	ReadConfigFile();
	if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_2);

	timer_init();
	timer_set_rate( digi_timer_rate );			// Tell our timer how fast to go (120 Hz)
	joy_set_timer_rate( digi_timer_rate );	 	// Tell joystick how fast timer is going

	if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_3);
	key_init();
	if (!FindArg( "-nomouse" ))	{
		if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_4);
		if (FindArg( "-nocyberman" ))
			mouse_init(0);
		else
			mouse_init(1);
	} else {
	 	if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_5);
	}
	if (!FindArg( "-nojoystick" ))	{
		if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_6);
		joy_init();
		if ( FindArg( "-joyslow" ))	{
			if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_7);
			joy_set_slow_reading(JOY_SLOW_READINGS);
		}
		if ( FindArg( "-joypolled" ))	{
			if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_8);
			joy_set_slow_reading(JOY_POLLED_READINGS);
		}
		if ( FindArg( "-joybios" ))	{
			if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_9);
			joy_set_slow_reading(JOY_BIOS_READINGS);
		}
		if ( FindArg( "-joynice" ))	{
			if (Inferno_verbose) printf( "\n%s", "Using nice joystick poller..." );
			joy_set_slow_reading(JOY_FRIENDLY_READINGS);
		}
		if ( FindArg( "-gameport" ))	{
			if ( init_gameport() )	{			
				joy_set_slow_reading(JOY_BIOS_READINGS);
			} else {
				Error( "\nCouldn't initialize the Notebook Gameport.\nMake sure the NG driver is loaded.\n" );
			}
		}
	} else {
		if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_10);
	}
	if (Inferno_verbose) printf( "\n%s", TXT_VERBOSE_11);
	div0_init(DM_ERROR);

	//------------ Init sound ---------------
	if (!FindArg( "-nosound" ))	{
		if (digi_init())	{
			printf( "\n%s\n", TXT_PRESS_ANY_KEY3);
			key_getch();
		}
	} else {
		if (Inferno_verbose) printf( "\n%s",TXT_SOUND_DISABLED );
	}

#ifdef NETWORK
	if (!FindArg( "-nonetwork" ))	{
		int socket=0, showaddress=0;
		int ipx_error;
		if (Inferno_verbose) printf( "\n%s ", TXT_INITIALIZING_NETWORK);
		if ((t=FindArg("-socket")))
			socket = atoi( Args[t+1] );
		if ( FindArg("-showaddress") ) showaddress=1;
		if ((ipx_error=ipx_init(IPX_DEFAULT_SOCKET+socket,showaddress))==0)	{
  			if (Inferno_verbose) printf( "%s %d.\n", TXT_IPX_CHANNEL, socket );
			Network_active = 1;
		} else {
			switch( ipx_error )	{
			case 3: 	if (Inferno_verbose) printf( "%s\n", TXT_NO_NETWORK); break;
			case -2: if (Inferno_verbose) printf( "%s 0x%x.\n", TXT_SOCKET_ERROR, IPX_DEFAULT_SOCKET+socket); break;
			case -4: if (Inferno_verbose) printf( "%s\n", TXT_MEMORY_IPX ); break;
			default:
				if (Inferno_verbose) printf( "%s %d", TXT_ERROR_IPX, ipx_error );
			}
			if (Inferno_verbose) printf( "%s\n",TXT_NETWORK_DISABLED);
			Network_active = 0;		// Assume no network
		}
		ipx_read_user_file( "descent.usr" );
		ipx_read_network_file( "descent.net" );
		if ( FindArg( "-dynamicsockets" ))
			Network_allow_socket_changes = 1;
		else
			Network_allow_socket_changes = 0;
	} else {
		if (Inferno_verbose) printf( "%s\n", TXT_NETWORK_DISABLED);
		Network_active = 0;		// Assume no network
	}

	if (!FindArg("-noserial"))
	{
		serial_active = 1;
	}
	else 
	{
		serial_active = 0;
	}
#endif

	i = FindArg( "-vfxtrak" );
	if ( i > 0 )
		kconfig_sense_init();
	else if ((Config_vr_type==1)&&(Config_vr_tracking>0))
		kconfig_sense_init();
		
	i = FindArg( "-maxxtrak" );
	if ( i > 0)	
		victor_init_tracking( atoi(Args[i+1]) );
	else if ((Config_vr_type==2)&&(Config_vr_tracking>0))
		victor_init_tracking( Config_vr_tracking );

	i = FindArg( "-viotrack" );
	if ( i > 0 )
		iglasses_init_tracking( atoi(Args[i+1]) );
	else if ((Config_vr_type==3)&&(Config_vr_tracking>0))
		iglasses_init_tracking( Config_vr_tracking );

	if ( FindArg( "-vfx" ) || (Config_vr_type==1) )	{
		vfx_init();
		Game_vfx_flag = 1;
		game_init_render_buffers(SM_640x480V, 320, 240, 0, VR_AREA_DET, 0 );
		VR_low_res = 3;
	} else if ( FindArg( "-cybermaxx" ) || (Config_vr_type==2) )	{
		Game_victor_flag = 1;
		game_init_render_buffers(SM_320x200C, 320, 100, 0, VR_INTERLACED, 1 );
		VR_low_res = 0;
	} else if ( FindArg( "-iglasses" ) || (Config_vr_type==3) )	{
		Game_vio_flag = 1;
		game_init_render_buffers(SM_320x400U, 320, 200, 1, VR_INTERLACED, 0 );
		VR_low_res = 3;
	} else if ( FindArg( "-cybermax2" ) || (Config_vr_type==6) )	{
		game_init_render_buffers(SM_320x400U, 320, 200, 1, VR_INTERLACED, 0 );
		VR_low_res = 3;
	} else if ( FindArg( "-3dmaxlo" ) || (Config_vr_type==4) )	{
		if (!is_3dbios_installed())	{
			printf( "Error: Kasan 3DBIOS needs to be installed.\n" );
			exit(1);
		}
		Game_3dmax_flag = 1;
		game_init_render_buffers(SM_320x200C, 320, 100, 0, VR_INTERLACED, 0 );
		VR_low_res = 0;
		VR_switch_eyes = 0;
	} else if ( FindArg( "-3dmaxhi" ) || (Config_vr_type==5) )	{
		if (!is_3dbios_installed())	{
			printf( "Error: Kasan's 3DBIOS needs to be installed.\n" );
			exit(1);
		}
		Game_3dmax_flag = 2;
		game_init_render_buffers(22, 320, 200, 1, VR_INTERLACED, 0 );
		VR_switch_eyes = 0;
		VR_low_res = 0;
	} else {
		int screen_mode = SM_320x200C;
		int screen_width = 320;
		int screen_height = 200;
		int vr_mode = VR_NONE;
		int screen_compatible = 1;
		int use_double_buffer = 0;

		if ( FindArg( "-lcdbios" ) )	{
			if (!is_3dbios_installed())	{
				printf( "Warning: LCDBIOS needs to be installed.\nPress Esc to continue anyway, any other key to exit.\n" );
				if ( key_getch()!=KEY_ESC )
					exit(1);
			}

			if (Inferno_verbose) printf( "Enabling LCDBIOS...\n" );
			Game_3dmax_flag = 3;
			screen_compatible = 0;
			VR_switch_eyes = 0;
			VR_low_res = 0;
			vr_mode = VR_INTERLACED;
		}

		if ( FindArg( "-simuleyes" ))  {
			Game_simuleyes_flag = 1;
		}

		if (Game_simuleyes_flag)  {  // default to 320x400 (x200 per eye)
			screen_mode = SM_320x400U;
			screen_width = 320;	
			screen_height = 400;
			screen_compatible = 0;
			use_double_buffer = 1;

			if ( FindArg( "-320x200" ))	{
				if (Inferno_verbose) printf( "Using 320x200...\n" );
				screen_mode = SM_320x200C;
				screen_width = 320;
				screen_height = 200;
				use_double_buffer = 0;
			}
			else  {
				if (Inferno_verbose) printf( "Using 320x400 ModeX...\n" );
			}
		}

		if ( FindArg( "-320x240" ))	{
			if (Inferno_verbose) printf( "Using 320x240 ModeX...\n" );
			screen_mode = SM_320x240U; 
			screen_width = 320;	
			screen_height = 240;
			screen_compatible = 0;
			use_double_buffer = 1;
		}
		if ( FindArg( "-320x400" ))	{
			if (Inferno_verbose) printf( "Using 320x400 ModeX...\n" );
			screen_mode = SM_320x400U; 
			screen_width = 320;	
			screen_height = 400;
			screen_compatible = 0;
			use_double_buffer = 1;
		}

		if (!Game_simuleyes_flag && FindArg( "-640x400" ))	{
			if (Inferno_verbose) printf( "Using 640x400 VESA...\n" );
			screen_mode = SM_640x400V; 
			screen_width = 640;	
			screen_height = 400;
			screen_compatible = 0;
			use_double_buffer = 1;
		}

		if (!Game_simuleyes_flag && FindArg( "-640x480" ))	{
			if (Inferno_verbose) printf( "Using 640x480 VESA...\n" );
			screen_mode = SM_640x480V; 
			screen_width = 640;	
			screen_height = 480;
			screen_compatible = 0;
			use_double_buffer = 1;
		}
		if ( FindArg( "-320x100" ))	{
			if (Inferno_verbose) printf( "Using 320x100 VGA...\n" );
			screen_mode = 19; 
			screen_width = 320;	
			screen_height = 100;
			screen_compatible = 0;
		}

		if ( FindArg( "-nodoublebuffer" ) )	{
			if (Inferno_verbose) printf( "Double-buffering disabled...\n" );
			use_double_buffer = 0;
		}

		if ( vr_mode == VR_INTERLACED ) 
			screen_height /= 2;
		game_init_render_buffers(screen_mode, screen_width, screen_height, use_double_buffer, vr_mode, screen_compatible );
	}

	if (Game_victor_flag) {
		char *vswitch = getenv( "CYBERMAXX" );
		if ( vswitch )	{
			char *p = strstr( vswitch, "/E:R" ); 
			if ( p )	{
				VR_switch_eyes = 1;
			} else 
				VR_switch_eyes = 0;
		} else {		
		 	VR_switch_eyes = 0;
		}
	}


#ifdef ARCADE
	i = FindArg( "-arcade" );
	if (i > 0 )	{
		arcade_init();
		coindev_init(0);
	}
#endif

#ifdef NETWORK
//	i = FindArg( "-rinvul" );
//	if (i > 0) {
//		int mins = atoi(Args[i+1]);
//		if (mins > 314)
//			mins = 314;
// 	control_invul_time = mins/5;
//	}
	control_invul_time = 0;
#endif

	i = FindArg( "-xcontrol" );
	if ( i > 0 )	{
		kconfig_init_external_controls( strtol(Args[i+1], NULL, 0), strtol(Args[i+2], NULL, 0) );
	}

	if (Inferno_verbose) printf( "\n%s\n\n", TXT_INITIALIZING_GRAPHICS);
	if ((t=gr_init( SM_ORIGINAL ))!=0)
		Error(TXT_CANT_INIT_GFX,t);
	// Load the palette stuff. Returns non-zero if error.
	mprintf( (0, "Going into graphics mode..." ));
	gr_set_mode(SM_320x200C);
	mprintf( (0, "\nInitializing palette system..." ));
   gr_use_palette_table( "PALETTE.256" );
	mprintf( (0, "\nInitializing font system..." ));
	gamefont_init();	// must load after palette data loaded.
	songs_play_song( SONG_TITLE, 1 );

	#ifndef RELEASE
	if ( !FindArg( "-notitles" ) ) 
	#endif
	{	//NOTE LINK TO ABOVE!
		show_title_screen( "iplogo1.pcx", 1 );
		show_title_screen( "logo.pcx", 1 );
	}

	{
		//grs_bitmap title_bm;
		int pcx_error;
		char filename[14];

		strcpy(filename, "descent.pcx");

		if ((pcx_error=pcx_read_bitmap( filename, &grd_curcanv->cv_bitmap, grd_curcanv->cv_bitmap.bm_type, title_pal ))==PCX_ERROR_NONE)	{
			vfx_set_palette_sub( title_pal );
			gr_palette_clear();
			//gr_bitmap( 0, 0, &title_bm );
			gr_palette_fade_in( title_pal, 32, 0 );
			//free(title_bm.bm_data);
		} else {
			gr_close();
			Error( "Couldn't load pcx file '%s', PCX load error: %s\n",filename, pcx_errormsg(pcx_error));
		}
	}

#ifdef EDITOR
	if ( !FindArg("-nobm") )
		bm_init_use_tbl();
	else
		bm_init();
#else
		bm_init();
#endif

	if ( FindArg( "-norun" ) )
		return(0);

	mprintf( (0, "\nInitializing 3d system..." ));
	g3_init();
	mprintf( (0, "\nInitializing texture caching system..." ));
	texmerge_init( 10 );		// 10 cache bitmaps
	mprintf( (0, "\nRunning game...\n" ));
	set_screen_mode(SCREEN_MENU);

	init_game();
	set_detail_level_parameters(Detail_level);

	Players[Player_num].callsign[0] = '\0';
	if (!Auto_demo) 	{
		key_flush();
		RegisterPlayer();		//get player's name
	}

	gr_palette_fade_out( title_pal, 32, 0 );

	//check for special stamped version
	if (registered_copy) {
		nm_messagebox("EVALUATION COPY",1,"Continue",
			"This special evaluation copy\n"
			"of DESCENT has been issued to:\n\n"
			"%s\n"
			"\n\n    NOT FOR DISTRIBUTION",
			name_copy);

		gr_palette_fade_out( gr_palette, 32, 0 );
	}

	//kconfig_load_all();

	Game_mode = GM_GAME_OVER;

	if (Auto_demo)	{
		newdemo_start_playback("DESCENT.DEM");		
		if (Newdemo_state == ND_STATE_PLAYBACK )
			Function_mode = FMODE_GAME;
	}

	build_mission_list(0);		// This also loads mission 0.

	while (Function_mode != FMODE_EXIT)
	{
		switch( Function_mode )	{
		case FMODE_MENU:
			if ( Auto_demo )	{
				newdemo_start_playback(NULL);		// Randomly pick a file
				if (Newdemo_state != ND_STATE_PLAYBACK)	
					Error("No demo files were found for autodemo mode!");
			} else {
				check_joystick_calibration();
				DoMenu();									 	
#ifdef EDITOR
				if ( Function_mode == FMODE_EDITOR )	{
					create_new_mine();
					SetPlayerFromCurseg();
				}
#endif
			}
			break;
		case FMODE_GAME:
			#ifdef EDITOR
				keyd_editor_mode = 0;
			#endif
			game();
			if ( Function_mode == FMODE_MENU )
				songs_play_song( SONG_TITLE, 1 );
			break;
		#ifdef EDITOR
		case FMODE_EDITOR:
			keyd_editor_mode = 1;
			editor();
			_harderr( (void *)descent_critical_error_handler );		// Reinstall game error handler
			if ( Function_mode == FMODE_GAME ) {
				Game_mode = GM_EDITOR;
				editor_reset_stuff_on_level();
				N_players = 1;
			}
			break;
		#endif
		default:
			Error("Invalid function mode %d",Function_mode);
		}
	}

	WriteConfigFile();

#ifndef ROCKWELL_CODE
	#ifndef RELEASE
	if (!FindArg( "-notitles" ))
	#endif
		//NOTE LINK TO ABOVE!!
	#ifndef EDITOR
		show_order_form();
	#endif
#endif

	#ifndef NDEBUG
	if ( FindArg( "-showmeminfo" ) )
//		show_mem_info = 1;		// Make memory statistics show
	#endif

	return(0);		//presumably successful exit
}


void check_joystick_calibration()	{
	int x1, y1, x2, y2, c;
	fix t1;

	if ( (Config_control_type!=CONTROL_JOYSTICK) &&
		  (Config_control_type!=CONTROL_FLIGHTSTICK_PRO) &&
		  (Config_control_type!=CONTROL_THRUSTMASTER_FCS) &&
		  (Config_control_type!=CONTROL_GRAVIS_GAMEPAD)
		) return;

	joy_get_pos( &x1, &y1 );

	t1 = timer_get_fixed_seconds();
	while( timer_get_fixed_seconds() < t1 + F1_0/100 )
		;

	joy_get_pos( &x2, &y2 );

	// If joystick hasn't moved...
	if ( (abs(x2-x1)<30) &&  (abs(y2-y1)<30) )	{
		if ( (abs(x1)>30) || (abs(x2)>30) ||  (abs(y1)>30) || (abs(y2)>30) )	{
			c = nm_messagebox( NULL, 2, TXT_CALIBRATE, TXT_SKIP, TXT_JOYSTICK_NOT_CEN );
			if ( c==0 )	{
				joydefs_calibrate();
			}
		}
	}

}

void show_order_form()
{
	int pcx_error;
	char title_pal[768];
	char	exit_screen[16];

	gr_set_current_canvas( NULL );
	gr_palette_clear();

	key_flush();		

#ifdef SHAREWARE
	strcpy(exit_screen, "order01.pcx");
#else
	#ifdef DEST_SAT
		strcpy(exit_screen, "order01.pcx");
	#else
		strcpy(exit_screen, "warning.pcx");
	#endif
#endif
	if ((pcx_error=pcx_read_bitmap( exit_screen, &grd_curcanv->cv_bitmap, grd_curcanv->cv_bitmap.bm_type, title_pal ))==PCX_ERROR_NONE) {
		vfx_set_palette_sub( title_pal );
		gr_palette_fade_in( title_pal, 32, 0 );
		{
			int done=0;
			fix time_out_value = timer_get_approx_seconds()+i2f(60*5);
			while(!done)	{
				if ( timer_get_approx_seconds() > time_out_value ) done = 1;
				if (key_inkey()) done = 1;
			}
		}
		gr_palette_fade_out( title_pal, 32, 0 );		
	}
	key_flush();		
}


#ifdef USE_CD

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dpmi.h"

typedef struct {
	char unit;
	ushort dev_offset;
	ushort dev_segment;
} dev_list;

typedef struct _Dev_Hdr {
	unsigned int dev_next;
	unsigned short dev_att;
	ushort dev_stat;
	ushort dev_int;
	char dev_name[8];
	short dev_resv;
	char dev_letr;
	char dev_units;
} dev_header;

int find_descent_cd()
{
	dpmi_real_regs rregs;
		
	// Get dos memory for call...
	dev_list * buf;
	dev_header *device;
	int num_drives, i;
	unsigned cdrive, cur_drive, cdrom_drive;

	memset(&rregs,0,sizeof(dpmi_real_regs));
	rregs.eax = 0x1500;
	rregs.ebx = 0;
	dpmi_real_int386x( 0x2f, &rregs );
	if ((rregs.ebx & 0xffff) == 0) {
		return -1;			// No cdrom
	}
	num_drives = rregs.ebx;

	buf = (dev_list *)dpmi_get_temp_low_buffer( sizeof(dev_list)*26 );	
	if (buf==NULL) {
		return -2;			// Error getting memory!
	}

	memset(&rregs,0,sizeof(dpmi_real_regs));
	rregs.es = DPMI_real_segment(buf);
	rregs.ebx = DPMI_real_offset(buf);
	rregs.eax = 0x1501;
	dpmi_real_int386x( 0x2f, &rregs );
	cdrom_drive = 0;
	_dos_getdrive(&cdrive);
	for (i = 0; i < num_drives; i++) {
		device = (dev_header *)((buf[i].dev_segment<<4)+ buf[i].dev_offset);
		_dos_setdrive(device->dev_letr,&cur_drive);
		_dos_getdrive(&cur_drive);
		if (cur_drive == device->dev_letr) {
			if (!chdir("\\descent")) {
				FILE * fp;
#ifdef DEST_SAT
				fp = fopen( "saturn.hog", "rb" );	
#else
				fp = fopen( "descent.hog", "rb" );	
#endif
				if ( fp )	{
					int write_failed = 1;
					fclose(fp);
					fp = fopen( "descent.tmp", "wb" );
					if ( fp )	{
						int temp = 0x15A90C23;
						if ( fwrite( &temp, sizeof(int), 1, fp )==1 )	{
							fp = fopen( "descent.tmp", "rb" );
							if ( fp )	{
								temp = 0;
								if ( fread( &temp, sizeof(int), 1, fp )==1 )	{
									if ( temp == 0x15A90C23 ) 
										write_failed = 0;
								}
								fclose(fp);
								unlink( "descent.tmp" );
							}
						}
						fclose(fp);
					}
					if ( write_failed )	{
						cdrom_drive = device->dev_letr;
						break;
					}
				}
			}
		}				
	}
	_dos_setdrive(cdrive,&cur_drive);
	return cdrom_drive;
}

#endif
