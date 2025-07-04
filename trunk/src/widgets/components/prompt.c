
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
#include <errno.h>

#include "xas/types.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/components/prompt.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _prompt_data_create(component_t *self, char *text, int size) {

    int len = 0;
    int stat = OK;
    char *value = NULL;
    widget_t *widget = WIDGET(self);

    when_error_in {

        errno = 0;
        value = calloc(1, widget->coordinates->width + 1);
        check_null(value);

        len = (size < widget->coordinates->width - 1)
            ? size
            : widget->coordinates->width - 1;

        memset(value, ' ', widget->coordinates->width);
        memcpy(value, (char *)text, len);
        strcat(value, ">");

        self->data = (void *)value;

        exit_when;
        
    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _prompt_draw(widget_t *widget) {

    int stat = ERR;
    char *value = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = E_INVOPS;

        if (self->data != NULL) {

            value = (char *)self->data;

            stat = wattron(self->area, widget->theme->attribute);
            check_status(stat);

            stat = wcoloron(self->area, 
                            widget->theme->foreground, widget->theme->background);
            check_status(stat);

            stat = mvwaddnstr(self->area, 0, 0, value, widget->coordinates->width - 1);
            check_status(stat);

            stat = wstandend(self->area);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _prompt_dtor(object_t *object) {

    int stat = OK;
    component_t *self = COMPONENT(object);

    when_error_in {

        errno = E_INVOPS;

        /* free local resources here */

        if (self->data != NULL) {

            free(self->data);
            self->data = NULL;

        }

        if (self->area != NULL) {

            stat = werase(self->area);
            check_status(stat);

            stat = delwin(self->area);
            check_status(stat);

            self->area = NULL;

        }

        /* walk the chain, freeing as we go */

        object_demote(object, object_t);

        stat = object_destroy(object);
        check_return(stat, object);

        exit_when;

    } use {

        stat = ERR;
        process_error(object);

    } end_when;

    return stat;

}

component_t *prompt_create(window_t *window, int startx, int starty, int width, char *value, int size) {

    int tab = 0;
    int stat = OK;
    int height = 1;
    int padding = FALSE;
    item_list_t items[3];
    component_t *component = NULL;

    when_error_in {

        component = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(component);

        stat = _prompt_data_create(component, value, size);
        check_return(stat, component);

        SET_ITEM(items[0], WIDGET_M_DRAW, _prompt_draw, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_DESTROY, _prompt_dtor, 0, NULL);
        SET_ITEM(items[2], 0, 0, 0, 0);

        stat = component_override(component, items);
        check_return(stat, component);

        exit_when;

    } use {

        process_error(component);

    } end_when;

    return component;

}

