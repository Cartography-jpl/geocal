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
  %python_attribute(quaternion_orbit_data_time, std::vector<boost::shared_ptr<Time> >);
  boost::shared_ptr<QuaternionOrbitData>
  quaternion_orbit_data_i(int I) const;
  %python_attribute(quaternion_orbit_data_size, int);
  %rename(_v_set_min_time) set_min_time;
  %rename(_v_set_max_time) set_max_time;
  void set_min_time(const Time& T);
  void set_max_time(const Time& T);
%pythoncode {
@property
def min_time(self):
  return self._v_min_time()

@min_time.setter
def min_time(self, value):
  self._v_set_min_time(value)

@property
def max_time(self):
  return self._v_max_time()

@max_time.setter
def max_time(self, value):
  self._v_set_max_time(value)

def quaternion_orbit_data_gen(self):
  '''Generator version of quaternion_orbit_data'''
  for i in range(self.quaternion_orbit_data_size):
      yield self.quaternion_orbit_data_i(i)
}
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
