
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _NET_H
#define _NET_H


#include "xas/types.h"
#include "xas/object.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _net_s net_t;

struct _net_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(net_t *, net_t *);
    int (*_override)(net_t *, item_list_t *);
    int (*_answer)(net_t *, double);
    int (*_call)(net_t *, int, int);
    int (*_complete)(net_t *, double, int);
    int (*_fd)(net_t *, int *);
    int (*_isreadable)(net_t *, int *);
    int (*_isup)(net_t *, int *);
    int (*_iswriteable)(net_t *, int *);
    int (*_listen)(net_t *, char *, int);
    int (*_name)(net_t *, char *);
    int (*_read)(net_t, double, int, void *, int *);
    int (*_request_pending)(net_t *, int *);
    int (*_setbuf)(net_t *, int, int);
    int (*_write)(net_t *, double, int, void *, int *);
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define NET(x) ((net_t *)(x))

#define NET_M_DESTRUCTOR       1
#define NET_M_ANSWER           2
#define NET_M_CALL             3
#define NET_M_COMPLETE         4
#define NET_M_FD               5
#define NET_M_ISREADABLE       6
#define NET_M_ISUP             7
#define NET_M_ISWRITEABLE      8
#define NET_M_LISTEN           9
#define NET_M_NAME             10
#define NET_M_READ             11
#define NET_M_REQUEST_PENDING  12
#define NET_M_SETBUF           13
#define NET_M_WRITE            14

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern net_t *net_create(item_list_t *);
extern int net_destroy(net_t *);
extern int net_compare(net_t *, net_t *);
extern int net_override(net_t *, item_list_t *);
extern char *net_version(net_t *);

extern int net_answer(net_t *, double);
extern int net_call(net_t *, int, int);
extern int net_complete(net_t *, double, int);
extern int net_fd(net_t *, int *);
extern int net_isreadable(net_t *, int *);
extern int net_isup(net_t *, int *);
extern int net_iswriteable(net_t *, int *);
extern int net_listen(net_t *, char *, int);
extern int net_name(net_t *, char *);
extern int net_read(net_t, double, int, void *, int *);
extern int net_request_pending(net_t *, int *);
extern int net_setbuf(net_t *, int, int);
extern int net_write(net_t *, double, int, void *, int *);

#define net_set_trace(self, trace)    object_set_trace(OBJECT(self), trace)

#endif

