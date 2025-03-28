
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

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include "xas/object.h"
#include "xas/types.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _template_s template_t;

struct _template_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(template_t *, template_t *);
    int (*_override)(template_t *, item_list_t *);
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define TEMPLATE(x) ((template_t *)(x))

#define TEMPLATE_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern template_t *template_create(item_list_t *);
extern int template_destroy(template_t *);
extern int template_compare(template_t *, template_t *);
extern int template_override(template_t *, item_list_t *);
extern char *template_version(template_t *);

#define template_set_trace(self, trace)    object_set_trace(OBJECT(self), trace)

#endif

