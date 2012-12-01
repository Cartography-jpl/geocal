// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "eci_tod_burl.h"
%}
%geocal_shared_ptr(EciTodBurl);
namespace GeoCal {
class EciTodBurl : public CartesianInertial {
public:
  EciTodBurl(double X, double Y, double Z);
  EciTodBurl(const boost::array<double, 3>& Pos);
  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const;
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;
  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const;
  virtual boost::shared_ptr<CartesianInertial>
    reference_surface_intersect_approximate(
    const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
    const;
  %extend {
    static void set_delta_ut1(double v) {GeoCal::EciTodBurl::delta_ut1 = v;}
    static double get_delta_ut1() { return GeoCal::EciTodBurl::delta_ut1; }
  }
  %pickle_init(self.position[0], self.position[1], self.position[2])
};
}
