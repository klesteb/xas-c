
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

#ifndef QUE_UTIL_H
#define QUE_UTIL_H 1

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
} queue;

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

extern int    que_init(queue  *q);
extern int    que_empty(queue *q);
extern int    que_size(queue *q);
extern int    que_start(queue *q);
extern int    que_end(queue *q);
extern int    que_push_head(queue *q, void *d);
extern int    que_push_tail(queue *q, void *d);
extern void  *que_first(queue *q);
extern void  *que_last(queue *q);
extern void  *que_pop_head(queue *q);
extern void  *que_pop_tail(queue *q);
extern void  *que_next(queue *q);
extern void  *que_prev(queue *q);
extern void  *que_delete(queue *q);
extern void  *que_get(queue *q);
extern int    que_put(queue *q, void *d);
extern int    que_insert(queue *q, void *d);
extern int    que_sort(queue *q, comp_proc *Comp);
extern int    que_find(queue *q, void *data, comp_proc *Comp);
extern void  *que_seek(queue *q, void *data, comp_proc *Comp);

#ifdef __cplusplus
    }
#endif

#endif

