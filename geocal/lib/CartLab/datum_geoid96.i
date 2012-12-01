// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "datum_geoid96.h"
%}

%geocal_shared_ptr(DatumGeoid96);
namespace GeoCal {
class DatumGeoid96 : public Datum {
public:
  DatumGeoid96(const std::string& Fname = "");
  virtual double undulation(const GroundCoordinate& Gc) const;
  %python_attribute(file_name, std::string)
  %pickle_init(self.file_name)
};

}
