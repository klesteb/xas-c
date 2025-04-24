#
# File: Makefile
# Date: 07-Jun-2019
# By  : Kevin Esteb
#
# Makefile to build logger.
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
STATIC = ../lib/liblogger.a
SHARED = ../lib/liblogger.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
#
OLBS =  $(STATIC) ../lib/libobjects.a  -llog4c
#
INCS = ../xas/logger.h ../xas/log4c_extensions.h
#
OBJS = logger.o log4c_extensions.o
#
all: $(OBJS) test1 test2 test3 test4 test5 test6
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f ../lib/liblogger.a
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test5
#
#
test1: test1.o
	$(CC) $(CFLAGS) test1.c
	$(LINK) $(LOPTS) test1 test1.o  $(OLBS)
#
test2: test2.o
	$(CC) $(CFLAGS) test2.c
	$(LINK) $(LOPTS) test2 test2.o  $(OLBS)
#
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LOPTS) test3 test3.o  $(OLBS)
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LOPTS) test4 test4.o  $(OLBS)
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LOPTS) test5 test5.o  $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LOPTS) test6 test6.o  $(OLBS)
#
logger.o: logger.c $(INCS)
	$(CC) $(CFLAGS) logger.c
	$(LIBR) $(STATIC) logger.o
#
log4c_extensions.o: log4c_extensions.c $(INCS)
	$(CC) $(CFLAGS) log4c_extensions.c
	$(LIBR) $(STATIC) log4c_extensions.o
#
man:
	pod2man -c " " -r "logger(3)" -s 3 logger.pod logger.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) logger.o logger.c
	$(LINK) $(SOPTS) $(SHARED) logger.o
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) log4c_extensions.o log4c_extensions.c
	$(LINK) $(SOPTS) $(SHARED) log4c_extensions.o
#
# eof
#
