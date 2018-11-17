// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_des.h"
%}
%base_import(generic_object)
%base_import(orbit)
%import "geocal_time.i"
%include "geocal_time_include.i"
%geocal_shared_ptr(GeoCal::PosCsephb);
%geocal_shared_ptr(GeoCal::OrbitDes);
namespace GeoCal {
class PosCsephb : public GenericObject {
public:
  PosCsephb(const Orbit& Orb, double Tstep);
  PosCsephb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep);
  blitz::Array<double, 1> pos_vel(const Time& T) const;
  blitz::Array<AutoDerivative<double>, 1>
    pos_vel(const TimeWithDerivative& T) const;
  %python_attribute(is_cf, bool);
  %python_attribute(min_time, Time);
  %python_attribute(max_time, Time);
  %python_attribute(time_step, double);
  void test_print(std::ostream& Os) const;
  void test_read(std::istream& Is) const;
  std::string print_to_string() const;
  %pickle_serialization();
};

class OrbitDes: public Orbit {
public:
  OrbitDes(const boost::shared_ptr<PosCsephb>& Pos);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  %python_attribute(pos_csephb, boost::shared_ptr<PosCsephb>);
  %pickle_serialization();
};
 
}
// List of things "import *" will include
%python_export("PosCsephb", "OrbitDes")
