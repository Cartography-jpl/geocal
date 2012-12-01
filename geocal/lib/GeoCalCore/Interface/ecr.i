// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "ecr.h"
%}

%geocal_shared_ptr(Ecr);
namespace GeoCal {
class CartesianFixed;
class Geodetic;

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) Ecr &OUTPUT (GeoCal::Ecr temp) {
  $1 = &temp;
 }

 %typemap(argout) Ecr &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::Ecr> *), SWIG_POINTER_OWN));
 }
#endif

class Ecr : public CartesianFixed {
public:
  Ecr(const GroundCoordinate& Gc);
  Ecr(double X, double Y, double Z);
  Ecr(const boost::array<double, 3>& Pos);
  Ecr();
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual boost::shared_ptr<CartesianInertial> convert_to_ci(const Time& T) 
  const;
  virtual boost::shared_ptr<CartesianFixed> 
  create(boost::array<double, 3> P) const;
  Geodetic convert_to_geodetic() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0) 
  const;
  %pickle_init(self.position[0], self.position[1], self.position[2])
};

}
