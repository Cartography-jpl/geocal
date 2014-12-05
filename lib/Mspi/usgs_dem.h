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
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    // Nothing to do
  }
#endif
};
}

#ifdef USE_BOOST_SERIALIZATON
// This is a little more complicated, because we can't really
// construct a object using a default constructor. So we need to
// directly handle the object construction.
namespace boost { namespace serialization {
template<class Archive> 
inline void save_construct_data(Archive & ar, const GeoCal::UsgsDem* d, 
			 const unsigned int version)
{
  void_cast_register(static_cast<GeoCal::UsgsDem*>(0),
		     static_cast<GeoCal::DemMapInfo*>(0));
  std::string directory_base = d->directory_base();
  bool outside_dem_is_error = d->outside_dem_is_error();
  boost::shared_ptr<GeoCal::Datum> datum = d->datum_ptr();
  ar << GEOCAL_NVP(directory_base)
     << GEOCAL_NVP(outside_dem_is_error)
     << GEOCAL_NVP(datum);
}
template<class Archive>
inline void load_construct_data(Archive & ar, GeoCal::UsgsDem* d,
				const unsigned int version)
{
  void_cast_register(static_cast<GeoCal::UsgsDem*>(0),
		     static_cast<GeoCal::DemMapInfo*>(0));
  std::string directory_base;
  bool outside_dem_is_error;
  boost::shared_ptr<GeoCal::Datum> datum;
  ar >> GEOCAL_NVP(directory_base)
     >> GEOCAL_NVP(outside_dem_is_error)
     >> GEOCAL_NVP(datum);
  ::new(d)GeoCal::UsgsDem(directory_base, outside_dem_is_error,
			  datum);
}
  }
}
#endif

#endif

