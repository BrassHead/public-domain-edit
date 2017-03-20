/*
    uemacs - CLI Spawn

    No copyright 2016 (CC0-1.0 Public Domain)

    See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
    'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT
    DISCLAIMERS.
*/

/*
	Spawn shell command line execution.
*/
int
spawncli(int f, int n, int k)
{
	register int	s;
	char		xname[NXNAME];
	va_list		dummy;

	if( (s=eread("! ",xname, NXNAME, EFNEW|EFCR, dummy)) != TRUE)
		return s;

	ttmove( nrow-1, 0 );
	ttputc( '\n' );
	ttflush();
	system( xname );
	ttputc( '\n' );
	(void) ttgetc();
	return( refresh(f, n, KRANDOM) );
}
