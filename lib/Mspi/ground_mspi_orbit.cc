#include "ground_mspi_orbit.h"
#include "constant.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. The azimuth and zenith angles should be in degrees.
//-----------------------------------------------------------------------

boost::shared_ptr<OrbitData> 
GroundMspiOrbit::orbit_data(Time T) const
{
  // Space craft to local north
  double zenith = (start_elevation_angle_ + 
		   (T - tstart) * rotation_rate_);
  boost::math::quaternion<double> sc_to_ln = 
    quat_rot("zy", (90 - azimuth_) * Constant::deg_to_rad,
	     -zenith * Constant::deg_to_rad);
  boost::math::quaternion<double> ln_to_cf = LnLookVector::enu_to_cf(*pos);
  boost::math::quaternion<double>  sc_to_cf = ln_to_cf * sc_to_ln;
  boost::array<double, 3> vel = {{0,0,0}};
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(T, pos->convert_to_cf(), vel, sc_to_cf));
}

boost::shared_ptr<OrbitData> 
GroundMspiOrbit::orbit_data(const TimeWithDerivative& T) const
{
  // Space craft to local north
  AutoDerivative<double> zenith = (start_elevation_angle_ + 
		   (T - tstart) * rotation_rate_);
  boost::math::quaternion<AutoDerivative<double> > sc_to_ln = 
    quat_rot("zy", 
	     AutoDerivative<double>((90 - azimuth_) * Constant::deg_to_rad),
	     -zenith * Constant::deg_to_rad);
  boost::math::quaternion<double> ln_to_cf = LnLookVector::enu_to_cf(*pos);
  boost::math::quaternion<AutoDerivative<double> >  
    sc_to_cf = ln_to_cf * sc_to_ln;
  boost::array<AutoDerivative<double>, 3> vel = {{AutoDerivative<double>(0),
						  AutoDerivative<double>(0),
						  AutoDerivative<double>(0)}};
  boost::shared_ptr<CartesianFixed> p = pos->convert_to_cf();
  boost::array<AutoDerivative<double>, 3> pos_der = 
    {{AutoDerivative<double>(p->position[0]),
      AutoDerivative<double>(p->position[1]),
      AutoDerivative<double>(p->position[2])}};
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(T, p, pos_der, vel, sc_to_cf));
}

void GroundMspiOrbit::print(std::ostream& Os) const {
  Os << "GroundMspiOrbit\n"
     << "  Start time: " << tstart << "\n"
     << "  Position:   " << *pos << "\n"
     << "  Azimuth:    " << azimuth_ << " deg\n"
     << "  Start elevation angle: " << start_elevation_angle_ << " deg\n"
     << "  Rotation rate:         " << rotation_rate_ << " deg/s\n";
}

