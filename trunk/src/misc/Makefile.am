
# Macro file for use with GNU AutoTools for generating config files and
# Makefiles. This file was manually created based on information found
# via Google and the GNU documentation. Although it works for the source
# code it's included with, it may not be 100% correct so additional 
# research/tweaking may be needed.

# The lib_LIBRARIES/lib_LTLIBRARIES  macro tells the autotools the name
# and type of library we want to build - in this case we are building
# both a shared and static library. A shared library ends with '.la' 
# instead of '.a' which is for a non-shared (static) library.
lib_LTLIBRARIES = libxasmisc.la

# This specially named variable contains a space delimited list of the
# source code files that must be compiled in order to create the library. 
# The variable name is in the format:
# <library_name>_<library_type>_SOURCES
# Where:
# <library_name> = the name of the library specified in lib_LIBRARIES
# <library_type> = either 'a' for non-shared library or 'la' for shared.
libxasmisc_la_SOURCES = crc.c dates.c getche.c hash.c hexdump.c insert.c left.c ltostr.c ltrim.c mid.c pos.c right.c rtrim.c seg.c soundex.c spaces.c stricmp.c string.c stripcr.c strlwr.c strnicmp.c strrev.c strupr.c sys_date.c trim.c wordwrap.c xgetch.c xgetline.c xindex.c xinstr.c xmalloc.c xstrtok.c
libxasmisc_la_LDFLAGS = -version-info 1:0:0

# The AM_CPPFLAGS macro allows us to tell the tools where needed header
# files are located if they aren't in the default paths. In this case it's
# a subdirectory in the source code directory where all the header files 
# are centrally located.
AM_CPPFLAGS = -I../../include

