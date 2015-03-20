#ifndef GDAL_DEM_H
#define GDAL_DEM_H
#include "dem_tiled_file.h"
#include "did_datum.h"
#include "geocal_gdal.h"
#include "ostream_pad.h"

namespace GeoCal {
/****************************************************************//**
  This uses a Gdal<T> as a DEM.

  The type of T should be one we can convert to a double, and should
  give the height in meters above sea level. We do a bilinear
  interpolation to get values between points, and we optionally return
  a height of 0 outside of the range of where we have data (or
  alternatively, throw an exception).

  Right now, the datum needs to be passed in. I don't believe there is
  any information available in the files to let us know which datum to
  use.
*******************************************************************/

class GdalDem : public DemTiledFile {
public:
  GdalDem(const std::string& Fname, const boost::shared_ptr<Datum>& D =
	  boost::shared_ptr<Datum>(new NoDatum()),
	  int Band_id = 1, bool Outside_dem_is_error = false, 
	  int Number_tile = 4, int Tile_number_line = -1,
	  int Tile_number_sample = -1);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GdalDem() {}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { 
    OstreamPad opad(Os, "    ");
    Os << "GDAL Dem:\n"
       << "  File:    " << *gdal
       << "  Band id: " << band_id() << "\n"
       << "  Map info:\n";
    opad << map_info();
    opad.strict_sync();
    Os << "  Datum:\n";
    opad << datum();
    opad.strict_sync();
    Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
  int band_id() const {return band_id_;}
  const std::string& file_name() const {return fname;}
private:
  std::string fname;
  int band_id_;
  boost::shared_ptr<GdalBase> gdal;

//-----------------------------------------------------------------------
/// Type dispatched initialization
//-----------------------------------------------------------------------

  template<class T> void initialize(const std::string& Fname,
				    const boost::shared_ptr<Datum>& D,
				    int Band_id, 
				    int Number_tile,
				    bool Outside_dem_is_error,
				    int Tile_number_line,
				    int Tile_number_sample)
  {
    boost::shared_ptr<Gdal<T> > t(new Gdal<T>(Fname, Band_id, Number_tile, 
					      false,
					      Tile_number_line, 
					      Tile_number_sample));
    gdal = t;
    DemTiledFile::initialize(t, D, t->map_info(), Outside_dem_is_error,
			     t->linear_unit_scale());
  }
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
inline void save_construct_data(Archive & ar, const GeoCal::GdalDem* d, 
			 const unsigned int version)
{
  void_cast_register(static_cast<GeoCal::GdalDem*>(0),
		     static_cast<GeoCal::DemTiledFile*>(0));
  void_cast_register(static_cast<GeoCal::DemTiledFile*>(0),
		     static_cast<GeoCal::DemMapInfo*>(0));
  void_cast_register(static_cast<GeoCal::DemMapInfo*>(0),
		     static_cast<GeoCal::Dem*>(0));
  std::string file_name = d->file_name();
  boost::shared_ptr<GeoCal::Datum> datum = d->datum_ptr();
  int band_id = d->band_id();
  bool outside_dem_is_error = d->outside_dem_is_error();
  int number_tile = d->number_tile();
  int number_tile_line = d->number_line_per_tile();
  int number_tile_sample = d->number_sample_per_tile();
  ar << GEOCAL_NVP(file_name)
     << GEOCAL_NVP(band_id)
     << GEOCAL_NVP(datum)
     << GEOCAL_NVP(outside_dem_is_error)
     << GEOCAL_NVP(number_tile)
     << GEOCAL_NVP(number_tile_line)
     << GEOCAL_NVP(number_tile_sample);
}
template<class Archive>
inline void load_construct_data(Archive & ar, GeoCal::GdalDem* d,
				const unsigned int version)
{
  void_cast_register(static_cast<GeoCal::GdalDem*>(0),
		     static_cast<GeoCal::DemTiledFile*>(0));
  void_cast_register(static_cast<GeoCal::DemTiledFile*>(0),
		     static_cast<GeoCal::DemMapInfo*>(0));
  void_cast_register(static_cast<GeoCal::DemMapInfo*>(0),
		     static_cast<GeoCal::Dem*>(0));
  std::string file_name;
  int band_id, number_tile, number_tile_line, number_tile_sample;
  bool outside_dem_is_error;
  boost::shared_ptr<GeoCal::Datum> datum;
  ar >> GEOCAL_NVP(file_name)
     >> GEOCAL_NVP(band_id)
     >> GEOCAL_NVP(datum)
     >> GEOCAL_NVP(outside_dem_is_error)
     >> GEOCAL_NVP(number_tile)
     >> GEOCAL_NVP(number_tile_line)
     >> GEOCAL_NVP(number_tile_sample);
  ::new(d)GeoCal::GdalDem(file_name, datum, band_id, outside_dem_is_error,
			  number_tile, number_tile_line, number_tile_sample);
}
  }
}
#endif

#endif
