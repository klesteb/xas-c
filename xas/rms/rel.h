
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2023 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _REL_H
#define _REL_H

#include "xas/queue.h"
#include "xas/rms/blk.h"

/*-------------------------------------------------------------*/
/* klass data                                                  */
/*-------------------------------------------------------------*/

typedef struct record {
    unsigned char flags;
    void *data;
} rel_record_t;

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _rel_s rel_t;

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

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define REL(x) ((rel_t *)(x))

#define REL_K_RECORDS 4
#define REL_K_RECSIZE 5
#define REL_K_NAME    6

#define REL_M_DESTRUCTOR    18
#define REL_M_OPEN          19
#define REL_M_GET           20
#define REL_M_PUT           21
#define REL_M_NEXT          22
#define REL_M_PREV          23
#define REL_M_LAST          24
#define REL_M_FIRST         25
#define REL_M_BUILD         26
#define REL_M_EXTEND        27
#define REL_M_ADD           28
#define REL_M_DEL           29
#define REL_M_NORMALIZE     30
#define REL_M_FIND          31
#define REL_M_SEARCH        32
#define REL_M_RECORD        33
#define REL_M_INIT          34
#define REL_M_MASTER_LOCK   35
#define REL_M_MASTER_UNLOCK 36
#define REL_M_WRITE_HEADER  37
#define REL_M_READ_HEADER   38
#define REL_M_UPDATE_HEADER 39

#define REL_F_DELETED   1

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rel_t *rel_create(char *, char *, int, int, int, int);
extern int rel_destroy(rel_t *);
extern int rel_compare(rel_t *, rel_t *);
extern int rel_override(rel_t *, item_list_t *);
extern char *rel_version(rel_t *);

extern int rel_open(rel_t *, int, mode_t);
extern int rel_remove(rel_t *);
extern int rel_del(rel_t *, off_t);
extern int rel_add(rel_t *, void *);
extern int rel_extend(rel_t *, int);
extern int rel_record(rel_t *, off_t *);
extern int rel_get(rel_t *, off_t, void *);
extern int rel_put(rel_t *, off_t, void *);
extern int rel_find(rel_t *, void *, int (*compare)(void *, void *), off_t *);
extern int rel_search(rel_t *, void *, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue_t *), queue_t *);

#define rel_close(self)               blk_close(BLK(self))
#define rel_set_trace(self, trace)    object_set_trace(OBJECT(self), trace)

#endif

