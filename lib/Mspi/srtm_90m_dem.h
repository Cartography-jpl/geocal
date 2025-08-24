#ifndef SRTM_90M_DEM_H
#define SRTM_90M_DEM_H
#include "dem_map_info.h"
#include "cart_lab_multifile.h"

namespace GeoCal {
/****************************************************************//**
  This is used to read the DEM data. This then get used by
  Srtm90mDem. Although you can use this class directly, generally you'll 
  use this through Srtm90mDem class.

  If we don't have data for a particular location, we return
  FILL_VALUE. 
*******************************************************************/
class Srtm90mData: public GdalCartLabMultifile {
public:
  enum {FILL_VALUE=0};

  Srtm90mData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
  virtual ~Srtm90mData() { }
private:
  Srtm90mData() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This class provides access to the SRTM 90m Dem.
*******************************************************************/
class Srtm90mDem : public DemMapInfo {
public:
  Srtm90mDem(const std::string& Dir,
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>());
  virtual ~Srtm90mDem() {}

  virtual void change_to_geodetic360()
  {
    f->change_to_geodetic360();
    map_info_ = f->map_info();
  }
  
  virtual void change_to_geodetic()
  {
    f->change_to_geodetic();
    map_info_ = f->map_info();
  }
  
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

//-----------------------------------------------------------------------
/// Number of files we have read.
//-----------------------------------------------------------------------

  int number_file_read() const { return f->number_file_read(); }

//-----------------------------------------------------------------------
/// Set number of files we have read back to zero.
//-----------------------------------------------------------------------

  void reset_number_file_read() { f->reset_number_file_read(); }

  virtual void print(std::ostream& Os) const;
protected:
//-----------------------------------------------------------------------
/// Return height in meters relative to datum().
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  { 
    return f->unchecked_read(Y_index, X_index);
  }
private:
  boost::shared_ptr<Srtm90mData> f;
  Srtm90mDem() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}

GEOCAL_EXPORT_KEY(Srtm90mData);
GEOCAL_EXPORT_KEY(Srtm90mDem);
#endif

