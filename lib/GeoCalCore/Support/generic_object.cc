#include "generic_object.h"
#include "geocal_serialize_support.h"
#include "geocal_config.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GenericObject::serialize
(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
    // Nothing to do
}

GEOCAL_IMPLEMENT(GenericObject);
#endif

//-----------------------------------------------------------------------
/// Geocal library version number.
//-----------------------------------------------------------------------

std::string GeoCal::geocal_version()
{
  // Note this doesn't really have anything to do with GenericObject,
  // but we need to stick the version *somewhere* and this is as good
  // a place as any.
  return VERSION;
}
