// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "orbit_quaternion_list.h"
%}

%geocal_shared_ptr(OrbitQuaternionList);

namespace GeoCal {
class OrbitQuaternionList : public Orbit {
public:
  OrbitQuaternionList(const 
  std::vector<boost::shared_ptr<QuaternionOrbitData> >& Data);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
};
}
