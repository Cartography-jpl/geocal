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
  if(Data.rows() != 4)
    throw GeoCal::Exception("Array must be size 4");
  // Move real part, but until we test this don't change active/passive
  return boost::math::quaternion<T>(Data(3), Data(0), Data(1), Data(2));
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 1>
quaternion_to_nitf(const boost::math::quaternion<T>& Q)
{
  blitz::Array<T, 1> res(4);
  // Move real part, but until we test this don't change active/passive
  res = Q.R_component_2(), Q.R_component_3(),
    Q.R_component_4(), Q.R_component_1();
  return res;
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 1>
quaternion_to_nitf(const blitz::Array<T, 1>& Q)
{
  if(Q.rows() != 4)
    throw GeoCal::Exception("Array must be size 4");
  blitz::Array<T, 1> res(4);
  // Move real part, but until we test this don't change active/passive
  res = Q(1), Q(2), Q(3), Q(0);
  return res;
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 2>
quaternion_to_nitf(const blitz::Array<T, 2>& Q)
{
  blitz::Range ra = blitz::Range::all();
  if(Q.cols() != 4)
    throw GeoCal::Exception("Array must be size 4");
  blitz::Array<T, 2> res(Q.rows(), 4);
  // Move real part, but until we test this don't change
  // active/passive
  res(ra,0) = Q(1,ra);
  res(ra,1) = Q(2,ra);
  res(ra,2) = Q(3,ra);
  res(ra,3) = Q(0,ra);
  return res;
}
  
}
#endif

