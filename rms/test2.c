
#include <stdio.h>
#include <errno.h>

#include "xas/rms/fib.h"

int main(int argc, char **argv) {

    fib_t *temp = NULL;
    error_trace_t trace;
    char *filename = "files.pod";

    if ((temp = fib_create(filename))) {

        object_set_error1(OBJECT(temp), EAGAIN);
        object_get_error(OBJECT(temp), &trace);

        printf("errno:    %d\n", trace.errnum);
        printf("lineno:   %d\n", trace.lineno);
        printf("file:     %s\n", trace.filename);
        printf("function: %s\n", trace.function);

        fib_destroy(temp);

    }

    return 0;

}

