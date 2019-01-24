# SYNOPSIS
#
#   AC_MSP([required])
#
# DESCRIPTION
#
# This looks for the MSP libraries. If we find them, we set the Makefile
# conditional HAVE_MSP. We as set MSP_CFLAGS and MSP_LIBS
# 
# Not finding this library might or might not indicate an error. If you
# want an error to occur if we don't find the library, then specify
# "required". Otherwise, leave it as empty and we'll just silently
# return if we don't find the library.

AC_DEFUN([AC_MSP],
[
# Guard against running twice
if test "x$done_msp" = "x"; then
AC_HANDLE_WITH_ARG([msp], [msp], [MSP], [cannot_build], [default_search], $1)
have_msp="no"
if test "x$want_msp" = "xyes"; then
        AC_MSG_CHECKING([for MSP library])
        succeeded=no
        if test "$ac_msp_path" != ""; then
            MSP_LIBS="-L$ac_msp_path/lib -R$ac_msp_path/lib -ldl -lMSPCoordinateConversionService -lMSPCovarianceService -lMSPImagingGeometryService -lMSPMensurationService -lMSPMensurationSessionRecordService -lMSPPointExtractionService -lMSPOutputMethodService -lMSPSensorModelService -lMSPSensorSpecificService -lMSPSupportDataService -lMSPTerrainService -lMSPasdetre -lMSPCCSUtils -lMSPcoordconverter -lMSPcsisd -lMSPcsm -lMSPcsmutil -lMSPDEIUtil -lMSPgeometry -lMSPjson -lMSPlas -lMSPmath -lMSPnitf -lMSPntmtre -lMSPrage -lMSPRageServiceUtils -lMSPrageutilities -lMSPSScovmodel -lMSPSSrutil -lMSPutilities -lMSPmiisd -lCSM_PCAPI"
            MSP_CFLAGS="-I$ac_msp_path/include -I$ac_msp_path/CCSUtils -I$ac_msp_path/CoordinateConversion -I$ac_msp_path/Covariance -I$ac_msp_path/PointExtraction -I$ac_msp_path/ImagingGeometry -I$ac_msp_path/Mensuration -I$ac_msp_path/MensurationSessionRecord -I$ac_msp_path/OutputMethod -I$ac_msp_path/SensorModel -I$ac_msp_path/SupportData -I$ac_msp_path/Terrain -I$ac_msp_path/common -I$ac_msp_path/common/csm -I$ac_msp_path/common/csmutil -I$ac_msp_path/common/dtcc -I$ac_msp_path/common/geometry -I$ac_msp_path/common/math -I$ac_msp_path/common/ntmtre -I$ac_msp_path/common/utilities"
	    MSP_PREFIX=$ac_msp_path
            succeeded=yes
        else
          for ac_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /data/smyth/MSP/install ; do
             if test -e "$ac_path_tmp/lib/libMSPnitf.so" && test -r "$ac_path_tmp/lib/libMSPnitf.so"; then
 	     	MSP_LIBS="-L$ac_path_tmp/lib -R$ac_path_tmp/lib -ldl -lMSPCoordinateConversionService -lMSPCovarianceService -lMSPImagingGeometryService -lMSPMensurationService -lMSPMensurationSessionRecordService -lMSPPointExtractionService -lMSPOutputMethodService -lMSPSensorModelService -lMSPSensorSpecificService -lMSPSupportDataService -lMSPTerrainService -lMSPasdetre -lMSPCCSUtils -lMSPcoordconverter -lMSPcsisd -lMSPcsm -lMSPcsmutil -lMSPDEIUtil -lMSPgeometry -lMSPjson -lMSPlas -lMSPmath -lMSPnitf -lMSPntmtre -lMSPrage -lMSPRageServiceUtils -lMSPrageutilities -lMSPSScovmodel -lMSPSSrutil -lMSPutilities -lMSPmiisd -lCSM_PCAPI"
		MSP_CFLAGS="-I$ac_path_tmp/include -I$ac_path_tmp/CCSUtils -I$ac_path_tmp/CoordinateConversion -I$ac_path_tmp/Covariance -I$ac_path_tmp/PointExtraction -I$ac_path_tmp/ImagingGeometry -I$ac_path_tmp/Mensuration -I$ac_path_tmp/MensurationSessionRecord -I$ac_path_tmp/OutputMethod -I$ac_path_tmp/SensorModel -I$ac_path_tmp/SupportData -I$ac_path_tmp/Terrain -I$ac_path_tmp/common -I$ac_path_tmp/common/csm -I$ac_path_tmp/common/csmutil -I$ac_path_tmp/common/dtcc -I$ac_path_tmp/common/geometry -I$ac_path_tmp/common/math -I$ac_path_tmp/common/ntmtre -I$ac_path_tmp/common/utilities"
		MSP_PREFIX=$ac_path_tmp
                succeeded=yes
                break
             fi
           done
        fi
        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
		MSP_CFLAGS=""
		MSP_LIBS=""
		have_msp="no"
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(MSP_CFLAGS)
                AC_SUBST(MSP_LIBS)
                AC_SUBST(MSP_PREFIX)
                AC_DEFINE(HAVE_MSP,,[Defined if we have MSP])
                have_msp="yes"
        fi
fi
AC_SUBST(have_msp)
AM_CONDITIONAL([HAVE_MSP], [test "$have_msp" = "yes"])
AM_CONDITIONAL([BUILD_MSP], [test "$build_msp" = "yes"])

AC_CHECK_FOUND([msp], [msp],[MSP],$1,[cannot_build])

done_msp="yes"
fi
])
