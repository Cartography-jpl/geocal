#include "cart_lab_multifile.h"
#include "vicar_lite_file.h"
#include "vicar_raster_image.h"
#include "gdal_raster_image.h"
#include "geocal_serialize_support.h"
#include <cstdlib>
#include <cmath>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CartLabMultifile::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage)
  GEOCAL_BASE(RasterMultifile, RasterImageVariable)
  GEOCAL_BASE(CartLabMultifile, RasterMultifile);
  int number_file = number_tile();
  ar & GEOCAL_NVP(dirbase)
    & GEOCAL_NVP(number_line_per_tile)
    & GEOCAL_NVP(number_sample_per_tile)
    & GEOCAL_NVP(number_tile_each_file)
    & GEOCAL_NVP(file_name_end)
    & GEOCAL_NVP(lat_ref)
    & GEOCAL_NVP(lon_ref)
    & GEOCAL_NVP(mi_ref)
    & GEOCAL_NVP_(no_coverage_is_error)
    & GEOCAL_NVP_(no_coverage_fill_value)
    & GEOCAL_NVP(number_file);
}

template<class Archive>
void CartLabMultifile::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage)
  GEOCAL_BASE(RasterMultifile, RasterImageVariable)
  GEOCAL_BASE(CartLabMultifile, RasterMultifile);
  int number_file;
  ar & GEOCAL_NVP(dirbase)
    & GEOCAL_NVP(number_line_per_tile)
    & GEOCAL_NVP(number_sample_per_tile)
    & GEOCAL_NVP(number_tile_each_file)
    & GEOCAL_NVP(file_name_end)
    & GEOCAL_NVP(lat_ref)
    & GEOCAL_NVP(lon_ref)
    & GEOCAL_NVP(mi_ref)
    & GEOCAL_NVP_(no_coverage_is_error)
    & GEOCAL_NVP_(no_coverage_fill_value)
    & GEOCAL_NVP(number_file);
  RasterMultifile::init(number_file, no_coverage_is_error_, 
			no_coverage_fill_value_);
  init_loc_to_file();
}

template<class Archive>
void GdalCartLabMultifile::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartLabMultifile);
}

template<class Archive>
void VicarCartLabMultifile::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartLabMultifile)
    & GEOCAL_NVP(favor_memory_mapped)
    & GEOCAL_NVP(force_area_pixel);
}

GEOCAL_SPLIT_IMPLEMENT(CartLabMultifile);
GEOCAL_IMPLEMENT(GdalCartLabMultifile);
GEOCAL_IMPLEMENT(VicarCartLabMultifile);
#endif

CartLabMultifile::CartLabMultifile
(const std::string& Dir,
 const std::string& Dir_env_variable,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file,
 int Number_tile,
 bool No_coverage_is_error, 
 int No_coverage_fill_value
)
  : RasterMultifile(Number_tile, No_coverage_is_error, 
		    No_coverage_fill_value),
    dirbase(Dir), number_line_per_tile(Number_line_per_tile),
    number_sample_per_tile(Number_sample_per_tile),
    number_tile_each_file(Number_tile_each_file)
{
  if(dirbase == "" && Dir_env_variable != "") {
    char *t = getenv(Dir_env_variable.c_str());
    if(!t)
      throw Exception("Need to either pass in the directory of the cart lab database, or set the environment variable " + Dir_env_variable);
    dirbase = t;
  }
}



void CartLabMultifile::init_loc_to_file()
{
  boost::regex fname_regex("([ns])(\\d+)([ew])(\\d+)" + file_name_end);
  boost::smatch m;
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
    // file or something like that with the data files). This also
    // breaks the files we do recognize into the pieces we need to
    // determine the upper left corner

    double x0, y0, x1, y1;
    mi_ref.coordinate(Geodetic(lat_ref, lon_ref), x0, y0);
    mi_ref.coordinate(Geodetic(lat_ref + 1, lon_ref + 1), x1, y1);
    int num_per_deg_lat = (int) round(y1 - y0);
    int num_per_deg_lon = (int) round(x1 - x0);
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
    e << "No data files found at " << dirbase;
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

RasterMultifileTile GdalCartLabMultifile::get_file(int Line, int Sample) const
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

RasterMultifileTile VicarCartLabMultifile::get_file(int Line, int Sample) const
{
  std::string fname = loc_to_file.find(Line, Sample);
  if(fname =="")
    return RasterMultifileTile();
  //-----------------------------------------------------------------------
  // If we are using memory mapped io by preference, try that first.
  // We allow this to fail, the file might be too large to do memory
  // mapped io (particular on the mac, which is limited to 2G).
  //-----------------------------------------------------------------------

  boost::shared_ptr<RasterImage> f;
  try {
    if(favor_memory_mapped) {
      boost::shared_ptr<VicarLiteRasterImage> 
	f2(new VicarLiteRasterImage(fname, 1, VicarLiteFile::READ,
				    -1, -1, force_area_pixel));
      if(!f2->is_compressed())  // Can only use memory mapped for
				// uncompressed files.
	f = f2;
    }
  } catch(const Exception& E) {
    // Ignore errors, we drop down to using the Vicar routines below.
  }
  if(!f.get())
    f.reset(new VicarRasterImage(fname, 1, VicarFile::READ,
				 number_line_per_tile, 
				 number_tile_each_file, force_area_pixel));

  ImageCoordinate ic = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(0,0))));
  ImageCoordinate ic2 = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(f->number_line() - 1,
					      f->number_sample() - 1))));
  int ln = (int) round(ic.line);
  int smp = (int) round(ic.sample);
  return RasterMultifileTile(f, ln, smp);
}
