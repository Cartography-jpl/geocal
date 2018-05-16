// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_quaternion_list_offset.h"
%}
%base_import(orbit_quaternion_list)
%base_import(orbit)
%geocal_shared_ptr(GeoCal::OrbitQuaternionListOffset);
%geocal_shared_ptr(GeoCal::OrbitScCoorOffset);
namespace GeoCal {
class OrbitQuaternionListOffset : public OrbitQuaternionList {
public:
  OrbitQuaternionListOffset(const boost::shared_ptr<OrbitQuaternionList>&
      Orbit_underlying, const blitz::Array<double, 1>&
      Position_offset_sc_coordinate);
  %python_attribute(orbit_underlying, boost::shared_ptr<OrbitQuaternionList>);
  %python_attribute(position_offset_sc_coordinate, blitz::Array<double, 1>);
  %pickle_serialization()
};

class OrbitScCoorOffset : public Orbit {
public:
  OrbitScCoorOffset(const boost::shared_ptr<Orbit>&
      Orbit_underlying, const blitz::Array<double, 1>&
      Position_offset_sc_coordinate);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(orbit_underlying, boost::shared_ptr<Orbit>);
  %python_attribute(position_offset_sc_coordinate, blitz::Array<double, 1>);
  %pickle_serialization()
}; 
}

// List of things "import *" will include
%python_export("OrbitQuaternionListOffset", "OrbitScCoorOffset")

