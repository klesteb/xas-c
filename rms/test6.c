
#include <stdio.h>

#include "xas/error_handler.h"
#include "xas/rms/lf.h"

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t count;
    char buffer[101];
    char filename[256];
    lf_t *temp = NULL;
    int flags = O_RDONLY;

    if (argc < 2) {

        printf("Usage: test6 <filename>\n");
        return 0;

    }

    strcpy(filename, argv[1]);

    when_error_in {

        temp = lf_create(filename);
        check_creation(temp);

        stat = lf_open(temp, flags, 0);
        check_return(stat, temp);

        do {

            stat = lf_gets(temp, buffer, 100, &count);
            check_return(stat, temp);

            printf("%ld: %s\n", count, buffer);

        } while (count > 0);

        stat = lf_close(temp);
        check_return(stat, temp);

        stat = lf_destroy(temp);
        check_return(stat, temp);

        exit_when;

    } use {

        printf("Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

        clear_error();

    } end_when;

    return 0;

}

