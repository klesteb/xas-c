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

#ifndef REL_KLASS_H
#define REL_KLASS_H

#include "xas/object.h"

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

struct _rel_s {
    blk_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(rel_t *, rel_t *);
    int (*_override)(rel_t *, item_list_t *);

    int (*_init)(rel_t *);
    int (*_open)(rel_t *, int, mode_t);
    int (*_remove)(rel_t *);
    int (*_del)(rel_t *, off_t);
    int (*_add)(rel_t *, void *);
    int (*_extend)(rel_t *, int);
    int (*_record)(rel_t *, off_t *);
    int (*_get)(rel_t *, off_t, void *);
    int (*_put)(rel_t *, off_t, void *);
    int (*_read_header)(rel_t *);
    int (*_write_header)(rel_t *);
    int (*_update_header)(rel_t *);
    int (*_master_lock)(rel_t *);
    int (*_master_unlock)(rel_t *);
    int (*_build)(rel_t *, void *, void *);
    int (*_next)(rel_t *, rel_record_t *, ssize_t *);
    int (*_prev)(rel_t *, rel_record_t *, ssize_t *);
    int (*_last)(rel_t *, rel_record_t *, ssize_t *);
    int (*_first)(rel_t *, rel_record_t *, ssize_t *);
    int (*_normalize)(rel_t *, void *, void *);
    int (*_find)(rel_t *, void *, int (*compare)(void *, void *), off_t *);
    int (*_search)(rel_t *, void *, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue_t *), queue_t *);

    int record;
    int records;
    int recsize;
    int lastrec;
    int autoextend;
    int master_locked;
    struct flock master;
};

#endif

