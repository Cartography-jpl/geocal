#ifndef GEOCAL_QUATERNION_H
#define GEOCAL_QUATERNION_H
#include "geocal_exception.h"
#include "geocal_external_config.h"
#include "geocal_matrix.h"
#include <boost/math/quaternion.hpp>
#include <blitz/array.h>
#include <cmath>

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/split_free.hpp>
// Add serialization for boost::math::quaternion
namespace boost {
  namespace serialization {
    template<class Archive, class T>
    void save(Archive& ar, const boost::math::quaternion<T>& q, 
		     const unsigned version);
    template<typename Archive, class T>
    void load(Archive& ar, boost::math::quaternion<T>& q, 
	      const unsigned version);
  }
}
BOOST_SERIALIZATION_SPLIT_FREE(boost::math::quaternion<double>);
#endif

namespace GeoCal {
  // Various support routines for working with quaternions. 

//-----------------------------------------------------------------------
/// Quaternion that rotate about x
///
/// If you don't happen to remember off the top of your head, you
/// rotate an angle 'a' around an axis 'u' by the quaternion cos(a / 2) +
/// sin(a / 2) * u.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------
template<class T> inline boost::math::quaternion<T> quat_rot_x(const T& A)
{
  return boost::math::quaternion<T>(std::cos(A / 2),
				    std::sin(A / 2),
				    T(0),
				    T(0));
}

//-----------------------------------------------------------------------
/// Quaternion that rotate about y
///
/// If you don't happen to remember off the top of your head, you
/// rotate an angle 'a' around an axis 'u' by the quaternion cos(a / 2) +
/// sin(a / 2) * u.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------
template<class T> inline boost::math::quaternion<T> quat_rot_y(const T& A)
{
  return boost::math::quaternion<T>(std::cos(A / 2),
				    T(0),
				    std::sin(A / 2),
				    T(0));
}

//-----------------------------------------------------------------------
/// Quaternion that rotate about z
///
/// If you don't happen to remember off the top of your head, you
/// rotate an angle 'a' around an axis 'u' by the quaternion cos(a / 2) +
/// sin(a / 2) * u.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------
template<class T> inline boost::math::quaternion<T> quat_rot_z(const T& A)
{
  return boost::math::quaternion<T>(std::cos(A / 2),
				    T(0),
				    T(0),
				    std::sin(A / 2));
}

//-----------------------------------------------------------------------
/// Take a character to indicate which direction we rotate about.
//-----------------------------------------------------------------------

template<class T> inline boost::math::quaternion<T> quat_rot_i(const T& A, char C)
{
  switch(C) {
  case '1':
  case 'X':
  case 'x':
    return quat_rot_x(A);
  case '2':
  case 'Y':
  case 'y':
    return quat_rot_y(A);
  case '3':
  case 'Z':
  case 'z':
    return quat_rot_z(A);
  default:
    Exception e;
    e << "Unrecognized axis character '" << C << "'";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Do a rotation about the one axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline boost::math::quaternion<T> 
quat_rot(const std::string& Rot, 
	 const T& A1)
{
  if(Rot.size() != 1) 
    throw Exception("Rotation string must be 1 characters long");
  return quat_rot_i(A1, Rot[0]);
}

//-----------------------------------------------------------------------
/// Do a rotation about the two axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline 
boost::math::quaternion<T> quat_rot(const std::string& Rot, 
					 const T& A1, const T& A2)
{
  if(Rot.size() != 2) 
    throw Exception("Rotation string must be 2 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]);
}

//-----------------------------------------------------------------------
/// Do a rotation about the three axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline 
boost::math::quaternion<T> quat_rot(const std::string& Rot, 
					 const T& A1, const T& A2, const T& A3)
{
  if(Rot.size() != 3) 
    throw Exception("Rotation string must be 3 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]) * 
    quat_rot_i(A3, Rot[2]);
}

//-----------------------------------------------------------------------
/// Do a rotation about the four axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline 
boost::math::quaternion<T> quat_rot(const std::string& Rot, 
			const T& A1, const T& A2, const T& A3, const T& A4)
{
  if(Rot.size() != 4) 
    throw Exception("Rotation string must be 4 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]) * 
    quat_rot_i(A3, Rot[2]) * quat_rot_i(A4, Rot[3]);
}

//-----------------------------------------------------------------------
/// Do a rotation about the five axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline 
boost::math::quaternion<T> quat_rot(const std::string& Rot, 
		const T& A1, const T& A2, const T& A3, const T& A4, const T& A5)
{
  if(Rot.size() != 5) 
    throw Exception("Rotation string must be 5 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]) * 
    quat_rot_i(A3, Rot[2]) * quat_rot_i(A4, Rot[3]) * quat_rot_i(A5, Rot[4]);
}

//-----------------------------------------------------------------------
/// Do a rotation about the six axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline 
boost::math::quaternion<T> quat_rot(const std::string& Rot, 
	const T& A1, const T& A2, const T& A3, const T& A4, const T& A5, const T& A6)
{
  if(Rot.size() != 6) 
    throw Exception("Rotation string must be 6 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]) * 
    quat_rot_i(A3, Rot[2]) * quat_rot_i(A4, Rot[3]) * quat_rot_i(A5, Rot[4]) *
    quat_rot_i(A6, Rot[5]);
}

//-----------------------------------------------------------------------
/// Do a rotation about the seven axis given.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline 
boost::math::quaternion<T> quat_rot(const std::string& Rot, 
const T& A1, const T& A2, const T& A3, const T& A4, const T& A5, const T& A6, 
const T& A7)
{
  if(Rot.size() != 7) 
    throw Exception("Rotation string must be 7 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]) * 
    quat_rot_i(A3, Rot[2]) * quat_rot_i(A4, Rot[3]) * quat_rot_i(A5, Rot[4]) *
    quat_rot_i(A6, Rot[5]) * quat_rot_i(A7, Rot[6]);
}

//-----------------------------------------------------------------------
/// Return the Euler angles that make up the quaternion rotation (epsilon,
/// beta, delta so quat_rot("zyx", epsilon,beta, delta) = qin.
//-----------------------------------------------------------------------

template<class T> inline 
void quat_to_euler(const boost::math::quaternion<T>& qin,
		   T& epsilon, T& beta, T& delta)
{
  T q0 = qin.R_component_1();
  T q1 = qin.R_component_2();
  T q2 = qin.R_component_3();
  T q3 = qin.R_component_4();
  epsilon = std::atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));
  beta = std::asin(2 * (q0 * q2 - q3 * q1));
  delta = std::atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
}

//-----------------------------------------------------------------------
/// Return the Euler angles that make up the quaternion rotation (yaw,
/// pitch, roll, so quat_rot("xyz", pitch, roll, yaw) = qin.
//-----------------------------------------------------------------------

template<class T> inline void 
quat_to_ypr(const boost::math::quaternion<T>& qin,
	    T& yaw, T& pitch, T& roll)
{
  quat_to_euler(conj(qin), yaw, roll, pitch);
  pitch = -pitch;
  roll = -roll;
  yaw = -yaw;
}

//-----------------------------------------------------------------------
/// Convert a quaternion to a rotation matrix.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

template<class T> inline blitz::Array<T, 2> 
quaternion_to_matrix(const boost::math::quaternion<T>& q)
{
  T q0 = q.R_component_1();
  T q1 = q.R_component_2();
  T q2 = q.R_component_3();
  T q3 = q.R_component_4();
  blitz::Array<T, 2> m(3,3);
  m(0, 0) = q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3;
  m(0, 1) = 2 * (q1 * q2 - q0 * q3);
  m(0, 2) = 2 * (q0 * q2 + q1 * q3);
  m(1, 0) = 2 * (q1 * q2 + q0 * q3);
  m(1, 1) = q0 * q0 - q1 * q1 + q2 * q2 - q3 * q3;
  m(1, 2) = 2 * (q2 * q3 - q0 * q1);
  m(2, 0) = 2 * (q1 * q3 - q0 * q2);
  m(2, 1) = 2 * (q2 * q3 + q0 * q1);
  m(2, 2) = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
  return m;
}

//-----------------------------------------------------------------------
/// Convert a rotation matrix to a quaternion.
/// http://www.j3d.org/matrix_faq/matrfaq_latest.html for this algorithm. 
//-----------------------------------------------------------------------

template<class T> inline boost::math::quaternion<T> 
matrix_to_quaternion(const blitz::Array<T, 2>& m)
{
  if(m.rows() != 3 || m.cols() != 3)
    throw GeoCal::Exception("Matrix must be 3x3");
  T t = m(0, 0) + m(1, 1) + m(2, 2);
  if(t > 1e-8) {
    T s = std::sqrt(t + 1) * 2;
    return boost::math::quaternion<T>(-s / 4.0,
					   -(m(2, 1) - m(1, 2)) / s,
					   -(m(0, 2) - m(2, 0)) / s,
					   -(m(1, 0) - m(0, 1)) / s);
  }
  if(m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) {
    T s = std::sqrt(1.0 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
    return boost::math::quaternion<T>(-(m(2, 1) - m(1, 2)) / s,
					   -s / 4.0,
					   -(m(0, 1) + m(1, 0)) / s,
					   -(m(0, 2) + m(2, 0)) / s);
  } else if(m(1, 1) > m(2, 2)) {
    T s = std::sqrt(1.0 - m(0, 0) + m(1, 1) - m(2, 2)) * 2;
    return boost::math::quaternion<T>(-(m(0, 2) - m(2, 0)) / s,
					   -(m(0, 1) + m(1, 0)) / s,
					   -s / 4.0,
					   -(m(1, 2) + m(2, 1)) / s);
  } else {
    T s = std::sqrt(1.0 - m(0, 0) - m(1, 1) + m(2, 2)) * 2;
    return boost::math::quaternion<T>(-(m(1, 0) - m(0, 1)) / s,
					   -(m(2, 0) + m(0, 2)) / s,
					   -(m(1, 2) + m(2, 1)) / s,
					   -s / 4.0);
  }
}

template<class T> inline boost::math::quaternion<T> 
matrix_to_quaternion(const T m[3][3])
{
  T t = m[0][0] + m[1][1] + m[2][2];
  if(t > 1e-8) {
    T s = std::sqrt(t + 1) * 2;
    return boost::math::quaternion<T>(-s / 4.0,
					   -(m[2][1] - m[1][2]) / s,
					   -(m[0][2] - m[2][0]) / s,
					   -(m[1][0] - m[0][1]) / s);
  }
  if(m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
    T s = std::sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]) * 2;
    return boost::math::quaternion<T>(-(m[2][1] - m[1][2]) / s,
					   -s / 4.0,
					   -(m[0][1] + m[1][0]) / s,
					   -(m[0][2] + m[2][0]) / s);
  } else if(m[1][1] > m[2][2]) {
    T s = std::sqrt(1.0 - m[0][0] + m[1][1] - m[2][2]) * 2;
    return boost::math::quaternion<T>(-(m[0][2] - m[2][0]) / s,
					   -(m[0][1] + m[1][0]) / s,
					   -s / 4.0,
					   -(m[1][2] + m[2][1]) / s);
  } else {
    T s = std::sqrt(1.0 - m[0][0] - m[1][1] + m[2][2]) * 2;
    return boost::math::quaternion<T>(-(m[1][0] - m[0][1]) / s,
					   -(m[2][0] + m[0][2]) / s,
					   -(m[1][2] + m[2][1]) / s,
					   -s / 4.0);
  }
}

//-----------------------------------------------------------------------
/// Give the angle between two quaternions. This can be used for a variety
/// of purposes, such as determining an outlier quaternion.
//-----------------------------------------------------------------------

inline double quaternion_delta_angle(const boost::math::quaternion<double>& Q1, 
			     const boost::math::quaternion<double>& Q2)
{
  boost::math::quaternion<double> delta_quat = Q2 * conj(Q1);
  double t = delta_quat.R_component_1();
  t = (t > 1 ? 1 : (t < -1 ? -1 : t)); // Handle t being slightly
                                       // out of range due to round
                                       // off.
  if(t < 0)
    t = -t;
  double delta_ang = 2.0 * std::acos(t);
  return delta_ang;
}

//-----------------------------------------------------------------------
/// Interpolate between 2 quaternions rotations. This often goes by
/// the name "Slerp". Note that the
/// quaternion rotations are double values, a rotation with Q and -Q
/// gives the same rotation. However when we interpolate we want to 
/// rotate the smallest angle. So if Q2 and Q1 have a larger angle
/// than pi, we switch the sign of Q2. You can see a discussion of
/// this in wikipedia https://en.wikipedia.org/wiki/Slerp.
//-----------------------------------------------------------------------

inline boost::math::quaternion<double> interpolate_quaternion_rotation
(const boost::math::quaternion<double>& Q1, 
 const boost::math::quaternion<double>& Q2,
 const double& toffset, double tspace)
{
  boost::math::quaternion<double> delta_quat = Q2 * conj(Q1);
  double t = delta_quat.R_component_1();
  t = (t > 1 ? 1 : (t < -1 ? -1 : t)); // Handle t being slightly
                                       // out of range due to round
                                       // off.
  if(t < 0) {
    // Switch sign of Q2 to give same rotation, but easier to
    // interpolate. 
    delta_quat = -delta_quat;
    t = -t;
  }
  double delta_ang = 2.0 * std::acos(t);
  if(delta_ang < 1e-8)	// Handle degenerate case of Q1 and Q2
    // almost the same.
    return Q1;
  double d_ang = delta_ang * toffset / tspace;
  double sratio = std::sin(d_ang / 2.0) / std::sin(delta_ang / 2.0);
  boost::math::quaternion<double> 
    d_quat(std::cos(d_ang / 2.0),
	   delta_quat.R_component_2() * sratio,
	   delta_quat.R_component_3() * sratio,
	   delta_quat.R_component_4() * sratio);
  return d_quat * Q1;
}

//-----------------------------------------------------------------------
/// Normalize a quaternion
//-----------------------------------------------------------------------
template<class T> inline void normalize(boost::math::quaternion<T>& Q)
{
  Q /= abs(Q);
}

//-----------------------------------------------------------------------
/// Determine quaternion that will rotate a vector v1 to point at v2.
/// Note that this isn't actually unique, but this is the 'shortest
/// arc' solution.
//-----------------------------------------------------------------------
template<class T> inline boost::math::quaternion<T> 
determine_quat_rot(const boost::array<T, 3>& V1, const boost::array<T, 3>& V2)
{
  boost::array<T, 3> a;
  cross(V1, V2, a);
  T w = norm(V1) * norm(V2) + dot(V1, V2);
  boost::math::quaternion<T> res(w, a[0], a[1], a[2]);
  normalize(res);
  return res;
}

//-----------------------------------------------------------------------
/// Return a blitz array with the quaternion elements. This is mostly
/// useful for python.
//-----------------------------------------------------------------------

template<class T> inline blitz::Array<T, 1>
quaternion_to_array(const boost::math::quaternion<T>& qin)
{
  blitz::Array<T, 1> res(4);
  res = qin.R_component_1(), qin.R_component_2(), qin.R_component_3(),
    qin.R_component_4();
  return res;
}

template<class T> inline boost::math::quaternion<T> 
array_to_quaternion(const blitz::Array<T, 1>& m)
{
  if(m.rows() != 4)
    throw GeoCal::Exception("Array must be size 4");
  return boost::math::quaternion<T>(m(0), m(1), m(2), m(3));
}

}
#endif
