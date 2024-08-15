#
# File: Makefile
# Date: 25-Jan-2023
# By  : Kevin Esteb
#
# Makefile to build queue.
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
STATIC = ../lib/libqueue.a
SHARED = ../lib/libqueue.so.$(VERSION)
MAN = ../man/queue.3
INCLUDE = -I ../ 
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o  
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
INCS = ../xas/queue.h
#
OBJS = queue.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f $(MAN)
#
#
man:
	pod2man -c " " -r "queue(3)" -s 3 queue.pod $(MAN)
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) queue.o queue.c
	$(LINK) $(SOPTS) $(SHARED) queue.o
#
#test1: test1.o
#	$(CC) $(CFLAGS) test1.c
#	$(LINK) $(SFLAGS) test1 test1.o -lqueue $(OLBS)
#
#test2: test1.o
#	$(CC) $(CFLAGS) test1.c
#	$(LINK) $(LFLAGS) test2 test1.o $(STATIC) $(OLBS)
#
queue.o: queue.c $(INCS)
	$(CC) $(CFLAGS) queue.c
	$(LIBR) $(STATIC) queue.o
#
# eof
#
