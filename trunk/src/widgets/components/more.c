
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

#include "xas/types.h"
#include "xas/misc/misc.h"
#include "xas/errors_xas.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/components/more.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

typedef struct _more_data_s {
    int vrow;
    int vcol;
    int vrows;
    int vcols;
    WINDOW *pad;
} more_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _more_data_display(component_t *self) {

    int stat = OK;
    int height = 0;
    int width  = 0;
    more_data_t *data = NULL;

    when_error_in {

        errno = E_INVOPS;

        height = getmaxy(self->area) - 1;
        width  = getmaxx(self->area) - 1;

        if (self->data != NULL) {

            data = (more_data_t *)self->data;

            stat = copywin(data->pad, self->area, data->vrow, data->vcol, 
                           0, 0, height, width, 0);
            check_status(stat);

            stat = wnoutrefresh(self->area);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _more_data_create(component_t *more, queue *text) {

    int len = 0;
    int stat = OK;
    int vrows = 0;
    int vcols = 0;
    char *line = NULL;
    more_data_t *data = NULL;

    when_error_in {

        errno = 0;
        data = calloc(1, sizeof(more_data_t));
        check_null(data);

        /* a pad needs to be at least the size of the window that    */
        /* will diplay the contents, otherwise you get an ERR return */
        /* code from copywin(). this is not documented.              */

        vrows = getmaxy(more->area);
        vcols = getmaxx(more->area);

        if ((que_size(text) > vrows)) {

            vrows = que_size(text);

        }

        line = que_first(text);

        do {

            len = strlen(line);
            if (len > vcols) vcols = len;

        } while ((line = que_next(text)));

        data->vrow = 0;
        data->vcol = 0;
        data->vcols = vcols;
        data->vrows = vrows;

        /* done adjusting the pad size */

        data->pad = newpad(data->vrows + 1, data->vcols + 1);
        check_null(data->pad);

        int x = 0;
        while ((line = que_pop_head(text))) {

            stat = mvwaddstr(data->pad, x, 0, line);
            check_status(stat);

            x++;
            free(line);

        }

        more->data = (void *)data;

        exit_when;

    } use {

        stat = ERR;
        process_error(more);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _more_dtor(object_t *object) {

    int stat = OK;
    more_data_t *data = NULL;
    component_t *self = COMPONENT(object);

    when_error_in {
        
        errno = E_INVOPS;

        /* free local resources here */

        if (self->data != NULL) {

            data = (more_data_t *)self->data;

            stat = werase(data->pad);
            check_status(stat);

            stat = delwin(data->pad);
            check_status(stat);

            free(data);
            self->data = NULL;

        }

        if (self->area) {

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

int _more_draw(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        stat = _more_data_display(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _more_erase(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        errno = E_INVOPS;

        if (self->area) {

            stat = werase(self->area);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _more_event(widget_t *widget, events_t *event) {

    int stat = OK;
    more_data_t *data = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            data = (more_data_t *)self->data;

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *key = (KEVENT *)event->data;

                switch(key->keycode) {
                    case KEY_NPAGE: { 
                        int rows = getmaxy(self->area);
                        data->vrow = ((data->vrow + rows) < (data->vrows - 1))
                                   ? data->vrow + rows
                                   : data->vrows - rows;
                        break;
                    }
                    case KEY_PPAGE: {
                        int rows = getmaxy(self->area);
                        data->vrow = ((data->vrow - rows) > 0)
                                   ? data->vrow - rows
                                   : 0;
                        break;
                    }
                    case KEY_LEFT: {
                        data->vcol = (data->vcol > 0) 
                                   ? data->vcol - 1 
                                   : 0;
                        break;
                    }
                    case KEY_RIGHT: {
                        int cols = getmaxx(self->area);
                        data->vcol = (data->vcol < (data->vcols - cols))
                                    ? data->vcol + 1
                                    : (data->vcols - cols) + 1;
                        break;
                    }
                    case KEY_UP: {
                        data->vrow = (data->vrow > 0) 
                                   ? data->vrow - 1 
                                   : 0;
                        break;
                    }
                    case KEY_DOWN: {
                        int rows = getmaxy(self->area);
                        data->vrow = (data->vrow < (data->vrows - rows))
                                   ? data->vrow + 1
                                   : data->vrows - rows;
                        break;
                    }
                    case KEY_HOME: {
                        data->vrow = 0;
                        data->vcol = 0;
                        break;
                    }
                    case KEY_END: {
                        int rows = getmaxy(self->area);
                        data->vcol = 0;
                        data->vrow = data->vrows - rows;
                        break;
                    }
                }

            }

            stat = _more_data_display(self);
            check_return(stat, self);

            COMPONENT(widget)->data = data;

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

component_t *more_create(window_t *window, int startx, int starty, int height, int width, int tab, queue *text) {

    int stat = OK;
    int padding = FALSE;
    item_list_t items[5];
    component_t *more = NULL;

    when_error_in {

        more = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(more);

        stat = _more_data_create(more, text);
        check_return(stat, more);

        SET_ITEM(items[0], WIDGET_M_DRAW, _more_draw, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_EVENT, _more_event, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_ERASE, _more_erase, 0, NULL);
        SET_ITEM(items[3], WIDGET_M_DESTROY, _more_dtor, 0, NULL);
        SET_ITEM(items[4], 0, 0, 0, 0);

        stat = component_override(more, items);
        check_return(stat, more);

        exit_when;

    } use {

        process_error(more);

    } end_when;

    return more;

}

