#include "hdf_orbit.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class PositionType, class TimeCreatorType> template<class Archive>
void HdfOrbit<PositionType, TimeCreatorType>::save(Archive & ar, 
			   const unsigned int version) const
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP(bgroup);
}

template<class PositionType, class TimeCreatorType> template<class Archive>
void HdfOrbit<PositionType, TimeCreatorType>::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP(bgroup);
  init();
}

using boost::archive::polymorphic_iarchive;
using boost::archive::polymorphic_oarchive;

BOOST_CLASS_EXPORT_IMPLEMENT(HdfOrbit_EciTod_TimeAcs);

template void HdfOrbit<EciTod, TimeAcsCreator>::save
(polymorphic_oarchive& ar, const unsigned version) const;

template void HdfOrbit<EciTod, TimeAcsCreator>::load
(polymorphic_iarchive& ar, const unsigned version);
#endif
