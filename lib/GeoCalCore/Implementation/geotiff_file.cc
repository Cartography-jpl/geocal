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
