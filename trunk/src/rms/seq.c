
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

#include "xas/rms/seq.h"
#include "xas/rms/fib.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"

extern int xgetline(int, char *, int, int);

require_klass(FIB_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _seq_ctor(object_t *, item_list_t *);
int _seq_dtor(object_t *);
int _seq_compare(seq_t *, seq_t *);
int _seq_override(seq_t *, item_list_t *);

int _seq_gets(seq_t *, char *, size_t, ssize_t *);
int _seq_puts(seq_t *, char *, ssize_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(SEQ_KLASS) {
    .size = KLASS_SIZE(seq_t),
    .name = KLASS_NAME(seq_t),
    .ctor = _seq_ctor,
    .dtor = _seq_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

seq_t *seq_create(char *filename) {

    int stat = ERR;
    seq_t *self = NULL;
    item_list_t items[2];

    SET_ITEM(items[0], FIB_K_PATH, filename, strlen(filename), NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    self = (seq_t *)object_create(SEQ_KLASS, items, &stat);

    return self;

}

int seq_destroy(seq_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, seq_t)) {

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

int seq_override(seq_t *self, item_list_t *items) {

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

int seq_compare(seq_t *us, seq_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, seq_t)) {

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

char *seq_version(seq_t *self) {

    char *version = PACKAGE_VERSION;

    return version;

}

int seq_gets(seq_t *self, char *buffer, size_t size, ssize_t *count) {

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
        process_error(self);

    } end_when;

    return stat;

}

int seq_puts(seq_t *self, char *buffer, ssize_t *count) {

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
        process_error(self);

    } end_when;

    return stat;

}

int seq_get_eol(seq_t *self, char *eol) {
    
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
        process_error(self);

    } end_when;

    return stat;

}

int seq_set_eol(seq_t *self, char *eol) {

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
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _seq_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    char *eol = "\n";
    seq_t *self = NULL;

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

            self = SEQ(object);

            /* assign our methods here */

            self->ctor = _seq_ctor;
            self->dtor = _seq_dtor;
            self->_compare = _seq_compare;
            self->_override = _seq_override;

            self->_gets = _seq_gets;
            self->_puts = _seq_puts;

            /* initialize internal variables here */

            self->eol = eol;

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _seq_dtor(object_t *object) {

    int stat = OK;
    fib_t *fib = FIB(object);

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, fib_t);
    fib_destroy(fib);

    return stat;

}

int _seq_override(seq_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {
    
        if (items != NULL) {

            stat = fib_override(FIB(self), items);
            check_return(stat, self);

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case SEQ_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case SEQ_M_GETS: {
                        self->_gets = NULL;
                        self->_gets = items[x].buffer_address;
                        check_null(self->_gets);
                        break;
                    }
                    case SEQ_M_PUTS: {
                        self->_puts = NULL;
                        self->_puts = items[x].buffer_address;
                        check_null(self->_puts);
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

int _seq_compare(seq_t *self, seq_t *other) {

    int stat = OK;

    when_error_in {

        if ((fib_compare(FIB(self), FIB(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override) &&
            (self->_gets == other->_gets) &&
            (self->_puts == other->_puts)) {

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

int _seq_puts(seq_t *self, char *buffer, ssize_t *count) {

    int fd;
    int stat = OK;
    char *output = NULL;
    int length = strlen(buffer) + 2;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

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
        process_error(self);

    } end_when;

    if (output != NULL) free(output);

    return stat;

}

int _seq_gets(seq_t *self, char *buffer, size_t length, ssize_t *count) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        errno = 0;
        if ((*count = xgetline(fd, buffer, length, '\n')) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

