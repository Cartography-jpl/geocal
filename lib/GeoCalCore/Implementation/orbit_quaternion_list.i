// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_quaternion_list.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::OrbitQuaternionList);

namespace GeoCal {
class OrbitQuaternionList : public Orbit {
public:
  OrbitQuaternionList(const 
  std::vector<boost::shared_ptr<QuaternionOrbitData> >& Data);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(quaternion_orbit_data, std::vector<boost::shared_ptr<QuaternionOrbitData> >)
  %pickle_init(1, self.quaternion_orbit_data);
protected:
  virtual boost::shared_ptr<QuaternionOrbitData> orbit_data_create(Time T) 
    const;
};
}
