#ifndef GEOCAL_AUTODERIVATIVE_QUATERNION_H
#define GEOCAL_AUTODERIVATIVE_QUATERNION_H
#include <boost/math/quaternion.hpp>

namespace GeoCal {
  // Useful to be able to convert to and from a
  // boost::math::quaternion<AutoDerivative<double> > and
  // boost::math::quaternion<double>. Since we don't want to change
  // the base classes in boost, just provide some stand alone
  // functions to do this.

template<class T> inline boost::math::quaternion<T>
value(const boost::math::quaternion<AutoDerivative<T> >& Q)
{
  return boost::math::quaternion<T>(Q.R_component_1().value(),
				    Q.R_component_2().value(),
				    Q.R_component_3().value(),
				    Q.R_component_4().value());
}

template<class T> inline boost::math::quaternion<AutoDerivative<T> >
to_autoderivative(const boost::math::quaternion<T>& Q)
{
  return boost::math::quaternion<AutoDerivative<T> >(Q.R_component_1(),
						     Q.R_component_2(),
						     Q.R_component_3(),
						     Q.R_component_4());
}
  
}
#endif
