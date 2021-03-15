// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_array.h"
#include "eci_tod.h"
%}
%base_import(orbit_quaternion_list)
%import "eci_tod.i"
namespace GeoCal {
template<class PositionType, class TimeCreatorType> class OrbitArray : 
    public OrbitQuaternionList {
public:
  OrbitArray(const blitz::Array<double, 1>& Eph_time,
	     const blitz::Array<double, 2>& Eph_pos,
	     const blitz::Array<double, 2>& Eph_vel,
	     const blitz::Array<double, 1>& Att_time,
	     const blitz::Array<double, 2>& Att_quat,
	     bool Att_from_sc_to_ref_frame = true,
	     bool Serialize_data = true);
  %pickle_serialization()
};
}

%geocal_shared_ptr(GeoCal::OrbitArray<GeoCal::EciTod, GeoCal::TimeAcsCreator>);
%template(OrbitArray_EciTod_TimeAcs) GeoCal::OrbitArray<GeoCal::EciTod, GeoCal::TimeAcsCreator>;
%geocal_shared_ptr(GeoCal::OrbitArray<GeoCal::Eci, GeoCal::TimePgsCreator>);
%template(OrbitArray_Eci_TimePgs) GeoCal::OrbitArray<GeoCal::Eci, GeoCal::TimePgsCreator>;
%geocal_shared_ptr(GeoCal::OrbitArray<GeoCal::Eci, GeoCal::TimeJ2000Creator>);
%template(OrbitArray_Eci_TimeJ2000) GeoCal::OrbitArray<GeoCal::Eci, GeoCal::TimeJ2000Creator>;
%geocal_shared_ptr(GeoCal::OrbitArray<GeoCal::EciTod, GeoCal::TimeJ2000Creator>);
%template(OrbitArray_EciTod_TimeJ2000) GeoCal::OrbitArray<GeoCal::EciTod, GeoCal::TimeJ2000Creator>;

// List of things "import *" will include
%python_export("OrbitArray_EciTod_TimeAcs", "OrbitArray_Eci_TimePgs", "OrbitArray_Eci_TimeJ2000", "OrbitArray_EciTod_TimeJ2000")
