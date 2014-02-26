# SYNOPSIS
#
#   AC_OGDI([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the ogdi library. If we find them, we set the Makefile
# conditional HAVE_OGDI. We as set OGDI_CFLAGS and OGDI_LIBS
# 
# To allow users to build there own copy of ogdi, we also define
# BUILD_OGDI

AC_DEFUN([AC_OGDI],
[
AC_HANDLE_WITH_ARG([ogdi], [ogdi], [ogdi], $2, $3)
if test "x$want_ogdi" = "xyes"; then
        AC_MSG_CHECKING([for Ogdi library])
        succeeded=no
        if test "$ac_ogdi_path" != ""; then
            OGDI_LIBS="-L$ac_ogdi_path/lib -logdi"
            OGDI_CFLAGS="-I$ac_ogdi_path/include"
            succeeded=yes
        else
	    AC_SEARCH_LIB([OGDI], [ogdi], , [ecs.h], ,
	                  [libogdi], [-logdi])
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(OGDI_CFLAGS)
                AC_SUBST(OGDI_LIBS)
                AC_DEFINE(HAVE_OGDI,,[Defined if we have Ogdi])
                have_ogdi="yes"
        fi
fi
AM_CONDITIONAL([HAVE_OGDI], [test "$have_ogdi" = "yes"])
AM_CONDITIONAL([BUILD_OGDI], [test "$build_ogdi" = "yes"])

AC_CHECK_FOUND([ogdi], [ogdi],[ogdi],$1,$2)
])