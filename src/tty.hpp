/*
	uemacs - Terminal control codes for ANSI virtual terminals

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT
	DISCLAIMERS.


	This code simulates scrolling regions by using the insert line and delete
	line functions.
 */


#define	BEL	0x07			/* BEL character.		*/
#define	LF	0x0A			/* Line feed.			*/

extern	int	ttrow;
extern	int	ttcol;
extern	int	tttop;
extern	int	ttbot;
extern	int	tthue;

int	tceeol	=	2;		/* Costs.			*/
int	tcinsl	=	11;
int	tcdell	=	11;

/*
	Send xterm-color "smcup" to go to alternative screen.
*/
void
ttinit()
{
	ttputc(ESC);		// Save Cursor
	ttputc('7');
	ttputc(ESC);		// Switch to alternate screen buffer
	ttputc('[');
	ttputc('?');
	ttputc('4');
	ttputc('7');
	ttputc('h');
}

/*
	Send xterm-color "rmcup" to restore main screen.
*/
void
tttidy()
{
	ttputc(ESC);		// Clear screen
	ttputc('[');
	ttputc('2');
	ttputc('J');
	ttputc(ESC);		// Switch to normal screen buffer (from alternate)
	ttputc('[');
	ttputc('?');
	ttputc('4');
	ttputc('7');
	ttputc('l');
	ttputc(ESC);		// Restore Cursor
	ttputc('8');
}

/*
	Move the cursor to the specified origin 0 row and column position.
*/
void
ttmove(int row, int col)
{
	if (ttrow!=row || ttcol!=col) {
		if (row > nrow)
			row = nrow;
		if (col > ncol)
			col = ncol;
		row++; col++;
		ttputc(ESC);
		ttputc('[');
		if (row>99)
			ttputc(row/100+'0');
		ttputc((row%100)/10+'0');
		ttputc(row%10+'0');
		ttputc(';');
		if (col>99)
			ttputc(col/100+'0');
		ttputc((col%100)/10+'0');
		ttputc(col%10+'0');
		ttputc('f');
		ttrow = --row;
		ttcol = --col;
	}
}

/*
 Erase to end of line.
 */
void
tteeol()
{
	ttputc(ESC);
	ttputc('[');
	ttputc('0');
	ttputc('K');
}

/*
    Erase to end of page.
*/
void
tteeop()
{
	ttputc(ESC);
	ttputc('[');
	ttputc('0');
	ttputc('J');
}

/*
    Make a noise.
*/
void
ttbeep()
{
	ttputc(BEL);
	ttflush();
}

/*
    Insert nchunk blank line(s) onto the screen, scrolling the last line on the
    screen off the bottom.
*/
void
ttinsl(int row, int bot, int nchunk)
{
	if (row == bot) {
		ttmove(row, 0);
		tteeol();
		return;
	}
	ttmove(1+bot-nchunk, 0);
	ttputc(ESC);
	ttputc('[');
	ttputc(nchunk/10+'0');
	ttputc(nchunk%10+'0');
	ttputc('M');

	ttmove(row, 0);
	ttputc(ESC);
	ttputc('[');
	ttputc(nchunk/10+'0');
	ttputc(nchunk%10+'0');
	ttputc('L');

	ttrow = row;
	ttcol = 0;
}

/*
    Delete nchunk line(s) "row", replacing the bottom line on the screen with a
    blank line. This is done with a crafty sequence of insert and delete lines.
    The presence of the echo area makes a boundry condition go away.
*/
void
ttdell(int row, int bot, int nchunk)
{
	if (row == bot) {
		ttmove(row, 0);
		tteeol();
		return;
	}
	ttmove(row, 0);
	ttputc(ESC);
	ttputc('[');
	ttputc(nchunk/10+'0');
	ttputc(nchunk%10+'0');
	ttputc('M');

	ttmove(1+bot-nchunk,0);
	ttputc(ESC);
	ttputc('[');
	ttputc(nchunk/10+'0');
	ttputc(nchunk%10+'0');
	ttputc('L');

	ttrow = bot-nchunk;
	ttcol = 0;
}

/*
    No-op.
*/
void
ttwindow(top, bot)
{
}

/*
    No-op.
*/
void
ttnowindow()
{
}

/*
    Set display color for buffer text and mode lines
*/
void
ttcolor(int color)
{
	if (color != tthue) {
		if (color == CTEXT) {			// Normal video for text
			ttputc(ESC);
			ttputc('[');
			ttputc('0');
			ttputc('m');
		} else if (color == CMODE) {	// Underline for mode line
			ttputc(ESC);
			ttputc('[');
			ttputc('4');
			ttputc('m');
		}
		tthue = color;
	}
}

/*
    Handle virtual terminal resize
*/
void
ttresize()
{
    get_screen_size();
}
