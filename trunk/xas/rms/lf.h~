
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2023 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _LF_H
#define _LF_H

#include "xas/rms/fib.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _lf_s lf_t;

struct _lf_s {
    fib_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(lf_t *, lf_t *);
    int (*_override)(lf_t *, item_list_t *);
    int (*_gets)(lf_t *, char *, size_t, ssize_t *);
    int (*_puts)(lf_t *, char *, ssize_t *);

    char *eol;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define LF(x) ((lf_t *)(x))

#define LF_M_DESTRUCTOR 18
#define LF_M_GETS       9
#define LF_M_PUTS       10

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern lf_t *lf_create(char *);
extern int lf_destroy(lf_t *);
extern int lf_compare(lf_t *, lf_t *);
extern int lf_override(lf_t *, item_list_t *);
extern char *lf_version(lf_t *);
extern int lf_gets(lf_t *, char *, size_t , ssize_t *);
extern int lf_puts(lf_t *, char *, ssize_t *);
extern int lf_get_eol(lf_t *, char *);
extern int lf_set_eol(lf_t *, char *);

#define lf_open(self, flags, mode) fib_open(FIB(self), flags, mode)
#define lf_close(self)             fib_close(FIB(self))
#define lf_exists(self, flag)      fib_exists(FIB(self), flag)
#define lf_size(self, length)      fib_size(FIB(self), length)
#define lf_stat(self, stat)        fib_stat(FIB(self), stat)
#define lf_unlink(self)            fib_unlink(FIB(self))
#define lf_get_fd(self, fd)        fib_get_fd(FIB(self), fd)
#define lf_creat(self, mode)       fib_creat(FIB(self), mode)
#define lf_chmod(self, mode)       fib_chmod(FIB(self). mode)

#endif

