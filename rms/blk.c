
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

#include "xas/rms/blk.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"

require_klass(FIB_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _blk_ctor(object_t *, item_list_t *);
int _blk_dtor(object_t *);
int _blk_compare(blk_t *, blk_t *);
int _blk_override(blk_t *, item_list_t *);

int _blk_lock(blk_t *, off_t, off_t);
int _blk_read(blk_t *, void *, size_t, ssize_t *);
int _blk_seek(blk_t *, off_t, int);
int _blk_tell(blk_t *, off_t *);
int _blk_write(blk_t *, void *, size_t, ssize_t *);
int _blk_unlock(blk_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(BLK_KLASS) {
    .size = KLASS_SIZE(blk_t),
    .name = KLASS_NAME(blk_t),
    .ctor = _blk_ctor,
    .dtor = _blk_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

blk_t *blk_create(char *filename, int retries, int timeout) {

    int stat = ERR;
    blk_t *self = NULL;
    item_list_t items[4];

    SET_ITEM(items[0], FIB_K_PATH, filename, strlen(filename), NULL);
    SET_ITEM(items[1], BLK_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[2], BLK_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[3], 0, 0, 0, 0);

    self = (blk_t *)object_create(BLK_KLASS, items, &stat);

    return self;

}

int blk_destroy(blk_t *self) {

    int stat = OK;

    when_error_in {

        if (self != NULL) {

            if (object_assert(self, blk_t)) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_override(blk_t *self, item_list_t *items) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_compare(blk_t *us, blk_t *them) {

    int stat = OK;

    when_error_in {

        if (us != NULL) {

            if (object_assert(them, blk_t)) {

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
        process_error(us);

    } end_when;

    return stat;

}

char *blk_version(blk_t *self) {

    char *version = VERSION;

    return version;

}

int blk_seek(blk_t *self, off_t offset, int whence) {
    
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
        process_error(self);

    } end_when;

    return stat;

}

int blk_tell(blk_t *self, off_t *offset) {
    
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
        process_error(self);

    } end_when;

    return stat;

}

int blk_read(blk_t *self, void *buffer, size_t size, ssize_t *count) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_write(blk_t *self, void *buffer, size_t size, ssize_t *count) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_lock(blk_t *self, off_t offset, off_t length) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_unlock(blk_t *self) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_get_timeout(blk_t *self, int *timeout) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_set_timeout(blk_t *self, int timeout) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_get_retries(blk_t *self, int *retries) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_set_retries(blk_t *self, int retries) {

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
        process_error(self);

    } end_when;

    return stat;

}

int blk_is_locked(blk_t *self, int *locked) {

    int stat = OK;

    when_error_in {

        if ((self != NULL)) {

            *locked = self->locked;

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

int _blk_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    int retries = 10;
    int timeout = 30;
    blk_t *self = NULL;

    if (object != NULL) {

        stat = OK;
        
        when_error_in {
            
            /* intialize the base klass here */
        
            stat = FIB_KLASS->ctor(object, items);
            check_return(stat, object);
        
            /* capture our items */

            if (items != NULL) {
               
               int x;
               for (x = 0;; x++) {
                                   
                   if ((items[x].buffer_length == 0) &&
                       (items[x].item_code == 0)) break;
                                   
                   switch(items[x].item_code) {
                       case BLK_K_TIMEOUT: {
                           memcpy(&timeout, 
                                  items[x].buffer_address, 
                                  items[x].buffer_length);
                           break;
                       }
                       case BLK_K_RETRIES: {
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

            self = BLK(object);

            /* assign our methods here */

            self->ctor = _blk_ctor;
            self->dtor = _blk_dtor;
            self->_compare = _blk_compare;
            self->_override = _blk_override;

            self->_read   = _blk_read;
            self->_write  = _blk_write;
            self->_seek   = _blk_seek;
            self->_tell   = _blk_tell;
            self->_lock   = _blk_lock;
            self->_unlock = _blk_unlock;

            /* initialize internal variables here */

            self->locked = FALSE;
            self->retries = retries;
            self->timeout = timeout;

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _blk_dtor(object_t *object) {

    int stat = OK;
    fib_t *fib = FIB(object);

    /* free local resources here */


    /* walk the chain, freeing as we go */

    object_demote(object, fib_t);
    fib_destroy(fib);

    return stat;

}

int _blk_override(blk_t *self, item_list_t *items) {

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
                    case BLK_M_DESTRUCTOR: {
                        self->dtor = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case BLK_M_READ: {
                        self->_read = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case BLK_M_WRITE: {
                        self->_write = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case BLK_M_SEEK: {
                        self->_seek = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case BLK_M_TELL: {
                        self->_tell = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case BLK_M_LOCK: {
                        self->_lock = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case BLK_M_UNLOCK: {
                        self->_unlock = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                }

            } 

        } use { 
            
            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _blk_compare(blk_t *self, blk_t *other) {

    int stat = ERR;

    if ((fib_compare(FIB(self), FIB(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_read == other->_read) &&
        (self->_write == other->_write) &&
        (self->_seek == other->_seek) &&
        (self->_tell == other->_tell) &&
        (self->_lock == other->_lock) &&
        (self->_unlock == other->_unlock)) {

        stat = OK;

    }

    return stat;

}

int _blk_seek(blk_t *self, off_t offset, int whence) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        errno = 0;
        if (lseek(fd, offset, whence) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _blk_tell(blk_t *self, off_t *offset) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        errno = 0;
        if ((*offset = lseek(fd, 0, SEEK_CUR)) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _blk_read(blk_t *self, void *buffer, size_t size, ssize_t *count) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        errno = 0;
        if ((*count = read(fd, buffer, size)) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _blk_write(blk_t *self, void *buffer, size_t size, ssize_t *count) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        errno = 0;
        if ((*count = write(fd, buffer, size)) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _blk_lock(blk_t *self, off_t offset, off_t length) {

    int fd;
    int stat = OK;
    int count = 0;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        self->lock.l_type = F_WRLCK;
        self->lock.l_start = offset;
        self->lock.l_len = length;
        self->lock.l_whence = SEEK_SET;
        self->lock.l_pid = getpid();

        for (;;) {

            errno = 0;
            if (fcntl(fd, F_SETLK, &self->lock) == -1) {

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

                self->locked = TRUE;
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

int _blk_unlock(blk_t *self) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        self->lock.l_type = F_UNLCK;

        errno = 0;
        if (fcntl(fd, F_SETLK, &self->lock) == -1) {

            cause_error(errno);

        }

        self->locked = FALSE;
        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

