
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
        capture_for_tracer(trace);

    } end_when;

    return stat;

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error_in {

        stat = init();
        check_status(stat, OK, E_INVOPS);

        stat = rel_open(temp, flags, mode);
        check_return(stat, temp);

        int x = 0;
        for (; x < 26; x++) {

            stat = rel_add(temp, values[x]);
            check_return(stat, temp);
            printf("after rel_add()\n");

        }

        stat = rel_close(temp);
        check_return(stat, temp);

        exit_when;

    } use {

        capture_for_tracer(trace);
        tracer_dump(trace, output_trace);

    } end_when;

    err_destroy(errors);
    tracer_destroy(trace);
    rel_destroy(temp);

    return 0;

}

