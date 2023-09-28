
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

#ifndef _FIB_H
#define _FIB_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "xas/object.h"
#include "xas/types.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _fib_s fib_t;

struct _fib_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(fib_t *, fib_t *);
    int (*_override)(fib_t *, item_list_t *);
    int (*_open)(fib_t *, int, mode_t);
    int (*_close)(fib_t *);
    int (*_read)(fib_t *, void *, size_t, ssize_t *);
    int (*_write)(fib_t *, void *, size_t, ssize_t *);
    int (*_exists)(fib_t *, int *);
    int (*_size)(fib_t *, off_t *);
    int (*_stat)(fib_t *, struct stat *);
    int (*_unlink)(fib_t *);
    int (*_seek)(fib_t *, off_t, int);
    int (*_tell)(fib_t *, off_t *);
    int (*_lock)(fib_t *, off_t, off_t);
    int (*_unlock)(fib_t *);

    int fd;
    int retries;
    int timeout;
    char path[1024];
    struct flock lock;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define FIB(x) ((fib_t *)(x))

#define FIB_K_PATH    1
#define FIB_K_TIMEOUT 2
#define FIB_K_RETRIES 3

#define FIB_M_DESTRUCTOR 1
#define FIB_M_OPEN       2
#define FIB_M_CLOSE      3
#define FIB_M_READ       4
#define FIB_M_WRITE      5
#define FIB_M_EXISTS     6
#define FIB_M_SIZE       7
#define FIB_M_STAT       8
#define FIB_M_UNLINK     9
#define FIB_M_LOCK       10
#define FIB_M_UNLOCK     11
#define FIB_M_SEEK       12
#define FIB_M_TELL       13

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern fib_t *fib_create(char *, int, int);
extern int fib_destroy(fib_t *);
extern int fib_compare(fib_t *, fib_t *);
extern int fib_override(fib_t *, item_list_t *);
extern char *fib_version(fib_t *);
extern int fib_open(fib_t *, int, mode_t);
extern int fib_close(fib_t *);
extern int fib_read(fib_t *, void *, size_t, ssize_t *);
extern int fib_write(fib_t *, void *, size_t, ssize_t *);
extern int fib_exists(fib_t *, int *);
extern int fib_lock(fib_t *, off_t, off_t);
extern int fib_seek(fib_t *, off_t, int);
extern int fib_size(fib_t *, off_t *);
extern int fib_stat(fib_t *, struct stat *);
extern int fib_tell(fib_t *, off_t *);
extern int fib_unlink(fib_t *);
extern int fib_unlock(fib_t *);
extern int fib_get_fd(fib_t *, int *);
extern int fib_get_retries(fib_t *, int *);
extern int fib_set_retries(fib_t *, int);
extern int fib_get_timeout(fib_t *, int *);
extern int fib_set_timeout(fib_t *, int);

#endif

