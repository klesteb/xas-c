
# Macro file for use with GNU AutoTools for generating config files and
# Makefiles. This file was manually created based on information found
# via Google and the GNU documentation. Although it works for the source
# code it's included with, it may not be 100% correct so additional 
# research/tweaking may be needed.

# The lib_LIBRARIES/lib_LTLIBRARIES  macro tells the autotools the name
# and type of library we want to build - in this case we are building
# both a shared and static library. A shared library ends with '.la' 
# instead of '.a' which is for a non-shared (static) library.
lib_LTLIBRARIES = libxasgpl.la

# This specially named variable contains a space delimited list of the
# source code files that must be compiled in order to create the library. 
# The variable name is in the format:
# <library_name>_<library_type>_SOURCES
# Where:
# <library_name> = the name of the library specified in lib_LIBRARIES
# <library_type> = either 'a' for non-shared library or 'la' for shared.
libxasgpl_la_SOURCES = crlf/crlf2nl.c crlf/nl2crlf.c crlf/nlcount.c \
drs/drs_core.c drs/drs_count.c drs/drs_create.c drs/drs_destroy.c \
drs/drs_first.c drs/drs_get.c drs/drs_next.c fnm/fnm_build.c \
fnm/fnm_destroy.c fnm/fnm_unix2vms.c fnm/fnm_core.c fnm/fnm_exists.c \
fnm/fnm_vms2unix.c get/get_arg.c get/get_field.c get/get_string.c \
get/get_word.c hash/hash_add.c hash/hash_delete.c hash/hash_search.c \
hash/hash_core.c hash/hash_destroy.c hash/hash_set_debug.c \
hash/hash_count.c hash/hash_dump.c hash/hash_statistics.c hash/hash_create.c \
hash/hash_get.c lfn/lfn_create.c lfn/lfn_getline.c lfn/lfn_iswriteable.c \
lfn/lfn_read.c lfn/lfn_destroy.c lfn/lfn_isreadable.c lfn/lfn_name.c \
lfn/lfn_write.c lfn/lfn_fd.c lfn/lfn_isup.c lfn/lfn_putline.c \
meo/meo_core.c meo/meo_load.c meo/meo_dump.c meo/meo_save.c \
misc/vperror.c net/net_addrof.c net/net_hostof.c net/net_portof.c \
nix/nxaddinput.c nix/nxmainloop.c nix/nxremoveworkproc.c \
nix/nxaddtimeout.c nix/nxmainloopef.c nix/nxsetdebug.c nix/nxaddworkproc.c \
nix/nxremoveinput.c nix/nxcreatecontext.c nix/nxremovetimeout.c \
opt/opt_core.c opt/opt_get.c opt/opt_reset.c opt/opt_create_argv.c \
opt/opt_index.c opt/opt_set.c opt/opt_delete_argv.c opt/opt_init.c \
opt/opt_term.c opt/opt_errors.c opt/opt_name.c \
port/port_core.c port/port_create.c port/port_destroy.c \
rex/rex_util.c rex/rex_utils_y.c \
skt/skt_isreadable.c skt/skt_iswriteable.c skt/skt_port.c skt/skt_isup.c \
skt/skt_peer.c skt/skt_setbuf.c str/strcasecmp.c str/str_detab.c \
str/str_match.c str/str_toupper.c str/str_cat.c str/strdup.c \
str/strncasecmp.c str/str_trim.c str/str_copy.c str/str_env.c \
str/strndup.c str/str_core.c str/str_etoa.c str/str_remove.c \
str/str_destring.c str/str_insert.c str/str_tolower.c \
tcp/tcp_answer.c tcp/tcp_isreadable.c tcp/tcp_read.c \
tcp/tcp_call.c tcp/tcp_isup.c tcp/tcp_request_pending.c \
tcp/tcp_complete.c tcp/tcp_iswriteable.c tcp/tcp_destroy.c \
tcp/tcp_listen.c tcp/tcp_write.c tcp/tcp_fd.c tcp/tcp_name.c \
tv/tv_add.c tv/tv_createf.c tv/tv_subtract.c tv/xdr_timeval.c \
tv/tv_compare.c tv/tv_float.c tv/tv_tod.c tv/tv_create.c \
tv/tv_show.c tv/tv_truncate.c xqt/xqt_fd.c xqt/xqt_poll.c \
xqt/xqt_close.c xqt/xqt_open.c xqt/xqt_read.c xqt/xqt_write.c

libxasgpl_la_LDFLAGS = -version-info 1:0:0

# The AM_CPPFLAGS macro allows us to tell the tools where needed header
# files are located if they aren't in the default paths. In this case it's
# a subdirectory in the source code directory where all the header files 
# are centrally located.
AM_CPPFLAGS = -I../../include

