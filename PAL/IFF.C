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
		routines to load an IFF picture into memory

*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>

#include "iff.h"

#define MIN(a,b) ((a<b)?a:b)

#define MAKE_SIG(a,b,c,d) (((long)(a)<<24)+((long)(b)<<16)+((c)<<8)+(d))

#define form_sig MAKE_SIG('F','O','R','M')
#define ilbm_sig MAKE_SIG('I','L','B','M')
#define body_sig MAKE_SIG('B','O','D','Y')
#define pbm_sig MAKE_SIG('P','B','M',' ')
#define bmhd_sig MAKE_SIG('B','M','H','D')
#define cmap_sig MAKE_SIG('C','M','A','P')

void printsig(long s)
{
	char *t=(char *) &s;

/*  printf("%c%c%c%c",*(&s+3),*(&s+2),*(&s+1),s);*/
	printf("%c%c%c%c",t[3],t[2],t[1],t[0]);
}

long get_sig(FILE *f)
{
	char s[4];

	if ((s[3]=getc(f))==EOF) return(EOF);
	if ((s[2]=getc(f))==EOF) return(EOF);
	if ((s[1]=getc(f))==EOF) return(EOF);
	if ((s[0]=getc(f))==EOF) return(EOF);

	return(*((long *) s));
}


int get_word(FILE *f)
{
	unsigned char c0,c1;

	c1=getc(f);
	c0=getc(f);

	if (c0==0xff) return(EOF);

	return(((int)c1<<8) + c0);

}

char get_byte(FILE *f)
{
	return getc(f);
}

long get_long(FILE *f)
{
	unsigned char c0,c1,c2,c3;

	c3=getc(f);
	c2=getc(f);
	c1=getc(f);
	c0=getc(f);

//printf("get_long %x %x %x %x\n",c3,c2,c1,c0);

//  if (c0==0xff) return(EOF);

	return(((long)c3<<24) + ((long)c2<<16) + ((long)c1<<8) + c0);

}

void parse_bmhd(FILE *ifile,long len,struct bitmap_header *bitmap_header)
{
	len++;  /* so no "parm not used" warning */

//  debug("parsing bmhd len=%ld\n",len);

	bitmap_header->w = get_word(ifile);
	bitmap_header->h = get_word(ifile);
	bitmap_header->x = get_word(ifile);
	bitmap_header->y = get_word(ifile);

	bitmap_header->nplanes = get_byte(ifile);
	bitmap_header->masking = get_byte(ifile);
	bitmap_header->compression = get_byte(ifile);
	get_byte(ifile);        /* skip pad */

	bitmap_header->transparentcolor = get_word(ifile);
	bitmap_header->xaspect = get_byte(ifile);
	bitmap_header->yaspect = get_byte(ifile);

	bitmap_header->pagewidth = get_word(ifile);
	bitmap_header->pageheight = get_word(ifile);

//  debug("w,h=%d,%d x,y=%d,%d\n",w,h,x,y);
//  debug("nplanes=%d, masking=%d ,compression=%d, transcolor=%d\n",nplanes,masking,compression,transparentcolor);

}


//  the buffer pointed to by raw_data is stuffed with a pointer to decompressed pixel data
int parse_body_pbm(FILE *ifile,long len,struct bitmap_header *bitmap_header)
{
	unsigned char  *p=bitmap_header->raw_data;
	int width=bitmap_header->w;
	long cnt,old_cnt;
	signed char n;
	int nn,wid_cnt;
	char ignore;

	if (bitmap_header->compression == cmpNone) {        /* no compression */
		int x,y;

		for (y=bitmap_header->h;y;y--) {
			for (x=bitmap_header->w;x;x--) *p++=getc(ifile);
			if (bitmap_header->w & 1) ignore = getc(ifile);
		}

	}
	else if (bitmap_header->compression == cmpByteRun1)
		for (old_cnt=cnt=len,wid_cnt=width;cnt>0;) {
			unsigned char c;

			if (old_cnt-cnt > 2048) {
//              printf(".");
				old_cnt=cnt;
			}

			if (wid_cnt <= 0) wid_cnt = width;

			n=getc(ifile);
			if (n >= 0) {                       // copy next n+1 bytes from source, they are not compressed
				nn = (int) n+1;
				cnt -= nn+1;
				wid_cnt -= nn;
				if (wid_cnt==-1) --nn;
				while (nn--) *p++=getc(ifile);
				if (wid_cnt==-1) ignore = getc(ifile);      /* extra char */
			}
			else if (n>=-127) {             // next -n + 1 bytes are following byte
				cnt -= 2;
				c=getc(ifile);
				nn = (int) -n+1;
				wid_cnt -= nn;
				if (wid_cnt==-1) --nn;
				while (nn--) *p++=c;
			}

		}

	if (len & 1) ignore = getc(ifile);

	if (ignore) ignore++;   // haha, suppress the evil warning message

	return IFF_NO_ERROR;
}

//  the buffer pointed to by raw_data is stuffed with a pointer to bitplane pixel data
int parse_body_ilbm(FILE *ifile,long len,struct bitmap_header *bitmap_header)
{
	unsigned char  *p=bitmap_header->raw_data;
	int width=bitmap_header->w;
	long cnt,old_cnt;
	signed char n;
	int nn,wid_cnt;
	char    ignore;

	if (bitmap_header->compression == cmpNone) {        /* no compression */
		int x,y;

		for (y=bitmap_header->h;y;y--) {
			for (x=bitmap_header->w;x;x--) *p++=getc(ifile);
			if (bitmap_header->w & 1) ignore = getc(ifile);
		}

	}
	else if (bitmap_header->compression == cmpByteRun1)
		for (old_cnt=cnt=len,wid_cnt=width;cnt>0;) {
			unsigned char c;

			if (old_cnt-cnt > 2048) {
//              printf(".");
				old_cnt=cnt;
			}

			if (wid_cnt <= 0) wid_cnt = width;

			n=getc(ifile);

			if (n >= 0) {                       // copy next n+1 bytes from source, they are not compressed
				nn = (int) n+1;
				cnt -= nn+1;
				wid_cnt -= nn;
				if (wid_cnt==-1) --nn;
				while (nn--) *p++=getc(ifile);
				if (wid_cnt==-1) ignore = getc(ifile);      /* extra char */
			}
			else if (n>=-127) {             // next -n + 1 bytes are following byte
				cnt -= 2;
				c=getc(ifile);
				nn = (int) -n+1;
				wid_cnt -= nn;
				if (wid_cnt==-1) --nn;
				while (nn--) *p++=c;
			}

		}

	if (len & 1) ignore = getc(ifile);

	if (ignore) ignore++;   // haha, suppress the evil warning message

	return IFF_NO_ERROR;
}

void skip_chunk(FILE *ifile,long len)
{
	len = len+1 & ~1;
	fseek(ifile,len,SEEK_CUR);
}

// Pass pointer to opened file, and to empty bitmap header.
int parse_iff(FILE *ifile,struct bitmap_header *bitmap_header)
{
	long sig,form_len,len,form_type;
	char    ignore;

	sig=get_sig(ifile);

//  printsig(sig);

	if (sig==form_sig) {

		form_len = get_long(ifile);
		form_len++;     /* get rid of never used message */

		form_type = get_sig(ifile);

//      printf(" %ld ",form_len);
//      printsig(form_type);
//      printf("\n");

		if ((form_type == pbm_sig) || (form_type == ilbm_sig)) {

			if (form_type == pbm_sig)
				bitmap_header->type = PBM_TYPE;
			else
				bitmap_header->type = ILBM_TYPE;

			while ((sig=get_sig(ifile)) != EOF) {

				len=get_long(ifile);

//              printf(" ");
//              printsig(sig);
//              printf(" %ld\n",len);

				switch (sig) {

					case bmhd_sig:

						parse_bmhd(ifile,len,bitmap_header);

						if (! (bitmap_header->raw_data = malloc((long) bitmap_header->w * bitmap_header->h*3))) exit(1);

						break;

					case cmap_sig:
					{
						int ncolors=(int) (len/3),cnum;
						unsigned char r,g,b;

						for (cnum=0;cnum<ncolors;cnum++) {
							r=getc(ifile);
							g=getc(ifile);
							b=getc(ifile);
							r >>= 2; bitmap_header->palette[cnum].r = r;
							g >>= 2; bitmap_header->palette[cnum].g = g;
							b >>= 2; bitmap_header->palette[cnum].b = b;
						}
						if (len & 1) ignore = getc(ifile);

						break;
					}

					case body_sig:
					{
						int r;
						switch (form_type) {
							case pbm_sig:
								if (!(r=parse_body_pbm(ifile,len,bitmap_header))) return r;
								break;
							case ilbm_sig:
								if (!(r=parse_body_ilbm(ifile,len,bitmap_header))) return r;
								break;
						}
						break;
					}
					default:
						skip_chunk(ifile,len);
						break;
				}
			}
		}
		else return IFF_UNKNOWN_FORM;
	}
	else
		{printf("Not an IFF file\n"); return IFF_NOT_IFF;}

	if (ignore) ignore++;

	return IFF_NO_ERROR;    /* ok! */
}

