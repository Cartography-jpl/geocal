// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_quaternion_list.h"
%}
%base_import(orbit)
%import "time_table.i"
%geocal_shared_ptr(GeoCal::OrbitQuaternionList);
%geocal_shared_ptr(GeoCal::OrbitListCache);

namespace GeoCal {
class OrbitQuaternionList : public Orbit {
public:
  OrbitQuaternionList(const 
  std::vector<boost::shared_ptr<QuaternionOrbitData> >& Data);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(quaternion_orbit_data, std::vector<boost::shared_ptr<QuaternionOrbitData> >)
  %python_attribute(quaternion_orbit_data_time, std::vector<Time>);
  boost::shared_ptr<QuaternionOrbitData>
  quaternion_orbit_data_i(int I) const;
protected:
  virtual void interpolate_or_extrapolate_data
  (Time T, const QuaternionOrbitData* Q1, const QuaternionOrbitData* Q2) const;
  virtual boost::shared_ptr<QuaternionOrbitData> orbit_data_create(Time T) 
    const;
};

class OrbitListCache: public OrbitQuaternionList {
public:
  OrbitListCache(const boost::shared_ptr<Orbit>& Orbit_underlying,
		 const boost::shared_ptr<TimeTable>& Tt,
		 double Sample = 0.0);
  %python_attribute(orbit_underlying, boost::shared_ptr<Orbit>);
  %python_attribute(time_table, boost::shared_ptr<TimeTable>);
  %python_attribute(sample, double);
  %pickle_serialization()
};
  
}

// List of things "import *" will include
%python_export("OrbitQuaternionList", "OrbitListCache")
