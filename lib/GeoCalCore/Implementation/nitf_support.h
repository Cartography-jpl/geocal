#ifndef NITF_SUPPORT_H
#define NITF_SUPPORT_H
#include "geocal_quaternion.h"

namespace GeoCal {
//-----------------------------------------------------------------------
/// Map 4 values read from a NITF file (e.g., SENSORB, CSATTB) to a
/// quaternion.
///
/// There are a few different conventions about the ordering of the
/// quaternion coefficients. The boost library places the real part
/// at the front, so we have a + b i + c j + d k and the quaternion
/// is 4-tuple (a, b, c, d). The
/// convention used by NITF data is q1 i + q2 j + q3 k + q4 with
/// the 4-tuple is (q1, q2, q3, q4). That means when we bring this over
/// to the boost library, we need to reorder this to the 4-tuple (q4,
///  q1, q2, q3).
///  
/// Something we do *not* change but you should be aware of is that
/// the sensor coordinate system has the line/along track direction as
/// +y and the sample/cross track as -x. We have support for this in
/// e.g., QuaternionCamera and SensrbCamera but a lot of our default
/// code based on the original EOS/MISR orbit has +x in the line
/// direction and +y in the sample direction.  Also, there are
/// "sensor" and "camera" coordinates which change by the quaternion
/// m_sen_to_cam = (0.5, 0.5, 0.5, -0.5), see SensrbCamera code for
/// samples of this. So you may need to multiply m_sen_to_cam or
/// conj(m_sen_to_cam) to get the quaternions you are expecting.
//-----------------------------------------------------------------------
  
template<class T> inline  boost::math::quaternion<T>
nitf_to_quaternion(const blitz::Array<T, 1>& Data)
{
  if(Data.rows() != 4)
    throw GeoCal::Exception("Array must be size 4");
  return boost::math::quaternion<T>(Data(3), -Data(0), -Data(1), -Data(2));
}

//-----------------------------------------------------------------------
/// Reverse nitf_to_quaternion. See nitf_to_quaternion for
/// documentation on what this does.
//-----------------------------------------------------------------------
  
template<class T> inline blitz::Array<T, 1>
quaternion_to_nitf(const boost::math::quaternion<T>& Q)
{
  blitz::Array<T, 1> res(4);
  res = -Q.R_component_2(), -Q.R_component_3(),
    -Q.R_component_4(), Q.R_component_1();
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
  res = -Q(1), -Q(2), -Q(3), Q(0);
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
  // Move real part
  res(ra,0) = -Q(1,ra);
  res(ra,1) = -Q(2,ra);
  res(ra,2) = -Q(3,ra);
  res(ra,3) = Q(0,ra);
  return res;
}
  
}
#endif

