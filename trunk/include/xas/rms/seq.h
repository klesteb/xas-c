
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

#ifndef _XAS_RMS_SEQ_H_
#define _XAS_RMS_SEQ_H_

#include "xas/rms/fib.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _seq_s seq_t;

struct _seq_s {
    fib_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(seq_t *, seq_t *);
    int (*_override)(seq_t *, item_list_t *);
    int (*_gets)(seq_t *, char *, size_t, ssize_t *);
    int (*_puts)(seq_t *, char *, ssize_t *);

    char *eol;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define SEQ(x) ((seq_t *)(x))

#define SEQ_M_DESTRUCTOR 18
#define SEQ_M_GETS       9
#define SEQ_M_PUTS       10

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern seq_t *seq_create(char *);
extern int seq_destroy(seq_t *);
extern int seq_compare(seq_t *, seq_t *);
extern int seq_override(seq_t *, item_list_t *);
extern char *seq_version(seq_t *);
extern int seq_gets(seq_t *, char *, size_t , ssize_t *);
extern int seq_puts(seq_t *, char *, ssize_t *);
extern int seq_get_eol(seq_t *, char *);
extern int seq_set_eol(seq_t *, char *);

#define seq_open(self, flags, mode) fib_open(FIB(self), flags, mode)
#define seq_close(self)             fib_close(FIB(self))
#define seq_exists(self, flag)      fib_exists(FIB(self), flag)
#define seq_size(self, length)      fib_size(FIB(self), length)
#define seq_stat(self, stat)        fib_stat(FIB(self), stat)
#define seq_unlink(self)            fib_unlink(FIB(self))
#define seq_get_fd(self, fd)        fib_get_fd(FIB(self), fd)
#define seq_creat(self, mode)       fib_creat(FIB(self), mode)
#define seq_chmod(self, mode)       fib_chmod(FIB(self). mode)
#define seq_set_trace(self, trace)  object_set_trace(OBJECT(self), trace)

#endif

