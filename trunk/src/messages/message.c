
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

#include "xas/message.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "messages/message_priv.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _message_ctor(object_t *, item_list_t *);
int _message_dtor(object_t *);

int _message_compare(message_t *, message_t *);
int _message_override(message_t *, item_list_t *);
int _message_del(message_t *, int);
int _message_add(message_t *, int, char *);
int _message_set(message_t *, int, char *);
int _message_get(message_t *, int, char *, int);
int _message_load(message_t *, message_code_t *, int);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(MESSAGE_KLASS) {
    .size = KLASS_SIZE(message_t),
    .name = KLASS_NAME(message_t),
    .ctor = _message_ctor,
    .dtor = _message_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

message_t *message_create(message_code_t *messages, int msg_size) {

    int stat = ERR;
    item_list_t items[2];
    message_t *self = NULL;

    if (messages != NULL) {

        SET_ITEM(items[0], MESSAGE_K_MESSAGES, messages, msg_size, NULL);
        SET_ITEM(items[1], 0, 0, 0, 0);

    } else {

        SET_ITEM(items[0], 0, 0, 0, 0);
        SET_ITEM(items[1], 0, 0, 0, 0);

    }

    self = (message_t *)object_create(MESSAGE_KLASS, items, &stat);

    return self;

}

int message_destroy(message_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, message_t)) {

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

int message_override(message_t *self, item_list_t *items) {

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

int message_compare(message_t *us, message_t *them) {

    int stat = OK;

    when_error_in {

        if ((us != NULL) && (them != NULL)) {

            if (object_assert(them, message_t)) {

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

char *message_version(message_t *self) {

    char *version = VERSION;

    return version;

}

int message_add(message_t *self, int nemonic, char *message) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (nemonic != 0) && (message != NULL)) {

            stat = self->_add_message(self, nemonic, message);
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

int message_set(message_t *self, int nemonic, char *text) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (nemonic != 0) && (text != NULL)) {

            stat = self->_set_message(self, nemonic, text);
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

int message_get(message_t *self, int nemonic, char *buffer, int size) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (nemonic != 0) && (buffer != NULL)) {

            stat = self->_get_message(self, nemonic, buffer, size);
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

int message_remove(message_t *self, int nemonic) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (nemonic != 0)) {

            stat = self->_del_message(self, nemonic);
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

int message_load(message_t *self, message_code_t *messages, int size) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (messages != NULL)) {

            stat = self->_load_messages(self, messages, size);
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

int _message_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    int msg_size = 0;
    message_t *self = NULL;
    message_code_t *messages = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case MESSAGE_K_MESSAGES: {
                        messages = items[x].buffer_address;
                        msg_size = items[x].buffer_length;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = MESSAGE(object);

        /* assign our methods here */

        self->ctor = _message_ctor;
        self->dtor = _message_dtor;
        self->_compare = _message_compare;
        self->_override = _message_override;
        self->_add_message = _message_add;
        self->_del_message = _message_del;
        self->_get_message = _message_get;
        self->_set_message = _message_set;
        self->_load_messages = _message_load;

        /* initialize internal variables here */

        when_error_in {

            errno = 0;
            stat = que_init(&self->messages);
            check_status(stat);

            /* load default messages */

            stat = self->_load_messages(self, defaults, sizeof(defaults));
            check_return(stat, self);

            /* load any user defined messages */

            if (messages != NULL) {

                stat = self->_load_messages(self, messages, msg_size);
                check_return(stat, self);

            }

            exit_when;

        } use {
            
            stat = ERR;
            process_error(self);

        } end_when;
        
    }

    return stat;

}

int _message_dtor(object_t *object) {

    int stat = OK;
    message_code_t *message = NULL;
    message_t *self = MESSAGE(object);

    /* free local resources here */

    when_error_in {

        while ((message = que_pop_head(&self->messages))) {

            free(message->text);
            free(message);

        }

        errno = 0;
        stat = que_init(&self->messages);
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

int _message_override(message_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {

        if (items != NULL) {

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case MESSAGE_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case MESSAGE_M_ADD_MESSAGE: {
                        self->_add_message = NULL;
                        self->_add_message = items[x].buffer_address;
                        check_null(self->_add_message);
                        break;
                    }
                    case MESSAGE_M_DEL_MESSAGE: {
                        self->_del_message = NULL;
                        self->_del_message = items[x].buffer_address;
                        check_null(self->_del_message);
                        break;
                    }
                    case MESSAGE_M_GET_MESSAGE: {
                        self->_get_message = NULL;
                        self->_get_message = items[x].buffer_address;
                        check_null(self->_get_message);
                        break;
                    }
                    case MESSAGE_M_SET_MESSAGE: {
                        self->_set_message = NULL;
                        self->_set_message = items[x].buffer_address;
                        check_null(self->_set_message);
                        break;
                    }
                    case MESSAGE_M_LOAD_MESSAGES: {
                        self->_load_messages = NULL;
                        self->_load_messages = items[x].buffer_address;
                        check_null(self->_load_messages);
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

int _message_compare(message_t *self, message_t *other) {

    int stat = ERR;

    when_error_in {

        if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
            (self->ctor == other->ctor) &&
            (self->dtor == other->dtor) &&
            (self->_compare == other->_compare) &&
            (self->_override == other->_override) &&
            (self->_get_message == other->_get_message) &&
            (self->_set_message == other->_set_message) &&
            (self->_add_message == other->_add_message) &&
            (self->_del_message == other->_del_message) &&
            (self->_load_messages == other->_load_messages)) {

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

int _message_add(message_t *self, int nemonic, char *text) {

    int stat = OK;
    message_code_t *message = NULL;

    when_error_in {

        errno = 0;
        message = calloc(1, sizeof(message_code_t));
        check_null(message);

        message->msgnum = nemonic;
        message->text = strdup(text);

        errno = 0;
        stat = que_push_tail(&self->messages, message);
        check_status(stat);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _message_del(message_t *self, int nemonic) {

    int stat = OK;
    int found = FALSE;
    message_code_t *message = NULL;

    when_error_in {

        for (message = que_first(&self->messages);
             message != NULL;
             message = que_next(&self->messages)) {

            if (message->msgnum == nemonic) {

                errno = 0;
                message = que_delete(&self->messages);
                check_null(message);

                free(message->text);
                free(message);
                found = TRUE;
                break;

            }

        }

        if (que_empty(&self->messages)) {

            errno = 0;
            stat = que_init(&self->messages);
            check_status(stat);

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

int _message_get(message_t *self, int nemonic, char *buffer, int size) {

    int stat = OK;
    int found = FALSE;
    message_code_t *message = NULL;

    when_error_in {

        for (message = que_first(&self->messages);
             message != NULL;
             message = que_next(&self->messages)) {

            if (message->msgnum == nemonic) {

                strncpy(buffer, message->text, size);
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

int _message_set(message_t *self, int nemonic, char *text) {

    int stat = OK;
    message_code_t *message = NULL;

    when_error_in {

        for (message = que_first(&self->messages);
             message != NULL;
             message = que_next(&self->messages)) {

            if (message->msgnum == nemonic) {

                free(message->text);
                message->text = strdup(text);

                errno = 0;
                stat = que_put(&self->messages, message);
                check_status(stat);
                break;

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _message_load(message_t *self, message_code_t *messages, int size) {

    int x;
    int stat = OK;
    int count = (size/sizeof(messages[0]));

    when_error_in {

        for (x = 0; x < count; x++) {

            stat = self->_add_message(self, messages[x].msgnum, messages[x].text);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

