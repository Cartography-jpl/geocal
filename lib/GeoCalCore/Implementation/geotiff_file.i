// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geotiff_file.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::GeotiffFile);
namespace GeoCal {
class GeotiffFile : public GenericObject {
public:
  typedef unsigned short geocode_t;
  // Note that all of these hard coded values are actually registered in
  // the GeoTIFF standard. This should mean that they are fairly stable
  // and unlikely to change.
  typedef enum {
    GTModelTypeGeoKey = 1024,
    GTRasterTypeGeoKey = 1025,
    GTCitationGeoKey = 1026,
    GeographicTypeGeoKey = 2048,
    GeogCitationGeoKey = 2049,
    GeogGeodeticDatumGeoKey = 2050,
    GeogPrimeMeridianGeoKey = 2051,
    GeogLinearUnitsGeoKey = 2052,
    GeogLinearUnitSizeGeoKey = 2053,
    GeogAngularUnitsGeoKey = 2054,
    GeogAngularUnitSizeGeoKey = 2055,
    GeogEllipsoidGeoKey = 2056,
    GeogSemiMajorAxisGeoKey = 2057,
    GeogSemiMinorAxisGeoKey = 2058,
    GeogInvFlatteningGeoKey = 2059,
    GeogAzimuthUnitsGeoKey = 2060,
    GeogPrimeMeridianLongGeoKey = 2061,
    ProjectedCSTypeGeoKey = 3072,
    PCSCitationGeoKey = 3073,
    ProjectionGeoKey = 3074,
    ProjCoordTransGeoKey = 3075,
    ProjLinearUnitsGeoKey = 3076,
    ProjLinearUnitSizeGeoKey = 3077,
    ProjStdParallel1GeoKey = 3078,
    ProjStdParallelGeoKey = ProjStdParallel1GeoKey,
    ProjStdParallel2GeoKey = 3079,
    ProjNatOriginLongGeoKey = 3080,
    ProjOriginLongGeoKey = ProjNatOriginLongGeoKey,
    ProjNatOriginLatGeoKey = 3081,
    ProjOriginLatGeoKey = ProjNatOriginLatGeoKey,
    ProjFalseEastingGeoKey = 3082,
    ProjFalseNorthingGeoKey = 3083,
    ProjFalseOriginLongGeoKey = 3084,
    ProjFalseOriginLatGeoKey = 3085,
    ProjFalseOriginEastingGeoKey = 3086,
    ProjFalseOriginNorthingGeoKey = 3087,
    ProjCenterLongGeoKey = 3088,
    ProjCenterLatGeoKey = 3089,
    ProjCenterEastingGeoKey = 3090,
    ProjCenterNorthingGeoKey = 3091,
    ProjScaleAtNatOriginGeoKey = 3092,
    ProjScaleAtOriginGeoKey = ProjScaleAtNatOriginGeoKey,
    ProjScaleAtCenterGeoKey = 3093,
    ProjAzimuthAngleGeoKey = 3094,
    ProjStraightVertPoleLongGeoKey = 3095,
    ProjRectifiedGridAngleGeoKey = 3096,
    VerticalCSTypeGeoKey = 4096,
    VerticalCitationGeoKey = 4097,
    VerticalDatumGeoKey = 4098,
    VerticalUnitsGeoKey = 4099
  } geokey_t;
  typedef enum {
    TYPE_BYTE=1,
    TYPE_SHORT=2,
    TYPE_LONG=3,
    TYPE_RATIONAL=4,
    TYPE_ASCII=5,
    TYPE_FLOAT=6,
    TYPE_DOUBLE=7,
    TYPE_SBYTE=8,
    TYPE_SSHORT=9,
    TYPE_SLONG=10,
    TYPE_UNKNOWN=11
  } tagtype_t;
  typedef uint32_t ttag_t;
  GeotiffFile(const std::string& Fname, const std::string& Mode);
  %python_attribute(file_name, std::string);
  %python_attribute(mode, std::string);
  std::string print_to_string() const;
  static tagtype_t key_type(geokey_t K);
  static const std::vector<int>& geotiff_tag_ascii();
  static const std::vector<int>& geotiff_tag_double();
  static const std::vector<int>& geotiff_tag_short();
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("GeotiffFile")
