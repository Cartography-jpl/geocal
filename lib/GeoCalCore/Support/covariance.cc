#include "covariance.h"

using namespace GeoCal;

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
