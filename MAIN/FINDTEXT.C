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
#include <stdio.h>
#include <ctype.h>
#include <io.h>

int main(int argc, char * argv[] )
{
	char string[100];
	int n;
	char c;
	int in_rcs;
	FILE * fp;

	if ( argc < 2 )	{
		printf( "Findtext by JAS. Usage: Findtext binaryfilename\n" );
		return 1;
	}

	fp = fopen( argv[1], "rb" );

	n = 0;
	in_rcs = 0;
	while( fread( &c, 1, 1, fp )==1 )	{

		if ( in_rcs )	{
			if ( c == '$' )	{
				string[n] = 0;
				printf( "'%s'\n", string );
				in_rcs = 0;
				n = 0;
			}
			string[n++] = c;
		} else {
			if ( (c == '$') && (n==0) )	{
				string[n++] = c;
			} else if ( (c == 'I') && (n==1) )	{
			  	string[n++] = c;
			} else if ( (c == 'd') && (n==2) )	{
				string[n++] = c;
			} else if ( (c == ':') && (n==3) )	{
				string[n++] = c;
				in_rcs = 1;
			} else {
				n = 0;
			}
		}
//		if ( isprint(c)) {	
//			if ( n > 78 )	{
//				string[n] = 0;
//				printf( "'%s'\n", string );
//				n = 0;
//			}
//			string[n++] = c;
//		} else {
//			if ( n > 4 )	{
//				string[n] = 0;
//				printf( "'%s'\n", string );
//				n = 0;
//			}
//		}
	}
	fclose(fp);
	return 0;
}

