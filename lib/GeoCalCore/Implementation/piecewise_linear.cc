#include "piecewise_linear.h"
#include "geocal_serialize_support.h"
#include "blitz_array_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PiecewiseLinear::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(PiecewiseLinear);
  ar & GEOCAL_NVP_(x) & GEOCAL_NVP_(y) & GEOCAL_NVP_(t)
    & GEOCAL_NVP_(parameter_size);
}

GEOCAL_IMPLEMENT(PiecewiseLinear);
#endif

PiecewiseLinear::PiecewiseLinear
(const blitz::Array<Time, 1>& X, 
 const ArrayAd<double, 1>& Y,
 const blitz::Array<int, 1>& T)
  : x_(X.copy()), y_(X.rows(), Y.number_variable()), t_(T.copy())
{
  if(T.rows() != X.rows() - 1)
    throw Exception("T must have number rows = X rows - 1");
  parameter_size_ = std::count(T.begin(), T.end(), (int) LINEAR) + 1;
  if(Y.rows() != parameter_size_) 
    throw Exception("Y.rows must be count(T) == LINEAR + 1");
  parameter_with_derivative(Y);
  for(int i = 0; i < X.rows() - 1; ++i)
    if(X(i) >= X(i+1))
      throw Exception("X must be sorted.");
}

PiecewiseLinear::PiecewiseLinear
(const std::vector<Time>& X, 
 const blitz::Array<int, 1>& T)
  : x_((int) X.size()), y_((int) X.size(), 0), t_(T.copy())
{
  if(t_.rows() != x_.rows() - 1)
    throw Exception("T must have number rows = X rows - 1");
  for(int i = 0; i < x_.rows(); ++i)
    x_(i) = X[i];
  parameter_size_ = std::count(t_.begin(), t_.end(), (int) LINEAR) + 1;
  blitz::Array<double, 1> y(parameter_size_);
  y = 0;
  parameter(y);
  for(int i = 0; i < x_.rows() - 1; ++i)
    if(x_(i) >= x_(i+1))
      throw Exception("X must be sorted.");
}

void PiecewiseLinear::parameter_with_derivative(const ArrayAd<double, 1>& Parm)
{
  if(Parm.rows() != parameter_size_)
    throw Exception("Parm is not the expected size");
  y_.resize_number_variable(Parm.number_variable());
  y_(0) = Parm(0);
  int j = 1;
  for(int i = 0; i < t_.rows(); ++i)
    switch(t_(i)) {
    case LINEAR:
      y_(i + 1) = Parm(j);
      j++;
      break;
    case LINEAR_TO_ZERO:
      y_(i + 1) = 0.0;
      break;
    case CONSTANT:
      y_(i + 1) = y_(i);
      break;
    default:
      throw Exception("This should be impossible");
    }
}

ArrayAd<double, 1> PiecewiseLinear::parameter_with_derivative() const
{
  ArrayAd<double, 1> res(parameter_size_, y_.number_variable());
  res(0) = y_(0);
  int j = 1;
  for(int i = 0; i < t_.rows(); ++i)
    if(t_(i) == LINEAR) {
      res(j) = y_(i + 1);
      j++;
    }
  return res;
}

double PiecewiseLinear::value(const Time& x) const
{
  if(x <= x_(0))
    return y_.value()(0);
  if(x >= x_(x_.rows() - 1))
    return y_.value()(x_.rows() - 1);
  int i = std::lower_bound(x_.data(), x_.data() + x_.rows(), x) - x_.data();
  switch(t_(i-1)) {
  case CONSTANT:
    return y_.value()(i-1);
    break;
  case LINEAR:
  case LINEAR_TO_ZERO:
    return y_.value()(i-1) + (y_.value()(i) -y_.value()(i-1)) *
      (x - x_(i-1)) / (x_(i) - x_(i-1));
    break;
  default:
    throw Exception("This should be impossible");
  }
}

AutoDerivative<double> PiecewiseLinear::value_with_derivative
(const TimeWithDerivative& x) const
{
  if(x.value() <= x_(0))
    return y_.value()(0);
  if(x.value() >= x_(x_.rows() - 1))
    return y_.value()(x_.rows() - 1);
  int i = std::lower_bound(x_.data(), x_.data() + x_.rows(), x.value()) - 
    x_.data();
  switch(t_(i-1)) {
  case CONSTANT:
    return y_(i-1);
    break;
  case LINEAR:
  case LINEAR_TO_ZERO:
    return y_(i-1) + (y_(i) -y_(i-1)) *
      (x - x_(i-1)) / (x_(i) - x_(i-1));
    break;
  default:
    throw Exception("This should be impossible");
  }
}

void PiecewiseLinear::print(std::ostream& Os) const
{
  Os << "PiecewiseLinear:\n";
  Os << "            X                        Y     Function Type\n";
  Os << "       ---------                ---------  -------------\n";
  for(int i = 0; i < x_.rows(); ++i)
    Os << std::setw(10) << x_(i) << " " 
       << std::setw(10) << y_.value()(i) << " "
       << std::setw(10) << (i == t_.rows() ? "C" :
			    (t_(i) == LINEAR ? "L" :
			     (t_(i) == CONSTANT ? "C" :
			      (t_(i) == LINEAR_TO_ZERO ? "LZ" : "?")))) << "\n";
}
