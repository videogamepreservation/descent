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
//	Driver program for silly gauge plotter.

#include <stdio.h>
#include <stdlib.h>

#include "mono.h"
#include "key.h"
#include "gr.h"

#define NUM_LINES 10

typedef struct line {
	float	x1,y1,x2,y2;
	int	color;
} line;

float	X1vec,Y1vec,X2vec,Y2vec;
float	X1acc,Y1acc,X2acc,Y2acc;
line	Line;
int	Line_index;

// ---------------------------------------------------------------------------------
// Return a random number in the range lo..hi-1
int rrand(int lo,int hi)
{

	return (rand() * (hi-lo)) / RAND_MAX + lo;

}

// ---------------------------------------------------------------------------------
void create_random_line(void)
{
	line	*lp;

	lp = &Line;

	lp->x1 = rrand(0,grd_curcanv->cv_bitmap.bm_w);
	lp->y1 = rrand(0,grd_curcanv->cv_bitmap.bm_h);

	lp->x2 = rrand(0,grd_curcanv->cv_bitmap.bm_w);
	lp->y2 = rrand(0,grd_curcanv->cv_bitmap.bm_h);

	lp->color = 15;
}

// ---------------------------------------------------------------------------------
void gauge_init(void)
{
	line	*lp;

	Line_index = 0;
	lp = &Line;

	create_random_line();
	X1vec = 0.1;
	Y1vec = 0.2;
	X2vec = 0.3;
	Y1vec = 0.4;

	X1acc = 0.003;
	Y1acc = 0.004;
	X2acc = 0.005;
	Y2acc = 0.003;

	gr_setcolor(lp->color);
	gr_line(fl2f(lp->x1),fl2f(lp->y1),fl2f(lp->x2),fl2f(lp->y2));

	Line_index++;
}

// ---------------------------------------------------------------------------------
void gauge_frame(void)
{
	float	x1,y1,x2,y2;

	line	*clp;
	int	max_x,max_y;

	// Create new line at Line_index.
	clp = &Line;

	x1 = clp->x1 + X1vec;
	y1 = clp->y1 + Y1vec;

	x2 = clp->x2 + X2vec;
	y2 = clp->y2 + Y2vec;

	X1vec += X1acc;
	Y1vec += Y1acc;

	X2vec += X2acc;
	Y2vec += Y2acc;

	max_x = grd_curcanv->cv_bitmap.bm_w;
	max_y = grd_curcanv->cv_bitmap.bm_h;

	if (x1 > max_x) {		X1vec = 0;		x1 = clp->x1;	X1acc = -X1acc;	}

	if (y1 > max_y) {		Y1vec = 0;		y1 = clp->y1;	Y1acc = -Y1acc;	}

	if (x2 > max_x) {		X2vec = 0;		x2 = clp->x2;	X2acc = -X2acc;	}

	if (y2 > max_y) {		Y2vec = 0;		y2 = clp->y2;	Y2acc = -Y2acc;	}

	// now check min
	if (x1 < 0) {		X1vec = 0;		x1 = clp->x1;	X1acc = -X1acc;	}

	if (y1 < 0) {		Y1vec = 0;		y1 = clp->y1;	Y1acc = -Y1acc;	}

	if (x2 < 0) {		X2vec = 0;		x2 = clp->x2;	X2acc = -X2acc;	}

	if (y2 < 0) {		Y2vec = 0;		y2 = clp->y2;	Y2acc = -Y2acc;	}


	clp->x1 = x1;
	clp->y1 = y1;
	clp->x2 = x2;
	clp->y2 = y2;
	clp->color = (clp->color + 1) % 256;

	gr_setcolor(clp->color);
	gr_line(fl2f(x1),fl2f(y1),fl2f(x2),fl2f(y2));
	Line_index++;
	if (Line_index >= NUM_LINES)
		Line_index = 0;

}

// ---------------------------------------------------------------------------------
// Do a gauge in the current canvas
void gauge_test(void)
{
	int	i;

	gauge_init();

	for (i=0; i<10000; i++)
		gauge_frame();
}


