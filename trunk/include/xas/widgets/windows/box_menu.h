
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

#ifndef _XAS_WIDGETS_WINDOWS_BOX_MENU_H_
#define _XAS_WIDGETS_WINDOWS_BOX_MENU_H_

#include "xas/widgets/window.h"
#include "xas/widgets/components/menus/menus.h"

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern window_t *box_menu(char *, int, int, int, int, int (*display)(const char *, error_trace_t *), menus_list_t *, int);

#endif

