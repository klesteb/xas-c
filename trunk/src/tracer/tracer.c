
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

#include "xas/tracer.h"
#include "xas/errors.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _tracer_ctor(object_t *, item_list_t *);
int _tracer_dtor(object_t *);

int _tracer_compare(tracer_t *, tracer_t *);
int _tracer_override(tracer_t *, item_list_t *);
int _tracer_add(tracer_t *, error_trace_t *);
int _tracer_dump(tracer_t *, int (*output)(char *));

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(TRACER_KLASS) {
    .size = KLASS_SIZE(tracer_t),
    .name = KLASS_NAME(tracer_t),
    .ctor = _tracer_ctor,
    .dtor = _tracer_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

tracer_t *tracer_create(err_t *errors) {

    int stat = ERR;
    item_list_t items[2];
    tracer_t *self = NULL;

    if (errors != NULL) {

        SET_ITEM(items[0], TRACER_K_ERRORS, errors, 0, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

    } else {

        SET_ITEM(items[0], 0, 0, 0, 0);
        SET_ITEM(items[1], 0, 0, 0, 0);

    }

    self = (tracer_t *)object_create(TRACER_KLASS, items, &stat);

    return self;

}

int tracer_destroy(tracer_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, tracer_t)) {

                stat = self->dtor(OBJECT(self));
                check_return(stat, self);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int tracer_override(tracer_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_override(self, items);
            check_return(stat, self);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int tracer_compare(tracer_t *us, tracer_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, tracer_t)) {

                stat = us->_compare(us, them);
                check_return(stat, us);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(us);

    } end_when;

    return stat;

}

int tracer_add(tracer_t *self, error_trace_t *error) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (error != NULL)) {

            stat = self->_add(self, error);
            check_return(stat, self);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int tracer_dump(tracer_t *self, int (*output)(char *)) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_dump(self, output);
            check_return(stat, self);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

char *tracer_version(tracer_t *self) {
    
    char *version = VERSION;

    return version;
    
}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _tracer_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    err_t *errs = NULL;
    tracer_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case TRACER_K_ERRORS: {
                        errs = items[x].buffer_address;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = TRACER(object);

        /* assign our methods here */

        self->ctor = _tracer_ctor;
        self->dtor = _tracer_dtor;
        self->_compare = _tracer_compare;
        self->_override = _tracer_override;
        self->_add = _tracer_add;
        self->_dump = _tracer_dump;

        /* initialize internal variables here */

        when_error_in {

            self->errs = errs;

            errno = 0;
            stat = que_init(&self->errors);
            check_status(stat);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _tracer_dtor(object_t *object) {

    int stat = OK;
    error_trace_t *error = NULL;
    tracer_t *tracer = TRACER(object);

    when_error_in {
        
        /* free local resources here */

        while ((error = que_pop_head(&tracer->errors))) {

            free(error->filename);
            free(error->function);
            free(error);

        }

        errno = 0;
        stat = que_init(&tracer->errors);
        check_status(stat);

        /* walk the chain, freeing as we go */

        object_demote(object, object_t);
        object_destroy(object);

        exit_when;

    } use {

        stat = ERR;
        process_error(tracer);

    } end_when;

    return stat;

}

int _tracer_override(tracer_t *self, item_list_t *items) {

    int stat = ERR;

    when_error_in {
        
        if (items != NULL) {

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case TRACER_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case TRACER_M_ADD: {
                        self->_add = NULL;
                        self->_add = items[x].buffer_address;
                        check_null(self->_add);
                        break;
                    }
                    case TRACER_M_DUMP: {
                        self->_dump = NULL;
                        self->_dump = items[x].buffer_address;
                        check_null(self->_dump);
                        break;
                    }
                }

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _tracer_compare(tracer_t *self, tracer_t *other) {

    int stat = OK;

    when_error_in {

        if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override) &&
            (self->_add == other->_add) &&
            (self->_dump == other->_dump)) {

            stat = OK;

        } else {

            cause_error(E_NOTSAME);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _tracer_add(tracer_t *self, error_trace_t *error) {

    int stat = OK;

    when_error_in {

        errno = 0;
        stat = que_push_tail(&self->errors, error);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _tracer_dump(tracer_t *self, int (*output)(char *)) {

    int stat = OK;
    char text[1024];
    char nemonic[32];
    char message[1024];
    error_trace_t *error = NULL;
    char *format = "  %s - %s; line: %d, file: %s, function: %s";

    when_error_in {

        output("error trace follows:");

        for (error = que_first(&self->errors);
             error != NULL;
             error = que_next(&self->errors)) {

            stat = err_get_text(self->errs, error->errnum, nemonic, 32);
            check_return(stat, self->errs);

            stat = err_get_message(self->errs, error->errnum, message, 1024);
            check_return(stat, self->errs);

            snprintf(text, 1024, format, nemonic, message, 
                     error->lineno, error->filename, error->function);

            output(text);

        }

        output("error trace done.");

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

