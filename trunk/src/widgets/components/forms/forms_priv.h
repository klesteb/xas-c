
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

#ifndef _COMPONENTS_FORMS_FORMS_PRIV_H
#define _COMPONENTS_FORMS_FORMS_PRIV_H

#include <form.h>

#include "xas/types.h"
#include "xas/errors_xas.h"
#include "xas/error_handler.h"
#include "xas/widgets/colors.h"
#include "xas/widgets/window.h"
#include "xas/widgets/component.h"

/*----------------------------------------------------------------*/
/* data strutures                                                 */
/*----------------------------------------------------------------*/

typedef struct _forms_data_s {
    int ins;
    FORM *form;
    FIELD **fields;
} forms_data_t;

#endif

