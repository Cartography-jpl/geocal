#include "covariance.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive> 
void Covariance::serialize(Archive& ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(Covariance)
  ar & GEOCAL_NVP(sum1) & GEOCAL_NVP(sum2) & GEOCAL_NVP(sum_product)
    & GEOCAL_NVP_(count);
}

GEOCAL_IMPLEMENT(Covariance);
#endif

//-----------------------------------------------------------------------
/// Print to stream
//-----------------------------------------------------------------------

void Covariance::print(std::ostream& os) const
{
  os << "Covariance:\n"
     << "  Covariance: " << covariance() << "\n"
     << "  Count:      " << count() << "\n"
     << "  Mean 1:     "  << mean1() << "\n"
     << "  Mean 2:     "  << mean2() << "\n";
}
