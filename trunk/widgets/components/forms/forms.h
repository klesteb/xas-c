
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _FORMS_H
#define _FORMS_H

#include <form.h>

#include "xas/widgets/component.h"

/*----------------------------------------------------------------*/
/* data strutures                                                 */
/*----------------------------------------------------------------*/

typedef struct _field_data_s {
    int col;
    int row;
    int width;
    int height;
    int required;
    int size;
    char *result;
    char *value;
    FIELD *field;
} field_data_t;

struct _form_list_s {
    int row;                    /* row for data input field                 */
    int col;                    /* col for data input field                 */
    int width;                  /* the width of the field                   */
    int height;                 /* the height of the field                  */
    int size;                   /* the size of value/result                 */
    int type;                   /* type of data to be inputed               */
    int required;               /* wither the field is required             */
    char *value;                /* pointer to the string to edit            */
    char *result;               /* pointer to the edited string             */
} form_list_t;

/*----------------------------------------------------------------*/
/* constants                                                      */
/*----------------------------------------------------------------*/

#define FORM_T_INPUT     1
#define FORM_T_LABEL     2
#define FORM_T_TEXTAREA  3
#define FORM_T_SEPERATOR 4

#define SET_FORM_FIELD(x,a,b,c,d,e,f,g,h,i) {\
    (x).row=(a);      \
    (x).col=(b);      \
    (x).length=(c);   \
    (x).width=(d);    \
    (x).type=(e);     \
    (x).length=(f);   \
    (x).required=(g); \
    (x).value=(h);    \
    (x).result=(i);   \
}

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *form_field_create(field_data_t *);
extern component_t *form_label_create(field_data_t *);
extern component_t *form_header_create(field_data_t *);
extern component_t *form_textarea_create(field_data_t *);
extern component_t *form_seperator_create(field_data_t *);

extern int form_field_results(widget_t *, void *, int);

#endif

