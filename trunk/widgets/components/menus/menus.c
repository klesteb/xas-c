
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

#include "xas/widgets/components/menus/menus.h"
#include "xas/widgets/components/menus/menus_priv.h"

require_klass(COMPONENT_KLASS);

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

int _menus_show_description(widget_t *widget) {

    int stat = ERR;
    ITEM *item = NULL;
    const char *description = NULL;
    menus_data_t *data = COMPONENT(widget)->data;

    when_error_in {

        if ((item = current_item(data->menu)) != NULL) {

            if ((description = item_description(item)) != NULL) {

                if (data->callback != NULL) {

                    errno = 0;
                    stat = (*data->callback)(description);
                    check_status(stat);

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

int _menus_data_create(component_t *menus, menus_list_t *list, int list_size) {

    int stat = OK;
    int startx = 0;
    int starty = 0;
    int height = 0;
    int width  = 0;
    menus_data_t *data = NULL;
    userptr_data_t *userptr = NULL;

    when_error_in {

        errno = 0;
        data = calloc(1, sizeof(menus_data_t));
        check_null(data);

        errno = 0;
        data->items_count = (list_size / sizeof(menus_list_t));
        data->items = calloc(data->items_count + 1, sizeof(ITEM));
        check_null(data->items);

        int x;
        for (x = 0; x < data->items_count; x++) {

            errno = 0;
            data->items[x] = new_item(list[x].label, list[x].description);
            check_null(data->items[x]);

            if (list[x].type == MENUS_T_SEPERATOR) {

                stat = item_opts_off(data->items[x], O_SELECTABLE);
                check_status2(stat, E_OK, stat);

            }

            errno = 0;
            userptr = calloc(1, sizeof(userptr_data_t));
            check_null(userptr);

            userptr->data = list[x].data;
            userptr->callback = list[x].callback;
            userptr->data_size = list[x].data_size;

            set_item_userptr(data->items[x], userptr);

        }

        data->items[x] = NULL;
        data->show_description = _menus_show_description;

        height = getmaxx(menus->area) - 1;
        width  = getmaxy(menus->area) - 1;

        errno = E_INVOPS;
        data->subwin = derwin(menus->area, width, height, startx, starty);
        check_null(data->subwin);

        stat = werase(data->subwin);
        check_status(stat);

        menus->data = data;

        exit_when;

    } use {

        stat = ERR;
        process_error(menus);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _menus_dtor(object_t *object) {

    int stat = OK;
    component_t *self = COMPONENT(object);
    menus_data_t *data = COMPONENT(object)->data;

    when_error_in {

        errno = E_INVOPS;

        /* free local resources here */

        if (data != NULL) {

            if (data->subwin != NULL) {

                stat = werase(data->subwin);
                check_status(stat);

                stat = delwin(data->subwin);
                check_status(stat);

            }

            int x;
            for (x = 0; x <= data->items_count; x++) {

                void *junk = item_userptr(data->items[x]);
                if (junk != NULL) free(junk);

                free(data->items[x]);

            }

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
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(object);

    } end_when;

    return stat;

}

int _menus_add(widget_t *widget, void *data) {

    return OK;

}

int _menus_draw(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);
    menus_data_t *data = COMPONENT(widget)->data;

    when_error_in {

        if (data != NULL) {

            stat = wattrset(self->area, widget->theme->attribute);
            check_status2(stat, OK, E_INVOPS);

            stat = wcolorset(self->area, widget->theme->foreground, widget->theme->background);
            check_status2(stat, OK, E_INVOPS);

            stat = wbkgd(self->area, COLOR_PAIR(colornum(widget->theme->foreground, widget->theme->background)));
            check_status2(stat, OK, E_INVOPS);

            errno = 0;
            data->menu = new_menu(data->items);
            check_null(data->menu);

            stat = set_menu_opts(data->menu, data->options);
            check_status2(stat, E_OK, stat);

            stat = set_menu_mark(data->menu, data->mark);
            check_status2(stat, E_OK, stat);

            stat = set_menu_format(data->menu, data->row, data->col);
            check_status2(stat, E_OK, stat);

            stat = set_menu_win(data->menu, self->area);
            check_status2(stat, E_OK, stat);

            stat = set_menu_sub(data->menu, data->subwin);
            check_status2(stat, E_OK, stat);

            stat = post_menu(data->menu);
            check_status2(stat, E_OK, stat);

            if (data->focus != NULL) {

                ITEM *item = data->focus;
                stat = set_current_item(data->menu, item);
                check_status(stat, E_OK, stat);

            } else {

                stat = set_current_item(data->menu, data->items[0]);
                check_status2(stat, E_OK, stat);

            }

            stat = (*data->show_description)(widget);
            check_return(stat, widget);

            stat = pos_menu_cursor(data->menu);
            check_status(stat, E_OK, stat);

            stat = curs_set(1);
            check_status2(stat, OK, E_INVOPS);

        }

        stat = wstandend(self->area);
        check_status2(stat, OK, E_INVOPS);

        stat = wnoutrefresh(self->area);
        check_status2(stat, OK, E_INVOPS);

        COMPONENT(widget)->data = data;

        exit_when;

    } use {

        stat = ERR;
        process_error(widget);

    } end_when;

    return stat;

}

int _menus_erase(widget_t *widget) {

    int stat = OK;
    component_t *self = COMPONENT(widget);

    when_error_in {

        if (self->data != NULL) {

            menus_data_t *data = (menus_data_t *)self->data;

            if (data->menu != NULL) {

                stat = unpost_menu(data->menu);
                check_status2(stat, E_OK, stat);

                stat = free_menu(data->menu);
                check_status2(stat, E_OK, stat);

                data->menu = NULL;

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

int _menus_event(widget_t *widget, events_t *event) {

    return OK;

}

int _menus_remove(widget_t *widget, void *thing) {

    return OK;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

component_t *menus_create(window_t *window, int startx, int starty, int height, int width, int tab, menus_list_t *list, int size) {

    int stat = OK;
    int padding = FALSE;
    item_list_t items[7];
    component_t *menus = NULL;

    when_error_in {

        menus = component_create(window, startx, starty, height, width, tab, padding, NULL, 0);
        check_creation(menus);

        stat = _menus_data_create(menus, list, size);
        check_return(stat, menus);

        SET_ITEM(items[0], WIDGET_M_ADD, _menus_add, 0, NULL);
        SET_ITEM(items[1], WIDGET_M_DRAW, _menus_draw, 0, NULL);
        SET_ITEM(items[2], WIDGET_M_EVENT, _menus_event, 0, NULL);
        SET_ITEM(items[3], WIDGET_M_ERASE, _menus_erase, 0, NULL);
        SET_ITEM(items[4], WIDGET_M_DESTROY, _menus_dtor, 0, NULL);
        SET_ITEM(items[5], WIDGET_M_REMOVE, _menus_remove, 0, NULL);
        SET_ITEM(items[6], 0, 0, 0, 0);

        stat = component_override(menus, items);
        check_return(stat, menus);

        exit_when;

    } use {

        process_error(menus);

    } end_when;

    return menus;

}

