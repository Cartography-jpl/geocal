#ifndef USGS_DEM_H
#define USGS_DEM_H
#include "dem_map_info.h"
#include "raster_multifile.h"
#include "location_to_file.h"
#include "ostream_pad.h"

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
  enum {FILL_VALUE=0};
  UsgsDemData(const std::string& Dir = "",
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
  virtual ~UsgsDemData() { }

//-----------------------------------------------------------------------
/// Database base directory
//-----------------------------------------------------------------------

  const std::string& directory_base() const {return dirbase;}

protected:
  virtual RasterMultifileTile get_file(int Line, int Sample) const;
private:
  std::string dirbase;
  int number_line_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_sample_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_tile_each_file;	///< Number of tiles in a single file.
  LocationToFile loc_to_file;
};

/****************************************************************//**
  This class provides access to the USGS Dem.
*******************************************************************/
class UsgsDem : public DemMapInfo {
public:
  UsgsDem(const std::string& Dir = "",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>());
  virtual ~UsgsDem() {}

//-----------------------------------------------------------------------
/// Database base directory
//-----------------------------------------------------------------------

  const std::string& directory_base() const {return f->directory_base();}

//-----------------------------------------------------------------------
/// Return height in meters relative to datum().
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  { 
    return (*f)(Y_index, X_index);
  }
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<UsgsDemData> f;
};
}
#endif

