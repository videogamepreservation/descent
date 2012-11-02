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
 * $Source: f:/miner/source/main/rcs/gamemine.c $
 * $Revision: 1.45 $
 * $Author: john $
 * $Date: 1994/10/27 18:51:42 $
 * 
 * Functions for loading mines in the game
 * 
 * $Log: gamemine.c $
 * Revision 1.45  1994/10/27  18:51:42  john
 * Added -piglet option that only loads needed textures for a 
 * mine.  Only saved ~1MB, and code still doesn't free textures
 * before you load a new mine.
 * 
 * Revision 1.44  1994/10/20  12:47:22  matt
 * Replace old save files (MIN/SAV/HOT) with new LVL files
 * 
 * Revision 1.43  1994/10/19  16:46:40  matt
 * Made tmap overrides for robots remap texture numbers
 * 
 * Revision 1.42  1994/10/03  23:37:01  mike
 * Adapt to changed fuelcen_activate parameters.
 * 
 * Revision 1.41  1994/09/23  22:14:49  matt
 * Took out obsolete structure fields
 * 
 * Revision 1.40  1994/08/01  11:04:11  yuan
 * New materialization centers.
 * 
 * Revision 1.39  1994/07/21  19:01:47  mike
 * Call Lsegment stuff.
 * 
 * 
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: gamemine.c 1.45 1994/10/27 18:51:42 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mono.h"

#include "inferno.h"
#include "segment.h"
#include "textures.h"
#include "wall.h"
#include "object.h"
#include "gamemine.h"
#include "error.h"
#include "gameseg.h"
#include "switch.h"

#include "game.h"

#ifdef EDITOR
#include "editor\editor.h"
#include "ui.h"
#endif

#include "nocfile.h"		
#include "fuelcen.h"

#include "hash.h"
#include "key.h"
#include "piggy.h"

//Structures for old mine versions 

#define	V15_MAX_VERTICES_PER_SEGMENT	8
#define	V15_MAX_SIDES_PER_SEGMENT		6

typedef struct v15_side {
	byte		type;									// replaces num_faces and tri_edge, 1 = quad, 2 = 0:2 triangulation, 3 = 1:3 triangulation
	short		tmap_num;
	short		tmap_num2;
	uvl		uvls[4];
	short		wall_num;
	vms_vector	normals[2];						// 2 normals, if quadrilateral, both the same.
} v15_side;

typedef struct v15_segment {
	short		segnum;								// segment number, not sure what it means
	v15_side	sides[V15_MAX_SIDES_PER_SEGMENT];	// 6 sides
	short		children[V15_MAX_SIDES_PER_SEGMENT];	// indices of 6 children segments, front, left, top, right, bottom, back
	short		verts[V15_MAX_VERTICES_PER_SEGMENT];	// vertex ids of 4 front and 4 back vertices
	short		group;								// group number to which the segment belongs.
	short		objects;								// pointer to objects in this segment
	short		special;								// special property of a segment (such as damaging, trigger, etc.)
	short		value;
} v15_segment;

typedef struct {
	int		num_segments;
	int		num_vertices;
	short		segments[MAX_SEGMENTS];
	short		vertices[MAX_VERTICES];
} v15_group;

//End of old structures

#define REMOVE_EXT(s)  (*(strchr( (s), '.' ))='\0')

struct mtfi mine_top_fileinfo;    // Should be same as first two fields below...
struct mfi mine_fileinfo;
struct mh mine_header;
struct me mine_editor;

int CreateDefaultNewSegment();

static char old_tmap_list[MAX_TEXTURES][13];
short tmap_xlate_table[MAX_TEXTURES];

static short tmap_times_used[MAX_TEXTURES];

//void JohnError( char * s )
//{
//	Int3();
//}


// -----------------------------------------------------------------------------
// returns 1 if error, else 0
int game_load_mine(char * filename)
{
	CFILE * LoadFile;

	LoadFile = cfopen( filename, CF_READ_MODE );
	if (!LoadFile)
	{
		#ifdef EDITOR
		char  ErrorMessage[200];

		sprintf( ErrorMessage, "ERROR: Unable to open mine %s\n", filename );
		stop_time();
		MessageBox( -2, -2, 1, ErrorMessage, "Ok" );
		start_time();
		#endif

		return 1;
	}

	load_mine_data(LoadFile);

	cfclose( LoadFile );

	return 0;
}


// -----------------------------------------------------------------------------
//loads from an already-open file
// returns 0=everything ok, 1=old version, -1=error
int load_mine_data(CFILE *LoadFile)
{
	int   i, j;
	short tmap_xlate;
	int 	translate;
	char 	*temptr;
	int	mine_start = cftell(LoadFile);

	fuelcen_reset();

	for (i=0; i<MAX_TEXTURES; i++ )
		tmap_times_used[i] = 0;
	
	#ifdef EDITOR
	// Create a new mine to initialize things.
	//texpage_goto_first();
	create_new_mine();
	#endif

	//===================== READ FILE INFO ========================

	// These are the default values... version and fileinfo_sizeof
	// don't have defaults.
	mine_fileinfo.header_offset     =   -1;
	mine_fileinfo.header_size       =   sizeof(mine_header);
	mine_fileinfo.editor_offset     =   -1;
	mine_fileinfo.editor_size       =   sizeof(mine_editor);
	mine_fileinfo.vertex_offset     =   -1;
	mine_fileinfo.vertex_howmany    =   0;
	mine_fileinfo.vertex_sizeof     =   sizeof(vms_vector);
	mine_fileinfo.segment_offset    =   -1;
	mine_fileinfo.segment_howmany   =   0;
	mine_fileinfo.segment_sizeof    =   sizeof(segment);
	mine_fileinfo.newseg_verts_offset     =   -1;
	mine_fileinfo.newseg_verts_howmany    =   0;
	mine_fileinfo.newseg_verts_sizeof     =   sizeof(vms_vector);
	mine_fileinfo.group_offset		  =	-1;
	mine_fileinfo.group_howmany	  =	0;
	mine_fileinfo.group_sizeof		  =	sizeof(group);
	mine_fileinfo.texture_offset    =   -1;
	mine_fileinfo.texture_howmany   =   0;
	mine_fileinfo.texture_sizeof    =   13;  // num characters in a name
 	mine_fileinfo.walls_offset		  =	-1;
	mine_fileinfo.walls_howmany	  =	0;
	mine_fileinfo.walls_sizeof		  =	sizeof(wall);  
 	mine_fileinfo.triggers_offset	  =	-1;
	mine_fileinfo.triggers_howmany  =	0;
	mine_fileinfo.triggers_sizeof	  =	sizeof(trigger);  
	mine_fileinfo.object_offset		=	-1;
	mine_fileinfo.object_howmany		=	1;
	mine_fileinfo.object_sizeof		=	sizeof(object);  

	// Read in mine_top_fileinfo to get size of saved fileinfo.
	
	memset( &mine_top_fileinfo, 0, sizeof(mine_top_fileinfo) );

	if (cfseek( LoadFile, mine_start, SEEK_SET ))
		Error( "Error moving to top of file in gamemine.c" );

	if (cfread( &mine_top_fileinfo, sizeof(mine_top_fileinfo), 1, LoadFile )!=1)
		Error( "Error reading mine_top_fileinfo in gamemine.c" );

	if (mine_top_fileinfo.fileinfo_signature != 0x2884)
		return -1;

	// Check version number
	if (mine_top_fileinfo.fileinfo_version < COMPATIBLE_VERSION )
		return -1;

	// Now, Read in the fileinfo
	if (cfseek( LoadFile, mine_start, SEEK_SET ))
		Error( "Error seeking to top of file in gamemine.c" );

	if (cfread( &mine_fileinfo, mine_top_fileinfo.fileinfo_sizeof, 1, LoadFile )!=1)
		Error( "Error reading mine_fileinfo in gamemine.c" );

	//===================== READ HEADER INFO ========================

	// Set default values.
	mine_header.num_vertices        =   0;
	mine_header.num_segments        =   0;

	if (mine_fileinfo.header_offset > -1 )
	{
		if (cfseek( LoadFile, mine_fileinfo.header_offset, SEEK_SET ))
			Error( "Error seeking to header_offset in gamemine.c" );
	
		if (cfread( &mine_header, mine_fileinfo.header_size, 1, LoadFile )!=1)
			Error( "Error reading mine_header in gamemine.c" );
	}

	//===================== READ EDITOR INFO ==========================

	// Set default values
	mine_editor.current_seg         =   0;
	mine_editor.newsegment_offset   =   -1; // To be written
	mine_editor.newsegment_size     =   sizeof(segment);
	mine_editor.Curside             =   0;
	mine_editor.Markedsegp          =   -1;
	mine_editor.Markedside          =   0;

	if (mine_fileinfo.editor_offset > -1 )
	{
		if (cfseek( LoadFile, mine_fileinfo.editor_offset, SEEK_SET ))
			Error( "Error seeking to editor_offset in gamemine.c" );
	
		if (cfread( &mine_editor, mine_fileinfo.editor_size, 1, LoadFile )!=1)
			Error( "Error reading mine_editor in gamemine.c" );
	}

	//===================== READ TEXTURE INFO ==========================

	if ( (mine_fileinfo.texture_offset > -1) && (mine_fileinfo.texture_howmany > 0))
	{
		if (cfseek( LoadFile, mine_fileinfo.texture_offset, SEEK_SET ))
			Error( "Error seeking to texture_offset in gamemine.c" );

		for (i=0; i< mine_fileinfo.texture_howmany; i++ )
		{
			if (cfread( &old_tmap_list[i], mine_fileinfo.texture_sizeof, 1, LoadFile )!=1)
				Error( "Error reading old_tmap_list[i] in gamemine.c" );
		}
	}

	//=============== GENERATE TEXTURE TRANSLATION TABLE ===============

	translate = 0;
	
	Assert (NumTextures < MAX_TEXTURES);

	{
		hashtable ht;
	
		hashtable_init( &ht, NumTextures );
	
		// Remove all the file extensions in the textures list
	
		for (i=0;i<NumTextures;i++)	{
			temptr = strchr(TmapInfo[i].filename, '.');
			if (temptr) *temptr = '\0';
			hashtable_insert( &ht, TmapInfo[i].filename, i );
		}
	
		// For every texture, search through the texture list
		// to find a matching name.
		for (j=0;j<mine_fileinfo.texture_howmany;j++) 	{
			// Remove this texture name's extension
			temptr = strchr(old_tmap_list[j], '.');
			if (temptr) *temptr = '\0';
	
			tmap_xlate_table[j] = hashtable_search( &ht,old_tmap_list[j]);
			if (tmap_xlate_table[j]	< 0 )	{
				tmap_xlate_table[j] = 0;
				mprintf( 0, "Couldn't find texture '%s'\n", old_tmap_list[j] );
			}
			if (tmap_xlate_table[j] != j ) translate = 1;
			tmap_times_used[tmap_xlate_table[j]]++;
		}
	
		{
			int count = 0;
			for (i=0; i<MAX_TEXTURES; i++ )
				if (tmap_times_used[i])
					count++;
			mprintf( 0, "This mine has %d unique textures in it (~%.2f MB)\n", count, ((float)(count*4096))/(1024.0*1024.0) );
		}
	
		mprintf( 0, "Translate=%d\n", translate );
	
		hashtable_free( &ht );
	}

	//====================== READ VERTEX INFO ==========================

	// New check added to make sure we don't read in too many vertices.
	if ( mine_fileinfo.vertex_howmany > MAX_VERTICES )
		{
		mprintf(0, "Num vertices exceeds maximum.  Loading MAX %d vertices\n", MAX_VERTICES);
		mine_fileinfo.vertex_howmany = MAX_VERTICES;
		}

	if ( (mine_fileinfo.vertex_offset > -1) && (mine_fileinfo.vertex_howmany > 0))
	{
		if (cfseek( LoadFile, mine_fileinfo.vertex_offset, SEEK_SET ))
			Error( "Error seeking to vertex_offset in gamemine.c" );

		for (i=0; i< mine_fileinfo.vertex_howmany; i++ )
		{
			// Set the default values for this vertex
			Vertices[i].x = 1;
			Vertices[i].y = 1;
			Vertices[i].z = 1;

			if (cfread( &Vertices[i], mine_fileinfo.vertex_sizeof, 1, LoadFile )!=1)
				Error( "Error reading Vertices[i] in gamemine.c" );
		}
	}

	//==================== READ SEGMENT INFO ===========================

	// New check added to make sure we don't read in too many segments.
	if ( mine_fileinfo.segment_howmany > MAX_SEGMENTS ) {
		mprintf(0, "Num segments exceeds maximum.  Loading MAX %d segments\n", MAX_SEGMENTS);
		mine_fileinfo.segment_howmany = MAX_SEGMENTS;
	}

	fuelcen_reset();

	if ( (mine_fileinfo.segment_offset > -1) && (mine_fileinfo.segment_howmany > 0))	{

		if (cfseek( LoadFile, mine_fileinfo.segment_offset,SEEK_SET ))

			Error( "Error seeking to segment_offset in gamemine.c" );

		for (i=0; i< mine_fileinfo.segment_howmany; i++ ) {
			segment v16_seg;
			v15_segment v15_seg;

			// Set the default values for this segment (clear to zero )
			//memset( &Segments[i], 0, sizeof(segment) );

			if (mine_top_fileinfo.fileinfo_version >= 16) {

				Assert(mine_fileinfo.segment_sizeof == sizeof(v16_seg));

				if (cfread( &v16_seg, mine_fileinfo.segment_sizeof, 1, LoadFile )!=1)
					Error( "Error reading segments in gamemine.c" );

			}				
			else if (mine_top_fileinfo.fileinfo_version < 16) {
				int t;

				Assert(mine_fileinfo.segment_sizeof == sizeof(v15_seg));

				if (cfread( &v15_seg, mine_fileinfo.segment_sizeof, 1, LoadFile )!=1)
					Error( "Error reading segments in gamemine.c" );

				v16_seg.segnum  = v15_seg.segnum;
				v16_seg.group   = v15_seg.group;
				v16_seg.objects = v15_seg.objects;
				v16_seg.special = v15_seg.special;
				v16_seg.value   = v15_seg.value;
				
				for (t=0;t<MAX_SIDES_PER_SEGMENT;t++) {
					int n;

					v16_seg.children[t] = v15_seg.children[t];

					v16_seg.sides[t].type		= v15_seg.sides[t].type;
					v16_seg.sides[t].wall_num	= v15_seg.sides[t].wall_num;
					v16_seg.sides[t].tmap_num	= v15_seg.sides[t].tmap_num;
					v16_seg.sides[t].tmap_num2	= v15_seg.sides[t].tmap_num2;
					v16_seg.sides[t].pad			= 0;

					for (n=0;n<4;n++)
						v16_seg.sides[t].uvls[n] = v15_seg.sides[t].uvls[n];

					for (n=0;n<2;n++)
						v16_seg.sides[t].normals[n] = v15_seg.sides[t].normals[n];

				}
				
				for (t=0;t<MAX_VERTICES_PER_SEGMENT;t++)
					v16_seg.verts[t]    = v15_seg.verts[t];

				v16_seg.static_light = 0;

			}

			Segments[i] = v16_seg;

			Segments[i].objects = -1;
			Segments[i].group = -1;

			if (mine_top_fileinfo.fileinfo_version < 15) {	//used old uvl ranges
				int sn,uvln;

				for (sn=0;sn<MAX_SIDES_PER_SEGMENT;sn++)
					for (uvln=0;uvln<4;uvln++) {
						Segments[i].sides[sn].uvls[uvln].u /= 64;
						Segments[i].sides[sn].uvls[uvln].v /= 64;
						Segments[i].sides[sn].uvls[uvln].l /= 32;
					}
			}

			fuelcen_activate( &Segments[i], Segments[i].special );

			if (translate == 1)
				for (j=0;j<MAX_SIDES_PER_SEGMENT;j++) {
					unsigned short orient;
					tmap_xlate = Segments[i].sides[j].tmap_num;
					Segments[i].sides[j].tmap_num = tmap_xlate_table[tmap_xlate];
					tmap_xlate = Segments[i].sides[j].tmap_num2 & 0x3FFF;
					orient = Segments[i].sides[j].tmap_num2 & (~0x3FFF);
					if (tmap_xlate != 0)
						Segments[i].sides[j].tmap_num2 = tmap_xlate_table[tmap_xlate] | orient;
				}
		}
	}

	//===================== READ NEWSEGMENT INFO =====================

	#ifdef EDITOR

	{		// Default segment created.
		vms_vector	sizevec;
		med_create_new_segment(vm_vec_make(&sizevec,DEFAULT_X_SIZE,DEFAULT_Y_SIZE,DEFAULT_Z_SIZE));		// New_segment = Segments[0];
		//memset( &New_segment, 0, sizeof(segment) );
	}

	if (mine_editor.newsegment_offset > -1)
	{
		if (cfseek( LoadFile, mine_editor.newsegment_offset,SEEK_SET ))
			Error( "Error seeking to newsegment_offset in gamemine.c" );
		if (cfread( &New_segment, mine_editor.newsegment_size,1,LoadFile )!=1)
			Error( "Error reading new_segment in gamemine.c" );
	}

	if ( (mine_fileinfo.newseg_verts_offset > -1) && (mine_fileinfo.newseg_verts_howmany > 0))
	{
		if (cfseek( LoadFile, mine_fileinfo.newseg_verts_offset, SEEK_SET ))
			Error( "Error seeking to newseg_verts_offset in gamemine.c" );
		for (i=0; i< mine_fileinfo.newseg_verts_howmany; i++ )
		{
			// Set the default values for this vertex
			Vertices[NEW_SEGMENT_VERTICES+i].x = 1;
			Vertices[NEW_SEGMENT_VERTICES+i].y = 1;
			Vertices[NEW_SEGMENT_VERTICES+i].z = 1;
			
			if (cfread( &Vertices[NEW_SEGMENT_VERTICES+i], mine_fileinfo.newseg_verts_sizeof,1,LoadFile )!=1)
				Error( "Error reading Vertices[NEW_SEGMENT_VERTICES+i] in gamemine.c" );

			New_segment.verts[i] = NEW_SEGMENT_VERTICES+i;
		}
	}

	#endif
															
	//========================= UPDATE VARIABLES ======================

	#ifdef EDITOR

	// Setting to Markedsegp to NULL ignores Curside and Markedside, which
	// we want to do when reading in an old file.
	
 	Markedside = mine_editor.Markedside;
	Curside = mine_editor.Curside;
	for (i=0;i<10;i++)
		Groupside[i] = mine_editor.Groupside[i];

	if ( mine_editor.current_seg != -1 )
		Cursegp = mine_editor.current_seg + Segments;
	else
 		Cursegp = NULL;

	if (mine_editor.Markedsegp != -1 ) 
		Markedsegp = mine_editor.Markedsegp + Segments;
	else
		Markedsegp = NULL;

	num_groups = 0;
	current_group = -1;

	#endif

	Num_vertices = mine_fileinfo.vertex_howmany;
	Num_segments = mine_fileinfo.segment_howmany;
	Highest_vertex_index = Num_vertices-1;
	Highest_segment_index = Num_segments-1;

	reset_objects(1);		//one object, the player

	#ifdef EDITOR
	Highest_vertex_index = MAX_SEGMENT_VERTICES-1;
	Highest_segment_index = MAX_SEGMENTS-1;
	set_vertex_counts();
	Highest_vertex_index = Num_vertices-1;
	Highest_segment_index = Num_segments-1;

	warn_if_concave_segments();
	#endif

	#ifdef EDITOR
		med_validate_segment_all();
	#endif

	create_local_segment_data();

	gamemine_find_textures();

	if (mine_top_fileinfo.fileinfo_version < MINE_VERSION )
		return 1;		//old version
	else
		return 0;

}

ubyte texcount[MAX_TEXTURES];

void gamemine_find_textures()
{
	int s, sn;
	int unique = 0;

	memset( texcount, 0, MAX_TEXTURES );

	for (s=0; s<=Highest_segment_index; s++)	{
		if (Segments[s].segnum != -1) {
			for (sn=0;sn<MAX_SIDES_PER_SEGMENT;sn++) {
				//if (Segments[s].sides[sn].wall_num > -1 ) {
				// mprintf( 0, "Segment %d, side %d has wall_num %d\n", s, sn, Segments[s].sides[sn].wall_num );
				//}
				if (Segments[s].sides[sn].tmap_num>-1) {
					//mprintf( 0, "Segment %d, side %d has tmap_num %d\n", s, sn, Segments[s].sides[sn].tmap_num );
					if (!texcount[Segments[s].sides[sn].tmap_num])	{
						piggy_read_bitmap_data(Textures[Segments[s].sides[sn].tmap_num]);
						unique++;
					}
					texcount[Segments[s].sides[sn].tmap_num]++;
				}
				if ((Segments[s].sides[sn].tmap_num2 & 0x3FFF)>0) {
					//mprintf( 0, "Segment %d, side %d has tmap_num2 %d\n", s, sn, Segments[s].sides[sn].tmap_num2 & 0x3FFF );
					if (!texcount[Segments[s].sides[sn].tmap_num2&0x3FFF])	{
						piggy_read_bitmap_data(Textures[Segments[s].sides[sn].tmap_num2&0x3FFF]);
						unique++;
					}
					texcount[Segments[s].sides[sn].tmap_num2&0x3FFF]++;
				}

			}
		}
	}	
	//mprintf( 0, "Mine used %d textures\n", unique );
}









