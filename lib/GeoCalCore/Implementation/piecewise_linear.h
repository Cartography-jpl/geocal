#ifndef PIECEWISE_LINEAR_H
#define PIECEWISE_LINEAR_H
#include "with_parameter.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This is a piecewise linear/constant function.
*******************************************************************/
class PiecewiseLinear : public WithParameter {
public:
  enum {LINEAR, CONSTANT, LINEAR_TO_ZERO} FunctionType;
  PiecewiseLinear(const blitz::Array<double, 1>& X, 
		  const ArrayAd<double, 1>& Y,
		  const blitz::Array<int, 1>& T);
  virtual ~PiecewiseLinear() {}
  double value(double x);
  AutoDerivative<double> 
  value_with_derivative(const AutoDerivative<double>& X);
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return y_; }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { if(Parm.rows() != y_.rows())
      throw Exception("Parm is not expected size");
    y_.reference(Parm.copy()); 
  }
  virtual void print(std::ostream& Os) const;
private:
  blitz::Array<double, 1> x_;
  ArrayAd<double, 1> y_;
  blitz::Array<int, 1> t_;
  blitz::Array<bool, 1> y_held_zero_;
  PiecewiseLinear() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PiecewiseLinear);
#endif

