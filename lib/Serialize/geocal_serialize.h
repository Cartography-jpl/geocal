#ifndef GEOCAL_SERIALIZE_H
#define GEOCAL_SERIALIZE_H


#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

// This is a top level file that can be included to serialize all 
// of the geocal classes that can be serialized. Before including this
// class, you should include all the archives that you want to be able
// to use (e.g., #include <boost/archive/xml_iarchive.hpp>, 
// #include <boost/archive/xml_oarchive.hpp>).
//
// **Note** This should *only* be included in one file. If you include
// multiple copies than boost will create multiple copies of some
// symbols. Kind of a pain, but the boost serialization code has some
// serious template stuff happening, and this is just the way it
// works.
#include "geocal_serialize_common.h"
#include "geocal_serialize_camera.h"
#include "geocal_serialize_dem.h"
#include "geocal_serialize_raster_image.h"
#include "geocal_serialize_function.h"

#endif
