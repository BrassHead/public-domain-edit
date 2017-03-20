#
# Cross platform Makefile
#

TARGETS = all build clean install

$(TARGETS):
	make -f Makefile.`uname` $@

