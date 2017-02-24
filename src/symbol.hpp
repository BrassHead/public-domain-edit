/*
    uemacs - Symbol tables and keymap setup

    No copyright 2016 (CC0-1.0 Public Domain)

    See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
    'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT
    DISCLAIMERS.

    This file has terminal independent key bindings (eg: control keys).
    It also has declarations for all the key-mappable functions.

    The terminal specific key mappings are in the terminal dependent source 
    file ttykbd.hpp (eg: F1 key)
*/

#define	DIRLIST	0			    /* Disarmed!			*/

void ttykeymapinit(void);       /* Terminal dependent key mapping */

/*
    Defined by "main.c".
*/
extern	int	ctrlg(int f, int n, int k);		    /* Abort out of things		*/
extern	int	quit(int f, int n, int k);			/* Quit				*/
extern	int	ctlxlp(int f, int n, int k);		/* Begin macro			*/
extern	int	ctlxrp(int f, int n, int k);		/* End macro			*/
extern	int	ctlxe(int f, int n, int k);		    /* Execute macro		*/
extern	int	jeffexit(int f, int n, int k);		/* Jeff Lomicka style exit.	*/
extern  int	showversion(int f, int n, int k);	/* Show version numbers, etc.	*/

/*
    Defined by "search.c".
*/
extern	int	forwsearch(int f, int n, int k);		/* Search forward		*/
extern	int	backsearch(int f, int n, int k);		/* Search backwards		*/
extern  int	searchagain(int f, int n, int k);		/* Repeat last search command	*/
extern  int	forwisearch(int f, int n, int k);		/* Incremental search forward	*/
extern  int	backisearch(int f, int n, int k);		/* Incremental search backwards	*/
extern  int	queryrepl(int f, int n, int k);		/* Query replace		*/

/*
    Defined by "basic.c".
*/
extern	int	gotobol(int f, int n, int k);		/* Move to start of line	*/
extern	int	backchar(int f, int n, int k);		/* Move backward by characters	*/
extern	int	gotoeol(int f, int n, int k);		/* Move to end of line		*/
extern	int	forwchar(int f, int n, int k);		/* Move forward by characters	*/
extern	int	gotobob(int f, int n, int k);		/* Move to start of buffer	*/
extern	int	gotoeob(int f, int n, int k);		/* Move to end of buffer	*/
extern	int	forwline(int f, int n, int k);		/* Move forward by lines	*/
extern	int	backline(int f, int n, int k);		/* Move backward by lines	*/
extern	int	forwpage(int f, int n, int k);		/* Move forward by pages	*/
extern	int	backpage(int f, int n, int k);		/* Move backward by pages	*/
extern	int	setmark(int f, int n, int k);		/* Set mark			*/
extern	int	swapmark(int f, int n, int k);		/* Swap "." and mark		*/
extern	int	gotoline(int f, int n, int k);		/* Go to a specified line.	*/

/*
    Defined by "buffer.c".
*/
extern	int	listbuffers(int f, int n, int k);		/* Display list of buffers	*/
extern	int	usebuffer(int f, int n, int k);		/* Switch a window to a buffer	*/
extern	int	killbuffer(int f, int n, int k);		/* Make a buffer go away.	*/

#if	DIRLIST
/*
    Defined by "dirlist.c".
*/
extern	int	dirlist(int f, int n, int k);		/* Directory list.		*/
#endif

/*
    Defined by echo.hpp
*/
extern int readmsg(int f, int n, int k);

/*
    Defined by "file.c".
*/
extern	int	fileread(int f, int n, int k);		/* Get a file, read only	*/
extern	int	filevisit(int f, int n, int k);		/* Get a file, read write	*/
extern	int	filewrite(int f, int n, int k);		/* Write a file			*/
extern	int	filesave(int f, int n, int k);		/* Save current file		*/
extern	int	filename(int f, int n, int k);		/* Adjust file name		*/

/*
    Defined by "random.c".
*/
extern	int	selfinsert(int f, int n, int k);		/* Insert character		*/
extern	int	showcpos(int f, int n, int k);		/* Show the cursor position	*/
extern	int	twiddle(int f, int n, int k);		/* Twiddle characters		*/
extern	int	quote(int f, int n, int k);		/* Insert literal		*/
extern	int	openline(int f, int n, int k);		/* Open up a blank line		*/
extern	int	newline(int f, int n, int k);		/* Insert CR-LF			*/
extern	int	deblank(int f, int n, int k);		/* Delete blank lines		*/
extern	int	indent(int f, int n, int k);		/* Insert CR-LF, then indent	*/
extern	int	forwdel(int f, int n, int k);		/* Forward delete		*/
extern	int	backdel(int f, int n, int k);		/* Backward delete		*/
extern	int	killline(int f, int n, int k);		/* Kill forward			*/
extern	int	yank(int f, int n, int k);			/* Yank back from killbuffer.	*/

/*
    Defined by "region.c".
*/
extern	int	killregion(int f, int n, int k);		/* Kill region.			*/
extern	int	copyregion(int f, int n, int k);		/* Copy region to kill buffer.	*/
extern	int	lowerregion(int f, int n, int k);		/* Lower case region.		*/
extern	int	upperregion(int f, int n, int k);		/* Upper case region.		*/

/*
    Defined by "spawn.c".
*/
extern	int	spawncli(int f, int n, int k);		/* Run CLI in a subjob.		*/

/*
    Defined by "window.c".
*/
extern	int	reposition(int f, int n, int k);		/* Reposition window		*/
extern	int	refresh(int f, int n, int k);		/* Refresh the screen		*/
extern	int	nextwind(int f, int n, int k);		/* Move to the next window	*/
extern  int	prevwind(int f, int n, int k);		/* Move to the previous window	*/
extern	int	mvdnwind(int f, int n, int k);		/* Move window down		*/
extern	int	mvupwind(int f, int n, int k);		/* Move window up		*/
extern	int	onlywind(int f, int n, int k);		/* Make current window only one	*/
extern	int	splitwind(int f, int n, int k);		/* Split current window		*/
extern	int	enlargewind(int f, int n, int k);		/* Enlarge display window.	*/
extern	int	shrinkwind(int f, int n, int k);		/* Shrink window.		*/

/*
    Defined by "word.c".
*/
extern	int	backword(int f, int n, int k);		/* Backup by words		*/
extern	int	forwword(int f, int n, int k);		/* Advance by words		*/
extern	int	upperword(int f, int n, int k);		/* Upper case word.		*/
extern	int	lowerword(int f, int n, int k);		/* Lower case word.		*/
extern	int	capword(int f, int n, int k);		/* Initial capitalize word.	*/
extern	int	delfword(int f, int n, int k);		/* Delete forward word.		*/
extern	int	delbword(int f, int n, int k);		/* Delete backward word.	*/

/*
    Defined by "extend.c".
*/
extern	int	extend(int f, int n, int k);		/* Extended commands.		*/
extern	int	help(int f, int n, int k);			/* Help key.			*/
extern	int	bindtokey(int f, int n, int k);		/* Modify key bindings.		*/
extern	int	wallchart(int f, int n, int k);		/* Make wall chart.		*/

typedef	struct	{
	short	k_key;              /* Key to bind.			*/
	int     (*k_funcp)();		/* Function.			*/
	const char	*k_name;		/* Function name string.	*/
}	KEY;

/*
    Default key binding table. This contains the function names, the symbol
    table name, and (possibly) a key binding for the builtin functions. There
    are no bindings for C-U or C-X. These are done with special code, but should
    be done normally.
*/
KEY	key[] = {
	KCTRL|'@',	setmark,	"set-mark",
	KCTRL|'A',	gotobol,	"goto-bol",
	KCTRL|'B',	backchar,	"back-char",
	KCTRL|'D',	forwdel,	"forw-del-char",
	KCTRL|'E',	gotoeol,	"goto-eol",
	KCTRL|'F',	forwchar,	"forw-char",
	KCTRL|'G',	ctrlg,		"abort",
	KCTRL|'H',	backdel,	"back-del-char",
	KCTRL|'I',	selfinsert,	"ins-self",
	KCTRL|'J',	indent,		"ins-nl-and-indent",
	KCTRL|'K',	killline,	"kill-line",
	KCTRL|'L',	refresh,	"refresh",
	KCTRL|'M',	newline,	"ins-nl",
	KCTRL|'N',	forwline,	"forw-line",
	KCTRL|'O',	openline,	"ins-nl-and-backup",
	KCTRL|'P',	backline,	"back-line",
	KCTRL|'Q',	quote,		"quote",
	KCTRL|'R',	backsearch,	"back-search",
	KCTRL|'S',	forwsearch,	"forw-search",
	KCTRL|'T',	twiddle,	"twiddle",
	KCTRL|'V',	forwpage,	"forw-page",
	KCTRL|'W',	killregion,	"kill-region",
	KCTRL|'Y',	yank,		"yank",
	KCTRL|'Z',	jeffexit,	"jeff-exit",
	KCTLX|KCTRL|'B',listbuffers,	"display-buffers",
	KCTLX|KCTRL|'C',quit,		"quit",
#if	DIRLIST
	KCTLX|KCTRL|'D',dirlist,	"display-directory",
#endif
	KCTLX|KCTRL|'F',filevisit,	"edit-file",
	KCTLX|KCTRL|'L',lowerregion,	"lower-region",
	KCTLX|KCTRL|'N',mvdnwind,	"down-window",
	KCTLX|KCTRL|'O',deblank,	"del-blank-lines",
	KCTLX|KCTRL|'P',mvupwind,	"up-window",
	KCTLX|KCTRL|'R',fileread,	"file-read",
	KCTLX|KCTRL|'S',filesave,	"file-save",
	KCTLX|KCTRL|'U',upperregion,	"upper-region",
	KCTLX|KCTRL|'V',filevisit,	"file-visit",
	KCTLX|KCTRL|'W',filewrite,	"file-write",
	KCTLX|KCTRL|'X',swapmark,	"swap-dot-and-mark",
	KCTLX|KCTRL|'Z',shrinkwind,	"shrink-window",
	KCTLX|'=',	showcpos,	"display-position",
	KCTLX|'(',	ctlxlp,		"start-macro",
	KCTLX|')',	ctlxrp,		"end-macro",
	KCTLX|'1',	onlywind,	"only-window",
	KCTLX|'2',	splitwind,	"split-window",
	KCTLX|'B',	usebuffer,	"use-buffer",
	KCTLX|'E',	ctlxe,		"execute-macro",
	KCTLX|'F',	filename,	"set-file-name",
	KCTLX|'K',	killbuffer,	"kill-buffer",
	KCTLX|'N',	nextwind,	"forw-window",
	KCTLX|'P',	prevwind,	"back-window",
	KCTLX|'R',	backisearch,	"back-i-search",
	KCTLX|'S',	forwisearch,	"forw-i-search",
	KCTLX|'Z',	enlargewind,	"enlarge-window",
	KMETA|KCTRL|'H',delbword,	"back-del-word",
	KMETA|KCTRL|'R',readmsg,	"display-message",
	KMETA|KCTRL|'V',showversion,	"display-version",
	KMETA|'!',	spawncli,	"spawn-cli",
	KMETA|'-',	reposition,	"reposition-window",
	KMETA|'>',	gotoeob,	"goto-eob",
	KMETA|'<',	gotobob,	"goto-bob",
	KMETA|'%',	queryrepl,	"query-replace",
	KMETA|'?',	help,		"help",
	KMETA|'B',	backword,	"back-word",
	KMETA|'C',	capword,	"cap-word",
	KMETA|'D',	delfword,	"forw-del-word",
	KMETA|'F',	forwword,	"forw-word",
	KMETA|'G',	gotoline,	"goto-line",
	KMETA|'L',	lowerword,	"lower-word",
	KMETA|'S',	searchagain,	"search-again",
	KMETA|'U',	upperword,	"upper-word",
	KMETA|'V',	backpage,	"back-page",
	KMETA|'W',	copyregion,	"copy-region",
	KMETA|'X',	extend,		"extended-command",
	-1,		wallchart,	"display-bindings",
	-1,		bindtokey,	"bind-to-key"
};

#define	NKEY	(sizeof(key) / sizeof(key[0]))

/*
    Take a string, and compute the symbol table bucket number. This is done by
    adding all of the characters together, and taking the sum mod NSHASH. The
    string probably should not contain any GR characters; if it does the "*cp"
    may get a nagative number on some machines, and the "%" will return a
    negative number!
*/
int
symhash(const char *cp)
{
	
    int	c;
	
    int	n;

	n = 0;
	while ((c = *cp++) != 0)
		n += c;
	return (n % NSHASH);
}

/*
    Symbol table lookup. Return a pointer to the SYMBOL node, or NULL if the
    symbol is not found.
*/
SYMBOL	*
symlookup(const char *cp)
{

    SYMBOL	*sp;

    sp = symbol[symhash(cp)];
    while (sp != NULL) {
        if (strcmp(cp, sp->s_name) == 0)
            return (sp);
        sp = sp->s_symp;
    }
    return (NULL);
}

/*
    Create a new builtin function "name" with function "funcp". If the "new" is
    a real key, bind it as a side effect. All errors are fatal.
*/
void
keyadd(int knew, int (*funcp)(), const char *name)
{

    SYMBOL	*sp;

    int	hash;

    if ((sp=(SYMBOL *)malloc(sizeof(SYMBOL))) == NULL)
        abort();
        hash = symhash(name);
        sp->s_symp = symbol[hash];
        symbol[hash] = sp;
        sp->s_nkey = 0;
        sp->s_name = name;
        sp->s_funcp = funcp;
        if (knew >= 0) {				/* Bind this key.	*/
            if (binding[knew] != NULL)
                abort();
            binding[knew] = sp;
            ++sp->s_nkey;
        }
}

/*
    Bind key "new" to the existing routine "name". If the name cannot be found,
    or the key is already bound, abort.
*/
void
keydup(int knew, const char *name)
{
	
    SYMBOL	*sp;

	if (binding[knew]!=NULL || (sp=symlookup(name))==NULL)
		abort();
	binding[knew] = sp;
	++sp->s_nkey;
}

/*
    Build initial keymap. The funny keys (commands, odd control characters) are
    mapped using a big table and calls to "keyadd". The printing characters are
    done with some do-it-yourself handwaving. The terminal specific keymap
    initialization code is called at the very end to finish up. All errors are
    fatal.
*/
void
keymapinit()
{

    SYMBOL	*sp;

    KEY	*kp;

    int	i;

    for (i=0; i<NKEYS; ++i)
        binding[i] = NULL;
    for (kp = &key[0]; kp < &key[NKEY]; ++kp)
        keyadd(kp->k_key, kp->k_funcp, kp->k_name);
    keydup(KCTLX|KCTRL|'G',	"abort");
    keydup(KMETA|KCTRL|'G',	"abort");
    keydup(0x7F,		"forw-del-char");
    keydup(KMETA|'.',	"set-mark");
    keydup(KMETA|'Q',	"quote");
    keydup(KMETA|0x7F,	"back-del-word");
    keydup(KMETA|'R',	"search-again");
    keydup(KCTLX|'O',	"forw-window");
    /*
        Should be bound by "tab" already.
    */
    if ((sp=symlookup("ins-self")) == NULL)
        abort();
    for (i=0x20; i<0x7F; ++i) {
        if (binding[i] != NULL)
            abort();
        binding[i] = sp;
        ++sp->s_nkey;
    }
    ttykeymapinit();
}
