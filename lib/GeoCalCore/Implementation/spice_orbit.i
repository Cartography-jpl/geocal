// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "spice_orbit.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::SpiceOrbit);

namespace GeoCal {
class SpiceOrbit : public Orbit {
public:
  enum {ISS_ID = 25544};
  SpiceOrbit(int Satellite_id, const std::string& Kernel_name="",
	     int Body_id = Ecr::EARTH_NAIF_CODE);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) const;
  %python_attribute(satellite_id, int)
  %pickle_serialization();
};
}


// List of things "import *" will include
%python_export("SpiceOrbit")
