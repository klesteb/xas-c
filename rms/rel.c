
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2023 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include "xas/rms/rel.h"
#include "xas/error_codes.h"
#include "xas/error_handler.h"
#include "xas/gpl/fnm_util.h"

require_klass(BLK_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _rel_ctor(object_t *, item_list_t *);
int _rel_dtor(object_t *);
int _rel_compare(rel_t *, rel_t *);
int _rel_override(rel_t *, item_list_t *);

int _rel_init(rel_t *);
int _rel_open(rel_t *, int, mode_t);
int _rel_remove(rel_t *);
int _rel_del(rel_t *, off_t);
int _rel_extend(rel_t *, int);
int _rel_add(rel_t *, void *);
int _rel_record(rel_t *, off_t *);
int _rel_get(rel_t *, off_t, void *);
int _rel_put(rel_t *, off_t, void *);
int _rel_read_header(rel_t *);
int _rel_write_header(rel_t *);
int _rel_update_header(rel_t *);
int _rel_build(rel_t *, void *, void *);
int _rel_next(rel_t *, void *, ssize_t *);
int _rel_prev(rel_t *, void *, ssize_t *);
int _rel_last(rel_t *, void *, ssize_t *);
int _rel_first(rel_t *, void *, ssize_t *);
int _rel_normalize(rel_t *, void *, void *);
int _rel_find(rel_t *, void *, int, int (*compare)(void *, void *), off_t *);
int _rel_search(rel_t *, void *, int, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue *), queue *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(REL_KLASS) {
    .size = KLASS_SIZE(rel_t),
    .name = KLASS_NAME(rel_t),
    .ctor = _rel_ctor,
    .dtor = _rel_dtor,
};

/*----------------------------------------------------------------*/
/* klass private data                                             */
/*----------------------------------------------------------------*/

typedef struct header {
    char type[4];
    char recsize[4];
    char records[4];
    char lastrec[4];
} rel_header_t;

/*----------------------------------------------------------------*/
/* klass private macros                                           */
/*----------------------------------------------------------------*/

#define REL_OFFSET(n, s) ((((n)) * (s)))
#define REL_RECORD(n, s) (((n) / (s)))

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

rel_t *rel_create(char *path, char *name, int records, int recsize, int retries, int timeout) {

    int stat = ERR;
    char xpath[256];
    rel_t *self = NULL;
    item_list_t items[7];

    memset(xpath, '\0', 256);
    strncpy(xpath, fnm_build(1, FnmPath, name, ".dat", path, NULL), 255);

    SET_ITEM(items[0], FIB_K_PATH, xpath, strlen(path), NULL);
    SET_ITEM(items[1], REL_K_NAME, name, strlen(name), NULL);
    SET_ITEM(items[2], BLK_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], BLK_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], REL_K_RECORDS, &records, sizeof(int), NULL);
    SET_ITEM(items[5], REL_K_RECSIZE, &recsize, sizeof(int), NULL);
    SET_ITEM(items[6], 0,0,0,0);

    self = (rel_t *)object_create(REL_KLASS, items, &stat);

    return self;

}

int rel_destroy(rel_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, rel_t)) {

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

int rel_override(rel_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
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

int rel_compare(rel_t *us, rel_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, rel_t)) {

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

char *rel_version(rel_t *self) {

    char *version = VERSION;

    return version;

}

int rel_open(rel_t *self, int flags, mode_t mode) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_open(self, flags, mode);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_remove(rel_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_remove(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_add(rel_t *self, void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add(self, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_del(rel_t *self, off_t recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || ((recnum < 0) && (recnum > self->records))) {

            cause_error(E_INVPARM);

        }

        stat = self->_del(self, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_get(rel_t *self, off_t recnum, void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) ||
            ((recnum < 0) && (recnum > self->records))) {

            cause_error(E_INVPARM);

        }

        stat = self->_get(self, recnum, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_put(rel_t *self, off_t recnum, void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) ||
            ((recnum < 0) && (recnum > self->records))) {

            cause_error(E_INVPARM);

        }

        stat = self->_put(self, recnum, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_record(rel_t *self, off_t *recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (recnum == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_record(self, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_find(rel_t *self, void *data, int len,  int (*compare)(void *, void *), off_t *recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) || (compare == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_find(self, data, len, compare, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_search(rel_t *self, void *data, int len,  int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue *), queue *results) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (compare == NULL) || (capture == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_search(self, data, len, compare, capture, results);
        check_return(stat, self);

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

int _rel_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    int records = 0;
    int recsize = 0;
    rel_t *self = NULL;

    if (object != NULL) {

        stat = OK;

        when_error_in {

            /* intialize the base klass here */
        
            stat = BLK_KLASS->ctor(object, items);
            check_return(stat, object);
        
            /* capture our items */

            if (items != NULL) {

                int x;
                for (x = 0;; x++) {

                    if ((items[x].buffer_length == 0) &&
                        (items[x].item_code == 0)) break;

                    switch(items[x].item_code) {
                        case REL_K_RECORDS: {
                            memcpy(&records, 
                                   items[x].buffer_address, 
                                   items[x].buffer_length);
                            break;
                        }
                        case REL_K_RECSIZE: {
                            memcpy(&recsize, 
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

            self = REL(object);

            /* assign our methods here */

            self->ctor = _rel_ctor;
            self->dtor = _rel_dtor;
            self->_compare = _rel_compare;
            self->_override = _rel_override;

            self->_add    = _rel_add;
            self->_del    = _rel_del;
            self->_get    = _rel_get;
            self->_put    = _rel_put;
            self->_init   = _rel_init;
            self->_find   = _rel_find;
            self->_last   = _rel_last;
            self->_next   = _rel_next;
            self->_open   = _rel_open;
            self->_prev   = _rel_prev;
            self->_build  = _rel_build;
            self->_first  = _rel_first;
            self->_extend = _rel_extend;
            self->_record = _rel_record;
            self->_remove = _rel_remove;
            self->_search = _rel_search;
            self->_normalize = _rel_normalize;
            self->_read_header = _rel_read_header;
            self->_write_header = _rel_write_header;
            self->_update_header = _rel_update_header;

            /* initialize internal variables here */

            self->record = 1;

            /* these are overwritten by the header */

            self->lastrec = 1;
            self->recsize = recsize;
            self->records = records;

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _rel_dtor(object_t *object) {

    int stat = OK;
    blk_t *blk = BLK(object);

    /* free local resources here */

    /* walk the chain, freeing as we go */

    object_demote(object, blk_t);
    blk_destroy(blk);

    return stat;

}

int _rel_override(rel_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        when_error_in {

            stat = blk_override(BLK(self), items);
            check_return(stat, self);

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case REL_M_DESTRUCTOR: {
                        self->dtor = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_OPEN: {
                        self->_open = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_GET: {
                        self->_get = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_PUT: {
                        self->_put = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_NEXT: {
                        self->_next = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_PREV: {
                        self->_prev = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_LAST: {
                        self->_last = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_FIRST: {
                        self->_first = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_BUILD: {
                        self->_build = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_EXTEND: {
                        self->_extend = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_ADD: {
                        self->_add = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_DEL: {
                        self->_del = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_NORMALIZE: {
                        self->_normalize = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_FIND: {
                        self->_find = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_SEARCH: {
                        self->_search = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_RECORD: {
                        self->_record = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                    case REL_M_INIT: {
                        self->_init = items[x].buffer_address;
                        stat = OK;
                        break;
                    }
                }

            }

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _rel_compare(rel_t *self, rel_t *other) {

    int stat = ERR;

    if ((blk_compare(BLK(self), BLK(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_open    == other->_open) &&
        (self->_add    == other->_add) &&
        (self->_del    == other->_del) &&
        (self->_get    == other->_get) &&
        (self->_put    == other->_put) &&
        (self->_next   == other->_next) &&
        (self->_prev   == other->_prev) &&
        (self->_last   == other->_last) &&
        (self->_first  == other->_first) &&
        (self->_build  == other->_build) &&
        (self->_find   == other->_find) &&
        (self->_record == other->_record) &&
        (self->_remove == other->_remove) &&
        (self->_extend == other->_extend) &&
        (self->_search == other->_search) &&
        (self->_normalize == other->_normalize) &&
        (self->_read_header == other->_read_header) &&
        (self->_write_header == other->_write_header) &&
        (self->_update_header == other->_update_header) &&
        (self->record == other->record) &&
        (self->lastrec == other->lastrec) &&
        (self->recsize == other->recsize) &&
        (self->records == other->records)) {

        stat = OK;

    }

    return stat;

}

int _rel_open(rel_t *self, int flags, mode_t mode) {

    int stat = OK;
    int exists = 0;

    when_error_in {

        stat = blk_exists(BLK(self), &exists);
        check_return(stat, self);

        if (exists) {

            stat = blk_open(BLK(self), flags, mode);
            check_return(stat, self);

            stat = self->_read_header(self);
            check_return(stat, self);

        } else {

            stat = blk_creat(BLK(self), mode);
            check_return(stat, self);

            stat = blk_open(BLK(self), flags, mode);
            check_return(stat, self);

            stat = self->_write_header(self);
            check_return(stat, self);

            stat = self->_extend(self, self->records);
            check_return(stat, self);

            stat = self->_init(self);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rel_remove(rel_t *self) {

    int stat = OK;

    when_error_in {

        stat = blk_close(BLK(self));
        check_return(stat, self);

        stat = blk_unlink(BLK(self));
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rel_record(rel_t *self, off_t *recnum) {

    *recnum = self->record;

    return OK;

}

int _rel_first(rel_t *self, void *data, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    void *ondisk = NULL;
    ssize_t position = 0;
    ssize_t recsize = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = blk_seek(BLK(self), recsize, SEEK_SET);
        check_return(stat, self);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        self->record = REL_RECORD(position, recsize);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, count);
        check_return(stat, self);

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        if (*count == recsize) {

            stat = self->_build(self, ondisk, data);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_next(rel_t *self, void *data, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    void *ondisk = NULL;
    ssize_t position = 0;
    ssize_t recsize = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        self->record = REL_RECORD(position, recsize);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, count);
        check_return(stat, self);

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        if (*count == recsize) {

            stat = self->_build(self, ondisk, data);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_prev(rel_t *self, void *data, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    void *ondisk = NULL;
    ssize_t position = 0;
    off_t offset = self->recsize;
    ssize_t recsize = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        if (position > 0) {

            stat = blk_seek(BLK(self), -offset, SEEK_CUR);
            check_return(stat, self);

            stat = blk_lock(BLK(self), position - offset, recsize);
            check_return(stat, self);

            self->record = REL_RECORD(position - offset, recsize);

            stat = blk_read(BLK(self), ondisk, recsize, count);
            check_return(stat, self);

            stat = blk_unlock(BLK(self));
            check_return(stat, self);

            stat = blk_seek(BLK(self), -offset, SEEK_CUR);
            check_return(stat, self);

            if (*count == recsize) {

                stat = self->_build(self, ondisk, data);
                check_return(stat, self);

            }

        } else {

            *count = 0;

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_last(rel_t *self, void *data, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    void *ondisk = NULL;
    ssize_t position = 0;
    off_t offset = self->recsize;
    ssize_t recsize = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = blk_seek(BLK(self), 0, SEEK_END);
        check_return(stat, self);

        stat = blk_seek(BLK(self), -offset, SEEK_CUR);
        check_return(stat, self);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        self->record = REL_RECORD(position, recsize);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, count);
        check_return(stat, self);

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        stat = blk_seek(BLK(self), -offset, SEEK_CUR);
        check_return(stat, self);

        if (*count == recsize) {

            stat = self->_build(self, ondisk, data);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_get(rel_t *self, off_t recnum, void *record) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    void *ondisk = NULL;
    ssize_t recsize = self->recsize;
    off_t offset = REL_OFFSET(recnum, self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = blk_seek(BLK(self), offset, SEEK_SET);
        check_return(stat, self);

        stat = blk_lock(BLK(self), offset, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        stat = self->_build(self, ondisk, record);
        check_return(stat, self);

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_put(rel_t *self, off_t recnum, void *record) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    void *ondisk = NULL;
    ssize_t recsize = self->recsize;
    off_t offset = REL_OFFSET(recnum, self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        check_null(ondisk);

        stat = blk_seek(BLK(self), offset, SEEK_SET);
        check_return(stat, self);

        stat = blk_lock(BLK(self), offset, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = self->_normalize(self, ondisk, record);
        check_return(stat, self);

        stat = blk_seek(BLK(self), -recsize, SEEK_CUR);
        check_return(stat, self);

        stat = blk_write(BLK(self), record, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_find(rel_t *self, void *data, int len, int (*compare)(void *, void *), off_t *recnum) {

    int stat = OK;
    ssize_t count = 0;
    void *ondisk = NULL;
    ssize_t recsize = self->recsize;

    when_error_in {

        *recnum = 0;

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, ondisk)) {

                *recnum = self->record;
                break;

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);

    } end_when;

    return stat;

}

int _rel_search(rel_t *self, void *data, int len, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue *), queue *results) {

    int stat = OK;
    ssize_t count = 0;
    void *ondisk = NULL;
    ssize_t recsize = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (compare(data, ondisk)) {

                stat = capture(self, ondisk, results);
                check_return(stat, self);

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        free(ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free(ondisk);

    } end_when;

    return stat;

}

int _rel_read_header(rel_t *self) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    rel_header_t header;
    ssize_t recsize = sizeof(rel_header_t);

    when_error_in {

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_lock(BLK(self), 0, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), &header, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        memcpy(&self->recsize, &header.recsize, 4);
        memcpy(&self->records, &header.records, 4);
        memcpy(&self->lastrec, &header.lastrec, 4);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_write_header(rel_t *self) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    void *ondisk = NULL;
    rel_header_t header;
    ssize_t recsize = self->recsize;

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        header.type[0] = 'R';
        header.type[1] = 'E';
        header.type[2] = 'L';
        header.type[3] = '\0';

        memcpy(&header.recsize, &self->recsize, 4);
        memcpy(&header.records, &self->records, 4);
        memcpy(&header.lastrec, &self->lastrec, 4);
        memcpy(ondisk, &header, sizeof(rel_header_t));

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_lock(BLK(self), 0, recsize);
        check_return(stat, self);

        stat = blk_write(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_update_header(rel_t *self) {

    int stat = OK;
    long records = 0;
    long lastrec = 0;
    long xrecsiz = 0;
    ssize_t count = 0;
    int locked = FALSE;
    rel_header_t header;
    ssize_t recsize = sizeof(rel_header_t);

    when_error_in {

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_lock(BLK(self), 0, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), &header, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        memcpy(&xrecsiz, &header.recsize, 4);
        memcpy(&records, &header.records, 4);
        memcpy(&lastrec, &header.lastrec, 4);

        if (xrecsiz != self->recsize) {

            cause_error(E_INVREC);

        }

        if (records != self->records) {

            self->records = records;

        }

        if (lastrec != self->lastrec) {

            self->lastrec = lastrec;

        }

        memcpy(&header.recsize, &self->recsize, 4);
        memcpy(&header.records, &self->records, 4);
        memcpy(&header.lastrec, &self->lastrec, 4);

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_write(BLK(self), &header, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_add(rel_t *self, void *record) {

    /* this needs to be overridden */

    return OK;

}

int _rel_build(rel_t *self, void *ondisk, void *data) {

    /* this needs to be overridden */

    return OK;

}
                            
int _rel_del(rel_t *self, off_t recnum) {

    /* this needs to be overriden */

    return OK;
    
}

int _rel_extend(rel_t *self, int amount) {

    /* this needs to be overridden */

    return OK;

}

int _rel_init(rel_t *self) {
    
    /* this may need to be overridden, default is to do nothing */

    return OK;

}

int _rel_normalize(rel_t *self, void *ondisk, void *data) {

    /* this needs to be overridden */

    return OK;

}

