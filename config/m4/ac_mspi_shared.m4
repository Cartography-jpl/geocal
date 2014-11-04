# SYNOPSIS
#
#   AC_MSPI_SHARED([required])
#
# DESCRIPTION
#
# This looks for the MSPI-Shared library. If we find it, then we set the
# Makefile conditional HAVE_MSPI_SHARED. We also set MSPI_SHARED_CPPFFLAGS and
# MSPI_SHARED_LIBS. 
#
# Not finding this library might or might not indicate an error. If you
# want an error to occur if we don't find the library, then specify
# "required". Otherwise, leave it as empty and we'll just silently
# return if we don't find the library.
# 

AC_DEFUN([AC_MSPI_SHARED],
[
AC_HANDLE_WITH_ARG([mspi_shared], [mspi-shared], [MSPI-Shared], [cannot_build], [default_search])

if test "x$want_mspi_shared" = "xyes"; then
        AC_MSG_CHECKING([for MSPI-Shared library])
        succeeded=no
        if test "$ac_mspi_shared_path" != ""; then
            MSPI_SHARED_LIBS="-R$ac_mspi_shared_path -L$ac_mspi_shared_path -lMSPI-Shared"
            MSPI_SHARED_CFLAGS="-I$ac_mspi_shared_path -I$ac_mspi_shared_path/.."
            succeeded=yes
        else
          for ac_path_tmp in $prefix/MSPI-Shared $THIRDPARTY/MSPI-Shared /data/smyth/MSPI-Shared ; do
             if test -e "$ac_path_tmp/libMSPI-Shared.la" && test -r "$ac_path_tmp/libMSPI-Shared.la"; then
                MSPI_SHARED_LIBS="-R$ac_path_tmp -L$ac_path_tmp -lMSPI-Shared"
                MSPI_SHARED_CFLAGS="-I$ac_path_tmp -I$ac_path_tmp/.."
                succeeded=yes
                break
             fi
           done
        fi
# We can only use MSPI shared if we also have HDFEOS5
        if test "$have_hdfeos5" != "yes"; then
	   succeeded=no
	fi
        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
		MSPI_SHARED_CFLAGS=""
		MSPI_SHARED_LIBS=""
		have_mspi_shared="no"
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(MSPI_SHARED_CFLAGS)
                AC_SUBST(MSPI_SHARED_BUILD_DEPEND)
                AC_SUBST(MSPI_SHARED_LIBS)
                AC_DEFINE(HAVE_MSPI_SHARED,,[Defined if we have MSPI-Shared])
                have_mspi_shared="yes"
        fi
fi

AM_CONDITIONAL([HAVE_MSPI_SHARED], [test "$have_mspi_shared" = "yes"])
AM_CONDITIONAL([BUILD_MSPI_SHARED], [test "$build_mspi_shared" = "yes"])

AC_CHECK_FOUND([mspi_shared], [mspi-shared],[MSPI-Shared],$1,$2)
])
