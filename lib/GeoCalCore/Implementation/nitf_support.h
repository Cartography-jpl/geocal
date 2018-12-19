#ifndef NITF_SUPPORT_H
#define NITF_SUPPORT_H
#include "geocal_quaternion.h"

namespace GeoCal {
//-----------------------------------------------------------------------
/// Map 4 values read from a NITF file (e.g., SENSORB, CSATTB) to a
/// quaternion.
///
/// Note what I believe are the differences:
///
/// 1. There are a few different conventions about the ordering of the
///    quaternion coefficients. The boost library places the real part
///    at the front, so we have a + b i + c j + d k and the quaternion
///    is 4-tuple (a, b, c, d). The
///    convention used by NITF data is q1 i + q2 j + q3 k + q4 with
///    the 4-tuple is (q1, q2, q3, q4). That means when we bring this over
///    to the boost library, we need to reorder this to the 4-tuple (q4,
///    q1, q2, q3).
/// 2. We use active transformation, which rotate a look vector. NITF
///    uses a passive transformation, which describes the relationship
///    of the sensor coordinate system to the ECI/ECF coordinate
///    system. In practice, this means that to get our standard
///    quaternions like that used in OrbitData we need to take the
///    inverse/conjugate of the NITF quaternion.
///
/// Something we do *not* change but you should be aware of is that
/// the sensor coordinate system has the line/along track direction as
/// +y and the sample/cross track as +x. We have support for this in
/// e.g., QuaternionCamera, but a lot of our default code based on the
/// original EOS/MISR orbit has +x in the line direction and +y in the
/// sample direction.  
//-----------------------------------------------------------------------
  
template<class T> inline  boost::math::quaternion<T>
nitf_to_quaternion(const blitz::Array<T, 1>& Data)
{
  // Until we test this out, short circuit the transformation
  return array_to_quaternion(Data);
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 1>
quaternion_to_nitf(const boost::math::quaternion<T>& Q)
{
  // Until we test this out, short circuit the transformation
  return quaternion_to_array(Q);
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 1>
quaternion_to_nitf(const blitz::Array<T, 1>& Q)
{
  // Until we test this out, short circuit the transformation
  return Q;
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 2>
quaternion_to_nitf(const blitz::Array<T, 2>& Q)
{
  // Until we test this out, short circuit the transformation
  return Q;
}
  
}
#endif

