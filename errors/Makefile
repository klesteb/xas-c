#
# File: Makefile
# Date: 09-Sep-2019
# By  : Kevin Esteb
#
# Makefile to build errors.
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
STATIC = ../lib/liberrors.a
SHARED = ../lib/liberrors.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
MAN = ../man/errors.3
#
OLBS = $(STATIC) ../lib/libobjects.a ../lib/libqueue.a ../lib/libmisc.a 
#
INCS = ../xas/errors.h ../xas/error_codes.h ../xas/errors_xas.h
#
OBJS = errors.o 
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
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o $(OLBS)
#
errors.o: errors.c $(INCS)
	$(CC) $(CFLAGS) errors.c
	$(LIBR) $(STATIC) errors.o
#
errnoname.o: errnoname.c $(INCS)
	$(CC) $(CFLAGS) errnoname.c
	$(LIBR) $(STATIC) errnoname.o
#
man:
	pod2man -c " " -r "errors(3)" -s 3 errors.pod $(MAN)
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) errors.o errors.c
	$(LINK) $(SOPTS) $(SHARED) errors.o
#
# eof
#
