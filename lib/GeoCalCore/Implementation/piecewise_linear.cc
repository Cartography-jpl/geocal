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
    & GEOCAL_NVP_(y_held_zero);
}

GEOCAL_IMPLEMENT(PiecewiseLinear);
#endif

PiecewiseLinear::PiecewiseLinear
(const blitz::Array<double, 1>& X, 
 const ArrayAd<double, 1>& Y,
 const blitz::Array<int, 1>& T)
  : x_(X.copy()), y_(Y.copy()), t_(T.copy()), y_held_zero_(Y.rows())
{
}

void PiecewiseLinear::print(std::ostream& Os) const
{
  Os << "PiecewiseLinear:\n";
  Os << "        X         Y     Function Type\n";
  Os << "   --------- ---------  -------------\n";
  for(int i = 0; i < x_.rows(); ++i)
    Os << std::setw(10) << x_(i) << " " 
       << std::setw(10) << y_.value()(i) << " "
       << std::setw(10) << (t_(i) == LINEAR ? "L" :
			    (t_(i) == CONSTANT ? "C" :
			     (t_(i) == LINEAR_TO_ZERO ? "LZ" : "?"))) << "\n";
}
