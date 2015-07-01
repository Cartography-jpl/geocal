#ifndef DFUNCTOR_WITH_DERIVATIVE_H
#define DFUNCTOR_WITH_DERIVATIVE_H
#include "functor.h"
#include "auto_derivative.h"

/****************************************************************//**
  This is a DFunctor that can also propagate derivatives. There are
  2 derivatives involved, the derivative of f with respect to x (so
  df/dx), and the derivative of f with respect to any parameters it
  might depend on (so d f / d p_i)
*******************************************************************/
namespace GeoCal {
class DFunctorWithDerivative : public DFunctor {
public:
//-----------------------------------------------------------------------
/// Return df / dx
//-----------------------------------------------------------------------

  virtual double df(double X) const = 0;

//-----------------------------------------------------------------------
/// Return df / dp_i as a AutoDerivative.
//-----------------------------------------------------------------------

  virtual AutoDerivative<double> f_with_derivative(double X) const = 0;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(DFunctorWithDerivative);
#endif
