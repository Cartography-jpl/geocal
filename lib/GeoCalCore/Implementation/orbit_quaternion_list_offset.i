// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_quaternion_list_offset.h"
%}
%base_import(orbit_quaternion_list)
%geocal_shared_ptr(GeoCal::OrbitQuaternionListOffset);
namespace GeoCal {
class OrbitQuaternionListOffset : public OrbitQuaternionList {
public:
  OrbitQuaternionListOffset(const boost::shared_ptr<OrbitQuaternionList>&
      Orbit_underlying, const blitz::Array<double, 1>&
      Position_offset_sc_coordinate);
  %python_attribute(orbit_underlying, boost::shared_ptr<OrbitQuaternionList>)
     %python_attribute(position_offset_sc_coordinate, blitz::Array<double, 1>);
  %pickle_serialization()
};
}

