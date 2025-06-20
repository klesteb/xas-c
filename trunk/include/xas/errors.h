
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _XAS_ERRORS_H_
#define _XAS_ERRORS_H_

#include "xas/object.h"
#include "xas/queue.h"
#include "xas/types.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _err_s err_t;

struct _err_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(err_t *, err_t *);
    int (*_override)(err_t *, item_list_t *);
    int (*_del_error)(err_t *, int);
    int (*_get_text)(err_t *, int, char *, int);
    int (*_get_message)(err_t *, int, char *, int);
    int (*_add_error)(err_t *, int, char *, char *);
    int (*_set_error)(err_t *, int, char *, char *);
    int (*_load_errors)(err_t *, error_code_t *, int);
    int (*_load_system_errors)(err_t *);
    queue_t errors;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define ERRORS(x) ((err_t *)(x))

#define ERR_K_ERROR_CODES  1

#define ERR_M_DESTRUCTOR  1
#define ERR_M_ADD         2
#define ERR_M_DEL         3
#define ERR_M_SET         4
#define ERR_M_LOAD        5
#define ERR_M_GET_TEXT    6
#define ERR_M_GET_MESSAGE 7

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern err_t *err_create(void);
extern int err_destroy(err_t *);
extern int err_remove(err_t *, int);
extern int err_compare(err_t *, err_t *);
extern int err_override(err_t *, item_list_t *);
extern char *err_version(err_t *);

extern int err_add(err_t *, int, char *, char *);
extern int err_set(err_t *, int, char *, char *);
extern int err_load(err_t *, error_code_t *, int);
extern int err_get_text(err_t *, int, char *, int);
extern int err_get_message(err_t *, int, char *, int);

#define err_set_trace(self, trace)    object_set_trace(OBJECT(self), trace)

#endif

