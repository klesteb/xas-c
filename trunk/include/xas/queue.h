
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       que_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the QUE package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the QUE utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef XAS_QUE_UTIL_H_
#define XAS_QUE_UTIL_H_

#ifdef __cplusplus
    extern  "C" {
#endif

/*----------------------------------------------------------------------*/
/*    QUE Structures and Definitions.                                   */
/*----------------------------------------------------------------------*/

typedef struct nodeptr datanode;

typedef struct nodeptr {
    void     *data;
    datanode *prev;
    datanode *next;
} node;

typedef struct {
    node     *head;
    node     *tail;
    node     *cursor;
    int       size;
    int       sorted;
    int       item_deleted;
    void     **index; 
    datanode **posn_index; 
} queue_t;

typedef  struct {
    void  *dataptr;
    node  *loc;
} index_elt;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

typedef int (comp_proc)(void *a, void *b);

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

extern int    que_init(queue_t  *q);
extern int    que_empty(queue_t *q);
extern int    que_size(queue_t *q);
extern int    que_start(queue_t *q);
extern int    que_end(queue_t *q);
extern int    que_push_head(queue_t *q, void *d);
extern int    que_push_tail(queue_t *q, void *d);
extern void  *que_first(queue_t *q);
extern void  *que_last(queue_t *q);
extern void  *que_pop_head(queue_t *q);
extern void  *que_pop_tail(queue_t *q);
extern void  *que_next(queue_t *q);
extern void  *que_prev(queue_t *q);
extern void  *que_delete(queue_t *q);
extern void  *que_get(queue_t *q);
extern int    que_put(queue_t *q, void *d);
extern int    que_insert(queue_t *q, void *d);
extern int    que_sort(queue_t *q, comp_proc *Comp);
extern int    que_find(queue_t *q, void *data, comp_proc *Comp);
extern void  *que_seek(queue_t *q, void *data, comp_proc *Comp);

#ifdef __cplusplus
    }
#endif

#endif

