
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

/*----------------------------------------------------------------------*/

int xgetch(void) {
/*
 * Function: xgetch.c
 * Version : 1.0
 * Created : 08-Sep-2021
 * Author  : Kevin Esteb
 * 
 * Description
 * 
 *     This function is for portibility with Borland C++ and GCC. I 
 *     renamed this for conflicts with ncurses getch().
 * 
 *     Return the key pressed on the terminal.
 * 
 * Modification History
 * 
 * Variables Used
 * 
 */

    int ch;
    struct termios oldattr;
    struct termios newattr;

/*
 * Main part of function.
 */

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;

}

