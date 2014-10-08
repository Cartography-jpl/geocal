// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "ground_coordinate.h"
%}
%base_import(generic_object)
%base_import(look_vector)
%import "geocal_time.i"
%geocal_shared_ptr(GeoCal::GroundCoordinate);
%geocal_shared_ptr(GeoCal::CartesianFixed);
%geocal_shared_ptr(GeoCal::CartesianInertial);
%geocal_shared_ptr(GeoCal::LnLookVector);
%geocal_shared_ptr(GeoCal::CartesianFixedLookVector);

namespace GeoCal {
class CartesianFixed;
class CartesianFixedLookVector;
class GroundCoordinate : public GenericObject {
public:
  %python_attribute(latitude, virtual double)
  %python_attribute(longitude, virtual double)
  %python_attribute(height_reference_surface, virtual double)
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const = 0;
  std::string print_to_string() const;
};

class CartesianInertial : public GenericObject {
public:
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const = 0;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const = 0;
  boost::math::quaternion<double> ci_to_cf_quat(const Time& T) const;
  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const = 0;
  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const = 0;
  std::string print_to_string() const;
  %python_attribute_boost_array(position, double, 3)
};

class CartesianFixed : public GroundCoordinate {
public:
  virtual boost::shared_ptr<CartesianInertial> convert_to_ci(const Time& T) 
    const = 0;
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual boost::shared_ptr<CartesianFixed> 
    create(boost::array<double, 3> P) const = 0;
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const = 0;
  boost::math::quaternion<double> ci_to_cf_quat(const Time& T) const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0) 
  const = 0;
  %python_attribute(min_radius_reference_surface, virtual double)
  %python_attribute_boost_array(position, double, 3)
};

  // Note this is from look_vector.h, but it is placed here to break
  // a circular dependency with swig.
class LnLookVector : public GeoCal::LookVector<double> {
public:
  LnLookVector();
  LnLookVector(const CartesianFixedLookVector& Lv, 
	       const GroundCoordinate& Ref_pt);
  LnLookVector(double x, double y, double z);
  LnLookVector(const boost::array<double, 3>& Lv);
  CartesianFixedLookVector to_cf(const GroundCoordinate& Ref_pt) const;
  std::string print_to_string() const;
  static boost::math::quaternion<double> 
  cf_to_enu(const GroundCoordinate& Ref_pt);
  static boost::math::quaternion<double> 
      enu_to_cf(const GroundCoordinate& Ref_pt);
  static LnLookVector solar_look_vector(const Time& T, 
					const GroundCoordinate& Ref_pt);
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) CartesianFixedLookVector &OUTPUT (GeoCal::CartesianFixedLookVector temp) {
  $1 = &temp;
 }

 %typemap(argout) CartesianFixedLookVector &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::CartesianFixedLookVector> *), SWIG_POINTER_OWN));
 }

 %typemap(in,numinputs=0) CartesianFixedLookVector &OUTPUT2 (GeoCal::CartesianFixedLookVector temp) {
  $1 = &temp;
 }

 %typemap(argout) CartesianFixedLookVector &OUTPUT2 {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::CartesianFixedLookVector> *), SWIG_POINTER_OWN));
 }
#endif

class CartesianFixedLookVector : public LookVector<double> {
public:
  CartesianFixedLookVector();
  CartesianFixedLookVector(const boost::array<double, 3>& Lv);
  CartesianFixedLookVector(double x, double y, double z);
  CartesianFixedLookVector(const GroundCoordinate& From,
			   const GroundCoordinate& To);
  // This conflicts in python with the version that take a boost
  // array. For now comment this out. We may want to investigate this
  // at some point, not sure where the conflict comes from
  // CartesianFixedLookVector(const boost::math::quaternion<double>& V);
  std::string print_to_string() const;
  static CartesianFixedLookVector solar_look_vector(const Time& T);
  %pickle_init(1, self.look_vector[0], self.look_vector[1],
	       self.look_vector[2])
};

double distance(const GroundCoordinate& G1, const GroundCoordinate& G2);

}

%template(Vector_GroundCoordinate) std::vector<boost::shared_ptr<GeoCal::GroundCoordinate> >;
