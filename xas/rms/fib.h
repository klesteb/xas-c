
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
#include <errno.h>

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
    int (*_creat)(fib_t *, mode_t);
    int (*_chmod)(fib_t *, mode_t);
    int (*_close)(fib_t *);
    int (*_exists)(fib_t *, int *);
    int (*_size)(fib_t *, off_t *);
    int (*_stat)(fib_t *, struct stat *);
    int (*_unlink)(fib_t *);

    int fd;
    char path[1024];
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define FIB(x) ((fib_t *)(x))

#define FIB_K_PATH    1

#define FIB_M_DESTRUCTOR 1
#define FIB_M_OPEN       2
#define FIB_M_CLOSE      3
#define FIB_M_EXISTS     4
#define FIB_M_SIZE       5
#define FIB_M_STAT       6
#define FIB_M_UNLINK     7
#define FIB_M_CREAT      8
#define FIB_M_CHMOD      9

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern fib_t *fib_create(char *);
extern int fib_destroy(fib_t *);
extern int fib_compare(fib_t *, fib_t *);
extern int fib_override(fib_t *, item_list_t *);
extern char *fib_version(fib_t *);
extern int fib_open(fib_t *, int, mode_t);
extern int fib_creat(fib_t *, mode_t);
extern int fib_chmod(fib_t *, mode_t);
extern int fib_close(fib_t *);
extern int fib_exists(fib_t *, int *);
extern int fib_size(fib_t *, off_t *);
extern int fib_stat(fib_t *, struct stat *);
extern int fib_unlink(fib_t *);
extern int fib_get_fd(fib_t *, int *);

#endif

