#
# File: Makefile
# Date: 19-Aug-1999
# By  : Kevin Esteb
#
# This is the makefile to create the tools library.
#
#
CC = gcc
CFLAGS = -Wall -c -I ../../
OLBS = ../lib/libmisc.a
LINK = gcc
LFLAGS = -o
LIBS = ../lib/libmisc.a
LIBR = ar -r
#
OBJS = dates.o left.o ltrim.o mid.o xinstr.o xindex.o \
       pos.o right.o rtrim.o seg.o soundex.o spaces.o \
       stricmp.o string.o strlwr.o strnicmp.o strrev.o \
       strupr.o trim.o xmalloc.o xstrtok.o \
       ltostr.o stripcr.o sys_date.o \
       xgetline.o crc.o hash.o xgetch.o \
       getche.o
#
all: $(OBJS)
#
debug:
#
clean:
	rm -f *.o
	rm -f *~
	rm -f $(LIBS)
#
dates.o: dates.c dates.h
	$(CC) $(CFLAGS) dates.c
	$(LIBR) $(OLBS) dates.o
#
sys_date.o: sys_date.c dates.h
	$(CC) $(CFLAGS) sys_date.c
	$(LIBR) $(OLBS) sys_date.o
#
left.o: left.c
	$(CC) $(CFLAGS) left.c
	$(LIBR) $(OLBS) left.o
#
ltrim.o: ltrim.c
	$(CC) $(CFLAGS) ltrim.c
	$(LIBR) $(OLBS) ltrim.o
#
mid.o: mid.c
	$(CC) $(CFLAGS) mid.c
	$(LIBR) $(OLBS) mid.o
#
pos.o: pos.c
	$(CC) $(CFLAGS) pos.c
	$(LIBR) $(OLBS) pos.o
#
xinstr.o: xinstr.c
	$(CC) $(CFLAGS) xinstr.c
	$(LIBR) $(OLBS) xinstr.o
#
xindex.o: xindex.c
	$(CC) $(CFLAGS) xindex.c
	$(LIBR) $(OLBS) xindex.o
#
right.o: right.c
	$(CC) $(CFLAGS) right.c
	$(LIBR) $(OLBS) right.o
#
rtrim.o: rtrim.c
	$(CC) $(CFLAGS) rtrim.c
	$(LIBR) $(OLBS) rtrim.o
#
seg.o: seg.c
	$(CC) $(CFLAGS) seg.c
	$(LIBR) $(OLBS) seg.o
#
soundex.o: soundex.c
	$(CC) $(CFLAGS) soundex.c
	$(LIBR) $(OLBS) soundex.o
#
spaces.o: spaces.c
	$(CC) $(CFLAGS) spaces.c
	$(LIBR) $(OLBS) spaces.o
#
stricmp.o: stricmp.c
	$(CC) $(CFLAGS) stricmp.c
	$(LIBR) $(OLBS) stricmp.o
#
string.o: string.c
	$(CC) $(CFLAGS) string.c
	$(LIBR) $(OLBS) string.o
#
strlwr.o: strlwr.c
	$(CC) $(CFLAGS) strlwr.c
	$(LIBR) $(OLBS) strlwr.o
#
strnicmp.o: strnicmp.c
	$(CC) $(CFLAGS) strnicmp.c
	$(LIBR) $(OLBS) strnicmp.o
#
strrev.o: strrev.c
	$(CC) $(CFLAGS) strrev.c
	$(LIBR) $(OLBS) strrev.o
#
strupr.o: strupr.c
	$(CC) $(CFLAGS) strupr.c
	$(LIBR) $(OLBS) strupr.o
#
trim.o: trim.c
	$(CC) $(CFLAGS) trim.c
	$(LIBR) $(OLBS) trim.o
#
xmalloc.o: xmalloc.c
	$(CC) $(CFLAGS) xmalloc.c
	$(LIBR) $(OLBS) xmalloc.o
#
xstrtok.o: xstrtok.c
	$(CC) $(CFLAGS) xstrtok.c
	$(LIBR) $(OLBS) xstrtok.o
#
wordwrap.o: wordwrap.c
	$(CC) $(CFLAGS) wordwrap.c
	$(LIBR) $(OLBS) wordwrap.o
#
ltostr.o: ltostr.c
	$(CC) $(CFLAGS) ltostr.c
	$(LIBR) $(OLBS) ltostr.o
#
xgetline.o: xgetline.c
	$(CC) $(CFLAGS) xgetline.c
	$(LIBR) $(OLBS) xgetline.o
#
hash.o: hash.c
	$(CC) $(CFLAGS) hash.c
	$(LIBR) $(OLBS) hash.o
#
stripcr.o: stripcr.c
	$(CC) $(CFLAGS) stripcr.c
	$(LIBR) $(OLBS) stripcr.o
#
xgetch.o: xgetch.c
	$(CC) $(CFLAGS) xgetch.c
	$(LIBR) $(OLBS) xgetch.o
#
getche.o: getche.c
	$(CC) $(CFLAGS) getche.c
	$(LIBR) $(OLBS) getche.o
#
# eof
#
