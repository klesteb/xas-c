
/*---------------------------------------------------------------------------*/
/*                   Copyright (c) 2024 by Kevin L. Esteb                    */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

/*                                                                           */
/* taken from: https://wiki.c2.com/?HexDumpInManyProgrammingLanguages        */
/* the example provided by: Layne Thomas                                     */
/* modified as needed.                                                       */
/*                                                                           */
/* usage: hexdump(buffer, strlen(buffer), width);                            */
/*                                                                           */

#include <stdio.h>

/*---------------------------------------------------------------------------*/

void hexdump(unsigned char *buffer, unsigned long index, unsigned long width) {

    unsigned long i = 0;
    unsigned long spacer = 0;

    for (i = 0; i < index; i++) {

        fprintf(stderr, "%02x ", buffer[i]);

    }

    for (spacer = index; spacer < width; spacer++) {

        fprintf(stderr, "        ");

    }

    fprintf(stderr, ": ");

    for (i = 0; i < index; i++) {

        if (buffer[i] < 32) {

            fprintf(stderr, ".");

        } else fprintf(stderr, "%c", buffer[i]);

    }

    fprintf(stderr, "\n");

}

