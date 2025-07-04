
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <ncurses.h>
#include <errno.h>

#include "xas/misc/misc.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/widgets/components/more.h"
#include "xas/widgets/components/yesno.h"
#include "xas/widgets/components/hline.h"
#include "xas/widgets/windows/query_window.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

window_t *query_window(char *title, int (*callback)(int), char *fmt, ...) {

    va_list ap;
    int col = 0;
    int row = 0;
    queue_t lines;
    int stat = OK;
    char buf[1024];
    int startx = 0;
    int starty = 0;
    int height = 6;
    int width  = 60;
    window_t *win = NULL;
    component_t *more = NULL;
    component_t *yesno = NULL;
    component_t *hline = NULL;
    char *label = "F7=Yes  F8=No";

    when_error_in {

        errno = 0;
        stat = que_init(&lines);
        check_status(stat);

        memset(buf, '\0', 1024);

        va_start(ap, fmt);
        vsnprintf(buf, 1023, fmt, ap);
        va_end(ap);

        errno = E_INVOPS;
        stat = wordwrap(buf, 48, &lines);
        check_status(stat);

        startx = ((getmaxx(stdscr) - width) / 2);
        starty = ((getmaxy(stdscr) - height) / 2);

        win = window_create(title, startx, starty, height, width, TRUE);
        check_creation(win);

        more = more_create(win, 1, 1, 3, 58, 1, &lines);
        check_creation(more);

        row = height - 2;
        hline = hline_create(win, row, col, width);
        check_creation(hline);

        row = height - 1;
        col = ((width - strlen(label)) / 2);
        width = strlen(label) + 1;
        yesno = yesno_create(win, row, col, 1, width, 2, label, callback);
        check_creation(yesno);

        stat = window_add(win, more);
        check_return(stat, win);

        stat = window_add(win, hline);
        check_return(stat, win);

        stat = window_add(win, yesno);
        check_return(stat, win);

        exit_when;

    } use {

        process_error(win);

    } end_when;

    return win;

}

