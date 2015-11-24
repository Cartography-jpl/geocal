// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "tle_orbit.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::TleOrbit);

namespace GeoCal {
class TleOrbit : public Orbit {
public:
  TleOrbit(const std::string& Tle);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) const;
  %python_attribute(tle, std::string)
  %pickle_serialization();
};
}

