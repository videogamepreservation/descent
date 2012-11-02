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
/* Use Turbo C */
#include <stdlib.h>
#include <dos.h>
#include <ctype.h>

// Returns: 0 Ok, -1=Out of space
int dos_change_master_env( char * variable, char * value, int append )
{
	unsigned psp, far * tmp;
	char far * env, far * e;
	char * v, *p;
	int size;
	char var_name[1024];	
	char * var_value;
	
	psp = _psp;
	while (psp!=*(tmp=MK_FP(psp,22))) 
		psp = *tmp;
	env = MK_FP( *(unsigned far *)MK_FP(psp,44),0);
	size = 16* *(int far *)MK_FP( FP_SEG(env)-1,3);

	var_value = value;
	strcpy( var_name, variable );
	strcat( var_name, "=" );

	p = var_name;
	while( *p )	{
		*p = toupper( *p );
		p++;
	}

	while (*env )	{
		e = env;
		v = var_name;
		while((*e++ == *v++ ) && (*v))
			;
		if ( !*v )	{
			if ( append )
				while( (*v++ = *e++) )
				 	;
			else
				while( *e++ )
				 	;
			if ( *e ) 
				while((--size) && ((*env++ = *e++ )) || (*e) )
					;
			*env = '\0';
		} else 
			while ((--size) && (*env++))
				;
	}

	p = var_value;		
	if ( *p	!= '\0' )	{
		v = var_name;	
		while ((*env = *v++) ) 	{
			++env ;
			 --size;
		}
		while((--size) && ((*env++ = *p++ ) != '\0' ))
			;
		*env = '\0';
		*--env = '\0';
		if ( size <= 0 )	{
			return -1;
		}		
	}
	return 0;
}

void main()
{
	if (dos_change_master_env( "john", "a parallax employee", 0  )) 	
		printf( "ERROR: Out of environment space!\n" );
}


