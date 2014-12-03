#ifndef GEOCAL_SERIALIZE_CAMERA_H
#define GEOCAL_SERIALIZE_CAMERA_H
// Serialization of Camera and derived objects.
#include "geocal_serialize_common.h"
#include "camera.h"
#include "spot_camera.h"
#include "quaternion_camera.h"
#include "argus_camera.h"
#include "galileo_camera.h"
#include "quickbird_camera.h"
#include "mspi_camera.h"

BOOST_CLASS_EXPORT(GeoCal::Camera);
BOOST_CLASS_EXPORT(GeoCal::SimpleCamera);
BOOST_CLASS_EXPORT(GeoCal::SpotCamera);
BOOST_CLASS_EXPORT(GeoCal::QuaternionCamera);
BOOST_CLASS_EXPORT(GeoCal::ArgusCamera);
BOOST_CLASS_EXPORT(GeoCal::GalileoCamera);
BOOST_CLASS_EXPORT(GeoCal::QuickBirdCamera);
BOOST_CLASS_EXPORT(GeoCal::MspiParaxialTransform);
#endif
