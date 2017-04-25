// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "eci_tod.h"
%}
%base_import(ground_coordinate)
%import "eci.i"
%import "geocal_time.i"
%include "geocal_time_include.i"
%geocal_shared_ptr(GeoCal::EciTod);
namespace GeoCal {
class EciTod : public CartesianInertial {
public:
  EciTod(const Eci& Eci_coor, const Time& T);
  EciTod(double X, double Y, double Z);
  EciTod(const boost::array<double, 3>& Pos);
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
  Eci to_eci(const Time& T);
  virtual int naif_code() const;
  %pickle_serialization();
};
}
