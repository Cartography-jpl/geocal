# Stuff for GeoCal.
# We have the top directory for geocal as a variable. This allows the
# full AFIDS system to include this in a subdirectory.

AC_DEFUN([GEOCAL_SOURCE_DIRECTORY],[
AC_SUBST([libgeocal], [${geocal_topdir}/geocal/lib])
AC_SUBST([srcpython], [${geocal_topdir}/python])
AC_SUBST([srcpythonlib], [${srcpython}/lib])
AC_SUBST([srcpythonbin], [${srcpython}/bin])
AC_SUBST([docpythonsrc], [${srcpython}/doc])
AC_SUBST([geocalunittestdata], [${geocal_topdir}/geocal/unit_test_data])
AC_SUBST([pythonunittestdata], [${geocal_topdir}/python/unit_test_data])
AC_SUBST([srcgeocalcore], [$libgeocal/GeoCalCore])
AC_SUBST([srcimplementation], [$srcgeocalcore/Implementation])
AC_SUBST([pythonswigsrc], [${geocal_topdir}/bindings/python])
AC_SUBST([swigsrc], [${geocal_topdir}/bindings/python/swig])
AC_SUBST([srcsupport], [$srcgeocalcore/Support])
AC_SUBST([srcinterface], [$srcgeocalcore/Interface])
AC_SUBST([srccartlab], [$libgeocal/CartLab])
AC_SUBST([srcpythonscript], [${geocal_topdir}/script])
AC_SUBST([srcsdbx], [${geocal_topdir}/thirdparty/libsdbx])
AC_SUBST([srcraw], [${geocal_topdir}/thirdparty/libraw])
AC_SUBST([pythondocdir], [${prefix}/share/doc/afids/python])
AC_SUBST([afidspkgpythondir],[${pythondir}/afids])
AC_SUBST([geocalpkgpythondir], [${platpythondir}/geocal])
AC_SUBST([geocalpkgpyexecdir], [${platpythondir}/geocal])
AC_SUBST([afidspythonvdevdir], [${prefix}/afids/vdev])
AC_SUBST([installafidspythondir], [${prefix}])
])
