
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

#include <string.h>
#include <errno.h>

#include "xas/misc/misc.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"

#include "xas/widgets/keys.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/window.h"
#include "xas/widgets/component.h"

require_klass(WIDGET_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _window_ctor(object_t *, item_list_t *);
int _window_dtor(object_t *);
int _window_compare(window_t *, window_t *);
int _window_override(window_t *, item_list_t *);
int _window_box(window_t *);

int _window_draw(widget_t *);
int _window_erase(widget_t *);
int _window_add(widget_t *, void *);
int _window_remove(widget_t *, void *);
int _window_event(widget_t *, events_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(WINDOW_KLASS) {
    .size = KLASS_SIZE(window_t),
    .name = KLASS_NAME(window_t),
    .ctor = _window_ctor,
    .dtor = _window_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

window_t *window_create(char *title, int startx, int starty, int height, int width, int boxed) {

    int stat = ERR;
    coordinates_t coords;
    item_list_t items[4];
    window_t *self = NULL;

    coords.startx = startx;
    coords.starty = starty;
    coords.height = height;
    coords.width  = width;

    if (title == NULL) {

        title = "";

    }

    SET_ITEM(items[0], WIDGET_K_COORDINATES, &coords, sizeof(coordinates_t), NULL);
    SET_ITEM(items[1], WINDOW_K_TITLE, title, strlen(title), NULL);
    SET_ITEM(items[2], WINDOW_K_BOXED, &boxed, sizeof(int), NULL);
    SET_ITEM(items[3], 0, 0, 0, 0);

    self = (window_t *)object_create(WINDOW_KLASS, items, &stat);

    return self;

}

int window_destroy(window_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, window_t)) {

                stat = self->dtor(OBJECT(self));
                check_return(stat, self);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int window_compare(window_t *us, window_t *them) {

    int stat = OK;

    when_error_in {

        if ((us != NULL) && (them != NULL)) {

            stat = us->_compare(us, them);
            check_return(stat, us);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(us);

    } end_when;

    return stat;

}

int window_set_tab(window_t *self, int tab) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        self->tab = tab;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int window_get_tab(window_t *self, int *tab) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        *tab = self->tab;

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

int _window_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    char title[32];
    int startx = 0;
    int starty = 0;
    int height = 0;
    int width  = 0;
    int boxed = FALSE;
    window_t *self = NULL;

    if (object != NULL) {

        when_error_in {

            /* initialize our base klass here */

            stat = WIDGET_KLASS->ctor(object, items);
            check_return(stat, object);

            WIDGET(object)->_add = _window_add;
            WIDGET(object)->_draw = _window_draw;
            WIDGET(object)->_erase = _window_erase;
            WIDGET(object)->_event = _window_event;
            WIDGET(object)->_remove = _window_remove;

            startx = WIDGET(object)->coordinates->startx;
            starty = WIDGET(object)->coordinates->starty;
            height = WIDGET(object)->coordinates->height;
            width  = WIDGET(object)->coordinates->width;

            /* capture our items */

            if (items != NULL) {

                int x;
                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case WINDOW_K_TITLE: {
                            int len = items[x].buffer_length <= 31 
                                ? items[x].buffer_length
                                : 31;
                            memset(title, '\0', 32);
                            memcpy(title, items[x].buffer_address, len);
                            break;
                        }
                        case WINDOW_K_BOXED: {
                            memcpy(&boxed, 
                                   items[x].buffer_address,
                                   items[x].buffer_length);
                            break;
                        }
                    }

                }

            }

            /* initialize our derived klass here */

            self = WINDOW(object);

            /* assign our methods here */

            self->ctor = _window_ctor;
            self->dtor = _window_dtor;
            self->_compare = _window_compare;
            self->_override = _window_override;
            self->_box = _window_box;

            self->tab = 0;
            self->tabs = 0;
            self->boxed = boxed;
            self->title = strdup(title);

            height += 2;
            width += 2;
            if ((self->outer = newwin(height, width, starty, startx)) == NULL) {

                cause_error(E_INVOPS);

            }

            height -= 2;
            width  -= 2;
            if ((self->inner = derwin(self->outer, height, width, 1, 1)) == NULL) {

                cause_error(E_INVOPS);

            }

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _window_dtor(object_t *object) {

    int stat = OK;
    component_t *component = NULL;
    window_t *self = WINDOW(object);
    widget_t *widget = WIDGET(object);

    /* free local resources here */

    while ((component = que_pop_head(&widget->things))) {

        component_destroy(component);

    }

    if (self->inner != NULL) {

        werase(self->inner);
        delwin(self->inner);

    }

    if (self->outer != NULL) {

        werase(self->outer);
        delwin(self->outer);

    }

    if (self->title != NULL) free(self->title);

    /* walk the chain, freeing as we go */

    object_demote(object, widget_t);
    widget_destroy(widget);

    return stat;

}

int _window_compare(window_t *self, window_t *other) {

    int stat = ERR;

    if (((widget_compare(WIDGET(self), WIDGET(other)) == OK)) &&
        (self->tab == other->tab) &&
        (self->tabs == other->tabs) &&
        (self->boxed == other->boxed) &&
        (self->title == other->title) &&
        (self->inner == other->inner) &&
        (self->outer == other->outer)) {

        stat = OK;

    }

    return stat;

}

int _window_override(window_t *self, item_list_t *items) {

    int stat = ERR;

    when_error_in {

        stat = widget_override(WIDGET(self), items);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

char *window_version(window_t *self) {
    
    char *version = PACKAGE_VERSION;

    return version;
    
}

int _window_add(widget_t *widget, void *data) {

    int stat = OK;
    window_t *self = WINDOW(widget);
    component_t *component = (component_t *)data;

    when_error_in {

        errno = 0;
        stat = que_push_head(&widget->things, component);
        check_status(stat);

        self->tab = component->tab;

        if (component->tab > self->tabs) {

            self->tabs = component->tab;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _window_draw(widget_t *widget) {

    int stat = OK;
    component_t *component = NULL;
    window_t *self = WINDOW(widget);

    when_error_in {

        errno = E_INVOPS;

        stat = wattrset(self->outer, widget->theme->attribute);
        check_status(stat);

        stat = wcolorset(self->outer, widget->theme->foreground, widget->theme->background);
        check_status(stat);

        stat = wbkgd(self->inner, COLOR_PAIR(colornum(widget->theme->foreground, widget->theme->background)));
        check_status(stat);

        if (self->boxed) {

            stat = self->_box(self);
            check_return(stat, self);

        }

        for (component = que_first(&widget->things);
             component != NULL;
             component = que_next(&widget->things)) {

            stat = component_draw(component);
            check_return(stat, component);

        }

        stat = wstandend(self->inner);
        check_status(stat);

        stat = wnoutrefresh(self->inner);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _window_erase(widget_t *widget) {

    int stat = OK;
    component_t *component = NULL;
    window_t *self = WINDOW(widget);

    when_error_in {

        errno = E_INVOPS;

        for (component = que_first(&widget->things);
             component != NULL;
             component = que_next(&widget->things)) {

            stat = component_erase(component);
            check_return(stat, component);

        }

        if (self->inner != NULL) {

            stat = wclear(self->inner);
            check_status(stat);

        }

        stat = wnoutrefresh(self->inner);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _window_remove(widget_t *widget, void *thing) {

    int stat = ERR;
    component_t *temp = NULL;
    component_t *component = (component_t *)thing;

    when_error_in {

        for (temp = que_first(&widget->things);
             temp != NULL;
             temp = que_next(&widget->things)) {

            if ((component_compare(temp, component)) == OK) {

                component_t *junk = que_delete(&widget->things);
                stat = component_destroy(junk);
                check_return(stat, junk);
                break;

            }

        }

        if (que_empty(&widget->things)) {

            errno = 0;
            stat = que_init(&widget->things);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _window_event(widget_t *widget, events_t *event) {

    int stat = OK;
    component_t *component = NULL;
    window_t *self = WINDOW(widget);

    when_error_in {

        if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *key = (KEVENT *)event->data;

            switch(key->keycode) {
                case KEY_TAB:
                    self->tab++;
                    if (self->tab > self->tabs) self->tab = 1;
                    break;
                case KEY_BTAB:
                    self->tab--;
                    if (self->tab < 1) self->tab = self->tabs;
                    break;
            }

        }

        for (component = que_first(&widget->things);
             component != NULL;
             component = que_next(&widget->things)) {

            if (self->tab == component->tab) {

                stat = component_event(component, event);
                check_return(stat, component);
                break;

            }

        }

        errno = E_INVOPS;
        stat = wnoutrefresh(self->inner);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _window_box(window_t *self) {

    int len = 0;
    int stat = OK;

    when_error_in {

        errno = E_INVOPS;

        if (self->outer != NULL) {

            stat = box(self->outer, ACS_VLINE, ACS_HLINE);
            check_status(stat);

            len = strlen(self->title);

            if (len > 0) {

                stat = wmove(self->outer, 0, 2);
                check_status(stat);

                stat = waddch(self->outer, ACS_RTEE);
                check_status(stat);

                stat = wmove(self->outer, 0, 3);
                check_status(stat);

                stat = waddstr(self->outer, self->title);
                check_status(stat);

                stat = wmove(self->outer, 0, 3 + len);
                check_status(stat);

                stat = waddch(self->outer, ACS_LTEE);
                check_status(stat);

                stat = wstandend(self->outer);
                check_status(stat);

            }

            stat = wnoutrefresh(self->outer);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

