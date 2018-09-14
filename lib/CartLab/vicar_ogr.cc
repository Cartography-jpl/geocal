#include "vicar_ogr.h"
#include <boost/utility.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "gdal_raster_image.h"
#include "geotiff_file.h"

//-----------------------------------------------------------------------
// Normally we wouldn't have prototypes defined directly in this
// class. However, we have a special case. We know that GDAL will
// supply the GeoTIFF library, because if it isn't found on the system
// GDAL will use its own private copy. However, we don't know that the 
// GeoTIFF headers will be available. GDAL doesn't install them, and
// unless there is a another copy of GeoTIFF available they won't be
// found. Since it is entirely possible to have GDAL installed but
// GeoTIFF not, we don't want to add a dependency on this library that
// we don't really need. So, we define the prototypes here. There is a
// risk that these prototypes will become out of date, in which case
// we'll need to update these. But the GeoTIFF library hasn't changed
// in some time, so the trade against adding a dependency is probably
// a good one.
//-----------------------------------------------------------------------
struct TIFF;
struct GTIF;
typedef unsigned short geocode_t;
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

//------------------------------------------
// All done with prototypes, ready to start code
//------------------------------------------

using namespace GeoCal;

//-----------------------------------------------------------------------
// We have a number of resources that we need to maintain. This set of
// classes is used to maintain these resources and make sure they get
// cleaned up.
//-----------------------------------------------------------------------
namespace GeoCal {
namespace VicarOgrNsp {
class TempFile {
public:
  TempFile() {temp_fname = tmpnam(0);}
  ~TempFile() {unlink(temp_fname);}
  char* temp_fname;
};

}
}
//-----------------------------------------------------------------------
/// Constructor. Normally we only have one of these objects around,
/// although nothing is hurt if you create multiple copies.
//-----------------------------------------------------------------------

VicarOgr::VicarOgr()
{
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_ascii()) {
    std::string kn = GTIFKeyName((geokey_t) t);
    boost::to_upper(kn);
    tag_to_vicar_name[t] = kn;
  }
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_double()) {
    std::string kn = GTIFKeyName((geokey_t) t);
    boost::to_upper(kn);
    tag_to_vicar_name[t] = kn;
  }
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_short()) {
    std::string kn = GTIFKeyName((geokey_t) t);
    boost::to_upper(kn);
    tag_to_vicar_name[t] = kn;
  }

  // Make sure we have registered all the drivers, including geotiff.
  GdalRegister::gdal_register();
}

//-----------------------------------------------------------------------
/// We can use the same code to create a gtiff file with the same
/// labels as either a VicarFile or a VicarLiteFile. We use a template
/// so we don't need to replace the code.
//-----------------------------------------------------------------------

template<class T> void VicarOgr::vicar_to_gtiff_template(const T& F, const char*Fname) 
{
  using namespace VicarOgrNsp;
  // Write out all of the keys we find in the VICAR file into the
  // temporary file.
  GeotiffFile g(Fname, "w");
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_short()) {
    if(F.has_label("GEOTIFF " + tag_to_vicar_name[t])) {
      std::string tv = F.label_string(tag_to_vicar_name[t],
				      "GEOTIFF");
      int v = atoi(tv.c_str());
      // Special handling to override GTRASTERTYPEGEOKEY and force
      // pixel is area. This is a workaround for an error in SRTM
      // data, see VicarFile force_area_pixel description for details.
      if(F.force_area_pixel() &&
	 tag_to_vicar_name[t] == "GTRASTERTYPEGEOKEY")
	v = 1;		// Force RasterPixelIsArea, which is
      // code 1.
      GTIFKeySet(g.gtif, (geokey_t) t, GeotiffFile::TYPE_SHORT, 1, (geocode_t) v);
    }
  }
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_double()) {
    if(F.has_label("GEOTIFF " + tag_to_vicar_name[t])) {
      std::string tv = F.label_string(tag_to_vicar_name[t],
				      "GEOTIFF");
      double v = atof(tv.c_str());
      GTIFKeySet(g.gtif, (geokey_t) t, GeotiffFile::TYPE_DOUBLE, 1, v);
    }
  }
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_ascii()) {
    if(F.has_label("GEOTIFF " + tag_to_vicar_name[t])) {
      std::string v = F.label_string(tag_to_vicar_name[t], "GEOTIFF");
      GTIFKeySet(g.gtif, (geokey_t) t, GeotiffFile::TYPE_ASCII, 0, v.c_str());
    }
  }
  if(F.has_label("GEOTIFF MODELPIXELSCALETAG")) {
    std::string vs = F.label_string("MODELPIXELSCALETAG", "GEOTIFF");
    BOOST_FOREACH(char& c, vs)
      if(c == '(' || c == ')' || c == ',')
	c = ' ';
    size_t istart = 0;
    size_t iend = 0;
    double v[3];
    for(int i = 0; i < 3; ++i) {
      istart = vs.find_first_not_of(" ", iend);
      iend = vs.find_first_of(" ", istart);
      v[i] = atof(vs.substr(istart, iend - istart).c_str());
    }
    TIFFSetField(g.tif, TIFFTAG_GEOPIXELSCALE, 3, v);
  }
  if(F.has_label("GEOTIFF MODELTIEPOINTTAG")) {
    std::string vs = F.label_string("MODELTIEPOINTTAG", "GEOTIFF");
    BOOST_FOREACH(char& c, vs)
      if(c == '(' || c == ')' || c == ',')
	c = ' ';
    size_t istart = 0;
    size_t iend = 0;
    double v[6];
    for(int i = 0; i < 6; ++i) {
      istart = vs.find_first_not_of(" ", iend);
      iend = vs.find_first_of(" ", istart);
      v[i] = atof(vs.substr(istart, iend - istart).c_str());
    }
    TIFFSetField(g.tif, TIFFTAG_GEOTIEPOINTS, 6, v);
  }
  if(F.has_label("GEOTIFF MODELTRANSFORMATIONTAG")) {
    std::string vs = F.label_string("MODELTRANSFORMATIONTAG", "GEOTIFF");
    BOOST_FOREACH(char& c, vs)
      if(c == '(' || c == ')' || c == ',')
	c = ' ';
    size_t istart = 0;
    size_t iend = 0;
    double v[16];
    for(int i = 0; i < 16; ++i) {
      istart = vs.find_first_not_of(" ", iend);
      iend = vs.find_first_of(" ", istart);
      v[i] = atof(vs.substr(istart, iend - istart).c_str());
    }
    TIFFSetField(g.tif, TIFFTAG_GEOTRANSMATRIX, 16, v);
  }

  //----------------------------------------------------------------
  // Special handling for the Geographic case. VICAR files sometimes
  // leave off the GeographicTypeGeoKey in the Geographic case. This 
  // is required by GDAL to know what to do with the file. So check
  // if we have a geographic model w/o the GeographicTypeGeoKey
  // specified, and if so add a WGS84 key. The Key is "4326", and
  // this corresponds to "GCS_WGS_84".
  //----------------------------------------------------------------

  if(!F.has_label("GEOTIFF GEOGRAPHICTYPEGEOKEY") &&
     F.has_label("GEOTIFF GTMODELTYPEGEOKEY") &&
     atoi(F.label_string("GTMODELTYPEGEOKEY", "GEOTIFF").c_str()) 
     == 2)
    GTIFKeySet(g.gtif, GeotiffFile::GeographicTypeGeoKey,
	       GeotiffFile::TYPE_SHORT, 1, 
	       (geocode_t) 4326); 
  
  //----------------------------------------------------------------
  // Set other tags needed to make a valid file, and write data.
  // This is just a 1x1 file.
  //----------------------------------------------------------------
  
  TIFFSetField(g.tif, TIFFTAG_IMAGEWIDTH, 1);
  TIFFSetField(g.tif, TIFFTAG_IMAGELENGTH, 1);
  TIFFSetField(g.tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
  TIFFSetField(g.tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(g.tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField(g.tif, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(g.tif, TIFFTAG_SAMPLESPERPIXEL, 1);
  char c = '\0';
  TIFFWriteEncodedStrip(g.tif, 0, &c, 1);
  GTIFWriteKeys(g.gtif);
}

//-----------------------------------------------------------------------
/// We can use the same code to get MapInfo for either a VicarFile or
/// a VicarLiteFile. We use a template here so we don't need to repeat
/// our code
//-----------------------------------------------------------------------

template<class T> MapInfo VicarOgr::from_vicar_template(const T& F) 
{
  using namespace VicarOgrNsp;
  TempFile f;
  vicar_to_gtiff_template(F, f.temp_fname);

  GdalRasterImage gd(f.temp_fname);
  MapInfo m = gd.map_info();

//----------------------------------------------------------------
// The MapInfo returned by gd is correct except that the size is
// wrong, since we shrunk the data down to 1x1. Create the full size
// MapInfo and return it.  
//----------------------------------------------------------------

  return m.subset(0, 0, F.number_sample(), F.number_line());
}


//-----------------------------------------------------------------------
/// Create a 1 pixel geotiff file that contains all the geotiff labels
/// from the Vicar file.
//-----------------------------------------------------------------------

void VicarOgr::vicar_to_gtiff(const VicarFile& F, const std::string& Fname)
{
  vicar_to_gtiff_template(F, Fname.c_str());
}

//-----------------------------------------------------------------------
/// Create a 1 pixel geotiff file that contains all the geotiff labels
/// from the Vicar file.
//-----------------------------------------------------------------------

void VicarOgr::vicar_to_gtiff(const VicarLiteFile& F, const std::string& Fname)
{
  vicar_to_gtiff_template(F, Fname.c_str());
}

//-----------------------------------------------------------------------
/// Read the metadata from a Vicar File, and use to create a MapInfo
/// based on GDAL.
//-----------------------------------------------------------------------

MapInfo VicarOgr::from_vicar(const VicarFile& F)
{
  return from_vicar_template(F);
}

//-----------------------------------------------------------------------
/// Read the metadata from a VicarLiteFile, and use to create a MapInfo
/// based on GDAL.
//-----------------------------------------------------------------------

MapInfo VicarOgr::from_vicar(const VicarLiteFile& F)
{
  return from_vicar_template(F);
}

//-----------------------------------------------------------------------
/// Use a MapInfo to write the GeoTIFF metadata for a Vicar File.
//-----------------------------------------------------------------------

void VicarOgr::to_vicar(const MapInfo& Mi, VicarFile& F)
{
  using namespace VicarOgrNsp;
  TempFile ft;

//----------------------------------------------------------------
// Shrink MapInfo down to 1 pixel, since we don't need to create a
// full size GeoTIFF file
//----------------------------------------------------------------

  MapInfo mi2 = Mi.subset(0,0,1,1);
  {
    GdalRasterImage g(ft.temp_fname, "gtiff", mi2, 1,
			    GdalRasterImage::Byte);
  } // Destructor called, which flushes data to disk.

  // Now open the file back as a GeoTIFF file and copy keys we find to
  // VICAR.

  GeotiffFile g(ft.temp_fname, "r");
  const int bufsize = 1000;
  char buf[bufsize];
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_short()) {
    geocode_t v;
    if(GTIFKeyGet(g.gtif, (geokey_t) t, &v, 0, 1) ==1) {
      snprintf(buf, bufsize, "%d", v);
      std::string s(buf);
      s = s + "(" + GTIFValueName((geokey_t) t, v) + ")";
      F.label_set(tag_to_vicar_name[t], s, "GEOTIFF");
    }
  }
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_double()) {
    double v;
    if(GTIFKeyGet(g.gtif, (geokey_t) t, &v, 0, 1) ==1) {
      snprintf(buf, bufsize, "%.12g", v);
      std::string s(buf);
      F.label_set(tag_to_vicar_name[t], s, "GEOTIFF");
    }
  }
  BOOST_FOREACH(int t, GeotiffFile::geotiff_tag_ascii()) {
    int size = GTIFKeyInfo(g.gtif, (geokey_t) t, 0, 0);
    if(size !=0) {
      std::vector<char> v(size);
      if(GTIFKeyGet(g.gtif, (geokey_t) t, &v[0], 0, size) ==0)
	throw Exception("Trouble reading ASCII tag in geotiff file");
      std::string vs(&v[0]);
      F.label_set(tag_to_vicar_name[t], vs, "GEOTIFF");
    }
  }
  
  double *tiepoint, *scale, *transform;
  uint16_t num;
  if(TIFFGetField(g.tif, TIFFTAG_GEOPIXELSCALE, &num, &scale) ==1) {
    if(num == 2)		// Fill in 0.0 for third entry if
				// missing
      snprintf(buf, bufsize, "(%.12g,%.12g,0.0)", scale[0], scale[1]);
    else if(num ==3)
      snprintf(buf, bufsize, "(%.12g,%.12g,%.12g)", scale[0], scale[1], scale[2]);
    else
      throw Exception("Not sure how to interpret TIFFTAG_GEOPIXELSCALE");
    std::string s(buf);
    F.label_set("MODELPIXELSCALETAG", s, "GEOTIFF");
  }
  if(TIFFGetField(g.tif, TIFFTAG_GEOTIEPOINTS, &num, &tiepoint) ==1) {
    if(num != 6)
      throw Exception("Not sure how to interpret TIFFTAG_GEOTIEPOINTS");
    snprintf(buf,bufsize, "(%.12g,%.12g,%.12g,%.12g,%.12g,%.12g)", tiepoint[0], 
	     tiepoint[1], tiepoint[2], tiepoint[3], tiepoint[4], tiepoint[5]);
    std::string s(buf);
    F.label_set("MODELTIEPOINTTAG", s, "GEOTIFF");
  }
  if(TIFFGetField(g.tif, TIFFTAG_GEOTRANSMATRIX, &num, &transform) ==1) {
    if(num != 16)
      throw Exception("Not sure how to interpret TIFFTAG_GEOTRANSMATRIX");
    snprintf(buf,bufsize, "(%.12g,%.12g,%.12g,%.12g,%.12g,%.12g,%12g,%12g,%.12g,%.12g,%.12g,%.12g,%.12g,%.12g,%12g,%12g)", 
	     transform[0], transform[1], transform[2], transform[3], transform[4], transform[5], transform[6], 
	     transform[7], transform[8], transform[9], transform[10], transform[11], transform[12], 
	     transform[13], transform[14], transform[15]);
    std::string s(buf);
    F.label_set("MODELTRANSFORMATIONTAG", s, "GEOTIFF");
  }
}
