#ifndef GEOCAL_SERIALIZE_SUPPORT_H
#define GEOCAL_SERIALIZE_SUPPORT_H
// This contains various support routines for *implementing* the boost
// serialization (as opposed to *using*  the serialization
#include "geocal_external_config.h"
#include "weak_ptr_serialize_support.h"
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include "serialize_shared_ptr.h"
#include <boost/serialization/optional.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
// At boost 1.64, the header file to include for boost::array changed
// names. Pretty annoying, but select the right file to include
#ifdef HAVE_BOOST_SERIALIZATION_BOOST_ARRAY_HPP
#include <boost/serialization/boost_array.hpp>
#else
#include <boost/serialization/array.hpp>
#endif
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
#define GEOCAL_SPLIT_IMPLEMENT(NAME) \
BOOST_CLASS_EXPORT_IMPLEMENT(GeoCal::NAME); \
template void NAME::save(boost::archive::polymorphic_oarchive& ar, \
				    const unsigned int version) const; \
template void NAME::load(boost::archive::polymorphic_iarchive& ar, \
				    const unsigned int version);

#define GEOCAL_BASE(NAME,BASE) boost::serialization::void_cast_register<GeoCal::NAME, GeoCal::BASE>();
#define GEOCAL_GENERIC_BASE(NAME) GEOCAL_BASE(NAME, GenericObject);

using boost::archive::polymorphic_iarchive;
using boost::archive::polymorphic_oarchive;

#endif

#include "blitz_array_serialize_support.h"
#endif
