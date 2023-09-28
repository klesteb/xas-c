
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

#include "xas/rms/fib.h"
#include "xas/object.h"
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
int _fib_read(fib_t *, void *, size_t, ssize_t *);
int _fib_write(fib_t *, void *, size_t, ssize_t *);
int _fib_exists(fib_t *, int *);
int _fib_lock(fib_t *, off_t, off_t);
int _fib_seek(fib_t *, off_t, int);
int _fib_stat(fib_t *, struct stat *);
int _fib_tell(fib_t *, off_t *);
int _fib_unlink(fib_t *);
int _fib_unlock(fib_t *);
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

fib_t *fib_create(char *filename, int retries, int timeout) {

    int stat = ERR;
    fib_t *self = NULL;
    item_list_t items[4];

    SET_ITEM(items[0], FIB_K_PATH, filename, strlen(filename), NULL);
    SET_ITEM(items[1], FIB_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[2], FIB_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[3], 0, 0, 0, 0);

    self = (fib_t *)object_create(FIB_KLASS, items, &stat);

    return self;

}

int fib_destroy(fib_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, fib_t)) {

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

int fib_override(fib_t *self, item_list_t *items) {

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

int fib_compare(fib_t *us, fib_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, fib_t)) {

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int fib_read(fib_t *self, void *buffer, size_t size, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL) && (count != NULL)) {

            stat = self->_read(self, buffer, size, count);
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

int fib_write(fib_t *self, void *buffer, size_t size, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self != NULL) && (buffer != NULL) && (count != NULL)) {

            stat = self->_write(self, buffer, size, count);
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

int fib_seek(fib_t *self, off_t offset, int whence) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_seek(self, offset, whence);
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

int fib_tell(fib_t *self, off_t *offset) {
    
    int stat = OK;

    when_error_in {

        if ((self != NULL) && (offset != NULL)) {

            stat = self->_tell(self, offset);
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

int fib_lock(fib_t *self, off_t offset, off_t length) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_lock(self, offset, length);
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

int fib_unlock(fib_t *self) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            stat = self->_unlock(self);
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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int fib_get_retries(fib_t *self, int *retries) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            *retries = self->retries;

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

int fib_set_retries(fib_t *self, int retries) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            self->retries = retries;

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

int fib_get_timeout(fib_t *self, int *timeout) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            *timeout = self->timeout;

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

int fib_set_timeout(fib_t *self, int timeout) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            self->timeout = timeout;

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

int _fib_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    int retries = 10;
    int timeout = 30;
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
                    case FIB_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case FIB_K_RETRIES: {
                        memcpy(&retries, 
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
        self->_read   = _fib_read;
        self->_write  = _fib_write;
        self->_exists = _fib_exists;
        self->_lock   = _fib_lock;
        self->_seek   = _fib_seek;
        self->_size   = _fib_size;
        self->_stat   = _fib_stat;
        self->_tell   = _fib_tell;
        self->_unlink = _fib_unlink;
        self->_unlock = _fib_unlock;

        /* mutators */
        

        /* initialize internal variables here */

        self->retries = retries;
        self->timeout = timeout;
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
                case FIB_M_READ: {
                    self->_read = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_WRITE: {
                    self->_write = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_SEEK: {
                    self->_seek = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_TELL: {
                    self->_tell = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_LOCK: {
                    self->_lock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case FIB_M_UNLOCK: {
                    self->_unlock = items[x].buffer_address;
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
        (self->_read == other->_read) &&
        (self->_write == other->_write) &&
        (self->_exists == other->_exists) &&
        (self->_stat == other->_stat) &&
        (self->_size == other->_size) &&
        (self->_seek == other->_seek) &&
        (self->_tell == other->_tell) &&
        (self->_lock == other->_lock) &&
        (self->_unlock == other->_unlock) &&
        (self->_unlink == other->_unlink)) {

        stat = OK;

    }

    return stat;

}

int _fib_open(fib_t *self, int flags, mode_t mode) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if (mode != 0) {

            if ((self->fd = open(self->path, flags, mode)) == -1) {

                cause_error(errno);

            }

        } else {

            if ((self->fd = open(self->path, flags)) == -1) {

                cause_error(errno);

            }

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _fib_read(fib_t *self, void *buffer, size_t size, ssize_t *count) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if ((*count = read(self->fd, buffer, size)) == -1) {

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

int _fib_write(fib_t *self, void *buffer, size_t size, ssize_t *count) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if ((*count = write(self->fd, buffer, size)) == -1) {

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

int _fib_lock(fib_t *self, off_t offset, off_t length) {

    int stat = OK;
    int count = 0;

    when_error_in {

        self->lock.l_type = F_WRLCK;
        self->lock.l_start = offset;
        self->lock.l_len = length;
        self->lock.l_whence = SEEK_SET;
        self->lock.l_pid = getpid();

        for (;;) {

            errno = 0;
            if (fcntl(self->fd, F_SETLK, &self->lock) == -1) {

                if ((errno == EAGAIN) || (errno == EACCES)) {

                    count++;

                    if (count > self->retries) {

                        cause_error(errno);

                    } else {

                        sleep(self->timeout);

                    }

                } else {

                    cause_error(errno);

                }

            } else {

                break;

            }

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _fib_unlock(fib_t *self) {

    int stat = OK;

    when_error_in {

        self->lock.l_type = F_UNLCK;

        errno = 0;
        if (fcntl(self->fd, F_SETLK, &self->lock) == -1) {

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

int _fib_seek(fib_t *self, off_t offset, int whence) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if (lseek(self->fd, offset, whence) == -1) {

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

int _fib_tell(fib_t *self, off_t *offset) {

    int stat = OK;

    when_error_in {

        errno = 0;
        if ((*offset = lseek(self->fd, 0, SEEK_CUR)) == -1) {

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return xstat;

}

