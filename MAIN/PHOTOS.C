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
 * $Source: f:/miner/source/main/rcs/photos.c $
 * $Revision: 2.1 $
 * $Author: john $
 * $Date: 1995/03/06 15:23:34 $
 * 
 * Take multiple-view "photos" of an object
 * 
 * $Log: photos.c $
 * Revision 2.1  1995/03/06  15:23:34  john
 * New screen techniques.
 * 
 * Revision 2.0  1995/02/27  11:29:34  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.8  1993/12/09  15:07:27  john
 * Changed palette stuff majorly... no more grd_curscreen->palette
 * 
 * Revision 1.7  1993/12/05  22:47:49  matt
 * Reworked include files in an attempt to cut down on build times
 * 
 * Revision 1.6  1993/12/01  00:27:08  yuan
 * Implemented new bitmap structure system...
 * overall bitmap scheme still needs some work.
 * 
 * Revision 1.5  1993/11/29  19:50:11  matt
 * Change vertex allocation
 * 
 * Revision 1.4  1993/11/29  18:57:49  matt
 * Changed vertex numbering around a bit
 * 
 * Revision 1.3  1993/11/23  14:35:50  matt
 * Don't do perspecive on photos; save up vectors to file
 * 
 * Revision 1.2  1993/11/22  10:53:05  matt
 * Now use correct uvl structure (not just array of uvs)
 * 
 * Revision 1.1  1993/11/21  22:53:24  matt
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: photos.c 2.1 1995/03/06 15:23:34 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>

#include "inferno.h"
#include "segment.h"
#include "error.h"
#include "vecmat.h"
#include "3d.h"
#include "iff.h"
#include "game.h"
#include "textures.h"
#include "screens.h"
#include "segpoint.h"

#include "editor\editor.h"

extern byte Vertex_active[];	// !0 means vertex is in use, 0 means not in use.

#define CUBE_SIZE i2f(5)

//the ordering here is the same as for a segment
vms_vector cube_coords[] =	{
										{ CUBE_SIZE, CUBE_SIZE,-CUBE_SIZE},
										{ CUBE_SIZE,-CUBE_SIZE,-CUBE_SIZE},
										{-CUBE_SIZE,-CUBE_SIZE,-CUBE_SIZE},
										{-CUBE_SIZE, CUBE_SIZE,-CUBE_SIZE},

										{ CUBE_SIZE, CUBE_SIZE, CUBE_SIZE},
										{ CUBE_SIZE,-CUBE_SIZE, CUBE_SIZE},
										{-CUBE_SIZE,-CUBE_SIZE, CUBE_SIZE},
										{-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE},

										{0,0,0}	//center of cube
									};


vms_vector cube_position={0,0,0};

uvl cube_uvls[] =	{
							i2f(64),0,0,
							i2f(64),i2f(64),0,
							0,i2f(64),0,
							0,0,0,
						};

//put a cube on the screen
draw_cube()
{
	int i,sn;
	short cube_vert_nums[9];

	//find 8 free vertices

	for (i=0;i<9;i++) {
		int vn;

		vn = cube_vert_nums[i] = alloc_vert();

		vm_vec_add(&Vertices[vn],&cube_position,&cube_coords[i]);
	}

	g3_rotate_list(9,cube_vert_nums);

	//fudge perspecive by using z value from cube center for all verts

	for (i=0;i<8;i++)
		Segment_points[cube_vert_nums[i]].z = Segment_points[cube_vert_nums[8]].z;
	
	//draw six sides

	for (sn=0;sn<6;sn++) {
		short side_vert_nums[4];

		for (i=0;i<4;i++) side_vert_nums[3-i] = cube_vert_nums[Side_to_verts[sn][i]];

//		gr_setcolor(sn+1);
//		g3_check_and_draw_poly(4,side_vert_nums,NULL);

		g3_check_and_draw_tmap(4,side_vert_nums,cube_uvls,Texture[39+sn]->bitmap,NULL);
	}

	//free 8 free vertices

	for (i=0;i<8;i++) free_vert(cube_vert_nums[i]);

}

#define PHOTO_DIST (CUBE_SIZE * 7 / 2)

//make a series of photographs
do_photos()
{
	FILE *vfile,*upvfile;
	int photo_num=0;
	char savename[13];
	grs_canvas *photo_canvas;
	vms_vector viewer_pos;
	vms_matrix viewer_orient;

	vfile=fopen("vectors.lst","rt");
	upvfile=fopen("upvecs.c","wt");

	Assert(vfile!=NULL && upvfile!=NULL);

	fprintf(upvfile,"\n\n#include \"vecmat.h\"\n\nvms_vector up_vecs[] = {\n");

	photo_canvas = gr_create_canvas(64,64);

	gr_set_current_canvas(photo_canvas);

	while (!feof(vfile)) {
		vms_vector v;
		vms_matrix m;
		float x,y,z;
		int nf;

		nf = fscanf(vfile,"%f %f %f",&x,&y,&z);

		if (nf!=3) break;

		vm_vec_make(&v,fl2f(x),fl2f(y),fl2f(z));

		vm_vector_2_matrix(&m,&v,NULL,NULL);

		fprintf(upvfile,"\t\t\t{%#x,%#x,%#x},\n",m.uvec.x,m.uvec.y,m.uvec.z);

		vm_vec_scale(&v,PHOTO_DIST);

		vm_vec_add(&viewer_pos,&cube_position,&v);

		viewer_orient = m;
		vm_vec_negate(&viewer_orient.fvec);
		vm_vec_negate(&viewer_orient.rvec);

		gr_clear_canvas(129);
		g3_start_frame();

		g3_set_view_matrix(&viewer_pos,&viewer_orient,0x9000);

		draw_cube();

		g3_end_frame();

gr_set_current_canvas(Canv_game);
gr_ubitmap(0,0,&photo_canvas->cv_bitmap);
gr_set_current_canvas(photo_canvas);

		sprintf(savename,"cube_%02d.bbm",photo_num);

		iff_write_bitmap(savename,&photo_canvas->cv_bitmap,gr_palette);

		photo_num++;

	}

	gr_free_canvas(photo_canvas);

	fprintf(upvfile,"\t\t};\n");

	fclose(vfile);
	fclose(upvfile);

}


