#
# File: Makefile
# Date: 07-Jun-2019
# By  : Kevin Esteb
#
# Makefile to build factory.
#
CC = gcc
CFLAGS = -Wall -c -I../../../
OLBS = factory.o worker.o worker1.o worker2.o ../../../lib/libobjects.a ../../../lib/libqueue.a ../../../lib/libmisc.a 
LINK = gcc
LFLAGS = -o test
LIBR = ar -r
#
INCS = factory.h worker.h worker1.h worker2.h
#
OBJS = factory.o worker.o worker1.o worker2.o
#
all: $(OBJS) test
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f test
#
#
test: test.o
	$(CC) $(CFLAGS) test.c
	$(LINK) -o test test.o  $(OLBS)
#
factory.o: factory.c $(INCS)
	$(CC) $(CFLAGS) factory.c
#
worker.o: worker.c $(INCS)
	$(CC) $(CFLAGS) worker.c
#
worker1.o: worker1.c $(INCS)
	$(CC) $(CFLAGS) worker1.c
#
worker2.o: worker2.c $(INCS)
	$(CC) $(CFLAGS) worker2.c
#
# eof
#
