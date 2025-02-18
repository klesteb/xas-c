
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

#ifndef _COMPONENTS_MENUS_LIST_MENU_H
#define _COMPONENTS_MENUS_LIST_MENU_H

#include "xas/widgets/window.h"
#include "xas/widgets/component.h"
#include "xas/widgets/components/menus/menus.h"

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern component_t *list_menu_create(window_t *, int, int, int, int, int, int (*display)(const char *), menus_list_t *, int);

#endif

