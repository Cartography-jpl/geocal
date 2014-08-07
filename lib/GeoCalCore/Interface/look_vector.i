// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geocal_time.h"
#include "look_vector.h"
%}
%base_import(generic_object)
%import "geocal_time.i"

%geocal_shared_ptr(GeoCal::LookVector);
%geocal_shared_ptr(GeoCal::ScLookVector);
%geocal_shared_ptr(GeoCal::CartesianInertialLookVector);
%geocal_shared_ptr(GeoCal::DcsLookVector);

namespace GeoCal {
class LookVector : public GenericObject {
public:
  %python_attribute_boost_array(look_vector, double, 3);
  %python_attribute(direction, boost::array<double, 3>)
  %python_attribute_with_set(look_quaternion, boost::math::quaternion<double>)
  %python_attribute(length, double)
  std::string print_to_string() const = 0;
};

class ScLookVector : public LookVector {
public:
  ScLookVector();
  ScLookVector(double x, double y, double z);
  ScLookVector(const boost::array<double, 3>& Lv);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

class CartesianInertialLookVector : public LookVector {
public:
  CartesianInertialLookVector();
  CartesianInertialLookVector(const boost::array<double, 3>& Lv);
  CartesianInertialLookVector(double x, double y, double z);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

// LnLookVector is in ground_coordinate.i instead of here, to break a
// SWIG circular dependency.

class DcsLookVector : public LookVector {
public:
  DcsLookVector();
  DcsLookVector(double x, double y, double z);
  DcsLookVector(const boost::array<double, 3>& Lv);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};


// CartesianFixedLookVector is in ground_coordinate.i instead of here,
// to break a SWIG circular dependency.

}
