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

  UsgsDemData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4)
  {
    init(Dir, No_coverage_is_error, Number_line_per_tile, 
	 Number_sample_per_tile, Number_tile_each_file, Number_file);
  }
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
  void init_loc_to_file();
  void init(const std::string& Dir,
	    bool No_coverage_is_error = true,
	    int Number_line_per_tile = -1,
	    int Number_sample_per_tile = -1, 
	    int Number_tile_each_file = 4, int Number_file = 4);

  UsgsDemData() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

/****************************************************************//**
  This class provides access to the USGS Dem.
*******************************************************************/
class UsgsDem : public DemMapInfo {
public:
  UsgsDem(const std::string& Dir,
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>());
  virtual ~UsgsDem() {}

//-----------------------------------------------------------------------
/// Database base directory
//-----------------------------------------------------------------------

  const std::string& directory_base() const {return f->directory_base();}

  virtual double distance_to_surface(const GroundCoordinate& Gp) const
  {
    // Faster to covert Gp to Geodetic and use in both undulation and
    // file lookup.
    Geodetic g(Gp);
    return g.height_reference_surface() - 
      DemMapInfo::height_reference_surface(g);
  }
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const
  {
    // Faster to covert Gp to Geodetic and use in both undulation and
    // file lookup.
    Geodetic g(Gp);
    return DemMapInfo::height_reference_surface(g);
  }

protected:
//-----------------------------------------------------------------------
/// Return height in meters relative to datum().
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  { 
    return f->unchecked_read(Y_index, X_index);
  }
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<UsgsDemData> f;
  UsgsDem() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}

GEOCAL_EXPORT_KEY(UsgsDemData);
GEOCAL_EXPORT_KEY(UsgsDem);
#endif

