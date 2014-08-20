#ifndef GEOCAL_QUATERNION_H
#define GEOCAL_QUATERNION_H
#include "geocal_exception.h"
#include <boost/math/quaternion.hpp>
#include <blitz/array.h>
#include <cmath>

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
inline boost::math::quaternion<double> quat_rot_x(double A)
{
  return boost::math::quaternion<double>(cos(A / 2),
					 sin(A / 2),
					 0,
					 0);
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
inline boost::math::quaternion<double> quat_rot_y(double A)
{
  return boost::math::quaternion<double>(cos(A / 2),
					 0,
					 sin(A / 2),
					 0);
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
inline boost::math::quaternion<double> quat_rot_z(double A)
{
  return boost::math::quaternion<double>(cos(A / 2),
					 0,
					 0,
					 sin(A / 2));
}

//-----------------------------------------------------------------------
/// Take a character to indicate which direction we rotate about.
//-----------------------------------------------------------------------

inline boost::math::quaternion<double> quat_rot_i(double A, char C)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
						double A1)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
						double A1, double A2)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
						double A1, double A2, double A3)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
			double A1, double A2, double A3, double A4)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
		double A1, double A2, double A3, double A4, double A5)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
	double A1, double A2, double A3, double A4, double A5, double A6)
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

inline boost::math::quaternion<double> quat_rot(const std::string& Rot, 
double A1, double A2, double A3, double A4, double A5, double A6, double A7)
{
  if(Rot.size() != 7) 
    throw Exception("Rotation string must be 7 characters long");
  return quat_rot_i(A1, Rot[0]) * quat_rot_i(A2, Rot[1]) * 
    quat_rot_i(A3, Rot[2]) * quat_rot_i(A4, Rot[3]) * quat_rot_i(A5, Rot[4]) *
    quat_rot_i(A6, Rot[5]) * quat_rot_i(A7, Rot[6]);
}

//-----------------------------------------------------------------------
/// Return the Euler angles that make up the quaternion rotation (yaw,
/// pitch, roll, so quat_rot("xyz", pitch, roll, yaw) = qin.
//-----------------------------------------------------------------------

inline void quat_to_ypr(const boost::math::quaternion<double>& qin,
			double& yaw, double& pitch, double& roll)
{
  // We got these equations from wikipedia, which uses a different
  // order for the rotation (321). So we just take the conjugate here
  // and reverse the angles
  double q0 = qin.R_component_1();
  double q1 = -qin.R_component_2();
  double q2 = -qin.R_component_3();
  double q3 = -qin.R_component_4();
  yaw = -atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));
  pitch = -atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
  roll = -asin(2 * (q0 * q2 - q3 * q1));
}

//-----------------------------------------------------------------------
/// Convert a quaternion to a rotation matrix.
///
/// This is an "active" transformation. For "passive", just reverse
/// the sign of A
//-----------------------------------------------------------------------

inline blitz::Array<double, 2> 
quaternion_to_matrix(const boost::math::quaternion<double>& q)
{
  double q0 = q.R_component_1();
  double q1 = q.R_component_2();
  double q2 = q.R_component_3();
  double q3 = q.R_component_4();
  blitz::Array<double, 2> m(3,3);
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

inline boost::math::quaternion<double> 
matrix_to_quaternion(const blitz::Array<double, 2>& m)
{
  if(m.rows() != 3 || m.cols() != 3)
    throw GeoCal::Exception("Matrix must be 3x3");
  double t = m(0, 0) + m(1, 1) + m(2, 2);
  if(t > 1e-8) {
    double s = sqrt(t + 1) * 2;
    return boost::math::quaternion<double>(-s / 4.0,
					   -(m(2, 1) - m(1, 2)) / s,
					   -(m(0, 2) - m(2, 0)) / s,
					   -(m(1, 0) - m(0, 1)) / s);
  }
  if(m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) {
    double s = sqrt(1.0 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
    return boost::math::quaternion<double>(-(m(2, 1) - m(1, 2)) / s,
					   -s / 4.0,
					   -(m(0, 1) + m(1, 0)) / s,
					   -(m(0, 2) + m(2, 0)) / s);
  } else if(m(1, 1) > m(2, 2)) {
    double s = sqrt(1.0 - m(0, 0) + m(1, 1) - m(2, 2)) * 2;
    return boost::math::quaternion<double>(-(m(0, 2) - m(2, 0)) / s,
					   -(m(0, 1) + m(1, 0)) / s,
					   -s / 4.0,
					   -(m(1, 2) + m(2, 1)) / s);
  } else {
    double s = sqrt(1.0 - m(0, 0) - m(1, 1) + m(2, 2)) * 2;
    return boost::math::quaternion<double>(-(m(1, 0) - m(0, 1)) / s,
					   -(m(2, 0) + m(0, 2)) / s,
					   -(m(1, 2) + m(2, 1)) / s,
					   -s / 4.0);
  }
}

inline boost::math::quaternion<double> 
matrix_to_quaternion(const double m[3][3])
{
  double t = m[0][0] + m[1][1] + m[2][2];
  if(t > 1e-8) {
    double s = sqrt(t + 1) * 2;
    return boost::math::quaternion<double>(-s / 4.0,
					   -(m[2][1] - m[1][2]) / s,
					   -(m[0][2] - m[2][0]) / s,
					   -(m[1][0] - m[0][1]) / s);
  }
  if(m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
    double s = sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]) * 2;
    return boost::math::quaternion<double>(-(m[2][1] - m[1][2]) / s,
					   -s / 4.0,
					   -(m[0][1] + m[1][0]) / s,
					   -(m[0][2] + m[2][0]) / s);
  } else if(m[1][1] > m[2][2]) {
    double s = sqrt(1.0 - m[0][0] + m[1][1] - m[2][2]) * 2;
    return boost::math::quaternion<double>(-(m[0][2] - m[2][0]) / s,
					   -(m[0][1] + m[1][0]) / s,
					   -s / 4.0,
					   -(m[1][2] + m[2][1]) / s);
  } else {
    double s = sqrt(1.0 - m[0][0] - m[1][1] + m[2][2]) * 2;
    return boost::math::quaternion<double>(-(m[1][0] - m[0][1]) / s,
					   -(m[2][0] + m[0][2]) / s,
					   -(m[1][2] + m[2][1]) / s,
					   -s / 4.0);
  }
}


}
#endif
