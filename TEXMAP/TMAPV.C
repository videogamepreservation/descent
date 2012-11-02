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
 * $Source: f:/miner/source/texmap/rcs/tmapv.c $
 * $Revision: 1.5 $
 * $Author: mike $
 * $Date: 1994/11/28 13:34:30 $
 * 
 * Vertical scanning in inner loop analogue to tmap.c
 * 
 * $Log: tmapv.c $
 * Revision 1.5  1994/11/28  13:34:30  mike
 * optimizations.
 * 
 * Revision 1.4  1994/05/24  17:31:01  mike
 * Vertical scanning texture mapper, hyperbolic in outer loop, linear in inner.
 * 
 * Revision 1.3  1994/05/24  11:02:54  mike
 * Fix bugs in vertically scanning texture mapper, hack in Do_vertical_scan.
 * 
 * Revision 1.2  1994/01/31  15:42:02  mike
 * Vertical scanning version of texture mapper.
 * 
 * Revision 1.1  1994/01/30  13:56:44  mike
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: tmapv.c 1.5 1994/11/28 13:34:30 mike Exp $";
#pragma on (unreferenced)

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

int	Is_sky_flag;

// ------------------------------------------------------------------------------------------
void compute_x_bounds(g3ds_tmap *t, int *vlt, int *vlb, int *vrt, int *vrb, int *left_x_ind)
{
	int	i;
	int	min_y,max_y;
	int	min_x_ind;
	int	original_vrt;
	fix	min_x,max_x;

	min_y = 9999;
	max_y = -9999;
	min_x = 9999;
	max_x = -9999;

	// Scan all vertices, set min_y_ind to vertex with smallest y coordinate.
	for (i=0; i<t->nv; i++) {
		if (f2i(t->verts[i].x2d) < min_x) {
			min_x = f2i(t->verts[i].x2d);
			min_x_ind = i;
			min_y = f2i(t->verts[i].y2d);
		} else if (f2i(t->verts[i].x2d) == min_x) {
			if (f2i(t->verts[i].y2d) < min_y) {
				min_x_ind = i;
				min_y = f2i(t->verts[i].y2d);
			}
		}
		if (f2i(t->verts[i].x2d) > max_x) {
			max_x = f2i(t->verts[i].x2d);
			*left_x_ind = i;
		}
	}

	// Set "vertex left top", etc. based on vertex with topmost y coordinate
	*vlt = min_x_ind;
	*vrt = *vlt;
	*vlb = succmod(*vlt,t->nv);
	*vrb = prevmod(*vrt,t->nv);

	// If right edge is vertical, then advance along polygon bound until it no longer is or until all
	// vertices have been examined.
	// (Left edge cannot be horizontal, because *vlt is set to leftmost point with highest y coordinate.)

	original_vrt = *vrt;

	while (f2i(t->verts[*vrt].x2d) == f2i(t->verts[*vrb].x2d)) {
		if (succmod(*vrt,t->nv) == original_vrt) {
			break;
		}
		*vrt = prevmod(*vrt,t->nv);
		*vrb = prevmod(*vrt,t->nv);
	}
}

// -------------------------------------------------------------------------------------
//	Render a texture map.
// Linear in outer loop, linear in inner loop.
//	Scan horizontally in outer loop, vertically in inner loop.
fix compute_dy_dx_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta x with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d - t->verts[top_vertex].x2d) + 1;
	if (dx)
		return (t->verts[bottom_vertex].y2d - t->verts[top_vertex].y2d) / dx;
	else
		return 0;

}

fix compute_dy_dx(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta y with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d); // + 1;
	if (dx)
		return (t->verts[bottom_vertex].y2d - t->verts[top_vertex].y2d) / dx;
	else
		return 0;

}

fix compute_du_dx(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta u with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d); // + 1;
	if (dx)
		return (fixmul(t->verts[bottom_vertex].u,t->verts[bottom_vertex].z) - fixmul(t->verts[top_vertex].u,t->verts[top_vertex].z)) / dx;
	else
		return 0;

}


fix compute_dv_dx(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta v with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d); // + 1;
	if (dx)
		return (fixmul(t->verts[bottom_vertex].v,t->verts[bottom_vertex].z) - fixmul(t->verts[top_vertex].v,t->verts[top_vertex].z)) / dx;
	else
		return 0;

}

fix compute_dz_dx(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta x with respect to y for the any edge
	// this is used in computing the x coordinate on the next row and in
	// scaling the add of Du_dx to the uleft and vleft coordinates.
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d); // + 1;
	if (dx)
		return (t->verts[bottom_vertex].z - t->verts[top_vertex].z) / dx;
	else
		return 0;

}

fix compute_dl_dx_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta l with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d) + 1;
	if (dx)
		return (t->verts[bottom_vertex].l - t->verts[top_vertex].l) / dx;
	else
		return 0;

}

fix compute_du_dx_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta u with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d) + 1;
	if (dx)
		return (t->verts[bottom_vertex].u - t->verts[top_vertex].u) / dx;
	else
		return 0;

}


fix compute_dv_dx_lin(g3ds_tmap *t, int top_vertex,int bottom_vertex)
{
	int	dx;

	// compute delta v with respect to y for any edge
	dx = f2i(t->verts[bottom_vertex].x2d) - f2i(t->verts[top_vertex].x2d) + 1;
	if (dx)
		return (t->verts[bottom_vertex].v - t->verts[top_vertex].v) / dx;
	else
		return 0;

}

// -------------------------------------------------------------------------------------
void tmap_scanline_lin_sky_v(grs_bitmap *srcb, int x, fix yleft, fix yright, fix uleft, fix uright, fix vleft, fix vright)
{
	fix	dy;

	dy = f2i(yright) - f2i(yleft);
	if ((dy < 0) || (yright < 0) || (yleft > yright))		// the (xleft > xright) term is not redundant with (dx < 0) because dx is computed using integers
		return;

	// setup to call assembler scanline renderer
	fx_u = uleft;
	fx_v = vleft;

	if (dy < f1_0) {
		fx_du_dx = uright-uleft;
		fx_dv_dx = vright-vleft;
	} else {
		fx_du_dx = fixdiv(uright - fx_u,dy);
		fx_dv_dx = fixdiv(vright - fx_v,dy);
	}

	fx_y = x;
	fx_xleft = yleft;
	fx_xright = yright;
	pixptr = srcb->bm_data;

	if (Is_sky_flag)
		asm_tmap_scanline_lin_sky_v();
	else
		asm_tmap_scanline_lin_v();

}

// -------------------------------------------------------------------------------------
void tmap_scanline_hyp_lin_v(grs_bitmap *srcb, int x, fix yleft, fix yright, fix uleft, fix uright, fix vleft, fix vright, fix zleft, fix zright)
{
//	fix	dy, recip_dy, z;
	int	dy;

	dy = f2i(yright) - f2i(yleft);
	if ((dy < 0) || (yright < 0) || (yleft > yright))		// the (xleft > xright) term is not redundant with (dx < 0) because dx is computed using integers
		return;

	// setup to call assembler scanline renderer
//	if (dy < FIX_RECIP_TABLE_SIZE)
//		recip_dy = fix_recip[dy];
//	else
//		recip_dy = F1_0/dy;

	uleft = fixdiv(uleft, zleft);
	uright = fixdiv(uright, zright);
	vleft = fixdiv(vleft, zleft);
	vright = fixdiv(vright, zright);

	if (dy > 0) {
		fx_du_dx = (uright - uleft)/dy;
		fx_dv_dx = (vright - vleft)/dy;
	} else {
		fx_du_dx = 0;
		fx_dv_dx = 0;
	}
//	fx_dz_dx = fixmul(zright - zleft,recip_dy);

	fx_u = uleft;
	fx_v = vleft;
	fx_z = zleft;
	fx_y = x;
	fx_xleft = yleft;
	fx_xright = yright;
	pixptr = srcb->bm_data;

	if (Is_sky_flag)
		asm_tmap_scanline_lin_sky_v();
	else
		asm_tmap_scanline_lin_v();

}


// -------------------------------------------------------------------------------------
void texture_map_hyp_lin_v(grs_bitmap *srcb, g3ds_tmap *t)
{
	int	vlt,vrt,vlb,vrb;	// vertex left top, vertex right top, vertex left bottom, vertex right bottom
	int	topy,boty,x;
	int	left_break,right_break;

	fix	dx_dy_left,dx_dy_right;
	fix	du_dy_left,du_dy_right;
	fix	dv_dy_left,dv_dy_right;
	fix	dz_dy_left,dz_dy_right;
	fix	zleft, zright;

	int	max_x_vertex;
	fix	yleft,yright,uleft,vleft,uright,vright;
	g3ds_vertex *v3d;

// mprintf(0, "width = %i, height = %i\n", srcb->bm_w, srcb->bm_h);
Is_sky_flag = 0;
if ((srcb->bm_w == 1024) && (srcb->bm_h == 128))
	Is_sky_flag = 1;

	//--now called from g3_start_frame-- init_interface_vars_to_assembler();

	v3d = t->verts;

	// Determine top and bottom y coords.
	compute_x_bounds(t,&vlt,&vlb,&vrt,&vrb,&max_x_vertex);

	// Set top and bottom (of entire texture map) y coordinates.
	topy = f2i(v3d[vlt].x2d);
	boty = f2i(v3d[max_x_vertex].x2d);

	// Set amount to change x coordinate for each advance to next scanline.
	dx_dy_left = compute_dy_dx(t,vlt,vlb);
	dx_dy_right = compute_dy_dx(t,vrt,vrb);

	du_dy_left = compute_du_dx(t,vlt,vlb);
	du_dy_right = compute_du_dx(t,vrt,vrb);

	dv_dy_left = compute_dv_dx(t,vlt,vlb);
	dv_dy_right = compute_dv_dx(t,vrt,vrb);

	dz_dy_left = compute_dz_dx(t,vlt,vlb);
	dz_dy_right = compute_dz_dx(t,vrt,vrb);

 	// Set initial values for x, u, v
	yleft = v3d[vlt].y2d;
	yright = v3d[vrt].y2d;

	zleft = v3d[vlt].z;
	zright = v3d[vrt].z;

	uleft = fixmul(v3d[vlt].u,zleft);
	uright = fixmul(v3d[vrt].u,zright);
	vleft = fixmul(v3d[vlt].v,zleft);
	vright = fixmul(v3d[vrt].v,zright);

	// scan all rows in texture map from top through first break.
	// @mk: Should we render the scanline for y==boty?  This violates Matt's spec.

	left_break = f2i(v3d[vlb].x2d);
	right_break = f2i(v3d[vrb].x2d);

	for (x = topy; x < boty; x++) {

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x,u,v
		if (x == left_break) {
			// Handle problem of double points.  Search until y coord is different.  Cannot get
			// hung in an infinite loop because we know there is a vertex with a lower y coordinate
			// because in the for loop, we don't scan all spanlines.
			while (x == f2i(v3d[vlb].x2d)) {
				vlt = vlb;
				vlb = succmod(vlb,t->nv);
			}
			left_break = f2i(v3d[vlb].x2d);

			dx_dy_left = compute_dy_dx(t,vlt,vlb);

			yleft = v3d[vlt].y2d;
			zleft = v3d[vlt].z;
			uleft = fixmul(v3d[vlt].u,zleft);
			vleft = fixmul(v3d[vlt].v,zleft);

			du_dy_left = compute_du_dx(t,vlt,vlb);
			dv_dy_left = compute_dv_dx(t,vlt,vlb);
			dz_dy_left = compute_dz_dx(t,vlt,vlb);

		}

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x.  Not necessary to set new values for u,v.
		if (x == right_break) {
			while (x == f2i(v3d[vrb].x2d)) {
				vrt = vrb;
				vrb = prevmod(vrb,t->nv);
			}
			right_break = f2i(v3d[vrb].x2d);

			dx_dy_right = compute_dy_dx(t,vrt,vrb);

			yright = v3d[vrt].y2d;
			zright = v3d[vrt].z;
			uright = fixmul(v3d[vrt].u,zright);
			vright = fixmul(v3d[vrt].v,zright);

			du_dy_right = compute_du_dx(t,vrt,vrb);
			dv_dy_right = compute_dv_dx(t,vrt,vrb);
			dz_dy_right = compute_dz_dx(t,vrt,vrb);

		}

		tmap_scanline_hyp_lin_v(srcb,x,yleft,yright,uleft,uright,vleft,vright, zleft, zright);

		uleft += du_dy_left;
		vleft += dv_dy_left;

		uright += du_dy_right;
		vright += dv_dy_right;

		yleft += dx_dy_left;
		yright += dx_dy_right;

		zleft += dz_dy_left;
		zright += dz_dy_right;
	}

	tmap_scanline_hyp_lin_v(srcb,x,yleft,yright,uleft,uright,vleft,vright, zleft, zright);
}


// -------------------------------------------------------------------------------------
void texture_map_lin_lin_sky_v(grs_bitmap *srcb, g3ds_tmap *t)
{
	int	vlt,vrt,vlb,vrb;	// vertex left top, vertex right top, vertex left bottom, vertex right bottom
	int	topy,boty,x;
	int	left_break,right_break;

	fix	dx_dy_left,dx_dy_right;
	fix	du_dy_left,du_dy_right;
	fix	dv_dy_left,dv_dy_right;

	int	max_x_vertex;
	fix	yleft,yright,uleft,vleft,uright,vright;
	g3ds_vertex *v3d;

// mprintf(0, "width = %i, height = %i\n", srcb->bm_w, srcb->bm_h);
Is_sky_flag = 0;
if ((srcb->bm_w == 1024) && (srcb->bm_h == 128))
	Is_sky_flag = 1;

	//--now called from g3_start_frame-- init_interface_vars_to_assembler();

	v3d = t->verts;

	// Determine top and bottom y coords.
	compute_x_bounds(t,&vlt,&vlb,&vrt,&vrb,&max_x_vertex);

	// Set top and bottom (of entire texture map) y coordinates.
	topy = f2i(v3d[vlt].x2d);
	boty = f2i(v3d[max_x_vertex].x2d);

	// Set amount to change x coordinate for each advance to next scanline.
	dx_dy_left = compute_dy_dx_lin(t,vlt,vlb);
	dx_dy_right = compute_dy_dx_lin(t,vrt,vrb);

	du_dy_left = compute_du_dx_lin(t,vlt,vlb);
	du_dy_right = compute_du_dx_lin(t,vrt,vrb);

	dv_dy_left = compute_dv_dx_lin(t,vlt,vlb);
	dv_dy_right = compute_dv_dx_lin(t,vrt,vrb);

 	// Set initial values for x, u, v
	yleft = v3d[vlt].y2d;
	yright = v3d[vrt].y2d;

	uleft = v3d[vlt].u;
	uright = v3d[vrt].u;
	vleft = v3d[vlt].v;
	vright = v3d[vrt].v;

	// scan all rows in texture map from top through first break.
	// @mk: Should we render the scanline for y==boty?  This violates Matt's spec.

	left_break = f2i(v3d[vlb].x2d);
	right_break = f2i(v3d[vrb].x2d);

	for (x = topy; x < boty; x++) {

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x,u,v
		if (x == left_break) {
			// Handle problem of double points.  Search until y coord is different.  Cannot get
			// hung in an infinite loop because we know there is a vertex with a lower y coordinate
			// because in the for loop, we don't scan all spanlines.
			while (x == f2i(v3d[vlb].x2d)) {
				vlt = vlb;
				vlb = succmod(vlb,t->nv);
			}
			left_break = f2i(v3d[vlb].x2d);

			dx_dy_left = compute_dy_dx_lin(t,vlt,vlb);

			yleft = v3d[vlt].y2d;
			uleft = v3d[vlt].u;
			vleft = v3d[vlt].v;

			du_dy_left = compute_du_dx_lin(t,vlt,vlb);
			dv_dy_left = compute_dv_dx_lin(t,vlt,vlb);
		}

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x.  Not necessary to set new values for u,v.
		if (x == right_break) {
			while (x == f2i(v3d[vrb].x2d)) {
				vrt = vrb;
				vrb = prevmod(vrb,t->nv);
			}
			right_break = f2i(v3d[vrb].x2d);

			dx_dy_right = compute_dy_dx_lin(t,vrt,vrb);

			yright = v3d[vrt].y2d;
			uright = v3d[vrt].u;
			vright = v3d[vrt].v;

			du_dy_right = compute_du_dx_lin(t,vrt,vrb);
			dv_dy_right = compute_dv_dx_lin(t,vrt,vrb);
		}

		tmap_scanline_lin_sky_v(srcb,x,yleft,yright,uleft,uright,vleft,vright);

		uleft += du_dy_left;
		vleft += dv_dy_left;

		uright += du_dy_right;
		vright += dv_dy_right;

		yleft += dx_dy_left;
		yright += dx_dy_right;

	}
	tmap_scanline_lin_sky_v(srcb,x,yleft,yright,uleft,uright,vleft,vright);
}


