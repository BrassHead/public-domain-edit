/*
    uemacs - Generic *nix Terminal I/O.

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.
*/

#include <stdio.h>
#include <sgtty.h>
#include <unistd.h>

#define FDIN 0
#define FDOUT 1

int     nrow;				/* Terminal size, rows.		*/
int     ncol;				/* Terminal size, columns.	*/
char	buff[1];			/* I/O buffer for read/write	*/
struct  sgttyb sgt;

void clamp_size(int *nrow, int *ncol)
{
    if (*nrow>NROW) {
        *nrow = NROW;
    }
    if (*ncol>NCOL) {
        *ncol = NCOL;
    }
}

void get_screen_size()
{
    struct winsize win_size;

    if (ioctl(FDIN, TIOCGWINSZ, &win_size)!=-1) {
        nrow = win_size.ws_row;
        ncol = win_size.ws_col;
    } else {
        nrow = 24;
        ncol = 80;
    }
    clamp_size(&nrow, &ncol);
}

int screen_size_changed()
{
    struct winsize win_size;
    int newnrow = 24;
    int newncol =80;

    if (ioctl(FDIN, TIOCGWINSZ, &win_size)!=-1) {
        newnrow = win_size.ws_row;
        newncol = win_size.ws_col;
    }
    clamp_size(&newnrow, &newncol);

    return newnrow!=nrow || newncol!=ncol;
}

/*
    Open. Determine the size of the virtual terminal and use
    ioctl for raw mode.
*/
void ttopen()
{
	ioctl(FDIN,TIOCGETP,&sgt);
	sgt.sg_flags |= RAW;
	sgt.sg_flags &= ~ECHO;
	ioctl(FDIN,TIOCSETP,&sgt);

    get_screen_size();
}

/*
    reset ioctl
*/
void ttclose()
{
	sgt.sg_flags &= ~RAW;
	sgt.sg_flags |= ECHO;
	ioctl(FDIN,TIOCSETP,&sgt);
}

/*
    Put character.
*/
void ttputc(int c)
{
	putchar(c & 0x7F);
}

/*
    Flush output
*/
void ttflush()
{
	fflush(stdout);
}

/*
    Get characters.
*/
int ttgetc()
{
	read(FDIN,buff,1);
	return (buff[0]);
}
