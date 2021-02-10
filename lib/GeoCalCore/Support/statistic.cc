#include "statistic.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Statistic::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(Statistic);
  ar & GEOCAL_NVP_(sum)
    & GEOCAL_NVP_(sum_squares)
    & GEOCAL_NVP_(min)
    & GEOCAL_NVP_(max)
    & GEOCAL_NVP_(sum_abs)
    & GEOCAL_NVP_(count)
    & GEOCAL_NVP_(min_index)
    & GEOCAL_NVP_(max_index);
}

GEOCAL_IMPLEMENT(Statistic);
#endif

//-----------------------------------------------------------------------
/// Add a new data point.
//-----------------------------------------------------------------------

void Statistic::add(double x)
{
  if (count_ == 0) {
    min_ = x;
    max_ = x;
  } else {
    if (min_ > x) {
      min_ = x;
      min_index_ = count_;
    } 
    if (max_ < x) {
      max_ = x;
      max_index_ = count_;
    }
  }
  sum_ += x;
  sum_abs_ += fabs(x);
  sum_squares_ += (x * x);
  count_++;
}

void Statistic::print(std::ostream& Os) const
{
  Os << "Statistic:\n"
     << "  count:     " << count() << "\n"
     << "  mean:      " << mean() << "\n"
     << "  sigma:     " << sigma() << "\n"
     << "  min:       " << min() << "\n"
     << "  max:       " << max() << "\n"
     << "  min_index: " << min_index() << "\n"
     << "  max_index: " << max_index() << "\n"
     << "  mean_abs:  " << mean_abs() << "\n";
}
