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
protected:
  // Needs to be protected to derived classes can use serialization
  // with this.
  GdalDem() {}
private:
  std::string fname;
  int band_id_;
  boost::shared_ptr<GdalBase> gdal;
  
  void initialize(const std::string& Fname, const boost::shared_ptr<Datum>& D,
		  int Band_id, bool Outside_dem_is_error, 
		  int Number_tile, int Tile_number_line, int Tile_number_sample);

//-----------------------------------------------------------------------
/// Type dispatched initialization
//-----------------------------------------------------------------------

  template<class T> void initialize_type(const std::string& Fname,
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
			     t->linear_unit_scale(), t->no_data_value());
  }
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}
GEOCAL_EXPORT_KEY(GdalDem);
#endif
