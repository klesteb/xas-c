
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
#include "xas/errors_xas.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/components/input.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private data structure                                         */
/*----------------------------------------------------------------*/

typedef struct input_data_s {
    char *tp;
    char *bp;
    char buf[256];
    int field;
    int defdisp;
    int insert;
} input_data_t;

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static char wordchar(void) {

    return 0x17;    /* ^W */

}

static input_data_t *_input_data_create(component_t *input, int width, char *value, int size) {

    input_data_t *data = NULL;

    when_error_in {

        errno = 0;
        data = calloc(1, sizeof(input_data_t));
        check_null(data);

        data->tp = NULL;
        data->bp = NULL;
        data->field = size;
        data->insert = FALSE;
        data->defdisp = TRUE;
        strncpy(data->buf, value, size);

        exit_when;

    } use {

        process_error(input);

    } end_when;

    return data;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _input_draw(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);
    input_data_t *data = self->data;

    when_error_in {

        errno = E_INVOPS;

        if (data != NULL) {

            stat = wattron(self->area, widget->theme->attribute);
            check_status(stat);

            stat = wcoloron(self->area, 
                            widget->theme->foreground, widget->theme->background);
            check_status(stat);

            stat = wprintw(self->area, "%s", data->bp);
            check_status(stat);

            stat = wstandend(self->area);
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

int _input_event(widget_t *widget, events_t *event) {

    int stat = OK;
    component_t *self = COMPONENT(widget);
    input_data_t *data = COMPONENT(widget)->data;

    when_error_in {
        
        errno = E_INVOPS;

        if (data != NULL) {

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *key = (KEVENT *)event->data;

                switch(key->keycode) {
                    case KEY_UP:
                        self->parent->tab--;
                        if (self->parent->tab < 0) {
                            self->parent->tab = 1;
                        }
                        break;
                    case KEY_ENTER:
                    case KEY_DOWN:
                        self->parent->tab++;
                        if (self->parent->tab > self->parent->tabs) {
                            self->parent->tab = 1;
                        }
                        break;
                    case KEY_LEFT:
                        if (data->bp > data->buf) {
                            data->bp--;
                        }
                        break;
                    case KEY_RIGHT:
                        data->defdisp = FALSE;
                        if (data->bp - data->buf < strlen(data->buf)) {
                            data->bp++;
                        }
                        break;

                    case '\t':            /* TAB -- because insert
                                         is broken on HPUX */
                    case KEY_IC:          /* enter insert mode */
                    case KEY_EIC:         /* exit insert mode */
                        data->defdisp = FALSE;
                        data->insert = !data->insert;
                        stat = curs_set(data->insert ? 2 : 1);
                        check_status(stat);
                        break;

                    default:
                        if (key->keycode == erasechar()) {       /* backspace, ^H */

                            if (data->bp > data->buf) {

                                memmove((void *)(data->bp - 1), 
                                        (const void *)data->bp, strlen(data->bp) + 1);
                                data->bp--;

                            }

                        } else if (key->keycode == killchar()) {   /* ^U */

                            data->bp = data->buf;
                            *data->bp = '\0';

                        } else if (key->keycode == wordchar()) {   /* ^W */

                            data->tp = data->bp;

                            while ((data->bp > data->buf) && (*(data->bp - 1) == ' ')) {
                                data->bp--;
                            }

                            while ((data->bp > data->buf) && (*(data->bp - 1) != ' ')) {
                                data->bp--;
                            }

                            memmove((void *)data->bp, 
                                    (const void *)data->tp, strlen(data->tp) + 1);
                    
                        } else if (isprint(key->keycode)) {
                        
                            if (data->defdisp) {

                                data->bp = data->buf;
                                *data->bp = '\0';
                                data->defdisp = FALSE;

                            }

                            if (data->insert) {
                        
                                if (strlen(data->buf) < data->field - 1) {
                            
                                    memmove((void *)(data->bp + 1), 
                                            (const void *)data->bp,
                                            strlen(data->bp) + 1);

                                    *data->bp++ = key->keycode;

                                }

                            } else if (data->bp - data->buf < data->field - 1) {
                        
                                /* append new string terminator */

                                if (!*data->bp)
                                    data->bp[1] = '\0';
            
                                *data->bp++ = key->keycode;

                            }

                        }
                        break;
                }
            
            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _input_dtor(object_t *object) {

    int stat = OK;
    widget_t = *widget = WIDGET(object);
    component_t *self = COMPONENT(object);

    when_error_in {

        /* free local resources here */

        stat = component_erase(self);
        check_return(stat, self);

        free(self->data);

        /* walk the chain, freeing as we go */

        object_demote(object, object_t);
        stat = widget_destroy(widget);
        check_return(stat, widget);

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

component_t *input_create(window_t *window, int startx, int starty, int width, int tab, char *value, int size) {

    int height = 1;
    int padding = FALSE;
    item_list_t items[4];
    component_t *input = NULL;
    input_data_t *data = NULL;

    when_error_in {

        input = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(input);

        data = _input_data_create(input, width, value, size);
        check_null(data);

        input->data = data;

        SET_ITEM(items[0], WIDGET_M_DRAW, _input_draw, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_EVENT, _input_event, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_DESTROY, _input_dtor, 0, NULL);
        SET_ITEM(items[3], 0, 0, 0, 0);

        stat = component_override(input, items);
        check_return(stat, input);

        exit_when;

    } use {
        
        process_error(input);
        
    } end_when;

    return input;

}

