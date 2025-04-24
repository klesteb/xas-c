
#include <stdio.h>
#include "xas/rms/lf.h"

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t count;
    int exists = 0;
    struct stat buf;
    char buffer[101];
    lf_t *temp = NULL;
    int flags = O_RDONLY;
    char *filename = "lf.pod";

    if ((temp = lf_create(filename))) {

        lf_exists(temp, &exists);
        if (exists) {

            lf_stat(temp, &buf);
            printf("file size = %ld\n", buf.st_size);
            printf("file mode = %ud\n", buf.st_mode);

            if (lf_open(temp, flags, 0) == OK) {

                do {

                    stat = lf_gets(temp, buffer, 100, &count);
                    if (stat == OK) {

                        printf("%ld: %s\n", count, buffer);

                    }

                } while ((stat == OK) && (count > 0));

                lf_close(temp);

            } else {

                printf("unable to open %s\n", filename);

            }

        } else {

            printf("unable to find %s\n", filename);

        }

        lf_destroy(temp);

    }

    return 0;

}

