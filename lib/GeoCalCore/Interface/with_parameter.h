#ifndef WITH_PARAMETER_H
#define WITH_PARAMETER_H
#include "array_ad.h"
#include <vector>

namespace GeoCal {
/****************************************************************//**
  It is common for a class to have some set of parameters that 
  control behavior, which we may then fit with something like the
  SBA. It can also be useful for only a subset of those parameter to
  be active. This mixin class supplies this functionality.
*******************************************************************/
class WithParameter: public virtual GenericObject {
public:
  virtual ~WithParameter() {}

//-----------------------------------------------------------------------
/// Return the parameters. 
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 1> parameter() const
  { // Default is to return parameter_with_derivative and strip off
    // the jacobian.
    return parameter_with_derivative().value(); 
  }

//-----------------------------------------------------------------------
/// Set the value of the parameters.
//-----------------------------------------------------------------------

  virtual void parameter(const blitz::Array<double, 1>& Parm)
  {
    // Default is to call parameter_with_derivative and set an
    // empty Jacobian.
    parameter_with_derivative(Parm);
  }

//-----------------------------------------------------------------------
/// Return parameters, including derivatives. The derivatives can be
/// with respect to whatever variables you like, this class just
/// handles propagating the derivatives.
//-----------------------------------------------------------------------

  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { // Default is no parameters.
    return blitz::Array<double, 1>(0); 
  }

//-----------------------------------------------------------------------
/// Set the value of the parameters, including derivatives of the
/// parameter. Useful for doing Jacobian calculations.
//-----------------------------------------------------------------------

  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
   {
     // Default is do nothing
   }

//-----------------------------------------------------------------------
/// Descriptive name of each parameter.
//-----------------------------------------------------------------------

  virtual std::vector<std::string> parameter_name() const
  {
    std::vector<std::string> res;
    return res;
  }

//-----------------------------------------------------------------------
/// Return the parameter subset mask, where "true" means include the
/// parameter and "false" means don't.
//-----------------------------------------------------------------------

  virtual const blitz::Array<bool, 1>& parameter_mask() const 
  {
    return parameter_mask_;
  }

  virtual blitz::Array<bool, 1>& parameter_mask()
  {
    return parameter_mask_;
  }

//-----------------------------------------------------------------------
/// Set the parameter mask.
//-----------------------------------------------------------------------

  virtual void parameter_mask(const blitz::Array<bool, 1>& M) 
  {
    if(M.rows() != parameter().rows())
      throw Exception("Mask does match the size of the full parameter.");
    parameter_mask_.reference(M.copy());
  }
  virtual blitz::Array<double, 1> parameter_subset() const;
  virtual void parameter_subset(const blitz::Array<double, 1>& P);
  virtual ArrayAd<double, 1> parameter_with_derivative_subset() const;
  virtual void parameter_with_derivative_subset(const ArrayAd<double, 1>& P);
  virtual std::vector<std::string> parameter_name_subset() const;
protected:
  blitz::Array<bool, 1> parameter_mask_;
};
}
#endif

