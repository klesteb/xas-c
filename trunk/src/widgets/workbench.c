
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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ncurses.h>
#include <panel.h>
#include <fcntl.h>
#include <signal.h>
#include <locale.h>

#include "xas/queue.h"
#include "xas/misc/misc.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/gpl/nix_util.h"

#include "xas/widgets/keys.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/events.h"
#include "xas/widgets/widget.h"
#include "xas/widgets/window.h"
#include "xas/widgets/workbench.h"

#define TIMEOUT 1.0

require_klass(WIDGET_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *, item_list_t *);
int _workbench_dtor(object_t *);
int _workbench_compare(workbench_t *, workbench_t *);
int _workbench_override(workbench_t *, item_list_t *);

int _workbench_draw(workbench_t *);
int _workbench_erase(workbench_t *);
int _workbench_add(workbench_t *, window_t *);
int _workbench_event(workbench_t *, events_t *);
int _workbench_remove(workbench_t *, window_t *);

int _workbench_read_stdin(workbench_t *);
int _workbench_init_terminal(workbench_t *);
int _workbench_dispatch(workbench_t *, int *);
int _workbench_set_menu(workbench_t *, window_t *);
int _workbench_get_focus(workbench_t *, window_t *);
int _workbench_set_focus(workbench_t *, window_t *);
int _workbench_queue_event(workbench_t *, events_t *);

/*----------------------------------------------------------------*/
/* private klass methods                                          */
/*----------------------------------------------------------------*/

static int _workbench_queue_exit(workbench_t *);
static int _workbench_remove_all(workbench_t *);
static int _workbench_handle_f10(workbench_t *);
static int _workbench_handle_f11(workbench_t *);
static int _workbench_handle_f12(workbench_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(WORKBENCH_KLASS) {
    .size = KLASS_SIZE(workbench_t),
    .name = KLASS_NAME(workbench_t),
    .ctor = _workbench_ctor,
    .dtor = _workbench_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

workbench_t *workbench_create(item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;

    self = (workbench_t *)object_create(WORKBENCH_KLASS, items, &stat);

    return self;

}

int workbench_destroy(workbench_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, workbench_t)) {

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_override(workbench_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_compare(workbench_t *us, workbench_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, workbench_t)) {

                stat = us->_compare(us, them);
                check_return(stat, us);

            } else {

                cause_error(E_INVOBJ);

            }

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

int workbench_inject_event(workbench_t *self, events_t *event) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (event == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_queue_event(self, event);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_add(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (window == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add(self, window);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_remove(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (window == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_remove(self, window);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_remove_async(workbench_t *self, window_t *window) {

    int stat = OK;
    events_t *event = NULL;

    when_error_in {

        if ((self == NULL) || (window == NULL)) {

            cause_error(E_INVPARM);

        }

        errno = 0;
        event = calloc(1, sizeof(events_t));
        check_null(event);

        event->type = EVENT_K_REMOVE;
        event->data = (void *)window;

        stat = self->_queue_event(self, event);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_set_menu(workbench_t *self, window_t *main) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (main == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_set_menu(self, main);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_get_focus(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (window == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get_focus(self, window);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_set_focus(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (window == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_set_focus(self, window);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_refresh(workbench_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_erase(self);
        check_return(stat, self);

        stat = self->_draw(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_capture(workbench_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_read_stdin(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int workbench_dispatch(workbench_t *self, int *again) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_dispatch(self, again);
        check_return(stat, self);

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

int _workbench_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;
    int (*init_terminal)(workbench_t *) = _workbench_init_terminal;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case WORKBENCH_M_INIT_TERMINAL: {
                        init_terminal = items[x].buffer_address;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = WORKBENCH(object);

        /* assign our methods here */

        self->ctor = _workbench_ctor;
        self->dtor = _workbench_dtor;
        self->_compare = _workbench_compare;
        self->_override = _workbench_override;

        self->_add = _workbench_add;
        self->_draw = _workbench_draw;
        self->_event = _workbench_event;
        self->_erase = _workbench_erase;
        self->_remove = _workbench_remove;

        self->_init_terminal = init_terminal;
        self->_dispatch = _workbench_dispatch;
        self->_set_menu = _workbench_set_menu;
        self->_get_focus = _workbench_get_focus;
        self->_set_focus = _workbench_set_focus;
        self->_read_stdin = _workbench_read_stdin;
        self->_queue_event = _workbench_queue_event;

        /* initialize internal variables here */

        when_error_in {

            errno = 0;
            stat = que_init(&self->events);
            check_status(stat);

            self->panels = 0;
            self->main = NULL;
            self->panel = NULL;

            stat = self->_init_terminal(self);
            check_return(stat, self);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _workbench_dtor(object_t *object) {

    int stat = OK;
    workbench_t *self = WORKBENCH(object);

    /* free local resources here */

    _workbench_remove_all(self);

    /* endwin(); */

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _workbench_override(workbench_t *self, item_list_t *items) {

    int stat = ERR;

    when_error_in {
        
        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case WORKBENCH_M_DESTROY: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case WORKBENCH_M_ADD: {
                        self->_add = NULL;
                        self->_add = items[x].buffer_address;
                        check_null(self->_add);
                        break;
                    }
                    case WORKBENCH_M_DRAW: {
                        self->_draw = NULL;
                        self->_draw = items[x].buffer_address;
                        check_null(self->_draw);
                        break;
                    }
                    case WORKBENCH_M_EVENT: {
                        self->_event = NULL;
                        self->_event = items[x].buffer_address;
                        check_null(self->_event);
                        break;
                    }
                    case WORKBENCH_M_ERASE: {
                        self->_erase = NULL;
                        self->_erase = items[x].buffer_address;
                        check_null(self->_erase);
                        break;
                    }
                    case WORKBENCH_M_REMOVE: {
                        self->_remove = NULL;
                        self->_remove = items[x].buffer_address;
                        check_null(self->_remove);
                        break;
                    }
                    case WORKBENCH_M_SET_FOCUS: {
                        self->_set_focus = NULL;
                        self->_set_focus = items[x].buffer_address;
                        check_null(self->_set_focus);
                        break;
                    }
                    case WORKBENCH_M_GET_FOCUS: {
                        self->_get_focus = NULL;
                        self->_get_focus = items[x].buffer_address;
                        check_null(self->_get_focus);
                        break;
                    }
                    case WORKBENCH_M_READ_STDIN: {
                        self->_read_stdin = NULL;
                        self->_read_stdin = items[x].buffer_address;
                        check_null(self->_read_stdin);
                        break;
                    }
                    case WORKBENCH_M_INIT_TERMINAL: {
                        self->_init_terminal = NULL;
                        self->_init_terminal = items[x].buffer_address;
                        check_null(self->_init_terminal);
                        break;
                    }
                    case WORKBENCH_M_DISPATCH: {
                        self->_dispatch = NULL;
                        self->_dispatch = items[x].buffer_address;
                        check_null(self->_dispatch);
                        break;
                    }
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

int _workbench_compare(workbench_t *self, workbench_t *other) {

    int stat = OK;

    when_error_in {

        if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override) &&
            (self->_add == other->_add) &&
            (self->_draw == other->_draw) &&
            (self->_event == other->_event) &&
            (self->_erase == other->_erase) &&
            (self->_remove == other->_remove) &&
            (self->_dispatch == other->_dispatch) &&
            (self->_set_menu == other->_set_menu) &&
            (self->_get_focus == other->_get_focus) &&
            (self->_set_focus == other->_set_focus) &&
            (self->_read_stdin == other->_read_stdin) &&
            (self->_queue_event == other->_queue_event) &&
            (self->_init_terminal == other->_init_terminal) &&
            (self->main == other->main) &&
            (self->panel == other->panel) &&
            (self->panels == other->panels) &&
            (self->messages == other->messages)) {

            stat = OK;

        } else {

            cause_error(E_NOTSAME);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_add(workbench_t *self, window_t *window) {

    int stat = OK;
    PANEL *panel = NULL;

    when_error_in {

        errno = E_INVOPS;
        panel = new_panel(window->outer);
        check_null(panel);

        self->panels++;
        self->panel = panel;

        stat = set_panel_userptr(panel, (void *)window);
        check_status(stat);

        stat = top_panel(panel);
        check_status(stat);

        stat = window_draw(window);
        check_return(stat, window);

        update_panels();

        stat = doupdate();
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_draw(workbench_t *self) {

    int stat = OK;
    PANEL *panel = NULL;
    window_t *window = NULL;

    when_error_in {

        errno = E_INVOPS;

        for (panel = panel_above(NULL);
             panel != NULL;
             panel = panel_above(panel)) {

            window = (window_t *)panel_userptr(panel);
            check_null(window);

            stat = window_draw(window);
            check_return(stat, window);

        }

        update_panels();

        stat = doupdate();
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_erase(workbench_t *self) {

    int stat = OK;
    PANEL *panel = NULL;
    window_t *window = NULL;

    when_error_in {

        errno = E_INVOPS;

        for (panel = panel_above(NULL);
             panel != NULL;
             panel = panel_above(panel)) {

            window = (window_t *)panel_userptr(panel);
            check_null(window);

            stat = window_erase(window);
            check_return(stat, window);

        }

        update_panels();

        stat = doupdate();
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_event(workbench_t *self, events_t *event) {

    int stat = OK;
    int length = 0;
    char message[256];

    when_error_in {

        errno = E_INVOPS;

        if (event->type == EVENT_K_EXIT) {

            raise(SIGTERM);

        } else if (event->type == EVENT_K_MESSAGE) {

            length = strlen((char *)event->data) + 3;
            length = (length > 255) ? 255 : length; 
            memset(message, '\0', 256);
            snprintf(message, length, "* %s", (char *)event->data);

            stat = werase(self->messages);
            check_status(stat);

            stat = curs_set(0);
            check_status(stat);

            stat = mvwaddstr(self->messages, 0, 0, message);
            check_status(stat);

            stat = wnoutrefresh(self->messages);
            check_status(stat);

        } else if (event->type == EVENT_K_REMOVE) {

            window_t *window = (window_t *)event->data;
            check_null(window);

            stat = self->_remove(self, window);
            check_return(stat, self);
            event->data = NULL;

        } else if (event->type == EVENT_K_KEYBOARD) {

            KEVENT *kevent = event->data;

            if ((kevent->keycode == KEY_RESIZE) || 
                (kevent->keycode == KEY_CTRL('w')) || 
                (kevent->keycode == KEY_CTRL('l'))) {

                stat = self->_erase(self);
                check_return(stat, self);

                stat = self->_draw(self);
                check_return(stat, self);

            } else if (kevent->keycode == KEY_F(10)) {

                stat = _workbench_handle_f10(self);
                check_return(stat, self);

            } else if (kevent->keycode == KEY_F(11)) {

                stat = _workbench_handle_f11(self);
                check_return(stat, self);

            } else if (kevent->keycode == KEY_F(12)) {

                stat = _workbench_handle_f12(self);
                check_return(stat, self);

            } else {

                if (self->panel != NULL) {

                    window_t *window = (window_t *)panel_userptr(self->panel);
                    check_null(window);

                    stat = window_event(window, event);
                    check_return(stat, window);

                }

            }

        }

        update_panels();

        stat = doupdate();
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;
    
    return stat;

}

int _workbench_remove(workbench_t *self, window_t *window) {

    int stat = OK;
    PANEL *panel = NULL;
    window_t *temp = NULL;

    when_error_in {

        errno = E_INVOPS;

        for (panel = panel_above(NULL);
             panel != NULL;
             panel = panel_above(panel)) {

            temp = (window_t *)panel_userptr(panel);
            check_null(temp);

            stat = window_compare(window, temp);
            check_return(stat, window);

            /* maintain the total number of panels */

            if (self->panels > 0) {

                self->panels--;

            }

            /* maintain the current panel */

            if (self->panel == panel) {

                if (self->panels > 0) {

                    self->panel = panel_below(panel);

                } else {

                    self->panel = NULL;

                }

            }

            /* remove the panel */

            stat = del_panel(panel);
            check_status(stat);

            stat = window_destroy(temp);
            check_return(stat, temp);

            break;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_set_menu(workbench_t *self, window_t *main) {

    int stat = OK;

    when_error_in {

        if (self->main != NULL) {

            stat = self->_remove(self, self->main);
            check_return(stat, self);

        }

        stat = self->_add(self, main);
        check_return(stat, self);

        self->main = main;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_set_focus(workbench_t *self, window_t *window) {

    int stat = OK;
    PANEL *panel = NULL;
    window_t *temp = NULL;

    when_error_in {

        errno = E_INVOPS;

        for (panel = panel_above(NULL);
             panel != NULL;
             panel = panel_above(panel)) {

            temp = (window_t *)panel_userptr(panel);
            check_null(temp);

            stat = window_compare(window, temp);
            check_return(stat, window);

            self->panel = panel;

            stat = top_panel(panel);
            check_status(stat);

            break;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_get_focus(workbench_t *self, window_t *window) {

    int stat = OK;
    window_t *temp = NULL;

    when_error_in {

        errno = E_INVOPS;

        if (self->panel == NULL) {

            cause_error(E_INVOPS);

        }

        temp = (window_t *)panel_userptr(self->panel);
        check_null(temp);

        memcpy(window, temp, sizeof(window_t));

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_init_terminal(workbench_t *self) {

    int row, col;
    int stat = OK;

    when_error_in {

        errno = E_INVOPS;

        setlocale(LC_ALL, "");

        /* initialize the terminal */

        errno = 0;
        if ((initscr() == NULL)) {

            cause_error(errno);

        }

        stat = cbreak();
        check_status(stat);

        stat = keypad(stdscr, TRUE);
        check_status(stat);

        stat = nodelay(stdscr, TRUE);
        check_status(stat);

        stat = noecho();
        check_status(stat);

        /* check for color capability */

        stat = has_colors();
        check_status2(stat, TRUE, E_NOCOLOR);

        init_colorpairs();

        /* prep the terminal screen */

        stat = erase();
        check_status(stat);

        stat = refresh();
        check_status(stat);

        stat = curs_set(0);
        check_status(stat);

        /* create the message window */

        getmaxyx(stdscr, row, col);
        self->messages = newwin(1, col, row - 1, 0);
        check_null(self->messages);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* event processing                                               */
/*----------------------------------------------------------------*/

int _workbench_dispatch(workbench_t *self, int *again) {

    int stat = OK;
    events_t *event = NULL;

    when_error_in {

        *again = FALSE;

        if ((event = que_pop_head(&self->events))) {

            stat = self->_event(self, event);
            check_return(stat, self);

            free(event->data);
            free(event);

            if (que_empty(&self->events)) {

                stat = que_init(&self->events);
                check_status(stat);

            } else {

                *again = TRUE;

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_queue_event(workbench_t *self, events_t *event) {

    int stat = OK;

    when_error_in {

        errno = E_INVOPS;

        if (que_empty(&self->events)) {

            stat = que_init(&self->events);
            check_status(stat);

        }

        stat = que_push_tail(&self->events, event);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _workbench_read_stdin(workbench_t *self) {

    int ch;
    int stat = OK;

fprintf(stderr, "entering _workbench_read_stdin()\n");
    when_error_in {

        if ((ch = getch()) != ERR) {

            errno = 0;
            events_t *event = calloc(1, sizeof(events_t));
            if (event == NULL) cause_error(errno);

            errno = 0;
            KEVENT *kevent = calloc(1, sizeof(KEVENT));
            if (kevent == NULL) cause_error(errno);

            kevent->keycode = ch;

            event->type = EVENT_K_KEYBOARD;
            event->data = (void *)kevent;

            stat = self->_queue_event(self, event);
            check_status2(stat, OK, E_NOQUEUE);

        }

        exit_when;

    } use {

        stat = ERR;
        stat = ERR;
        process_error(self);

    } end_when;

fprintf(stderr, "leaving _workbench_read_stdin() - stat: %d\n", stat);
    return stat;

}

/* private methods --------------------------------------------------------*/

static int _workbench_handle_f10(workbench_t *self) {

    int stat = OK;
    PANEL *panel = NULL;
    window_t *temp = NULL;

    when_error_in {

        if (self->main != NULL) {

            for (panel = panel_above(NULL);
                 panel != NULL;
                 panel = panel_above(panel)) {

                temp = (window_t *)panel_userptr(panel);

                if ((window_compare((window_t *)self->main, temp)) == OK) {

                    self->panel = panel;

                    stat = top_panel(panel);
                    check_status2(stat, OK, E_INVOPS);

                    stat = window_refresh(self->main);
                    check_return(stat, self->main);

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

static int _workbench_handle_f11(workbench_t *self) {

    int stat = OK;
    PANEL *current = NULL;
    window_t *window = NULL;

    when_error_in {

        if ((self->panels > 0)) {

            if ((current = panel_above(NULL)) != NULL) {

                window = panel_userptr(current);
                self->panel = current;

                stat = top_panel(current);
                check_status2(stat, OK, E_INVOPS);

                stat = window_refresh(window);
                check_return(stat, window);

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _workbench_handle_f12(workbench_t *self) {

    int stat = OK;
    PANEL *current = NULL;
    window_t *window = NULL;

    when_error_in {

        if ((self->panels > 1)) {

            window = panel_userptr(self->panel);

            if ((window_compare((window_t *)self->main, window)) == ERR) {

                stat = self->_remove(self, window);
                check_return(stat, self);

                update_panels();
                stat = doupdate();
                check_status2(stat, OK, E_INVOPS);

                if ((current = panel_below(NULL)) != NULL) {

                    self->panel = current;

                } else {

                    stat = _workbench_queue_exit(self);
                    check_return(stat, self);

                }

            }

        } else {

            stat = _workbench_queue_exit(self);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _workbench_remove_all(workbench_t *self) {

    int stat = OK;
    PANEL *panel = NULL;
    events_t *event = NULL;
    window_t *window = NULL;

fprintf(stderr, "entering _workbench_remove_all()\n");
    when_error_in {

        while ((event = que_pop_head(&self->events))) {

            free(event->data);
            free(event);

        }

        if (que_empty(&self->events)) {

            stat = que_init(&self->events);
            check_status2(stat, OK, E_INVOPS);

        }

        while ((panel = panel_above(NULL))) {

            window = (window_t *)panel_userptr(panel);

            stat = window_destroy(window);
            check_return(stat, window);

            stat = del_panel(panel);
            check_status2(stat, OK, E_INVOPS);

        }

        stat = werase(self->messages);
        check_status2(stat, OK, E_INVOPS);

        stat = wnoutrefresh(self->messages);
        check_status2(stat, OK, E_INVOPS);

        stat = delwin(self->messages);
        check_status2(stat, OK, E_INVOPS);

        update_panels();
        stat = doupdate();
        check_status2(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

fprintf(stderr, "leaving _workbench_remove_all() - stat: %d\n", stat);
    return stat;

}

static int _workbench_queue_exit(workbench_t *self) {

    int stat = OK;
    events_t *event = NULL;

    when_error_in {

        errno = 0;
        event = calloc(1, sizeof(events_t));
        if (event == NULL) cause_error(errno);
                    
        event->type = EVENT_K_EXIT;
        event->data = NULL;
                    
        stat = self->_queue_event(self, event);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;
    
}

