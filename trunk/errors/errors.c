
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

#include <stdio.h>
#include <string.h>

#include "xas/errors.h"
#include "xas/errors_xas.h"
#include "xas/error_handler.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _err_ctor(object_t *object, item_list_t *);
int _err_dtor(object_t *);

int _err_compare(err_t *, err_t *);
int _err_override(err_t *, item_list_t *);
int _err_del(err_t *, int);
int _err_add(err_t *, int, char *, char *);
int _err_set(err_t *, int, char *, char *);
int _err_load(err_t *, error_code_t *, int);
int _err_get_text(err_t *, int, char *, int);
int _err_get_message(err_t *, int, char *, int);
int _err_load_system_errors(err_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(ERROR_KLASS) {
    .size = KLASS_SIZE(err_t),
    .name = KLASS_NAME(err_t),
    .ctor = _err_ctor,
    .dtor = _err_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

err_t *err_create(void) {

    int stat = ERR;
    err_t *self = NULL;

    self = (err_t *)object_create(ERROR_KLASS, NULL, &stat);

    return self;

}

int err_destroy(err_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, err_t)) {

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

int err_override(err_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (items != NULL)) {

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

int err_compare(err_t *us, err_t *them) {

    int stat = OK;

    when_error_in {

        if ((us != NULL) && (them != NULL)) {

            if (object_assert(them, err_t)) {

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

char *err_version(err_t *self) {
    
    char *version = VERSION;

    return version;
    
}

int err_get_text(err_t *self, int errnum, char *buffer, int size) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL)) {

            stat = self->_get_text(self, errnum, buffer, size);
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

int err_get_message(err_t *self, int errnum, char *buffer, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (buffer != NULL)) {

            stat = self->_get_message(self, errnum, buffer, size);
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

int err_add(err_t *self, int errnum, char *text, char *message) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (errnum != 0) && (text != NULL) && (message != NULL)) {

            stat = self->_add_error(self, errnum, text, message);
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

int err_remove(err_t *self, int errnum) {

    int stat = ERR;

    when_error {

        if ((self != NULL) && (errnum != 0)) {

            stat = self->_del_error(self, errnum);
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

int err_set(err_t *self, int errnum, char *text, char *message) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (errnum != 0)) {

            stat = self->_set_error(self, errnum, text, message);
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

int err_load(err_t *self, error_code_t *codes, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (codes != NULL)) {

            stat = self->_load_errors(self, codes, size);
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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _err_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    int codes_size = 0;
    err_t *self = NULL;
    error_code_t *codes = NULL;
    
    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case ERR_K_ERROR_CODES: {
                        codes = items[x].buffer_address;
                        codes_size = items[x].buffer_length;
                        break;
                    }

                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = ERRORS(object);

        /* assign our methods here */

        self->ctor = _err_ctor;
        self->dtor = _err_dtor;
        self->_compare = _err_compare;
        self->_override = _err_override;

        self->_add_error = _err_add;
        self->_del_error = _err_del;
        self->_set_error = _err_set;
        self->_get_text = _err_get_text;
        self->_get_message = _err_get_message;
        self->_load_errors = _err_load;
        self->_load_system_errors = _err_load_system_errors;

        when_error_in {

            /* initialize internal variables here */

            errno = 0;
            stat = que_init(&self->error_codes);
            check_status(stat);

            /* load the system error codes */

            stat = self->_load_system_errors(self);
            check_return(stat, self);

            /* load our error codes */

            stat = self->_load_errors(self, error_codes, sizeof(error_codes));
            check_return(stat, self);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _err_dtor(object_t *object) {

    int stat = OK;
    err_t *self = ERRORS(object);
    error_code_t *error_code = NULL;

    /* free local resources here */

    when_error_in {

        while ((error_code = que_pop_head(&self->error_codes))) {

            free(error_code->text);
            free(error_code->message);
            free(error_code);

        }

        errno = 0;
        stat = que_init(&self->error_codes);
        check_status(stat);

        /* walk the chain, freeing as we go */

        object_demote(object, object_t);
        object_destroy(object);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_override(err_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (items != NULL) {

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case ERR_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case ERR_M_ADD: {
                        self->_add_error = NULL;
                        self->_add_error = items[x].buffer_address;
                        check_null(self->_add_error);
                        break;
                    }
                    case ERR_M_DEL: {
                        self->_del_error = NULL;
                        self->_del_error = items[x].buffer_address;
                        check_null(self->_del_error);
                        break;
                    }
                    case ERR_M_SET: {
                        self->_set_error = NULL;
                        self->_set_error = items[x].buffer_address;
                        check_null(self->_set_error);
                        break;
                    }
                    case ERR_M_LOAD: {
                        self->_load_errors = NULL;
                        self->_load_errors = items[x].buffer_address;
                        check_null(self->_load_errors);
                        break;
                    }
                    case ERR_M_GET_TEXT: {
                        self->_get_text = NULL;
                        self->_get_text = items[x].buffer_address;
                        check_null(self->_get_text);
                        break;
                    }
                    case ERR_M_GET_MESSAGE: {
                        self->_get_message = NULL;
                        self->_get_message = items[x].buffer_address;
                        check_null(self->_get_message);
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

int _err_compare(err_t *self, err_t *other) {

    int stat = ERR;

    when_error_in {

        if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override) &&
            (self->_add_error == other->_add_error) &&
            (self->_del_error == other->_del_error) &&
            (self->_set_error == other->_set_error) &&
            (self->_load_errors == other->_load_errors) &&
            (self->_get_text == other->_get_text) &&
            (self->_get_message == other->_get_message)) {

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

int _err_get_text(err_t *self, int errnum, char *buffer, int bufsiz) {

    int stat = OK;
    int found = FALSE;
    error_code_t *error_code = NULL;

    when_error_in {

        for (error_code = que_first(&self->error_codes);
             error_code != NULL;
             error_code = que_next(&self->error_codes)) {

            if (error_code->errnum == errnum) {

                int x = strlen(error_code->text);
                int size = (bufsiz > x) ? bufsiz : x; 
                strncpy(buffer, error_code->text, size);
                found = TRUE;
                break;

            }

        }

        if (! found) {

            cause_error(E_NODATA);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_get_message(err_t *self, int errnum, char *buffer, int bufsiz) {

    int stat = OK;
    int found = FALSE;
    error_code_t *error_code = NULL;

    when_error_in {

        for (error_code = que_first(&self->error_codes);
             error_code != NULL;
             error_code = que_next(&self->error_codes)) {

            if (error_code->errnum == errnum) {

                int x = strlen(error_code->message);
                int size = (bufsiz > x) ? bufsiz : x; 
                strncpy(buffer, error_code->message, size);
                found = TRUE;
                break;

            }

        }

        if (! found) {

            cause_error(E_NODATA);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_add(err_t *self, int errnum, char *text, char *message) {

    int stat = OK;
    error_code_t *error_code = NULL;

    when_error_in {

        errno = 0;
        error_code = calloc(1, sizeof(error_code_t));
        check_null(error_code);

        error_code->errnum = errnum;
        error_code->text = strdup(text);
        error_code->message = strdup(message);

        errno = 0;
        stat = que_push_tail(&self->error_codes, error_code);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_set(err_t *self, int errnum, char *text, char *message) {

    int stat = OK;
    error_code_t *error_code = NULL;

    when_error_in {

        for (error_code = que_first(&self->error_codes);
             error_code != NULL;
             error_code = que_next(&self->error_codes)) {

            if (error_code->errnum == errnum) {

                free(error_code->text);
                free(error_code->message);

                error_code->text = strdup(text);
                error_code->message = strdup(message);

                errno = 0;
                stat = que_put(&self->error_codes, error_code);
                check_status(stat);

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_del(err_t *self, int errnum) {

    int stat = OK;
    error_code_t *error_code = NULL;

    when_error_in {
        
        for (error_code = que_first(&self->error_codes);
             error_code != NULL;
             error_code = que_next(&self->error_codes)) {

            if (error_code->errnum == errnum) {

                error_code = que_delete(&self->error_codes);

                free(error_code->text);
                free(error_code->message);
                free(error_code);
            
                break;

            }

        }

        if (que_empty(&self->error_codes)) {

            errno = 0;
            stat = que_init(&self->error_codes);
            check_status(stat);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_load(err_t *self, error_code_t *codes, int size) {

    int x;
    int stat = OK;
    int count = (size/sizeof(codes[0]));

    when_error_in {

        for (x = 0; x < count; x++) {

            stat = self->_add_error(self, codes[x].errnum, codes[x].text, codes[x].message);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _err_load_system_errors(err_t *self) {

    int stat = OK;
    
    /*                                                            */
    /* concept and macro magic taken from:                        */
    /*                                                            */
    /* https://github.com/mentalisttraceur/errnoname/errnoname.c  */
    /*                                                            */
    /* load the system error codes and associated error message.  */
    /*                                                            */

    when_error_in {

#ifdef E2BIG
        stat = self->_add_error(self, E2BIG, "E2BIG", strerror(E2BIG));
        check_return(stat, self);
#endif
#ifdef EACCES
        stat = self->_add_error(self, EACCES, "EACCES", strerror(EACCES));
        check_return(stat, self);
#endif
#ifdef EADDRINUSE
        stat = self->_add_error(self, EADDRINUSE, "EADDRINUSE", strerror(EADDRINUSE));
        check_return(stat, self);
#endif
#ifdef EADDRNOTAVAIL
        stat = self->_add_error(self, EADDRNOTAVAIL, "EADDRNOTAVAIL", strerror(EADDRNOTAVAIL));
        check_return(stat, self);
#endif
#ifdef EADI
        stat = self->_add_error(self, EADI, "EADI", strerror(EADI));
        check_return(stat, self);
#endif
#ifdef EADV
        stat = self->_add_error(self, EADV, "EADV", strerror(EADV));
        check_return(stat, self);
#endif
#ifdef EAFNOSUPPORT
        stat = self->_add_error(self, EAFNOSUPPORT, "EAFNOSUPPORT", strerror(EAFNOSUPPORT));
        check_return(stat, self);
#endif
#ifdef EAGAIN
        stat = self->_add_error(self, EAGAIN, "EAGAIN", strerror(EAGAIN));
        check_return(stat, self);
#endif
#ifdef EAIO
        stat = self->_add_error(self,EAIO , "EAIO", strerror(EAIO));
        check_return(stat, self);
#endif
#ifdef EALIGN
        stat = self->_add_error(self, EALIGN , "EALIGN", strerror(EALIGN));
        check_return(stat, self);
#endif
#ifdef EALREADY
        stat = self->_add_error(self, EALREADY, "EALREADY", strerror(EALREADY));
        check_return(stat, self);
#endif
#ifdef EASYNC
        stat = self->_add_error(self, EASYNC, "EASYNC", strerror(EASYNC));
        check_return(stat, self);
#endif
#ifdef EAUTH
        stat = self->_add_error(self, EAUTH, "EAUTH", strerror());
        check_return(stat, self);
#endif
#ifdef EBADARCH
        stat = self->_add_error(self, EBADARCH, "EBADARCH", strerror(EBADARCH));
        check_return(stat, self);
#endif
#ifdef EBADE
        stat = self->_add_error(self, EBADE, "EBADE", strerror(EBADE));
        check_return(stat, self);
#endif
#ifdef EBADEXEC
        stat = self->_add_error(self, EBADEXEC, "EBADEXEC", strerror(EBADEXEC));
        check_return(stat, self);
#endif
#ifdef EBADF
        stat = self->_add_error(self, EBADF, "EBADF", strerror(EBADF));
        check_return(stat, self);
#endif
#ifdef EBADFD
        stat = self->_add_error(self, EBADFD, "EBADFD", strerror(EBADFD));
        check_return(stat, self);
#endif
#ifdef EBADMACHO
        stat = self->_add_error(self, EBADMACHO, "EBADMACHO", strerror(EBADMACHO));
        check_return(stat, self);
#endif
#ifdef EBADMSG
        stat = self->_add_error(self, EBADMSG, "EBADMSG", strerror(EBADMSG));
        check_return(stat, self);
#endif
#ifdef EBADR
        stat = self->_add_error(self, EBADR, "EBADR", strerror(EBADR));
        check_return(stat, self);
#endif
#ifdef EBADRPC
        stat = self->_add_error(self, EBADRPC, "EBADRPC", strerror(EBADRPC));
        check_return(stat, self);
#endif
#ifdef EBADRQC
        stat = self->_add_error(self, EBADRQC, "EBADRQC", strerror(EBADRQC));
        check_return(stat, self);
#endif
#ifdef EBADSLT
        stat = self->_add_error(self, EBADSLT, "EBADSLT", strerror(EBADSLT));
        check_return(stat, self);
#endif
#ifdef EBADVER
        stat = self->_add_error(self, EBADVER, "EBADVER", strerror(EBADVER));
        check_return(stat, self);
#endif
#ifdef EBFONT
        stat = self->_add_error(self, EBFONT, "EBFONT", strerror(EBFONT));
        check_return(stat, self);
#endif
#ifdef EBUSY
        stat = self->_add_error(self, EBUSY, "EBUSY", strerror(EBUSY));
        check_return(stat, self);
#endif
#ifdef ECANCELED
        stat = self->_add_error(self, ECANCELED, "ECANCELED", strerror(ECANCELED));
        check_return(stat, self);
#endif
#if defined(ECANCELLED) && (!defined(ECANCELED) || ECANCELLED != ECANCELED)
        stat = self->_add_error(self, ECANCELLED, "ECANCELLED", strerror(ECANCELLED));
        check_return(stat, self);
#endif
#ifdef ECAPMODE
        stat = self->_add_error(self, ECAPMODE, "ECAPMODE", strerror(ECAPMODE));
        check_return(stat, self);
#endif
#ifdef ECHILD
        stat = self->_add_error(self, ECHILD, "ECHILD", strerror(ECHILD));
        check_return(stat, self);
#endif
#ifdef ECHRNG
        stat = self->_add_error(self, ECHRNG, "ECHRNG", strerror(ECHRNG));
        check_return(stat, self);
#endif
#ifdef ECKPT
        stat = self->_add_error(self, ECKPT, "ECKPT", strerror(ECKPT));
        check_return(stat, self);
#endif
#ifdef ECLONEME
        stat = self->_add_error(self, ECLONEME, "ECLONEME", strerror(ECLONEME));
        check_return(stat, self);
#endif
#ifdef ECOMM
        stat = self->_add_error(self, ECOMM, "ECOMM", strerror(ECOMM));
        check_return(stat, self);
#endif
#ifdef ECONFIG
        stat = self->_add_error(self, ECONFIG, "ECONFIG", strerror(ECONFIG));
        check_return(stat, self);
#endif
#ifdef ECONNABORTED
        stat = self->_add_error(self, ECONNABORTED, "ECONNABORTED", strerror(ECONNABORTED));
        check_return(stat, self);
#endif
#ifdef ECONNREFUSED
        stat = self->_add_error(self, ECONNREFUSED, "ECONNREFUSED", strerror(ECONNREFUSED));
        check_return(stat, self);
#endif
#ifdef ECONNRESET
        stat = self->_add_error(self, ECONNRESET, "ECONNRESET", strerror(ECONNRESET));
        check_return(stat, self);
#endif
#ifdef ECORRUPT
        stat = self->_add_error(self, ECORRUPT, "ECORRUPT", strerror(ECORRUPT));
        check_return(stat, self);
#endif
#ifdef ECVCERORR
        stat = self->_add_error(self, ECVCERORR, "ECVCERORR", strerror(ECVCERORR));
        check_return(stat, self);
#endif
#ifdef ECVPERORR
        stat = self->_add_error(self, ECVPERORR, "ECVPERORR", strerror(ECVPERORR));
        check_return(stat, self);
#endif
#ifdef EDEADLK
        stat = self->_add_error(self, EDEADLK, "EDEADLK", strerror(EDEADLK));
        check_return(stat, self);
#endif
#if defined(EDEADLOCK) && (!defined(EDEADLK) || EDEADLOCK != EDEADLK)
        stat = self->_add_error(self, EDEADLOCK, "EDEADLOCK", strerror(EDEADLOCK));
        check_return(stat, self);
#endif
#ifdef EDESTADDREQ
        stat = self->_add_error(self, EDESTADDREQ, "EDESTADDREQ", strerror(EDESTADDREQ));
        check_return(stat, self);
#endif
#ifdef EDESTADDRREQ
        stat = self->_add_error(self, EDESTADDRREQ, "EDESTADDRREQ", strerror(EDESTADDRREQ));
        check_return(stat, self);
#endif
#ifdef EDEVERR
        stat = self->_add_error(self, EDEVERR, "EDEVERR", strerror(EDEVERR));
        check_return(stat, self);
#endif
#ifdef EDIRIOCTL
        stat = self->_add_error(self, EDIRIOCTL, "EDIRIOCTL", strerror(EDIRIOCTL));
        check_return(stat, self);
#endif
#ifdef EDIRTY
        stat = self->_add_error(self, EDIRTY, "EDIRTY", strerror(EDIRTY));
        check_return(stat, self);
#endif
#ifdef EDIST
        stat = self->_add_error(self, EDIST, "EDIST", strerror(EDIST));
        check_return(stat, self);
#endif
#ifdef EDOM
        stat = self->_add_error(self, EDOM, "EDOM", strerror(EDOM));
        check_return(stat, self);
#endif
#ifdef EDOOFUS
        stat = self->_add_error(self, EDOOFUS, "EDOOFUS", strerror(EDOOFUS));
        check_return(stat, self);
#endif
#ifdef EDOTDOT
        stat = self->_add_error(self, EDOTDOT, "EDOTDOT", strerror(EDOTDOT));
        check_return(stat, self);
#endif
#ifdef EDQUOT
        stat = self->_add_error(self, EDQUOT, "EDQUOT", strerror(EDQUOT));
        check_return(stat, self);
#endif
#ifdef EDUPFD
        stat = self->_add_error(self, EDUPFD, "EDUPFD", strerror(EDUPFD));
        check_return(stat, self);
#endif
#ifdef EDUPPKG
        stat = self->_add_error(self, EDUPPKG, "EDUPPKG", strerror(EDUPPKG));
        check_return(stat, self);
#endif
#ifdef EEXIST
        stat = self->_add_error(self, EEXIST, "EEXIST", strerror(EEXIST));
        check_return(stat, self);
#endif
#ifdef EFAIL
        stat = self->_add_error(self, EFAIL, "EFAIL", strerror(EFAIL));
        check_return(stat, self);
#endif
#ifdef EFAULT
        stat = self->_add_error(self, EFAULT, "EFAULT", strerror(EFAULT));
        check_return(stat, self);
#endif
#ifdef EFBIG
        stat = self->_add_error(self, EFBIG, "EFBIG", strerror(EFBIG));
        check_return(stat, self);
#endif
#ifdef EFORMAT
        stat = self->_add_error(self, EFORMAT, "EFORMAT", strerror(EFORMAT));
        check_return(stat, self);
#endif
#ifdef EFSCORRUPTED
        stat = self->_add_error(self, EFSCORRUPTED, "EFSCORRUPTED", strerror(EFSCORRUPTED));
        check_return(stat, self);
#endif
#ifdef EFTYPE
        stat = self->_add_error(self, EFTYPE, "EFTYPE", strerror(EFTYPE));
        check_return(stat, self);
#endif
#ifdef EHOSTDOWN
        stat = self->_add_error(self, EHOSTDOWN, "EHOSTDOWN", strerror(EHOSTDOWN));
        check_return(stat, self);
#endif
#ifdef EHOSTUNREACH
        stat = self->_add_error(self, EHOSTUNREACH, "EHOSTUNREACH", strerror(EHOSTUNREACH));
        check_return(stat, self);
#endif
#ifdef EHWPOISON
        stat = self->_add_error(self, EHWPOISON, "EHWPOISON", strerror(EHWPOISON));
        check_return(stat, self);
#endif
#ifdef EIDRM
        stat = self->_add_error(self, EIDRM, "EIDRM", strerror(EIDRM));
        check_return(stat, self);
#endif
#ifdef EILSEQ
        stat = self->_add_error(self, EILSEQ, "EILSEQ", strerror(EILSEQ));
        check_return(stat, self);
#endif
#ifdef EINIT
        stat = self->_add_error(self, EINIT, "EINIT", strerror(EINIT));
        check_return(stat, self);
#endif
#ifdef EINPROG
        stat = self->_add_error(self, EINPROG, "EINPROG", strerror(EINPROG));
        check_return(stat, self);
#endif
#ifdef EINPROGRESS
        stat = self->_add_error(self, EINPROGRESS, "EINPROGRESS", strerror(EINPROGRESS));
        check_return(stat, self);
#endif
#ifdef EINTEGRITY
        stat = self->_add_error(self, EINTEGRITY, "EINTEGRITY", strerror(EINTEGRITY));
        check_return(stat, self);
#endif
#ifdef EINTR
        stat = self->_add_error(self, EINTR, "EINTR", strerror(EINTR));
        check_return(stat, self);
#endif
#ifdef EINVAL
        stat = self->_add_error(self, EINVAL, "EINVAL", strerror(EINVAL));
        check_return(stat, self);
#endif
#ifdef EIO
        stat = self->_add_error(self, EIO, "EIO", strerror(EIO));
        check_return(stat, self);
#endif
#ifdef EIPSEC
        stat = self->_add_error(self, EIPSEC, "EIPSEC", strerror(EIPSEC));
        check_return(stat, self);
#endif
#ifdef EISCONN
        stat = self->_add_error(self, EISCONN, "EISCONN", strerror(EISCONN));
        check_return(stat, self);
#endif
#ifdef EISDIR
        stat = self->_add_error(self, EISDIR, "EISDIR", strerror(EISDIR));
        check_return(stat, self);
#endif
#ifdef EISNAM
        stat = self->_add_error(self, EISNAM, "EISNAM", strerror(EISNAM));
        check_return(stat, self);
#endif
#ifdef EJUSTRETURN
        stat = self->_add_error(self, EJUSTRETURN, "EJUSTRETURN", strerror(EJUSTRETURN));
        check_return(stat, self);
#endif
#ifdef EKEEPLOOKING
        stat = self->_add_error(self, EKEEPLOOKING, "EKEEPLOOKING", strerror(EKEEPLOOKING));
        check_return(stat, self);
#endif
#ifdef EKEYEXPIRED
        stat = self->_add_error(self, EKEYEXPIRED, "EKEYEXPIRED", strerror(EKEYEXPIRED));
        check_return(stat, self);
#endif
#ifdef EKEYREJECTED
        stat = self->_add_error(self, EKEYREJECTED, "EKEYREJECTED", strerror(EKEYREJECTED));
        check_return(stat, self);
#endif
#ifdef EKEYREVOKED
        stat = self->_add_error(self, EKEYREVOKED, "EKEYREVOKED", strerror(EKEYREVOKED));
        check_return(stat, self);
#endif
#ifdef EL2HLT
        stat = self->_add_error(self, EL2HLT, "EL2HLT", strerror(EL2HLT));
        check_return(stat, self);
#endif
#ifdef EL2NSYNC
        stat = self->_add_error(self, EL2NSYNC, "EL2NSYNC", strerror(EL2NSYNC));
        check_return(stat, self);
#endif
#ifdef EL3HLT
        stat = self->_add_error(self, EL3HLT, "EL3HLT", strerror(EL3HLT));
        check_return(stat, self);
#endif
#ifdef EL3RST
        stat = self->_add_error(self, EL3RST, "EL3RST", strerror(EL3RST));
        check_return(stat, self);
#endif
#ifdef ELIBACC
        stat = self->_add_error(self, ELIBACC, "ELIBACC", strerror(ELIBACC));
        check_return(stat, self);
#endif
#ifdef ELIBBAD
        stat = self->_add_error(self, ELIBBAD, "ELIBBAD", strerror(ELIBBAD));
        check_return(stat, self);
#endif
#ifdef ELIBEXEC
        stat = self->_add_error(self, ELIBEXEC, "ELIBEXEC", strerror(ELIBEXEC));
        check_return(stat, self);
#endif
#ifdef ELIBMAX
        stat = self->_add_error(self, ELIBMAX, "ELIBMAX", strerror(ELIBMAX));
        check_return(stat, self);
#endif
#ifdef ELIBSCN
        stat = self->_add_error(self, ELIBSCN, "ELIBSCN", strerror(ELIBSCN));
        check_return(stat, self);
#endif
#ifdef ELNRNG
        stat = self->_add_error(self, ELNRNG, "ELNRNG", strerror(ELNRNG));
        check_return(stat, self);
#endif
#ifdef ELOCKUNMAPPED
        stat = self->_add_error(self, ELOCKUNMAPPED, "ELOCKUNMAPPED", strerror(ELOCKUNMAPPED));
        check_return(stat, self);
#endif
#ifdef ELOOP
        stat = self->_add_error(self, ELOOP, "ELOOP", strerror(ELOOP));
        check_return(stat, self);
#endif
#ifdef EMEDIA
        stat = self->_add_error(self, EMEDIA, "EMEDIA", strerror(EMEDIA));
        check_return(stat, self);
#endif
#ifdef EMEDIUMTYPE
        stat = self->_add_error(self, EMEDIUMTYPE, "EMEDIUMTYPE", strerror(EMEDIUMTYPE));
        check_return(stat, self);
#endif
#ifdef EMFILE
        stat = self->_add_error(self, EMFILE, "EMFILE", strerror(EMFILE));
        check_return(stat, self);
#endif
#ifdef EMLINK
        stat = self->_add_error(self, EMLINK, "EMLINK", strerror(EMLINK));
        check_return(stat, self);
#endif
#ifdef EMOUNTEXIT
        stat = self->_add_error(self, EMOUNTEXIT, "EMOUNTEXIT", strerror(EMOUNTEXIT));
        check_return(stat, self);
#endif
#ifdef EMOVEFD
        stat = self->_add_error(self, EMOVEFD, "EMOVEFD", strerror(EMOVEFD));
        check_return(stat, self);
#endif
#ifdef EMSGSIZE
        stat = self->_add_error(self, EMSGSIZE, "EMSGSIZE", strerror(EMSGSIZE));
        check_return(stat, self);
#endif
#ifdef EMTIMERS
        stat = self->_add_error(self, EMTIMERS, "EMTIMERS", strerror(EMTIMERS));
        check_return(stat, self);
#endif
#ifdef EMULTIHOP
        stat = self->_add_error(self, EMULTIHOP, "EMULTIHOP", strerror(EMULTIHOP));
        check_return(stat, self);
#endif
#ifdef ENAMETOOLONG
        stat = self->_add_error(self, ENAMETOOLONG, "ENAMETOOLONG", strerror(ENAMETOOLONG));
        check_return(stat, self);
#endif
#ifdef ENAVAIL
        stat = self->_add_error(self, ENAVAIL, "ENAVAIL", strerror(ENAVAIL));
        check_return(stat, self);
#endif
#ifdef ENEEDAUTH
        stat = self->_add_error(self, ENEEDAUTH, "ENEEDAUTH", strerror(ENEEDAUTH));
        check_return(stat, self);
#endif
#ifdef ENETDOWN
        stat = self->_add_error(self, ENETDOWN, "ENETDOWN", strerror(ENETDOWN));
        check_return(stat, self);
#endif
#ifdef ENETRESET
        stat = self->_add_error(self, ENETRESET, "ENETRESET", strerror(ENETRESET));
        check_return(stat, self);
#endif
#ifdef ENETUNREACH
        stat = self->_add_error(self, ENETUNREACH, "ENETUNREACH", strerror(ENETUNREACH));
        check_return(stat, self);
#endif
#ifdef ENFILE
        stat = self->_add_error(self, ENFILE, "ENFILE", strerror(ENFILE));
        check_return(stat, self);
#endif
#ifdef ENFSREMOTE
        stat = self->_add_error(self, ENFSREMOTE, "ENFSREMOTE", strerror(ENFSREMOTE));
        check_return(stat, self);
#endif
#ifdef ENOANO
        stat = self->_add_error(self, ENOANO, "ENOANO", strerror(ENOANO));
        check_return(stat, self);
#endif
#ifdef ENOATTR
        stat = self->_add_error(self, ENOATTR, "ENOATTR", strerror(ENOATTR));
        check_return(stat, self);
#endif
#ifdef ENOBUFS
        stat = self->_add_error(self, ENOBUFS, "ENOBUFS", strerror(ENOBUFS));
        check_return(stat, self);
#endif
#ifdef ENOCONNECT
        stat = self->_add_error(self, ENOCONNECT, "ENOCONNECT", strerror(ENOCONNECT));
        check_return(stat, self);
#endif
#ifdef ENOCSI
        stat = self->_add_error(self, ENOCSI, "ENOCSI", strerror(ENOCSI));
        check_return(stat, self);
#endif
#ifdef ENODATA
        stat = self->_add_error(self, ENODATA, "ENODATA", strerror(ENODATA));
        check_return(stat, self);
#endif
#ifdef ENODEV
        stat = self->_add_error(self, ENODEV, "ENODEV", strerror(ENODEV));
        check_return(stat, self);
#endif
#ifdef ENOENT
        stat = self->_add_error(self, ENOENT, "ENOENT", strerror(ENOENT));
        check_return(stat, self);
#endif
#ifdef ENOEXEC
        stat = self->_add_error(self, ENOEXEC, "ENOEXEC", strerror(ENOEXEC));
        check_return(stat, self);
#endif
#ifdef ENOIOCTL
        stat = self->_add_error(self, ENOIOCTL, "ENOIOCTL", strerror(ENOIOCTL));
        check_return(stat, self);
#endif
#ifdef ENOKEY
        stat = self->_add_error(self, ENOKEY, "ENOKEY", strerror(ENOKEY));
        check_return(stat, self);
#endif
#ifdef ENOLCK
        stat = self->_add_error(self, ENOLCK, "ENOLCK", strerror(ENOLCK));
        check_return(stat, self);
#endif
#ifdef ENOLINK
        stat = self->_add_error(self, ENOLINK, "ENOLINK", strerror(ENOLINK));
        check_return(stat, self);
#endif
#ifdef ENOLOAD
        stat = self->_add_error(self, ENOLOAD, "ENOLOAD", strerror(ENOLOAD));
        check_return(stat, self);
#endif
#ifdef ENOMATCH
        stat = self->_add_error(self, ENOMATCH, "ENOMATCH", strerror(ENOMATCH));
        check_return(stat, self);
#endif
#ifdef ENOMEDIUM
        stat = self->_add_error(self, ENOMEDIUM, "ENOMEDIUM", strerror(ENOMEDIUM));
        check_return(stat, self);
#endif
#ifdef ENOMEM
        stat = self->_add_error(self, ENOMEM, "ENOMEM", strerror(ENOMEM));
        check_return(stat, self);
#endif
#ifdef ENOMSG
        stat = self->_add_error(self, ENOMSG, "ENOMSG", strerror(ENOMSG));
        check_return(stat, self);
#endif
#ifdef ENONET
        stat = self->_add_error(self, ENONET, "ENONET", strerror(ENONET));
        check_return(stat, self);
#endif
#ifdef ENOPKG
        stat = self->_add_error(self, ENOPKG, "ENOPKG", strerror(ENOPKG));
        check_return(stat, self);
#endif
#ifdef ENOPOLICY
        stat = self->_add_error(self, ENOPOLICY, "ENOPOLICY", strerror(ENOPOLICY));
        check_return(stat, self);
#endif
#ifdef ENOPROTOOPT
        stat = self->_add_error(self, ENOPROTOOPT, "ENOPROTOOPT", strerror(ENOPROTOOPT));
        check_return(stat, self);
#endif
#ifdef ENOREG
        stat = self->_add_error(self, ENOREG, "ENOREG", strerror(ENOREG));
        check_return(stat, self);
#endif
#ifdef ENOSPC
        stat = self->_add_error(self, ENOSPC, "ENOSPC", strerror(ENOSPC));
        check_return(stat, self);
#endif
#ifdef ENOSR
        stat = self->_add_error(self, ENOSR, "ENOSR", strerror(ENOSR));
        check_return(stat, self);
#endif
#ifdef ENOSTR
        stat = self->_add_error(self, ENOSTR, "ENOSTR", strerror(ENOSTR));
        check_return(stat, self);
#endif
#ifdef ENOSYM
        stat = self->_add_error(self, ENOSYM, "ENOSYM", strerror(ENOSYM));
        check_return(stat, self);
#endif
#ifdef ENOSYS
        stat = self->_add_error(self, ENOSYS, "ENOSYS", strerror(ENOSYS));
        check_return(stat, self);
#endif
#ifdef ENOTACTIVE
        stat = self->_add_error(self, ENOTACTIVE, "ENOTACTIVE", strerror(ENOTACTIVE));
        check_return(stat, self);
#endif
#ifdef ENOTBLK
        stat = self->_add_error(self, ENOTBLK, "ENOTBLK", strerror(ENOTBLK));
        check_return(stat, self);
#endif
#ifdef ENOTCAPABLE
        stat = self->_add_error(self, ENOTCAPABLE, "ENOTCAPABLE", strerror(ENOTCAPABLE));
        check_return(stat, self);
#endif
#ifdef ENOTCONN
        stat = self->_add_error(self, ENOTCONN, "ENOTCONN", strerror(ENOTCONN));
        check_return(stat, self);
#endif
#ifdef ENOTDIR
        stat = self->_add_error(self, ENOTDIR, "ENOTDIR", strerror(ENOTDIR));
        check_return(stat, self);
#endif
#ifdef ENOTEMPTY
        stat = self->_add_error(self, ENOTEMPTY, "ENOTEMPTY", strerror(ENOTEMPTY));
        check_return(stat, self);
#endif
#ifdef ENOTNAM
        stat = self->_add_error(self, ENOTNAM, "ENOTNAM", strerror(ENOTNAM));
        check_return(stat, self);
#endif
#ifdef ENOTREADY
        stat = self->_add_error(self, ENOTREADY, "ENOTREADY", strerror(ENOTREADY));
        check_return(stat, self);
#endif
#ifdef ENOTRECOVERABLE
        stat = self->_add_error(self, ENOTRECOVERABLE, "ENOTRECOVERABLE", strerror(ENOTRECOVERABLE));
        check_return(stat, self);
#endif
#ifdef ENOTRUST
        stat = self->_add_error(self, ENOTRUST, "ENOTRUST", strerror(ENOTRUST));
        check_return(stat, self);
#endif
#ifdef ENOTSOCK
        stat = self->_add_error(self, ENOTSOCK, "ENOTSOCK", strerror(ENOTSOCK));
        check_return(stat, self);
#endif
#ifdef ENOTSUP
        stat = self->_add_error(self, ENOTSUP, "ENOTSUP", strerror(ENOTSUP));
        check_return(stat, self);
#endif
#ifdef ENOTTY
        stat = self->_add_error(self, ENOTTY, "ENOTTY", strerror(ENOTTY));
        check_return(stat, self);
#endif
#ifdef ENOTUNIQ
        stat = self->_add_error(self, ENOTUNIQ, "ENOTUNIQ", strerror(ENOTUNIQ));
        check_return(stat, self);
#endif
#ifdef ENOUNLD
        stat = self->_add_error(self, ENOUNLD, "ENOUNLD", strerror(ENOUNLD));
        check_return(stat, self);
#endif
#ifdef ENOUNREG
        stat = self->_add_error(self, ENOUNREG, "ENOUNREG", strerror(ENOUNREG));
        check_return(stat, self);
#endif
#ifdef ENXIO
        stat = self->_add_error(self, ENXIO, "ENXIO", strerror(ENXIO));
        check_return(stat, self);
#endif
#ifdef EOPCOMPLETE
        stat = self->_add_error(self, EOPCOMPLETE, "EOPCOMPLETE", strerror(EOPCOMPLETE));
        check_return(stat, self);
#endif
#if defined(EOPNOTSUPP) && (!defined(ENOTSUP) || EOPNOTSUPP != ENOTSUP)
        stat = self->_add_error(self, EOPNOTSUPP, "EOPNOTSUPP", strerror(EOPNOTSUPP));
        check_return(stat, self);
#endif
#ifdef EOVERFLOW
        stat = self->_add_error(self, EOVERFLOW, "EOVERFLOW", strerror(EOVERFLOW));
        check_return(stat, self);
#endif
#ifdef EOWNERDEAD
        stat = self->_add_error(self, EOWNERDEAD, "EOWNERDEAD", strerror(EOWNERDEAD));
        check_return(stat, self);
#endif
#ifdef EPASSTHROUGH
        stat = self->_add_error(self, EPASSTHROUGH, "EPASSTHROUGH", strerror(EPASSTHROUGH));
        check_return(stat, self);
#endif
#ifdef EPATHREMOTE
        stat = self->_add_error(self, EPATHREMOTE, "EPATHREMOTE", strerror(EPATHREMOTE));
        check_return(stat, self);
#endif
#ifdef EPERM
        stat = self->_add_error(self, EPERM, "EPERM", strerror(EPERM));
        check_return(stat, self);
#endif
#ifdef EPFNOSUPPORT
        stat = self->_add_error(self, EPFNOSUPPORT, "EPFNOSUPPORT", strerror(EPFNOSUPPORT));
        check_return(stat, self);
#endif
#ifdef EPIPE
        stat = self->_add_error(self, EPIPE, "EPIPE", strerror(EPIPE));
        check_return(stat, self);
#endif
#ifdef EPOWERF
        stat = self->_add_error(self, EPOWERF, "EPOWERF", strerror(EPOWERF));
        check_return(stat, self);
#endif
#ifdef EPROCLIM
        stat = self->_add_error(self, EPROCLIM, "EPROCLIM", strerror(EPROCLIM));
        check_return(stat, self);
#endif
#ifdef EPROCUNAVAIL
        stat = self->_add_error(self, EPROCUNAVAIL, "EPROCUNAVAIL", strerror(EPROCUNAVAIL));
        check_return(stat, self);
#endif
#ifdef EPROGMISMATCH
        stat = self->_add_error(self, EPROGMISMATCH, "EPROGMISMATCH", strerror(EPROGMISMATCH));
        check_return(stat, self);
#endif
#ifdef EPROGUNAVAIL
        stat = self->_add_error(self, EPROGUNAVAIL, "EPROGUNAVAIL", strerror(EPROGUNAVAIL));
        check_return(stat, self);
#endif
#ifdef EPROTO
        stat = self->_add_error(self, EPROTO, "EPROTO", strerror(EPROTO));
        check_return(stat, self);
#endif
#ifdef EPROTONOSUPPORT
        stat = self->_add_error(self, EPROTONOSUPPORT, "EPROTONOSUPPORT", strerror(EPROTONOSUPPORT));
        check_return(stat, self);
#endif
#ifdef EPROTOTYPE
        stat = self->_add_error(self, EPROTOTYPE, "EPROTOTYPE", strerror(EPROTOTYPE));
        check_return(stat, self);
#endif
#ifdef EPWROFF
        stat = self->_add_error(self, EPWROFF, "EPWROFF", strerror(EPWROFF));
        check_return(stat, self);
#endif
#ifdef EQFULL
        stat = self->_add_error(self, EQFULL, "EQFULL", strerror(EQFULL));
        check_return(stat, self);
#endif
#ifdef EQSUSPENDED
        stat = self->_add_error(self, EQSUSPENDED, "EQSUSPENDED", strerror(EQSUSPENDED));
        check_return(stat, self);
#endif
#ifdef ERANGE
        stat = self->_add_error(self, ERANGE, "ERANGE", strerror(ERANGE));
        check_return(stat, self);
#endif
#ifdef ERECYCLE
        stat = self->_add_error(self, ERECYCLE, "ERECYCLE", strerror(ERECYCLE));
        check_return(stat, self);
#endif
#ifdef EREDRIVEOPEN
        stat = self->_add_error(self, EREDRIVEOPEN, "EREDRIVEOPEN", strerror(EREDRIVEOPEN));
        check_return(stat, self);
#endif
#ifdef EREFUSED
        stat = self->_add_error(self, EREFUSED, "EREFUSED", strerror(EREFUSED));
        check_return(stat, self);
#endif
#ifdef ERELOC
        stat = self->_add_error(self, ERELOC, "ERELOC", strerror(ERELOC));
        check_return(stat, self);
#endif
#ifdef ERELOCATED
        stat = self->_add_error(self, ERELOCATED, "ERELOCATED", strerror(ERELOCATED));
        check_return(stat, self);
#endif
#ifdef ERELOOKUP
        stat = self->_add_error(self, ERELOOKUP, "ERELOOKUP", strerror(ERELOOKUP));
        check_return(stat, self);
#endif
#ifdef EREMCHG
        stat = self->_add_error(self, EREMCHG, "EREMCHG", strerror(EREMCHG));
        check_return(stat, self);
#endif
#ifdef EREMDEV
        stat = self->_add_error(self, EREMDEV, "EREMDEV", strerror(EREMDEV));
        check_return(stat, self);
#endif
#ifdef EREMOTE
        stat = self->_add_error(self, EREMOTE, "EREMOTE", strerror(EREMOTE));
        check_return(stat, self);
#endif
#ifdef EREMOTEIO
        stat = self->_add_error(self, EREMOTEIO, "EREMOTEIO", strerror(EREMOTEIO));
        check_return(stat, self);
#endif
#ifdef EREMOTERELEASE
        stat = self->_add_error(self, EREMOTERELEASE, "EREMOTERELEASE", strerror(EREMOTERELEASE));
        check_return(stat, self);
#endif
#ifdef ERESTART
        stat = self->_add_error(self, ERESTART, "ERESTART", strerror(ERESTART));
        check_return(stat, self);
#endif
#ifdef ERFKILL
        stat = self->_add_error(self, ERFKILL, "ERFKILL", strerror(ERFKILL));
        check_return(stat, self);
#endif
#ifdef EROFS
        stat = self->_add_error(self, EROFS, "EROFS", strerror(EROFS));
        check_return(stat, self);
#endif
#ifdef ERPCMISMATCH
        stat = self->_add_error(self, ERPCMISMATCH, "ERPCMISMATCH", strerror(ERPCMISMATCH));
        check_return(stat, self);
#endif
#ifdef ESAD
        stat = self->_add_error(self, ESAD, "ESAD", strerror(ESAD));
        check_return(stat, self);
#endif
#ifdef ESHLIBVERS
        stat = self->_add_error(self, ESHLIBVERS, "ESHLIBVERS", strerror(ESHLIBVERS));
        check_return(stat, self);
#endif
#ifdef ESHUTDOWN
        stat = self->_add_error(self, ESHUTDOWN, "ESHUTDOWN", strerror(ESHUTDOWN));
        check_return(stat, self);
#endif
#ifdef ESOCKTNOSUPPORT
        stat = self->_add_error(self, ESOCKTNOSUPPORT, "ESOCKTNOSUPPORT", strerror(ESOCKTNOSUPPORT));
        check_return(stat, self);
#endif
#ifdef ESOFT
        stat = self->_add_error(self, ESOFT, "ESOFT", strerror(ESOFT));
        check_return(stat, self);
#endif
#ifdef ESPIPE
        stat = self->_add_error(self, ESPIPE, "ESPIPE", strerror(ESPIPE));
        check_return(stat, self);
#endif
#ifdef ESRCH
        stat = self->_add_error(self, ESRCH, "ESRCH", strerror(ESRCH));
        check_return(stat, self);
#endif
#ifdef ESRMNT
        stat = self->_add_error(self, ESRMNT, "ESRMNT", strerror(ESRMNT));
        check_return(stat, self);
#endif
#ifdef ESTALE
        stat = self->_add_error(self, ESTALE, "ESTALE", strerror(ESTALE));
        check_return(stat, self);
#endif
#ifdef ESTART
        stat = self->_add_error(self, ESTART, "ESTART", strerror(ESTART));
        check_return(stat, self);
#endif
#ifdef ESTRPIPE
        stat = self->_add_error(self, ESTRPIPE, "ESTRPIPE", strerror(ESTRPIPE));
        check_return(stat, self);
#endif
#ifdef ESYSERROR
        stat = self->_add_error(self, ESYSERROR, "ESYSERROR", strerror(ESYSERROR));
        check_return(stat, self);
#endif
#ifdef ETIME
        stat = self->_add_error(self, ETIME, "ETIME", strerror(ETIME));
        check_return(stat, self);
#endif
#ifdef ETIMEDOUT
        stat = self->_add_error(self, ETIMEDOUT, "ETIMEDOUT", strerror(ETIMEDOUT));
        check_return(stat, self);
#endif
#ifdef ETOOMANYREFS
        stat = self->_add_error(self, ETOOMANYREFS, "ETOOMANYREFS", strerror(ETOOMANYREFS));
        check_return(stat, self);
#endif
#ifdef ETXTBSY
        stat = self->_add_error(self, ETXTBSY, "ETXTBSY", strerror(ETXTBSY));
        check_return(stat, self);
#endif
#ifdef EUCLEAN
        stat = self->_add_error(self, EUCLEAN, "EUCLEAN", strerror(EUCLEAN));
        check_return(stat, self);
#endif
#ifdef EUNATCH
        stat = self->_add_error(self, EUNATCH, "EUNATCH", strerror(EUNATCH));
        check_return(stat, self);
#endif
#ifdef EUSERS
        stat = self->_add_error(self, EUSERS, "EUSERS", strerror(EUSERS));
        check_return(stat, self);
#endif
#ifdef EVERSION
        stat = self->_add_error(self, EVERSION, "EVERSION", strerror(EVERSION));
        check_return(stat, self);
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || EWOULDBLOCK != EAGAIN)
        stat = self->_add_error(self, EWOULDBLOCK, "EWOULDBLOCK", strerror(EWOULDBLOCK));
        check_return(stat, self);
#endif
#ifdef EWRONGFS
        stat = self->_add_error(self, EWRONGFS, "EWRONGFS", strerror(EWRONGFS));
        check_return(stat, self);
#endif
#ifdef EWRPROTECT
        stat = self->_add_error(self, EWRPROTECT, "EWRPROTECT", strerror(EWRPROTECT));
        check_return(stat, self);
#endif
#ifdef EXDEV
        stat = self->_add_error(self, EXDEV, "EXDEV", strerror(EXDEV));
        check_return(stat, self);
#endif
#ifdef EXFULL
        stat = self->_add_error(self, EXFULL, "EXFULL", strerror(EXFULL));
        check_return(stat, self);
#endif

        exit_when;

	} use {

        stat = ERR;
        process_error(self);

    } end_when;
    
    /* this is ridicules, there has to be a better way. oh,       */
    /* there is, but this has been marked as "depreciated".       */
    /*                                                            */
    /* for (x = 1; x <= sys_nerr; x++) {                          */
    /*                                                            */
    /*     text = strdup(errnoname(x));                           */
    /*     message = strdup(strerror(x));                         */
    /*                                                            */
    /*     self->_add_error(self, x, text, message);              */
    /*                                                            */
    /*     free(test);                                            */
    /*     free(message);                                         */
    /*                                                            */
    /* }                                                          */
    /*                                                            */
    /* thank you standards bodies                                 */
    /*                                                            */

    return stat;

}

