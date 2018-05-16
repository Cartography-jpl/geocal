// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
  HdfOrbit(const std::string& Fname, const std::string& Base_group = "Orbit",
	   const std::string& Eph_time = "/Ephemeris/Time",
	   const std::string& Eph_pos = "/Ephemeris/Position",
	   const std::string& Eph_vel = "/Ephemeris/Velocity",
	   const std::string& Att_time = "/Attitude/Time",
	   const std::string& Att_quat = "/Attitude/Quaternion"
	   );
  %python_attribute(file_name, std::string)
  %python_attribute(base_group, std::string)
  %pickle_serialization()
};
}

%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>);
%template(HdfOrbit_EciTod_TimeAcs) GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeAcsCreator>;
%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::Eci, GeoCal::TimePgsCreator>);
%template(HdfOrbit_Eci_TimePgs) GeoCal::HdfOrbit<GeoCal::Eci, GeoCal::TimePgsCreator>;
%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::Eci, GeoCal::TimeJ2000Creator>);
%template(HdfOrbit_Eci_TimeJ2000) GeoCal::HdfOrbit<GeoCal::Eci, GeoCal::TimeJ2000Creator>;
%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeJ2000Creator>);
%template(HdfOrbit_EciTod_TimeJ2000) GeoCal::HdfOrbit<GeoCal::EciTod, GeoCal::TimeJ2000Creator>;

// List of things "import *" will include
%python_export("HdfOrbit_EciTod_TimeAcs", "HdfOrbit_Eci_TimePgs", "HdfOrbit_Eci_TimeJ2000", "HdfOrbit_EciTod_TimeJ2000")
