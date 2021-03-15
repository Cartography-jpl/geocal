#include "orbit_array.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class PositionType, class TimeCreatorType> template<class Archive>
void OrbitArray<PositionType, TimeCreatorType>::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(att_from_sc_to_ref_frame)
    & GEOCAL_NVP(serialize_data);
  if(serialize_data) {
    ar & GEOCAL_NVP(att_map) & GEOCAL_NVP(pos_map);
    boost::serialization::split_member(ar, *this, version);
  }
}

template<class PositionType, class TimeCreatorType> template<class Archive>
void OrbitArray<PositionType, TimeCreatorType>::save(Archive & UNUSED(ar), const unsigned int UNUSED(version)) const
{
  // Nothing more to do
}

template<class PositionType, class TimeCreatorType> template<class Archive>
void OrbitArray<PositionType, TimeCreatorType>::load(Archive & UNUSED(ar), const unsigned int UNUSED(version)) 
{
  for(auto i = att_map.begin(); i != att_map.end(); ++i)
    orbit_data_map[i->first] = nullptr;
}

GEOCAL_IMPLEMENT(OrbitArray_EciTod_TimeAcs);
GEOCAL_IMPLEMENT(OrbitArray_Eci_TimePgs);
GEOCAL_IMPLEMENT(OrbitArray_Eci_TimeJ2000);
GEOCAL_IMPLEMENT(OrbitArray_EciTod_TimeJ2000);
#endif
