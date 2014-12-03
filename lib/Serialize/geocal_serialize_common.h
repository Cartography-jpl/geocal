#ifndef GEOCAL_SERIALIZE_COMMON_H
#define GEOCAL_SERIALIZE_COMMON_H

// Common classes used by other serialization header files. See
// geocal_serialize.h for details.
#ifndef USE_BOOST_SERIALIZATON
#define USE_BOOST_SERIALIZATON
#endif
#include <boost/serialization/vector.hpp>
#include "generic_object.h"
#include "image_coordinate.h"
#include "geocal_quaternion.h"
#include "geocal_autoderivative_quaternion.h"
#include "auto_derivative.h"
#include "frame_coordinate.h"
BOOST_CLASS_EXPORT(GeoCal::GenericObject);
BOOST_CLASS_EXPORT(GeoCal::ImageCoordinate);
BOOST_CLASS_EXPORT(GeoCal::VicarImageCoordinate);
BOOST_CLASS_EXPORT(GeoCal::ImageCoordinateWithDerivative);
BOOST_CLASS_EXPORT(GeoCal::FrameCoordinate);
BOOST_CLASS_EXPORT(GeoCal::FrameCoordinateWithDerivative);
BOOST_CLASS_EXPORT(std::vector<GeoCal::FrameCoordinateWithDerivative>);
BOOST_CLASS_EXPORT(boost::math::quaternion<double>);
BOOST_CLASS_EXPORT(boost::math::quaternion<GeoCal::AutoDerivative<double> >);
BOOST_CLASS_EXPORT(std::vector<double>);
BOOST_CLASS_EXPORT(blitz_double_array_1d);
BOOST_CLASS_EXPORT(blitz_double_array_2d);
BOOST_CLASS_EXPORT(blitz_bool_array_1d);
BOOST_CLASS_EXPORT(GeoCal::AutoDerivative<double>);

#endif
