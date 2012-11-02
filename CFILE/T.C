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
#include <stdlib.h>
#include <stdio.h>

typedef struct hogfile {
	char	name[13];
	int	offset;
	int 	length;
} hogfile;

void main()
{
	char id[4];
	FILE * fp, *fp1;
	int i, len;
	char name[16];
	char * ptr;

	fp = fopen( "DESCENT.HOG", "rb" );
	if ( fp == NULL ) return;

	fread( id, 3, 1, fp );
	if ( strncmp( id, "DHF", 3 ) )	{
		fclose(fp);
		return;
	}

	while( 1 )	
	{	
		i = fread( name, 13, 1, fp );
		if ( i != 1 )	{
			fclose(fp);
			return;
		}
		i = fread( &len, 4, 1, fp );
		if ( i != 1 )	{
			fclose(fp);
			return;
		}
		fp1 = fopen( name, "wb" );
		if ( fp1 == NULL ) printf( "Error opening '%s'\n", name );
		printf( "Extracting: '%s'\n", name );

		ptr = malloc( len );
		if ( ptr == NULL ) printf( "Error mallocing '%s'\n", name );

		fread( ptr, len, 1, fp );
		fwrite( ptr, len, 1, fp1 );
		free(ptr);
		fclose(fp1);
	}	
}
 

