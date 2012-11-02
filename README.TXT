Legal Stuff:


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

We make no warranties as to the usability or correctness of this code.

About the Source Code:

Included is almost all the source code to Descent 1, ver. 1.5.  We removed
all code to which we did not own the copyright.  This mainly involved low-
level sound and modem code.

Kevin Bentley, maintainer of the Descent Developer Resources and author of
Kahn, has agreed to support this code base through his website at
www.ladder.org/ddr.  If you have questions or comments about the code, please
go there.  Please do not contact Parallax Software, its employees or its
affiliates.

The following tools were used in the development of Descent:
Watcom C/C++, version 9.5
Microsoft Macro Assembler, version 6.1x
Opus Make, version 6.01

Have fun with the code!
Parallax Software Corporation
Jan. 12, 1998

Kevin Bentley's notes:

As included in this zip file, the source should compile and run with the 
following:

Watcom 10.6
Miscrosoft Macro Assembler 6.11
GNU (or compatible, such as Microsoft, or Borland) Make utility

To build the executable, you should be able to just run maked1.bat. 
Descentr.exe should be built in the main subdirectory.

When you unzip the archive, make sure you use the -d option to expand subdirectories, or it will not 
compile.

The compiled code will run fine with no sound, and no serial port functionality 
(This means no VR equipment that uses serial interfaces, and no modem or serial 
play games.)

If you port this to another platform, please drop me a note 
kevinb@stargatenetworks.com , I would be happy to post a copy of your port on 
the web site. *PLEASE* make an attempt to keep the existing code compilable (ie. 
use #IFDEFS)), so other people can benefit from your work.

To build the editor, change the CCFLAGS variable in all the makefiles to 
"CCFLAGS = /dNETWORK /dEDITOR" and rebuild all files. You will need the files 
in editdata.zip (included in this archive) to run the editor. Also, I was only 
able to get the editor to run using the -nobm command line arguement.


---

Interview with Matt Toschlog and Mike Kulas regarding the release of the
Descent source code:

Q: Why are you releasing the source code?

Mike: The main reason is we figured a lot of people aspiring to work in the
game industry would like to see production "quality" code.  There are two
reasons for this.  One, you can learn a lot by looking at working code.
Two, people will see that you can write a decent game without writing
beautiful code.

Matt: That's a good thing?

Mike: Not really, except that it might make people think writing production
quality code isn't that hard.

Matt: That's a good thing?

Mike: Well, not really, unless they learn that they have to focus on
designing a brilliant game, rather than writing brilliant code.

Matt: Ah, that's a good thing.

Mike: Yeah, I don't think I understood that until we started working on
Descent.  At first all I cared about was writing technically good code.

Matt: Then we ran out of money and all we cared about was finishing our game.

Mike: Right.  Our code got ugly, but our game got done.

Matt: Writing code is easy.  Finishing is hard.

Q: Is there any code you're particularly proud of?

Matt: No.

Mike: No.

Matt: I'm most ashamed of the vector intersection code.

Mike: Yeah, that's pretty bad.

Matt: Mike's AI code is the worst I've ever seen.

Mike: Only because you program with your eyes closed.

Matt: I'm proud we managed to finish without going bankrupt.

Mike: Yeah, finishing is all.  Who said that?

Q: Seriously, what should people look at?

Matt: I really don't know.  It's not like there's a lot of reusable code in
there.  Some low level stuff could probably be used.  The vector-matrix
library is probably fairly instructive.

Mike: The AI is an example of how not to write an AI system.

Matt: You're doing it the same way in FreeSpace, right?

Mike: Yeah.

Matt: The texture mapper is worth looking at, not that you'll be needing a
software texture mapper much longer.

Q: You're including the editor, right?

Mike: Yeah, people will be able to use our editor.  On the whole, I doubt
it's any better than DMB2.  And it doesn't run under Windows.

Q: What enhancements do you hope to see people make?

Matt: If an aftermarket sprouts for it, we'd be very happy.  People ask for
lots of little features that we just don't have the time to add.

Mike: People could probably roll in some of the D2 multiplayer features
without too much difficulty.

Matt: I don't think we can guess what people will do.  We had no idea how
much would be done with third party levels.

Q: The license states that people can't use the code for commercial gain.
What if some kid develops something and wants to recoup some of the cost
through shareware?

Mike: We're not opposed to that in principle.  They need to get written
permission from us, though.  And, it would have to be after the thing is
done so we know what we're permitting to be commercialized, if you want to
call it that.

Q: Any plans to release the D2 source code?

Matt: No definite plans.  Though, I guess I don't see why all our source
code wouldn't eventually get released.

Mike: Me, too.  It loses commercial value in just a few years.  And,
releasing it brings us closer to our customers, which is a very good thing
to do.

Q: Any final comments?

Mike: Yeah, have fun with the code.

