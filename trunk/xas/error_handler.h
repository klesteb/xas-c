
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "xas/object.h"
#include "xas/types.h"

/**
 * @file error_handler.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief A macro package to implement structured error handling.
 * 
 * @par Description
 * This is a macro package to implement a structured error handling
 * solution. This takes a different path from the more common 
 * "try/catch" approach. This emulates the DEC/HP Basic "when error"
 * blocks for error handling. 
 * 
 * There are some caveats; you can't nest blocks, you can't easily 
 * propagate an error up the call chain. The tracer package helps with
 * this problem.
 * 
 * But it does handle the chore of checking return codes and then
 * branching to an error handler when an error occurs. Errors are 
 * determined if an OK or ERR is returned.
 * 
 **/

#define when_error \
    do { \
        static int trace_lines = 0;     \
        static error_trace_t _er_trace; \

#define when_error_in \
    do { \
        static int trace_lines = 0;     \
        static error_trace_t _er_trace; \

#define end_when                        \
    } while(0);

#define exit_when                       \
        trace_lineno = trace_lines;     \
        break;

#define use when_handler:

#define trace_errnum   _er_trace.errnum
#define trace_lineno   _er_trace.lineno
#define trace_filename _er_trace.filename
#define trace_function _er_trace.function

#define clear_error(error) {               \
    trace_lines  = 0;                      \
    trace_errnum = 0;                      \
    trace_lineno = 0;                      \
    free(trace_filename);                  \
    free(trace_function);                  \
}

#define retry(label) {                     \
    clear_error();                         \
    goto label;                            \
}

#define cause_error(error) {               \
    trace_errnum = (error);                \
    trace_lineno = __LINE__ - trace_lines; \
    trace_filename = strdup(__FILE__);     \
    trace_function = strdup(__func__);     \
    goto when_handler;                     \
}

#define check_status(status) {             \
    if ((status) != (OK)) {                \
        trace_lines = 1;                   \
        cause_error((errno));              \
    }                                      \
}

/* So why a second way to check the return status?            */
/*                                                            */
/* The standard form is to return a success/failure           */
/* status from the function call and on failure, check        */ 
/* the global variable errno to retrieve the error code.      */
/*                                                            */
/* This is how the kernel and the original utilities did it.  */
/* Those utilities would then return a success/failure status */
/* to the command line and the reason on stderr. This is the  */
/* proper way to write robust systems and the only way to     */
/* code in C, which has no built in error handling.           */
/*                                                            */
/* Since errno is a global variable, there is perceived       */
/* problems with this approach. Especially in threaded        */
/* code. Those problems have been resolved over the years,    */
/* but some things never change or have been standardized     */
/* in a certain way.                                          */
/*                                                            */ 
/* Ncurses is a prime example. The main routines as           */
/* defined in ncurses.h return success/failure and dosen't    */
/* bother to set errno. The companion routines as defined     */
/* in menu.h, panel.h and form.h. return success or an        */
/* error code. Why? Just the whim of the original authors.    */
/* I am sure they had their reasons. But consistence would    */
/* have been nice.                                            */
/*                                                            */
/* This is what happens when nobody enforces a set of rules   */
/* and UNIX/Linux is full of this type of stuff.              */
/*                                                            */
/* Contrary to what Linus says, this is not a good thing.     */
/*                                                            */

#define check_status2(status, wanted, error) { \
    if ((status) != (wanted)) {                \
        trace_lines = 1;                       \
        errno = error;                         \
        cause_error((errno));                  \
    }                                          \
}

#define check_null(value) {                \
    if ((value) == NULL) {                 \
        trace_lines = 1;                   \
        cause_error((errno));              \
    }                                      \
}

/* error handling for objects */

#define retrieve_error(self) {                   \
    object_get_error(OBJECT(self), &_er_trace);  \
}

#define check_return(status, self) {             \
    if ((status) != (OK)) {                      \
        retrieve_error((self));                  \
        trace_lines = 1;                         \
        errno = trace_errnum;                    \
        cause_error((errno));                    \
    }                                            \
}

#define check_creation(self) {                   \
    retrieve_error((self));                      \
    if (trace_errnum != (OK)) {                  \
        trace_lines = 1;                         \
        errno = trace_errnum;                    \
        cause_error((errno));                    \
    }                                            \
}

#define process_error(self) {     \
    object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function); \
    clear_error();                \
}

#endif

