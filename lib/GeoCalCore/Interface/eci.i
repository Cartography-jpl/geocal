// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "eci.h"
%}
%import "look_vector.i"
%import "ecr.i"
%base_import(ground_coordinate)

%geocal_shared_ptr(GeoCal::Eci);
namespace GeoCal {
class Eci : public CartesianInertial {
public:
  Eci(double X, double Y, double Z);
  Eci(const boost::array<double, 3>& Pos);
  Eci(const Ecr& P, const Time& T);
  Eci(const GroundCoordinate& Gc, const Time& T);
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const;
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;
  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const;
  virtual boost::shared_ptr<CartesianInertial>
    reference_surface_intersect_approximate(
    const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
    const;
  virtual int naif_code() const;
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("Eci")
