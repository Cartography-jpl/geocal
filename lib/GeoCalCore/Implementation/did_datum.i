// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "did_datum.h"
%}

%base_import(geocal_datum)
%geocal_shared_ptr(GeoCal::DidDatum);
namespace GeoCal {
class DidDatum : public Datum {
public:
  DidDatum(const std::string& Fname);
  virtual double undulation(const GroundCoordinate& Gc) const;
  %python_attribute(file_name, std::string)
  %pickle_serialization();
};

}
// List of things "import *" will include
%python_export("DidDatum")
