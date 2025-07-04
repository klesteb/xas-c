#
# File: Makefile.linux
# Date: 26-Nov-2005
# By  : Kevin Esteb
#
# Makefile to build libwin.olb.
#
CC = gcc
CFLAGS = -Wall -c -I.. -I../misc -I../../gpl
OLBS = libwindows.a ../lib/libmisc.a -l ncurses
LINK = gcc
LFLAGS = 
LIBR = ar -r
LIBS = libwindows.a
#
INCS = windows.h
#
OBJS = winalert.o winbar.o windows.o winforms.o  \
       wininput.o winlists.o winpd.o winpopup.o  \
       matchkey.o colors.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.a
	rm -f *.o
	rm -f *~
	rm -f *demo
#
demos: bardemo formdemo listdemo pdemo popdemo
tests: wintest kbtest
#
winalert.o: winalert.c $(INCS)
	$(CC) $(CFLAGS) winalert.c
	$(LIBR) $(LIBS) winalert.o

winbar.o: winbar.c $(INCS)
	$(CC) $(CFLAGS) winbar.c
	$(LIBR) $(LIBS) winbar.o
#
windows.o: windows.c $(INCS)
	$(CC) $(CFLAGS) windows.c
	$(LIBR) $(LIBS) windows.o
#
winedtbuff.o: winedtbuff.c $(INCS)
	$(CC) $(CFLAGS) winedtbuff.c
	$(LIBR) $(LIBS) winedtbuff.o
#
winforms.o: winforms.c $(INCS)
	$(CC) $(CFLAGS) winforms.c
	$(LIBR) $(LIBS) winforms.o
#
wingetedt.o: wingetedt.c $(INCS)
	$(CC) $(CFLAGS) wingetedt.c
	$(LIBR) $(LIBS) wingetedt.o
#
wininput.o: wininput.c $(INCS)
	$(CC) $(CFLAGS) wininput.c
	$(LIBR) $(LIBS) wininput.o
#
winlists.o: winlists.c $(INCS)
	$(CC) $(CFLAGS) winlists.c
	$(LIBR) $(LIBS) winlists.o
#
winpd.o: winpd.c $(INCS)
	$(CC) $(CFLAGS) winpd.c
	$(LIBR) $(LIBS) winpd.o
#
winpopup.o: winpopup.c $(INCS)
	$(CC) $(CFLAGS) winpopup.c
	$(LIBR) $(LIBS) winpopup.o
#
matchkey.o: matchkey.c $(INCS)
	$(CC) $(CFLAGS) matchkey.c
	$(LIBR) $(LIBS) matchkey.o
#
colors.o: colors.c $(INCS)
	$(CC) $(CFLAGS) colors.c
	$(LIBR) $(LIBS) colors.o
#
# Demo programs
#
bardemo: bardemo.c
	$(CC) $(CFLAGS) bardemo.c
	$(LINK) bardemo.o -o bardemo $(OLBS)
#
formdemo: formdemo.c
	$(CC) $(CFLAGS) formdemo.c
	$(LINK) formdemo.o -o formdemo $(OLBS)
#
listdemo: listdemo.c
	$(CC) $(CFLAGS) listdemo.c
	$(LINK) listdemo.o -o listdemo $(OLBS)
#
pdemo: pdemo.c
	$(CC) $(CFLAGS) pdemo.c
	$(LINK) pdemo.o -o pdemo $(OLBS)
#
popdemo: popdemo.c
	$(CC) $(CFLAGS) popdemo.c
	$(LINK) popdemo.o -o popdemo $(OLBS)
#
wintest: wintest.c
	$(CC) $(CFLAGS) wintest.c
	$(LINK) wintest.o -o wintest $(OLBS)
#
kbtest: kbtest.c
	$(CC) $(CFLAGS) kbtest.c
	$(LINK) kbtest.o -o kbtest $(OLBS)
#
# eof
#
