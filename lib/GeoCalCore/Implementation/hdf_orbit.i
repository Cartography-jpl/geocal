// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "hdf_orbit.h"
#include "eci_tod.h"
%}
%base_import(orbit_quaternion_list)
%import "eci_tod.i"
namespace GeoCal {
template<class PositionType, class TimeCreatorType> class HdfOrbit : 
    public OrbitQuaternionList {
public:
  HdfOrbit(const std::string& Fname, const std::string& Base_group = "Orbit");
  %python_attribute(file_name, std::string)
  %python_attribute(base_group, std::string)
  %pickle_serialization()
};
}

%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>);
%template(HdfOrbit_EciTod_TimeAcs) GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>;
