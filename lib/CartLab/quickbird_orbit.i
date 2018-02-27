// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "quickbird_orbit.h"
%}
%base_import(generic_object)
%base_import(orbit)
%import "geocal_time.i"
%include "geocal_time_include.i"
%geocal_shared_ptr(GeoCal::QuickBirdEphemeris);
%geocal_shared_ptr(GeoCal::QuickBirdAttitude);
%geocal_shared_ptr(GeoCal::QuickBirdOrbit);
namespace GeoCal {
class QuickBirdEphemeris : public GenericObject {
public:
  QuickBirdEphemeris(const std::string& Fname);
  %python_attribute(min_time, Time)
  %python_attribute(max_time, Time)
  %python_attribute(time_spacing, double)
  %python_attribute(ephemeris, std::vector<boost::array<double, 12> >)
  %python_attribute(file_name, std::string)
  std::string print_to_string() const;
  %pickle_serialization();
};

class QuickBirdAttitude : public GenericObject {
public:
  QuickBirdAttitude(const std::string& Fname);
  %python_attribute(min_time, Time)
  %python_attribute(max_time, Time)
  %python_attribute(time_spacing, double)
  %python_attribute(attitude, std::vector<boost::array<double, 14> >)
  %python_attribute(file_name, std::string)
  std::string print_to_string() const;
  %pickle_serialization();
};

class QuickBirdOrbit : public Orbit {
public:
  QuickBirdOrbit(const std::string& Ephemeris_file,
		 const std::string& Attitude_file = "");
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(ephemeris_file_name, std::string)
  %python_attribute(attitude_file_name, std::string)
  %pickle_serialization();
};

}
// List of things "import *" will include
%python_export("QuickBirdOrbit")
