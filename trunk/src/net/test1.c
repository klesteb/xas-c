
#include <stdio.h>
#include "xas/net/uds.h"

int main(int argc, char **argv) {

    uds_t *temp = NULL;

    if ((temp = uds_create(NULL))) {

        printf("version = %s\n", uds_version(temp));
        uds_destroy(temp);

    }

    return 0;

}

