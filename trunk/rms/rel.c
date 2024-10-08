
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
#include "xas/misc/bitops.h"

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
int _rel_next(rel_t *, rel_record_t *, ssize_t *);
int _rel_prev(rel_t *, rel_record_t *, ssize_t *);
int _rel_last(rel_t *, rel_record_t *, ssize_t *);
int _rel_first(rel_t *, rel_record_t *, ssize_t *);
int _rel_normalize(rel_t *, void *, void *);
int _rel_find(rel_t *, void *, int (*compare)(void *, void *), off_t *);
int _rel_search(rel_t *, void *, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue_t *), queue_t *);
int _rel_master_unlock(rel_t *);
int _rel_master_lock(rel_t *);

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
    unsigned long recsize;
    unsigned long records;
    unsigned long lastrec;
} rel_header_t;

/*----------------------------------------------------------------*/
/* klass private macros                                           */
/*----------------------------------------------------------------*/

/* the "8" constant is a size of rel_record_t in bytes on x86-64  */
/* with GNU C "gcc (Debian 10.2.1-6) 10.2.1 20210110". I have no  */ 
/* idea if this value would change on differant platforms but it  */
/* is something to consider when moving files between different   */ 
/* platforms.                                                     */

#define REL_RECSIZE(s)   ((s) + 8)
#define REL_RECORD(n, s) (((n) / REL_RECSIZE(s)))
#define REL_OFFSET(n, s) ((((n)) * REL_RECSIZE(s)))

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

    SET_ITEM(items[0], FIB_K_PATH, xpath, strlen(xpath), NULL);
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

int rel_override(rel_t *self, item_list_t *items) {

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

int rel_compare(rel_t *us, rel_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, rel_t)) {

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
            ((recnum < 1) && (recnum > self->records))) {

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

int rel_get_records(rel_t *self, off_t *recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (recnum == NULL)) {

            cause_error(E_INVPARM);

        }

        *recnum = self->records;
        
        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_get_recsize(rel_t *self, off_t *recsize) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (recsize == NULL)) {

            cause_error(E_INVPARM);

        }

        *recsize = self->recsize;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_find(rel_t *self, void *data, int (*compare)(void *, void *), off_t *recnum) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (data == NULL) || (compare == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_find(self, data, compare, recnum);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int rel_search(rel_t *self, void *data, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue_t *), queue_t *results) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (compare == NULL) || (capture == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_search(self, data, compare, capture, results);
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
            self->_master_lock = _rel_master_lock;
            self->_master_unlock = _rel_master_unlock;

            /* initialize internal variables here */

            self->record = 1;
            self->autoextend = FALSE;

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

    when_error_in {

        if (items != NULL) {

            stat = blk_override(BLK(self), items);
            check_return(stat, self);

            errno = E_UNKOVER;

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case REL_M_DESTRUCTOR: {
                        self->dtor = NULL;
                        self->dtor = items[x].buffer_address;
                        check_null(self->dtor);
                        break;
                    }
                    case REL_M_OPEN: {
                        self->_open = NULL;
                        self->_open = items[x].buffer_address;
                        check_null(self->_open);
                        break;
                    }
                    case REL_M_GET: {
                        self->_get = NULL;
                        self->_get = items[x].buffer_address;
                        check_null(self->_get);
                        break;
                    }
                    case REL_M_PUT: {
                        self->_put = NULL;
                        self->_put = items[x].buffer_address;
                        check_null(self->_put);
                        break;
                    }
                    case REL_M_NEXT: {
                        self->_next = NULL;
                        self->_next = items[x].buffer_address;
                        check_null(self->_next);
                        break;
                    }
                    case REL_M_PREV: {
                        self->_prev = NULL;
                        self->_prev = items[x].buffer_address;
                        check_null(self->_prev);
                        break;
                    }
                    case REL_M_LAST: {
                        self->_last = NULL;
                        self->_last = items[x].buffer_address;
                        check_null(self->_last);
                        break;
                    }
                    case REL_M_FIRST: {
                        self->_first = NULL;
                        self->_first = items[x].buffer_address;
                        check_null(self->_first);
                        break;
                    }
                    case REL_M_BUILD: {
                        self->_build = NULL;
                        self->_build = items[x].buffer_address;
                        check_null(self->_build);
                        break;
                    }
                    case REL_M_EXTEND: {
                        self->_extend = NULL;
                        self->_extend = items[x].buffer_address;
                        check_null(self->_extend);
                        break;
                    }
                    case REL_M_ADD: {
                        self->_add = NULL;
                        self->_add = items[x].buffer_address;
                        check_null(self->_add);
                        break;
                    }
                    case REL_M_DEL: {
                        self->_del = NULL;
                        self->_del = items[x].buffer_address;
                        check_null(self->_del);
                        break;
                    }
                    case REL_M_NORMALIZE: {
                        self->_normalize = NULL;
                        self->_normalize = items[x].buffer_address;
                        check_null(self->_normalize);
                        break;
                    }
                    case REL_M_FIND: {
                        self->_find = NULL;
                        self->_find = items[x].buffer_address;
                        check_null(self->_find);
                        break;
                    }
                    case REL_M_SEARCH: {
                        self->_search = NULL;
                        self->_search = items[x].buffer_address;
                        check_null(self->_search);
                        break;
                    }
                    case REL_M_RECORD: {
                        self->_record = NULL;
                        self->_record = items[x].buffer_address;
                        check_null(self->_record);
                        break;
                    }
                    case REL_M_INIT: {
                        self->_init = NULL;
                        self->_init = items[x].buffer_address;
                        check_null(self->_init);
                        break;
                    }
                    case REL_M_READ_HEADER: {
                        self->_read_header = NULL;
                        self->_read_header = items[x].buffer_address;
                        check_null(self->_read_header);
                        break;
                    }
                    case REL_M_WRITE_HEADER: {
                        self->_write_header = NULL;
                        self->_write_header = items[x].buffer_address;
                        check_null(self->_write_header);
                        break;
                    }
                    case REL_M_UPDATE_HEADER: {
                        self->_update_header = NULL;
                        self->_update_header = items[x].buffer_address;
                        check_null(self->_update_header);
                        break;
                    }
                    case REL_M_MASTER_LOCK: {
                        self->_master_lock = NULL;
                        self->_master_lock = items[x].buffer_address;
                        check_null(self->_master_lock);
                        break;
                    }
                    case REL_M_MASTER_UNLOCK: {
                        self->_master_unlock = NULL;
                        self->_master_unlock = items[x].buffer_address;
                        check_null(self->_master_unlock);
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

int _rel_compare(rel_t *self, rel_t *other) {

    int stat = OK;

    when_error_in {

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
            (self->_master_lock == other->_master_lock) &&
            (self->_master_unlock == other->_master_unlock) &&
            (self->autoextend == other->autoextend) &&
            (self->master_locked == other->master_locked) &&
            (self->record == other->record) &&
            (self->lastrec == other->lastrec) &&
            (self->recsize == other->recsize) &&
            (self->records == other->records)) {

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

int _rel_first(rel_t *self, rel_record_t *ondisk, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    ssize_t position = 0;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        stat = blk_seek(BLK(self), recsize, SEEK_SET);
        check_return(stat, self);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        self->record = REL_RECORD(position, self->recsize);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, count);
        check_return(stat, self);

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

int _rel_next(rel_t *self, rel_record_t *ondisk, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    ssize_t position = 0;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        self->record = REL_RECORD(position, self->recsize);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, count);
        check_return(stat, self);

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

int _rel_prev(rel_t *self, rel_record_t *ondisk, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    ssize_t position = 0;
    off_t offset = REL_RECSIZE(self->recsize);
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        *count = 0;

        stat = blk_seek(BLK(self), -offset, SEEK_CUR);
        check_return(stat, self);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        if (position > recsize) {

            stat = blk_lock(BLK(self), position - offset, recsize);
            check_return(stat, self);

            self->record = REL_RECORD(position - offset, self->recsize);

            stat = blk_read(BLK(self), ondisk, recsize, count);
            check_return(stat, self);

            stat = blk_unlock(BLK(self));
            check_return(stat, self);

            stat = blk_seek(BLK(self), -offset, SEEK_CUR);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_last(rel_t *self, rel_record_t *ondisk, ssize_t *count) {

    int stat = OK;
    int locked = FALSE;
    ssize_t position = 0;
    off_t offset = REL_RECSIZE(self->recsize);
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        stat = blk_seek(BLK(self), offset, SEEK_END);
        check_return(stat, self);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        self->record = REL_RECORD(position, self->recsize);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, count);
        check_return(stat, self);

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        stat = blk_seek(BLK(self), -offset, SEEK_CUR);
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

int _rel_get(rel_t *self, off_t recnum, void *record) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);
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

        if (bit_test(ondisk->flags, REL_F_DELETED)) {

            cause_error(E_RMSDEL);

        }
        
        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        stat = self->_build(self, &ondisk->data, record);
        check_return(stat, self);

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_put(rel_t *self, off_t recnum, void *record) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);
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

        stat = self->_normalize(self, &ondisk->data, record);
        check_return(stat, self);

        stat = blk_seek(BLK(self), -recsize, SEEK_CUR);
        check_return(stat, self);

        stat = blk_write(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_find(rel_t *self, void *data, int (*compare)(void *, void *), off_t *recnum) {

    int stat = OK;
    ssize_t count = 0;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        *recnum = 0;

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count == recsize) {

            if (! bit_test(ondisk->flags, REL_F_DELETED)) {

                if (compare(data, &ondisk->data)) {

                    *recnum = self->record;
                    break;

                }

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);

    } end_when;

    return stat;

}

int _rel_search(rel_t *self, void *data, int (*compare)(void *, void *), int (*capture)(rel_t *, void *, queue_t *), queue_t *results) {

    int stat = OK;
    ssize_t count = 0;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count == recsize) {

            if (! bit_test(ondisk->flags, REL_F_DELETED)) {

                if (compare(data, &ondisk->data)) {

                    stat = capture(self, &ondisk->data, results);
                    check_return(stat, self);

                }

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);

    } end_when;

    return stat;

}

int _rel_add(rel_t *self, void *record) {

    int stat = OK;
    ssize_t count = 0;
    int created = FALSE;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, self->recsize);
        check_null(ondisk);

        stat = self->_master_lock(self);
        check_return(stat, self);

        stat = self->_first(self, ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (bit_test(ondisk->flags, REL_F_DELETED)) {

                memcpy(&ondisk->data, record, self->recsize);
                bit_clear(ondisk->flags, REL_F_DELETED);

                stat = blk_seek(BLK(self), -recsize, SEEK_CUR);
                check_return(stat, self);

                stat = blk_write(BLK(self), ondisk, recsize, &count);
                check_return(stat, self);

                if (count != recsize) {

                    cause_error(EIO);

                }

                created = TRUE;
                break;

            }

            stat = self->_next(self, ondisk, &count);
            check_return(stat, self);

        }

        stat = self->_master_unlock(self);
        check_return(stat, self);

        if (! created) {

            cause_error(EOVERFLOW);

        }

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        if (self->master_locked) self->_master_unlock(self);

    } end_when;

    return stat;

}

int _rel_del(rel_t *self, off_t recnum) {

    int stat = OK;
    ssize_t count = 0;
    int locked = FALSE;
    ssize_t position = 0;
    rel_record_t *ondisk = NULL;
    off_t recsize = REL_RECSIZE(self->recsize);
    off_t offset = REL_OFFSET(recnum, self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = blk_seek(BLK(self), offset, SEEK_SET);
        check_return(stat, self);

        stat = blk_tell(BLK(self), &position);
        check_return(stat, self);

        stat = blk_lock(BLK(self), position, recsize);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        bit_set(ondisk->flags, REL_F_DELETED);
        memset(&ondisk->data, '\0', self->recsize);

        stat = blk_seek(BLK(self), -recsize, SEEK_CUR);
        check_return(stat, self);

        stat = blk_write(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = blk_unlock(BLK(self));
        check_return(stat, self);

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        blk_is_locked(BLK(self), &locked);
        if (locked) blk_unlock(BLK(self));

    } end_when;

    return stat;

}

int _rel_extend(rel_t *self, int amount) {

    int stat = OK;
    ssize_t count = 0;
    rel_record_t *ondisk = NULL;
    off_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        bit_set(ondisk->flags, REL_F_MARK);
        bit_set(ondisk->flags, REL_F_DELETED);
        memset(&ondisk->data, '\0', self->recsize);

        stat = self->_master_lock(self);
        check_return(stat, self);

        int x = 0;
        for (; x < self->records; x++) {

            stat = blk_write(BLK(self), ondisk, recsize, &count);
            check_return(stat, self);

            if (count != recsize) {

                cause_error(EIO);

            }

        }

        stat = self->_master_unlock(self);
        check_return(stat, self);

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        if (self->master_locked) self->_master_unlock(self);

    } end_when;

    return stat;

}

int _rel_master_lock(rel_t *self) {

    int fd;
    int stat = OK;
    int count = 0;
    int retries = 0;
    int timeout = 0;
    off_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        stat = blk_get_retries(BLK(self), &retries);
        check_return(stat, self);

        stat = blk_get_timeout(BLK(self), &timeout);
        check_return(stat, self);

        self->master.l_type = F_WRLCK;
        self->master.l_start = 1;
        self->master.l_len = recsize;
        self->master.l_whence = SEEK_SET;
        self->master.l_pid = getpid();

        for (;;) {

            errno = 0;
            if (fcntl(fd, F_SETLK, &self->master) == -1) {

                if ((errno == EAGAIN) || (errno == EACCES)) {

                    count++;

                    if (count > retries) {

                        cause_error(errno);

                    } else {

                        sleep(timeout);

                    }

                } else {

                    cause_error(errno);

                }

            } else {

                self->master_locked = TRUE;
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

int _rel_master_unlock(rel_t *self) {

    int fd;
    int stat = OK;

    when_error_in {

        stat = fib_get_fd(FIB(self), &fd);
        check_return(stat, self);

        self->master.l_type = F_UNLCK;

        errno = 0;
        if (fcntl(fd, F_SETLK, &self->master) == -1) {

            cause_error(errno);

        }

        self->master_locked = FALSE;
        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rel_read_header(rel_t *self) {

    int stat = OK;
    ssize_t count = 0;
    rel_header_t header;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        stat = self->_master_lock(self);
        check_return(stat, self);

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = self->_master_unlock(self);
        check_return(stat, self);

        memcpy(&header, &ondisk->data, sizeof(rel_header_t));

        self->recsize = header.recsize;
        self->records = header.records;
        self->lastrec = header.lastrec;

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        if (self->master_locked) self->_master_unlock(self);

    } end_when;

    return stat;

}

int _rel_write_header(rel_t *self) {

    int stat = OK;
    ssize_t count = 0;
    rel_header_t header;
    rel_record_t *ondisk = NULL;
    ssize_t recsize = REL_RECSIZE(self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, recsize);
        check_null(ondisk);

        header.type[0] = 'R';
        header.type[1] = 'E';
        header.type[2] = 'L';
        header.type[3] = '\0';

        header.recsize = self->recsize;
        header.records = self->records;
        header.lastrec = self->lastrec;

        ondisk->flags = 0;
        memcpy(&ondisk->data, &header, sizeof(rel_header_t));

        stat = self->_master_lock(self);
        check_return(stat, self);

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_write(BLK(self), ondisk, recsize, &count);
        check_return(stat, self);

        if (count != recsize) {

            cause_error(EIO);

        }

        stat = self->_master_unlock(self);
        check_return(stat, self);

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        if (self->master_locked) self->_master_unlock(self);

    } end_when;

    return stat;

}

int _rel_update_header(rel_t *self) {

    int stat = OK;
    long records = 0;
    long lastrec = 0;
    long recsize = 0;
    ssize_t count = 0;
    rel_header_t header;
    rel_record_t *ondisk = NULL;
    ssize_t xrecsize = REL_RECSIZE(self->recsize);

    when_error_in {

        errno = 0;
        ondisk = calloc(1, xrecsize);
        check_null(ondisk);

        stat = self->_master_lock(self);
        check_return(stat, self);

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_read(BLK(self), ondisk, xrecsize, &count);
        check_return(stat, self);

        if (count != xrecsize) {

            cause_error(EIO);

        }

        memcpy(&header, ondisk->data, sizeof(rel_header_t));

        recsize = header.recsize;
        records = header.records;
        lastrec = header.lastrec;

        if (recsize != self->recsize) {

            cause_error(E_INVREC);

        }

        if (records < self->records) {

            cause_error(EIO);

        }

        self->records = records;
        self->lastrec = lastrec;

        header.recsize = self->recsize;
        header.records = self->records;
        header.lastrec = self->lastrec;

        memcpy(&ondisk->data, &header, sizeof(rel_header_t));

        stat = blk_seek(BLK(self), 0, SEEK_SET);
        check_return(stat, self);

        stat = blk_write(BLK(self), ondisk, xrecsize, &count);
        check_return(stat, self);

        if (count != xrecsize) {

            cause_error(EIO);

        }

        stat = self->_master_unlock(self);
        check_return(stat, self);

        free((void *)ondisk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (ondisk) free((void *)ondisk);
        if (self->master_locked) self->_master_unlock(self);

    } end_when;

    return stat;

}

int _rel_build(rel_t *self, void *ondisk, void *data) {

    /* default behavior, this should to be overridden */

    int stat = OK;
    void *junk = NULL;

    when_error_in {

        errno = ENOMEM;
        junk = memcpy(data, ondisk, self->recsize);
        check_null(junk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _rel_init(rel_t *self) {

    /* this may need to be overridden, default is to do nothing */

    return OK;

}

int _rel_normalize(rel_t *self, void *ondisk, void *data) {

    /* default behavior, this should to be overridden */

    int stat = OK;
    void *junk = NULL;

    when_error_in {

        errno = ENOMEM;
        junk = memcpy(ondisk, data, self->recsize);
        check_null(junk);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

