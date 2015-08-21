#ifndef PIECEWISE_LINEAR_H
#define PIECEWISE_LINEAR_H
#include "with_parameter.h"
#include "geocal_time.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This is a piecewise linear/constant function.
*******************************************************************/
class PiecewiseLinear : public WithParameter, 
			public Printable<PiecewiseLinear> {
public:
  enum {LINEAR, CONSTANT, LINEAR_TO_ZERO} FunctionType;
  PiecewiseLinear(const blitz::Array<Time, 1>& X, 
		  const ArrayAd<double, 1>& Y,
		  const blitz::Array<int, 1>& T);
  PiecewiseLinear(const PiecewiseLinear& P)
    : x_(P.x_.copy()), y_(P.y_.copy()), t_(P.t_.copy()),
      parameter_size_(P.parameter_size_)
  { }
  PiecewiseLinear() {}
  virtual ~PiecewiseLinear() {}
  double value(const Time& x) const;
  AutoDerivative<double> 
  value_with_derivative(const TimeWithDerivative& X) const;
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual void print(std::ostream& Os) const;
private:
  blitz::Array<Time, 1> x_;
  ArrayAd<double, 1> y_;
  blitz::Array<int, 1> t_;
  int parameter_size_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PiecewiseLinear);
#endif

