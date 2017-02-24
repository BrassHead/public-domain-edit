/*
    uemacs - Virtual terminal options

	No copyright 2016 (CC0-1.0 Public Domain)

	See http://creativecommons.org/publicdomain/zero/1.0/ and the attached file
	'CC0 1.0 Universal.txt' for a licence to use this software and IMPORTANT 
	DISCLAIMERS.
*/

#define	GOSLING	0			/* Use fancy redisplay.		*/
#define	MEMMAP	0			/* Not memory mapped video.	*/

/* Max screen size */

#define NROW	200
#define	NCOL	250

/*
    Special keys.
*/
#define	KF1     K01
#define	KF2     K02
#define	KF3     K03
#define	KF4     K04
#define	KF5     K05
#define	KF6     K06
#define	KF7     K07
#define	KF8     K08
#define	KF9     K09
#define	KF10	K0A
#define	KF11	K0B
#define	KF12	K0C
#define	KHOME	K0D
#define	KEND    K0E
#define	KUP     K0F
#define	KLEFT	K10
#define	KDOWN	K11
#define	KRIGHT	K12
#define KBKSP	K13         /* Mac Delete key that does backspace */
#define KDELETE K14         /* Mac fn-Delete or Delete on most keyboards */
#define KPGUP   K15
#define KPGDOWN K16
