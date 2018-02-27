// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "datum_geoid96.h"
%}
%base_import(geocal_datum)
%geocal_shared_ptr(GeoCal::DatumGeoid96);
namespace GeoCal {
class DatumGeoid96 : public Datum {
public:
  DatumGeoid96(const std::string& Fname = "");
  virtual double undulation(const GroundCoordinate& Gc) const;
  %python_attribute(file_name, std::string)
  %pickle_serialization()
};

}
// List of things "import *" will include
%python_export("DatumGeoid96")
