
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

#ifndef _BLK_H
#define _BLK_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "xas/object.h"
#include "xas/types.h"
#include "xas/rms/fib.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _blk_s blk_t;

struct _blk_s {
    fib_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(blk_t *, blk_t *);
    int (*_override)(blk_t *, item_list_t *);
    int (*_seek)(blk_t *, off_t, int);
    int (*_tell)(blk_t *, off_t *);
    int (*_read)(blk_t *, void *, size_t, ssize_t *);
    int (*_write)(blk_t *, void *, size_t, ssize_t *);
    int (*_lock)(blk_t *, off_t, off_t);
    int (*_unlock)(blk_t *);

    int timeout;
    int retries;
    struct flock lock;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define BLK(x) ((blk_t *)(x))

#define BLK_K_RETRIES    2
#define BLK_K_TIMEOUT    3

#define BLK_M_DESTRUCTOR 11
#define BLK_M_SEEK       12
#define BLK_M_TELL       13
#define BLK_M_READ       14
#define BLK_M_WRITE      15
#define BLK_M_LOCK       16
#define BLK_M_UNLOCK     17

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern blk_t *blk_create(char *, int, int);
extern int blk_destroy(blk_t *);
extern int blk_compare(blk_t *, blk_t *);
extern int blk_override(blk_t *, item_list_t *);
extern char *blk_version(blk_t *);

extern int blk_seek(blk_t *, off_t, int);
extern int blk_tell(blk_t *, off_t *);
extern int blk_read(blk_t *, void *, size_t, ssize_t *);
extern int blk_write(blk_t *, void *, size_t, ssize_t *);
extern int blk_lock(blk_t *, off_t, off_t);
extern int blk_unlock(blk_t *);

extern int blk_get_retries(blk_t*, int *);
extern int blk_set_retries(blk_t *, int);
extern int blk_get_timeout(blk_t*, int *);
extern int blk_set_timeout(blk_t *, int);

#define blk_open(self, flags, mode) fib_open(FIB(self), flags, mode)
#define blk_close(self)             fib_close(FIB(self))
#define blk_exists(self, flag)      fib_exists(FIB(self), flag)
#define blk_size(self, length)      fib_size(FIB(self), length)
#define blk_stat(self, stat)        fib_stat(FIB(self), stat)
#define blk_unlink(self)            fib_unlink(FIB(self))
#define blk_get_fd(self, fd)        fib_get_fd(FIB(self), fd)

#endif

