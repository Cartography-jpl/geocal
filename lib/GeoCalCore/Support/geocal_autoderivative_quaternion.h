#ifndef GEOCAL_AUTODERIVATIVE_QUATERNION_H
#define GEOCAL_AUTODERIVATIVE_QUATERNION_H
#include "auto_derivative.h"
#include "geocal_quaternion.h"

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
BOOST_SERIALIZATION_SPLIT_FREE(boost::math::quaternion<GeoCal::AutoDerivative<double> >);
#endif

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

// Specialization of matrix_to_quaternion. This just makes sure we
// do comparisons with value.

template<> inline boost::math::quaternion<AutoDerivative<double> > 
matrix_to_quaternion(const blitz::Array<AutoDerivative<double> , 2>& m)
{
  if(m.rows() != 3 || m.cols() != 3)
    throw GeoCal::Exception("Matrix must be 3x3");
  AutoDerivative<double>  t = m(0, 0) + m(1, 1) + m(2, 2);
  if(t.value() > 1e-8) {
    AutoDerivative<double>  s = std::sqrt(t + 1) * 2;
    return boost::math::quaternion<AutoDerivative<double> >(-s / 4.0,
					   -(m(2, 1) - m(1, 2)) / s,
					   -(m(0, 2) - m(2, 0)) / s,
					   -(m(1, 0) - m(0, 1)) / s);
  }
  if(m(0, 0).value() > m(1, 1).value() && m(0, 0).value() > m(2, 2).value()) {
    AutoDerivative<double>  s = std::sqrt(1.0 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
    return boost::math::quaternion<AutoDerivative<double> >(-(m(2, 1) - m(1, 2)) / s,
					   -s / 4.0,
					   -(m(0, 1) + m(1, 0)) / s,
					   -(m(0, 2) + m(2, 0)) / s);
  } else if(m(1, 1).value() > m(2, 2).value()) {
    AutoDerivative<double>  s = std::sqrt(1.0 - m(0, 0) + m(1, 1) - m(2, 2)) * 2;
    return boost::math::quaternion<AutoDerivative<double> >(-(m(0, 2) - m(2, 0)) / s,
					   -(m(0, 1) + m(1, 0)) / s,
					   -s / 4.0,
					   -(m(1, 2) + m(2, 1)) / s);
  } else {
    AutoDerivative<double>  s = std::sqrt(1.0 - m(0, 0) - m(1, 1) + m(2, 2)) * 2;
    return boost::math::quaternion<AutoDerivative<double> >(-(m(1, 0) - m(0, 1)) / s,
					   -(m(2, 0) + m(0, 2)) / s,
					   -(m(1, 2) + m(2, 1)) / s,
					   -s / 4.0);
  }
}

inline boost::math::quaternion<AutoDerivative<double> > interpolate_quaternion_rotation
(const boost::math::quaternion<AutoDerivative<double> >& Q1, 
 const boost::math::quaternion<AutoDerivative<double> >& Q2,
 const AutoDerivative<double>& toffset, double tspace)
{
  boost::math::quaternion<AutoDerivative<double> > delta_quat = Q2 * conj(Q1);
  AutoDerivative<double> t = delta_quat.R_component_1();
  // Handle t being slightly out of range due to round off.
  t.value() = (t.value() > 1 ? 1 : (t.value() < -1 ? -1 : t.value())); 
  if(t.value() < 0) {
    // Switch sign of Q2 to give same rotation, but easier to
    // interpolate. 
    delta_quat = -delta_quat;
    t = -t;
  }
  AutoDerivative<double> delta_ang = 2.0 * std::acos(t);
  AutoDerivative<double> d_ang = delta_ang * toffset / tspace;
  // Handle degenerate case of Q1 and Q2 almost the same.
  // Note unlike the quaternion<double> case, we can't just return Q1
  // because this doesn't have the right derivatives. Instead, we use
  // the small angle approximation to avoid doing a divide by zero.
  if(delta_ang < 1e-8)	{
    AutoDerivative<double> sratio = toffset / tspace;
    boost::math::quaternion<AutoDerivative<double> >
      d_quat(1 - (sratio * sratio) / 2.0,
	     delta_quat.R_component_2() * sratio,
	     delta_quat.R_component_3() * sratio,
	     delta_quat.R_component_4() * sratio);
    return d_quat * Q1;
  }
  AutoDerivative<double> sratio = 
    std::sin(d_ang / 2.0) / std::sin(delta_ang / 2.0);
  boost::math::quaternion<AutoDerivative<double> >
    d_quat(std::cos(d_ang / 2.0),
	   delta_quat.R_component_2() * sratio,
	   delta_quat.R_component_3() * sratio,
	   delta_quat.R_component_4() * sratio);
  return d_quat * Q1;
}

//-----------------------------------------------------------------------
/// Normalize a quaternion
//-----------------------------------------------------------------------

inline void normalize(boost::math::quaternion<AutoDerivative<double> >& Q)
{
  Q /= abs(value(Q));
}

}
#endif
