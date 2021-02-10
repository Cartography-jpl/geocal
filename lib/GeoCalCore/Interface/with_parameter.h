#ifndef WITH_PARAMETER_H
#define WITH_PARAMETER_H
#include "array_ad.h"
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

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

  virtual void parameter_with_derivative(const ArrayAd<double, 1>& UNUSED(Parm))
   {
     // Default is do nothing
   }

//-----------------------------------------------------------------------
/// Descriptive name of each parameter.
//-----------------------------------------------------------------------

  virtual std::vector<std::string> parameter_name() const
  {
    std::vector<std::string> res;
    for(int i = 0; i < parameter().rows(); ++i)
      res.push_back("Parameter " + boost::lexical_cast<std::string>(i));
    return res;
  }

//-----------------------------------------------------------------------
/// Return the parameter subset mask, where "true" means include the
/// parameter and "false" means don't.
//-----------------------------------------------------------------------

  virtual blitz::Array<bool, 1> parameter_mask() const 
  {
    // Default is to just return all Trues. Derived classes can
    // override this.
    blitz::Array<bool, 1> res(parameter().rows());
    res = true;
    return res;
  }
  virtual blitz::Array<double, 1> parameter_subset() const;
  virtual void parameter_subset(const blitz::Array<double, 1>& P);
  virtual ArrayAd<double, 1> parameter_with_derivative_subset() const;
  virtual void parameter_with_derivative_subset(const ArrayAd<double, 1>& P);
  virtual std::vector<std::string> parameter_name_subset() const;
  void add_identity_gradient();
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  It is common for a class WithParameter to be made up of subobjects
  an just string them together (e.g., a OrbitDataImageGroundConnection 
  where the orbit and camera each have parameters). This handles this 
  simple case.
*******************************************************************/

class WithParameterNested : public virtual WithParameter {
public:
  WithParameterNested() {}
  virtual ~WithParameterNested() {}
  void add_object(const boost::shared_ptr<WithParameter>& Obj)
  { obj_list.push_back(Obj); }
  void clear_object() { obj_list.resize(0); }
  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<bool, 1> parameter_mask() const;
private:
  std::vector<boost::shared_ptr<WithParameter> > obj_list;
  int total_size() const;
  int max_num_var() const;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}
GEOCAL_EXPORT_KEY(WithParameter);
GEOCAL_EXPORT_KEY(WithParameterNested);
#endif

