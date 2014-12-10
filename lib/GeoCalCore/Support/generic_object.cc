#define HAVE_BOOST_SERIALIZATON2
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include "generic_object.h"
using namespace GeoCal;

template<class Archive>
void GenericObject::serialize(Archive & ar, const unsigned int version)
{
    // Nothing to do
}

BOOST_CLASS_EXPORT_IMPLEMENT(GeoCal::GenericObject);

template void GenericObject::serialize(boost::archive::polymorphic_oarchive& ar, 
				    const unsigned int version);
template void GenericObject::serialize(boost::archive::polymorphic_iarchive& ar, 
				    const unsigned int version);
