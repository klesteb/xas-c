#
# File: Makefile
# Date: 27-Nov-2019
# By  : Kevin Esteb
#
# Makefile to build template.
#
MAJOR=0
MINOR=1
PATCH=0
VERSION="$(MAJOR).$(MINOR).$(PATCH)"
#
CC = gcc
LINK = gcc
LIBR = ar rs
WORKING = $(shell pwd)
STATIC = libtemplate.a
SHARED = libtemplate.so.$(VERSION)
INCLUDE = -I ../../../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS = $(STATIC) ../../../lib/libobjects.a  
#
INCS = template.h
#
OBJS = template.o
#
all: $(OBJS) test1 test2 test3
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f template.3
	rm -f test1
	rm -f test2
	rm -f test3
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o $(OLBS)
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LOPTS) test3 test3.o $(OLBS)
#
template.o: template.c $(INCS)
	$(CC) $(CFLAGS) template.c
	$(LIBR) $(STATIC) template.o
#
man:
	pod2man -c " " -r "template(3)" -s 3 template.pod template.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) template.o template.c
	$(LINK) $(SOPTS) $(SHARED) template.o
#
# eof
#
