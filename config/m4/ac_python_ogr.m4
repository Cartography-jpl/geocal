# SYNOPSIS
#
#   AC_PYTHON_OGR([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the python OGR library
# 

AC_DEFUN([AC_PYTHON_OGR],
[
AC_HANDLE_WITH_ARG([python_ogr], [python-ogr], [python OGR], $2, $3)

if test "x$want_python_ogr" = "xyes"; then
        AC_MSG_CHECKING([for python OGR])
        succeeded=no
	AC_PYTHON_MODULE(ogr)
        if test "$HAVE_PYMOD_OGR" = "yes"; then
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                have_python_ogr="yes"
        fi
fi
AM_CONDITIONAL([HAVE_PYTHON_OGR], [test "$have_python_ogr" = "yes"])
AM_CONDITIONAL([BUILD_PYTHON_OGR], [test "$build_python_ogr" = "yes"])

AC_CHECK_FOUND([python_ogr], [python-ogr],[python OGR],$1,$2)

])