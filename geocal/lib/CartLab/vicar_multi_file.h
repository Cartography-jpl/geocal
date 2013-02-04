#ifndef VICAR_MULTI_FILE_H
#define VICAR_MULTI_FILE_H
#include "raster_multifile.h"
#include "location_to_file.h"

namespace GeoCal {
/****************************************************************//**
  This gives read access to some of the larger AFIDS datasets, such as
  CIB and SRTM.

  These larger datasets split into a number of VICAR files. Each one
  of these is a some extent in latitude/longitude, for example 1 x 1
  degree tiles for the SRTM. There is an index IBIS file that gives a
  listing of all the input datasets and where they are located.

  This class gives an interface to this set of VICAR files that looks
  like one really big RasterImage.

  Note that the files might not cover the full extent of area, for
  example one of the 1x1 degree SRTM tiles might be missing. If we
  attempt to access the area covered by a missing file then an error
  is thrown. We may want to come up with a more robust interface, but
  for now this interface seems to be sufficient.
*******************************************************************/

class VicarMultiFile : public RasterMultifile {
public:
  VicarMultiFile(const std::string& Db_name, const std::string& Dirbase,
		 const std::string& Extension = "",
		 int Number_tile_line = 10000, 
		 int Number_tile_sample = 10000, 
		 int Number_line_per_tile = 100000, 
		 int Number_tile_each_file = 4,
		 int Number_file = 4, bool Favor_memory_mapped = true,
		 bool No_coverage_is_error = true, 
		 int No_coverage_fill_value = 0,
		 bool Force_area_pixel = false);
  virtual ~VicarMultiFile() {}
  virtual  RasterMultifileTile get_file(int Line, int Sample) const;

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "VicarMultifile " << number_line() << " x " << number_sample() 
       << "\n"
       << "  Database file:  " << db_name << "\n"
       << "  Directory base: " << dirbase << "\n"
       << "  File exention:  " << extension << "\n"
       << "  Map info:       " << map_info() << "\n"
       << "  Force area based pixel: " << force_area_pixel_ << "\n"
       << "  Favor memory mapped: " << favor_memory_mapped << "\n";
  }
private:
  static const double map_info_tolerance;
  std::string db_name, dirbase, extension;
  bool favor_memory_mapped;	///< Whether we use memory mapping or
				///not when reading an uncompressed
				///file. 
  bool force_area_pixel_;
  int number_line_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_tile_each_file;	///< Number of tiles in a single file.
  LocationToFile loc_to_file;
};
}
#endif
