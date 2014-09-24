# SYNOPSIS
#
#   AC_HDFEOS5([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the Hdfeos5 library. If we find it, then we set the
# Makefile conditional HAVE_HDFEOS5. We also set HDFEOS5_CFLAGS and
# HDFEOS5_LIBS
#
# To allow users to build there own copy of Hdfeos5, we also define
# BUILD_HDFEOS5.
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

AC_DEFUN([AC_HDFEOS5],
[
AC_HANDLE_WITH_ARG([hdfeos5], [hdfeos5], [Hdfeos5 library], $2, $3)
if test "x$want_hdfeos5" = "xyes"; then
        AC_HDF5($1, $2, default_search)
        AC_MSG_CHECKING([for Hdfeos5 library])
        succeeded=no
        if test "$build_hdfeos5" == "yes"; then
            HDFEOS5_LIBS="libhe5_hdfeos.la ${HDF5_LIBS}"
            HDFEOS5_CFLAGS="-I$srcdir/hdfeos/external/HDF-EOS/v5.1.15/include"
            succeeded=yes
        elif test "$ac_hdfeos5_path" != ""; then
            HDFEOS5_LIBS="-R$ac_hdfeos5_path/lib -L$ac_hdfeos5_path/lib -lhe5_hdfeos -lGctp ${HDF5_LIBS}"
            HDFEOS5_CFLAGS="-I$ac_hdfeos5_path/include/hdfeos5"
            succeeded=yes
        else
	    AC_SEARCH_LIB([HDFEOS5], [hdfeos5], [hdfeos5/], [HE5_HdfEosDef.h], ,
                          [libhe5_hdfeos], [-lhe5_hdfeos -lGctp ${HDF5_LIBS} ])
            if test "$succeeded" != "yes" ; then
 	       AC_SEARCH_LIB([HDFEOS5], [hdfeos5], , [HE5_HdfEosDef.h], ,
                             [libhe5_hdfeos], [-lhe5_hdfeos -lGctp ${HDF5_LIBS}])
            fi
        fi
	if test "$have_hdf5" == "no"; then
            succeeded=no
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                HDFEOS5_CFLAGS="$HDFEOS5_CFLAGS $HDF5_CFLAGS"
                AC_MSG_RESULT([yes])
                AC_SUBST(HDFEOS5_CFLAGS)
                AC_SUBST(HDFEOS5_LIBS)
                have_hdfeos5="yes"
        fi
fi
AM_CONDITIONAL([HAVE_HDFEOS5], [test "$have_hdfeos5" = "yes"])
AM_CONDITIONAL([BUILD_HDFEOS5], [test "$build_hdfeos5" = "yes"])

AC_CHECK_FOUND([hdfeos5], [hdfeos5],[Hdfeos5 library],$1,$2)

])
