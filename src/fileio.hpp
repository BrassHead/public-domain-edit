/*
    uemacs - File I/O

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.
*/

/*
	Macros used by the buffer name making code. Start at the end of the file
	name, scan to the left until BDC1 (or BDC2, if defined) is reached. The
	buffer name starts just to the right of that location, and stops at end of
	string (or at the next BDC3 character, if defined). BDC2 and BDC3 are mainly
	for VMS.
 */
#define	BDC1	'/'
// #define BDC2	':'
// #define BDC3 ';'

/*
	Forward declarations
*/
void adjustcase(char *fn);

static	FILE	*ffp;

/*
	Open a file for reading.
*/
int
ffropen(char *fn)
{
	ffp = fopen(fn, "r");
	adjustcase(fn);
	if (ffp == NULL)
		return (FIOFNF);

	// Decrypt buffer if encryption is enabled
	if (encrypt_flag) {
		char buffer[NLINE];
		int length;
		while ((length = fread(buffer, 1, NLINE, ffp)) > 0) {
			decrypt_buffer(buffer, length);
		}
		rewind(ffp);
	}

	return (FIOSUC);
}

/*
	Open a file for writing. Return TRUE if all is well, and FALSE on error
	(cannot create).
*/
int
ffwopen(char *fn)
{
	ffp = fopen(fn, "w");
	adjustcase(fn);
	if (ffp == NULL) {
		eprintf("Cannot open file for writing");
		return (FIOERR);
	}

	// Encrypt buffer if encryption is enabled
	if (encrypt_flag) {
		char buffer[NLINE];
		int length;
		while ((length = fread(buffer, 1, NLINE, ffp)) > 0) {
			encrypt_buffer(buffer, length);
		}
		rewind(ffp);
	}

	return (FIOSUC);
}

/*
	Close a file. Should look at the status.
*/
int
ffclose()
{
	fclose(ffp);
	return (FIOSUC);
}

/*
	Write a line to the already opened file. The "buf" points to the buffer, and
	the "nbuf" is its length, less the free newline. Return the status. Check
	only at the newline.
*/
int
ffputline(char buf[], int nbuf)
{
	int	i;

	for (i=0; i<nbuf; ++i)
		putc(buf[i]&0xFF, ffp);
	putc('\n', ffp);
	if (ferror(ffp) != FALSE) {
		eprintf("Write I/O error");
		return (FIOERR);
	}
	return (FIOSUC);
}

/*
	Read a line from a file, and store the bytes in the supplied buffer. Stop on
	end of file or end of line. Don't get upset by files that don't have an end
	of line on the last line; this seem to be common on CP/M-86 and MS-DOS (the
	suspected culprit is VAX/VMS kermit, but this has not been confirmed. If
	this is sufficiently researched it may be possible to pull this kludge).
	Delete any CR followed by an LF. This is mainly for runoff documents, both
	on VMS and on Ultrix (they get copied over from VMS systems with DECnet).
*/
int
ffgetline(char buf[], int nbuf)
{
	int	c;
	int	i;

	i = 0;
	for (;;) {
		c = getc(ffp);
		if (c == '\r') {		/* Delete any non-stray	*/
			c = getc(ffp);		/* carriage returns.	*/
			if (c != '\n') {
				if (i >= nbuf-1) {
					eprintf("File has long line");
					return (FIOERR);
				}
				buf[i++] = '\r';
			}
		}
		if (c==EOF || c=='\n')		/* End of line.		*/
			break;
		if (i >= nbuf-1) {
			eprintf("File has long line");
			return (FIOERR);
		}
		buf[i++] = c;
	}
	if (c == EOF) {				/* End of file.		*/
		if (ferror(ffp) != FALSE) {
			eprintf("File read error");
			return (FIOERR);
		}
		if (i == 0)			/* Don't get upset if	*/
			return (FIOEOF);	/* no newline at EOF.	*/
	}
	buf[i] = 0;
	return (FIOSUC);
}

/*
	Create a backup file in nix.
*/
int
fbackupfile(const char *fname)
{
	char	bfname[NFILEN];

	strcpy(bfname,fname);
	strcat(bfname,"~");

	if (access(bfname,0)==0)
		if (unlink(bfname))	/* Note: may unlink original file */
			return FALSE;

	if (link(fname,bfname)==0)	/* We don't care if these fail	  */
		unlink(fname);		/* because, we're about to write  */

	return TRUE;
}

/*
	Zap file name to lower case, for systems that need it.
*/
void
adjustcase(char *fn)
{
/*	register int	c;

	while ((c = *fn) != 0) {
		if (ISUPPER(c) != FALSE)
			*fn = TOLOWER(c);
		++fn;
	}
--not needed for NIX  */
}
