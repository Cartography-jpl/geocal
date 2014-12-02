#ifndef GEOCAL_SERIALIZE_COMMON_H
#define GEOCAL_SERIALIZE_COMMON_H

// Common classes used by other serialization header files. See
// geocal_serialize.h for details.
#ifndef USE_BOOST_SERIALIZATON
#define USE_BOOST_SERIALIZATON
#endif
#include "generic_object.h"
#include "image_coordinate.h"

BOOST_CLASS_EXPORT(GeoCal::GenericObject);
BOOST_CLASS_EXPORT(GeoCal::ImageCoordinate);

#endif
