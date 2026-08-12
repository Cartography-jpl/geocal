#include "ypr_orbit_data.h"
#include "geocal_serialize_support.h"
#include "geocal_matrix.h"
#include "ground_coordinate.h"
#include "geocal_exception.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void YprOrbitData::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionOrbitData)
     & GEOCAL_NVP_(yaw)
     & GEOCAL_NVP_(pitch)
     & GEOCAL_NVP_(roll)
     & GEOCAL_NVP_(euler_order);
}

GEOCAL_IMPLEMENT(YprOrbitData);
#endif

//-----------------------------------------------------------------------
/// Calculate orbital frame to ECI transformation from position and velocity
/// This follows the DprepOrbit convention (dprep_file.py lines 268-275):
///   z = -position / ||position||  (nadir, toward Earth center)
///   x = velocity orthogonalized to z (along-track)
///   y = cross(z, x) (cross-track)
//-----------------------------------------------------------------------

boost::math::quaternion<double>
YprOrbitData::calc_orbital_to_ci(const boost::array<double, 3>& pos_ci,
                                  const boost::array<double, 3>& vel_ci)
{
  // Calculate z-axis (nadir)
  double pos_norm = std::sqrt(pos_ci[0]*pos_ci[0] +
                              pos_ci[1]*pos_ci[1] +
                              pos_ci[2]*pos_ci[2]);
  boost::array<double, 3> z = {{-pos_ci[0]/pos_norm,
                                -pos_ci[1]/pos_norm,
                                -pos_ci[2]/pos_norm}};

  // Calculate x-axis (velocity direction, orthogonalized to z)
  double v_dot_z = vel_ci[0]*z[0] + vel_ci[1]*z[1] + vel_ci[2]*z[2];
  boost::array<double, 3> x = {{vel_ci[0] - z[0]*v_dot_z,
                                vel_ci[1] - z[1]*v_dot_z,
                                vel_ci[2] - z[2]*v_dot_z}};
  double x_norm = std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
  x[0] /= x_norm;
  x[1] /= x_norm;
  x[2] /= x_norm;

  // Calculate y-axis (cross product: y = cross(z, x))
  boost::array<double, 3> y = {{z[1]*x[2] - z[2]*x[1],
                                z[2]*x[0] - z[0]*x[2],
                                z[0]*x[1] - z[1]*x[0]}};

  // Create rotation matrix with orbital frame axes as columns
  double m[3][3];
  m[0][0] = x[0]; m[0][1] = y[0]; m[0][2] = z[0];
  m[1][0] = x[1]; m[1][1] = y[1]; m[1][2] = z[1];
  m[2][0] = x[2]; m[2][1] = y[2]; m[2][2] = z[2];

  return matrix_to_quaternion(m);
}

//-----------------------------------------------------------------------
/// Constructor from QuaternionOrbitData
/// Extracts Euler angles from quaternion relative to local orbital frame
//-----------------------------------------------------------------------

YprOrbitData::YprOrbitData(const QuaternionOrbitData& Od,
                           const std::string& Euler_order)
  : euler_order_(Euler_order)
{
  // Get position and velocity in CI frame (always use CI for orbital frame calc)
  boost::array<double, 3> pos_ci = {{Od.position_ci()->position[0],
                                     Od.position_ci()->position[1],
                                     Od.position_ci()->position[2]}};
  boost::array<double, 3> vel_ci = Od.velocity_ci();

  // Calculate orbital-to-eci transformation
  boost::math::quaternion<double> orbital_to_eci = calc_orbital_to_ci(pos_ci, vel_ci);

  // Get spacecraft-to-eci quaternion
  boost::math::quaternion<double> sc_to_eci = Od.sc_to_ci();

  // Extract spacecraft-to-orbital quaternion
  // sc_to_eci = orbital_to_eci * sc_to_orbital
  // => sc_to_orbital = conj(orbital_to_eci) * sc_to_eci
  boost::math::quaternion<double> sc_to_orbital = conj(orbital_to_eci) * sc_to_eci;

  // Convert quaternion to Euler angles
  double e1, e2, e3;
  quat_to_euler(sc_to_orbital, e1, e2, e3);

  // Map euler angles to yaw/pitch/roll based on order
  // For "ZYX": e1=yaw(Z), e2=pitch(Y), e3=roll(X)
  // For "XYZ": e1=roll(X), e2=pitch(Y), e3=yaw(Z)
  for(int i = 0; i < 3; ++i) {
    double angle_deg = (i == 0 ? e1 : (i == 1 ? e2 : e3)) * Constant::rad_to_deg;
    char axis = Euler_order[i];
    if(axis == 'Z' || axis == 'z' || axis == '3') {
      yaw_ = angle_deg;
    } else if(axis == 'Y' || axis == 'y' || axis == '2') {
      pitch_ = angle_deg;
    } else if(axis == 'X' || axis == 'x' || axis == '1') {
      roll_ = angle_deg;
    }
  }

  // Initialize parent with same state as Od
  // Use the original sc_to_eci quaternion to preserve exact transformation
  if(Od.prefer_cf()) {
    QuaternionOrbitData::initialize(Od.time(), Od.position_cf(),
                                    Od.velocity_cf(), Od.sc_to_cf());
  } else {
    QuaternionOrbitData::initialize(Od.time(), Od.position_ci(),
                                    Od.velocity_ci(), sc_to_eci);
  }
}

//-----------------------------------------------------------------------
/// Constructor with yaw, pitch, roll and CartesianFixed position
//-----------------------------------------------------------------------

YprOrbitData::YprOrbitData(const Time& Tm,
                           const boost::shared_ptr<CartesianFixed>& pos_cf,
                           const boost::array<double, 3>& vel_fixed,
                           double Yaw, double Pitch, double Roll,
                           const std::string& Euler_order)
{
  initialize_from_ypr(Tm, pos_cf, vel_fixed, Yaw, Pitch, Roll, Euler_order);
}

//-----------------------------------------------------------------------
/// Constructor with yaw, pitch, roll and CartesianInertial position
//-----------------------------------------------------------------------

YprOrbitData::YprOrbitData(const Time& Tm,
                           const boost::shared_ptr<CartesianInertial>& pos_ci,
                           const boost::array<double, 3>& vel_inertial,
                           double Yaw, double Pitch, double Roll,
                           const std::string& Euler_order)
{
  initialize_from_ypr(Tm, pos_ci, vel_inertial, Yaw, Pitch, Roll, Euler_order);
}

//-----------------------------------------------------------------------
/// Initialize from yaw, pitch, roll (degrees) with CartesianFixed
//-----------------------------------------------------------------------

void YprOrbitData::initialize_from_ypr(
    const Time& Tm,
    const boost::shared_ptr<CartesianFixed>& pos_cf,
    const boost::array<double, 3>& vel_fixed,
    double Yaw, double Pitch, double Roll,
    const std::string& Euler_order)
{
  yaw_ = Yaw;
  pitch_ = Pitch;
  roll_ = Roll;
  euler_order_ = Euler_order;

  // Convert position and velocity to CI for orbital frame calculation
  boost::shared_ptr<CartesianInertial> pos_ci;
  boost::array<double, 3> vel_ci;
  convert_position_and_velocity(Tm, *pos_cf, vel_fixed, pos_ci, vel_ci);

  boost::array<double, 3> pos_ci_arr = {{pos_ci->position[0],
                                         pos_ci->position[1],
                                         pos_ci->position[2]}};

  // Calculate orbital-to-eci transformation
  boost::math::quaternion<double> orbital_to_eci = calc_orbital_to_ci(pos_ci_arr, vel_ci);

  // Map yaw/pitch/roll to euler angles based on order
  double e1, e2, e3;
  for(int i = 0; i < 3; ++i) {
    char axis = Euler_order[i];
    double angle_deg;
    if(axis == 'Z' || axis == 'z' || axis == '3') {
      angle_deg = Yaw;
    } else if(axis == 'Y' || axis == 'y' || axis == '2') {
      angle_deg = Pitch;
    } else if(axis == 'X' || axis == 'x' || axis == '1') {
      angle_deg = Roll;
    } else {
      Exception e;
      e << "Invalid euler order character: " << axis;
      throw e;
    }
    if(i == 0) e1 = angle_deg * Constant::deg_to_rad;
    else if(i == 1) e2 = angle_deg * Constant::deg_to_rad;
    else e3 = angle_deg * Constant::deg_to_rad;
  }

  // Create sc_to_orbital quaternion
  boost::math::quaternion<double> sc_to_orbital = quat_rot(Euler_order, e1, e2, e3);

  // Combine to get sc_to_ci quaternion
  // sc_to_eci = orbital_to_eci * sc_to_orbital
  boost::math::quaternion<double> sc_to_ci = orbital_to_eci * sc_to_orbital;

  // Convert back to CF frame for the final quaternion
  boost::shared_ptr<CartesianFixed> pos_cf_result;
  boost::array<double, 3> vel_cf_result;
  boost::math::quaternion<double> ci_to_cf_q;
  convert_position_and_velocity(Tm, *pos_ci, vel_ci, pos_cf_result, vel_cf_result, ci_to_cf_q);

  // Transform quaternion to CF frame: sc_to_cf = ci_to_cf_q * sc_to_ci
  boost::math::quaternion<double> sc_to_cf = ci_to_cf_q * sc_to_ci;

  // Initialize parent QuaternionOrbitData
  QuaternionOrbitData::initialize(Tm, pos_cf, vel_fixed, sc_to_cf);
}

//-----------------------------------------------------------------------
/// Initialize from yaw, pitch, roll (degrees) with CartesianInertial
//-----------------------------------------------------------------------

void YprOrbitData::initialize_from_ypr(
    const Time& Tm,
    const boost::shared_ptr<CartesianInertial>& pos_ci,
    const boost::array<double, 3>& vel_inertial,
    double Yaw, double Pitch, double Roll,
    const std::string& Euler_order)
{
  yaw_ = Yaw;
  pitch_ = Pitch;
  roll_ = Roll;
  euler_order_ = Euler_order;

  // Get position and velocity arrays
  boost::array<double, 3> pos_ci_arr = {{pos_ci->position[0],
                                         pos_ci->position[1],
                                         pos_ci->position[2]}};

  // Calculate orbital-to-eci transformation
  boost::math::quaternion<double> orbital_to_eci = calc_orbital_to_ci(pos_ci_arr, vel_inertial);

  // Map yaw/pitch/roll to euler angles based on order
  double e1, e2, e3;
  for(int i = 0; i < 3; ++i) {
    char axis = Euler_order[i];
    double angle_deg;
    if(axis == 'Z' || axis == 'z' || axis == '3') {
      angle_deg = Yaw;
    } else if(axis == 'Y' || axis == 'y' || axis == '2') {
      angle_deg = Pitch;
    } else if(axis == 'X' || axis == 'x' || axis == '1') {
      angle_deg = Roll;
    } else {
      Exception e;
      e << "Invalid euler order character: " << axis;
      throw e;
    }
    if(i == 0) e1 = angle_deg * Constant::deg_to_rad;
    else if(i == 1) e2 = angle_deg * Constant::deg_to_rad;
    else e3 = angle_deg * Constant::deg_to_rad;
  }

  // Create sc_to_orbital quaternion
  boost::math::quaternion<double> sc_to_orbital = quat_rot(Euler_order, e1, e2, e3);

  // Combine to get sc_to_ci quaternion
  // sc_to_eci = orbital_to_eci * sc_to_orbital
  boost::math::quaternion<double> sc_to_ci = orbital_to_eci * sc_to_orbital;

  // Initialize parent QuaternionOrbitData
  QuaternionOrbitData::initialize(Tm, pos_ci, vel_inertial, sc_to_ci);
}

//-----------------------------------------------------------------------
/// Calculate the orbital-to-eci transformation for current state
//-----------------------------------------------------------------------

boost::math::quaternion<double> YprOrbitData::orbital_to_eci() const
{
  boost::array<double, 3> pos_ci = {{position_ci()->position[0],
                                     position_ci()->position[1],
                                     position_ci()->position[2]}};
  boost::array<double, 3> vel_ci = velocity_ci();
  return calc_orbital_to_ci(pos_ci, vel_ci);
}

//-----------------------------------------------------------------------
/// Helper functions to map yaw/pitch/roll to euler angle positions
//-----------------------------------------------------------------------

double YprOrbitData::euler_angle1() const
{
  char axis = euler_order_[0];
  if(axis == 'Z' || axis == 'z' || axis == '3') return yaw_;
  if(axis == 'Y' || axis == 'y' || axis == '2') return pitch_;
  if(axis == 'X' || axis == 'x' || axis == '1') return roll_;
  Exception e;
  e << "Invalid euler order character: " << axis;
  throw e;
}

double YprOrbitData::euler_angle2() const
{
  char axis = euler_order_[1];
  if(axis == 'Z' || axis == 'z' || axis == '3') return yaw_;
  if(axis == 'Y' || axis == 'y' || axis == '2') return pitch_;
  if(axis == 'X' || axis == 'x' || axis == '1') return roll_;
  Exception e;
  e << "Invalid euler order character: " << axis;
  throw e;
}

double YprOrbitData::euler_angle3() const
{
  char axis = euler_order_[2];
  if(axis == 'Z' || axis == 'z' || axis == '3') return yaw_;
  if(axis == 'Y' || axis == 'y' || axis == '2') return pitch_;
  if(axis == 'X' || axis == 'x' || axis == '1') return roll_;
  Exception e;
  e << "Invalid euler order character: " << axis;
  throw e;
}

//-----------------------------------------------------------------------
/// Print to stream
//-----------------------------------------------------------------------

void YprOrbitData::print(std::ostream& Os) const
{
  Os << "YprOrbitData:\n"
     << "  Time:          " << time() << "\n"
     << "  Position:      " << *position_ci() << "\n"
     << "  Euler Order:   " << euler_order() << "\n"
     << "  Yaw:           " << yaw() << " deg\n"
     << "  Pitch:         " << pitch() << " deg\n"
     << "  Roll:          " << roll() << " deg\n";
}
