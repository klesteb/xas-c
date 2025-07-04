
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

#ifndef _XAS_WIDGETS_COMPONENTS_MORE_H_
#define _XAS_WIDGETS_COMPONENTS_MORE_H_

#include "xas/queue.h"

#include "xas/widgets/window.h"
#include "xas/widgets/component.h"

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

extern component_t *more_create(window_t *, int, int, int, int, int, queue_t *text);

#endif

