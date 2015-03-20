#include "geocal_quaternion.h"
#include "geocal_autoderivative_quaternion.h"
#include "geocal_serialize_support.h"

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive, class T>
void boost::serialization::save
(Archive& ar, const boost::math::quaternion<T>& q, const unsigned version) 
{
  // Note a subtle issue here. Since we are copying the data, the
  // object tracking boost does will be these *local* values, not the
  // values actually in the quaternion. To keep this from causing
  // problems, T *must* be a type that does not have object tracking
  // turned on (either a primative type like double, or something
  // marked using GEOCAL_DONT_TRACK like AutoDerivative<double>. 
  T q1 = q.R_component_1();
  T q2 = q.R_component_2();
  T q3 = q.R_component_3();
  T q4 = q.R_component_4();
  ar & GEOCAL_NVP(q1) & GEOCAL_NVP(q2)
    & GEOCAL_NVP(q3) & GEOCAL_NVP(q4);
}

template<typename Archive, class T>
void boost::serialization::load
(Archive& ar, boost::math::quaternion<T>& q, 
 const unsigned version) 
{
  // Note a subtle issue here. Since we are copying the data, the
  // object tracking boost does will be these *local* values, not the
  // values actually in the quaternion. To keep this from causing
  // problems, T *must* be a type that does not have object tracking
  // turned on (either a primative type like double, or something
  // marked using GEOCAL_DONT_TRACK like AutoDerivative<double>. 
  T q1, q2, q3, q4;
  ar & GEOCAL_NVP(q1) & GEOCAL_NVP(q2)
    & GEOCAL_NVP(q3) & GEOCAL_NVP(q4);
  q = boost::math::quaternion<T>(q1, q2, q3, q4);
}

using boost::archive::polymorphic_iarchive;
using boost::archive::polymorphic_oarchive;
template void boost::serialization::load
(polymorphic_iarchive& ar, boost::math::quaternion<double>& q, 
 const unsigned version);

template void boost::serialization::save
(polymorphic_oarchive& ar, const boost::math::quaternion<double>& q, 
 const unsigned version);

template void boost::serialization::load
(polymorphic_iarchive& ar, boost::math::quaternion<GeoCal::AutoDerivative<double> >& q, 
 const unsigned version);

template void boost::serialization::save
(polymorphic_oarchive& ar, const boost::math::quaternion<GeoCal::AutoDerivative<double> >& q, 
 const unsigned version);

#endif

