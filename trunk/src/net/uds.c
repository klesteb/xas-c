
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

#include "xas/net/uds.h"
#include "xas/errors_xas.h"
#include "xas/error_handler.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _uds_ctor(object_t *, item_list_t *);
int _uds_dtor(object_t *);

int _uds_compare(uds_t *, uds_t *);
int _uds_override(uds_t *, item_list_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(UDS_KLASS) {
    .size = KLASS_SIZE(uds_t),
    .name = KLASS_NAME(uds_t),
    .ctor = _uds_ctor,
    .dtor = _uds_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

uds_t *uds_create(item_list_t *items) {

    int stat = ERR;
    uds_t *self = NULL;

    self = (uds_t *)object_create(UDS_KLASS, items, &stat);

    return self;

}

int uds_destroy(uds_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, uds_t)) {

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

int uds_override(uds_t *self, item_list_t *items) {

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

int uds_compare(uds_t *us, uds_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, uds_t)) {

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

char *uds_version(uds_t *self) {

    char *version = PACKAGE_VERSION;

    return version;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _uds_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    uds_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case UDS_K_TYPE: { */
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

        self = UDS(object);

        /* assign our methods here */

        self->ctor = _uds_ctor;
        self->dtor = _uds_dtor;
        self->_compare = _uds_compare;
        self->_override = _uds_override;

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

int _uds_dtor(object_t *object) {

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

int _uds_override(uds_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (items != NULL) {

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case UDS_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
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

int _uds_compare(uds_t *self, uds_t *other) {

    int stat = ERR;

    when_error_in {

        if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override)) {

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

