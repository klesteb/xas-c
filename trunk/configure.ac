
# Macro file for use with GNU AutoTools for generating config files and
# Makefiles. This file was manually created based on information found
# via Google and the GNU documentation. Although it works for the source
# code it's included with, it may not be 100% correct so additional 
# research/tweaking may be needed.

# AC_INIT takes two parameters, the package name and a version number.
AC_INIT([xas-c], [1.0])

# AC_CONFIG_SRCDIR takes one parameter, the path and filename
# for one of the source code files.
#AC_CONFIG_SRCDIR([src/xas-c.c])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL
AC_PROG_CC
LT_INIT
CFLAGS=
LDFLAGS=
LIBS=

# AC_OUTPUT takes a space delimited list of where the Makefiles are to
# be created. You need to pass all directories where there is source code
# including the base directory containing all of the source code (which won't
# need a path pre-pended to the 'Makefile' keyword).
AC_OUTPUT(Makefile 
          src/Makefile
          src/objects/Makefile
          src/errors/Makefile 
          src/events/Makefile
          src/logger/Makefile
          src/messages/Makefile
          src/misc/Makefile
          src/queue/Makefile
          src/rms/Makefile)

