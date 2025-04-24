
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

#ifndef LF_KLASS_H
#define LF_KLASS_H

#include "xas/object.h"

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

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

#endif

