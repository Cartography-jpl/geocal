// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "spice_kepler_orbit.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::SpiceKeplerOrbit);

namespace GeoCal {
class SpiceKeplerOrbit : public Orbit {
public:
  SpiceKeplerOrbit(const blitz::Array<double, 1>& elements, Time min_time = Time::min_valid_time,
		   Time max_time = Time::max_valid_time);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) const;
  static double mu(const std::string& Body_name);
  static blitz::Array<double, 1> spice_conics(const blitz::Array<double, 1>& elements, Time T);
  static blitz::Array<double, 1> spice_oscelt(const OrbitData& Od);
  %python_attribute(elements, blitz::Array<double, 1>);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("SpiceKeplerOrbit")


