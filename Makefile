#
# Stupid simple makefile
#
all:
	cc -o ../build/ue main.c

clean:
	rm -rf ../build/*

install:
	cp ../build/ue /usr/local/bin
