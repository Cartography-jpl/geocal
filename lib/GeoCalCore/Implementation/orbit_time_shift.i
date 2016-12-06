// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "orbit_time_shift.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::OrbitTimeShift);

namespace GeoCal {
class OrbitTimeShift : public Orbit {
public:
  OrbitTimeShift(const boost::shared_ptr<Orbit> Orbit_original,
		 double Time_shift);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) const;
  %python_attribute(orbit_original, boost::shared_ptr<Orbit>);
  %python_attribute(time_shift, double);     
  %pickle_serialization();
};
}


