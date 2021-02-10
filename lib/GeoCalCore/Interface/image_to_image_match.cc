#include "image_to_image_match.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ImageToImageMatch::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(ImageToImageMatch);
}

GEOCAL_IMPLEMENT(ImageToImageMatch);
#endif

