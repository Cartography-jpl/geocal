#define HAVE_BOOST_SERIALIZATON2
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include "generic_object.h"
using namespace GeoCal;

BOOST_CLASS_EXPORT_IMPLEMENT(GeoCal::GenericObject);
