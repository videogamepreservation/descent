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
/* vector.h */
/* Created 12/14/94 */
/* John Schultz */

#ifndef VECTOR_H
#define VECTOR_H

typedef struct {coord x,y;} vec2;
typedef struct {coord x,y,z;} vec3;
typedef struct {coord x,y,z,w;} vec4;
typedef struct {vec3 x,y,z;} matrix33;
typedef struct {matrix33 m; vec3 w;} matrix34;
typedef struct {vec4 x,y,z,w;} matrix44;

#define SETVEC(v,a,b,c) (v).x=a;(v).y=b;(v).z=c
#define VZERO(a) (a).x=(a).y=(a).z=0
#define NEGVEC(a)    (a).x=-(a).x;(a).y=-(a).y;(a).z=-(a).z
#define NEGVECN(a,b) (a).x=-(b).x;(a).y=-(b).y;(a).z=-(b).z

#define ADDVEC(a,b,c) (a).x=(b).x+(c).x;(a).y=(b).y+(c).y;(a).z=(b).z+(c).z
#define INCVEC(a,b) (a).x+=(b).x;(a).y+=(b).y;(a).z+=(b).z;
#define SUBVEC(a,b,c) (a).x=(b).x-(c).x;(a).y=(b).y-(c).y;(a).z=(b).z-(c).z
#define DECVEC(a,b) (a).x-=(b).x;(a).y-=(b).y;(a).z-=(b).z
#define DOT2(a,b) (MUL((a).x,(b).x)+MUL((a).y,(b).y))
#define DOT3(a,b) (MUL((a).x,(b).x)+MUL((a).y,(b).y)+MUL((a).z,(b).z))
#define DOT4(a,b) (MUL((a).x,(b).x)+MUL((a).y,(b).y)+MUL((a).z,(b).z)+MUL((a).w,(b).w))

coord normalize2(vec2 * v);

coord normalize3(vec3 * v);

//*** Matrix/matrix, matrix/vector routines ***

void matrix33unitize(matrix33 * m);

void rotate33(vec3 * out,matrix33 * m,vec3 * in);

void rotate33rev(vec3 * out,matrix33 * m,vec3 * in);

void rotate34(vec3 * out,matrix34 * m,vec3 * in);

void rotate34rev(vec3 * out,matrix34 * m,vec3 * in);

void matrixMul33(matrix33 * out,matrix33 * a,matrix33 * b);

void matrixMul34(matrix34 * out,matrix34 * a,matrix34 * b);

void rotate44(vec4 * out,matrix44 * m,vec4 * in);

void matrixMul44(matrix44 * out,matrix44 * a,matrix44 * b);

void matrix33Axis(matrix33 * m,vec3 * axis,coord angle);

#endif

/* vector.h */
