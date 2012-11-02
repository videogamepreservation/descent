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
/*
 * $Source: f:/miner/source/texmap/rcs/tmapsky.c $
 * $Revision: 1.4 $
 * $Author: mike $
 * $Date: 1994/11/28 13:34:33 $
 * 
 * Sky texture mapper for Sim City 2000 Drive Through
 * 
 * $Log: tmapsky.c $
 * Revision 1.4  1994/11/28  13:34:33  mike
 * optimizations.
 * 
 * Revision 1.3  1994/06/09  16:10:10  mike
 * Kill definition of SC2000.
 * 
 * Revision 1.2  1994/03/15  14:49:58  mike
 * *** empty log message ***
 * 
 * Revision 1.1  1994/03/15  10:30:53  mike
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: tmapsky.c 1.4 1994/11/28 13:34:33 mike Exp $";
#pragma on (unreferenced)

// This is evil and it's all Matt's fault, and it is only here so Assert will work, but that
// should work off ifdef, not if, so there!
#define	DEBUG_ON 1

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "mono.h"
#include "fix.h"
#include "3d.h"
#include "gr.h"
#include "error.h"

#include "texmap.h"
#include "texmapl.h"

#define F15_5 (F1_0*15 + F0_5)

int Do_sky_vertical = 0;

void tmap_scanline_lin_sky(grs_bitmap *srcb, int y, fix xleft, fix xright, fix uleft, fix uright, fix vleft, fix vright)
{
	fix	u,v;
	fix	dx;

	u = uleft;
	v = vleft;

	dx = xright - xleft;
	if (dx < 0)
		return;

	// setup to call assembler scanline renderer
	fx_u = uleft;
	fx_v = vleft;

	if (dx < f1_0) {
		fx_du_dx = uright-uleft;
		fx_dv_dx = vright-vleft;
	} else {
		fx_du_dx = fixdiv(uright - fx_u,dx);
		fx_dv_dx = fixdiv(vright - fx_v,dx);
	}

	fx_y = y;
	fx_xleft = xleft;
	fx_xright = xright;
	pixptr = srcb->bm_data;

	asm_tmap_scanline_lin_sky();

}

// -------------------------------------------------------------------------------------
//	Render a texture map.
// Linear in outer loop, linear in inner loop.
// -------------------------------------------------------------------------------------
void texture_map_lin_lin_sky(grs_bitmap *srcb, g3ds_tmap *t)
{
	int	vlt,vrt,vlb,vrb;	// vertex left top, vertex right top, vertex left bottom, vertex right bottom
	int	topy,boty,y;
	int	left_break,right_break;

	fix	dx_dy_left,dx_dy_right;
	fix	du_dy_left,du_dy_right;
	fix	dv_dy_left,dv_dy_right;

	int	max_y_vertex;
	fix	xleft,xright,uleft,vleft,uright,vright;
	g3ds_vertex *v3d;

if (Do_sky_vertical) {
	texture_map_lin_lin_sky_v(srcb, t);
	return;
}

	//--now called from g3_start_frame-- init_interface_vars_to_assembler();

	v3d = t->verts;

	// Determine top and bottom y coords.
	compute_y_bounds(t,&vlt,&vlb,&vrt,&vrb,&max_y_vertex);

	// Set top and bottom (of entire texture map) y coordinates.
	topy = f2i(v3d[vlt].y2d);
	boty = f2i(v3d[max_y_vertex].y2d);

	// Set amount to change x coordinate for each advance to next scanline.
	dx_dy_left = compute_dx_dy_lin(t,vlt,vlb);
	dx_dy_right = compute_dx_dy_lin(t,vrt,vrb);

	du_dy_left = compute_du_dy_lin(t,vlt,vlb);
	du_dy_right = compute_du_dy_lin(t,vrt,vrb);

	dv_dy_left = compute_dv_dy_lin(t,vlt,vlb);
	dv_dy_right = compute_dv_dy_lin(t,vrt,vrb);

 	// Set initial values for x, u, v
	xleft = v3d[vlt].x2d;
	xright = v3d[vrt].x2d;

	uleft = v3d[vlt].u;
	uright = v3d[vrt].u;
	vleft = v3d[vlt].v;
	vright = v3d[vrt].v;

	// scan all rows in texture map from top through first break.
	// @mk: Should we render the scanline for y==boty?  This violates Matt's spec.

	left_break = f2i(v3d[vlb].y2d);
	right_break = f2i(v3d[vrb].y2d);

	for (y = topy; y < boty; y++) {

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x,u,v
		if (y == left_break) {
			// Handle problem of double points.  Search until y coord is different.  Cannot get
			// hung in an infinite loop because we know there is a vertex with a lower y coordinate
			// because in the for loop, we don't scan all spanlines.
			while (y == f2i(v3d[vlb].y2d)) {
				vlt = vlb;
				vlb = prevmod(vlb,t->nv);
			}
			left_break = f2i(v3d[vlb].y2d);

			dx_dy_left = compute_dx_dy_lin(t,vlt,vlb);

			xleft = v3d[vlt].x2d;
			uleft = v3d[vlt].u;
			vleft = v3d[vlt].v;

			du_dy_left = compute_du_dy_lin(t,vlt,vlb);
			dv_dy_left = compute_dv_dy_lin(t,vlt,vlb);
		}

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x.  Not necessary to set new values for u,v.
		if (y == right_break) {
			while (y == f2i(v3d[vrb].y2d)) {
				vrt = vrb;
				vrb = succmod(vrb,t->nv);
			}
			right_break = f2i(v3d[vrb].y2d);

			dx_dy_right = compute_dx_dy_lin(t,vrt,vrb);

			xright = v3d[vrt].x2d;
			uright = v3d[vrt].u;
			vright = v3d[vrt].v;

			du_dy_right = compute_du_dy_lin(t,vrt,vrb);
			dv_dy_right = compute_dv_dy_lin(t,vrt,vrb);
		}

		tmap_scanline_lin_sky(srcb,y,xleft,xright,uleft,uright,vleft,vright);

		uleft += du_dy_left;
		vleft += dv_dy_left;

		uright += du_dy_right;
		vright += dv_dy_right;

		xleft += dx_dy_left;
		xright += dx_dy_right;

	}
	tmap_scanline_lin_sky(srcb,y,xleft,xright,uleft,uright,vleft,vright);
}


