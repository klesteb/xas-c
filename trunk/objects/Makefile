#
# File: Makefile
# Date: 07-Jun-2019
# By  : Kevin Esteb
#
# Makefile to build objects.
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
STATIC = ../lib/libobjects.a
SHARED = ../lib/libobjects.so.$(VERSION)
INCLUDE = -I ../ 
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
MAN = ../man/object.3
#
INCS = ../xas/object.h
#
OBJS = object.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)
	rm -f $(SHARED)
	rm -f $(MAN)
#
object.o: object.c $(INCS)
	$(CC) $(CFLAGS) object.c
	$(LIBR) $(STATIC) object.o
#
man:
	pod2man -c " " -r "object(3)" -s 3 object.pod $(MAN)
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) object.o object.c
	$(LINK) $(SOPTS) $(SHARED) object.o
#
# eof
#
