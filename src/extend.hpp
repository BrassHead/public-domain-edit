/*
	uemacs - Extended (M-X) commands

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.
*/

/*
	This function modifies the keyboard binding table, by adjusting the entries
	in the big "bindings" array. Most of the grief deals with the prompting for
	additional arguments. This code does not work right if there is a keyboard
	macro floating around. Should be fixed.
*/
int
bindtokey(int f , int n, int k)
{
	int	s;
	SYMBOL	*sp;
	int	c;
	char		xname[NXNAME];

	if (kbdmip!=NULL || kbdmop!=NULL) {
		eprintf("Not now");
		return (FALSE);
	}
	if ((s=eread("Function: ", xname, NXNAME, EFAUTO, NULL)) != TRUE)
		return (s);
	if ((sp=symlookup(xname)) == NULL) {
		eprintf("Unknown function for binding");
		return (FALSE);
	}
	eputc(' ');
	eputc('K');
	eputc('e');
	eputc('y');
	eputc(':');
	eputc(' ');
	ttflush();
	c = getkey();				/* Read key.		*/
	keyname(xname, c);			/* Display keyname.	*/
	eputs(xname);
	ttflush();
	if (binding[c] != NULL)		/* Unbind old, and	*/
		--binding[c]->s_nkey;
	binding[c] = sp;			/* rebind new.		*/
	++sp->s_nkey;
	return (TRUE);
}
 
/*
	Extended command. Call the message line routine to read in the command name
	and apply autocompletion to it. When it comes back, look the name up in the
	symbol table and run the command if it is found and has the right type.
	Print an error if there is anything wrong.
*/
int
extend(int f , int n, int k)
{
	SYMBOL	*sp;
	int	s;
	char		xname[NXNAME];

	if ((s=eread(": ", xname, NXNAME, EFNEW|EFAUTO, NULL)) != TRUE)
		return (s);
	if ((sp=symlookup(xname)) != NULL)
		return ((*sp->s_funcp)(f, n, KRANDOM));
	eprintf("Unknown extended command");
	return (ABORT);
}

/*
	Read a key from the keyboard, and look it up in the binding table. Display
	the name of the function currently bound to the key. Say that the key is not
	bound if it is indeed not bound, or if the type is not a "builtin". This is
	a bit of overkill, because this is the only kind of function there is.
*/
int
help(int f , int n, int k)
{
	SYMBOL	*sp;
	int	c;
	char		b[20];

	c = getkey();
	keyname(b, c);
	if ((sp=binding[c]) == NULL)
		eprintf("[%s is unbound]", b);
	else
		eprintf("[%s is bound to %s]", b, sp->s_name);
	return (TRUE);
}

/*
	This function creates a table, listing all of the command keys and their
	current bindings, and stores the table in the standard pop-op buffer (the
	one used by the directory list command, the buffer list command, etc.). This
	lets MicroEMACS produce it's own wall chart. The bindings to "ins-self" are
	only displayed if there is an argument.
*/
int
wallchart(int f , int n, int k)
{
	int	s;
	int	key;
	SYMBOL	*sp;
	char	*cp1;
	const char	*cp2;
	char		buf[64];

	if ((s=bclear(blistp)) != TRUE)		/* Clear it out.	*/
		return (s);
	(void) strcpy(blistp->b_fname, "");
	for (key=0; key<NKEYS; ++key) {		/* For all keys.	*/
		sp = binding[key];
		if (sp != NULL
		&& (f!=FALSE || strcmp(sp->s_name, "ins-self")!=0)) {
			keyname(buf, key);
			cp1 = &buf[0];				/* Find end.		*/
			while (*cp1 != 0)
				++cp1;
			while (cp1 < &buf[16])		/* Goto column 16.	*/
				*cp1++ = ' ';				
			cp2 = sp->s_name;			/* Add function name.	*/
			while ((*cp1++ = *cp2++))
				;
			if (addline(buf) == FALSE)
				return (FALSE);
		}
	}
	return (popblist());
}
