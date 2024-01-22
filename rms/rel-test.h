
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2024 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _REL_TEST_H
#define _REL_TEST_H

/* 
 * constants used with the rel tests
 */

char *values[26] = {
    "aaaaaaaaaaaaaaaaaaaaaaaaaa",
    "bbbbbbbbbbbbbbbbbbbbbbbbbb",
    "cccccccccccccccccccccccccc",
    "dddddddddddddddddddddddddd",
    "eeeeeeeeeeeeeeeeeeeeeeeeee",
    "ffffffffffffffffffffffffff",
    "gggggggggggggggggggggggggg",
    "hhhhhhhhhhhhhhhhhhhhhhhhhh",
    "iiiiiiiiiiiiiiiiiiiiiiiiii",
    "jjjjjjjjjjjjjjjjjjjjjjjjjj",
    "kkkkkkkkkkkkkkkkkkkkkkkkkk",
    "llllllllllllllllllllllllll",
    "mmmmmmmmmmmmmmmmmmmmmmmmmm",
    "nnnnnnnnnnnnnnnnnnnnnnnnnn",
    "oooooooooooooooooooooooooo",
    "pppppppppppppppppppppppppp",
    "qqqqqqqqqqqqqqqqqqqqqqqqqq",
    "rrrrrrrrrrrrrrrrrrrrrrrrrr",
    "ssssssssssssssssssssssssss",
    "tttttttttttttttttttttttttt",
    "uuuuuuuuuuuuuuuuuuuuuuuuuu",
    "vvvvvvvvvvvvvvvvvvvvvvvvvv",
    "wwwwwwwwwwwwwwwwwwwwwwwwww",
    "xxxxxxxxxxxxxxxxxxxxxxxxxx",
    "yyyyyyyyyyyyyyyyyyyyyyyyyy",
    "zzzzzzzzzzzzzzzzzzzzzzzzzz",
};

int records = 26;
int retries = 30;
int timeout = 30;
char *path = "";
char *filename = "testing.dat";
int recsize = 26;

int flags = O_RDWR;
int mode = (S_IRWXU | S_IRWXG);

#endif

