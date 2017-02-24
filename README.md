Public Domain Editor
====================

This is ue, a derivative of MicroEmacs, a public domain editor from the 
1980s. This version continues the public domain spirit of the original authors
by using the CC0-1.0 Universal Public Domain License. See the file 
'CC0 1.0 Universal.txt' for the license.

The code format, build style, and comments have been modernized, but the
majority of the code is little changed. This version only supports modern
keyboards and virtual terminals on *nix systems. The support for obsolete
computers and operating systems has been dropped. It is only tested on MacOS
with iTerm2.

The previous version in this branch of the code supported the Atari-ST in the
Minix operating system.

You can get a current keypad binding chart by entering the command

	ESC-X display-bindings

or by typing the F1 key.

If you want to change the key bindings edit:
- symbol.hpp		for ascii control keys (eg: C-x, C-s, etc.)
- ttykbd.hpp		for special keys (eg: cursor keys, function keys, etc.)

Note the following abbreviations:

	C-<something>		Control-<something>
	M-<something>		Meta -- generally the <ESC> key.
	F10					(etc. -- a keypad function key).

