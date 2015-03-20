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

template<class T, std::size_t D> inline void mul(const T m[D][D], 
					const boost::array<T, D>& x,
					boost::array<T, D>& res)
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

template<class T> inline void cross(const boost::array<T, 3>& x,
		  const boost::array<T, 3>& y,
		  boost::array<T, 3>& res)
{
  res[0] = x[1] * y[2] - x[2] * y[1];
  res[1] = x[2] * y[0] - x[0] * y[2];
  res[2] = x[0] * y[1] - x[1] * y[0];
}


//-----------------------------------------------------------------------
/// Copy a matrix. On exit, mcopy = m
//-----------------------------------------------------------------------

template<class T, std::size_t D> inline void mat_copy(const T m[D][D], 
					     T mcopy[D][D])
{
  for(std::size_t i = 0; i < D; ++i)
    for(std::size_t j = 0; j < D; ++j)
      mcopy[i][j] = m[i][j];
}

//-----------------------------------------------------------------------
/// Copy a matrix transpose. On exit, mcopy = m^t
//-----------------------------------------------------------------------

template<class T, std::size_t D> inline void mat_t_copy(const T m[D][D], 
					       T mcopy[D][D])
{
  for(std::size_t i = 0; i < D; ++i)
    for(std::size_t j = 0; j < D; ++j)
      mcopy[i][j] = m[j][i];
}

//-----------------------------------------------------------------------
/// Multiply a transpose of a fixed size matrix with a vector. On exit
/// res = m^t * x 
//-----------------------------------------------------------------------

template<class T, std::size_t D> inline void mul_t(const T m[D][D], 
					  const boost::array<T, D>& x,
					  boost::array<T, D>& res)
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

template<class T, std::size_t D> inline 
T dot(const boost::array<T, D>& A,
	   const boost::array<T, D>& B)
{
  T res = 0;
  for(std::size_t i = 0; i < D; ++i)
    res += A[i] * B[i];
  return res;
}

//-----------------------------------------------------------------------
/// Norm of a array.
//-----------------------------------------------------------------------

template<class T, std::size_t D> 
inline T norm(const boost::array<T, D>& A)
{
  return sqrt(dot(A, A));
}

//-----------------------------------------------------------------------
/// Normalize a vector so it has length of 1.
//-----------------------------------------------------------------------

template<class T, std::size_t D> 
inline void normalize(boost::array<T, D>& A)
{
  T u = norm(A);
  for(std::size_t i = 0; i < D; ++i)
    A[i] /= u;
}

/** @} */
}
#endif
