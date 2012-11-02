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
void c_tmap_scanline_per()
{
	ubyte *dest;
	uint c;
	int x;
	fix u,v,z,l,dudx, dvdx, dzdx, dldx;

	u = fx_u;
	v = fx_v*64;
	z = fx_z;
	dudx = fx_du_dx; 
	dvdx = fx_dv_dx*64; 
	dzdx = fx_dz_dx;

	l = fx_l>>8;
	dldx = fx_dl_dx>>8;
	dest = (ubyte *)(write_buffer + fx_xleft + (bytes_per_row * fx_y)  );


	{
		fix u0 = fx_u;
		fix u2 = fx_u + fx_du_dx*(fx_xright-fx_xleft+1);
		fix v0 = fx_v;
		fix v2 = fx_v + fx_dv_dx*(fx_xright-fx_xleft+1);
		fix w0 = fx_z;
		fix w2 = fx_z + fx_dz_dx*(fx_xright-fx_xleft+1);
		fix u1 = fixdiv((u0+u2),(w0+w2));
		fix v1 = fixdiv((v0+v2),(w0+w2));
		fix a0, a1, a2, b0, b1, b2, dudx1, dvdx1;
		int dx = fx_xright-fx_xleft+1;
		u0 = fixdiv( u0, w0 );
		v0 = fixdiv( v0, w0 );
		u2 = fixdiv( u2, w2 );
		v2 = fixdiv( v2, w2 );
		a0 = u0;									b0 = v0;
		a1 = (-3*u0+4*u1-u2)/dx;			b1 = (-3*v0+4*v1-v2)/dx;
		a2 = (2*(u0-2*u1+u2))/(dx*dx);			b2 = (2*(v0-2*v1+v2))/(dx*dx);
		dudx = a1 + a2;
		dvdx = b1 + b2;
		dudx1 = 2*a2;
		dvdx1 = 2*b2;
		u = u0;
		v = v0;


		if (!Transparency_on)	{
			for (x= fx_xright-fx_xleft+1 ; x > 0; --x ) {
				*dest++ = gr_fade_table[ (l&(0xff00)) + (uint)pixptr[  (f2i(v)&63)*64 + (f2i(u)&63) ] ];
				l += dldx;
				u += dudx;
				v += dvdx;
				dudx += dudx1;
				dvdx += dvdx1;
				//z += dzdx;
			}
		} else {
			for (x= fx_xright-fx_xleft+1 ; x > 0; --x ) {
				c = (uint)pixptr[  (f2i(v)&63)*64 + (f2i(u)&63) ];
				if ( c!=255)
					*dest = gr_fade_table[ (l&(0xff00)) + c ];
				dest++;
				l += dldx;
				u += dudx;
				v += dvdx;
				dudx += dudx1;
				dvdx += dvdx1;
				//z += dzdx;
			}
		}
	}
}


