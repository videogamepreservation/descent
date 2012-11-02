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
 * $Source: f:/miner/source/texmap/rcs/tmapz.c $
 * $Revision: 1.3 $
 * $Author: mike $
 * $Date: 1994/11/28 13:34:26 $
 * 
 * .
 * 
 * $Log: tmapz.c $
 * Revision 1.3  1994/11/28  13:34:26  mike
 * optimizations.
 * 
 * Revision 1.2  1994/07/08  17:43:13  john
 * Added flat-shaded-zbuffered polygon.
 * 
 * Revision 1.1  1994/07/08  10:45:13  john
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: tmapz.c 1.3 1994/11/28 13:34:26 mike Exp $";
#pragma on (unreferenced)


#include <math.h>
// #include <graph.h>
#include <limits.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

// #include "hack3df.h"
#include "fix.h"
#include "mono.h"
#include "gr.h"
#include "grdef.h"
// #include "ui.h"
#include "texmap.h"
#include "texmapl.h"
#include "error.h"

//#include "tmapext.h"

extern fix * tmap_z_buffer;
extern ubyte tmap_flat_color_z;
extern void asm_tmap_scanline_flat_z();
extern fix compute_dz_dy(g3ds_tmap *t, int top_vertex,int bottom_vertex);

// Temporary texture map, interface from Matt's 3d system to Mike's texture mapper.
extern g3ds_tmap Tmap1;

// -------------------------------------------------------------------------------------
//	Texture map current scanline.
//	Uses globals Du_dx and Dv_dx to incrementally compute u,v coordinates
// -------------------------------------------------------------------------------------
void tmap_scanline_flat_z(int y, fix xleft, fix xright, fix zleft, fix zright )
{
	fix	dx,recip_dx;

	dx = f2i(xright) - f2i(xleft);

	if ((dx < 0) || (xright < 0) || (xleft > xright))		// the (xleft > xright) term is not redundant with (dx < 0) because dx is computed using integers
		return;

	if (dx < FIX_RECIP_TABLE_SIZE)
		recip_dx = fix_recip[dx];
	else
		recip_dx = F1_0/dx;

	fx_y = y << 16;
	fx_z = zleft;
	fx_dz_dx = fixmul(zright - zleft,recip_dx);
	fx_xleft = xleft;
	fx_xright = xright;
	
	asm_tmap_scanline_flat_z();

}

//	-----------------------------------------------------------------------------------------
//	This is the gr_upoly-like interface to the texture mapper which uses texture-mapper compatible
//	(ie, avoids cracking) edge/delta computation.

void texture_map_flat_z(g3ds_tmap *t, int color )
{
	int	vlt,vrt,vlb,vrb;	// vertex left top, vertex right top, vertex left bottom, vertex right bottom
	int	topy,boty,y;
	fix	dx_dy_left,dx_dy_right;
	fix	dz_dy_left,dz_dy_right;
	int	max_y_vertex;
	fix	xleft,xright;
	fix	zleft,zright;
	g3ds_vertex *v3d;

	v3d = t->verts;

	tmap_flat_color_z = color;

	// Determine top and bottom y coords.
	compute_y_bounds(t,&vlt,&vlb,&vrt,&vrb,&max_y_vertex);

	// Set top and bottom (of entire texture map) y coordinates.
	topy = f2i(v3d[vlt].y2d);
	boty = f2i(v3d[max_y_vertex].y2d);

	// Set amount to change x coordinate for each advance to next scanline.
	dx_dy_left = compute_dx_dy_lin(t,vlt,vlb);
	dx_dy_right = compute_dx_dy_lin(t,vrt,vrb);

	dz_dy_left = compute_dz_dy(t,vlt,vlb);
	dz_dy_right = compute_dz_dy(t,vrt,vrb);

 	// Set initial values for x, u, v
	xleft = v3d[vlt].x2d;
	xright = v3d[vrt].x2d;

	zleft = v3d[vlt].z;
	zright = v3d[vrt].z;

	// scan all rows in texture map from top through first break.
	// @mk: Should we render the scanline for y==boty?  This violates Matt's spec.

	for (y = topy; y < boty; y++) {

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x,u,v
		if (y == f2i(v3d[vlb].y2d)) {
			// Handle problem of double points.  Search until y coord is different.  Cannot get
			// hung in an infinite loop because we know there is a vertex with a lower y coordinate
			// because in the for loop, we don't scan all spanlines.
			while (y == f2i(v3d[vlb].y2d)) {
				vlt = vlb;
				vlb = prevmod(vlb,t->nv);
			}
			dx_dy_left = compute_dx_dy_lin(t,vlt,vlb);

			xleft = v3d[vlt].x2d;
			zleft = v3d[vlt].z;

			dz_dy_left = compute_dz_dy(t,vlt,vlb);

		}

		// See if we have reached the end of the current left edge, and if so, set
		// new values for dx_dy and x.  Not necessary to set new values for u,v.
		if (y == f2i(v3d[vrb].y2d)) {
			while (y == f2i(v3d[vrb].y2d)) {
				vrt = vrb;
				vrb = succmod(vrb,t->nv);
			}
			dx_dy_right = compute_dx_dy_lin(t,vrt,vrb);

			xright = v3d[vrt].x2d;
			zright = v3d[vrt].z;

			dz_dy_right = compute_dz_dy(t,vrt,vrb);
		}

		tmap_scanline_flat_z(y, xleft, xright, zleft, zright );

		xleft += dx_dy_left;
		xright += dx_dy_right;

		zleft += dz_dy_left;
		zright += dz_dy_right;

	}
	tmap_scanline_flat_z(y, xleft, xright, zleft, zright );
}

// -------------------------------------------------------------------------------------
// Interface from Matt's data structures to Mike's texture mapper.
// -------------------------------------------------------------------------------------
void draw_tmap_z(grs_bitmap *bp,int nverts,g3s_point **vertbuf)
{
	int	i;

	//	These variables are used in system which renders texture maps which lie on one scanline as a line.
	fix	div_numerator;

	Assert(nverts <= MAX_TMAP_VERTS);

	bp = NULL;
	
	if (tmap_z_buffer==NULL) return;

	//--now called from g3_start_frame-- init_interface_vars_to_assembler();

	// Setup texture map in Tmap1
	Tmap1.nv = nverts;						// Initialize number of vertices

	div_numerator = f1_0*3;

	for (i=0; i<nverts; i++) {
		g3ds_vertex	*tvp = &Tmap1.verts[i];
		g3s_point	*vp = vertbuf[i];

		tvp->x2d = vp->p3_sx;
		tvp->y2d = vp->p3_sy;
		tvp->z = vp->z;

		//	Check for overflow on fixdiv.  Will overflow on vp->z <= 20.  Allow only as low as 24.
		//if (vp->z < 24) {
		//	vp->z = 24;
		//	// Int3();		// we would overflow if we divided!
		//}
		//tvp->z = fixdiv(div_numerator,vp->z);

	}

	texture_map_flat_z( &Tmap1, COLOR );
	
}
