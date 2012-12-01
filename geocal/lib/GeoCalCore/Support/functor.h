#ifndef FUNCTOR_H
#define FUNCTOR_H
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This is a general Functor (Function object). This maps a type T to 
  type S. We have typedefs for common mappings.
*******************************************************************/

template<class T, class S> class Functor {
public:
//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

  virtual ~Functor() {}

//-----------------------------------------------------------------------
/// Mapping operator. This takes a T to an S.
//-----------------------------------------------------------------------

  virtual S operator()(const T& X) const = 0;
};

/****************************************************************//**
  Mapping from a double to a double
*******************************************************************/
  typedef Functor<double, double> DFunctor;

/****************************************************************//**
  Mapping from a vector to a vector
*******************************************************************/
  typedef Functor<blitz::Array<double, 1>, blitz::Array<double, 1> > VFunctor;
}
#endif
