
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

#include <ncurses.h>
#include <menu.h>
#include <errno.h>

#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/widgets/component.h"
#include "xas/widgets/windows/box_menu.h"
#include "xas/widgets/components/menus/box.h"

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

window_t *box_menu(char *title, int startx, int starty, int height, int width, int (*display)(const char *, error_trace_t *), menus_list_t *list, int list_size) {

    int tab = 1;
    int stat = OK;
    window_t *window = NULL;
    component_t *menu = NULL;

    when_error_in {

        window = window_create(title, startx, starty, height, width, TRUE);
        check_creation(window);

        startx = 0;
        starty = 0;
        height -= 1;
        width  -= 1;

        menu = box_menu_create(window, startx, starty, height, width, tab, display, list, list_size);
        check_creation(menu);

        stat = window_add(window, menu);
        check_return(stat, window);

        stat = window_set_tab(window, tab);
        check_return(stat, window);

        exit_when;

    } use {

        process_error(window);

    } end_when;

    return window;

}

