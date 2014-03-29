#include "vicar_multi_file.h"
#include "vicar_lite_file.h"
#include "vicar_raster_image.h"
#include "ibis_file.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
// How far the a particular files map projection can be off without us
// considering this an error. 
//-----------------------------------------------------------------------

const double VicarMultiFile::map_info_tolerance = 0.01;

//-----------------------------------------------------------------------
/// Constructor. This takes the database file (an IBIS file) and the
/// directory that the files pointed to by the database file are
/// located. The directory is needed because although the files are
/// often in the same directory as the database file they aren't
/// always. The database file doesn't contain directory information,
/// just the file name, so we need to pass the directory in.
///
/// For some odd reason, the file names also don't always have the
/// extension on them. For some databases, you need to add something
/// like ".img" to get the actual file name. You can pass any
/// extension that is needed.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_tile. For each
/// file, we read that it tiles with the given Number_line_per_tile
/// and Number_tile_each_file tiles.
///
/// There are two strategies that can be used to reading a Vicar file.
/// One reads a tile of a given number of lines and store it in
/// memory. The other uses memory mapped file io to read data on
/// demand. Often but not always the memory mapped file is faster,
/// particularly for small files or for files with a large number of
/// samples where only a portion is used. You can control which is
/// used by setting Favor_memory_mapped. If the underlying data uses
/// VICAR compression, then we always use the VICAR IO routines since
/// memory mapping won't work. This class handles this case - it
/// checks if the file is compressed before trying to do memory
/// mapping. 
///
///
/// The Force_area_pixel forces the file to be treated as
/// "pixel as area" rather than "pixel as point". This is really just
/// meant as a work around for the SRTM data, which incorrectly labels
/// the data as "point" rather than "area". Since this is a 15 meter
/// difference, it matters for many applications. Most users should
/// just ignore this value.
//-----------------------------------------------------------------------

VicarMultiFile::VicarMultiFile(const std::string& Db_name, 
			       const std::string& Dirbase,
			       const std::string& Extension,
			       int Number_tile_line,
			       int Number_tile_sample,
			       int Number_line_per_tile, 
			       int Number_tile_each_file,
			       int Number_tile,
			       bool Favor_memory_mapped,
			       bool No_coverage_is_error,
			       int No_coverage_fill_value,
			       bool Force_area_pixel)
: RasterMultifile(Number_tile, No_coverage_is_error, 
		  No_coverage_fill_value),
  db_name_(Db_name), dirbase_(Dirbase), extension_(Extension),
  favor_memory_mapped_(Favor_memory_mapped),
  force_area_pixel_(Force_area_pixel),
  number_line_per_tile_(Number_line_per_tile),
  number_tile_each_file_(Number_tile_each_file)
{
  IbisFile ibf(Db_name);
  if(ibf.number_col() < 5)
    throw Exception("Ibis file doesn't have the expected columns");

//-----------------------------------------------------------------------
// Determine extent in latitude and longitude of the area covered.
//-----------------------------------------------------------------------

  float lat_min = ibf.data<float>(0,1);
  float lat_max = lat_min;
  float lon_min = ibf.data<float>(0,2);
  float lon_max = lon_min;
  for(int i = 0; i < ibf.number_row(); ++i) {
    lat_min = std::min(std::min(lat_min, ibf.data<float>(i, 1)), 
		       ibf.data<float>(i, 3));
    lat_max = std::max(std::max(lat_max, ibf.data<float>(i, 1)), 
		       ibf.data<float>(i, 3));
    lon_min = std::min(std::min(lon_min, ibf.data<float>(i, 2)), 
		       ibf.data<float>(i, 4));
    lon_max = std::max(std::max(lon_max, ibf.data<float>(i, 2)), 
		       ibf.data<float>(i, 4));
  }
  if(lon_max >= 180.0)		// Handle border cases.
    lon_max = 179.99999;

//-----------------------------------------------------------------------
// Open the first file we find, and use that to determine the map
// projection pixel size.
//-----------------------------------------------------------------------

  VicarRasterImage m(Dirbase + "/" + ibf.data<std::string>(0,0) +
		     Extension, 1, VicarFile::READ, 100, 4, force_area_pixel_);
  std::vector<boost::shared_ptr<GroundCoordinate> > gp;
  gp.push_back(m.map_info().coordinate_converter().
	       convert_from_coordinate(lon_min, lat_max));
  gp.push_back(m.map_info().coordinate_converter().
	       convert_from_coordinate(lon_max, lat_min));
  map_info_.reset(new MapInfo(m.map_info().cover(gp)));
  number_line_ = map_info_->number_y_pixel();
  number_sample_ = map_info_->number_x_pixel();
  number_tile_line_ = Number_tile_line;
  number_tile_sample_ = Number_tile_sample;

//-----------------------------------------------------------------------
// Now, go back through and set up loc_to_file.
//-----------------------------------------------------------------------

  for(int i = 0; i < ibf.number_row(); ++i) {
    lat_min = std::min(ibf.data<float>(i, 1), ibf.data<float>(i, 3));
    lat_max = std::max(ibf.data<float>(i, 1), ibf.data<float>(i, 3));
    lon_min = std::min(ibf.data<float>(i, 2), ibf.data<float>(i, 4));
    lon_max = std::max(ibf.data<float>(i, 2), ibf.data<float>(i, 4));
    if(lon_max >= 180.0)		// Handle border cases.
      lon_max = 179.99999;
    ImageCoordinate ic1 = 
      coordinate(*m.map_info().coordinate_converter().
		 convert_from_coordinate(lon_min, lat_max));
    ImageCoordinate ic2 = 
      coordinate(*m.map_info().coordinate_converter().
		 convert_from_coordinate(lon_max, lat_min));
    loc_to_file.add((int) round(ic1.line), (int) round(ic1.sample),
		    (int) round(ic2.line), (int) round(ic2.sample),
		    Dirbase + "/" + ibf.data<std::string>(i,0) + Extension);
  }
}

//-----------------------------------------------------------------------
/// Get a file that covers the given location.
//-----------------------------------------------------------------------

RasterMultifileTile VicarMultiFile::get_file(int Line, int Sample) const
{
  std::string fname = loc_to_file.find(Line, Sample);
  if(fname =="") {
    return RasterMultifileTile();
  }

  //-----------------------------------------------------------------------
  // If we are using memory mapped io by preference, try that first.
  // We allow this to fail, the file might be too large to do memory
  // mapped io (particular on the mac, which is limited to 2G).
  //-----------------------------------------------------------------------

  boost::shared_ptr<RasterImage> f;
  try {
    if(favor_memory_mapped_) {
      boost::shared_ptr<VicarLiteRasterImage> 
	f2(new VicarLiteRasterImage(fname, 1, VicarLiteFile::READ,
				    -1, -1, force_area_pixel_));
      if(!f2->is_compressed())  // Can only use memory mapped for
				// uncompressed files.
	f = f2;
    }
  } catch(const Exception& E) {
    // Ignore errors, we drop down to using the Vicar routines below.
  }
  if(!f.get())
    f.reset(new VicarRasterImage(fname, 1, VicarFile::READ,
				 number_line_per_tile_, 
				 number_tile_each_file_,
				 force_area_pixel_));

  //-----------------------------------------------------------------------
  // Check that MapInfo of file aligns with a pixel boundary, and that
  // the pixel resolution is the same as for the VicarMultiFile. We
  // allow a bit of slop here, since roundoff in the geotiff label
  // might be slightly different from one file to the next.
  //-----------------------------------------------------------------------
  
  ImageCoordinate ic = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(0,0))));
  ImageCoordinate ic2 = 
    coordinate(*(f->ground_coordinate(ImageCoordinate(1,1))));
  int ln = (int) round(ic.line);
  int smp = (int) round(ic.sample);
  if(fabs(ic.line - ln) > map_info_tolerance ||
     fabs(ic.sample - smp) > map_info_tolerance) {
    Exception e;
    e << "The file " << fname << " doesn't align with a pixel boundary for this VicarMultiFile";
    throw e;
  }
  if(fabs(ic2.line - (ln + 1)) > map_info_tolerance ||
     fabs(ic2.sample - (smp + 1)) > map_info_tolerance) {
    Exception e;
    e << "The file " << fname << " doesn't have a pixel size that matches VicarMultiFile";
    throw e;
  }
  return RasterMultifileTile(f, ln, smp);
}

