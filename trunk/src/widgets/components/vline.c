
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

#include "xas/types.h"
#include "xas/errors_xas.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/components/vline.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _vline_draw(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = E_INVOPS;

        stat = wattron(self->area, widget->theme->attribute);
        check_status(stat);

        stat = wcoloron(self->area, 
                        widget->theme->foreground, widget->theme->background);
        check_status(stat);

        stat = mvwvline(self->area, 0, 0, ACS_VLINE, widget->coordinates->height);
        check_status(stat);

        stat = wstandend(self->area);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *vline_create(window_t *window, int startx, int starty, int height) {

    int tab = 0;
    int stat = OK;
    int width = 1;
    int padding = FALSE;
    item_list_t items[2];
    component_t *vline = NULL;

    when_error_in {
        
        vline = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(vline);

        SET_ITEM(items[0], WIDGET_M_DRAW, _vline_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        stat = component_override(vline, items);
        check_return(stat, vline);

        exit_when;

    } use {

        process_error(vline);

    } end_when;

    return vline;

}

