// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "did_datum.h"
%}


%geocal_shared_ptr(DidDatum);
namespace GeoCal {
class DidDatum : public Datum {
public:
  DidDatum(const std::string& Fname);
  virtual double undulation(const GroundCoordinate& Gc) const;
  %python_attribute(file_name, std::string)
  %pickle_init(self.file_name)
};

}
