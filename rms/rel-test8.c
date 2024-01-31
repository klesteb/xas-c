
#include <stdio.h>

#include "xas/types.h"
#include "xas/errors.h"
#include "xas/tracer.h"
#include "xas/error_handler.h"
#include "xas/error_codes.h"
#include "xas/gpl/vperror.h"
#include "xas/rms/rel.h"
#include "xas/misc/misc.h"
#include "rel-test.h"

rel_t *temp = NULL;
err_t *errors = NULL;
tracer_t *trace = NULL;


int capture(rel_t *temp, void *data, queue_t *results) {

    int stat = OK;
    off_t recsize = 0;
    void *junk = NULL;

    when_error_in {

        stat = rel_get_recsize(temp, &recsize);
        check_return(stat, temp);

        errno = 0;
        junk = calloc(1, recsize);
        check_null(junk);

        memcpy(junk, data, recsize);

        stat = que_push_head(results, junk);
        check_status(stat, OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;
        capture_error(trace);

    } end_when;

    return stat;

}

int compare(void *wanted, void *data) {

    int stat = FALSE;

    if (strncmp((char *)wanted, (char *)data, 19) == 0) {

        stat = TRUE;

    }

    return stat;

}

int output_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

void capture_trace(error_trace_t *error) {

    tracer_add(trace, error);

}

int init(void) {

    int stat = OK;

    when_error_in {

        errors = err_create();
        check_creation(errors);

        trace = tracer_create(errors);
        check_creation(trace);

        vperror_init(capture_trace);

        temp = rel_create(path, filename, records, recsize, retries, timeout);
        check_creation(temp);

        stat = rel_set_trace(temp, capture_trace);
        check_return(stat, temp);

        exit_when;

    } use {

        stat = ERR;
        capture_error(trace);

    } end_when;

    return stat;

}

int main(int argc, char **argv) {

    int stat = OK;
    queue_t results;
    off_t recsize = 0;
    char *buffer = NULL;
    char *record = "22222222222222222222222222";

    when_error_in {

        stat = init();
        check_status(stat, OK, E_INVOPS);

        stat = que_init(&results);
        check_status(stat, OK, E_NOQUEUE);

        stat = rel_open(temp, flags, mode);
        check_return(stat, temp);

        stat = rel_get_recsize(temp, &recsize);
        check_return(stat, temp);

        stat = rel_add(temp, record);
        check_return(stat, temp);

        stat = rel_search(temp, record, compare, capture, &results);
        check_return(stat, temp);

        while ((buffer = que_pop_tail(&results))) {

            printf("result = %*s\n", recsize, buffer);
            free(buffer);

        }

        stat = rel_close(temp);
        check_return(stat, temp);

        exit_when;

    } use {

        capture_error(trace);
        tracer_dump(trace, output_trace);

    } end_when;

    free((void *)buffer);

    err_destroy(errors);
    tracer_destroy(trace);
    rel_destroy(temp);

    return 0;

}

