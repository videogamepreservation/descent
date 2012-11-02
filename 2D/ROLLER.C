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
 * $Source: f:/miner/source/2d/rcs/roller.c $
 * $Revision: 1.15 $
 * $Author: john $
 * $Date: 1994/10/26 23:55:54 $
 * 
 * bitmap scaler, roller
 * 
 * $Log: roller.c $
 * Revision 1.15  1994/10/26  23:55:54  john
 * Took out roller; Took out inverse table.
 * 
 * Revision 1.14  1994/05/06  12:49:48  john
 * Added supertransparency; neatend things up; took out warnings.
 * 
 * Revision 1.13  1994/02/04  13:21:38  matt
 * Took out test code
 * 
 * Revision 1.12  1994/02/01  17:25:31  john
 * Made transparency color be 255 all the time.
 * 
 * Revision 1.11  1993/12/08  16:40:47  mike
 * Prevent blowup bug when xright < xleft.
 * 
 * Revision 1.10  1993/12/07  12:32:19  john
 * moved bmd_palette to gr_palette
 * 
 * Revision 1.9  1993/12/03  18:41:10  unknown
 * Put in check for trying to render_scanline wider than compiled code.
 * 
 * Revision 1.8  1993/12/03  15:13:49  mike
 * Fix transparency problem by moving setting of Transparency_color.
 * 
 * Revision 1.7  1993/12/03  12:05:13  mike
 * Make compiled code bitmaps clip.
 * 
 * Revision 1.6  1993/12/02  18:40:38  mike
 * Suppress "Too many compiled code pointers" Error.
 * 
 * Revision 1.5  1993/11/28  12:07:35  mike
 * Fix bugs in compiled code roller
 * 
 * Revision 1.4  1993/11/23  13:03:17  mike
 * Add compiled code roller.
 * 
 * Revision 1.3  1993/11/18  14:22:44  mike
 * fix bugs
 * 
 * Revision 1.2  1993/11/17  19:01:10  mike
 * Add roller.
 * 
 * Revision 1.1  1993/11/16  09:36:51  mike
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: roller.c 1.15 1994/10/26 23:55:54 john Exp $";
#pragma on (unreferenced)


#define COMPILED_CODE 1
#define TRANSPARENCY_COLOR 255

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

//#define USE_ROLLER 1

#ifdef USE_ROLLER

typedef struct xyuv {
	fix	x,y,u,v;
} xyuv;

extern grs_bitmap ** Texture;  // Array of textmaps

int	Compiled_code_width;

//int Transparency_color = TRANSPARENCY_COLOR;

// --------------------------------------------------------------------------------------
//	Create 4 xyuv points at array vb4, extracting data from 3 g3s_point points in vb.
//	Create 4th point using formula (p3 = p0 + p2 - p1)
void create_4_points(grs_point *vb, xyuv *vb4, int width, int height)
{
	int	i;

	for (i=0; i<3; i++) {
		vb4[i].x = vb[i].x;
		vb4[i].y = vb[i].y;
	}

	vb4[3].x = vb[0].x + vb[2].x - vb[1].x;
	vb4[3].y = vb[0].y + vb[2].y - vb[1].y;
	
	//	Set u,v coordinates.
	vb4[0].u = 0;
	vb4[0].v = 0;

	vb4[1].u = width*F1_0 - 1;
	vb4[1].v = 0;

	vb4[2].u = width*F1_0 - 1;
	vb4[2].v = height*F1_0 - 1;

	vb4[3].u = 0;
	vb4[3].v = height*F1_0 - 1;
}


// --------------------------------------------------------------------------------------
//	Compute deltas given 4 points.  Actually, only the first three are needed.
//	For du_dx, dv_dx, determine whether p1 - p0 or p2 - p1 has larger absolute value, and
//	compute deltas from those two points.
//	Do similarly for du_dy, dv_dy.
//	If, in either x or y, the maximum has absolute value of 0, then the bitmap is
//	degenerate.  Delta values of 0 are returned in those cases.
void compute_deltas(xyuv *vb, int *min_y, int *max_y, int *lbind, int *rbind,
	fix *du_dx, fix *dv_dx, fix *xleft, fix *uleft, fix *vleft, fix *xright,
	fix *dx_dy_left, fix *du_dy_left, fix *dv_dy_left, fix *dx_dy_right, fix *du_dy_right, fix *dv_dy_right, int *min_x, int *max_x)
{
	int	i;
//--	fix	denom;
	int	min_y_ind,min_x_ind;
	int	left_break_ind,right_break_ind;
	float	fldenom;

	fldenom = f2fl(vb[1].x - vb[0].x) * f2fl(vb[1].y - vb[2].y) - f2fl(vb[1].y - vb[0].y) * f2fl(vb[1].x - vb[2].x);
	*du_dx = fl2f(( f2fl(vb[1].u - vb[0].u) * f2fl(vb[1].y - vb[2].y) - f2fl(vb[1].y - vb[0].y) * f2fl(vb[1].u - vb[2].u)) / fldenom);
	*dv_dx = fl2f(( f2fl(vb[1].v - vb[0].v) * f2fl(vb[1].y - vb[2].y) - f2fl(vb[1].y - vb[0].y) * f2fl(vb[1].v - vb[2].v)) / fldenom);


//--	denom = fixmul(vb[1].x - vb[0].x , vb[1].y - vb[2].y) - fixmul(vb[1].y - vb[0].y , vb[1].x - vb[2].x);
//--
//--	//	If denom is very small, we risk overflow
//--	if (abs(denom) < F1_0/2) {
//--		mprintf(0,"Warning -- denom is nearly 0, bashing to 1.0.\n");
//--		Int3();
//--		denom = F1_0;
//--	}
//--
//--	*du_dx = fixdiv( fixmul(vb[1].u - vb[0].u , vb[1].y - vb[2].y) - fixmul(vb[1].y - vb[0].y , vb[1].u - vb[2].u), denom );
//--	*dv_dx = fixdiv( fixmul(vb[1].v - vb[0].v , vb[1].y - vb[2].y) - fixmul(vb[1].y - vb[0].y , vb[1].v - vb[2].v), denom );

	//	Compute vertex containing minimum x, y, set min_x, max_x, min_y, max_y
	*min_y =   10000;
	*min_x =   10000;
	for (i=0; i<4; i++) {
		if (f2i(vb[i].y) < *min_y) {
			*min_y = f2i(vb[i].y);
			min_y_ind = i;
		}

		if (f2i(vb[i].x) < *min_x) {
			*min_x = f2i(vb[i].x);
			min_x_ind = i;
		}
	}

	*max_y = f2i(vb[(min_y_ind + 2) % 4].y);
	*max_x = f2i(vb[(min_x_ind + 2) % 4].x);
	*xleft = vb[min_y_ind].x;
	*uleft = vb[min_y_ind].u;
	*vleft = vb[min_y_ind].v;

	//	Check for horizontal bitmap (min_y and following vertex have same y coord integer)
	left_break_ind = (min_y_ind + 3) % 4;
	if (f2i(vb[(min_y_ind+1) % 4].y) == *min_y) {
		right_break_ind = (min_y_ind + 2) % 4;
		*xright = vb[(min_y_ind+1)%4].x;
	} else {
		right_break_ind = (min_y_ind + 1) % 4;
		*xright = *xleft;					// bitmap not horizontal, left and right start out the same
	}

	*lbind = left_break_ind;
	*rbind = right_break_ind;

	//	Compute delta x for left and right.
	if (*max_y - *min_y) {
		int	dy;
		dy = f2i(vb[left_break_ind].y) - *min_y;
		if (dy) {
			*dx_dy_left = (vb[*lbind].x - *xleft)/(f2i(vb[left_break_ind].y) - *min_y);
			*du_dy_left = (vb[left_break_ind].u - vb[min_y_ind].u)/(f2i(vb[left_break_ind].y) - *min_y);
			*dv_dy_left = (vb[left_break_ind].v - vb[min_y_ind].v)/(f2i(vb[left_break_ind].y) - *min_y);
		} else {
			*dx_dy_left = 0;
			*du_dy_left = 0;
			*dv_dy_left = 0;
		}
		dy = f2i(vb[right_break_ind].y) - *min_y;
		if (dy) {
			*dx_dy_right = (vb[*rbind].x - *xright)/(f2i(vb[right_break_ind].y) - *min_y);
			*du_dy_right = (vb[right_break_ind].u - vb[min_y_ind].u)/(f2i(vb[right_break_ind].y) - *min_y);
			*dv_dy_right = (vb[right_break_ind].v - vb[min_y_ind].v)/(f2i(vb[right_break_ind].y) - *min_y);
		} else {
			*dx_dy_right = 0;
			*du_dy_right = 0;
			*dv_dy_right = 0;
		}
	} else {		// bitmap is one row tall, don't care about delta x
		*dx_dy_left = 0;
		*du_dy_left = 0;
		*dv_dy_left = 0;
		*dx_dy_right = 0;
		*du_dy_right = 0;
		*dv_dy_right = 0;
	}


}

// 8A 06                mov    al,[esi]
// 3C 81                cmp    al,129
// 74 02                je     skip
// 88 07                mov    [edi],al
// 47          skip:    inc    edi
// 81 C6   00001234     add    esi,1234h
 
//      6	00000000  8A 1E				 mov	 bl,[esi]
//      7	00000002  8A 03				 mov	 al,[ebx]
//      8	00000004  8A 04	03			 mov	 al,[ebx+eax]

//      6	00000000  8A 0E				 mov	 cl,[esi]
//      7	00000002  8A 03				 mov	 al,[ebx]
//      8	00000004  8A 04	0A			 mov	 al,[edx+ecx]
//      9	00000007  80 F9	81			 cmp	 cl,129

#define	mov_cl_si_1	0x8a
#define	mov_cl_si_2	0x0e
#define	mov_bl_si_1	0x8a
#define	mov_bl_si_2	0x1e
#define	mov_al_si_1	0x8a
#define	mov_al_si_2	0x06
#define	cmp_al_k		0x3c
#define	cmp_cl_k_1	0x80
#define	cmp_cl_k_2	0xf9
#define	je_addr		0x74
#define	mov_al_edx_ecx_1	0x8a
#define	mov_al_edx_ecx_2	0x04
#define	mov_al_edx_ecx_3	0x0a
#define	mov_di_al_1	0x88
#define	mov_di_al_2	0x07
#define	add_si_k_1	0x81
#define	add_si_k_2	0xc6
#define	inc_di		0x47
#define	ret			0xc3

#define	MAX_COMPILED_CODE_POINTERS 1000
#define	MAX_COMPILED_CODE	(MAX_COMPILED_CODE_POINTERS * 15)

char	Compiled_code[MAX_COMPILED_CODE];
int	Compiled_code_pointers[MAX_COMPILED_CODE_POINTERS];

// --------------------------------------------------------------------------------------
// 8a 04            mov   al,[si]
// 88 05            mov   [di],al
// 81 c6 34 12      add   si,1234
// 47               inc   di
// c3               ret
//	Return true if too much compiled code, else return false.
int create_compiled_code(fix du_dx, fix dv_dx, int width, int min_x, int max_x)
{
#if COMPILED_CODE
	int	x;
	char	*ccp;
	int	*ccpp;
	int	*ccp_int;
	int	delta_si;
	fix	cur_u, cur_v, new_u, new_v;
	int	delta_x;

	cur_u = 0;
	cur_v = 0;
	new_u = du_dx;
	new_v = dv_dx;
	ccp = Compiled_code;
	ccpp = Compiled_code_pointers;

	//	Initialize all code pointers to something which will cause an exception.
	for (x=0; x<MAX_COMPILED_CODE_POINTERS; x++)
		Compiled_code_pointers[x] = 0xffffffff;

	if (max_x - min_x +2 >= MAX_COMPILED_CODE_POINTERS)
		delta_x = MAX_COMPILED_CODE_POINTERS-1;
	else
		delta_x = max_x - min_x + 2;

	Compiled_code_width = delta_x;

	for (x=0; x<=delta_x; x++) {
		*ccpp++ = (int) ccp;
		*ccp++ = mov_cl_si_1;
		*ccp++ = mov_cl_si_2;

		*ccp++ = cmp_cl_k_1;
		*ccp++ = cmp_cl_k_2;
		*ccp++ = TRANSPARENCY_COLOR;				//Transparency_color;			// transparency color

		*ccp++ = je_addr;
		*ccp++ = 5;				// je $+5

		*ccp++ = mov_al_edx_ecx_1;
		*ccp++ = mov_al_edx_ecx_2;
		*ccp++ = mov_al_edx_ecx_3;

		*ccp++ = mov_di_al_1;
		*ccp++ = mov_di_al_2;

		*ccp++ = add_si_k_1;
		*ccp++ = add_si_k_2;

		delta_si = f2i(new_u) - f2i(cur_u) + (  f2i(new_v) - f2i(cur_v)) * width;
		ccp_int = (int *)ccp;	
		*ccp_int = delta_si;
		ccp += sizeof(int)/sizeof(char);

		*ccp++ = inc_di;

		if (ccp - Compiled_code >= MAX_COMPILED_CODE - 15)
			return 1; //Error("Too much compiled code.\n");

		if (ccpp - Compiled_code_pointers >= MAX_COMPILED_CODE_POINTERS)
			return 1; //Error("Too many compiled code pointers.\n");

		cur_u = new_u;
		cur_v = new_v;
		new_u += du_dx;
		new_v += dv_dx;
	}

	*ccpp++ = (int) ccp;
	*ccp = ret;

	return 0;
#else
	if (min_x + max_x + width + du_dx == dv_dx - 9000)
		mprintf(0,"Holy smokes!");

	return 0;
#endif
}

int call_compiled_code(char *start_si, char *start_di, int xleft, int xright, int light_value);
#pragma aux call_compiled_code parm [esi] [edi] [eax] [ebx] [edx] value [eax] modify [ecx edx] = \
	"pusha"	\
	"sub	ebx,eax" \
	"mov	eax,Compiled_code_pointers" \
	"inc	ebx"	\
	"mov	ebx,Compiled_code_pointers[ebx*4]"	\
	"mov	cl,[ebx]"	\
	"push	ebx"	\
	"push	ecx"	\
	"mov	byte ptr [ebx],0c3h"	\
	"sub	ecx,ecx" \
	"shl	edx,8"	\
	"add	edx,offset gr_fade_table" \
	"call	eax"	\
	"pop	ecx"	\
	"pop	ebx"	\
	"mov	[ebx],cl"	\
	"popa"

// --------------------------------------------------------------------------------------
void render_scanline(unsigned char *pixptr, int width, int y, fix xleft, fix xright, fix u, fix v, fix du_dx, fix dv_dx, int light_value)
{

#if COMPILED_CODE
	unsigned char *start_si, *start_di;

	if (xright - xleft < 0)
		return;

	start_si = pixptr + f2i(u) + f2i(v) * width;
	start_di = grd_curcanv->cv_bitmap.bm_data + grd_curcanv->cv_bitmap.bm_rowsize*y + f2i(xleft);
	if (f2i(xright) - f2i(xleft) < Compiled_code_width)
		call_compiled_code(start_si, start_di, f2i(xleft), f2i(xright), light_value);
	else
		mprintf(0,"Not calling compiled code for xleft = %i, xright = %i, width = %i\n",f2i(xleft), f2i(xright), f2i(width));

	if (du_dx + dv_dx == 3)
		mprintf(0,"That's peculiar...\n");
#else
	int	x;

	for (x=f2i(xleft); x<f2i(xright); x++) {
		int	color;

		color = *(pixptr + f2i(v)*width + f2i(u));
		if (color != TRANSPARENCY_COLOR )	{					//Transparency_color) {
			gr_setcolor(color);
			gr_pixel(x,y);
		}
		u += du_dx;
		v += dv_dx;
	}
#endif

}

// --------------------------------------------------------------------------------------
void render_bitmap(grs_bitmap *bp, xyuv *vb, int min_y, int max_y, int lbind, int rbind,
	fix du_dx, fix dv_dx, fix xleft, fix uleft, fix vleft, fix xright,
	fix dx_dy_left, fix du_dy_left, fix dv_dy_left, fix dx_dy_right, fix du_dy_right, fix dv_dy_right, int light_value)
{
	int	y;
	int	lby, rby;
	fix	dx_dyl,du_dyl,dv_dyl;
	fix	dx_dyr,du_dyr,dv_dyr;
	fix	fix_width;

	unsigned char *pixptr = bp->bm_data;
	int	width = bp->bm_w;

	lby = f2i(vb[lbind].y);
	rby = f2i(vb[rbind].y);

	dx_dyl = dx_dy_left;
	du_dyl = du_dy_left;
	dv_dyl = dv_dy_left;
	dx_dyr = dx_dy_right;
	du_dyr = du_dy_right;
	dv_dyr = dv_dy_right;

	//	Now, render the bitmap.
	if (max_y >= grd_curcanv->cv_bitmap.bm_h)
		max_y = grd_curcanv->cv_bitmap.bm_h-1;

	for (y=min_y; y<=max_y; y++) {
		fix u,v;
		u = uleft;
		v = vleft;
		if (y == lby) {
			dx_dy_left = dx_dyr;
			du_dy_left = du_dyr;
			dv_dy_left = dv_dyr;
			xleft = vb[lbind].x;
			uleft = vb[lbind].u;
			vleft = vb[lbind].v;
		}

		if (y == rby) {
			dx_dy_right = dx_dyl;
			xright = vb[rbind].x;
		}

		fix_width = i2f(grd_curcanv->cv_bitmap.bm_w);
		if (y >= 0) {
			fix	txright = xright;
			fix	txleft = xleft;

			if (txleft < 0) {
				u -= fixmul(txleft,du_dx);
				v -= fixmul(txleft,dv_dx);
				txleft = 0;
			}
			if (txright > fix_width)
				txright = fix_width-F1_0;
			if ((xright > 0) && (xleft < fix_width))
				render_scanline(pixptr, width, y, txleft, txright, u, v, du_dx, dv_dx, light_value);
		}

		xleft += dx_dy_left;
		xright += dx_dy_right;
		uleft += du_dy_left;
		vleft += dv_dy_left;
	}

}

// --------------------------------------------------------------------------------------
//	A scaled/rotated bitmap is defined by 3 vertices, upper left, upper right, lower right
void rotate_bitmap(grs_bitmap *bp, grs_point *vertbuf, int light_value)
{
	fix	du_dx,dv_dx;
	int	min_y,max_y;
	int	lbind,rbind;
	fix	xleft,xright;
	fix	uleft,vleft;
	fix	dx_dy_left, du_dy_left, dv_dy_left, dx_dy_right, du_dy_right, dv_dy_right;
	int	min_x, max_x;
	//char	*pixptr = bp->bm_data;
	//int	width = bp->bm_w;
	
	xyuv		vb[4];

	create_4_points(vertbuf, vb, bp->bm_w, bp->bm_h);

	compute_deltas(vb, &min_y, &max_y, &lbind, &rbind, &du_dx, &dv_dx, &xleft, &uleft, &vleft, &xright,
			&dx_dy_left, &du_dy_left, &dv_dy_left, &dx_dy_right, &du_dy_right, &dv_dy_right, &min_x, &max_x);

//	//	Nasty, ugly, illegal hack -- set transparency color based on top 3 left pixels.
//	if ((*pixptr == *(pixptr+width)) && (*pixptr == *(pixptr+1)))			// try upper left corner
//		Transparency_color = *pixptr;
//	else if ((*(pixptr+width-1) == *(pixptr+width-2)) && (*(pixptr+width-1) == *(pixptr+2*width-1)))	// try upper right corner
//		Transparency_color = *(pixptr + width - 1);
//	else
// 	Transparency_color = TRANSPARENCY_COLOR;

	if (max_x - min_x < MAX_COMPILED_CODE_POINTERS) {
		if (!create_compiled_code(du_dx, dv_dx, bp->bm_w, min_x, max_x))
			render_bitmap(bp, vb, min_y, max_y, lbind, rbind, du_dx, dv_dx, xleft, uleft, vleft, xright,
								dx_dy_left, du_dy_left, dv_dy_left, dx_dy_right, du_dy_right, dv_dy_right, light_value);
	}
}

// --------------------------------------------------------------------------------------
#define DEFAULT_X_SIZE 32
#define DEFAULT_Y_SIZE 32
#define CENTER_X 160
#define CENTER_Y 100

void set_coords(grs_point *verts, float bank, float scale)
{
	int	i;
	float	x[3],y[3];
	float	sinbank,cosbank;

	cosbank = cos(bank*3.14159/180.0);
	sinbank = sin(bank*3.14159/180.0);

	x[0] = -DEFAULT_X_SIZE * scale;
	y[0] = -DEFAULT_Y_SIZE * scale;

	x[1] = +DEFAULT_X_SIZE * scale;
	y[1] = -DEFAULT_Y_SIZE * scale;

	x[2] = +DEFAULT_X_SIZE * scale;
	y[2] = +DEFAULT_Y_SIZE * scale;

	for (i=0; i<3; i++) {
		verts[i].x = fl2f(x[i] * cosbank - y[i] * sinbank + CENTER_X);
		verts[i].y = fl2f(x[i] * sinbank + y[i] * cosbank + CENTER_Y);
	}

}

//##// --------------------------------------------------------------------------------------
//##void test_rotate_bitmap(void)
//##{
//##	static float	bank = 0.0;
//##
//##	grs_point	verts[3];
//##
//##	bank += 4.0;
//##
//##	set_coords(verts, bank, 2*pow(sin(bank/40),2)+.8);
//##
//##//	verts[0].x = 50 * F1_0;
//##//	verts[0].y = 50 * F1_0;
//##//
//##//	verts[1].x = 150 * F1_0;
//##//	verts[1].y = 80 * F1_0;
//##//
//##//	verts[2].x = 180 * F1_0;
//##//	verts[2].y = 120 * F1_0;
//##
//##	rotate_bitmap(Texture[36],verts, 15);
//##}

#else

void rotate_bitmap(grs_bitmap *bp, grs_point *vertbuf, int light_value)
{
	bp = bp;
	vertbuf = vertbuf;
	light_value = light_value;
	
	Int3();			// ROLLER IS IFDEF'D OUT !!!!!!!
}


#endif 		// USE_ROLLER

