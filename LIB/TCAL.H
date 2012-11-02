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
/* tcal.h */
/* Created 12/16/94 */
/* John Schultz */

#ifndef TCAL_H
#define TCAL_H

typedef enum {TA_X,TA_Y,TA_Z,TA_P,TA_R} TrackerAxis;

TrackerStatus readTrackerGain(TrackerData * td,TrackerAxis ta);

TrackerStatus writeTrackerGain(TrackerData * td,TrackerAxis ta);

TrackerStatus readTrackerOffset(TrackerData * td,TrackerAxis ta);

TrackerStatus writeTrackerOffset(TrackerData * td,TrackerAxis ta);

TrackerStatus readTrackerOffsets(TrackerData * td);

TrackerStatus writeTrackerOffsets(TrackerData * td);

TrackerStatus readTrackerGains(TrackerData * td);

TrackerStatus writeTrackerGains(TrackerData * td);

TrackerStatus readTrackerMagFilts(TrackerData * td);

TrackerStatus writeTrackerMagFilts(TrackerData * td);

TrackerStatus readTrackerGrvFilts(TrackerData * td);

TrackerStatus writeTrackerGrvFilts(TrackerData * td);

TrackerStatus readTrackerFilters(TrackerData * td);

TrackerStatus writeTrackerFilters(TrackerData * td);

TrackerStatus readTrackerAllVars(TrackerData * td);

TrackerStatus writeTrackerAllVars(TrackerData * td);

char * getOffsetsStr(TrackerData * td);

char * getGainsStr(TrackerData * td);

char * getFiltersStr(TrackerData * td);

char * getAllVarsStr(TrackerData * td);

#endif

/* tcal.h */

