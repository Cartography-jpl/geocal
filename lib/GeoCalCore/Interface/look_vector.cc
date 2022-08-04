#include "look_vector.h"
#include "geocal_exception.h"
#include "ground_coordinate.h"
#include "geocal_serialize_support.h"
#include "ecr.h"
#include "constant.h"
#include <cmath>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ScLookVector::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(ScLookVector);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void ScLookVectorWithDerivative::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(ScLookVectorWithDerivative);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void CartesianInertialLookVector::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CartesianInertialLookVector);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void CartesianInertialLookVectorWithDerivative::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CartesianInertialLookVectorWithDerivative);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void CartesianFixedLookVector::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CartesianFixedLookVector);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void CartesianFixedLookVectorWithDerivative::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CartesianFixedLookVectorWithDerivative);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void LnLookVector::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(LnLookVector);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void LnLookVectorWithDerivative::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(LnLookVectorWithDerivative);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void DcsLookVector::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(DcsLookVector);
  ar & GEOCAL_NVP(look_vector);
}

template<class Archive>
void DcsLookVectorWithDerivative::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(DcsLookVectorWithDerivative);
  ar & GEOCAL_NVP(look_vector);
}

GEOCAL_IMPLEMENT(ScLookVector);
GEOCAL_IMPLEMENT(ScLookVectorWithDerivative);
GEOCAL_IMPLEMENT(CartesianInertialLookVector);
GEOCAL_IMPLEMENT(CartesianInertialLookVectorWithDerivative);
GEOCAL_IMPLEMENT(CartesianFixedLookVector);
GEOCAL_IMPLEMENT(CartesianFixedLookVectorWithDerivative);
GEOCAL_IMPLEMENT(LnLookVector);
GEOCAL_IMPLEMENT(LnLookVectorWithDerivative);
GEOCAL_IMPLEMENT(DcsLookVector);
GEOCAL_IMPLEMENT(DcsLookVectorWithDerivative);
#endif

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ScLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in spacecraft coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ScLookVectorWithDerivative::print(std::ostream& Os) const
{
  boost::array<AutoDerivative<double>, 3> d = direction();
  Os << "Look vector in spacecraft coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CartesianInertialLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in Cartesian inertial coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CartesianInertialLookVectorWithDerivative::print(std::ostream& Os) const
{
  boost::array<AutoDerivative<double>, 3> d = direction();
  Os << "Look vector in Cartesian inertial coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CartesianFixedLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in Cartesian fixed coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CartesianFixedLookVectorWithDerivative::print(std::ostream& Os) const
{
  boost::array<AutoDerivative<double>, 3> d = direction();
  Os << "Look vector in Cartesian fixed coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void LnLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in local north coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void LnLookVectorWithDerivative::print(std::ostream& Os) const
{
  boost::array<AutoDerivative<double>, 3> d = direction();
  Os << "Look vector in local north coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Constructor going from a GroundCoordinate to a GroundCoordinate.
//-----------------------------------------------------------------------

CartesianFixedLookVector::CartesianFixedLookVector
(const GroundCoordinate& From,
 const GroundCoordinate& To)
{
  boost::shared_ptr<CartesianFixed> from_cf = From.convert_to_cf();
  boost::shared_ptr<CartesianFixed> to_cf = To.convert_to_cf();
  for(int i = 0; i < 3; ++i)
    look_vector[i] = to_cf->position[i] - from_cf->position[i];
}

//-----------------------------------------------------------------------
/// Return quaternion to go from ENU coordinates to CartesianFixed for
/// the given location.
//-----------------------------------------------------------------------

boost::math::quaternion<double> 
LnLookVector::enu_to_cf(const GroundCoordinate& Ref_pt)
{
  // This matrix is available from a variety of sources. See for
  // example the GroundMSPI L1B2 ATB.
  return quat_rot("zx", (Ref_pt.longitude() - 270) * Constant::deg_to_rad, 
		  (90 - Ref_pt.latitude()) * Constant::deg_to_rad);
}


//-----------------------------------------------------------------------
/// Return the solar look vector, which points from the center of the
/// the earth to the sun.
//-----------------------------------------------------------------------

CartesianFixedLookVector CartesianFixedLookVector::solar_look_vector
(const Time& T)
{
  return CartesianFixedLookVector(Ecr::sub_solar_point(T).position);
}

//-----------------------------------------------------------------------
/// Return the body look vector, which points from the center of the
/// the earth to the body.
//-----------------------------------------------------------------------

CartesianFixedLookVector CartesianFixedLookVector::body_look_vector
(int Body_id, const Time& T)
{
  return CartesianFixedLookVector(Ecr::sub_body_point(Body_id, T).position);
}

//-----------------------------------------------------------------------
/// Return the solar look vector, which points from the given
/// reference point to the sun in the local north coordinates. Can
/// calculate solar zenith and azimuth from this.
//-----------------------------------------------------------------------

LnLookVector LnLookVector::solar_look_vector
(const Time& T, 
 const GroundCoordinate& Ref_pt)
{
  return LnLookVector(CartesianFixedLookVector::solar_look_vector(T), Ref_pt);
}

//-----------------------------------------------------------------------
/// Return the body look vector, which points from the given
/// reference point to the body in the local north coordinates. Can
/// calculate body zenith and azimuth from this.
//-----------------------------------------------------------------------

LnLookVector LnLookVector::body_look_vector
(int Body_id, const Time& T, 
 const GroundCoordinate& Ref_pt)
{
  return LnLookVector(CartesianFixedLookVector::body_look_vector(Body_id, T),
		      Ref_pt);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void DcsLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in Detector Coordinate System coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void DcsLookVectorWithDerivative::print(std::ostream& Os) const
{
  boost::array<AutoDerivative<double>, 3> d = direction();
  Os << "Look vector in Detector Coordinate System coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Sometimes we compare against something that uses field angles, so
/// return the x and y field angle in degrees for the given look
/// direction. 
//-----------------------------------------------------------------------

void ScLookVector::field_angle(double& Fa_x_deg, double& Fa_y_deg) const
{
  Fa_x_deg = Constant::rad_to_deg * atan2(look_vector[0], look_vector[2]);
  Fa_y_deg = Constant::rad_to_deg * atan2(look_vector[1], look_vector[2]);
}

//-----------------------------------------------------------------------
/// Create a ScLookVector from the field angle values in degrees.
//-----------------------------------------------------------------------

ScLookVector ScLookVector::create_from_field_angle
(double Fa_x_deg, double Fa_y_deg)
{
  return ScLookVector(tan(Fa_x_deg * Constant::deg_to_rad),
		      tan(Fa_y_deg * Constant::deg_to_rad),
		      1);
}
