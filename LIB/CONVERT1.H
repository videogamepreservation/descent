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
/* Convert.h */
/* Converts raw tracker data to orientation information */
/* Created 11/17/94 */
/* John Schultz */

#ifndef VIO_CONVERT_H
#define VIO_CONVERT_H

void rawToEulerInt(long rawMagneticX,long rawMagneticY,long rawMagneticZ,
                   long rawGravimetricX,long rawGravimetricY,
                   long * eulerYaw,long * eulerPitch,long * eulerRoll);
                    
#endif

/* convert.h */
/* tcomp.h */
/* Tilt sensor non-linear compensation */
/* Created 1/2/95 */
/* John Schultz */

#ifndef TCOMP_H
#define TCOMP_H

float compAngle(float degrees);

#endif

/* tcomp.h */

