
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1998 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>

#include "xas/types.h"
#include "xas/queue.h"

/*----------------------------------------------------------------------*/

void *que_delete(queue_t *q) {

    void     *d = NULL; 
    datanode *n = NULL;
    datanode *p = NULL;
    datanode *c = NULL; 


    if (q->cursor == NULL) return(NULL); 
    if (q->cursor == q->head) return(que_pop_head(q)); 
    if (q->cursor == q->tail) return(que_pop_tail(q)); 

    n = q->cursor->next; 
    p = q->cursor->prev; 
    d = q->cursor->data; 
    c = q->cursor;

    if (p != NULL) { 

        p->next = n;
        n->prev = p;
        q->cursor = p;

    } else {

        n->prev = p;
        p->next = n;
        q->cursor = n;

    }

    free(c); 

    q->size--; 
    q->sorted = FALSE; 

    return(d);

} 

int que_empty(queue_t *q) {

    return(q->size == 0);

} 

int que_end(queue_t *q) {

    return(q->cursor == q->tail);

} 

int que_find(queue_t *q, void *data, comp_proc *Comp) {

    void *d = NULL; 


    d = que_first(q); 

    do { 

        if (Comp(d, data) == 0) return(OK); 
        d = que_next(q); 

    } while (!que_end(q)); 

    if (Comp(d, data) == 0) return(OK); 

    return(ERR); 

} 

void *que_first(queue_t *q) {

    if (que_empty(q)) return(NULL); 

    q->cursor = q->head; 

    return(q->cursor->data);

} 

void *que_get(queue_t *q) {

    if (q->cursor == NULL) return(NULL); 

    return(q->cursor->data);

} 

int que_init(queue_t *q) {

    node *x = NULL;


    if ((x = malloc(sizeof(datanode))) != NULL) {

        q->head = (node *)x;
        q->tail = (node *)x;

        q->head->next = q->tail;
        q->head->prev = NULL;
        q->head->data = NULL;
        q->tail->prev = q->head;
        q->tail->next = NULL;
        q->tail->data = NULL;

        q->size = 0; 
        q->cursor = q->tail; 
        q->sorted = ERR; 

        return(OK);

    }

    return(ERR);

} 

int que_insert(queue_t *q, void *d) {

    node     *c = NULL;
    node     *n = NULL; 
    datanode *x = NULL;


    if (q->cursor == NULL) return(ERR);
    if (q->cursor == q->head) return(que_push_head(q, d));
    if (q->cursor == q->tail) return(que_push_tail(q, d));

    if ((x = malloc(sizeof(datanode))) != NULL) {

        c = q->cursor;
        n = q->cursor->next;

        x->next = n;
        x->prev = c;
        c->next = x;
        n->prev = x;
        x->data = d;

        q->size++;
        q->cursor = x;
        q->sorted = ERR;

        return(OK);

    }

    return(ERR);

} 

void *que_last(queue_t *q) {

    if (que_empty(q)) return(NULL); 

    q->cursor = q->tail; 

    return(q->cursor->data);

} 

void *que_next(queue_t *q) {

    if (q->cursor->next == NULL) return(NULL); 

    q->cursor = (node *)q->cursor->next; 

    return(q->cursor->data);

} 

void *que_pop_head(queue_t *q) {

    datanode *n = NULL; 
    void     *d = NULL; 


    if (que_empty(q)) return(NULL); 

    d = q->head->data; 
    n = q->head->next; 

    free(q->head); 
    q->size--; 

    if (q->size == 0) { 

        q->head = q->tail = q->cursor = NULL; 

    } else { 

        q->head = (node *)n; 
        q->head->prev = NULL; 
        q->cursor = q->head;

    } 

    q->sorted = FALSE; 

    return(d); 

} 

void *que_pop_tail(queue_t *q) {

    datanode *p = NULL; 
    void     *d = NULL; 


    if (que_empty(q)) return(NULL); 

    d = q->tail->data; 
    p = q->tail->prev; 

    free(q->tail); 
    q->size--; 

    if (q->size == 0) { 

        q->head = q->tail = q->cursor = NULL; 

    } else { 

        q->tail = (node *)p; 
        q->tail->next = NULL; 
        q->cursor = q->tail; 

    } 

    q->sorted = FALSE; 

    return(d);

} 

void *que_prev(queue_t *q) {

    if (q->cursor->prev == NULL) return(NULL); 

    q->cursor = (node *)q->cursor->prev; 

    return(q->cursor->data);

} 

int que_push_head(queue_t *q, void *d) {

    node     *n = NULL;
    datanode *p = NULL;


    if (q->head->data != NULL) {

        if ((n = malloc(sizeof(datanode))) != NULL) {

            p = q->head;
            q->head = (node *)n; 
            q->head->prev = NULL; 
            p->prev = q->head;
            q->head->next = p;

        } else return(ERR);

    }

    q->head->data = d; 
    q->size++;
    q->cursor = q->head; 
    q->sorted = ERR;
    q->head->data = d; 

    return(OK);

} 

int que_push_tail(queue_t *q, void *d) {

    node     *p = NULL;
    datanode *n = NULL; 


    if (q->tail->data != NULL) {

        if ((n = malloc(sizeof(datanode))) != NULL) {

            p = q->tail;
            q->tail = (node *)n; 
            q->tail->next = NULL; 
            p->next = q->tail;
            q->tail->prev = p;

        } else return(ERR);

    }

    q->tail->data = d; 
    q->size++; 
    q->cursor = q->tail; 
    q->sorted = ERR;

    return(OK);

} 

int que_put(queue_t *q, void *data) { 

    if (q->cursor == NULL) return(ERR); 

    q->cursor->data = data; 

    return(OK);

} 

static int _que_bsearch(queue_t *q, void *key, comp_proc *Comp) {

    int low = 0;
    int mid = 0;
    int val = 0; 
    int hi = q->size - 1;


    while (low <= hi) { 

        mid = (low + hi) / 2; 
        val = Comp(key, q->index[mid]); 

        if (val < 0) { 

            hi = mid - 1; 

        } else if (val > 0) { 

            low = mid + 1; 

        } else return(mid);                /* Success                   */ 

    } 

    /* Not Found */ 

    return(-1); 

} 

void *que_seek(queue_t *q, void *data, comp_proc *Comp) {

    int idx = 0; 


    if (!q->sorted) { 

        if (!que_sort(q, Comp)) return NULL;

    } 

    idx = _que_bsearch(q, data, Comp); 

    if (idx < 0) return NULL; 

    q->cursor = q->posn_index[idx]; 

    return(q->index[idx]); 

} 

int que_size(queue_t *q) {

    return(q->size);

} 

static void _que_quick_sort(void *list[], int low, int high, comp_proc *Comp) {

    int i = 0;
    int j = 0; 
    int flag = 1;
    void *key = NULL;
    void *temp = NULL; 


    if (low < high) { 

        i = low; 
        j = high + 1; 
        key = list[low]; 

        while (flag) { 

            i++; 

            while (Comp(list[i], key) < 0) 
                i++; 

            j--; 

            while (Comp(list[j], key) > 0) 
                j--; 

            if (i < j) { 

                temp = list[i]; 
                list[i] = list[j]; 
                list[j] = temp; 

            } else  flag = 0; 

        } 

        temp = list[low]; 
        list[low] = list[j]; 
        list[j] = temp; 

        _que_quick_sort(list, low, j-1, Comp); 
        _que_quick_sort(list, j+1, high, Comp); 

    } 

} 

int que_sort(queue_t *q, comp_proc *Comp) {

    int      i = 0; 
    void     *d = NULL; 
    datanode *dn = NULL; 


    /* if already sorted free memory for tag array                      */ 

    if (q->sorted) { 

        free(q->index); 
        free(q->posn_index); 
        q->sorted = ERR;

    } 

    /* Now allocate memory of array, array of pointers                  */ 

    q->index = malloc(q->size * sizeof(q->cursor->data)); 
    if (q->index == NULL) return(ERR); 

    q->posn_index = malloc(q->size * sizeof(q->cursor)); 

    if (q->posn_index == NULL) { 

        free(q->index); 

        return(ERR); 

    } 

    /* Walk queue putting pointers into array                           */ 

    d = que_first(q); 

    for (i = 0; i < q->size; i++) { 

        q->index[i] = d; 
        q->posn_index[i] = q->cursor; 
        d = que_next(q);

    } 

    /* Now sort the index */ 

    _que_quick_sort(q->index, 0, q->size - 1, Comp); 

    /* Rearrange the actual queue into correct order */ 

    dn = q->head; 
    i = 0; 

    while (dn != NULL) { 

        dn->data = q->index[i++]; 
        dn = dn->next;

    } 

    /* Re-position to original element */ 

    if (d != NULL) { 

        que_find(q, d, Comp);

    } else  que_first(q); 

    q->sorted = OK; 

    return(OK); 

} 

int que_start(queue_t *q) {

    return(q->cursor == q->head);

} 

