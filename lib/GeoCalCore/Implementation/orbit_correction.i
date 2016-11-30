// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_correction.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::OrbitCorrection);
namespace GeoCal {
%nodefaultctor OrbitCorrection;
class OrbitCorrection: public Orbit {
public:
  %python_attribute(orbit_uncorrected, boost::shared_ptr<Orbit>);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %pickle_serialization();
};
}

