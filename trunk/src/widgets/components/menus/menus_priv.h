
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

#ifndef _COMPONENTS_MENUS_PRIV_H
#define _COMPONENTS_MENUS_PRIV_H

#include <menu.h>

#include "xas/types.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/window.h"
#include "xas/widgets/widget.h"
#include "xas/widgets/component.h"

/*----------------------------------------------------------------*/
/* private data structure                                         */
/*----------------------------------------------------------------*/

typedef struct _menus_data_s {
    WINDOW *subwin;
    MENU *menu;
    ITEM **items;
    ITEM *focus;
    Menu_Options options;
    char *mark;
    int row;
    int col;
    int items_count;
    int (*show_description)(widget_t *);
    int (*callback)(const char *);
} menus_data_t;

typedef struct _userptr_data_s {
        int (*callback)(void *, int);
        int data_size;
        void *data;
} userptr_data_t;

#endif

