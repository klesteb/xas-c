
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2024 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include "xas/net/net.h"
#include "xas/errors_xas.h"
#include "xas/error_handler.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _net_ctor(object_t *, item_list_t *);
int _net_dtor(object_t *);

int _net_compare(net_t *, net_t *);
int _net_override(net_t *, item_list_t *);

int _net_answer(net_t *, double);
int _net_call(net_t *, int, int);
int _net_complete(net_t *, double, int);
int _net_fd(net_t *, int *);
int _net_isreadable(net_t *, int *);
int _net_isup(net_t *, int *);
int _net_iswriteable(net_t *, int *);
int _net_listen(net_t *, char *, int);
int _net_name(net_t *, char *);
int _net_read(net_t, double, int, void *, int *);
int _net_request_pending(net_t *, int *);
int _net_setbuf(net_t *, int, int);
int _net_write(net_t *, double, int, void *, int *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(NET_KLASS) {
    .size = KLASS_SIZE(net_t),
    .name = KLASS_NAME(net_t),
    .ctor = _net_ctor,
    .dtor = _net_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

net_t *net_create(item_list_t *items) {

    int stat = ERR;
    net_t *self = NULL;

    self = (net_t *)object_create(NET_KLASS, items, &stat);

    return self;

}

int net_destroy(net_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, net_t)) {

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

int net_override(net_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
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

int net_compare(net_t *us, net_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, net_t)) {

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

char *net_version(net_t *self) {

    char *version = PACKAGE_VERSION;

    return version;

}

int net_answer(net_t *self, double timeout) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (timeout > 0)) {

            stat = self->_answer(self, timeout);
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

int _net_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    net_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case NET_K_TYPE: { */
                /*         memcpy(&type,  */
                /*                items[x].buffer_address,  */
                /*                items[x].buffer_length); */
                /*         break; */
                /*     } */
                /* } */

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = NET(object);

        /* assign our methods here */

        self->ctor = _net_ctor;
        self->dtor = _net_dtor;
        self->_compare = _net_compare;
        self->_override = _net_override;

        self->_answer = _net_answer;
        self->_call = _net_call;
        self->_complete = _net_complete;
        self->_fd = _net_fd;
        self->_isreadable = _net_isreadable;
        self->_isup = _net_isup;
        self->_iswriteable = _net_iswriteable;
        self->_listen = _net_listen;
        self->_name = _net_name;
        self->_read = _net_read;
        self->_request_pending = _net_request_pending;
        self->_setbuf = _net_setbuf;
        self->_write = _net_write;

        /* initialize internal variables here */
        
        /* when_error_in { */

        /*     exit_when; */

        /* } use { */

        /*     stat = ERR; */
        /*     process_error(self); */

        /* } end_when; */

    }

    return stat;

}

int _net_dtor(object_t *object) {

    int stat = OK;

    /* errno = E_INVOPS; */

    /* when_error_in { */

        /* free local resources here */

        /* walk the chain, freeing as we go */

        object_demote(object, object_t);
        object_destroy(object);

    /*     exit_when; */

    /* } use { */

    /*     stat = ERR; */
    /*     process_error(self); */

    /* } end_when; */

    return stat;

}

int _net_override(net_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (items != NULL) {

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case NET_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case NET_M_ANSWER: {
                        self->_answer = NULL;
                        self->_answer = items[x].buffer_address;
                        check_null(self->_answer);
                        break;
                    }
                    case NET_M_CALL: {
                        self->_call = NULL;
                        self->_call = items[x].buffer_address;
                        check_null(self->_call);
                        break;
                    }
                    case NET_M_COMPLETE: {
                        self->_complete = NULL;
                        self->_complete = items[x].buffer_address;
                        check_null(self->_complete);
                        break;
                    }
                    case NET_M_FD: {
                        self->_fd = NULL;
                        self->_fd = items[x].buffer_address;
                        check_null(self->_fd);
                        break;
                    }
                    case NET_M_ISREADABLE: {
                        self->_isreadable = NULL;
                        self->_isreadable = items[x].buffer_address;
                        check_null(self->_isreadable);
                        break;
                    }
                    case NET_M_ISUP: {
                        self->_isup = NULL;
                        self->_isup = items[x].buffer_address;
                        check_null(self->_isup);
                        break;
                    }
                    case NET_M_ISWRITEABLE: {
                        self->_iswriteable = NULL;
                        self->_iswriteable = items[x].buffer_address;
                        check_null(self->_writeable);
                        break;
                    }
                    case NET_M_LISTEN: {
                        self->_listen = NULL;
                        self->_listen = items[x].buffer_address;
                        check_null(self->_listen);
                        break;
                    }
                    case NET_M_NAME: {
                        self->_name = NULL;
                        self->_name = items[x].buffer_address;
                        check_null(self->_name);
                        break;
                    }
                    case NET_M_READ: {
                        self->_read = NULL;
                        self->_read = items[x].buffer_address;
                        check_null(self->_read);
                        break;
                    }
                    case NET_M_REQUEST_PENDING: {
                        self->_request_pending = NULL;
                        self->_request_pending = items[x].buffer_address;
                        check_null(self->_request_pending);
                        break;
                    }
                    case NET_M_SETBUF: {
                        self->_setbuf = NULL;
                        self->_setbuf = items[x].buffer_address;
                        check_null(self->_setbuf);
                        break;
                    }
                    case NET_M_WRITE: {
                        self->_write = NULL;
                        self->_write = items[x].buffer_address;
                        check_null(self->_write);
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

int _net_compare(net_t *self, net_t *other) {

    int stat = ERR;

    when_error_in {

        if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override) &&
            (self->_answer == other->_answer) &&
            (self->_call == other->_call) &&
            (self->_complete == other->_complete) &&
            (self->_fd == other->_fd) &&
            (self->_isreadable == other->_isreadable) &&
            (self->_isup == other->_isup) &&
            (self->_iswriteable == other->_iswriteable) &&
            (self->_listen == other->_listen) &&
            (self->_name == other->_name) &&
            (self->_read == other->_read) &&
            (self->_request_pending == other->_request_pending) &&
            (self->_setbuf == other->_setbuf) &&
            (self->_write == other->_write)) {

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

int _net_answer(net_t *self, double timeout) {

    int stat = OK;

    when_error_in {


        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

