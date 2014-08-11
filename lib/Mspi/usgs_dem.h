#ifndef USGS_DEM_H
#define USGS_DEM_H
#include "dem_map_info.h"
#include "did_datum.h"
#include "raster_multifile.h"
#include "location_to_file.h"

namespace GeoCal {
/****************************************************************//**
  This is used to read the USGS DEM data. This then get used by
  UsgsDem. Although you can use this class directly, generally you'll 
  use this through UsgsDem class.

  If we don't have data for a particular location, we return
  FILL_VALUE. 
*******************************************************************/
class UsgsDemData: public RasterMultifile {
public:
  enum {FILL_VALUE=-9999};
  UsgsDemData(const std::string& Dir = "",
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
  virtual ~UsgsDemData() { }
protected:
  virtual RasterMultifileTile get_file(int Line, int Sample) const;
private:
  int number_line_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_sample_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_tile_each_file;	///< Number of tiles in a single file.
  LocationToFile loc_to_file;
};
}
#endif

