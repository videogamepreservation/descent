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
 * $Source: f:/miner/source/main/rcs/segment2.h $
 * $Revision: 1.2 $
 * $Author: mike $
 * $Date: 1993/09/20 15:17:49 $
 *
 * Include file for makeseg program, not necessary for game or editor.
 *
 * $Log: segment2.h $
 * Revision 1.2  1993/09/20  15:17:49  mike
 * Define constants: NUM_FACES, NUM_VERTICES, NUM_UVS, NUM_RGBS, NUM_FACE_VERTS
 * 
 * Revision 1.1  1993/09/15  19:12:32  matt
 * Initial revision
 * 
 *
 */

// ----------------------------------------------------------------------------------------------------------
#define ST_NONE			0		//no draw, for timing or whatever
#define ST_FLAT			1
#define ST_TMAP			2
#define ST_TMAP_NOLIGHT	3
#define ST_TMAP_RGB		4

// ----------------------------------------------------------------------------------------------------------
extern	float		Cx,Cy,Cz,Lx,Ly,Lz,Rx,Ry,Rz;
extern	int		Num_faces,Num_vertices,Num_uvs,Num_rgbs,Num_face_verts;

extern	segment *	make_segment(void);
extern	void			init_segments(void);
extern	void			write_segment(segment *sp);
extern	void			write_globals(void);

extern	void			fix_write_segment(segment *sp);
extern	void			fix_write_globals(void);

extern	int		Face_verts_free;



