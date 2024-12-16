#
# Cross platform Makefile
#

TARGETS = all clean install debug

$(TARGETS):
	make -f Makefile.`uname` $@

