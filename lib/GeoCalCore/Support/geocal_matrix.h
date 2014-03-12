#ifndef GEOCAL_MATRIX_H
#define GEOCAL_MATRIX_H
#include "geocal_exception.h"
#include <boost/array.hpp>
#include <boost/math/quaternion.hpp>
#include <cmath>

namespace GeoCal {
/** \defgroup Matrix Minor support for matrices 
* This provides some minor support for matrices. This is not meant to
* be anything close to a full matrix library - we would get a
* third party library for that. This supplies some basic functionality
* when we don't want to worry about using and depending on a full
* library. 
*
* This is primarily intended for small fixed size matrices. For
* larger matrices the library blitz++ supplies a full library and very
* good performance. For small fixed size matrices, these functions
* will be a little faster than blitz++.
* @{
*/

//-----------------------------------------------------------------------
/// Quaternion that rotate about x
///
/// If you don't happen to remember off the top of your head, you
/// rotate an angle 'a' around an axis 'u' by the quaternion cos(a / 2) +
/// sin(a / 2) * u.
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
/// Do a rotation about the three axis given.
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
/// Convert a quaternion to a rotation matrix.
//-----------------------------------------------------------------------

inline void quaternion_to_matrix(const boost::math::quaternion<double>& q, 
 			         double m[3][3])
{
  double q0 = q.R_component_1();
  double q1 = q.R_component_2();
  double q2 = q.R_component_3();
  double q3 = q.R_component_4();
  m[0][0] = q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3;
  m[0][1] = 2 * (q1 * q2 - q0 * q3);
  m[0][2] = 2 * (q0 * q2 + q1 * q3);
  m[1][0] = 2 * (q1 * q2 + q0 * q3);
  m[1][1] = q0 * q0 - q1 * q1 + q2 * q2 - q3 * q3;
  m[1][2] = 2 * (q2 * q3 - q0 * q1);
  m[2][0] = 2 * (q1 * q3 - q0 * q2);
  m[2][1] = 2 * (q2 * q3 + q0 * q1);
  m[2][2] = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
}

//-----------------------------------------------------------------------
/// Convert a rotation matrix to a quaternion.
/// http://www.j3d.org/matrix_faq/matrfaq_latest.html for this algorithm. 
//-----------------------------------------------------------------------

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

//-----------------------------------------------------------------------
/// Multiply a fixed size matrix with a vector. On exit res = m * x
//-----------------------------------------------------------------------

template<std::size_t D> inline void mul(const double m[D][D], 
					const boost::array<double, D>& x,
					boost::array<double, D>& res)
{
  for(std::size_t i = 0; i < D; ++i) {
    res[i] = 0;
    for(std::size_t j = 0; j < D; ++j)
      res[i] += m[i][j] * x[j];
  }
}

//-----------------------------------------------------------------------
/// Cross product
//-----------------------------------------------------------------------

inline void cross(const boost::array<double, 3>& x,
		  const boost::array<double, 3>& y,
		  boost::array<double, 3>& res)
{
  res[0] = x[1] * y[2] - x[2] * y[1];
  res[1] = x[2] * y[0] - x[0] * y[2];
  res[2] = x[0] * y[1] - x[1] * y[0];
}


//-----------------------------------------------------------------------
/// Copy a matrix. On exit, mcopy = m
//-----------------------------------------------------------------------

template<std::size_t D> inline void mat_copy(const double m[D][D], 
					     double mcopy[D][D])
{
  for(std::size_t i = 0; i < D; ++i)
    for(std::size_t j = 0; j < D; ++j)
      mcopy[i][j] = m[i][j];
}

//-----------------------------------------------------------------------
/// Copy a matrix transpose. On exit, mcopy = m^t
//-----------------------------------------------------------------------

template<std::size_t D> inline void mat_t_copy(const double m[D][D], 
					       double mcopy[D][D])
{
  for(std::size_t i = 0; i < D; ++i)
    for(std::size_t j = 0; j < D; ++j)
      mcopy[i][j] = m[j][i];
}

//-----------------------------------------------------------------------
/// Multiply a transpose of a fixed size matrix with a vector. On exit
/// res = m^t * x 
//-----------------------------------------------------------------------

template<std::size_t D> inline void mul_t(const double m[D][D], 
					  const boost::array<double, D>& x,
					  boost::array<double, D>& res)
{
  for(std::size_t i = 0; i < D; ++i) {
    res[i] = 0;
    for(std::size_t j = 0; j < D; ++j)
      res[i] += m[j][i] * x[j];
  }
}

//-----------------------------------------------------------------------
/// Dot product of two arrays.
//-----------------------------------------------------------------------

template<std::size_t D> inline double dot(const boost::array<double, D>& A,
					  const boost::array<double, D>& B)
{
  double res = 0;
  for(std::size_t i = 0; i < D; ++i)
    res += A[i] * B[i];
  return res;
}

//-----------------------------------------------------------------------
/// Norm of a array.
//-----------------------------------------------------------------------

template<std::size_t D> inline double norm(const boost::array<double, D>& A)
{
  return sqrt(dot(A, A));
}

/** @} */
}
#endif
