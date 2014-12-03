#ifndef GEOCAL_SERIALIZE_CAMERA_H
#define GEOCAL_SERIALIZE_CAMERA_H
// Serialization of Camera and derived objects.
#include "geocal_serialize_common.h"
#include "camera.h"
#include "spot_camera.h"
#include "quaternion_camera.h"
BOOST_CLASS_EXPORT(GeoCal::Camera);
BOOST_CLASS_EXPORT(GeoCal::SimpleCamera);
BOOST_CLASS_EXPORT(GeoCal::SpotCamera);
BOOST_CLASS_EXPORT(GeoCal::QuaternionCamera);
#endif
