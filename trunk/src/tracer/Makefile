#
# File: Makefile
# Date: 07-Oct-2020
# By  : Kevin Esteb
#
# Makefile to build tracer.
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
STATIC = ../lib/libtracer.a
SHARED = libtracer.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
MAN = ../man/tracer.3
#
OLBS = $(STATIC) ../lib/liberrors.a ../lib/libobjects.a ../lib/libmisc.a ../lib/libqueue.a
#
INCS = ../xas/tracer.h
#
OBJS = tracer.o
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
	rm -f $(MAN)
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
tracer.o: tracer.c $(INCS)
	$(CC) $(CFLAGS) tracer.c
	$(LIBR) $(STATIC) tracer.o
#
man:
	pod2man -c " " -r "tracer(3)" -s 3 tracer.pod $(MAN)
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) tracer.o tracer.c
	$(LINK) $(SOPTS) $(SHARED) tracer.o
#
# eof
#
