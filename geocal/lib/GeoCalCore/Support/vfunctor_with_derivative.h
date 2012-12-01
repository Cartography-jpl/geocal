#ifndef VFUNCTOR_WITH_DERIVATIVE_H
#define VFUNCTOR_WITH_DERIVATIVE_H
#include "functor.h"

/****************************************************************//**
  This is a VFunctor that can also calculate the Jacobian.
*******************************************************************/
namespace GeoCal {
class VFunctorWithDerivative : public VFunctor {
public:
//-----------------------------------------------------------------------
/// Return jacobian. J_ij = Df_i / dx_j
//-----------------------------------------------------------------------
  virtual blitz::Array<double, 2> df(const blitz::Array<double, 1>& X) 
    const = 0;

//-----------------------------------------------------------------------
/// Return both the value of the function and the Jacobian. Often this
/// can be calculated more quickly. The default implementation here
/// just calls opererator() and df separately.
//-----------------------------------------------------------------------
  virtual void f_and_df(const blitz::Array<double, 1>& X, 
			blitz::Array<double, 1>& F,
			blitz::Array<double, 2>& Df) const
  {
    F.reference((*this)(X));
    Df.reference(df(X));
  }
};
}
#endif
