
# Macro file for use with GNU AutoTools for generating config files and
# Makefiles. This file was manually created based on information found
# via Google and the GNU documentation. Although it works for the source
# code it's included with, it may not be 100% correct so additional 
# research/tweaking may be needed.

# The lib_LIBRARIES/lib_LTLIBRARIES  macro tells the autotools the name
# and type of library we want to build - in this case we are building
# both a shared and static library. A shared library ends with '.la' 
# instead of '.a' which is for a non-shared (static) library.
lib_LTLIBRARIES = libxaswidgets.la

# This specially named variable contains a space delimited list of the
# source code files that must be compiled in order to create the library. 
# The variable name is in the format:
# <library_name>_<library_type>_SOURCES
# Where:
# <library_name> = the name of the library specified in lib_LIBRARIES
# <library_type> = either 'a' for non-shared library or 'la' for shared.
libxaswidgets_la_SOURCES = colors.c component.c widget.c window.c \
workbench.c components/bell.c components/hline.c components/input.c \
components/label.c components/more.c components/prompt.c \
components/textarea.c components/text.c components/vline.c \
components/yesno.c components/menus/bar.h components/menus/box.c \
components/menus/list.c components/menus/menus.c windows/alert.c \
windows/bar_menu.c windows/base_window.c \
windows/box_menu.c windows/list_menu.c \
windows/query_window.c windows/status.c

libxaswidgets_la_LDFLAGS = -version-info 1:0:0

# The AM_CPPFLAGS macro allows us to tell the tools where needed header
# files are located if they aren't in the default paths. In this case it's
# a subdirectory in the source code directory where all the header files 
# are centrally located.
AM_CPPFLAGS = -I../../include

