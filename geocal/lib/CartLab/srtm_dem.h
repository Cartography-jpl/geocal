#ifndef SRTM_DEM_H
#define SRTM_DEM_H
#include "dem_map_info.h"
#include "datum_geoid96.h"
#include "vicar_multi_file.h"
namespace GeoCal {
/****************************************************************//**
  This class provides access to the SRTM.
*******************************************************************/

class SrtmDem : public DemMapInfo {
public:
  SrtmDem(const std::string& Db_name = "", const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual ~SrtmDem() {}


//-----------------------------------------------------------------------
/// Database name
//-----------------------------------------------------------------------

  const std::string& database_name() const {return dbname;}

//-----------------------------------------------------------------------
/// Database base directory
//-----------------------------------------------------------------------

  const std::string& directory_base() const {return dirbase;}

//-----------------------------------------------------------------------
/// Return height in meters relative to datum().
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  { 
    return (*f)(Y_index, X_index);
  }

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "SRTM DEM:\n"
       << "  Datum:\n"
       << datum() << "\n"
       << "  Data:\n"
       << *f
       << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
private:
  boost::shared_ptr<VicarMultiFile> f;
  std::string dbname, dirbase;
};

}
#endif
