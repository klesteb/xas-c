
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2023 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "xas/rms/lf.h"
#include "xas/rms/fib.h"
#include "xas/object.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"

extern int xgetline(int, char *, int, int);

require_klass(FIB_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _lf_ctor(object_t *, item_list_t *);
int _lf_dtor(object_t *);
int _lf_compare(lf_t *, lf_t *);
int _lf_override(lf_t *, item_list_t *);

int _lf_gets(lf_t *, char *, size_t, ssize_t *);
int _lf_puts(lf_t *, char *, ssize_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(LF_KLASS) {
    .size = KLASS_SIZE(lf_t),
    .name = KLASS_NAME(lf_t),
    .ctor = _lf_ctor,
    .dtor = _lf_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

lf_t *lf_create(char *filename) {

    int stat = ERR;
    lf_t *self = NULL;
    item_list_t items[2];

    SET_ITEM(items[0], FIB_K_PATH, filename, strlen(filename), NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    self = (lf_t *)object_create(LF_KLASS, items, &stat);

    return self;

}

int lf_destroy(lf_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, lf_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

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

int lf_override(lf_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

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

int lf_compare(lf_t *us, lf_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, lf_t)) {

                stat = us->_compare(us, them);
                check_status(stat, OK, E_NOTSAME);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

char *lf_version(lf_t *self) {

    char *version = VERSION;

    return version;

}

int lf_gets(lf_t *self, char *buffer, size_t size, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL) && (count != NULL)) {

            stat = self->_gets(self, buffer, size, count);
            check_return(stat, self);

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

int lf_puts(lf_t *self, char *buffer, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL) && (count != NULL)) {

            stat = self->_puts(self, buffer, count);
            check_return(stat, self);

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

int lf_get_eol(lf_t *self, char *eol) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (eol != NULL)) {

            eol = self->eol;

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

int lf_set_eol(lf_t *self, char *eol) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (eol != NULL)) {

            self->eol = eol;

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _lf_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    char *eol = "\n";
    lf_t *self = NULL;

    if (object != NULL) {

        stat = OK;
        
        when_error_in {
            
            /* intialize the base klass here */
        
            stat = FIB_KLASS->ctor(object, items);
            check_return(stat, object);
        
            /* capture our items */

            /* if (items != NULL) { */
               
            /*    int x; */
            /*    for (x = 0;; x++) { */
                                   
            /*        if ((items[x].buffer_length == 0) && */
            /*            (items[x].item_code == 0)) break; */
                                   
            /*        switch(items[x].item_code) { */
            /*            case BELL_K_TIMES: { */
            /*                memcpy(&times, */
            /*                       items[x].buffer_address, */
            /*                       items[x].buffer_length); */
            /*                break; */
            /*            } */
            /*        } */

            /*    } */

            /* } */

            /* initilize our base klass here */

            object_set_error1(object, OK);

            /* initialize our derived klass here */

            self = LF(object);

            /* assign our methods here */

            self->ctor = _lf_ctor;
            self->dtor = _lf_dtor;
            self->_compare = _lf_compare;
            self->_override = _lf_override;

            self->_gets = _lf_gets;
            self->_puts = _lf_puts;

            /* initialize internal variables here */

            self->eol = eol;

            exit_when;

        } use {

            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _lf_dtor(object_t *object) {

    int stat = OK;
    fib_t *fib = FIB(object);

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, fib_t);
    fib_destroy(fib);

    return stat;

}

int _lf_override(lf_t *self, item_list_t *items) {

    int stat = OK;

    if (items != NULL) {

        when_error_in {

            stat = fib_override(FIB(self), items);
            check_return(stat, self);

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case LF_M_DESTRUCTOR: {
                        self->dtor = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case LF_M_GETS: {
                        self->_gets = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case LF_M_PUTS: {
                        self->_puts = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                }

            } 

        } use { 
            
            stat = ERR;
            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _lf_compare(lf_t *self, lf_t *other) {

    int stat = ERR;

    if ((fib_compare(FIB(self), FIB(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_gets == other->_gets) &&
        (self->_puts == other->_puts)) {

        stat = OK;

    }

    return stat;

}

int _lf_puts(lf_t *self, char *buffer, ssize_t *count) {

    int stat = OK;
    char *output = NULL;
    int fd = FIB(self)->fd;
    int length = strlen(buffer) + 2;

    when_error_in {

        errno = 0;
        if ((output = calloc(1, length + 2)) == NULL) {

            cause_error(errno);

        }

        sprintf(output, "%s%s", buffer, self->eol);

        errno = 0;
        if ((*count = write(fd, output, strlen(output))) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    if (output != NULL) free(output);

    return stat;

}

int _lf_gets(lf_t *self, char *buffer, size_t length, ssize_t *count) {

    int stat = OK;
    int fd = FIB(self)->fd;

    when_error_in {

        errno = 0;
        if ((*count = xgetline(fd, buffer, length, '\n')) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

