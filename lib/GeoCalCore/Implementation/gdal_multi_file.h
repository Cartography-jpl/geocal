#ifndef GDAL_MULTI_FILE_H
#define GDAL_MULTI_FILE_H
#include "map_projected_multifile.h"
#include "location_to_file.h"
#include "memory_raster_image.h"

namespace GeoCal {
/****************************************************************//**
  This gives read access to a large dataset that is represented as a
  number of tiles.  The tiles all need to have the same map
  projection, and all need to have an origin a integral number of
  pixels from a common origin.

  This class gives an interface to this set of VICAR files that looks
  like one really big MapProjectedImage.

  Note that the files might not cover the full extent of the area. If
  you try to read a pixel without a file covering it, then depending
  on the application this is either an error, or you might just want a
  fill value returned. This can be controlled by passing in a true or
  false value for No_coverage_is_error.
*******************************************************************/

class GdalMultiFile : public MapProjectedMultifile {
public:
  GdalMultiFile(const std::vector<std::string>& File_list, int band = 1,
		bool No_coverage_is_error = true,
		int Size_multiple_of = 1, int Number_line_per_tile = -1,
		int Number_sample_per_tile = -1, 
		int Number_tile_each_file = 4, int Number_file = 4,
		int No_coverage_fill_value = 0);
  virtual ~GdalMultiFile() {}
  virtual  MapProjectedMultifileTile get_file(int Line, int Sample) const;
  virtual void print(std::ostream& Os) const;
  static void tile_for_google_earth(const std::vector<std::string>& File_list,
				    const std::string& Fbase,
				    std::vector<std::string>& Out_list,
				    std::vector<MapInfo> Out_mapinfo);
private:
  static const double map_info_tolerance;
  int band_;
  int number_line_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_sample_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_tile_each_file;	///< Number of tiles in a single file.
  LocationToFile loc_to_file;
};

}
#endif
