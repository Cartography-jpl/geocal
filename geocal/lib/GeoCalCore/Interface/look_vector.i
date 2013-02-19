// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "look_vector.h"
%}

%geocal_shared_ptr(LookVector);
%geocal_shared_ptr(ScLookVector);
%geocal_shared_ptr(CartesianInertialLookVector);
%geocal_shared_ptr(CartesianFixedLookVector);

namespace GeoCal {
class LookVector {
public:
  %python_attribute_boost_array(look_vector, double, 3);
  %python_attribute(direction, boost::array<double, 3>)
  %python_attribute(length, double)
  std::string print_to_string() const = 0;
};

class ScLookVector : public LookVector {
public:
  ScLookVector();
  ScLookVector(double x, double y, double z);
  ScLookVector(const boost::array<double, 3>& Lv);
  std::string print_to_string() const;
  %pickle_init(self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

class CartesianInertialLookVector : public LookVector {
public:
  CartesianInertialLookVector();
  CartesianInertialLookVector(const boost::array<double, 3>& Lv);
  CartesianInertialLookVector(double x, double y, double z);
  std::string print_to_string() const;
  %pickle_init(self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) CartesianFixedLookVector &OUTPUT (GeoCal::CartesianFixedLookVector temp) {
  $1 = &temp;
 }

 %typemap(argout) CartesianFixedLookVector &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::CartesianFixedLookVector> *), SWIG_POINTER_OWN));
 }

 %typemap(in,numinputs=0) CartesianFixedLookVector &OUTPUT2 (GeoCal::CartesianFixedLookVector temp) {
  $1 = &temp;
 }

 %typemap(argout) CartesianFixedLookVector &OUTPUT2 {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::CartesianFixedLookVector> *), SWIG_POINTER_OWN));
 }
#endif

class CartesianFixedLookVector : public LookVector {
public:
  CartesianFixedLookVector();
  CartesianFixedLookVector(const boost::array<double, 3>& Lv);
  CartesianFixedLookVector(double x, double y, double z);
  std::string print_to_string() const;
  %pickle_init(self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};
}
