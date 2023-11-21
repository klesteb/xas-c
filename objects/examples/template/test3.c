
#include <stdio.h>
#include <errno.h>

#include "template.h"

void callback(error_trace_t *error) {
    
    printf("errno:    %d\n", error->errnum);
    printf("lineno:   %d\n", error->lineno);
    printf("file:     %s\n", error->filename);
    printf("function: %s\n", error->function);

}

int main(int argc, char **argv) {

    template_t *temp = NULL;

    if ((temp = template_create(NULL))) {

        object_set_trace(OBJECT(temp), callback);
        object_set_error1(OBJECT(temp), EAGAIN);

        template_destroy(temp);

    }

    return 0;

}

