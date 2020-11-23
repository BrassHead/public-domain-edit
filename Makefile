#
# Cross platform Makefile
#

TARGETS = all build clean install debug

$(TARGETS):
	make -f Makefile.`uname` $@

