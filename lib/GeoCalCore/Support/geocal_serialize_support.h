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
#define GEOCAL_NVP2(x, y) boost::serialization::make_nvp(BOOST_PP_STRINGIZE(x), y)
#define GEOCAL_IMPLEMENT(x) \
BOOST_CLASS_EXPORT_IMPLEMENT(GeoCal:: ## x); \
template void x::serialize(boost::archive::polymorphic_oarchive& ar, \
				    const unsigned int version); \
template void x::serialize(boost::archive::polymorphic_iarchive& ar, \
				    const unsigned int version);

#endif

#endif
