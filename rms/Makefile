#
# File: Makefile
# Date: 08-Apr-2021
# By  : Kevin Esteb
#
# Makefile to build the rms libraries.
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
STATIC = ../lib/librms.a
SHARED = ../lib/librms.so.$(VERSION)
INCLUDE = -I ../
CFLAGS = -DVERSION=\"$(VERSION)\" -Wall -c $(INCLUDE)
LOPTS = -o 
SOPTS = -shared -o 
SFLAGS = -L$(WORKING) -Wl,-rpath=$(WORKING) -fPIC 
MAN = ../man
#
OLBS = $(STATIC) ../lib/libtracer.a ../lib/liberrors.a ../lib/libobjects.a ../lib/libmisc.a ../lib/libqueue.a  ../lib/libgpl.a
#
INCS = btree.h ../xas/rms/fib.h ../xas/rms/lf.h ../xas/rms/blk.h ../xas/rms/rel.h
#
OBJS = btree.o fib.o lf.o blk.o rel.o
#
TESTS = test1 test2 test3 test4 test5 test6 test7 test8 rel-test1 \
        rel-test2 rel-test3 rel-test4
#
all: $(OBJS) $(TESTS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(STATIC)
	rm -f $(SHARED)
	rm -f $(TESTS)
	rm -f *.3
#
btree.o: btree.c $(INCS)
	$(CC) $(CFLAGS) btree.c
	$(LIBR) $(STATIC) btree.o
#
fib.o: fib.c $(INCS)
	$(CC) $(CFLAGS) fib.c
	$(LIBR) $(STATIC) fib.o
#
lf.o: lf.c $(INCS)
	$(CC) $(CFLAGS) lf.c
	$(LIBR) $(STATIC) lf.o
#
blk.o: blk.c $(INCS)
	$(CC) $(CFLAGS) blk.c
	$(LIBR) $(STATIC) blk.o
#
rel.o: rel.c $(INCS)
	$(CC) $(CFLAGS) rel.c
	$(LIBR) $(STATIC) rel.o
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
test3: test3.o
	$(CC) $(CFLAGS) test3.c
	$(LINK) $(LOPTS) test3 test3.o $(OLBS)
#
test4: test4.o
	$(CC) $(CFLAGS) test4.c
	$(LINK) $(LOPTS) test4 test4.o $(OLBS)
#
test5: test5.o
	$(CC) $(CFLAGS) test5.c
	$(LINK) $(LOPTS) test5 test5.o $(OLBS)
#
test6: test6.o
	$(CC) $(CFLAGS) test6.c
	$(LINK) $(LOPTS) test6 test6.o $(OLBS)
#
test7: test7.o
	$(CC) $(CFLAGS) test7.c
	$(LINK) $(LOPTS) test7 test7.o $(OLBS)
#
test8: test8.o
	$(CC) $(CFLAGS) test8.c
	$(LINK) $(LOPTS) test8 test8.o $(OLBS)
#
rel-test1: rel-test1.o 
	$(CC) $(CFLAGS) rel-test1.c
	$(LINK) $(LOPTS) rel-test1 rel-test1.o $(OLBS)
#
rel-test2: rel-test2.o rel-test.h
	$(CC) $(CFLAGS) rel-test2.c
	$(LINK) $(LOPTS) rel-test2 rel-test2.o $(OLBS)
#
rel-test3: rel-test3.o rel-test.h
	$(CC) $(CFLAGS) rel-test3.c
	$(LINK) $(LOPTS) rel-test3 rel-test3.o $(OLBS)
#
rel-test4: rel-test4.o rel-test.h
	$(CC) $(CFLAGS) rel-test4.c
	$(LINK) $(LOPTS) rel-test4 rel-test4.o $(OLBS)
#
man:
	pod2man -c " " -r "fib(3)" -s 3 fib.pod fib.3
	pod2man -c " " -r "lf(3)" -s 3 lf.pod lf.3
	pod2man -c " " -r "blk(3)" -s 3 blk.pod blk.3
	pod2man -c " " -r "rel(3)" -s 3 rel.pod rel.3
	pod2man -c " " -r "btree(3)" -s 3 btree.pod btree.3
#
shared:
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) fib.o fib.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) lf.o lf.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) blk.o blk.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) rel.o rel.c
	$(CC) $(CFLAGS) $(SFLAGS) $(LOPTS) btree.o btree.c
	$(LINK) $(SOPTS) $(SHARED) $(OBJS)
#
# eof
#
