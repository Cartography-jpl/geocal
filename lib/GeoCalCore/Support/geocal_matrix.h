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
