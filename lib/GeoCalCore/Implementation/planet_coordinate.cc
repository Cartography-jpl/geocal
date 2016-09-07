#include "planet_coordinate.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<> template<class Archive>
void MarsFixed::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianFixed);
}

template<> template<class Archive>
void MarsInertial::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianInertial);
}

template<> template<class Archive>
void MarsPlanetocentric::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(MarsPlanetocentric, GroundCoordinate);
  ar & GEOCAL_NVP2("latitude", lat_)
    & GEOCAL_NVP2("longitude", lon_)
    & GEOCAL_NVP_(height_ellipsoid);
}

template<> template<class Archive>
void MarsPlanetocentricConverter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(MarsPlanetocentricConverter, CoordinateConverter);
}

template<> template<class Archive>
void EuropaFixed::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianFixed);
}

template<> template<class Archive>
void EuropaInertial::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianInertial);
}

template<> template<class Archive>
void EuropaPlanetocentric::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GroundCoordinate);
  GEOCAL_BASE(EuropaPlanetocentric, GroundCoordinate);
  ar & GEOCAL_NVP2("latitude", lat_)
    & GEOCAL_NVP2("longitude", lon_)
    & GEOCAL_NVP_(height_ellipsoid);
}

template<> template<class Archive>
void EuropaPlanetocentricConverter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(EuropaPlanetocentricConverter, CoordinateConverter);
}

GEOCAL_IMPLEMENT(MarsFixed);
GEOCAL_IMPLEMENT(MarsInertial);
GEOCAL_IMPLEMENT(MarsPlanetocentric);
GEOCAL_IMPLEMENT(MarsPlanetocentricConverter);
GEOCAL_IMPLEMENT(EuropaFixed);
GEOCAL_IMPLEMENT(EuropaInertial);
GEOCAL_IMPLEMENT(EuropaPlanetocentric);
GEOCAL_IMPLEMENT(EuropaPlanetocentricConverter);
#endif

// Constants for Mars
template<> SpicePlanetConstant MarsConstant::h(MarsConstant::NAIF_CODE);
template<> const char* MarsConstant::name_ = "Mars";

// Constants for Europa
template<> SpicePlanetConstant EuropaConstant::h(EuropaConstant::NAIF_CODE);
template<> const char* EuropaConstant::name_ = "Europa";


