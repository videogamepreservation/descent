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
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

FILE * HogFile;

void hog_add_file( char * filename )
{
	FILE * fp;
	int length;
	char *data;
	int i;

	fp = fopen( filename, "rb" );
	if ( fp == NULL )	{
		printf( "Error opening '%s'... not added!\n", filename );
		fclose( HogFile );	
		exit(1);
	}	
	length = filelength(fileno(fp));
	data = malloc( length );
	if ( data == NULL )	{
		printf( "Error mallocing '%s'... not added!\n", filename );
		fclose(fp);
		fclose( HogFile );	
		exit(1);
	}	
	i = fread( data, length, 1, fp );
	if ( i != 1 )	{
		printf( "Error reading '%s'... not added!\n", filename );
		fclose(fp);
		free(data);
		fclose( HogFile );	
		exit(1);
	}
	fclose(fp);

	if (fwrite( filename, 13, 1, HogFile )!=1 )	{
		printf("Error writing hogfile\n" );
		fclose( HogFile );	
		exit(1);
	}
	if (fwrite( &length, 4, 1, HogFile)!=1 )	{
		printf("Error writing hogfile\n" );
		fclose( HogFile );	
		exit(1);
	}
	if (fwrite( data, length, 1, HogFile)!=1 )	{
		printf("Error writing hogfile\n" );
		fclose( HogFile );	
		exit(1);
	}

}


#define REMOVE_EOL(s)		remove_char((s),'\n')
#define REMOVE_COMMENTS(s)	remove_char((s),';')
#define REMOVE_DOTS(s)  	remove_char((s),'.')

void remove_char( char * s, char c )
{
	char *p;
	p = strchr(s,c);
	if (p) *p = '\0';
}

int main(int argc, char * argv[] )	
{
	char * id = "DHF";
	char	inputline[160+1];
	char filename[80];
	FILE * fp;

	if ( argc < 3 )	{
		printf( "Usage: hogfile inputfile outputfile\n" );
		exit(1);
	}
	
	fp = fopen( argv[1], "rt" );
	if ( fp == NULL )	{
		printf( "Error opening '%s'\n", argv[1] );
		exit(1);
	}

	HogFile = fopen( argv[2], "wb" );
	if ( HogFile == NULL )		{
		printf( "Error opening '%s'\n", argv[2] );
		fclose(fp);
		exit(1);
	}
	if (fwrite( id, 3, 1, HogFile )!=1)	{
		printf("Error writing hogfile\n" );
		fclose( HogFile );	
		exit(1);
	}
		
	while (fgets(inputline, 160, fp )) {
		REMOVE_EOL(inputline);
		REMOVE_COMMENTS(inputline);
		
		sscanf( inputline, " %s ", filename );
			
		if ( strlen( inputline ) > 0 )	{
			//printf( "Adding '%s'...", filename );
			hog_add_file( filename );
			//printf( "\n" );
		}
	}

	fclose(fp);
	fclose( HogFile );	

	return 0;
}






