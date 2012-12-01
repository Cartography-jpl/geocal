// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "hdf_orbit.h"
#include "eci_tod.h"
#include "eci_tod_burl.h"
%}


namespace GeoCal {
template<class PositionType, class TimeCreatorType> class HdfOrbit : public Orbit {
public:
  HdfOrbit(const std::string& Fname, const std::string& Base_group = "Orbit");
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  %python_attribute(file_name, std::string)
  %python_attribute(base_group, std::string)
  %pickle_init(self.file_name, self.base_group)
};
}

%shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>);
%template(HdfOrbit_EciTod_TimeAcs) GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>;
%shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTodBurl, GeoCal::TimeAcsCreator>);
%template(HdfOrbit_EciTodBurl_TimeAcs) GeoCal::HdfOrbit<GeoCal::EciTodBurl, GeoCal::TimeAcsCreator>;
