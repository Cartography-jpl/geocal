// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "hdf_orbit.h"
#include "eci_tod.h"
%}
%base_import(orbit)
%import "eci_tod.i"
namespace GeoCal {
template<class PositionType, class TimeCreatorType> class HdfOrbit : public Orbit {
public:
  HdfOrbit(const std::string& Fname, const std::string& Base_group = "Orbit");
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(file_name, std::string)
  %python_attribute(base_group, std::string)
  %pickle_init(1, self.file_name, self.base_group)
};
}

%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>);
%template(HdfOrbit_EciTod_TimeAcs) GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>;
