#include "usgs_dem.h"
#include "gdal_raster_image.h"
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for USGS DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have USGS files that completely cover the area. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of FILL_VALUE
/// instead. This is controlled by No_coverage_is_error.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_file. For each
/// file, we read that it tiles with the given Number_line_per_tile x
/// Number_sample_per_tile
/// Number_tile_each_file tiles.
//-----------------------------------------------------------------------

UsgsDemData::UsgsDemData
(const std::string& Dir,
 bool No_coverage_is_error,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file, 
 int Number_file
)
: RasterMultifile(Number_file, No_coverage_is_error, FILL_VALUE),
  number_line_per_tile(Number_line_per_tile),
  number_sample_per_tile(Number_sample_per_tile),
  number_tile_each_file(Number_tile_each_file)
{
  std::string dirbase = Dir;
  if(dirbase == "") {
    char *t = getenv("USGSDATA");
    if(!t)
      throw Exception("Need to either pass in the directory of the USGS DEM data, or set the environment variable USGSDATA");
    dirbase = t;
  }
  boost::regex fname_regex("([ns])(\\d+)([ew])(\\d+)_10m.tif");
  boost::smatch m;
  MapInfo mi_ref;
  int lat_ref = -1, lon_ref = -1;
  int num_per_deg_lat = -1, num_per_deg_lon = -1;
  std::vector<std::string> flist;
  std::vector<int> loffset;
  std::vector<int> soffset;
  int lmin = 0, lmax = 0, smin = 0, smax = 0;
  BOOST_FOREACH(boost::filesystem::directory_entry d, 
   		std::make_pair(boost::filesystem::directory_iterator(dirbase),
			       boost::filesystem::directory_iterator())) {
    std::string fname = d.path().filename();
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
      if(flist.size() == 1) {
	mi_ref = GdalRasterImage(flist[0]).map_info();
	// If this is the first file, then initialize the mapinfo that we
	// will calculate all the loffset and soffset against. We'll
	// adjust this after going through all the files for the upper
	// left corner is 0,0.
	loffset.push_back(0);
	soffset.push_back(0);
	lat_ref = lat;
	lon_ref = lon;
	double x0, y0, x1, y1;
	mi_ref.coordinate_to_index(lon_ref, lat_ref, x0, y0);
	mi_ref.coordinate_to_index(lon_ref + 1.0, lat_ref + 1.0, x1, y1);
	num_per_deg_lon = (int) round(x1 - x0);
	num_per_deg_lat = (int) round(y1 - y0);
      } else {
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
  f(new UsgsDemData(Dir, Outside_dem_is_error))
{
  initialize(D, f->map_info(), Outside_dem_is_error);
}

