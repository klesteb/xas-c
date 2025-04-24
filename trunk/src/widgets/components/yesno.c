
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
#include "xas/errors_xas.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/components/vline.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _yesno_data_s {
    int (*callback)(int);
    char *label;
} yesno_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _yesno_data_create(component_t *self, char *label, int (*callback)(int)) {

    int stat = OK;
    yesno_data_t *data = NULL;

    when_error_in {

        errno = 0;
        data = calloc(1, sizeof(yesno_data_t));
        check_null(data);

        data->callback = callback;
        data->label = strdup(label);

        self->data = (void *)data;

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

int _yesno_dtor(object_t *object) {

    int stat = OK;
    yesno_data_t *data = NULL;
    component_t *self = COMPONENT(object);

    when_error_in {

        /* free local resources here */

        if (self->data != NULL) {

            data = (yesno_data_t *)self->data;

            free(data->label);
            free(data);

            self->data = NULL;

        }

        if (self->area != NULL) {

            errno = E_INVOPS;

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

int _yesno_event(widget_t *widget, events_t *event) {

    int stat = OK;
    error_trace_t error;
    yesno_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = E_INVOPS;

        if (self->data != NULL) {

            data = (yesno_data_t *)self->data;
            
            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *key = (KEVENT *)event->data;

                switch(key->keycode) {
                    case KEY_F(7): { 
                        if (data->callback != NULL) {
                            stat = (*data->callback)(TRUE);
                            check_status(stat);
                        }
                        break;
                    }
                    case KEY_F(8): {
                        if (data->callback != NULL) {
                            stat = (*data->callback)(FALSE);
                            check_status(stat);
                        }
                        break;
                    }
                }

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _yesno_draw(widget_t *widget) {

    int len = 0;
    int stat = OK;
    yesno_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = E_INVOPS;

        if (self->data != NULL) {

            data = (yesno_data_t *)self->data;
            len = strlen(data->label);

            stat = wattron(self->area, widget->theme->attribute);
            check_status(stat);

            stat = wcoloron(self->area, 
                            widget->theme->foreground, widget->theme->background);
            check_status(stat);

            stat = mvwaddnstr(self->area, 0, 0, data->label, len);
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

component_t *yesno_create(window_t *window, int startx, int starty, int height, int width, int tab, char *label, int (*callback)(int)) {

    int stat = OK;
    int padding = FALSE;
    item_list_t items[4];
    component_t *yesno = NULL;

    when_error_in {

        yesno = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(yesno);

        stat = _yesno_data_create(yesno, label, callback);
        check_return(stat, yesno);

        SET_ITEM(items[0], WIDGET_M_DRAW, _yesno_draw, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_EVENT, _yesno_event, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_DESTROY, _yesno_dtor, 0, NULL);
        SET_ITEM(items[3], 0, 0, 0, 0);

        stat = component_override(yesno, items);
        check_return(stat, yesno);

        exit_when;

    } use {

        process_error(yesno);

    } end_when;

    return yesno;

}

