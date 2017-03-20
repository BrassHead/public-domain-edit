/*
    uemacs - Generic *nix Terminal I/O.

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.
*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define FDIN 0
#define FDOUT 1

int     nrow;				/* Terminal size, rows.		*/
int     ncol;				/* Terminal size, columns.	*/
char	buff[1];			/* I/O buffer for read/write	*/
static struct termios save_tio;

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
    Open. Determine the size of the virtual terminal and set
    for raw mode I/O.
*/
void ttopen()
{
    struct termios newt;

    if (tcgetattr(FDIN, &save_tio)<0) {
        exit(1);
    }
    newt = save_tio;
    newt.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
    newt.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR |
                      INPCK | ISTRIP | IXON | PARMRK);
    newt.c_oflag &= ~OPOST;
    newt.c_cc[VMIN] = 1;        // One byte at a time
    newt.c_cc[VTIME] = 0;       // No timer
    if (tcsetattr(FDIN, TCSAFLUSH, &newt)<0) {
        exit(1);
    }
    
    get_screen_size();
}

/*
    reset ioctl
*/
void ttclose()
{
    tcsetattr(FDIN, TCSAFLUSH, &save_tio);
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
