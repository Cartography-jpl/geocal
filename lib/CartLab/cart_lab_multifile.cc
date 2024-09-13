#include "cart_lab_multifile.h"
#include "gdal_raster_image.h"
#include "constant_raster_image.h"
#include "geocal_serialize_support.h"
#include "geocal_temp_file.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include "vicar_raster_image.h"

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

template<class Archive>
void VicarCartLabMultifileSetup::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VicarCartLabMultifile);
}

GEOCAL_SPLIT_IMPLEMENT(CartLabMultifile);
GEOCAL_IMPLEMENT(GdalCartLabMultifile);
GEOCAL_IMPLEMENT(VicarCartLabMultifile);
GEOCAL_IMPLEMENT(VicarCartLabMultifileSetup);
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
    std::string fname = d.path().filename().string();
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
  first_file = flist[0];
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
  auto fname_list = loc_to_file.find_region(Line, Sample, 1, 1);
  for(auto & fname: fname_list) {
    boost::shared_ptr<GdalRasterImage> f =
      boost::make_shared<GdalRasterImage>(fname, 1, number_tile_each_file,
					  false, number_line_per_tile,
					  number_sample_per_tile);
    ImageCoordinate ic = 
      coordinate(*(f->ground_coordinate(ImageCoordinate(0,0))));
    int ln = (int) round(ic.line);
    int smp = (int) round(ic.sample);
    RasterMultifileTile res(f, ln, smp);
    if(res.in_tile(Line, Sample))
      return res;
  }
  if(!no_coverage_is_error_) {
    boost::shared_ptr<RasterImage> cf
      (new ConstantRasterImage(mi_ref.number_y_pixel(), mi_ref.number_x_pixel(),
			       no_coverage_fill_value_));
    int ln = (Line / cf->number_line()) * cf->number_line();
    int smp = (Sample / cf->number_sample()) * cf->number_sample();
    return RasterMultifileTile(cf, ln, smp);
  } else {
    return RasterMultifileTile();
  }
}

RasterMultifileTile VicarCartLabMultifile::get_file(int Line, int Sample) const
{
  auto fname_list = loc_to_file.find_region(Line, Sample, 1, 1);
  for(auto & fname: fname_list) {
    //-----------------------------------------------------------------------
    // If we are using memory mapped io by preference, try that first.
    // We allow this to fail, the file might be too large to do memory
    // mapped io (particular on the mac, which is limited to 2G).
    //-----------------------------------------------------------------------
    boost::shared_ptr<RasterImage> f = 
      vicar_open(fname, 1, VicarFile::READ, favor_memory_mapped,
		 number_line_per_tile, number_tile_each_file, force_area_pixel);
    ImageCoordinate ic = 
      coordinate(*(f->ground_coordinate(ImageCoordinate(0,0))));
    ImageCoordinate ic2 = 
      coordinate(*(f->ground_coordinate(ImageCoordinate(f->number_line() - 1,
					      f->number_sample() - 1))));
    int ln = (int) round(ic.line);
    int smp = (int) round(ic.sample);
    RasterMultifileTile res(f, ln, smp);
    if(res.in_tile(Line, Sample))
      return res;
  }
  if(!no_coverage_is_error_) {
    boost::shared_ptr<RasterImage> cf
      (new ConstantRasterImage(mi_ref.number_y_pixel(), mi_ref.number_x_pixel(),
			       no_coverage_fill_value_));
    int ln = (Line / cf->number_line()) * cf->number_line();
    int smp = (Sample / cf->number_sample()) * cf->number_sample();
    return RasterMultifileTile(cf, ln, smp);
  } else {
    return RasterMultifileTile();
  }

}

//-----------------------------------------------------------------------
/// Create a stand alone file that contains a subset of the full file.
/// This handles whatever mosaicing/subsetting is needed for the
/// underlying tile files. We use GDAL to generate this, so you pass
/// in the driver to use like you do with a GdalRasterImage (e.g.,
/// "gtiff") and whatever options (e.g., 
/// "TILED=YES BLOCKXSIZE=16 BLOCKYSIZE=32 COMPRESS=JPEG
/// JPEG_QUALITY=90").
/// We cover the given set of points, along with whatever boundary you
/// request (just like MapInfo cover function).
///
/// Note that datasets often have holes in them (e.g., no data over
/// the ocean). These get filled in as zero. It is perfectly possible
/// (and not an error) for there to be *no* data over the desired
/// subset. In that case, we create a file with all zeros.
///
/// This executes the command shell gdalbuildvrt, which must be in the
/// path. Right now with GDAL the same functionality can't be done
/// through C++, but there is talk of making VRTBuilder found in
/// gdalbuildvrt available. For now though, we just use a system call.
///
/// You can either supply a set of points to cover (which will use the
/// native map_info() for this object), or you can supply an explicit
/// Desired_map_info (e.g., you are matching an existing file in
/// extent and resolution).
///
/// You can optionally supply an argument string to use with
/// gdal_translate on the gdalbuildvrt file before writing to the
/// output. This can be useful to do things like change the output
/// type or scale the data. See gdal_translate documentation for what
/// these options should be (e.g., "-ot Int16 -outsize 50% 50% -r
/// average" to convert to Int16 and 2x2 pixel averaging to make a
/// coarser image).
//-----------------------------------------------------------------------

void CartLabMultifile::create_subset_file
(const std::string& Oname, const std::string& Driver,
 const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
 const boost::shared_ptr<MapInfo>& Desired_map_info,
 const std::string& Translate_arg,
 const std::string& Options,
 int Boundary,
 bool Verbose) const
{
  boost::shared_ptr<MapInfo> msub;
  if(Desired_map_info)
    msub = Desired_map_info;
  else
    msub = boost::make_shared<MapInfo>(map_info().cover(Pt, Boundary));
  // Check that map projection uses the same coordinates, we don't
  // handle with this function doing something like converting to UTM.
  if(!map_info().coordinate_converter().is_same(msub->coordinate_converter()))
    throw Exception("Desired_map_info needs to be in the same coordinate system as CartLabMultifile.");
  ImageCoordinate ic = coordinate(*msub->ground_coordinate(0,0));
  ImageCoordinate icend = coordinate(*msub->ground_coordinate(msub->number_x_pixel(), msub->number_y_pixel()));
  int lstart = (int) floor(ic.line);
  int sstart = (int) floor(ic.sample);
  int numl = ((int) ceil(icend.line)) - lstart;
  int nums = ((int) ceil(icend.sample)) - sstart;
  if(Verbose)
    std::cout << "Looking for region (" << lstart << ", " << sstart
	      << ") size (" << numl << ", " << nums << ")\n";
  std::vector<std::string> flist = 
    loc_to_file.find_region(lstart, sstart, numl, nums);
  if(flist.size() == 0) {
    if(Verbose)
      std::cout << "No files cover that area, creating a file of all zeros\n";
  }
  std::vector<double> lat, lon;
  Geodetic g1 = Geodetic(*msub->ground_coordinate(-0.5,-0.5));
  Geodetic g2 = Geodetic(*msub->ground_coordinate(-0.5,
						  msub->number_y_pixel()-0.5));
  Geodetic g3 = Geodetic(*msub->ground_coordinate(msub->number_x_pixel()-0.5,
						  msub->number_y_pixel()-0.5));
  Geodetic g4 = Geodetic(*msub->ground_coordinate(msub->number_x_pixel()-0.5,
						  -0.5));
  double lat_min = std::min(g1.latitude(), std::min(g2.latitude(),
			    std::min(g3.latitude(), g4.latitude())));
  double lat_max = std::max(g1.latitude(), std::max(g2.latitude(),
			    std::max(g3.latitude(), g4.latitude())));
  double lon_min = std::min(g1.longitude(), std::min(g2.longitude(),
			    std::min(g3.longitude(), g4.longitude())));
  double lon_max = std::max(g1.longitude(), std::max(g2.longitude(),
			    std::max(g3.longitude(), g4.longitude())));
  double tres_lon = fabs((msub->ulc_x()-msub->lrc_x()) /
			 msub->number_x_pixel());
  double tres_lat = fabs((msub->ulc_y()-msub->lrc_y()) /
			 msub->number_y_pixel());
  // We ran into a case where gdalbuildvrt created a vrt file with one
  // of the files having a zero size (it was once that just touched
  // the edge of the desired map info). To work around this, do the
  // vrt in two steps - one just joins the files together and the
  // other subsets it.
  
  std::ostringstream command;
  std::ostringstream command2;
  command << "gdalbuildvrt -q";
  command2 << "gdalbuildvrt -q"
	   << " -te " << std::setprecision(12)
	   << lon_min << " " << lat_min << " " << lon_max << " "
	   << lat_max << " -tr " << tres_lon << " " << tres_lat << " -r average";
  GeoCalTempFile f;
  command << " " << f.temp_fname();
  GeoCalTempFile f2;
  command2 << " " << f2.temp_fname();
  BOOST_FOREACH(std::string f, flist)
    command << " " << f;
  // Make sure we have at least one file in the list, even if this
  // doesn't actually cover the area at all. This will force
  // gdalbuildvrt to make an empty file
  if(flist.size() == 0)
    command << " " << first_file;
  command2 << " " << f.temp_fname();
  if(Verbose)
    std::cout << "GDAL command: " << command.str() << "\n"
	      << "GDAL command: " << command2.str() << "\n";
  int status = system(command.str().c_str());
  if(status != 0) {
    Exception e;
    e << "CartLabMultifile::create_subset_file failed system command.\n"
      << "  Command: " << command.str() << "\n";
    throw e;
  }
  status = system(command2.str().c_str());
  if(status != 0) {
    Exception e;
    e << "CartLabMultifile::create_subset_file failed system command.\n"
      << "  Command: " << command2.str() << "\n";
    throw e;
  }
  if(Translate_arg != "") {
    std::string t2 = std::string(f2.temp_fname()) + "_2";
    std::ostringstream command3;
    command3 << "gdal_translate " << Translate_arg << " "
	     << f2.temp_fname() << " " << t2;
    if(Verbose)
      std::cout << "GDAL command: " << command3.str() << "\n";
    status = system(command3.str().c_str());
    if(status != 0) {
      Exception e;
      e << "CartLabMultifile::create_subset_file failed system command.\n"
	<< "  Command: " << command3.str() << "\n";
      throw e;
    }
    GdalRasterImage d(t2);
    gdal_create_copy(Oname, Driver, *d.data_set(), Options);
    unlink(t2.c_str());
  } else {
    GdalRasterImage d(f2.temp_fname());
    gdal_create_copy(Oname, Driver, *d.data_set(), Options);
  }
}
