// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_datum.h"
%}
%base_import(generic_object)
%import "look_vector.i"
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::Datum);
%geocal_shared_ptr(GeoCal::SimpleDatum);

namespace GeoCal {
class Datum : public GenericObject {
public:
  virtual double undulation(const GroundCoordinate& Gc) const = 0;
  std::string print_to_string() const;
};

class SimpleDatum : public Datum {
public:
  SimpleDatum(double U = 0.0);
  %python_attribute(u, double)
  virtual double undulation(const GroundCoordinate& Gc) const;
  %pickle_serialization();
};
}
