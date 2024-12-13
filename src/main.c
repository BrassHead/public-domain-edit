/*
	uemacs - Mainline, macro commands

    No copyright 2016 (CC0-1.0 Public Domain)

    See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
    'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT
    DISCLAIMERS.
*/

#include	"def.h"

int execute(int c, int f, int n);

#include "version.hpp"
#include "cinfo.hpp"
#include "symbol.hpp"
#include "ttyio.hpp"
#include "ttykbd.hpp"
#include "kbd.hpp"
#include "tty.hpp"


#include "display.hpp"

#include "echo.hpp"
#include "line.hpp"
#include "buffer.hpp"
#include "fileio.hpp"
#include "file.hpp"

/*
    mapable routines
*/
#include "search.hpp"
#include "basic.hpp"
#if DIRLIST
    #include "dirlist.hpp"
#endif
#include "random.hpp"
#include "region.hpp"
#include "spawn.hpp"
#include "window.hpp"
#include "word.hpp"
#include "extend.hpp"


void edinit(char bname[]);

#ifdef TOS
int     _mneed = 30000;			/* Edit buffer size in Lattice  */
#endif

int     thisflag;			/* Flags, this command		*/
int     lastflag;			/* Flags, last command		*/
int     curgoal;			/* Goal column			*/
BUFFER	*curbp;				/* Current buffer		*/
WINDOW	*curwp;				/* Current window		*/
BUFFER	*bheadp;			/* BUFFER listhead		*/
WINDOW	*wheadp;			/* WINDOW listhead		*/
BUFFER	*blistp;			/* Buffer list BUFFER		*/
short	kbdm[NKBDM] = {(KCTLX|')')};	/* Macro			*/
short	*kbdmip;			/* Input  for above		*/
short	*kbdmop;			/* Output for above		*/
char	pat[NPAT];			/* Pattern			*/
SYMBOL	*symbol[NSHASH];		/* Symbol table listhead.	*/
SYMBOL	*binding[NKEYS];		/* Key bindings.		*/
int encrypt_flag = 0; // Global variable to indicate if encryption is enabled

int main(int argc, char *argv[])
{
	int	c;
	int	f;
	int	n;
	int	mflag;
	char		bname[NBUFN];

	strcpy(bname, "main");			/* Get buffer name.	*/
	if (argc > 1)
		makename(bname, argv[1]);
	vtinit();				/* Virtual terminal.	*/
	edinit(bname);				/* Buffers, windows.	*/
	keymapinit();				/* Symbols, bindings.	*/

	// Parse CLI options to set encrypt_flag if --enc or --encrypt is given
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--enc") == 0 || strcmp(argv[i], "--encrypt") == 0) {
			encrypt_flag = 1;
		}
	}

	if (argc > 1) {
		update();
		readin(argv[1]);
	}
	lastflag = 0;				/* Fake last flags.	*/
loop:
	update();				/* Fix up the screen.	*/
	c = getkey();
	if (epresf != FALSE) {
		eerase();
		update();
	}
	f = FALSE;
	n = 1;
	if (c == (KCTRL|'U')) {			/* ^U, start argument.	*/
		f = TRUE;
		n = 4;
		while ((c=getkey()) == (KCTRL|'U'))
			n *= 4;
		if ((c>='0' && c<='9') || c=='-') {
			if (c == '-') {
				n = 0;
				mflag = TRUE;
			} else {
				n = c - '0';
				mflag = FALSE;
			}
			while ((c=getkey())>='0' && c<='9')
				n = 10*n + c - '0';
			if (mflag != FALSE)
				n = -n;
		}
	}
	if (kbdmip != NULL) {			/* Save macro strokes.	*/
		if (c!=(KCTLX|')') && kbdmip>&kbdm[NKBDM-6]) {
			ctrlg(FALSE, 0, KRANDOM);
			goto loop;
		}
		if (f != FALSE) {
			*kbdmip++ = (KCTRL|'U');
			*kbdmip++ = n;
		}
		*kbdmip++ = c;
	}
	execute(c, f, n);			/* Do it.		*/
	goto loop;

    return 0;
}

/*
	Command execution. Look up the binding in the the binding array, and do what
	it says. Return a very bad status if there is no binding, or if the symbol
	has a type that is not usable (there is no way to get this into a symbol
	table entry now). Also fiddle with the flags.
*/
int
execute(int c, int f, int n)
{
	SYMBOL	*sp;
	int	status;

	if ((sp=binding[c]) != NULL) {
		thisflag = 0;
		status = (*sp->s_funcp)(f, n, c);
		lastflag = thisflag;
		return (status);
	}
	lastflag = 0;
	return (ABORT);
}

/*
	Initialize all of the buffers and windows. The buffer name is passed down as
	an argument, because the main routine may have been told to read in a file
	by default, and we want the buffer name to be right.
*/
void
edinit(char bname[])
{
	BUFFER	*bp;
	WINDOW	*wp;

	bp = bfind(bname, TRUE);		/* Text buffer.		*/
	blistp = bcreate("");			/* Special list buffer.	*/
	wp = (WINDOW *) malloc(sizeof(WINDOW));	/* Initial window.	*/
	if (bp==NULL || wp==NULL || blistp==NULL)
		abort();
	curbp  = bp;				/* Current ones.	*/
	wheadp = wp;
	curwp  = wp;
	wp->w_wndp  = NULL;			/* Initialize window.	*/
	wp->w_bufp  = bp;
	bp->b_nwnd  = 1;			/* Displayed.		*/
	wp->w_linep = bp->b_linep;
	wp->w_dotp  = bp->b_linep;
	wp->w_doto  = 0;
	wp->w_markp = NULL;
	wp->w_marko = 0;
	wp->w_toprow = 0;
	wp->w_ntrows = nrow-2;			/* 2 = mode, echo.	*/
	wp->w_force = 0;
	wp->w_flag  = WFMODE|WFHARD;		/* Full.		*/
}
	
/*
	Fancy quit command, as implemented by Jeff. If the current buffer has
	changed do a write current buffer. Otherwise run a command interpreter in a
	subjob. Two of these will get you out. Bound to "C-Z". Modified 89/12/13 to
	do save and exit -- BBRY
*/
int
jeffexit(int f, int n, int k)
{
	if ((curbp->b_flag&BFCHG) != 0)		/* Changed.		*/
		return (filesave(f, n, KRANDOM));
	return (quit(f, n, KRANDOM));		/* Quit			*/
}

/*
	Quit command. If an argument, always quit. Otherwise confirm if a buffer has
	been changed and not written out. Normally bound to "C-X C-C".
*/
int
quit(int f, int n, int k)
{
	int	s;

	if (f != FALSE				/* Argument forces it.	*/
	|| anycb() == FALSE			/* All buffers clean.	*/
	|| (s=eyesno("Quit")) == TRUE) {	/* User says it's OK.	*/
		vttidy();
		exit(GOOD);
	}
	return (s);
}

/*
	Begin a keyboard macro. Error if not at the top level in keyboard
	processing. Set up variables and return.
*/
int
ctlxlp(int f, int n, int k)
{
	if (kbdmip!=NULL || kbdmop!=NULL) {
		eprintf("Not now");
		return (FALSE);
	}
	eprintf("[Start macro]");
	kbdmip = &kbdm[0];
	return (TRUE);
}

/*
	End keyboard macro. Check for the same limit conditions as the above
	routine. Set up the variables and return to the caller.
*/
int
ctlxrp(int f, int n, int k)
{
	if (kbdmip == NULL) {
		eprintf("Not now");
		return (FALSE);
	}
	eprintf("[End macro]");
	kbdmip = NULL;
	return (TRUE);
}

/*
	Execute a macro. The command argument is the number of times to loop. Quit
	as soon as a command gets an error. Return TRUE if all ok, else FALSE.
*/
int
ctlxe(int f, int n, int k)
{
	int	c;
	int	af;
	int	an;
	int	s;

	if (kbdmip!=NULL || kbdmop!=NULL) {
		eprintf("Not now");
		return (FALSE);
	}
	if (n <= 0) 
		return (TRUE);
	do {
		kbdmop = &kbdm[0];
		do {
			af = FALSE;
			an = 1;
			if ((c = *kbdmop++) == (KCTRL|'U')) {
				af = TRUE;
				an = *kbdmop++;
				c  = *kbdmop++;
			}
			s = TRUE;
		} while (c!=(KCTLX|')') && (s=execute(c, af, an))==TRUE);
		kbdmop = NULL;
	} while (s==TRUE && --n);
	return (s);
}

/*
	Abort. Beep the beeper. Kill off any keyboard macro, etc., that is in
	progress. Sometimes called as a routine, to do general aborting of stuff.
*/
int
ctrlg(int f, int n, int k)
{
	ttbeep();
	if (kbdmip != NULL) {
		kbdm[0] = (KCTLX|')');
		kbdmip  = NULL;
	}
	return (ABORT);
}

/*
	Display the version. All this does is copy the text in the external
	"version" array into the message system, and call the message reading code.
	Don't call display if there is an argument.
*/
int
showversion(int f, int n, int k)
{
	char	**cpp;
	char	*cp;

	cpp = &version[0];
	while ((cp = *cpp++) != NULL) {
		if (writemsg(cp) == FALSE)
			return (FALSE);
	}
	if (f != FALSE)				/* No display if arg.	*/
		return (TRUE);
	return (readmsg(f,n,k));
}
