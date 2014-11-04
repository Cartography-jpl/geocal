#ifndef AUTO_DERIVATIVE_QUATERNION_H
#define AUTO_DERIVATIVE_QUATERNION_H
#include "auto_derivative.h"
#include <boost/math/quaternion.hpp>

namespace GeoCal {
  /// Multiple quaternion<double> with
  /// quaternion<AutoDerivative<double> >
inline boost::math::quaternion<AutoDerivative<double> > 
operator*(const boost::math::quaternion<double>& lhs,
	  const boost::math::quaternion<AutoDerivative<double> >& rhs)
{
  double a = lhs.R_component_1();
  double b = lhs.R_component_2();
  double c = lhs.R_component_3();
  double d = lhs.R_component_4();
  AutoDerivative<double> ar = rhs.R_component_1();
  AutoDerivative<double> br = rhs.R_component_2();
  AutoDerivative<double> cr = rhs.R_component_3();
  AutoDerivative<double> dr = rhs.R_component_4();
  AutoDerivative<double> at = +a*ar-b*br-c*cr-d*dr;
  AutoDerivative<double> bt = +a*br+b*ar+c*dr-d*cr;
  AutoDerivative<double> ct = +a*cr-b*dr+c*ar+d*br;
  AutoDerivative<double> dt = +a*dr+b*cr-c*br+d*ar;
  return boost::math::quaternion<AutoDerivative<double> >(at, bt, ct, dt);
}

inline boost::math::quaternion<AutoDerivative<double> > 
operator*(const boost::math::quaternion<AutoDerivative<double> >& lhs,
	  const boost::math::quaternion<double>& rhs)
{
  AutoDerivative<double> a = lhs.R_component_1();
  AutoDerivative<double> b = lhs.R_component_2();
  AutoDerivative<double> c = lhs.R_component_3();
  AutoDerivative<double> d = lhs.R_component_4();
  double ar = rhs.R_component_1();
  double br = rhs.R_component_2();
  double cr = rhs.R_component_3();
  double dr = rhs.R_component_4();
  AutoDerivative<double> at = a*ar-b*br-c*cr-d*dr;
  AutoDerivative<double> bt = a*br+b*ar+c*dr-d*cr;
  AutoDerivative<double> ct = a*cr-b*dr+c*ar+d*br;
  AutoDerivative<double> dt = a*dr+b*cr-c*br+d*ar;
  return boost::math::quaternion<AutoDerivative<double> >(at, bt, ct, dt);
}

}
#endif
