#include "geotiff_file.h"
#include "geocal_serialize_support.h"
#include "geocal_exception.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GeotiffFile::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void GeotiffFile::load(Archive & ar, const unsigned int version)
{
  init();
}

template<class Archive>
void GeotiffFile::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GeotiffFile);
  ar & GEOCAL_NVP_(fname)
    & GEOCAL_NVP_(mode);
  boost::serialization::split_member(ar, *this, version);
}

GEOCAL_IMPLEMENT(GeotiffFile);
#endif

// Note that all of these hard coded values are actually registered in
// the TIFF standard. This should mean that they are fairly stable
// and unlikely to change.

#define TIFFTAG_GEOPIXELSCALE       33550
#define TIFFTAG_GEOTIEPOINTS        33922
#define TIFFTAG_GEOTRANSMATRIX      34264
#define	TIFFTAG_IMAGEWIDTH		256
#define	TIFFTAG_IMAGELENGTH		257
#define	TIFFTAG_COMPRESSION		259
#define	TIFFTAG_PLANARCONFIG		284
#define	TIFFTAG_PHOTOMETRIC		262
#define	PHOTOMETRIC_MINISBLACK	        1
#define	COMPRESSION_NONE		1
#define	PLANARCONFIG_CONTIG		1
#define	TIFFTAG_BITSPERSAMPLE		258
#define	TIFFTAG_SAMPLESPERPIXEL		277
extern "C" {
typedef int geokey_t;
typedef int tagtype_t;
typedef uint32_t ttag_t;
TIFF * XTIFFOpen(const char* name, const char* mode);
void XTIFFClose(TIFF *tif);
GTIF *GTIFNew(void *tif);
void GTIFFree(GTIF *gtif);
int  GTIFWriteKeys(GTIF *gtif);
int GTIFKeyInfo(GTIF *gtif, geokey_t key, int *size, tagtype_t* type);
int GTIFKeyGet(GTIF *gtif, geokey_t key, void *val, int index, int count);
int GTIFKeySet(GTIF *gtif, geokey_t keyID, tagtype_t type, int count,...);
char *GTIFKeyName(geokey_t key);
char *GTIFValueName(geokey_t key,int value);
int TIFFSetField(TIFF*, ttag_t, ...);
int TIFFGetField(TIFF*, ttag_t, ...);
int32_t TIFFWriteEncodedStrip(TIFF*, uint32_t, void*, int32_t);
}

void GeotiffFile::init()
{
  tif = 0;
  gtif = 0;
  tif = XTIFFOpen(fname_.c_str(), mode_.c_str());
  if(!tif) {
    Exception e;
    e << "Trouble opening TIFF file " << fname_;
    throw ;
  }
  gtif = GTIFNew(tif);
  if(!gtif) {
    Exception e;
    e << "Trouble calling GTIFNew for file " << fname_;
    throw ;
  }
}

GeotiffFile::~GeotiffFile()
{
  if(gtif)
    GTIFFree(gtif);
  if(tif)
    XTIFFClose(tif);
}

//-----------------------------------------------------------------------
/// Return the type for the given key.
//-----------------------------------------------------------------------

GeotiffFile::tagtype_t GeotiffFile::key_type(GeotiffFile::geokey_t K)
{
  switch(K) {
  case GTCitationGeoKey:
  case GeogCitationGeoKey:
  case PCSCitationGeoKey:
  case VerticalCitationGeoKey:
    return TYPE_ASCII;
    break;
  case GeogInvFlatteningGeoKey:
  case GeogSemiMajorAxisGeoKey:
  case GeogSemiMinorAxisGeoKey:
  case ProjAzimuthAngleGeoKey:
  case ProjCenterLatGeoKey:
  case ProjCenterLongGeoKey:
  case ProjFalseEastingGeoKey:
  case ProjFalseNorthingGeoKey:
  case ProjFalseOriginEastingGeoKey:
  case ProjFalseOriginLatGeoKey:
  case ProjFalseOriginLongGeoKey:
  case ProjFalseOriginNorthingGeoKey:
  case ProjLinearUnitSizeGeoKey:
  case ProjNatOriginLatGeoKey:
  case ProjNatOriginLongGeoKey:
  case ProjRectifiedGridAngleGeoKey:
  case ProjScaleAtNatOriginGeoKey:
  case ProjStdParallel1GeoKey:
  case ProjStdParallel2GeoKey:
  case ProjStraightVertPoleLongGeoKey:
  case GeogLinearUnitSizeGeoKey:
  case GeogAngularUnitSizeGeoKey:
  case GeogPrimeMeridianLongGeoKey:
  case ProjCenterEastingGeoKey:
  case ProjCenterNorthingGeoKey:
  case ProjScaleAtCenterGeoKey:
    return TYPE_DOUBLE;
    break;
  case GTModelTypeGeoKey:
  case GTRasterTypeGeoKey:
  case GeogAngularUnitsGeoKey:
  case GeogEllipsoidGeoKey:
  case GeogGeodeticDatumGeoKey:
  case GeographicTypeGeoKey:
  case ProjCoordTransGeoKey:
  case ProjLinearUnitsGeoKey:
  case ProjectedCSTypeGeoKey:
  case ProjectionGeoKey:
  case GeogPrimeMeridianGeoKey:
  case GeogLinearUnitsGeoKey:
  case GeogAzimuthUnitsGeoKey:
  case VerticalCSTypeGeoKey:
  case VerticalDatumGeoKey:
  case VerticalUnitsGeoKey:
    return TYPE_SHORT;
  default:
    Exception e;
    e << "Unrecognized key " << K;
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Return the list of tags that take ASCII data.
//-----------------------------------------------------------------------

const std::vector<int>& GeotiffFile::geotiff_tag_ascii()
{
  static bool filled_in = false;
  static std::vector<int> data;
  if(!filled_in) {
    data.push_back(GTCitationGeoKey);
    data.push_back(GeogCitationGeoKey);
    data.push_back(PCSCitationGeoKey);
    data.push_back(VerticalCitationGeoKey);
    filled_in = true;
  }
  return data;
}

//-----------------------------------------------------------------------
/// Return the list of tags that take double data.
//-----------------------------------------------------------------------

const std::vector<int>& GeotiffFile::geotiff_tag_double()
{
  static bool filled_in = false;
  static std::vector<int> data;
  if(!filled_in) {
    data.push_back(GeogInvFlatteningGeoKey);
    data.push_back(GeogSemiMajorAxisGeoKey);
    data.push_back(GeogSemiMinorAxisGeoKey);
    data.push_back(ProjAzimuthAngleGeoKey);
    data.push_back(ProjCenterLatGeoKey);
    data.push_back(ProjCenterLongGeoKey);
    data.push_back(ProjFalseEastingGeoKey);
    data.push_back(ProjFalseNorthingGeoKey);
    data.push_back(ProjFalseOriginEastingGeoKey);
    data.push_back(ProjFalseOriginLatGeoKey);
    data.push_back(ProjFalseOriginLongGeoKey);
    data.push_back(ProjFalseOriginNorthingGeoKey);
    data.push_back(ProjLinearUnitSizeGeoKey);
    data.push_back(ProjNatOriginLatGeoKey);
    data.push_back(ProjNatOriginLongGeoKey);
    data.push_back(ProjRectifiedGridAngleGeoKey);
    data.push_back(ProjScaleAtNatOriginGeoKey);
    data.push_back(ProjStdParallel1GeoKey);
    data.push_back(ProjStdParallel2GeoKey);
    data.push_back(ProjStraightVertPoleLongGeoKey);
    data.push_back(GeogLinearUnitSizeGeoKey);
    data.push_back(GeogAngularUnitSizeGeoKey);
    data.push_back(GeogPrimeMeridianLongGeoKey);
    data.push_back(ProjCenterEastingGeoKey);
    data.push_back(ProjCenterNorthingGeoKey);
    data.push_back(ProjScaleAtCenterGeoKey);
    filled_in = true;
  }
  return data;
}

//-----------------------------------------------------------------------
/// Return the list of tags that take short data.
//-----------------------------------------------------------------------

const std::vector<int>& GeotiffFile::geotiff_tag_short()
{
  static bool filled_in = false;
  static std::vector<int> data;
  if(!filled_in) {
    data.push_back(GTModelTypeGeoKey);
    data.push_back(GTRasterTypeGeoKey);
    data.push_back(GeogAngularUnitsGeoKey);
    data.push_back(GeogEllipsoidGeoKey);
    data.push_back(GeogGeodeticDatumGeoKey);
    data.push_back(GeographicTypeGeoKey);
    data.push_back(ProjCoordTransGeoKey);
    data.push_back(ProjLinearUnitsGeoKey);
    data.push_back(ProjectedCSTypeGeoKey);
    data.push_back(ProjectionGeoKey);
    data.push_back(GeogPrimeMeridianGeoKey);
    data.push_back(GeogLinearUnitsGeoKey);
    data.push_back(GeogAzimuthUnitsGeoKey);
    data.push_back(VerticalCSTypeGeoKey);
    data.push_back(VerticalDatumGeoKey);
    data.push_back(VerticalUnitsGeoKey);
    filled_in = true;
  }
  return data;
}
