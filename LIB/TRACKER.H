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
/* tracker.h */
/* Created 12/12/94 */
#ifndef TRACKER_H
#define TRACKER_H

typedef enum {TP_COM1,TP_COM2,TP_COM3,TP_COM4} TrackerPort;
typedef enum {
  TS_OK,TS_NO_SERIAL,TS_NO_TRACKER,TS_TIMEOUT,
  TS_CHECKSUM,TS_HEADER,TS_WRITEVERIFY,TS_INVALID,TS_CMD_ERROR
} TrackerStatus;

typedef struct {
  void * sd;  // Serial Data
  vec3 magnetic;
  vec2 gravimetric;
  vec3 euler;
  vec3 magOff;
  vec3 magGain;
  vec2 grvOff;
  vec2 grvGain;
  long magFilts;
  long grvFilts;
} TrackerData;

extern long debugTrackerIO;

TrackerStatus initTracker(TrackerData * td,TrackerPort port,long comRate,long timeout);
void closeTracker(TrackerData * td);
TrackerStatus readPacket(TrackerData * td,char * data,long size,long timeout);
TrackerStatus readTracker(TrackerData * td,long timeout);
TrackerStatus readTrackerEuler(TrackerData * td,long timeout);
void writeSerString(TrackerData * td,char * s);
void requestTrackerData(TrackerData * td);
char * trackerInfo(TrackerStatus ts);

TrackerStatus resetTracker(TrackerData * td,long timeout);
TrackerStatus sendTrackerCMD(TrackerData * td,char * cmd,long timeout);
TrackerStatus getTrackerResponse(TrackerData * td,long timeout);

#endif

/* tracker.h */
