#
# File: Makefile
# Date: 24-Aug-2024
# By  : Kevin Esteb
#
# Makefile to build uds.
#
MAJOR=1
MINOR=0
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBR = ar rs
WORKING = $(shell pwd)
STATIC = libuds.a
SHARED = libuds.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../lib/libobjects.a ../lib/libgpl.a ../lib/libmisc.a 
#
INCS = ../xas/net/uds.h
#
OBJS = uds.o
#
all: $(OBJS) test1 test2
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f uds.3
	rm -f test1
	rm -f test2
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o $(OLBS)
#
uds.o: uds.c $(INCS)
	$(CC) $(CFLAGS) uds.c
	$(LIBR) $(STATIC) uds.o
#
man:
	pod2man -c " " -r "uds(3)" -s 3 uds.pod uds.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) uds.o uds.c
	$(LINK) $(SOPTS) $(SHARED) uds.o
#
# eof
#
