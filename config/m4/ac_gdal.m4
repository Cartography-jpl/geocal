# SYNOPSIS
#
#   AC_GDAL([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the GDAL libraries. If we find them, we set the Makefile
# conditional HAVE_GDAL. We as set GDAL_CFLAGS and GDAL_LIBS
# 
# To allow users to build there own copy of GDAL, we also define
# BUILD_GDAL

AC_DEFUN([AC_GDAL],
[
AC_HANDLE_WITH_ARG([gdal], [gdal], [GDAL], $2, $3)

if test "x$want_gdal" = "xyes"; then
        AC_MSG_CHECKING([for GDAL library])
        succeeded=no
        if test "$ac_gdal_path" != ""; then
            GDAL_LIBS="-R$ac_gdal_path/lib -L$ac_gdal_path/lib -lgdal -logdi -lgeos -lopenjpeg"
            GDAL_CFLAGS="-I$ac_gdal_path/include"
            succeeded=yes
        else
	    AC_SEARCH_LIB([GDAL], [gdal], , [gdal.h], , [libgdal], [-lgdal])
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(GDAL_CFLAGS)
                AC_SUBST(GDAL_LIBS)
                AC_DEFINE(HAVE_GDAL,,[Defined if we have GDAL])
                have_gdal="yes"
        fi
fi
if test "$build_gdal" = "yes"; then
  AC_GEOS(required, $2, default_search)
  AC_OGDI(required, $2, default_search)
else # Not building GDAL
  AM_CONDITIONAL([HAVE_GEOS], [false])
  AM_CONDITIONAL([HAVE_OGDI], [false])
  AM_CONDITIONAL([BUILD_GEOS], [false])
  AM_CONDITIONAL([BUILD_OGDI], [false])
  build_geos="no"
  build_ogdi="no"
fi # End if/else building GDAL
AM_CONDITIONAL([HAVE_GDAL], [test "$have_gdal" = "yes"])
AM_CONDITIONAL([BUILD_GDAL], [test "$build_gdal" = "yes"])

AC_CHECK_FOUND([gdal], [gdal],[Gdal],$1,$2)

])