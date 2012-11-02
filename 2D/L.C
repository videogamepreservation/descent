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

// From linear to SVGA
void gr_bm_ubitblt02_2x(int w, int h, int dx, int dy, int sx, int sy, grs_bitmap * src, grs_bitmap * dest)
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
			gr_linear_rep_movsd_2x( (void *)sbits, (void *)(offset+0xA0000), w );

			VideoLocation += dbpr;
			sbits += sbpr;
		}
		else
		{
			BytesToMove = 0xFFFF-offset+1;

			gr_linear_rep_movsd_2x( (void *)sbits, (void *)(offset+0xA0000), BytesToMove );

			page++;
			gr_vesa_setpage(page);

			gr_linear_rep_movsd_2x( (void *)(sbits+BytesToMove/2), (void *)0xA0000, EndingOffset - 0xFFFF );

			VideoLocation += dbpr;
			sbits += sbpr;
		}


	}
}


