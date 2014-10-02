// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geocal_time.h"
#include "look_vector.h"
%}
%base_import(generic_object)
%import "geocal_time.i"
%import "auto_derivative.i"

%geocal_shared_ptr(GeoCal::ScLookVector);
%geocal_shared_ptr(GeoCal::ScLookVectorWithDerivative);
%geocal_shared_ptr(GeoCal::CartesianInertialLookVector);
%geocal_shared_ptr(GeoCal::DcsLookVector);
%geocal_shared_ptr(GeoCal::DcsLookVectorWithDerivative);

namespace GeoCal {
template<class T> class LookVector : public GenericObject {
public:
  %python_attribute_boost_array(look_vector, T, 3);
  %python_attribute(direction, boost::array<T, 3>)
  %python_attribute_with_set(look_quaternion, boost::math::quaternion<T>)
  %python_attribute(length, T)
  std::string print_to_string() const = 0;
};
}

%geocal_shared_ptr(GeoCal::LookVector<double>);
%geocal_shared_ptr(GeoCal::LookVector<GeoCal::AutoDerivative<double> >);

namespace GeoCal {
%template(LookVectorDouble) GeoCal::LookVector<double>;
%template(LookVectorAutoDerivativeDouble) GeoCal::LookVector<GeoCal::AutoDerivative<double> >;

class ScLookVector : public LookVector<double> {
public:
  ScLookVector();
  ScLookVector(double x, double y, double z);
  ScLookVector(const boost::array<double, 3>& Lv);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

class ScLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
  ScLookVectorWithDerivative();
  ScLookVectorWithDerivative(const AutoDerivative<double>& x, 
			     const AutoDerivative<double>&  y,
			     const AutoDerivative<double>&  z);
  ScLookVectorWithDerivative(const boost::array<AutoDerivative<double> , 3>& Lv);
  ScLookVectorWithDerivative(const ScLookVector& Slv);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

class CartesianInertialLookVector : public LookVector<double> {
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

class DcsLookVector : public LookVector<double> {
public:
  DcsLookVector();
  DcsLookVector(double x, double y, double z);
  DcsLookVector(const boost::array<double, 3>& Lv);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

class DcsLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
  DcsLookVectorWithDerivative();
  DcsLookVectorWithDerivative(const AutoDerivative<double>& x, 
			     const AutoDerivative<double>&  y,
			     const AutoDerivative<double>&  z);
  DcsLookVectorWithDerivative(const boost::array<AutoDerivative<double> , 3>& Lv);
  std::string print_to_string() const;
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};


// CartesianFixedLookVector is in ground_coordinate.i instead of here,
// to break a SWIG circular dependency.

}
