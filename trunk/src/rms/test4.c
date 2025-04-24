
#include <stdio.h>
#include "xas/rms/lf.h"

int main(int argc, char **argv) {

    int x = 0;
    int stat = OK;
    ssize_t count;
    lf_t *temp = NULL;
    char *filename = "junk.txt";
    char *text = "this is a text line";
    int mode = S_IRWXU | S_IRWXG | S_IROTH;
    int flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((temp = lf_create(filename))) {

        if (lf_open(temp, flags, mode) == OK) {

            for (x = 0; x < 10; x++) {

                stat = lf_puts(temp, text, &count);
                if (stat != OK) break;

            }

            lf_close(temp);

        } else {

            printf("unable to open %s\n", filename);

        }

        lf_destroy(temp);

    }

    return 0;

}

