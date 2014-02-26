
AC_INIT(sdbx, 0.1, dustin@virtualroadside.com)
AC_CONFIG_SRCDIR([sdbx/DiskTreeBackend.h])
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE



AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_LANG([C++])



AM_OPTIONS_WXCONFIG
reqwx=2.6.2
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi
 

LIBS="$LIBS $WX_LIBS"

echo -n "Checking for --enable-debug... "
AC_ARG_ENABLE([debug],[AS_HELP_STRING([--enable-debug],[add debugging information to files])])

if test "x$enable_debug" == "xyes" 
then
    CPPFLAGS="$CPPFLAGS -Wall -g -ggdb -DDEBUG $WX_CPPFLAGS"
    CXXFLAGS="$CXXFLAGS -Wall -g -ggdb -DDEBUG $WX_CXXFLAGS_ONLY"
    CFLAGS="$CFLAGS -Wall -g -ggdb -DDEBUG $WX_CFLAGS_ONLY"
    echo "yes"
else
	CPPFLAGS="$CPPFLAGS -Wall -DNDEBUG $WX_CPPFLAGS"
    CXXFLAGS="$CXXFLAGS -Wall -DNDEBUG $WX_CXXFLAGS_ONLY"
    CFLAGS="$CFLAGS -DNDEBUG $WX_CFLAGS_ONLY"
    echo "no"
fi



AC_CONFIG_FILES([
	Makefile
	tests/Makefile
	win32/Makefile
])

AC_OUTPUT

