#include "image_matcher.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ImageMatcher::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(ImageMatcher);
}


GEOCAL_IMPLEMENT(ImageMatcher);
#endif
