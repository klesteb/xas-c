
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2023 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _XAS_TYPES_H_
#define _XAS_TYPES_H_

/* 
 * Create some new data types. Mainly for easier reading and 
 * portability concerns.
 */

typedef char               BYTE;
typedef unsigned char      UBYTE;
typedef short int          WORD;
typedef unsigned short int UWORD;
typedef long               LONG;
typedef unsigned long      ULONG;
typedef unsigned char *    STRPTR;    /* long live the AMIGA!       */

typedef unsigned short      ushort;   /* must be 16 bits wide       */
typedef unsigned long       ulong;    /* must be 32 bits wide       */
typedef unsigned char       uchar;    /* must be  8 bits wide       */
typedef unsigned int        uint;
typedef unsigned long long	uid;

#ifndef unix
typedef unsigned long long	off64_t;
#endif

/* 
 * Create some constants, This may be a little heavy handed, but it
 * works for ncurses, so why not.
 */

#undef  ERR
#define ERR (-1)
#undef  OK
#define OK  (0)

#undef TRUE
#define TRUE    1
#undef FALSE
#define FALSE   0

/* 
 * A data structure used to pass multiple parameters.
 * 
 * This was derived from the OpenVMS item_list_3 data structure. Where 
 * it is used to pass parameters and return data from System Services. 
 */

typedef struct _item_list_s {
    UWORD buffer_length;       /* the length of the buffer            */
    UWORD item_code;           /* the data type nemonic               */
    void *buffer_address;      /* the address of the buffer           */
    void *return_address;      /* the length of a returned buffer     */
} item_list_t;

/* A macro to help fill in the fields of the above data structure */

#define SET_ITEM(x,a,b,c,d) {(x).item_code=(a);\
                             (x).buffer_address=(b);\
                             (x).buffer_length=(c);\
                             (x).return_address=(d);}

/* 
 * A data structure to maintain error information. This is used to give
 * a consitent view of the information.
 */

typedef struct _error_code_s {
    int errnum;                /* the error number                    */
    char *text;                /* the nemonic for the error number    */
    char *message;             /* message text to explain the error   */
} error_code_t;

/* 
 * A data structure used to keep track of errors. This is used to
 * keep track of errors and where they happened. Most of the information
 * is supplied by the preprocessor. Nothing magical.
 */

typedef struct _error_trace_s {
    int errnum;                 /* the error number                   */
    int lineno;                 /* the line number in the source file */ 
    char *filename;             /* the source file name               */
    char *function;             /* the function name                  */
} error_trace_t;

/* 
 * A data structure to maintain message information. This is used to give
 * a consitent view of the information.
 */

typedef struct _message_code_s {
    int msgnum;                /* the message number                  */
    char *text;                /* the nemonic for the error number    */
} message_code_t;

#endif

