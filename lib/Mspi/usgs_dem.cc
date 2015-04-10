#include "usgs_dem.h"
#include "gdal_raster_image.h"
#include "geocal_serialize_support.h"
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#ifdef HAVE_VICAR_RTL
#include "datum_geoid96.h"
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void UsgsDemData::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage)
  GEOCAL_BASE(RasterMultifile, RasterImageVariable)
  GEOCAL_BASE(UsgsDemData, RasterMultifile);
  int number_file = number_tile();
  ar & GEOCAL_NVP(dirbase)
    & GEOCAL_NVP(number_line_per_tile)
    & GEOCAL_NVP(number_sample_per_tile)
    & GEOCAL_NVP(number_tile_each_file)
    & GEOCAL_NVP_(no_coverage_is_error)
    & GEOCAL_NVP(number_file);
}

template<class Archive>
void UsgsDemData::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage)
  GEOCAL_BASE(RasterMultifile, RasterImageVariable)
  GEOCAL_BASE(UsgsDemData, RasterMultifile);
  int number_file;
  ar & GEOCAL_NVP(dirbase)
    & GEOCAL_NVP(number_line_per_tile)
    & GEOCAL_NVP(number_sample_per_tile)
    & GEOCAL_NVP(number_tile_each_file)
    & GEOCAL_NVP_(no_coverage_is_error)
    & GEOCAL_NVP(number_file);
  init(dirbase, no_coverage_is_error_, number_line_per_tile,
       number_sample_per_tile, number_tile_each_file, number_file);
}

template<class Archive>
void UsgsDem::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  GEOCAL_BASE(UsgsDem, DemMapInfo);
  ar & GEOCAL_NVP_(datum)
    & GEOCAL_NVP_(outside_dem_is_error)
    & GEOCAL_NVP2("usgs_dem_data", f);
}

template<class Archive>
void UsgsDem::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  GEOCAL_BASE(UsgsDem, DemMapInfo);
  ar & GEOCAL_NVP_(datum)
    & GEOCAL_NVP_(outside_dem_is_error)
    & GEOCAL_NVP2("usgs_dem_data", f);
  map_info_ = f->map_info();
}

GEOCAL_SPLIT_IMPLEMENT(UsgsDemData);
GEOCAL_SPLIT_IMPLEMENT(UsgsDem);
#endif

void UsgsDemData::init
(const std::string& Dir,
 bool No_coverage_is_error,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file, 
 int Number_file
)
{
  RasterMultifile::init(Number_file, No_coverage_is_error, FILL_VALUE);
  number_line_per_tile = Number_line_per_tile;
  number_sample_per_tile = Number_sample_per_tile;
  number_tile_each_file =Number_tile_each_file;
  dirbase = Dir;
  if(dirbase == "") {
    char *t = getenv("USGSDATA");
    if(!t)
      throw Exception("Need to either pass in the directory of the USGS DEM data, or set the environment variable USGSDATA");
    dirbase = t;
  }
  init_loc_to_file();
}

void UsgsDemData::init_loc_to_file()
{
  boost::regex fname_regex("([ns])(\\d+)([ew])(\\d+)_10m.tif");
  boost::smatch m;
  // We got the values here by looking at one of the map info files,
  // and just recording the information. This is from n47w087_10m.tif.
  // We start with a 1 degree tile, and then add a 6 pixel border
  int lat_ref = 47, lon_ref = -87;
  int num_per_deg_lat = -10800, num_per_deg_lon = 10800;
  int border = 6;
  MapInfo mi_ref(boost::shared_ptr<CoordinateConverter> 
		 (new GeodeticConverter()),
		 lon_ref, lat_ref, lon_ref + 1, lat_ref - 1, num_per_deg_lon, 
		 -num_per_deg_lat);
  mi_ref = mi_ref.subset(-border, -border, num_per_deg_lon + 2 * border,
			 -num_per_deg_lat + 2 * border);
  std::vector<std::string> flist;
  std::vector<int> loffset;
  std::vector<int> soffset;
  int lmin = 0, lmax = 0, smin = 0, smax = 0;
  BOOST_FOREACH(boost::filesystem::directory_entry d, 
   		std::make_pair(boost::filesystem::directory_iterator(dirbase),
			       boost::filesystem::directory_iterator())) {
    // Slightly different syntax depending on version of BOOST
    // filesystem. We need to support both, at least for a while
    // (version 2 is old and deprecated, but we need to support some
    // old systems)
#if defined(BOOST_FILESYSTEM_VERSION) && BOOST_FILESYSTEM_VERSION == 3
    std::string fname = d.path().filename().string();
#else
    std::string fname = d.path().filename();
#endif
    // Ignore files that have names we don't recognize (e.g., a README
    // file or something like that with the data files. This also
    // breaks the files we do recognize into the pieces we need to
    // determine the upper left corner
    if(boost::regex_match(fname, m, fname_regex)) {
      flist.push_back(d.path().string());
      // Note that this is *not* the ULC of the file. There is a
      // border around the file (of 6 pixels). However this border is
      // the same for all the files, so we can determine the relative
      // position of the files by comparing the lat/lon between files.
      int lat = boost::lexical_cast<int>(m[2]) * (m[1] == "n" ? 1 : -1);
      int lon = boost::lexical_cast<int>(m[4]) * (m[3] == "e" ? 1 : -1);
      // Calculate the line/sample relative to our initial mapinfo.
      int loff = (lat - lat_ref) * num_per_deg_lat;
      int soff = (lon - lon_ref) * num_per_deg_lon;
      loffset.push_back(loff);
      soffset.push_back(soff);
      lmin = std::min(loff, lmin);
      smin = std::min(soff, smin);
      lmax = std::max(loff, lmax);
      smax = std::max(soff, smax);
    }
  }
  if(flist.size() == 0) {
    Exception e;
    e << "No USGS DEM data found at " << dirbase;
    throw e;
  }

  // Calculate the map_info that fill cover all the data.
  map_info_.reset(new MapInfo(mi_ref.subset(smin, lmin, 
		    smax - smin + mi_ref.number_x_pixel(),
		    lmax - lmin + mi_ref.number_y_pixel())));
  number_line_ = map_info().number_y_pixel();
  number_sample_ = map_info().number_x_pixel();
  number_tile_line_ = number_line_;
  number_tile_sample_ = number_sample_;

  // Now go back through and populate loc_to_file.
  for(std::vector<std::string>::size_type i = 0; i < flist.size(); ++i)
    loc_to_file.add(loffset[i] - lmin,
		    soffset[i] - smin, 
		    loffset[i] - lmin + mi_ref.number_y_pixel(),
		    soffset[i] - smin + mi_ref.number_x_pixel(),
		    flist[i]);
}
    
RasterMultifileTile UsgsDemData::get_file(int Line, int Sample) const
{
  std::string fname = loc_to_file.find(Line, Sample);
  if(fname =="")
    return RasterMultifileTile();
  boost::shared_ptr<GdalRasterImage> f
    (new GdalRasterImage(fname, 1, number_tile_each_file, false, 
			 number_line_per_tile, number_sample_per_tile));
  ImageCoordinate ic = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(0,0))));
  int ln = (int) round(ic.line);
  int smp = (int) round(ic.sample);
  return RasterMultifileTile(f, ln, smp);
}

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

void UsgsDem::print(std::ostream& Os) const
{     
  OstreamPad opad(Os, "    ");
  Os << "USGS DEM:\n"
     << "  Datum:\n";
  opad << datum();
  opad.strict_sync();
  Os << "  Data directory: " << directory_base() << "\n";
  Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
}


//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for USGS DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have USGS files that completely cover the area. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of 0
/// instead. This is controlled by Outside_dem_is_error.
//-----------------------------------------------------------------------

UsgsDem::UsgsDem
(const std::string& Dir,
 bool Outside_dem_is_error,
 const boost::shared_ptr<Datum>& D
)
: 
  f(new UsgsDemData(Dir, Outside_dem_is_error, 10812, 10812))
{
  boost::shared_ptr<Datum> d(D);
  if(!d) {
#ifdef HAVE_VICAR_RTL    
    d.reset(new DatumGeoid96());
#else
    throw Exception("DatumGeoid96 wasn't included in the build");
#endif
  }
  // The 10812 up above is the size of the file, so we are reading all
  // the data. Not sure if we really want this hardcoded, or if we
  // really want all this data read. But for now, leave this like this
  // and we can revisit if needed.
  initialize(d, f->map_info(), Outside_dem_is_error);
}

