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
 * $Source: f:/miner/source/2d/rcs/bitblt.c $
 * $Revision: 1.29 $
 * $Author: john $
 * $Date: 1995/03/14 12:14:28 $
 *
 * Routines for bitblt's.
 *
 * $Log: bitblt.c $
 * Revision 1.29  1995/03/14  12:14:28  john
 * Added code to double horz/vert bitblts.
 * 
 * Revision 1.28  1995/03/13  09:01:48  john
 * Fixed bug with VFX1 screen not tall enough.
 * 
 * Revision 1.27  1995/03/01  15:38:10  john
 * Better ModeX support.
 * 
 * Revision 1.26  1994/12/15  12:19:00  john
 * Added gr_bm_bitblt (clipped!) function.
 * 
 * Revision 1.25  1994/12/09  18:58:42  matt
 * Took out include of 3d.h
 * 
 * Revision 1.24  1994/11/28  17:08:32  john
 * Took out some unused functions in linear.asm, moved
 * gr_linear_movsd from linear.asm to bitblt.c, made sure that
 * the code in ibiblt.c sets the direction flags before rep movsing.
 * 
 * Revision 1.22  1994/11/23  16:04:00  john
 * Fixed generic rle'ing to use new bit method.
 * 
 * Revision 1.21  1994/11/18  22:51:03  john
 * Changed a bunch of shorts to ints in calls.
 * 
 * Revision 1.20  1994/11/10  15:59:48  john
 * Fixed bugs with canvas's being created with bogus bm_flags.
 * 
 * Revision 1.19  1994/11/09  21:03:35  john
 * Added RLE for svga gr_ubitmap.
 * 
 * Revision 1.18  1994/11/09  17:41:29  john
 * Made a slow version of rle bitblt to svga, modex.
 * 
 * Revision 1.17  1994/11/09  16:35:15  john
 * First version with working RLE bitmaps.
 * 
 * Revision 1.16  1994/11/04  10:06:58  john
 * Added fade table for fading fonts. Made font that partially clips
 * not print a warning message.
 * 
 * Revision 1.15  1994/09/22  16:08:38  john
 * Fixed some palette stuff.
 * 
 * Revision 1.14  1994/09/19  11:44:27  john
 * Changed call to allocate selector to the dpmi module.
 * 
 * Revision 1.13  1994/08/08  13:03:00  john
 * Fixed bug in gr_bitmap in modex 
 * 
 * Revision 1.12  1994/07/13  19:47:23  john
 * Fixed bug with modex bitblt to page 2 not working.
 * 
 * Revision 1.11  1994/05/31  11:10:52  john
 * *** empty log message ***
 * 
 * Revision 1.10  1994/03/18  15:24:34  matt
 * Removed interlace stuff
 * 
 * Revision 1.9  1994/02/18  15:32:20  john
 * *** empty log message ***
 * 
 * Revision 1.8  1994/02/01  13:22:54  john
 * *** empty log message ***
 * 
 * Revision 1.7  1994/01/13  08:28:25  mike
 * Modify rect copy to copy alternate scanlines when in interlaced mode.
 * 
 * Revision 1.6  1993/12/28  12:09:46  john
 * added lbitblt.asm
 * 
 * Revision 1.5  1993/10/26  13:18:09  john
 * *** empty log message ***
 * 
 * Revision 1.4  1993/10/15  16:23:30  john
 * y
 * 
 * Revision 1.3  1993/09/13  17:52:58  john
 * Fixed bug in BitBlt linear to SVGA
 * 
 * Revision 1.2  1993/09/08  14:47:00  john
 * Made bitmap00 add rowsize instead of bitmap width.
 * Other routines might have this problem too.
 * 
 * Revision 1.1  1993/09/08  11:43:01  john
 * Initial revision
 * 
 *
 */

#include "mem.h"
#include "gr.h"
#include "grdef.h"
#include "rle.h"
#include "mono.h"

int gr_bitblt_dest_step_shift = 0;
int gr_bitblt_double = 0;
ubyte *gr_bitblt_fade_table=NULL;

extern void gr_vesa_bitmap( grs_bitmap * source, grs_bitmap * dest, int x, int y );

// This code aligns edi so that the destination is aligned to a dword boundry before rep movsd
void gr_linear_movsd(ubyte * src, ubyte * dest, int num_pixels );
#pragma aux gr_linear_movsd parm [esi] [edi] [ecx] modify exact [ecx esi edi eax ebx] = \
" cld "					\
" mov		ebx, ecx	"	\
" mov		eax, edi"	\
" and		eax, 011b"	\
" jz		d_aligned"	\
" mov		ecx, 4"		\
" sub		ecx, eax"	\
" sub		ebx, ecx"	\
" rep		movsb"		\
" d_aligned: "			\
" mov		ecx, ebx"	\
" shr		ecx, 2"		\
" rep 	movsd"		\
" mov		ecx, ebx"	\
" and 	ecx, 11b"	\
" rep 	movsb";

void gr_linear_rep_movsdm(ubyte * src, ubyte * dest, int num_pixels );
#pragma aux gr_linear_rep_movsdm parm [esi] [edi] [ecx] modify exact [ecx esi edi eax] = \
"nextpixel:"					\
	"mov	al,[esi]"			\
	"inc	esi"					\
	"cmp	al, 255"				\
	"je	skip_it"				\
	"mov	[edi], al"			\
"skip_it:"						\
	"inc	edi"					\
	"dec	ecx"					\
	"jne	nextpixel";

void gr_linear_rep_movsdm_faded(ubyte * src, ubyte * dest, int num_pixels, ubyte fade_value );
#pragma aux gr_linear_rep_movsdm_faded parm [esi] [edi] [ecx] [ebx] modify exact [ecx esi edi eax ebx] = \
"  xor eax, eax"	\
"  mov ah, bl"  \
"nextpixel:"					\
	"mov	al,[esi]"			\
	"inc	esi"					\
	"cmp	al, 255"				\
	"je	skip_it"				\
	"mov  al, gr_fade_table[eax]"	\
	"mov	[edi], al"			\
"skip_it:"						\
	"inc	edi"					\
	"dec	ecx"					\
	"jne	nextpixel";


void gr_linear_rep_movsd_2x(ubyte * src, ubyte * dest, int num_dest_pixels );
#pragma aux gr_linear_rep_movsd_2x parm [esi] [edi] [ecx] modify exact [ecx esi edi eax ebx] = \
	"shr	ecx, 1"				\
	"jnc	nextpixel"			\
	"mov	al, [esi]"			\
	"mov	[edi], al"			\
	"inc	esi"					\
	"inc	edi"					\
	"cmp	ecx, 0"				\
	"je	done"					\
"nextpixel:"					\
	"mov	al,[esi]"			\
	"mov	ah, al"				\
	"mov	[edi], ax"			\
	"inc	esi"					\
	"inc	edi"					\
	"inc	edi"					\
	"dec	ecx"					\
	"jne	nextpixel"			\
"done:"



void modex_copy_column(ubyte * src, ubyte * dest, int num_pixels, int src_rowsize, int dest_rowsize );
#pragma aux modex_copy_column parm [esi] [edi] [ecx] [ebx] [edx] modify exact [ecx esi edi] = \
"nextpixel:"							\
	"mov	al,[esi]"			\
	"add	esi, ebx"	\
	"mov	[edi], al"	\
	"add	edi, edx"	\
	"dec	ecx"			\
	"jne	nextpixel"	

void modex_copy_column_m(ubyte * src, ubyte * dest, int num_pixels, int src_rowsize, int dest_rowsize );
#pragma aux modex_copy_column_m parm [esi] [edi] [ecx] [ebx] [edx] modify exact [ecx esi edi] = \
"nextpixel:"							\
	"mov	al,[esi]"			\
	"add	esi, ebx"	\
	"cmp	al, 255"		\
	"je	skip_itx"		\
	"mov	[edi], al"	\
"skip_itx:"				\
	"add	edi, edx"	\
	"dec	ecx"			\
	"jne	nextpixel"	


void gr_ubitmap00( int x, int y, grs_bitmap *bm )
{
	register int y1;
	int dest_rowsize;

	unsigned char * dest;
	unsigned char * src;

	dest_rowsize=grd_curcanv->cv_bitmap.bm_rowsize << gr_bitblt_dest_step_shift;
	dest = &(grd_curcanv->cv_bitmap.bm_data[ dest_rowsize*y+x ]);

	src = bm->bm_data;

	for (y1=0; y1 < bm->bm_h; y1++ )    {
		if (gr_bitblt_double)
			gr_linear_rep_movsd_2x( src, dest, bm->bm_w );
		else
			gr_linear_movsd( src, dest, bm->bm_w );
		src += bm->bm_rowsize;
		dest+= (int)(dest_rowsize);
	}
}

void gr_ubitmap00m( int x, int y, grs_bitmap *bm )
{
	register int y1;
	int dest_rowsize;

	unsigned char * dest;
	unsigned char * src;

	dest_rowsize=grd_curcanv->cv_bitmap.bm_rowsize << gr_bitblt_dest_step_shift;
	dest = &(grd_curcanv->cv_bitmap.bm_data[ dest_rowsize*y+x ]);

	src = bm->bm_data;

	if (gr_bitblt_fade_table==NULL)	{
		for (y1=0; y1 < bm->bm_h; y1++ )    {
			gr_linear_rep_movsdm( src, dest, bm->bm_w );
			src += bm->bm_rowsize;
			dest+= (int)(dest_rowsize);
		}
	} else {
		for (y1=0; y1 < bm->bm_h; y1++ )    {
			gr_linear_rep_movsdm_faded( src, dest, bm->bm_w, gr_bitblt_fade_table[y1+y] );
			src += bm->bm_rowsize;
			dest+= (int)(dest_rowsize);
		}
	}
}

//"		jmp 	aligned4				"	\
//"		mov	eax, edi				"	\
//"		and	eax, 11b				"	\
//"		jz		aligned4				"	\
//"		mov	ebx, 4				"	\
//"		sub	ebx, eax				"	\
//"		sub	ecx, ebx				"	\
//"alignstart:						"	\
//"		mov	al, [esi]			"	\
//"		add	esi, 4				"	\
//"		mov	[edi], al			"	\
//"		inc	edi					"	\
//"		dec	ebx					"	\
//"		jne	alignstart			"	\
//"aligned4:							"	\

void modex_copy_scanline( ubyte * src, ubyte * dest, int npixels );
#pragma aux modex_copy_scanline parm [esi] [edi] [ecx] modify exact [ecx esi edi eax ebx edx] = \
"		mov	ebx, ecx				"	\
"		and	ebx, 11b				"	\
"		shr	ecx, 2				"	\
"		cmp	ecx, 0				"	\
"		je		no2group				"	\
"next4pixels:						"	\
"		mov	al, [esi+8]			"	\
"		mov	ah, [esi+12]		"	\
"		shl	eax, 16				"	\
"		mov	al, [esi]			"	\
"		mov	ah, [esi+4]			"	\
"		mov	[edi], eax			"	\
"		add	esi, 16				"	\
"		add	edi, 4				"	\
"		dec	ecx					"	\
"		jne	next4pixels			"	\
"no2group:							"	\
"		cmp	ebx, 0				"	\
"		je		done2					"	\
"finishend:							"	\
"		mov	al, [esi]			"	\
"		add	esi, 4				"	\
"		mov	[edi], al			"	\
"		inc	edi					"	\
"		dec	ebx					"	\
"		jne	finishend			"	\
"done2:								";

void modex_copy_scanline_2x( ubyte * src, ubyte * dest, int npixels );
#pragma aux modex_copy_scanline_2x parm [esi] [edi] [ecx] modify exact [ecx esi edi eax ebx edx] = \
"		mov	ebx, ecx				"	\
"		and	ebx, 11b				"	\
"		shr	ecx, 2				"	\
"		cmp	ecx, 0				"	\
"		je		no2group				"	\
"next4pixels:						"	\
"		mov	al, [esi+4]			"	\
"		mov	ah, [esi+6]			"	\
"		shl	eax, 16				"	\
"		mov	al, [esi]			"	\
"		mov	ah, [esi+2]			"	\
"		mov	[edi], eax			"	\
"		add	esi, 8				"	\
"		add	edi, 4				"	\
"		dec	ecx					"	\
"		jne	next4pixels			"	\
"no2group:							"	\
"		cmp	ebx, 0				"	\
"		je		done2					"	\
"finishend:							"	\
"		mov	al, [esi]			"	\
"		add	esi, 2				"	\
"		mov	[edi], al			"	\
"		inc	edi					"	\
"		dec	ebx					"	\
"		jne	finishend			"	\
"done2:								";



// From Linear to ModeX
void gr_bm_ubitblt01(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	ubyte * dbits;
	ubyte * sbits;
	int sstep,dstep;
	int y,plane;
	int w1;

	if ( w < 4 ) return;

	sstep = src->bm_rowsize;
	dstep = dest->bm_rowsize << gr_bitblt_dest_step_shift;

	if (!gr_bitblt_double)	{
		for (plane=0; plane<4; plane++ )	{
			gr_modex_setplane( (plane+dx)&3 );
			sbits = src->bm_data + (src->bm_rowsize * sy) + sx + plane;
			dbits = &gr_video_memory[(dest->bm_rowsize * dy) + ((plane+dx)/4) ];
			w1 = w >> 2;
			if ( (w&3) > plane ) w1++;
			for (y=dy; y < dy+h; y++ )		{
				modex_copy_scanline( sbits, dbits, w1 );		
				dbits += dstep;
				sbits += sstep;
			}
		}
	} else {
		for (plane=0; plane<4; plane++ )	{
			gr_modex_setplane( (plane+dx)&3 );
			sbits = src->bm_data + (src->bm_rowsize * sy) + sx + plane/2;
			dbits = &gr_video_memory[(dest->bm_rowsize * dy) + ((plane+dx)/4) ];
			w1 = w >> 2;
			if ( (w&3) > plane ) w1++;
			for (y=dy; y < dy+h; y++ )		{
				modex_copy_scanline_2x( sbits, dbits, w1 );		
				dbits += dstep;
				sbits += sstep;
			}
		}
	}
}


// From Linear to ModeX masked
void gr_bm_ubitblt01m(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	//ubyte * dbits1;
	//ubyte * sbits1;
	
	ubyte * dbits;
	ubyte * sbits;

	int x;
//	int y;

	sbits =   src->bm_data  + (src->bm_rowsize * sy) + sx;
	dbits =   &gr_video_memory[(dest->bm_rowsize * dy) + dx/4];

	for (x=dx; x < dx+w; x++ )	{	
		gr_modex_setplane( x&3 );

		//sbits1 = sbits;
		//dbits1 = dbits;
		//for (y=0; y < h; y++ )    {
		//	*dbits1 = *sbits1;
		//	sbits1 += src_bm_rowsize;
		//	dbits1 += dest_bm_rowsize;
		//	}
		modex_copy_column_m(sbits, dbits, h, src->bm_rowsize, dest->bm_rowsize << gr_bitblt_dest_step_shift );

		sbits++;
		if ( (x&3)==3 )
			dbits++;
	}
}




void gr_ubitmap012( int x, int y, grs_bitmap *bm )
{
	register int x1, y1;
	unsigned char * src;

	src = bm->bm_data;

	for (y1=y; y1 < (y+bm->bm_h); y1++ )    {
		for (x1=x; x1 < (x+bm->bm_w); x1++ )    {
			gr_setcolor( *src++ );
			gr_upixel( x1, y1 );
		}
	}
}

void gr_ubitmap012m( int x, int y, grs_bitmap *bm )
{
	register int x1, y1;
	unsigned char * src;

	src = bm->bm_data;

	for (y1=y; y1 < (y+bm->bm_h); y1++ )    {
		for (x1=x; x1 < (x+bm->bm_w); x1++ )    {
			if ( *src != 255 )	{
				gr_setcolor( *src );
				gr_upixel( x1, y1 );
			}
			src++;
		}
	}
}


void gr_ubitmapGENERIC(int x, int y, grs_bitmap * bm)
{
	register int x1, y1;

	for (y1=0; y1 < bm->bm_h; y1++ )    {
		for (x1=0; x1 < bm->bm_w; x1++ )    {
			gr_setcolor( gr_gpixel(bm,x1,y1) );
			gr_upixel( x+x1, y+y1 );
		}
	}
}

void gr_ubitmapGENERICm(int x, int y, grs_bitmap * bm)
{
	register int x1, y1;
	ubyte c;

	for (y1=0; y1 < bm->bm_h; y1++ )    {
		for (x1=0; x1 < bm->bm_w; x1++ )    {
			c = gr_gpixel(bm,x1,y1);
			if ( c != 255 )	{
				gr_setcolor( c );
				gr_upixel( x+x1, y+y1 );
			}
		}
	}
}


void gr_ubitmap( int x, int y, grs_bitmap *bm )
{   int source, dest;

	source = bm->bm_type;
	dest = TYPE;

	if (source==BM_LINEAR) {
		switch( dest )
		{
		case BM_LINEAR:
			if ( bm->bm_flags & BM_FLAG_RLE )
				gr_bm_ubitblt00_rle(bm->bm_w, bm->bm_h, x, y, 0, 0, bm, &grd_curcanv->cv_bitmap );
			else
				gr_ubitmap00( x, y, bm );
			return;
		case BM_SVGA:
			if ( bm->bm_flags & BM_FLAG_RLE )
				gr_bm_ubitblt0x_rle(bm->bm_w, bm->bm_h, x, y, 0, 0, bm, &grd_curcanv->cv_bitmap );
			else
				gr_vesa_bitmap( bm, &grd_curcanv->cv_bitmap, x, y );
			return;
		case BM_MODEX:
			gr_bm_ubitblt01(bm->bm_w, bm->bm_h, x+XOFFSET, y+YOFFSET, 0, 0, bm, &grd_curcanv->cv_bitmap);
			return;
		default:
			gr_ubitmap012( x, y, bm );
			return;
		}
	} else  {
		gr_ubitmapGENERIC(x, y, bm);
	}
}

void gr_ubitmapm( int x, int y, grs_bitmap *bm )
{   int source, dest;

	source = bm->bm_type;
	dest = TYPE;

	if (source==BM_LINEAR) {
		switch( dest )
		{
		case BM_LINEAR:
			if ( bm->bm_flags & BM_FLAG_RLE )
				gr_bm_ubitblt00m_rle(bm->bm_w, bm->bm_h, x, y, 0, 0, bm, &grd_curcanv->cv_bitmap );
			else
				gr_ubitmap00m( x, y, bm );
			return;
		case BM_SVGA:
			gr_ubitmapGENERICm(x, y, bm);
			return;
		case BM_MODEX:
			gr_bm_ubitblt01m(bm->bm_w, bm->bm_h, x+XOFFSET, y+YOFFSET, 0, 0, bm, &grd_curcanv->cv_bitmap);
			return;
		default:
			gr_ubitmap012m( x, y, bm );
			return;
		}
	} else  {
		gr_ubitmapGENERICm(x, y, bm);
	}
}



// From linear to SVGA
void gr_bm_ubitblt02(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	unsigned char * sbits;

	unsigned int offset, EndingOffset, VideoLocation;

	int sbpr, dbpr, y1, page, BytesToMove;

	sbpr = src->bm_rowsize;

	dbpr = dest->bm_rowsize << gr_bitblt_dest_step_shift;

	VideoLocation = (unsigned int)dest->bm_data + (dest->bm_rowsize * dy) + dx;

	sbits = src->bm_data + ( sbpr*sy ) + sx;

	for (y1=0; y1 < h; y1++ )    {

		page    = VideoLocation >> 16;
		offset  = VideoLocation & 0xFFFF;

		gr_vesa_setpage( page );

		EndingOffset = offset+w-1;

		if ( EndingOffset <= 0xFFFF )
		{
			if ( gr_bitblt_double )
				gr_linear_rep_movsd_2x( (void *)sbits, (void *)(offset+0xA0000), w );
			else
				gr_linear_movsd( (void *)sbits, (void *)(offset+0xA0000), w );

			VideoLocation += dbpr;
			sbits += sbpr;
		}
		else
		{
			BytesToMove = 0xFFFF-offset+1;

			if ( gr_bitblt_double )
				gr_linear_rep_movsd_2x( (void *)sbits, (void *)(offset+0xA0000), BytesToMove );
			else
				gr_linear_movsd( (void *)sbits, (void *)(offset+0xA0000), BytesToMove );

			page++;
			gr_vesa_setpage(page);

			if ( gr_bitblt_double )
				gr_linear_rep_movsd_2x( (void *)(sbits+BytesToMove/2), (void *)0xA0000, EndingOffset - 0xFFFF );
			else
				gr_linear_movsd( (void *)(sbits+BytesToMove), (void *)0xA0000, EndingOffset - 0xFFFF );

			VideoLocation += dbpr;
			sbits += sbpr;
		}
	}
}

// From SVGA to linear
void gr_bm_ubitblt20(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	unsigned char * dbits;

	unsigned int offset, offset1, offset2;

	int sbpr, dbpr, y1, page;

	dbpr = dest->bm_rowsize;

	sbpr = src->bm_rowsize;

	for (y1=0; y1 < h; y1++ )    {

		offset2 =   (unsigned int)src->bm_data  + (sbpr * (y1+sy)) + sx;
		dbits   =   dest->bm_data + (dbpr * (y1+dy)) + dx;

		page = offset2 >> 16;
		offset = offset2 & 0xFFFF;
		offset1 = offset+w-1;
		gr_vesa_setpage( page );

		if ( offset1 > 0xFFFF )  {
			// Overlaps two pages
			while( offset <= 0xFFFF )
				*dbits++ = gr_video_memory[offset++];
			offset1 -= (0xFFFF+1);
			offset = 0;
			page++;
			gr_vesa_setpage(page);
		}
		while( offset <= offset1 )
			*dbits++ = gr_video_memory[offset++];

	}
}

//@extern int Interlacing_on;

// From Linear to Linear
void gr_bm_ubitblt00(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	unsigned char * dbits;
	unsigned char * sbits;
	//int	src_bm_rowsize_2, dest_bm_rowsize_2;
	int dstep;

	int i;

	sbits =   src->bm_data  + (src->bm_rowsize * sy) + sx;
	dbits =   dest->bm_data + (dest->bm_rowsize * dy) + dx;

	dstep = dest->bm_rowsize << gr_bitblt_dest_step_shift;

	// No interlacing, copy the whole buffer.
	for (i=0; i < h; i++ )    {
		if (gr_bitblt_double)
			gr_linear_rep_movsd_2x( sbits, dbits, w );
		else
			gr_linear_movsd( sbits, dbits, w );
		sbits += src->bm_rowsize;
		dbits += dstep;
	}
}
// From Linear to Linear Masked
void gr_bm_ubitblt00m(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	unsigned char * dbits;
	unsigned char * sbits;
	//int	src_bm_rowsize_2, dest_bm_rowsize_2;

	int i;

	sbits =   src->bm_data  + (src->bm_rowsize * sy) + sx;
	dbits =   dest->bm_data + (dest->bm_rowsize * dy) + dx;

	// No interlacing, copy the whole buffer.

	if (gr_bitblt_fade_table==NULL)	{
		for (i=0; i < h; i++ )    {
			gr_linear_rep_movsdm( sbits, dbits, w );
			sbits += src->bm_rowsize;
			dbits += dest->bm_rowsize;
		}
	} else {
		for (i=0; i < h; i++ )    {
			gr_linear_rep_movsdm_faded( sbits, dbits, w, gr_bitblt_fade_table[dy+i] );
			sbits += src->bm_rowsize;
			dbits += dest->bm_rowsize;
		}
	}
}


extern void gr_lbitblt( grs_bitmap * source, grs_bitmap * dest, int height, int width );

void gr_bm_bitblt(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	int dx1=dx, dx2=dx+dest->bm_w-1;
	int dy1=dy, dy2=dy+dest->bm_h-1;

	int sx1=sx, sx2=sx+src->bm_w-1;
	int sy1=sy, sy2=sy+src->bm_h-1;

	if ((dx1 >= dest->bm_w ) || (dx2 < 0)) return;
	if ((dy1 >= dest->bm_h ) || (dy2 < 0)) return;
	if ( dx1 < 0 ) { sx1 += -dx1; dx1 = 0; }
	if ( dy1 < 0 ) { sy1 += -dy1; dy1 = 0; }
	if ( dx2 >= dest->bm_w )	{ dx2 = dest->bm_w-1; }
	if ( dy2 >= dest->bm_h )	{ dy2 = dest->bm_h-1; }

	if ((sx1 >= src->bm_w ) || (sx2 < 0)) return;
	if ((sy1 >= src->bm_h ) || (sy2 < 0)) return;
	if ( sx1 < 0 ) { dx1 += -sx1; sx1 = 0; }
	if ( sy1 < 0 ) { dy1 += -sy1; sy1 = 0; }
	if ( sx2 >= src->bm_w )	{ sx2 = src->bm_w-1; }
	if ( sy2 >= src->bm_h )	{ sy2 = src->bm_h-1; }

	// Draw bitmap bm[x,y] into (dx1,dy1)-(dx2,dy2)
	if ( dx2-dx1+1 < w )
		w = dx2-dx1+1;
	if ( dy2-dy1+1 < h )
		h = dy2-dy1+1;
	if ( sx2-sx1+1 < w )
		w = sx2-sx1+1;
	if ( sy2-sy1+1 < h )
		h = sy2-sy1+1;

	gr_bm_ubitblt(w,h, dx1, dy1, sx1, sy1, src, dest );
}

void gr_bm_ubitblt(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	register int x1, y1;

	if ( (src->bm_type == BM_LINEAR) && (dest->bm_type == BM_LINEAR ))
	{
		if ( src->bm_flags & BM_FLAG_RLE )	
			gr_bm_ubitblt00_rle( w, h, dx, dy, sx, sy, src, dest );
		else
			gr_bm_ubitblt00( w, h, dx, dy, sx, sy, src, dest );
		return;
	}

	if ( (src->bm_flags & BM_FLAG_RLE ) && (src->bm_type == BM_LINEAR) )	{
		gr_bm_ubitblt0x_rle(w, h, dx, dy, sx, sy, src, dest );
	 	return;
	}

	if ( (src->bm_type == BM_LINEAR) && (dest->bm_type == BM_SVGA ))
	{
		gr_bm_ubitblt02( w, h, dx, dy, sx, sy, src, dest );
		return;
	}

	if ( (src->bm_type == BM_SVGA) && (dest->bm_type == BM_LINEAR ))
	{
		gr_bm_ubitblt20( w, h, dx, dy, sx, sy, src, dest );
		return;
	}

	if ( (src->bm_type == BM_LINEAR) && (dest->bm_type == BM_MODEX ))
	{
		gr_bm_ubitblt01( w, h, dx+XOFFSET, dy+YOFFSET, sx, sy, src, dest );
		return;
	}

	for (y1=0; y1 < h; y1++ )    {
		for (x1=0; x1 < w; x1++ )    {
			gr_bm_pixel( dest, dx+x1, dy+y1, gr_gpixel(src,sx+x1,sy+y1) );
		}
	}
}

// Clipped bitmap ... 

void gr_bitmap( int x, int y, grs_bitmap *bm )
{
	int dx1=x, dx2=x+bm->bm_w-1;
	int dy1=y, dy2=y+bm->bm_h-1;
	int sx=0, sy=0;

	if ((dx1 >= grd_curcanv->cv_bitmap.bm_w ) || (dx2 < 0)) return;
	if ((dy1 >= grd_curcanv->cv_bitmap.bm_h) || (dy2 < 0)) return;
	if ( dx1 < 0 ) { sx = -dx1; dx1 = 0; }
	if ( dy1 < 0 ) { sy = -dy1; dy1 = 0; }
	if ( dx2 >= grd_curcanv->cv_bitmap.bm_w )	{ dx2 = grd_curcanv->cv_bitmap.bm_w-1; }
	if ( dy2 >= grd_curcanv->cv_bitmap.bm_h )	{ dy2 = grd_curcanv->cv_bitmap.bm_h-1; }
		
	// Draw bitmap bm[x,y] into (dx1,dy1)-(dx2,dy2)

	gr_bm_ubitblt(dx2-dx1+1,dy2-dy1+1, dx1, dy1, sx, sy, bm, &grd_curcanv->cv_bitmap );

}

void gr_bitmapm( int x, int y, grs_bitmap *bm )
{
	int dx1=x, dx2=x+bm->bm_w-1;
	int dy1=y, dy2=y+bm->bm_h-1;
	int sx=0, sy=0;

	if ((dx1 >= grd_curcanv->cv_bitmap.bm_w ) || (dx2 < 0)) return;
	if ((dy1 >= grd_curcanv->cv_bitmap.bm_h) || (dy2 < 0)) return;
	if ( dx1 < 0 ) { sx = -dx1; dx1 = 0; }
	if ( dy1 < 0 ) { sy = -dy1; dy1 = 0; }
	if ( dx2 >= grd_curcanv->cv_bitmap.bm_w )	{ dx2 = grd_curcanv->cv_bitmap.bm_w-1; }
	if ( dy2 >= grd_curcanv->cv_bitmap.bm_h )	{ dy2 = grd_curcanv->cv_bitmap.bm_h-1; }
		
	// Draw bitmap bm[x,y] into (dx1,dy1)-(dx2,dy2)

	if ( (bm->bm_type == BM_LINEAR) && (grd_curcanv->cv_bitmap.bm_type == BM_LINEAR ))
	{
		if ( bm->bm_flags & BM_FLAG_RLE )	
			gr_bm_ubitblt00m_rle(dx2-dx1+1,dy2-dy1+1, dx1, dy1, sx, sy, bm, &grd_curcanv->cv_bitmap );
		else
			gr_bm_ubitblt00m(dx2-dx1+1,dy2-dy1+1, dx1, dy1, sx, sy, bm, &grd_curcanv->cv_bitmap );
		return;
	}

	gr_bm_ubitbltm(dx2-dx1+1,dy2-dy1+1, dx1, dy1, sx, sy, bm, &grd_curcanv->cv_bitmap );

}

void gr_bm_ubitbltm(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	register int x1, y1;
	ubyte c;

	for (y1=0; y1 < h; y1++ )    {
		for (x1=0; x1 < w; x1++ )    {
			if ((c=gr_gpixel(src,sx+x1,sy+y1))!=255)
				gr_bm_pixel( dest, dx+x1, dy+y1,c  );
		}
	}

}

//-NOT-used // From linear to SVGA
//-NOT-used void gr_bm_ubitblt02_2x(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
//-NOT-used {
//-NOT-used 	unsigned char * sbits;
//-NOT-used 
//-NOT-used 	unsigned int offset, EndingOffset, VideoLocation;
//-NOT-used 
//-NOT-used 	int sbpr, dbpr, y1, page, BytesToMove;
//-NOT-used 
//-NOT-used 	sbpr = src->bm_rowsize;
//-NOT-used 
//-NOT-used 	dbpr = dest->bm_rowsize << gr_bitblt_dest_step_shift;
//-NOT-used 
//-NOT-used 	VideoLocation = (unsigned int)dest->bm_data + (dest->bm_rowsize * dy) + dx;
//-NOT-used 
//-NOT-used 	sbits = src->bm_data + ( sbpr*sy ) + sx;
//-NOT-used 
//-NOT-used 	for (y1=0; y1 < h; y1++ )    {
//-NOT-used 
//-NOT-used 		page    = VideoLocation >> 16;
//-NOT-used 		offset  = VideoLocation & 0xFFFF;
//-NOT-used 
//-NOT-used 		gr_vesa_setpage( page );
//-NOT-used 
//-NOT-used 		EndingOffset = offset+w-1;
//-NOT-used 
//-NOT-used 		if ( EndingOffset <= 0xFFFF )
//-NOT-used 		{
//-NOT-used 			gr_linear_rep_movsd_2x( (void *)sbits, (void *)(offset+0xA0000), w );
//-NOT-used 
//-NOT-used 			VideoLocation += dbpr;
//-NOT-used 			sbits += sbpr;
//-NOT-used 		}
//-NOT-used 		else
//-NOT-used 		{
//-NOT-used 			BytesToMove = 0xFFFF-offset+1;
//-NOT-used 
//-NOT-used 			gr_linear_rep_movsd_2x( (void *)sbits, (void *)(offset+0xA0000), BytesToMove );
//-NOT-used 
//-NOT-used 			page++;
//-NOT-used 			gr_vesa_setpage(page);
//-NOT-used 
//-NOT-used 			gr_linear_rep_movsd_2x( (void *)(sbits+BytesToMove/2), (void *)0xA0000, EndingOffset - 0xFFFF );
//-NOT-used 
//-NOT-used 			VideoLocation += dbpr;
//-NOT-used 			sbits += sbpr;
//-NOT-used 		}
//-NOT-used 
//-NOT-used 
//-NOT-used 	}
//-NOT-used }


//-NOT-used // From Linear to Linear
//-NOT-used void gr_bm_ubitblt00_2x(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
//-NOT-used {
//-NOT-used 	unsigned char * dbits;
//-NOT-used 	unsigned char * sbits;
//-NOT-used 	//int	src_bm_rowsize_2, dest_bm_rowsize_2;
//-NOT-used 
//-NOT-used 	int i;
//-NOT-used 
//-NOT-used 	sbits =   src->bm_data  + (src->bm_rowsize * sy) + sx;
//-NOT-used 	dbits =   dest->bm_data + (dest->bm_rowsize * dy) + dx;
//-NOT-used 
//-NOT-used 	// No interlacing, copy the whole buffer.
//-NOT-used 	for (i=0; i < h; i++ )    {
//-NOT-used 		gr_linear_rep_movsd_2x( sbits, dbits, w );
//-NOT-used 
//-NOT-used 		sbits += src->bm_rowsize;
//-NOT-used 		dbits += dest->bm_rowsize << gr_bitblt_dest_step_shift;
//-NOT-used 	}
//-NOT-used }

void gr_bm_ubitblt00_rle(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	unsigned char * dbits;
	unsigned char * sbits;

	int i;

	sbits = &src->bm_data[4 + src->bm_h];
	for (i=0; i<sy; i++ )
		sbits += (int)src->bm_data[4+i];

	dbits = dest->bm_data + (dest->bm_rowsize * dy) + dx;

	// No interlacing, copy the whole buffer.
	for (i=0; i < h; i++ )    {
		gr_rle_expand_scanline( dbits, sbits, sx, sx+w-1 );
		sbits += (int)src->bm_data[4+i+sy];
		dbits += dest->bm_rowsize << gr_bitblt_dest_step_shift;
	}
}

void gr_bm_ubitblt00m_rle(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	unsigned char * dbits;
	unsigned char * sbits;

	int i;

	sbits = &src->bm_data[4 + src->bm_h];
	for (i=0; i<sy; i++ )
		sbits += (int)src->bm_data[4+i];

	dbits = dest->bm_data + (dest->bm_rowsize * dy) + dx;

	// No interlacing, copy the whole buffer.
	for (i=0; i < h; i++ )    {
		gr_rle_expand_scanline_masked( dbits, sbits, sx, sx+w-1 );
		sbits += (int)src->bm_data[4+i+sy];
		dbits += dest->bm_rowsize << gr_bitblt_dest_step_shift;
	}
}

// in rle.c

extern void gr_rle_expand_scanline_generic( grs_bitmap * dest, int dx, int dy, ubyte *src, int x1, int x2  );


void gr_bm_ubitblt0x_rle(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
{
	int i;
	register int y1;
	unsigned char * sbits;

	//mprintf( 0, "SVGA RLE!\n" );

	sbits = &src->bm_data[4 + src->bm_h];
	for (i=0; i<sy; i++ )
		sbits += (int)src->bm_data[4+i];

	for (y1=0; y1 < h; y1++ )    {
		gr_rle_expand_scanline_generic( dest, dx, dy+y1,  sbits, sx, sx+w-1  );
		sbits += (int)src->bm_data[4+y1+sy];
	}

}



