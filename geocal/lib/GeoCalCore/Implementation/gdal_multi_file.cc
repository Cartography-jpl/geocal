#include "gdal_multi_file.h"
#include "gdal_map_projected_image.h"
#include <boost/foreach.hpp>
#include <sstream>
using namespace GeoCal;

//-----------------------------------------------------------------------
// How far the a particular files map projection can be off without us
// considering this an error. 
//-----------------------------------------------------------------------

const double GdalMultiFile::map_info_tolerance = 0.01;

//-----------------------------------------------------------------------
/// Read the given list of files to create a multifile. The files all
/// need to have the same map projection, same resolution, and be an
/// integral number of pixels from a common origin. For each file, we
/// read the given band.
/// 
/// If you would like areas without file coverage to simple return a 0
/// rather than an error, set No_coverage_is_error to false.
///
/// For some applications (e.g., image pyramids for google earth) it
/// is convenient to restrict the size of the full image to multiple
/// of a fixed tile size, e.g. 256. If this is desired, then you can
/// pass this in as Size_multiple_of.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_file. For each
/// file, we read that it tiles with the given Number_line_per_tile x
/// Number_sample_per_tile
/// Number_tile_each_file tiles.
//-----------------------------------------------------------------------

GdalMultiFile::GdalMultiFile(const std::vector<std::string>& File_list, 
				  int b,
				  bool No_coverage_is_error,
				  int Size_multiple_of, 
				  int Number_line_per_tile,
				  int Number_sample_per_tile, 
			     int Number_tile_each_file, int Number_file,
			     int No_coverage_fill_value)
  : MapProjectedMultifile(Number_file, No_coverage_is_error, 
			  No_coverage_fill_value),
  band_(b),
  number_line_per_tile(Number_line_per_tile),
  number_sample_per_tile(Number_sample_per_tile),
  number_tile_each_file(Number_tile_each_file)
{
  if(File_list.size() ==0)
    throw Exception("File_list is empty");
  GdalMapProjectedImage gfbase(File_list[0], band_);
  MapInfo mi = gfbase.map_info();
  int minline = 0;
  int maxline = gfbase.number_line() - 1;
  int minsample = 0;
  int maxsample = gfbase.number_sample() - 1;
  BOOST_FOREACH(const std::string& fname, File_list) {
    GdalMapProjectedImage gf(fname, band_);
    MapInfo mi2 = gf.map_info();
    if(!mi.coordinate_converter().is_same(mi2.coordinate_converter())) {
      Exception e;
      e << "The file " << fname << " did not have the same coordinate system as this GdalMultiFile.";
      throw e;
    }
    ImageCoordinate ic = 
      gfbase.coordinate(*(gf.ground_coordinate(ImageCoordinate(0,0))));
    ImageCoordinate ic2 = 
      gfbase.coordinate(*(gf.ground_coordinate(ImageCoordinate(1,1))));
    int ln = (int) round(ic.line);
    int smp = (int) round(ic.sample);
    if(fabs(ic.line - ln) > map_info_tolerance ||
       fabs(ic.sample - smp) > map_info_tolerance) {
      Exception e;
      e << "The file " << fname << " doesn't align with a pixel boundary for this GdalMultiFile";
      throw e;
    }
    if(fabs(ic2.line - (ln + 1)) > map_info_tolerance ||
       fabs(ic2.sample - (smp + 1)) > map_info_tolerance) {
      Exception e;
      e << "The file " << fname << " doesn't have a pixel size that matches GdalMultiFile";
      throw e;
    }
    minline = (ln < minline ? ln : minline);
    minsample = (smp < minsample ? smp : minsample);
    ln += gf.number_line() - 1;
    smp += gf.number_sample() - 1;
    maxline = (ln > maxline ? ln : maxline);
    maxsample = (smp > maxsample ? smp : maxsample);
  }
  number_line_ = maxline - minline + 1;
  number_sample_ = maxsample - minsample + 1;
  if(number_line_ % Size_multiple_of > 0) {
    int number_line_new = (number_line_ / Size_multiple_of + 1) * 
      Size_multiple_of;
    minline -= (number_line_new - number_line_) / 2;
    number_line_ = number_line_new;
  }
  if(number_sample_ % Size_multiple_of > 0) {
    int number_sample_new = (number_sample_ / Size_multiple_of + 1) * 
      Size_multiple_of;
    minsample -= (number_sample_new - number_sample_) / 2;
    number_sample_ = number_sample_new;
  }
  map_info_ = mi.subset(minsample, minline, number_sample_, number_line_);
  BOOST_FOREACH(const std::string& fname, File_list) {
    GdalMapProjectedImage gf(fname, band_);
    ImageCoordinate ic = 
      coordinate(*(gf.ground_coordinate(ImageCoordinate(0,0))));
    int ln = (int) round(ic.line);
    int smp = (int) round(ic.sample);
    loc_to_file.add(ln, smp, ln + gf.number_line(), smp + gf.number_sample(), 
		    fname);
  }
}

//-----------------------------------------------------------------------
/// Get a file that covers the given location.
//-----------------------------------------------------------------------

MapProjectedMultifileTile GdalMultiFile::get_file(int Line, int Sample) const
{
  std::string fname = loc_to_file.find(Line, Sample);
  if(fname =="") {
    return MapProjectedMultifileTile();
  }
  boost::shared_ptr<GdalMapProjectedImage> f(new GdalMapProjectedImage(fname, band_, number_tile_each_file, false, number_line_per_tile, number_sample_per_tile));
  ImageCoordinate ic = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(0,0))));
  ImageCoordinate ic2 = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(1,1))));
  int ln = (int) round(ic.line);
  int smp = (int) round(ic.sample);
  return MapProjectedMultifileTile(f, ln, smp);
}

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

void GdalMultiFile::print(std::ostream& Os) const
{
  Os << "GdalMultiFile " << number_line() << " x " << number_sample()
     << "\n"
     << "  Band:     " << band_ << "\n"
     << "  No coverage is error: " << (no_coverage_is_error ? "true" : "false")
     << "\n"
     << "  Map info: " << map_info() << "\n";
}

//-----------------------------------------------------------------------
/// Create a GdalMultiFile for the given file list, and write out into
/// fixed size tiles PNG (256 x 256) for google earth. We create the
/// file name as Fbase + line_tile + "_" + sample_tile + ".png", e.g.
/// foo/0_1.png. We treat 0 as transparent fill value.
///
/// Note that this *only* generates the PNG files, we don't produce
/// the KML files or image pyramid. This is intended as a low level
/// function to be used with some higher level tool that handles the
/// other pieces (e.g., Ruby, which is better than C++ for generating 
/// XML files)
///
/// We return the list of files created and the MapInfo for each file.
//-----------------------------------------------------------------------

void GdalMultiFile::tile_for_google_earth(
   const std::vector<std::string>& File_list,
   const std::string& Fbase, std::vector<std::string>& Out_list,
   std::vector<MapInfo> Out_mapinfo)
{
  const int tile_size = 256;
  GdalMultiFile d1(File_list, 1, false, tile_size, tile_size, tile_size);
  GdalMultiFile d2(File_list, 2, false, tile_size, tile_size, tile_size);
  GdalMultiFile d3(File_list, 3, false, tile_size, tile_size, tile_size);
  std::cerr << "Have " << d1.number_line() / tile_size << " x " 
	    << d1.number_sample() / tile_size << " tiles to generate.\n";
  for(int i = 0; i < d1.number_line() / tile_size; ++i)
    for(int j = 0; j < d1.number_sample() / tile_size; ++j) {
      if(j % 10 == 0)
	std::cerr << "  Doing (" << i << ", " << j << ")\n";
      std::ostringstream os;
      os << Fbase << i << "_" << j << ".png";
      SubMapProjectedImage d1_sub(d1, i * tile_size, j * tile_size, 
				  tile_size, tile_size);
      SubMapProjectedImage d2_sub(d2, i * tile_size, j * tile_size, 
				  tile_size, tile_size);
      SubMapProjectedImage d3_sub(d3, i * tile_size, j * tile_size, 
				  tile_size, tile_size);
      GdalMapProjectedImage::save(os.str(), "PNG", d1_sub, d2_sub, d3_sub,
 				  GDT_Byte, "", true, 0);
//       GdalMapProjectedImage::save(os.str(), "GTiff", d1_sub, d2_sub, d3_sub,
// 				  GDT_Byte, "", true, 0);
      Out_list.push_back(os.str());
      Out_mapinfo.push_back(d1_sub.map_info());
    }
}
