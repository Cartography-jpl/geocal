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
  SpiceKeplerOrbit();
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) const;
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("SpiceKeplerOrbit")


