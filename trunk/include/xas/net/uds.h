
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

#ifndef _UDS_H
#define _UDS_H


#include "xas/types.h"
#include "xas/object.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _uds_s uds_t;

struct _uds_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(uds_t *, uds_t *);
    int (*_override)(uds_t *, item_list_t *);
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define UDS(x) ((uds_t *)(x))

#define UDS_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern uds_t *uds_create(item_list_t *);
extern int uds_destroy(uds_t *);
extern int uds_compare(uds_t *, uds_t *);
extern int uds_override(uds_t *, item_list_t *);
extern char *uds_version(uds_t *);

#endif

