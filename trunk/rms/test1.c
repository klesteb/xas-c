
#include <stdio.h>

#include "xas/rms/fib.h"

int main(int argc, char **argv) {

    fib_t *temp = NULL;
    char *filename = "fib.pod";
    
    if ((temp = fib_create(filename))) {

        printf("version: %s\n", fib_version(temp));
        fib_destroy(temp);

    }

    return 0;

}

