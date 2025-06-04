
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

#ifndef SEQ_KLASS_H
#define SEQ_KLASS_H

#include "xas/object.h"

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

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

#endif

