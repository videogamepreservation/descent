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
/* test.c */

#include <stdio.h>
#include <math.h>
#include "imath1.h"
#include "vector1.h"
#include "convert1.h"
#include "tracker.h"
#include "tcal.h"

void main(int argc,char ** argv) {
TrackerData td;
TrackerStatus ts;
  
  ts = initTracker(&td,TP_COM1,9600,timerSecs(2));
  printf("%s.\n",trackerInfo(ts));
  if (ts != TS_OK) {
    closeTracker(&td);
    exit(0);
  } // if

  ts = sendTrackerCMD(&td,"!M1,P,B\r",timerSecs(2));
  if (ts != TS_OK) {
    printf("%s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if

#if 0
#if 0
  ts = readTrackerOffsets(&td);
  if (ts != TS_OK) {
    printf("%s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if

  ts = readTrackerGains(&td);
  if (ts != TS_OK) {
    printf("%s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if

  ts = readTrackerFilters(&td);
  if (ts != TS_OK) {
    printf("%s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if

  printf("Offsets: x %5d y %5d z %5d p %5d r %5d\n",
         td.magOff.x,td.magOff.y,td.magOff.z,td.grvOff.x,td.grvOff.y);

  printf("Gains:   x %5d y %5d z %5d p %5d r %5d\n",
         td.magGain.x,td.magGain.y,td.magGain.z,td.grvGain.x,td.grvGain.y);

  printf("Filters: x %5d y %5d z %5d p %5d r %5d\n",
         td.magFilt.x,td.magFilt.y,td.magFilt.z,td.grvFilt.x,td.grvFilt.y);

#else
  ts = readTrackerAllVars(&td);
  if (ts != TS_OK) {
    printf("%s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if
  printf("1 %s\n %s\n %s\n",getOffsetsStr(&td),getGainsStr(&td),getFiltersStr(&td));

#if 1
  td.magOff.x = atoi(argv[1]);
  ts = writeTrackerOffset(&td,TA_X);
  if (ts != TS_OK) {
    printf("Write: %s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if
#endif  

  ts = readTrackerAllVars(&td);
  if (ts != TS_OK) {
    printf("%s.\n",trackerInfo(ts));
    closeTracker(&td);
    exit(0);
  } // if

  printf("2 %s\n %s\n %s\n",getOffsetsStr(&td),getGainsStr(&td),getFiltersStr(&td));

#endif


  printf("Press a key"); fflush(stdout);

  if (getch() == 'q') {
    closeTracker(&td);
    exit(0);
  } // if
#endif

  requestTrackerData(&td);
  while (1) {
    float y,p,p2,r;
    if (kbhit() && getch() == 'q') break;
//delayTime(10);    
    ts = readTracker(&td,timerSecs(1));
    if (ts != TS_OK) {
      printf("\n%s.\n",trackerInfo(ts));
//      closeTracker(&td);
//      exit(0);
    } // if
    requestTrackerData(&td);

    y = TOFLOAT(td.euler.y);
    p = TOFLOAT(td.euler.x);
    r = TOFLOAT(td.euler.z);
    
    p2 = compAngle(p);
    
//    printf("x %5d y %5d z %5d y %4.2f p %4.2f p2 %4.2f r %4.2f\n",
//      td.magnetic.x,td.magnetic.y,td.magnetic.z,y,p,p2,r);
    printf("x %6d y %6d z %6d y %6.2f p %6.2f r %6.2f\n",
      td.magnetic.x,td.magnetic.y,td.magnetic.z,y,p,r);
  
  
  } // while

  closeTracker(&td);

} // main

/* test.c */

