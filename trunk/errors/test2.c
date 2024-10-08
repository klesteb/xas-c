
#include <stdio.h>
#include <errno.h>

#include "xas/errors.h"
#include "xas/error_codes.h"

int main(void) {

    char message[1024];
    err_t *errors = err_create();

    err_get_message(errors, E_INVPARM, message, 1023);
    printf("%s\n", message);

    err_get_message(errors, EAGAIN, message, 1023);
    printf("%s\n", message);

    memset(message, '\0', 1024);
    err_remove(errors, EAGAIN);
    err_get_message(errors, EAGAIN, message, 1023);
    printf("%s\n", message);

    err_add(errors, EAGAIN, "EAGAIN", strerror(EAGAIN));
    err_get_message(errors, EAGAIN, message, 1023);
    printf("%s\n", message);

    err_destroy(errors);

    return 0;

}

