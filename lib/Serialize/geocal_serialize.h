#ifndef GEOCAL_SERIALIZE_H
#define GEOCAL_SERIALIZE_H
// This is a top level file that can be included to serialize all 
// of the geocal classes that can be serialized. Before including this
// class, you should include all the archives that you want to be able
// to use (e.g., #include <boost/archive/xml_iarchive.hpp>, 
// #include <boost/archive/xml_oarchive.hpp>).
#include "geocal_serialize_common.h"
#include "geocal_serialize_camera.h"
#include "geocal_serialize_dem.h"
#include "geocal_serialize_raster_image.h"
#endif
