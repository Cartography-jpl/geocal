# YprOrbitData Implementation Plan

## Executive Summary

**Purpose**: Create `YprOrbitData`, a C++ class that extends `QuaternionOrbitData` to represent spacecraft attitude using Euler angles (yaw, pitch, roll) relative to a local orbital frame (LVLH).

**Key Features**:
- Euler angles relative to **Local Vertical Local Horizontal (LVLH)** frame
- **Configurable Euler angle order** (default "ZYX", supports all orders like "XYZ", "321", etc.)
- **Dual accessors**: generic (`euler_angle1/2/3`) and physical (`yaw/pitch/roll`)
- Conversion to/from `QuaternionOrbitData`
- Supports both `CartesianFixed` and `CartesianInertial` coordinates

**Status**: ✅ Ready for implementation - All design questions resolved

## Overview

Create a new `YprOrbitData` C++ class that extends `QuaternionOrbitData` and provides Euler angles in degrees. This is similar to `AircraftOrbitData` but uses Euler angles relative to a **local orbital frame** instead of a local north frame.

## Key Design Decisions

### Coordinate Frame Convention

**Local Orbital Frame Definition** (from DprepOrbit, lines 268-275 in dprep_file.py):
- **z-axis**: Points toward Earth center (nadir): `-position / ||position||`
- **x-axis**: In velocity direction, orthogonalized to z: `(velocity - z·velocity·z) / ||(velocity - z·velocity·z)||`
- **y-axis**: Completes right-handed system: `cross(z, x)`

This is a **Local Vertical Local Horizontal (LVLH)** frame, also called an orbital frame.

**Euler Angle Convention:**
- The Euler angle order (e.g., "ZYX", "XYZ", "321") is specified as a **string parameter**
- Default order: **"ZYX"** (Yaw-Pitch-Roll)
- The order can be overridden in the constructor to support any rotation sequence
- Euler angles are stored in radians internally, exposed as degrees via accessor methods

**Transformation Chain:**
```
Spacecraft Body → (Euler angles) → Orbital Frame → (orbital_to_eci) → ECI/ECR
```

Where:
- `sc_to_orbital = quat_rot(euler_order, angle1, angle2, angle3)`
- `orbital_to_eci = matrix_to_quaternion([x, y, z])` where x, y, z are the orbital frame axes
- `sc_to_eci = orbital_to_eci * sc_to_orbital`

### Position Storage

Like `QuaternionOrbitData`, the position can be stored in either:
- **CartesianFixed** (ECR - Earth-Centered Rotating)
- **CartesianInertial** (ECI - Earth-Centered Inertial)

The constructor takes either CF or CI, and the class **prefers** whichever was initially provided (stored as `prefer_cf_` flag). Data can be reported in either coordinate system through the base class interface.

### Conversion from QuaternionOrbitData

The key constructor takes a `QuaternionOrbitData` and extracts yaw, pitch, roll:

```cpp
YprOrbitData::YprOrbitData(const QuaternionOrbitData& Od, 
                           const std::string& Euler_order)
{
  // 1. Calculate the local orbital frame (LVLH)
  //    z = -position / ||position||  (nadir)
  //    x = velocity orthogonalized to z and normalized
  //    y = cross(z, x)
  
  // 2. Get spacecraft-to-eci quaternion from Od
  boost::math::quaternion<double> sc_to_eci = Od.sc_to_ci();
  
  // 3. Calculate orbital-to-eci quaternion from frame axes
  boost::math::quaternion<double> orbital_to_eci = matrix_to_quaternion(orbital_frame_matrix);
  
  // 4. Extract sc_to_orbital quaternion
  //    sc_to_eci = orbital_to_eci * sc_to_orbital
  //    => sc_to_orbital = conj(orbital_to_eci) * sc_to_eci
  boost::math::quaternion<double> sc_to_orbital = conj(orbital_to_eci) * sc_to_eci;
  
  // 5. Convert sc_to_orbital quaternion to Euler angles
  quat_to_euler(sc_to_orbital, euler1, euler2, euler3);
  
  // 6. Store angles in degrees
  euler_angle_[0] = euler1 * Constant::rad_to_deg;
  euler_angle_[1] = euler2 * Constant::rad_to_deg;
  euler_angle_[2] = euler3 * Constant::rad_to_deg;
  
  // 7. Store euler order and initialize parent
  euler_order_ = Euler_order;
  QuaternionOrbitData::initialize(...);
}
```

## Implementation Files

### 1. Header File: `ypr_orbit_data.h`

Location: `/home/smyth/Local/geocal-repo/lib/GeoCalCore/Implementation/ypr_orbit_data.h`

```cpp
#ifndef YPR_ORBIT_DATA_H
#define YPR_ORBIT_DATA_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This class stores orbit data with attitude represented as 
  Yaw, Pitch, Roll (YPR) angles in degrees. This is a common
  representation for spacecraft attitude.
  
  The YPR angles follow the standard aerospace convention:
  - Yaw (ψ): Rotation about Z-axis
  - Pitch (θ): Rotation about Y-axis  
  - Roll (φ): Rotation about X-axis
  - Rotation order: Yaw first, then Pitch, then Roll
  
  This class can be constructed from a QuaternionOrbitData,
  extracting the YPR representation from the quaternion.
  
  The position can be in either CartesianFixed or CartesianInertial
  coordinates, inherited from QuaternionOrbitData.
*******************************************************************/

class YprOrbitData : public QuaternionOrbitData {
public:
  //-----------------------------------------------------------------------
  /// Constructor from QuaternionOrbitData
  /// Extracts Euler angles from the quaternion attitude relative to local orbital frame
  /// 
  /// @param Od - QuaternionOrbitData to convert from
  /// @param Euler_order - String specifying Euler angle order (e.g., "ZYX", "XYZ", "321")
  ///                      Default is "ZYX" (Yaw-Pitch-Roll)
  //-----------------------------------------------------------------------
  YprOrbitData(const QuaternionOrbitData& Od, 
               const std::string& Euler_order = "ZYX");

  //-----------------------------------------------------------------------
  /// Constructor with explicit Euler angle values and CartesianFixed position
  /// 
  /// @param Tm - Time
  /// @param pos_cf - Position in CartesianFixed coordinates
  /// @param vel_fixed - Velocity in CartesianFixed coordinates
  /// @param Euler1 - First Euler angle in degrees
  /// @param Euler2 - Second Euler angle in degrees  
  /// @param Euler3 - Third Euler angle in degrees
  /// @param Euler_order - String specifying rotation order (default "ZYX")
  //-----------------------------------------------------------------------
  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianFixed>& pos_cf,
               const boost::array<double, 3>& vel_fixed,
               double Euler1, double Euler2, double Euler3,
               const std::string& Euler_order = "ZYX");

  //-----------------------------------------------------------------------
  /// Constructor with explicit Euler angle values and CartesianInertial position
  /// 
  /// @param Tm - Time
  /// @param pos_ci - Position in CartesianInertial coordinates
  /// @param vel_inertial - Velocity in CartesianInertial coordinates
  /// @param Euler1 - First Euler angle in degrees
  /// @param Euler2 - Second Euler angle in degrees
  /// @param Euler3 - Third Euler angle in degrees
  /// @param Euler_order - String specifying rotation order (default "ZYX")
  //-----------------------------------------------------------------------
  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianInertial>& pos_ci,
               const boost::array<double, 3>& vel_inertial,
               double Euler1, double Euler2, double Euler3,
               const std::string& Euler_order = "ZYX");

  //-----------------------------------------------------------------------
  /// Destructor
  //-----------------------------------------------------------------------
  virtual ~YprOrbitData() {}
  
  virtual void print(std::ostream& Os) const;

  //-----------------------------------------------------------------------
  /// First Euler angle in degrees (typically Yaw if using "ZYX" order)
  //-----------------------------------------------------------------------
  double euler_angle1() const { return euler_angle_[0]; }

  //-----------------------------------------------------------------------
  /// Second Euler angle in degrees (typically Pitch if using "ZYX" order)
  //-----------------------------------------------------------------------
  double euler_angle2() const { return euler_angle_[1]; }

  //-----------------------------------------------------------------------
  /// Third Euler angle in degrees (typically Roll if using "ZYX" order)
  //-----------------------------------------------------------------------
  double euler_angle3() const { return euler_angle_[2]; }
  
  //-----------------------------------------------------------------------
  /// Yaw angle in degrees (rotation about Z-axis)
  /// Returns the Euler angle corresponding to Z rotation regardless of order
  //-----------------------------------------------------------------------
  double yaw() const;

  //-----------------------------------------------------------------------
  /// Pitch angle in degrees (rotation about Y-axis)
  /// Returns the Euler angle corresponding to Y rotation regardless of order
  //-----------------------------------------------------------------------
  double pitch() const;

  //-----------------------------------------------------------------------
  /// Roll angle in degrees (rotation about X-axis)
  /// Returns the Euler angle corresponding to X rotation regardless of order
  //-----------------------------------------------------------------------
  double roll() const;
  
  //-----------------------------------------------------------------------
  /// Euler angle order string (e.g., "ZYX", "XYZ")
  //-----------------------------------------------------------------------
  const std::string& euler_order() const { return euler_order_; }
  
  //-----------------------------------------------------------------------
  /// Calculate the orbital-to-eci transformation quaternion for current position/velocity
  //-----------------------------------------------------------------------
  boost::math::quaternion<double> orbital_to_eci() const;

protected:
  YprOrbitData() {}
  
  void initialize_from_euler(const Time& Tm,
                             const boost::shared_ptr<CartesianFixed>& pos_cf,
                             const boost::array<double, 3>& vel_fixed,
                             double Euler1, double Euler2, double Euler3,
                             const std::string& Euler_order);
  
  void initialize_from_euler(const Time& Tm,
                             const boost::shared_ptr<CartesianInertial>& pos_ci,
                             const boost::array<double, 3>& vel_inertial,
                             double Euler1, double Euler2, double Euler3,
                             const std::string& Euler_order);
  
  static boost::math::quaternion<double> 
  calc_orbital_to_ci(const boost::array<double, 3>& pos_ci,
                     const boost::array<double, 3>& vel_ci);
  
  int yaw_index() const;
  int pitch_index() const;
  int roll_index() const;

private:
  boost::array<double, 3> euler_angle_;  // In degrees
  std::string euler_order_;  // e.g., "ZYX", "XYZ", "321"
  
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(YprOrbitData);
#endif
```

### 2. Implementation File: `ypr_orbit_data.cc`

Location: `/home/smyth/Local/geocal-repo/lib/GeoCalCore/Implementation/ypr_orbit_data.cc`

```cpp
#include "ypr_orbit_data.h"
#include "geocal_serialize_support.h"
#include "geocal_matrix.h"
#include "ground_coordinate.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void YprOrbitData::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionOrbitData)
     & GEOCAL_NVP_(euler_angle)
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
  
  // Store angles in degrees
  euler_angle_[0] = e1 * Constant::rad_to_deg;
  euler_angle_[1] = e2 * Constant::rad_to_deg;
  euler_angle_[2] = e3 * Constant::rad_to_deg;
  
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
/// Constructor with explicit Euler angle values and CartesianFixed position
//-----------------------------------------------------------------------

YprOrbitData::YprOrbitData(const Time& Tm,
                           const boost::shared_ptr<CartesianFixed>& pos_cf,
                           const boost::array<double, 3>& vel_fixed,
                           double Euler1, double Euler2, double Euler3,
                           const std::string& Euler_order)
{
  initialize_from_euler(Tm, pos_cf, vel_fixed, Euler1, Euler2, Euler3, Euler_order);
}

//-----------------------------------------------------------------------
/// Constructor with explicit Euler angle values and CartesianInertial position
//-----------------------------------------------------------------------

YprOrbitData::YprOrbitData(const Time& Tm,
                           const boost::shared_ptr<CartesianInertial>& pos_ci,
                           const boost::array<double, 3>& vel_inertial,
                           double Euler1, double Euler2, double Euler3,
                           const std::string& Euler_order)
{
  initialize_from_euler(Tm, pos_ci, vel_inertial, Euler1, Euler2, Euler3, Euler_order);
}

//-----------------------------------------------------------------------
/// Initialize from Euler angles (degrees) with CartesianFixed
//-----------------------------------------------------------------------

void YprOrbitData::initialize_from_euler(
    const Time& Tm,
    const boost::shared_ptr<CartesianFixed>& pos_cf,
    const boost::array<double, 3>& vel_fixed,
    double Euler1, double Euler2, double Euler3,
    const std::string& Euler_order)
{
  euler_angle_[0] = Euler1;
  euler_angle_[1] = Euler2;
  euler_angle_[2] = Euler3;
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
  
  // Convert Euler angles from degrees to radians and create sc_to_orbital quaternion
  double e1_rad = Euler1 * Constant::deg_to_rad;
  double e2_rad = Euler2 * Constant::deg_to_rad;
  double e3_rad = Euler3 * Constant::deg_to_rad;
  boost::math::quaternion<double> sc_to_orbital =
    quat_rot(Euler_order, e1_rad, e2_rad, e3_rad);
  
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
/// Initialize from Euler angles (degrees) with CartesianInertial
//-----------------------------------------------------------------------

void YprOrbitData::initialize_from_euler(
    const Time& Tm,
    const boost::shared_ptr<CartesianInertial>& pos_ci,
    const boost::array<double, 3>& vel_inertial,
    double Euler1, double Euler2, double Euler3,
    const std::string& Euler_order)
{
  euler_angle_[0] = Euler1;
  euler_angle_[1] = Euler2;
  euler_angle_[2] = Euler3;
  euler_order_ = Euler_order;
  
  // Get position and velocity arrays
  boost::array<double, 3> pos_ci_arr = {{pos_ci->position[0],
                                         pos_ci->position[1],
                                         pos_ci->position[2]}};
  
  // Calculate orbital-to-eci transformation
  boost::math::quaternion<double> orbital_to_eci = calc_orbital_to_ci(pos_ci_arr, vel_inertial);
  
  // Convert Euler angles from degrees to radians and create sc_to_orbital quaternion
  double e1_rad = Euler1 * Constant::deg_to_rad;
  double e2_rad = Euler2 * Constant::deg_to_rad;
  double e3_rad = Euler3 * Constant::deg_to_rad;
  boost::math::quaternion<double> sc_to_orbital =
    quat_rot(Euler_order, e1_rad, e2_rad, e3_rad);
  
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
/// Helper functions to map euler_order to yaw/pitch/roll indices
//-----------------------------------------------------------------------

int YprOrbitData::yaw_index() const
{
  // Yaw is rotation about Z-axis
  for(int i = 0; i < 3; ++i) {
    if(euler_order_[i] == 'Z' || euler_order_[i] == 'z' || euler_order_[i] == '3')
      return i;
  }
  throw Exception("Euler order string must contain 'Z', 'z', or '3' for yaw");
}

int YprOrbitData::pitch_index() const
{
  // Pitch is rotation about Y-axis
  for(int i = 0; i < 3; ++i) {
    if(euler_order_[i] == 'Y' || euler_order_[i] == 'y' || euler_order_[i] == '2')
      return i;
  }
  throw Exception("Euler order string must contain 'Y', 'y', or '2' for pitch");
}

int YprOrbitData::roll_index() const
{
  // Roll is rotation about X-axis
  for(int i = 0; i < 3; ++i) {
    if(euler_order_[i] == 'X' || euler_order_[i] == 'x' || euler_order_[i] == '1')
      return i;
  }
  throw Exception("Euler order string must contain 'X', 'x', or '1' for roll");
}

//-----------------------------------------------------------------------
/// Yaw angle in degrees (rotation about Z-axis)
//-----------------------------------------------------------------------

double YprOrbitData::yaw() const
{
  return euler_angle_[yaw_index()];
}

//-----------------------------------------------------------------------
/// Pitch angle in degrees (rotation about Y-axis)
//-----------------------------------------------------------------------

double YprOrbitData::pitch() const
{
  return euler_angle_[pitch_index()];
}

//-----------------------------------------------------------------------
/// Roll angle in degrees (rotation about X-axis)
//-----------------------------------------------------------------------

double YprOrbitData::roll() const
{
  return euler_angle_[roll_index()];
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
     << "  Euler Angle 1: " << euler_angle1() << " deg\n"
     << "  Euler Angle 2: " << euler_angle2() << " deg\n"
     << "  Euler Angle 3: " << euler_angle3() << " deg\n";
}
```

### 3. SWIG Interface: `ypr_orbit_data.i`

Location: `/home/smyth/Local/geocal-repo/lib/GeoCalCore/Implementation/ypr_orbit_data.i`

```swig
// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ypr_orbit_data.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::YprOrbitData);

namespace GeoCal {
class YprOrbitData : public QuaternionOrbitData {
public:
  YprOrbitData(const QuaternionOrbitData& Od, 
               const std::string& Euler_order = "ZYX");
  
  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianFixed>& pos_cf,
               const boost::array<double, 3>& vel_fixed,
               double Euler1, double Euler2, double Euler3,
               const std::string& Euler_order = "ZYX");
  
  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianInertial>& pos_ci,
               const boost::array<double, 3>& vel_inertial,
               double Euler1, double Euler2, double Euler3,
               const std::string& Euler_order = "ZYX");
  
  %python_attribute(euler_angle1, double)
  %python_attribute(euler_angle2, double)
  %python_attribute(euler_angle3, double)
  %python_attribute(yaw, double)
  %python_attribute(pitch, double)
  %python_attribute(roll, double)
  %python_attribute(euler_order, std::string)
  %python_attribute(orbital_to_eci, boost::math::quaternion<double>)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("YprOrbitData")
```

### 4. Unit Test: `ypr_orbit_data_test.cc`

Location: `/home/smyth/Local/geocal-repo/lib/GeoCalCore/Implementation/ypr_orbit_data_test.cc`

```cpp
#include "unit_test_support.h"
#include "ypr_orbit_data.h"
#include "ecr.h"

using namespace GeoCal;
using namespace blitz;

class YprOrbitDataFixture : public GlobalFixture {
public:
  YprOrbitDataFixture() {
    // Create test position and velocity using concrete Ecr/Eci classes
    t = Time::time_pgs(100.0);
    
    // Position on X-axis at ~7000 km altitude
    boost::array<double, 3> pos = {{7000e3, 0, 0}};
    pos_cf.reset(new Ecr(pos[0], pos[1], pos[2]));
    
    // Convert to ECI for alternate constructor
    pos_ci = pos_cf->convert_to_ci(t);
    
    // Typical orbital velocity
    vel_cf[0] = 0;
    vel_cf[1] = 7500;  // ~7.5 km/s
    vel_cf[2] = 0;
    
    // Convert velocity to CI
    boost::shared_ptr<CartesianInertial> pos_ci_tmp;
    convert_position_and_velocity(t, *pos_cf, vel_cf, pos_ci_tmp, vel_ci);
    
    // Test Euler angles in degrees (use distinct values to catch index errors)
    yaw = 10.0;
    pitch = 20.0;
    roll = 30.0;
    
    // Create sc_to_orbital quaternion from Euler angles for comparison
    double yaw_rad = yaw * Constant::deg_to_rad;
    double pitch_rad = pitch * Constant::deg_to_rad;
    double roll_rad = roll * Constant::deg_to_rad;
    quat_sc_to_orb = quat_rot("ZYX", yaw_rad, pitch_rad, roll_rad);
  }
  
  Time t;
  boost::shared_ptr<CartesianFixed> pos_cf;
  boost::shared_ptr<CartesianInertial> pos_ci;
  boost::array<double, 3> vel_cf, vel_ci;
  double yaw, pitch, roll;
  boost::math::quaternion<double> quat_sc_to_orb;
};

BOOST_FIXTURE_TEST_SUITE(ypr_orbit_data, YprOrbitDataFixture)

BOOST_AUTO_TEST_CASE(basic_construction_cf)
{
  // Construct with CartesianFixed and default "ZYX" order
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);
  
  // Check that Euler angles are stored correctly
  BOOST_CHECK_CLOSE(ypr.euler_angle1(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.euler_angle2(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.euler_angle3(), roll, 1e-6);
  
  // Check that yaw/pitch/roll accessors work
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);
  
  // Check euler order
  BOOST_CHECK_EQUAL(ypr.euler_order(), "ZYX");
  
  // Check position is preserved
  boost::shared_ptr<CartesianFixed> pos_result = ypr.position_cf();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_cf->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_cf->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_cf->position[2], 1e-6);
  
  // Check prefer_cf
  BOOST_CHECK_EQUAL(ypr.prefer_cf(), true);
}

BOOST_AUTO_TEST_CASE(basic_construction_ci)
{
  // Construct with CartesianInertial
  YprOrbitData ypr(t, pos_ci, vel_ci, yaw, pitch, roll);
  
  // Check that YPR values are stored correctly
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);
  
  // Check position
  boost::shared_ptr<CartesianInertial> pos_result = ypr.position_ci();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_ci->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_ci->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_ci->position[2], 1e-6);
  
  // Check prefer_cf
  BOOST_CHECK_EQUAL(ypr.prefer_cf(), false);
}

BOOST_AUTO_TEST_CASE(conversion_from_quaternion_cf)
{
  // Create QuaternionOrbitData with known quaternion
  QuaternionOrbitData qod(t, pos_cf, vel_cf, quat_test);
  
  // Convert to YprOrbitData
  YprOrbitData ypr(qod);
  
  // Check that YPR values match original
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-4);  // Slightly looser tolerance for round-trip
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-4);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-4);
  
  // Check position is preserved
  boost::shared_ptr<CartesianFixed> pos_result = ypr.position_cf();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_cf->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_cf->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_cf->position[2], 1e-6);
}

BOOST_AUTO_TEST_CASE(conversion_from_quaternion_ci)
{
  // Create QuaternionOrbitData with CartesianInertial
  QuaternionOrbitData qod(t, pos_ci, vel_ci, quat_test);
  
  // Convert to YprOrbitData
  YprOrbitData ypr(qod);
  
  // Check that YPR values match original
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-4);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-4);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-4);
}

BOOST_AUTO_TEST_CASE(round_trip_conversion)
{
  // Create YprOrbitData
  YprOrbitData ypr1(t, pos_cf, vel_cf, yaw, pitch, roll);
  
  // Convert to QuaternionOrbitData (implicit via base class)
  QuaternionOrbitData qod = ypr1;
  
  // Convert back to YprOrbitData
  YprOrbitData ypr2(qod);
  
  // Check that YPR values are preserved
  BOOST_CHECK_CLOSE(ypr2.yaw(), yaw, 1e-4);
  BOOST_CHECK_CLOSE(ypr2.pitch(), pitch, 1e-4);
  BOOST_CHECK_CLOSE(ypr2.roll(), roll, 1e-4);
}

BOOST_AUTO_TEST_CASE(zero_angles)
{
  // Test with zero angles (identity rotation)
  YprOrbitData ypr(t, pos_cf, vel_cf, 0.0, 0.0, 0.0);
  
  BOOST_CHECK_CLOSE(ypr.yaw(), 0.0, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), 0.0, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), 0.0, 1e-6);
  
  // Identity quaternion is (1, 0, 0, 0)
  boost::math::quaternion<double> quat = ypr.sc_to_cf();
  BOOST_CHECK_CLOSE(quat.R_component_1(), 1.0, 1e-6);
  BOOST_CHECK_CLOSE(quat.R_component_2(), 0.0, 1e-6);
  BOOST_CHECK_CLOSE(quat.R_component_3(), 0.0, 1e-6);
  BOOST_CHECK_CLOSE(quat.R_component_4(), 0.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(different_euler_order)
{
  // Test with XYZ order (roll, pitch, yaw)
  // With XYZ order: euler_angle1=roll, euler_angle2=pitch, euler_angle3=yaw
  YprOrbitData ypr(t, pos_cf, vel_cf, roll, pitch, yaw, "XYZ");
  
  // Check euler order
  BOOST_CHECK_EQUAL(ypr.euler_order(), "XYZ");
  
  // Check generic accessors match construction order
  BOOST_CHECK_CLOSE(ypr.euler_angle1(), roll, 1e-6);   // X rotation first
  BOOST_CHECK_CLOSE(ypr.euler_angle2(), pitch, 1e-6);  // Y rotation second
  BOOST_CHECK_CLOSE(ypr.euler_angle3(), yaw, 1e-6);    // Z rotation third
  
  // Check physical accessors map correctly regardless of order
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);     // Still gets Z rotation
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6); // Still gets Y rotation
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);   // Still gets X rotation
}

BOOST_AUTO_TEST_CASE(numeric_euler_order)
{
  // Test with numeric order "321" (equivalent to "ZYX")
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll, "321");
  
  BOOST_CHECK_EQUAL(ypr.euler_order(), "321");
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);
}

BOOST_AUTO_TEST_CASE(large_angles)
{
  // Test with large angles (distinct values to catch mixing)
  double yaw_large = 100.0;
  double pitch_large = 80.0;
  double roll_large = -45.0;
  
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw_large, pitch_large, roll_large);
  
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw_large, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch_large, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll_large, 1e-6);
}

BOOST_AUTO_TEST_CASE(look_vector_conversion)
{
  // Create YprOrbitData
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);
  
  // Create a test spacecraft look vector
  ScLookVector sl(1.0, 0.0, 0.0);  // Looking along +X in spacecraft frame
  
  // Convert to CartesianFixed look vector
  CartesianFixedLookVector cfl = ypr.cf_look_vector(sl);
  BOOST_CHECK(cfl.length() > 0.99 && cfl.length() < 1.01);
  
  // Convert back to spacecraft frame
  ScLookVector sl_back = ypr.sc_look_vector(cfl);
  BOOST_CHECK_CLOSE(sl_back.direction[0], sl.direction[0], 1e-4);
  BOOST_CHECK_CLOSE(sl_back.direction[1], sl.direction[1], 1e-4);
  BOOST_CHECK_CLOSE(sl_back.direction[2], sl.direction[2], 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);
  
  std::string d = serialize_write_string(ypr);
  if(false)
    std::cerr << d;
  
  boost::shared_ptr<YprOrbitData> ypr_restored = 
    serialize_read_string<YprOrbitData>(d);
  
  // Check all values are preserved
  BOOST_CHECK_CLOSE(ypr_restored->yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr_restored->pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr_restored->roll(), roll, 1e-6);
  
  boost::shared_ptr<CartesianFixed> pos_result = ypr_restored->position_cf();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_cf->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_cf->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_cf->position[2], 1e-6);
}

BOOST_AUTO_TEST_CASE(print)
{
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);
  
  std::ostringstream os;
  os << ypr;
  std::string output = os.str();
  
  // Check that output contains expected strings
  BOOST_CHECK(output.find("YprOrbitData") != std::string::npos);
  BOOST_CHECK(output.find("Yaw") != std::string::npos);
  BOOST_CHECK(output.find("Pitch") != std::string::npos);
  BOOST_CHECK(output.find("Roll") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
```

### 5. Build System Updates

Update `/home/smyth/Local/geocal-repo/lib/GeoCalCore/Implementation/implementation.am`:

```makefile
# Add to geocal_HEADERS
geocal_HEADERS+= @srclib@/ypr_orbit_data.h

# Add to libgeocal_la_SOURCES
libgeocal_la_SOURCES+= @srclib@/ypr_orbit_data.cc

# Add to SWIG_SRC
SWIG_SRC += @swigsrc@/ypr_orbit_data_wrap.cc

# Add to geocalswiginc_HEADERS
geocalswiginc_HEADERS+= @srclib@/ypr_orbit_data.i

# Add to geocal_test_all_SOURCES
geocal_test_all_SOURCES+= @srclib@/ypr_orbit_data_test.cc
```

## Remaining Questions and Clarifications

### 1. ~~Euler Angle Convention~~ ✓ RESOLVED
**Answer:** Euler angle order is now a parameter (default "ZYX"). All orders supported.

### 2. ~~Reference Frame~~ ✓ RESOLVED  
**Answer:** Euler angles are relative to local orbital frame (LVLH), as defined in DprepOrbit.

### 3. ~~DprepOrbit Reference~~ ✓ RESOLVED
**Answer:** Found in `python/geocal/dprep_file.py`. Orbital frame definition extracted from lines 268-275.

### 4. ~~Coordinate System Details~~ ✓ RESOLVED
**Answer:** Like QuaternionOrbitData, constructor takes either CF or CI, class prefers whichever was initially given.

### 5. ~~Additional Functionality~~ ✓ RESOLVED
**Answer:** No additional velocity functionality needed. Just CI/CF presentation.

### 6. ~~Coordinate Conversion Methods~~ ✓ RESOLVED
**Answer:** Found in `ground_coordinate.h`:
- `CartesianFixed::convert_to_ci(Time)` - convert CF to CI position
- `convert_position_and_velocity(Time, CartesianFixed, vel_cf, CartesianInertial&, vel_ci&)` - convert position and velocity
- `convert_position_and_velocity(Time, CartesianInertial, vel_ci, CartesianFixed&, vel_cf&, ci_to_cf_q&)` - convert with quaternion
- Quaternion conversions: `ci_to_cf_quat(Time)` member function on CartesianFixed/CartesianInertial

### 7. ~~Quaternion Multiplication~~ ✓ RESOLVED
**Answer:** Use direct quaternion multiplication in C++ (preferred). The matrix approach in DprepOrbit is only because quaternion multiplication wasn't wrapped in SWIG Python bindings.

### 8. Yaw/Pitch/Roll Mapping ✓ RESOLVED
**Answer:** Provide both generic accessors (`euler_angle1/2/3`) AND physical accessors (`yaw/pitch/roll`). The physical accessors map to the correct index based on the euler_order string:
- `yaw()` returns euler_angle at index of 'Z'/'z'/'3' in euler_order
- `pitch()` returns euler_angle at index of 'Y'/'y'/'2' in euler_order
- `roll()` returns euler_angle at index of 'X'/'x'/'1' in euler_order

### 9. Abstract vs Concrete Classes ✓ RESOLVED
**Answer:** 
- Class implementation uses abstract base classes: `CartesianFixed`, `CartesianInertial`
- Unit tests use concrete implementations: `Ecr`, `Eci`

## Implementation Order

1. Create header file with class declaration
2. Create implementation file with constructors and conversion logic
3. Create SWIG interface file
4. Update build system (implementation.am)
5. Create unit test with comprehensive coverage
6. Build and verify all tests pass
7. Stage files for git commit (no commit)

## Testing Strategy

The unit tests will verify:
1. **Basic construction** with explicit Euler angle values (both CF and CI)
2. **Conversion from QuaternionOrbitData** (both CF and CI)
3. **Round-trip conversion** (Euler → Quaternion → Euler)
4. **Edge cases** (zero angles, large angles, 90° rotations, gimbal lock)
5. **Different Euler orders** ("ZYX", "XYZ", "321", etc.)
6. **Local orbital frame calculation** (verify LVLH frame is correct)
7. **Look vector transformations** work correctly
8. **Serialization** preserves all state including Euler order
9. **Print output** is formatted correctly

Each test will check that:
- Euler angles are stored and retrieved correctly in degrees
- Position and velocity are preserved
- Quaternion representation is consistent with Euler angles and orbital frame
- Orbital frame (LVLH) is calculated correctly from position/velocity
- Inheritance from QuaternionOrbitData works properly
- The `prefer_cf()` flag matches the constructor used

## Key Implementation Details

### Local Orbital Frame (LVLH)
Following DprepOrbit convention:
- **z-axis**: Nadir (toward Earth center) = `-position / ||position||`
- **x-axis**: Along-track (velocity direction, orthogonal to nadir)
- **y-axis**: Cross-track = `cross(z, x)`

### Transformation Chain
```
Body Frame → (Euler angles) → Orbital Frame → (LVLH) → ECI/ECR
```

Where:
- `sc_to_orbital = quat_rot(euler_order, e1, e2, e3)`
- `orbital_to_eci` = quaternion from LVLH frame axes
- `sc_to_eci = orbital_to_eci * sc_to_orbital`

### Conversion Direction
When constructing from QuaternionOrbitData:
1. Extract `sc_to_eci` quaternion
2. Calculate `orbital_to_eci` from position/velocity
3. Extract `sc_to_orbital = conj(orbital_to_eci) * sc_to_eci`
4. Convert `sc_to_orbital` to Euler angles using `quat_to_euler`

When constructing from Euler angles:
1. Create `sc_to_orbital` from Euler angles using `quat_rot`
2. Calculate `orbital_to_eci` from position/velocity
3. Combine: `sc_to_eci = orbital_to_eci * sc_to_orbital`
4. Initialize parent QuaternionOrbitData with `sc_to_eci`
