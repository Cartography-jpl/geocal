# SYNOPSIS
#
#   AC_BOOST([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the BOOST libraries. If we find them, we set the Makefile
# conditional HAVE_BOOST. We as set BOOST_CPPFLAGS, and BOOST_LIBDIR.
#
# Because the libraries can have different names on different systems,
# we also set BOOST_REGEX_LIB and BOOST_DATETIME_LIB that can be used
# on a link line (e.g., it might be "-lboost_regex" on a number of linux
# systems). We can extend this with other boost libraries if needed, but
# right now this is all we use.
# 
# To allow users to build there own copy of BOOST, we also define
# BUILD_BOOST
#
# A particular package might not have the library source code, so you
# can supply the "can_build" argument as "can_build". Empty string means we
# can't build this, and various help messages etc. are adjusted for this.
#
# Not finding this library might or might not indicate an error. If you
# want an error to occur if we don't find the library, then specify
# "required". Otherwise, leave it as empty and we'll just silently
# return if we don't find the library.
# 
# If the user doesn't otherwise specify the "with" argument for this
# library, we can either have a default behavior of searching for the
# library on the system or of building our own copy of it. You can
# specify "default_build" if this should build, otherwise we just look
# for this on the system.

AC_DEFUN([AC_BOOST],
[
AC_HANDLE_WITH_ARG([boost], [boost], [BOOST], $2, $3)

if test "x$want_boost" = "xyes"; then
        AC_MSG_CHECKING([for BOOST library])
        succeeded=no
        if test "$ac_boost_path" != ""; then
            BOOST_CPPFLAGS="-I$ac_boost_path/include"
            succeeded=yes
        else
            for ac_boost_path_tmp in $prefix $THIRDPARTY /opt/afids_support /usr /usr/local /opt /opt/local /sw ; do
                  if test -e "$ac_boost_path_tmp/include/boost/smart_ptr.hpp" && test -r "$ac_boost_path_tmp/include/boost/smart_ptr.hpp"; then
                      ac_boost_path="$ac_boost_path_tmp"
                      BOOST_CPPFLAGS="-I$ac_boost_path_tmp/include"
                      succeeded=yes
                      break;
                  fi
            done
        fi
        if test "$succeeded" = "yes" ; then
            boost_done=no
            if test "$build_boost" = "yes"; then
               BOOST_LIBDIR="$ac_boost_path/lib"
               BOOST_REGEX_LIB="-lboost_regex"
               BOOST_DATETIME_LIB="-lboost_date_time"
               boost_done=yes
            fi
            for ac_boost_lib_base in "$ac_boost_path/lib" "$ac_boost_path/lib64"; do
              if test "$boost_done" = "no"; then
                for ac_boost_lib in libboost_regex.la libboost_regex.so libboost_regex.dylib ; do
                   if test -e "$ac_boost_lib_base/$ac_boost_lib"; then
                       BOOST_LIBDIR="$ac_boost_lib_base"
                       BOOST_REGEX_LIB="-lboost_regex"
                       BOOST_DATETIME_LIB="-lboost_date_time"
                       boost_done=yes
                       break;
                   fi
                done
              fi
              if test "$boost_done" = "no"; then
                for ac_boost_lib in libboost_regex-mt.la libboost_regex-mt.so libboost_regex-mt.dylib ; do
                   if test -e "$ac_boost_lib_base/$ac_boost_lib"; then
                       BOOST_LIBDIR="$ac_boost_lib_base"
                       BOOST_REGEX_LIB="-lboost_regex-mt"
                       BOOST_DATETIME_LIB="-lboost_date_time-mt"
                       boost_done=yes
                       break;
                   fi
                done
              fi
            done
            if test "$boost_done" = "no"; then
                succeeded=no
            fi
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
	        BOOST_CPPFLAGS="$BOOST_CPPFLAGS -DBOOST_TEST_DYN_LINK"
                AC_SUBST(BOOST_CPPFLAGS)
                AC_SUBST(BOOST_LIBDIR)
		AC_SUBST(BOOST_REGEX_LIB)
		AC_SUBST(BOOST_DATETIME_LIB)
                AC_DEFINE(HAVE_BOOST,,[Defined if we have BOOST])
                have_boost="yes"
        fi
fi
AM_CONDITIONAL([HAVE_BOOST], [test "$have_boost" = "yes"])
AM_CONDITIONAL([BUILD_BOOST], [test "$build_boost" = "yes"])

AC_CHECK_FOUND([boost], [boost],[BOOST],$1,$2)
])
