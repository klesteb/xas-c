/*
@(#)  FILE: vperror.h  RELEASE: 1.3  DATE: 03/16/95, 12:07:07
*/
/*******************************************************************************

    vperror.h

    Formatted System Error Message Output.

*******************************************************************************/

#ifndef  VPERROR_H	               /* Has the file been INCLUDE'd already? */
#define  VPERROR_H  yes

#ifdef __cplusplus
    extern  "C" {
#endif


#include <stdio.h>                 /* Standard I/O definitions. */
#include "xas/gpl/ansi_setup.h"    /* ANSI or non-ANSI C?       */
#include "xas/types.h"             /* xas types                 */

/****************************************************************************
    VPERROR_PRINT - turns VPERROR output on or off.  Set VPERROR_PRINT to a
        a non-zero value to turn output on; set it to zero to turn output off.
    VPERROR_FILE - is the (FILE *) pointer for VPERROR's output device; the
        default destination of error output is STDERR.
*****************************************************************************/

extern  int vperror_print;          /* 0 = no print, !0 = print */
extern  FILE  *vperror_file;        /* VPERROR output device    */

/****************************************************************************
    Public functions.
*****************************************************************************/

extern void vperror_init(void (*stuff)(error_trace_t *error));
extern void vperror_output P_((int lineno, char *file, const char *func, char *format, ...));

#define vperror(format, ...) vperror_output(__LINE__, __FILE__, __func__, format, ##__VA_ARGS__)

#ifdef __cplusplus
    }
#endif

#endif				/* If this file was not INCLUDE'd previously. */
