
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

#include "xas/rms/fib.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _fib_ctor(object_t *, item_list_t *);
int _fib_dtor(object_t *);

int _fib_compare(fib_t *, fib_t *);
int _fib_override(fib_t *, item_list_t *);

int _fib_open(fib_t *, int, mode_t);
int _fib_close(fib_t *);
int _fib_creat(fib_t *, mode_t);
int _fib_chmod(fib_t *, mode_t);
int _fib_exists(fib_t *, int *);
int _fib_stat(fib_t *, struct stat *);
int _fib_unlink(fib_t *);
int _fib_size(fib_t *, off_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(FIB_KLASS) {
    .size = KLASS_SIZE(fib_t),
    .name = KLASS_NAME(fib_t),
    .ctor = _fib_ctor,
    .dtor = _fib_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

fib_t *fib_create(char *filename) {

    int stat = ERR;
    fib_t *self = NULL;
    item_list_t items[2];

    SET_ITEM(items[0], FIB_K_PATH, filename, strlen(filename), NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    self = (fib_t *)object_create(FIB_KLASS, items, &stat);

    return self;

}

int fib_destroy(fib_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, fib_t)) {

                self->dtor(OBJECT(self));

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

int fib_override(fib_t *self, item_list_t *items) {

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

int fib_compare(fib_t *us, fib_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, fib_t)) {

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

char *fib_version(fib_t *self) {

    char *version = VERSION;

    return version;

}

int fib_open(fib_t *self, int flags, mode_t mode) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_open(self, flags, mode);
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

int fib_creat(fib_t *self, mode_t mode) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_creat(self, mode);
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

int fib_chmod(fib_t *self, mode_t mode) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_chmod(self, mode);
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

int fib_close(fib_t *self) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_close(self);
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

int fib_exists(fib_t *self, int *exists) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_exists(self, exists);
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

int fib_size(fib_t *self, off_t *size) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            stat = self->_size(self, size);
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

int fib_stat(fib_t *self, struct stat *buf) {

    int xstat = OK;

    when_error_in {

        if ((self != NULL)) {

            xstat = self->_stat(self, buf);
            check_return(xstat, self);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        xstat = ERR;
        process_error(self);

    } end_when;

    return xstat;

}

int fib_unlink(fib_t *self) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_unlink(self);
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

int fib_get_fd(fib_t *self, int *fd) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            *fd = self->fd;

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

int _fib_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    char path[1024];
    fib_t *self = NULL;

    if (object != NULL) {

        memset(path, '\0', 1024);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case FIB_K_PATH: {
                        strncpy(path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = FIB(object);

        /* assign our methods here */

        self->ctor = _fib_ctor;
        self->dtor = _fib_dtor;
        self->_compare = _fib_compare;
        self->_override = _fib_override;

        /* methods */
        
        self->_open   = _fib_open;
        self->_close  = _fib_close;
        self->_creat  = _fib_creat;
        self->_chmod  = _fib_chmod;
        self->_exists = _fib_exists;
        self->_size   = _fib_size;
        self->_stat   = _fib_stat;
        self->_unlink = _fib_unlink;

        /* mutators */
        

        /* initialize internal variables here */

        strncpy(self->path, path, 1023);

        stat = OK;

    }

    return stat;

}

int _fib_dtor(object_t *object) {

    int stat = OK;

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _fib_override(fib_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case FIB_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_EXISTS: {
                    self->_exists = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_STAT: {
                    self->_stat = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_SIZE: {
                    self->_size = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_UNLINK: {
                    self->_unlink = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_CREAT: {
                    self->_creat = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_CHMOD: {
                    self->_chmod = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _fib_compare(fib_t *self, fib_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_open == other->_open) &&
        (self->_close == other->_close) &&
        (self->_exists == other->_exists) &&
        (self->_stat == other->_stat) &&
        (self->_size == other->_size) &&
        (self->_creat == other->_creat) &&
        (self->_chmod == other->_chmod) &&
        (self->_unlink == other->_unlink)) {

        stat = OK;

    }

    return stat;

}

int _fib_open(fib_t *self, int flags, mode_t mode) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if ((self->fd = open(self->path, flags, mode)) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _fib_creat(fib_t *self, mode_t mode) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if ((self->fd = creat(self->path, mode)) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _fib_chmod(fib_t *self, mode_t mode) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if (chmod(self->path, mode) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _fib_close(fib_t *self) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if (close(self->fd) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _fib_exists(fib_t *self, int *exists) {

    int xstat = OK;
    struct stat buf;

    when_error_in {

        *exists = TRUE;

        errno = 0;
        if (stat(self->path, &buf) == -1) {

            *exists = FALSE;

           if ((errno != EACCES) && (errno != ENOENT)) {

              cause_error(errno);

           }

        }

        exit_when;

    } use {

        xstat = ERR;
        process_error(self);

    } end_when;

    return xstat;

}

int _fib_stat(fib_t *self, struct stat *buf) {

    int xstat = OK;

    when_error_in {

        errno = 0;
        if (stat(self->path, buf) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        xstat = ERR;
        process_error(self);

    } end_when;

    return xstat;

}

int _fib_unlink(fib_t *self) {
    
    int stat = OK;

    when_error_in {

        errno = 0;
        if (unlink(self->path) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _fib_size(fib_t *self, off_t *size) {

    int xstat = OK;
    struct stat buf;

    when_error_in {

        errno = 0;
        if (stat(self->path, &buf) == -1) {

            cause_error(errno);

        }

        *size = buf.st_size;

        exit_when;

    } use {

        xstat = ERR;
        process_error(self);

    } end_when;

    return xstat;

}

