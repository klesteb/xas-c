
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
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "xas/types.h"
#include "xas/queue.h"
#include "xas/misc/misc.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/components/textarea.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _wrapped_s {
    int size;
    int cur_pos;
    char **buffer;
} wrapped_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _data_display(widget_t *);
static int _data_destroy(wrapped_t *);
static int _data_create(component_t *, int, char *, int, wrapped_t *);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _textarea_dtor(object_t *object) {

    int stat = OK;
    widget_t *widget = WIDGET(object);
    component_t *textarea = COMPONENT(object);

    when_error_in {

        errno = E_INVOPS;

        /* free local resources here */

        if (textarea->data != NULL) {

            stat = _data_destroy(textarea->data);
            check_status(stat);

        }

        if (textarea->area) {

            stat = delwin(textarea->area);
            check_status(stat);

        }

        stat = widget_erase(widget);
        check_return(stat, widget);

        /* walk the chain, freeing as we go */

        object_demote(object, widget_t);

        stat = widget_destroy(widget);
        check_return(stat, widget);

        exit_when;

    } use {

        stat = ERR;
        process_error(object);

    } end_when;

    return stat;

}

int _textarea_draw(widget_t *widget) {

    int stat = OK;
    wrapped_t *data = COMPONENT(widget)->data;

    when_error_in {

        errno = E_INVOPS;

        if (data != NULL) {

            data->cur_pos = 0;
            COMPONENT(widget)->data = data;

            stat = _data_display(widget);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _textarea_event(widget_t *widget, events_t *event) {

    int cols = 0;
    int rows = 0;
    int stat = OK;
    WINDOW *area = COMPONENT(widget)->area;
    wrapped_t *data = COMPONENT(widget)->data;

    when_error_in {

        errno = E_INVOPS;

        if (data != NULL) {

            getmaxyx(area, rows, cols);

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *key = (KEVENT *)event->data;

                switch(key->keycode) {
                    case KEY_UP: {
                        if (data->cur_pos < (data->size - 1)) { 
                            data->cur_pos++;
                        }
                        COMPONENT(widget)->data = data;
                        stat = werase(area);
                        check_status(stat);
                        stat = _data_display(widget);
                        check_status(stat);
                        stat = wnoutrefresh(area);
                        check_status(stat);
                        break;
                    }
                    case KEY_DOWN: {
                        if (data->cur_pos > 0) { 
                            data->cur_pos--;
                        }
                        COMPONENT(widget)->data = data;
                        stat = werase(area);
                        check_status(stat);
                        stat = _data_display(widget);
                        check_status(stat);
                        stat = wnoutrefresh(area);
                        check_status(stat);
                        break;
                    }
                    case KEY_HOME: {
                        data->cur_pos = 0;
                        COMPONENT(widget)->data = data;
                        stat = werase(area);
                        check_status(stat);
                        stat = _data_display(widget);
                        check_status(stat);
                        stat = wnoutrefresh(area);
                        check_status(stat);
                        break;
                    }
                    case KEY_END: {
                        data->cur_pos = (data->size - rows) + 1;
                        COMPONENT(widget)->data = data;
                        stat = werase(area);
                        check_status(stat);
                        stat = _data_display(widget);
                        check_status(stat);
                        stat = wnoutrefresh(area);
                        check_status(stat);
                        break;
                    }
                    case KEY_NPAGE: {
                        data->cur_pos += rows;
                        if (data->cur_pos > rows) {
                            data->cur_pos = (data->size - rows) + 1;
                        }
                        COMPONENT(widget)->data = data;
                        stat = werase(area);
                        check_status(stat);
                        stat = _data_display(widget);
                        check_status(stat);
                        stat = wnoutrefresh(area);
                        check_status(stat);
                        break;
                    }
                    case KEY_PPAGE: {
                        data->cur_pos -= rows;
                        if (data->cur_pos < 0) {
                            data->cur_pos = 0;
                        }
                        COMPONENT(widget)->data = data;
                        stat = werase(area);
                        check_status(stat);
                        stat = _data_display(widget);
                        check_status(stat);
                        stat = wnoutrefresh(area);
                        check_status(stat);
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

int _textarea_erase(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);
    wrapped_t *data = COMPONENT(widget)->data;

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *textarea_create(window_t *window, int startx, int starty, int height, int width, int tab, char *value, int size) {

    int stat = OK;
    int padding = FALSE;
    item_list_t items[5];
    wrapped_t *data = NULL;
    component_t *textarea = NULL;

    when_error_in {

        textarea = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(textarea);

        errno = 0;
        data = (wrapped_t *)calloc(1, sizeof(wrapped_t));
        check_null(data);

        stat = _data_create(textarea, width, value, size, data);
        check_return(stat, textarea);

        textarea->data = data;

        SET_ITEM(items[0], WIDGET_M_DRAW, _textarea_draw, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_EVENT, _textarea_event, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_ERASE, _textarea_erase, 0, NULL);
        SET_ITEM(items[3], WIDGET_M_DESTROY, _textarea_dtor, 0, NULL);
        SET_ITEM(items[3], 0, 0, 0, 0);

        stat = component_override(textarea, items);
        check_return(stat, textarea);

        exit_when;
        
    } use {
        
        process_error(textarea);
        
    } end_when;
    
    return textarea;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _data_display(widget_t *widget) {

    int x = 0;
    int stat = OK;
    WINDOW *area = COMPONENT(widget)->area;
    wrapped_t *data = (wrapped_t *)COMPONENT(widget)->data;

    when_error_in {

        errno = E_INVOPS;

        if (data != NULL) {

            stat = wattron(area, widget->theme->attribute);
            check_status(stat);
            
            stat = wbkgd(area, COLOR_PAIR(colornum(widget->theme->foreground, widget->theme->background)));
            check_status(stat);

            for (x = data->cur_pos; x < data->size; x++) {

                if ((stat = wprintw(area, "%s\n", data->buffer[x])) != OK) {

                    /* reached the end of the window */

                    stat = OK;
                    break;

                }

            }

            stat = wstandend(area);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

static int _data_create(component_t *self, int width, char *value, int size, wrapped_t *data) {

    queue_t lines;
    int stat = OK;
    char *line = NULL;
    char **buffer = NULL;

    when_error_in {

        errno = 0;
        stat = que_init(&lines);
        check_status(stat);

        if ((wordwrap(value, width, &lines)) == 0) {

            data->cur_pos = 0;
            data->size = que_size(&lines);

            errno = 0;
            buffer = (char **)calloc(data->size, width);
            check_null(buffer);

            int x = 0;

            while ((line = que_pop_head(&lines))) {

                buffer[x] = strndup(line, strlen(line));
                free(line);
                x++;

            }

            data->buffer = buffer;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _data_destroy(wrapped_t *data) {

    int x = 0;
    int stat = ERR;

    if (data != NULL) {

        for (x = 0; x < data->size; x++) {

            free(data->buffer[x]);

        }

        free(data->buffer);
        free(data);

        stat = OK;

    }

    return stat;

}

