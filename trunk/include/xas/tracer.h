
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _XAS_TRACER_H_
#define _XAS_TRACER_H_

#include "xas/types.h"
#include "xas/object.h"
#include "xas/queue.h"
#include "xas/errors.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _tracer_s tracer_t;

struct _tracer_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(tracer_t *, tracer_t *);
    int (*_override)(tracer_t *, item_list_t *);
    int (*_add)(tracer_t *, error_trace_t *);
    int (*_dump)(tracer_t *, int (*output)(char *));
    err_t *errs;
    queue_t errors;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define TRACER(x) ((tracer_t *)(x))

#define TRACER_K_ERRORS 1

#define TRACER_M_DESTRUCTOR 1
#define TRACER_M_ADD        2
#define TRACER_M_DUMP       3

/*----------------------------------------------------------------*/
/* macros                                                         */
/*----------------------------------------------------------------*/

#define capture_error(trace) {                                  \
    if ((trace) != NULL) {                                      \
        error_trace_t *junk = calloc(1, sizeof(error_trace_t)); \
        if (junk != NULL) {                                     \
            junk->errnum = trace_errnum;                        \
            junk->lineno = trace_lineno;                        \
            junk->filename = strdup(trace_filename);            \
            junk->function = strdup(trace_function);            \
            tracer_add(trace, junk);                            \
        }                                                       \
    }                                                           \
    clear_error();                                              \
}

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern tracer_t *tracer_create(err_t *);
extern int tracer_destroy(tracer_t *);
extern int tracer_compare(tracer_t *, tracer_t *);
extern int tracer_override(tracer_t *, item_list_t *);
extern int tracer_add(tracer_t *, error_trace_t *);
extern int tracer_dump(tracer_t *, int (*output)(char *));
extern char *tracer_version(tracer_t *);

#endif

