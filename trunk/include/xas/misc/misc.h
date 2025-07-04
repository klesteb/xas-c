
#ifndef _XAS_MISC_H_
#define _XAS_MISC_H_

#include <string.h>
#include <ctype.h>
#include "xas/queue.h"

#ifndef TRUE
#define TRUE (-1)
#define FALSE (0)
#endif

/*----------------------------------------------------------------------*/
/* DEC Basic - string handling routines                                 */
/*----------------------------------------------------------------------*/

char *spaces(int);
char *trim(char *);
char *rtrim(char *);
char *ltrim(char *);
char *string(int, int);
char *left(const char *, int);
char *right(const char *, int);
char *mid(const char *, int, int);
char *seg(const char *, int, int);

int pos(const char *, const char *, int);
int xindex(const char *, const char, int);
int xinstr(int, const char *, const char *);

/*----------------------------------------------------------------------*/
/* Misc string routines                                                 */
/*----------------------------------------------------------------------*/

char *strlwr(char *);
char *strupr(char *);
char *strrev(char *);
char *stripcr(char *);
char *soundex(char *, char *);
char *fmtasc(char *, char *);
char *xstrtok(char *, char *);
char *ltostr(long, char *, size_t, unsigned);

int stricmp(const char *, const char *);
int wordwrap(const char *, int, queue_t *);
int strnicmp(const char *, const char *, int);

void insert(char *, char *, int, int);
void hexdump(unsigned char *, unsigned long, unsigned long);

/*----------------------------------------------------------------------*/
/* Borland conio routines                                               */
/*----------------------------------------------------------------------*/

int xgetch(void);
int getche(void);

/*----------------------------------------------------------------------*/
/* System routines                                                      */
/*----------------------------------------------------------------------*/

int xgetline(int, char *, int, int);

void *xmalloc(int);
void syserr(char *, ...);

/*----------------------------------------------------------------------*/
/* crc routines                                                         */
/*----------------------------------------------------------------------*/

unsigned long crc32(char *, int);

unsigned short crc16(char *, int);
unsigned short crc_ccitt(char *, int);

/*----------------------------------------------------------------------*/
/* hash routines                                                        */
/*----------------------------------------------------------------------*/

long pal_hash(const unsigned char *);

unsigned long elf_hash(const unsigned char *);

#endif

