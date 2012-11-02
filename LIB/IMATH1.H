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
/* imath.h */
/* Created 12/14/94 */
/* John Schultz */

#ifndef IMATH_H
#define IMATH_H

typedef long coord;
typedef unsigned long ulong;
typedef struct {coord sin,cos;} sinCos;

#define FBITS 14
#define SHR(n) ((n)>>FBITS)
#define SHL(n) ((n)<<FBITS)
#define ONE (1<<FBITS)
#define FONE ((float)ONE)
#define TOFIXED(n) (long)((n)*ONE)
#define TOFLOAT(n) ((float)(n)/FONE)

#ifdef __WATCOMC__
#define M_PI 3.14159265358979323846264338327950288
#endif

#define M_TWOPI (M_PI*2)

#define RAD090DEG TOFIXED(M_PI/2)
#define RAD180DEG TOFIXED(M_PI)
#define RAD270DEG TOFIXED(M_PI*3/2)
#define RAD360DEG TOFIXED(M_PI*2)

#define DEG090 TOFIXED(90.)
#define DEG180 TOFIXED(180.)
#define DEG270 TOFIXED(270.)
#define DEG360 TOFIXED(360.)

#define TORADIANS(a) MUL(a,TOFIXED(M_PI/180.))
#define TODEGREES(a) MUL(a,TOFIXED(180./M_PI))

// The 14 below is FBITS
long mul64(long a,long b);
#pragma aux mul64 =      \
	"imul   edx"         \
	"shrd   eax,edx,14"  \
	"adc    eax,0"       \
	parm [eax] [edx]     \
	modify [eax edx]     \
	value [eax];

long div64(long a,long b);
#pragma aux div64 =      \
	"push   ebx"         \
	"mov    ebx,edx"     \
	"cdq"                \
	"shld   edx,eax,14"  \
	"shl    eax,14"      \
	"idiv   ebx"         \
	"pop    ebx"         \
	parm [eax] [edx]     \
	modify [eax edx]     \
	value [eax];

long muldiv64(long a,long b,long c);
#pragma aux muldiv64 =     \
	"imul    edx"          \
	"idiv    ebx"          \
	parm [eax] [edx] [ebx] \
	modify [eax edx ebx]   \
	value [eax];

#define MUL(a,b) mul64(a,b)
#define DIV(a,b) div64(a,b)
#define MULDIV(a,b,c) muldiv64(a,b,c)

#define abs(x) ((x) < 0 ? -(x) : (x))
#define sgn(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

#define CLAMPMIN(n,min) if ((n) < (min)) (n) = (min);
#define CLAMPMAX(n,max) if ((n) > (max)) (n) = (max);
#define CLAMPMINMAX(n,min,max) CLAMPMIN(n,min) else CLAMPMAX(n,max)

ulong isqrt(ulong n);
coord getsin(coord radians);
coord getcos(coord radians);
coord getatan(coord sine);
coord getatan2(coord sine,coord cosine);
void sincos(sinCos * sc,coord radians);

#endif

/* imath.h */

