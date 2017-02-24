/*
    uemacs - Basic cursor motion commands

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.


    The routines in this file are the basic command functions for moving the
    cursor around on the screen, setting mark, and swapping dot with mark. Only
    moves between lines, which might make the current buffer framing bad, are
    hard.
*/


/* 
	Local Declarations 
*/
void setgoal();
int getgoal(LINE *dlp);

/*
	Go to beginning of line.
*/
int
gotobol(int f, int n, int k)
{
	curwp->w_doto  = 0;
	return (TRUE);
}

/*
	Move cursor backwards. Do the right thing if the count is less than 0. Error
	if you try to move back from the beginning of the buffer.
*/
int
backchar(int f, int n, int k)
{
	LINE	*lp;

	if (n < 0)
		return (forwchar(f, -n, KRANDOM));
	while (n--) {
		if (curwp->w_doto == 0) {
			if ((lp=lback(curwp->w_dotp)) == curbp->b_linep)
				return (FALSE);
			curwp->w_dotp  = lp;
			curwp->w_doto  = llength(lp);
			curwp->w_flag |= WFMOVE;
		} else
			curwp->w_doto--;
	}
	return (TRUE);
}

/*
	Go to end of line.
*/
int
gotoeol(int f, int n, int k)
{
	curwp->w_doto  = llength(curwp->w_dotp);
	return (TRUE);
}

/*
	Move cursor forwards. Do the right thing if the count is less than 0. Error
	if you try to move forward from the end of the buffer.
*/
int
forwchar(int f, int n, int k)
{
	if (n < 0)
		return (backchar(f, -n, KRANDOM));
	while (n--) {
		if (curwp->w_doto == llength(curwp->w_dotp)) {
			if (curwp->w_dotp == curbp->b_linep)
				return (FALSE);
			curwp->w_dotp  = lforw(curwp->w_dotp);
			curwp->w_doto  = 0;
			curwp->w_flag |= WFMOVE;
		} else
			curwp->w_doto++;
	}
	return (TRUE);
}

/*
	Go to the beginning of the buffer. Setting WFHARD is conservative, but
	almost always the case.
*/
int
gotobob(int f, int n, int k)
{
	curwp->w_dotp  = lforw(curbp->b_linep);
	curwp->w_doto  = 0;
	curwp->w_flag |= WFHARD;
	return (TRUE);
}

/*
	Go to the end of the buffer. Setting WFHARD is conservative, but almost
	always the case.
*/
int
gotoeob(int f, int n, int k)
{
	curwp->w_dotp  = curbp->b_linep;
	curwp->w_doto  = 0;
	curwp->w_flag |= WFHARD;
	return (TRUE);
}

/*
	Move forward by full lines. If the number of lines to move is less than
	zero, call the backward line function to actually do it. The last command
	controls how the goal column is set.
*/
int
forwline(int f, int n, int k)
{
	LINE	*dlp;

	if (n < 0)
		return (backline(f, -n, KRANDOM));
	if ((lastflag&CFCPCN) == 0)		/* Fix goal.		*/
		setgoal();
	thisflag |= CFCPCN;
	dlp = curwp->w_dotp;
	while (n-- && dlp!=curbp->b_linep)
		dlp = lforw(dlp);
	curwp->w_dotp  = dlp;
	curwp->w_doto  = getgoal(dlp);
	curwp->w_flag |= WFMOVE;
	return (TRUE);
}

/*
	This function is like "forwline", but goes backwards. The scheme is exactly
	the same. Check for arguments that are less than zero and call your
	alternate. Figure out the new line and call "movedot" to perform the motion.
*/
int
backline(int f, int n, int k)
{
	LINE	*dlp;

	if (n < 0)
		return (forwline(f, -n, KRANDOM));
	if ((lastflag&CFCPCN) == 0)		// Fix goal
		setgoal();
	thisflag |= CFCPCN;
	dlp = curwp->w_dotp;
	while (n-- && lback(dlp)!=curbp->b_linep)
		dlp = lback(dlp);
	curwp->w_dotp  = dlp;
	curwp->w_doto  = getgoal(dlp);
	curwp->w_flag |= WFMOVE;
	return (TRUE);
}

/*
	Set the current goal column, which is saved in the external variable
	"curgoal", to the current cursor column. The column is never off the edge of
	the screen; it's more like display then show position.
*/
void
setgoal()
{
	int	c;
	int	i;

	curgoal = 0;						// Get the position
	for (i=0; i<curwp->w_doto; ++i) {
		c = lgetc(curwp->w_dotp, i);
		if (c == '\t')
			curgoal |= TABMASK;
		else if (ISCTRL(c) != FALSE)
			++curgoal;
		++curgoal;
	}
	if (curgoal >= ncol)				// Chop to tty width
		curgoal = ncol-1;
}

/*
	This routine looks at a line (pointed to by the LINE pointer "dlp") and the
	current vertical motion goal column (set by the "setgoal" routine above) and
	returns the best offset to use when a vertical motion is made into the line.
*/
int
getgoal(LINE *dlp)
{
	int	c;
	int	col;
	int	newcol;
	int	dbo;

	col = 0;
	dbo = 0;
	while (dbo != llength(dlp)) {
		c = lgetc(dlp, dbo);
		newcol = col;
		if (c == '\t')
			newcol |= TABMASK;
		else if (ISCTRL(c) != FALSE)
			++newcol;
		++newcol;
		if (newcol > curgoal)
			break;
		col = newcol;
		++dbo;
	}
	return (dbo);
}

/*
	Scroll forward by a specified number of lines, or by a full page if no
	argument. The "2" is the window overlap (this is the default value from ITS
	EMACS). Because the top line in the window is zapped, we have to do a hard
	update and get it back.
*/
int
forwpage(int f, int n, int k)
{
    LINE	*lp;

	if (f == FALSE) {
		n = curwp->w_ntrows - 2;	// Default scroll
		if (n <= 0)					// Forget the overlap
			n = 1;					// if tiny window
	} else if (n < 0)
		return (backpage(f, -n, KRANDOM));
#if	CVMVAS
	else							// Convert from pages to lines
		n *= curwp->w_ntrows;
#endif
	lp = curwp->w_linep;
	while (n-- && lp!=curbp->b_linep)
		lp = lforw(lp);
	curwp->w_linep = lp;
	curwp->w_dotp  = lp;
	curwp->w_doto  = 0;
	curwp->w_flag |= WFHARD;
	return (TRUE);
}

/*
	This command is like "forwpage", but it goes backwards. The "2", like above,
	is the overlap between the two windows. The value is from the ITS EMACS
	manual. The hard update is done because the top line in the window is
	zapped.
*/
int
backpage(int f, int n, int k)
{
	LINE	*lp;

	if (f == FALSE) {
		n = curwp->w_ntrows - 2;	// Default scroll
		if (n <= 0)					// Don't blow up if the	window is tiny
			n = 1;
	} else if (n < 0)
		return (forwpage(f, -n, KRANDOM));
#if	CVMVAS
	else							// Convert from pages to lines
		n *= curwp->w_ntrows;
#endif
	lp = curwp->w_linep;
	while (n-- && lback(lp)!=curbp->b_linep)
		lp = lback(lp);
	curwp->w_linep = lp;
	curwp->w_dotp  = lp;
	curwp->w_doto  = 0;
	curwp->w_flag |= WFHARD;
	return (TRUE);
}

/*
	Set the mark in the current window to the value of dot. A message is written
	to the echo line unless we are running in a keyboard macro, when it would be
	silly.
*/
int
setmark(int f, int n, int k)
{
	curwp->w_markp = curwp->w_dotp;
	curwp->w_marko = curwp->w_doto;
	if (kbdmop == NULL)
		eprintf("[Mark set]");
	return (TRUE);
}

/*
	Swap the values of "dot" and "mark" in the current window. This is pretty
	easy, because all of the hard work gets done by the standard routine that
	moves the mark about. The only possible error is "no mark".
*/
int
swapmark(int f, int n, int k)
{
    LINE	*odotp;
	int	odoto;

	if (curwp->w_markp == NULL) {
		eprintf("No mark in this window");
		return (FALSE);
	}
	odotp = curwp->w_dotp;
	odoto = curwp->w_doto;
	curwp->w_dotp  = curwp->w_markp;
	curwp->w_doto  = curwp->w_marko;
	curwp->w_markp = odotp;
	curwp->w_marko = odoto;
	curwp->w_flag |= WFMOVE;
	return (TRUE);
}

/*
	Go to a specific line, mostly for looking up errors in C programs, which
	give the error a line number. If an argument is present, then it is the line
	number, else prompt for a line number to use.
*/
int
gotoline(int f, int n, int k)
{
	LINE	*clp;
	int	s;
	char		buf[32];

	if (f == FALSE) {
		if ((s=ereply("Goto line: ", buf, sizeof(buf))) != TRUE)
			return (s);
		n = atoi(buf);
	}
	if (n <= 0) {
		eprintf("Bad line");
		return (FALSE);
	}
	clp = lforw(curbp->b_linep);		/* "clp" is first line	*/
	while (n != 1) {
		if (clp == curbp->b_linep) {
			eprintf("Line number too large");
			return (FALSE);
		}
		clp = lforw(clp);
		--n;
	}
	curwp->w_dotp = clp;
	curwp->w_doto = 0;
	curwp->w_flag |= WFMOVE;
	return (TRUE);
}
