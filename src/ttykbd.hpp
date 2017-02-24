/*
    uemacs - Generic *nix keyboard input and mappings

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.
*/

#define ESC 0x1b

/*
    Key names.
*/
char	*keystrings[] = {
	NULL,		"F1",		"F2",		"F3",
	"F4",		"F5",		"F6",		"F7",
	"F8",		"F9",		"F10",		"F11",
	"F12",		"Home",     "End",		"Up",
	"Left",		"Down",		"Right",	"Backspace",
	"Delete",	"PageUp",	"PageDown",	NULL,
	NULL,		NULL,		NULL,		NULL,
	NULL,		NULL,		NULL,		NULL
};

/*
    This function is used to read the keyboard for normal input (ie: with
    special keys active). If the ttgetc routine returns an escape prefix, remap
    the codes. The "Alt" key works like a "Meta" key, by looking the high order
    bit.
*/
int
getkbd()
{
	int	k;

	k = ttgetc();

    if (screen_size_changed()==TRUE) {
        /* we refresh screen on resize before handing the input */
        refresh(1, 1, KRANDOM);
    }

    if( k=='\x7f' ) {    /* Unix keyboard RUBOUT key (Mac Delete) */
        return KBKSP;
    }

    if( k==ESC ) {
		k = ttgetc();
		switch( k ) {

		    case 'O':		/* Function Key 1-4 */
			k = ttgetc();
			if( k>='P' && k<='S')
				return k-'P'+KF1;
			else
				return KRANDOM;

		    case '[':		/* Special purpose key */
			k = ttgetc();
			switch( k ) {
			    case 'A':  return KUP;
			    case 'B':  return KDOWN;
			    case 'C':  return KRIGHT;
			    case 'D':  return KLEFT;
			    case 'H':  return KHOME;
			    case 'F':  return KEND;
                case '1':
                    k = ttgetc();
                    switch( k ) {
                        case '5': k=ttgetc(); if(k=='~') return KF5;
                        case '7': k=ttgetc(); if(k=='~') return KF6;
                        case '8': k=ttgetc(); if(k=='~') return KF7;
                        case '9': k=ttgetc(); if(k=='~') return KF8;
                        default: return KRANDOM;
                    }
                case '2':
                    k = ttgetc();
                    switch( k ) {
                        case '0': k=ttgetc(); if(k=='~') return KF9;
                        case '1': k=ttgetc(); if(k=='~') return KF10;
                        case '3': k=ttgetc(); if(k=='~') return KF11;
                        case '4': k=ttgetc(); if(k=='~') return KF12;
                        default: return KRANDOM;
                    }
                case '3':  k=ttgetc(); if(k=='~') return KDELETE;
                case '5':  k=ttgetc(); if(k=='~') return KPGUP;
                case '6':  k=ttgetc(); if(k=='~') return KPGDOWN;

                default:   return KRANDOM;
			}

		    default:		/* Actual ESC key depressed */
			if (ISLOWER(k) != FALSE)
				k = TOUPPER(k);
			if (k>=0x00 && k<=0x1F)		/* Relocate control. */
				k = KCTRL | (k+'@');
			return KMETA | k;
		}

	} else {

		if( k&0x80 ) {
			k &= 0x7f;
			if (ISLOWER(k) != FALSE)
				k = TOUPPER(k);
			if (k>=0x00 && k<=0x1F)		/* Relocate control. */
				k = KCTRL | (k+'@');
			return KMETA | k;
		} else
			return k;

	}
}

/*
    Establish default keypad bindings
*/
void
ttykeymapinit()
{
	keydup(KHOME,	"goto-bol");
    keydup(KEND,	"goto-eol");
    keydup(KUP,     "back-line");
	keydup(KLEFT,	"back-char");
	keydup(KDOWN,	"forw-line");
	keydup(KRIGHT,	"forw-char");
	keydup(KPGUP,	"back-page");
	keydup(KPGDOWN, "forw-page");
    keydup(KBKSP,	"back-del-char");
	keydup(KDELETE,	"forw-del-char");
    keydup(KF1,     "display-bindings");
    keydup(KF5,     "refresh");
    keydup(KF10,    "file-save");
    keydup(KF12,    "quit");
}
