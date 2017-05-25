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
    & GEOCAL_NVP(bgroup)
    & GEOCAL_NVP(eph_time)
    & GEOCAL_NVP(eph_pos)
    & GEOCAL_NVP(eph_vel)
    & GEOCAL_NVP(att_time)
    & GEOCAL_NVP(att_quat);
}

template<class PositionType, class TimeCreatorType> template<class Archive>
void HdfOrbit<PositionType, TimeCreatorType>::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(fname)
    & GEOCAL_NVP(bgroup);
  // Older version didn't have the field names saved, it used the default
  // values
  if(version == 0) {
    eph_time = "Ephemeris/Time";
    eph_pos = "Ephemeris/Position";
    eph_vel = "Ephemeris/Velocity";
    att_time = "Attitude/Time";
    att_quat = "Attitude/Quaternion";
  } else {
    ar & GEOCAL_NVP(eph_time)
      & GEOCAL_NVP(eph_pos)
      & GEOCAL_NVP(eph_vel)
      & GEOCAL_NVP(att_time)
      & GEOCAL_NVP(att_quat);
  }
  
  init();
}

using boost::archive::polymorphic_iarchive;
using boost::archive::polymorphic_oarchive;

BOOST_CLASS_EXPORT_IMPLEMENT(HdfOrbit_EciTod_TimeAcs);

template void HdfOrbit<EciTod, TimeAcsCreator>::save
(polymorphic_oarchive& ar, const unsigned version) const;

template void HdfOrbit<EciTod, TimeAcsCreator>::load
(polymorphic_iarchive& ar, const unsigned version);

BOOST_CLASS_EXPORT_IMPLEMENT(HdfOrbit_Eci_TimePgs);

template void HdfOrbit<Eci, TimePgsCreator>::save
(polymorphic_oarchive& ar, const unsigned version) const;

template void HdfOrbit<Eci, TimePgsCreator>::load
(polymorphic_iarchive& ar, const unsigned version);

BOOST_CLASS_EXPORT_IMPLEMENT(HdfOrbit_Eci_TimeJ2000);

template void HdfOrbit<Eci, TimeJ2000Creator>::save
(polymorphic_oarchive& ar, const unsigned version) const;

template void HdfOrbit<Eci, TimeJ2000Creator>::load
(polymorphic_iarchive& ar, const unsigned version);

BOOST_CLASS_EXPORT_IMPLEMENT(HdfOrbit_EciTod_TimeJ2000);

template void HdfOrbit<EciTod, TimeJ2000Creator>::save
(polymorphic_oarchive& ar, const unsigned version) const;

template void HdfOrbit<EciTod, TimeJ2000Creator>::load
(polymorphic_iarchive& ar, const unsigned version);
#endif
