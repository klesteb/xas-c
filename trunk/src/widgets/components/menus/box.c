
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

#include "menus_priv.h"
#include "xas/widgets/components/menus/menus.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _box_menu_event(widget_t *widget, events_t *event) {

    int stat = ERR;
    error_trace_t errors;
    userptr_data_t *userptr = NULL;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            menus_data_t *data = (menus_data_t *)self->data;

            if (event->type == EVENT_K_KEYBOARD) {

                KEVENT *kevent = (KEVENT *)event->data;

                switch(kevent->keycode) {
                    case KEY_UP: {
                        stat = menu_driver(data->menu, REQ_UP_ITEM);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case 9:
                    case KEY_DOWN: {
                        stat = menu_driver(data->menu, REQ_DOWN_ITEM);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case KEY_LEFT: {
                        stat = menu_driver(data->menu, REQ_LEFT_ITEM);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case KEY_RIGHT: {
                        stat = menu_driver(data->menu, REQ_RIGHT_ITEM);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case KEY_HOME: {
                        stat = menu_driver(data->menu, REQ_FIRST_ITEM);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case KEY_END: {
                        stat = menu_driver(data->menu, REQ_LAST_ITEM);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case KEY_NPAGE: {
                        stat = menu_driver(data->menu, REQ_SCR_DPAGE);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case KEY_PPAGE: {
                        stat = menu_driver(data->menu, REQ_SCR_UPAGE);
                        check_status2(stat, E_OK, stat);
                        break;
                    }
                    case 10:
                    case KEY_ENTER: {
                        ITEM *item = NULL;
                        item = current_item(data->menu);
                        if (item != NULL) {
                            if ((userptr = item_userptr(item)) != NULL) {
                                if (userptr->callback != NULL) {
                                    errno = 0;
                                    stat = userptr->callback(userptr->data, userptr->data_size);
                                    check_status(stat);
                                }
                            }
                        }
                        break;
                    }
                }

                stat = (*data->show_description)(widget);
                check_return(stat, widget);

                stat = pos_menu_cursor(data->menu);
                check_status2(stat, E_OK, stat);

                data->focus = (void *)current_item(data->menu);

            }

            stat = wnoutrefresh(self->area);
            check_status2(stat, OK, E_INVOPS);

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

component_t *box_menu_create(window_t *window, int startx, int starty, int height, int width, int tab, int (*display)(const char *), menus_list_t *list, int list_size) {

    int x;
    int stat = OK;
    int count = 0;
    int length = 0;
    item_list_t items[2];
    component_t *self = NULL;
    menus_data_t *data = NULL;

    when_error_in {

        self = menus_create(window, startx, starty, height, width, tab, list, list_size);
        check_creation(self);

        /* find the longest label */

        count = (list_size / sizeof(menus_list_t));
        for (x = 0; x < count; x++) {

            if (strlen(list[x].label) > length) {

                length = strlen(list[x].label);

            }

        }

        data = self->data;
        data->col = width / length;
        data->row = height;
        data->mark = ">";
        data->callback = display;
        data->options = (O_ONEVALUE | O_ROWMAJOR | O_IGNORECASE | O_SHOWMATCH);
        self->data = data;

        SET_ITEM(items[0], WIDGET_M_EVENT, _box_menu_event, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

        stat = component_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        process_error(self);

    } end_when;

    return self;

}

