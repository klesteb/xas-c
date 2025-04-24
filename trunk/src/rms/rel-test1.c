
#include <stdio.h>

#include "xas/rms/rel.h"
#include "rel-test.h"

int main(int argc, char **argv) {

    rel_t *temp = NULL;
    
    if ((temp = rel_create(path, filename, records, recsize, retries, timeout))) {

        printf("version: %s\n", rel_version(temp));
        rel_destroy(temp);

    }

    return 0;

}

