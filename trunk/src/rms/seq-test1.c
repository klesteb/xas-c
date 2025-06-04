
#include <stdio.h>
#include "xas/rms/seq.h"

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t count;
    int exists = 0;
    struct stat buf;
    char buffer[101];
    seq_t *temp = NULL;
    int flags = O_RDONLY;
    char *filename = "seq.pod";

    if ((temp = seq_create(filename))) {

        seq_exists(temp, &exists);
        if (exists) {

            seq_stat(temp, &buf);
            printf("file size = %ld\n", buf.st_size);
            printf("file mode = %ud\n", buf.st_mode);

            if (seq_open(temp, flags, 0) == OK) {

                do {

                    stat = seq_gets(temp, buffer, 100, &count);
                    if (stat == OK) {

                        printf("%ld: %s\n", count, buffer);

                    }

                } while ((stat == OK) && (count > 0));

                seq_close(temp);

            } else {

                printf("unable to open %s\n", filename);

            }

        } else {

            printf("unable to find %s\n", filename);

        }

        seq_destroy(temp);

    }

    return 0;

}

