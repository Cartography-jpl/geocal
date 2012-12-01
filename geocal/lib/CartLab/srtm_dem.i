// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "srtm_dem.h"
%}

%geocal_shared_ptr(SrtmDem);

namespace GeoCal {
class SrtmDem : public DemMapInfo {
public:
  SrtmDem(const std::string& Db_name = "", const std::string& Dirbase ="",
         bool Outside_dem_is_error = true,
	 const boost::shared_ptr<Datum>& D = 
         boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual double elevation(int Y_index, int X_index) const;
  %python_attribute(database_name, std::string)
  %python_attribute(directory_base, std::string)
  %pickle_init(self.database_name, self.directory_base, 
	       self.outside_dem_is_error, self.datum)
};
}
