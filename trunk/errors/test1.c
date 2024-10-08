
#include <stdio.h>
#include <errno.h>
#include "xas/errors.h"
#include "xas/error_codes.h"

int main(void) {

    char text[1024];
    char message[1024];
    err_t *errors = err_create();

    err_get_message(errors, E_INVPARM, message, 1023);
    printf("%s\n", message);

    err_get_text(errors, EAGAIN, text, 1023);
    err_get_message(errors, EAGAIN, message, 1023);
    printf("%s: %s\n", text, message);

    err_destroy(errors);

    return 0;

}

