// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "look_vector.h"
%}
%base_import(generic_object)
%import "geocal_time.i"
%include "geocal_time_include.i"
%import "auto_derivative.i"

%geocal_shared_ptr(GeoCal::ScLookVector);
%geocal_shared_ptr(GeoCal::ScLookVectorWithDerivative);
%geocal_shared_ptr(GeoCal::CartesianInertialLookVector);
%geocal_shared_ptr(GeoCal::CartesianInertialLookVectorWithDerivative);
%geocal_shared_ptr(GeoCal::DcsLookVector);
%geocal_shared_ptr(GeoCal::DcsLookVectorWithDerivative);
%geocal_shared_ptr(GeoCal::CartesianFixedLookVectorWithDerivative);

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
  void field_angle(double& OUTPUT, double& OUTPUT) const;
  static create_from_field_angle(double Fa_x_deg, double Fa_y_deg);
  %pickle_serialization();
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
  %pickle_serialization();
};

class CartesianInertialLookVector : public LookVector<double> {
public:
  CartesianInertialLookVector();
  CartesianInertialLookVector(const boost::array<double, 3>& Lv);
  CartesianInertialLookVector(double x, double y, double z);
  CartesianInertialLookVector(const boost::math::quaternion<double>& V);
  std::string print_to_string() const;
  %pickle_serialization();
};

class CartesianInertialLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
  CartesianInertialLookVectorWithDerivative();
  CartesianInertialLookVectorWithDerivative(const boost::array<AutoDerivative<double>, 3>& Lv);
  CartesianInertialLookVectorWithDerivative(const AutoDerivative<double>& x, 
					    const AutoDerivative<double>& y, 
					    const AutoDerivative<double>& z);
  CartesianInertialLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V);
  std::string print_to_string() const;
  %pickle_serialization();
};

// LnLookVector is in ground_coordinate.i instead of here, to break a
// SWIG circular dependency.

// LnLookVectorWithDerivative is in ground_coordinate.i instead of
// here, to break a SWIG circular dependency.

class DcsLookVector : public LookVector<double> {
public:
  DcsLookVector();
  DcsLookVector(double x, double y, double z);
  DcsLookVector(const boost::array<double, 3>& Lv);
  std::string print_to_string() const;
  %pickle_serialization();
};

class DcsLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
  DcsLookVectorWithDerivative();
  DcsLookVectorWithDerivative(const AutoDerivative<double>& x, 
			     const AutoDerivative<double>&  y,
			     const AutoDerivative<double>&  z);
  DcsLookVectorWithDerivative(const boost::array<AutoDerivative<double> , 3>& Lv);
  std::string print_to_string() const;
  %pickle_serialization();
};


class CartesianFixedLookVectorWithDerivative : public LookVector<AutoDerivative<double> > {
public:
  CartesianFixedLookVectorWithDerivative();
  CartesianFixedLookVectorWithDerivative(const boost::array<AutoDerivative<double>, 3>& Lv);
  CartesianFixedLookVectorWithDerivative(const AutoDerivative<double>& x, 
					 const AutoDerivative<double>& y, 
					 const AutoDerivative<double>& z);
  CartesianFixedLookVectorWithDerivative(const boost::math::quaternion<AutoDerivative<double> >& V);
  std::string print_to_string() const;
  %pickle_serialization();
};

// CartesianFixedLookVector is in ground_coordinate.i instead of here,
// to break a SWIG circular dependency.

}
// List of things "import *" will include
%python_export("ScLookVector", "ScLookVectorWithDerivative", "CartesianInertialLookVector", "CartesianInertialLookVectorWithDerivative", "DcsLookVector", "DcsLookVectorWithDerivative", "CartesianFixedLookVectorWithDerivative")

