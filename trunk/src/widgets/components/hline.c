
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
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/widgets/components/hline.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _hline_draw(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        stat = wattron(self->area, widget->theme->attribute);
        check_status2(stat, OK, E_INVOPS);
    
        stat = wcoloron(self->area, 
                        widget->theme->foreground, widget->theme->background);
        check_status2(stat, OK, E_INVOPS);
    
        stat = mvwhline(self->area, 0, 0, ACS_HLINE, widget->coordinates->width);
        check_status2(stat, OK, E_INVOPS);

        stat = wstandend(self->area);
        check_status2(stat, OK, E_INVOPS);

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

component_t *hline_create(window_t *window, int startx, int starty, int width) {

    int tab = 0;
    int height = 1;
    int padding = FALSE;
    item_list_t items[2];
    component_t *hline = NULL;

    if ((hline = component_create(window, startx, starty, height, width, tab, padding, NULL, 0))) {

        SET_ITEM(items[0], WIDGET_M_DRAW, _hline_draw, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        component_override(hline, items);

    }

    return hline;

}

