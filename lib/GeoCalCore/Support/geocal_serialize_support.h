#ifndef GEOCAL_SERIALIZE_SUPPORT_H
#define GEOCAL_SERIALIZE_SUPPORT_H
// This contains various support routines for *implementing* the boost
// serialization (as opposed to *using*  the serialization
#include "geocal_config.h"
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

// Do this a lot, so give a shortcut for this
#define GEOCAL_NVP(x) BOOST_SERIALIZATION_NVP(x)
#define GEOCAL_NVP_(x) boost::serialization::make_nvp(BOOST_PP_STRINGIZE(x), x ## _)
#define GEOCAL_NVP2(x, y) boost::serialization::make_nvp(x, y)
#define GEOCAL_IMPLEMENT(NAME) \
BOOST_CLASS_EXPORT_IMPLEMENT(GeoCal::NAME); \
template void NAME::serialize(boost::archive::polymorphic_oarchive& ar, \
				    const unsigned int version); \
template void NAME::serialize(boost::archive::polymorphic_iarchive& ar, \
				    const unsigned int version);

#define GEOCAL_BASE(NAME,BASE) boost::serialization::void_cast_register<GeoCal::NAME, GeoCal::BASE>();
#define GEOCAL_GENERIC_BASE(NAME) GEOCAL_BASE(NAME, GenericObject);

#endif

#endif
