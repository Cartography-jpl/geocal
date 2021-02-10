#include "generic_object.h"
#include "geocal_serialize_support.h"

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
