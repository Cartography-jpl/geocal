#include "tie_point.h"
#include "geocal_quaternion.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void TiePoint::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(TiePoint);
  ar & GEOCAL_NVP_(id) & GEOCAL_NVP_(is_gcp) & GEOCAL_NVP_(ground_location)
    & GEOCAL_NVP_(ic) & GEOCAL_NVP_(line_sigma) & GEOCAL_NVP_(sample_sigma);
}

GEOCAL_IMPLEMENT(TiePoint);
#endif

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void TiePoint::print(std::ostream& Os) const
{
  Os << "TiePoint\n"
     << "  Id:              " << id() << "\n"
     << "  Is GCP:          " << (is_gcp() ? "True" : "False") << "\n"
     << "  Ground location: ";
  if(ground_location())
    Os << *ground_location() << "\n";
  else
    Os << "None\n";
  Os  << "  Image coordinate:\n";
  for(int i = 0; i < number_image(); ++i) {
    Os << "     ";
    if(image_coordinate(i))
      Os << *image_coordinate(i) << ", " << line_sigma(i) << ", "
	 << sample_sigma(i) << "\n";
    else
      Os << "None\n";
  }
}
