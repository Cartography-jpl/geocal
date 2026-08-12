#ifndef YPR_ORBIT_DATA_H
#define YPR_ORBIT_DATA_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This class stores orbit data with attitude represented as
  Euler angles (Yaw, Pitch, Roll) in degrees relative to a local
  orbital frame.

  The local orbital frame follows the LVLH (Local Vertical Local
  Horizontal) convention:
  - z-axis: Points toward Earth center (nadir)
  - x-axis: In velocity direction, orthogonalized to z
  - y-axis: Completes right-handed system (cross-track)

  The Euler angle order (e.g., "ZYX", "XYZ", "321") is configurable
  with "ZYX" as the default. The order specifies the sequence of
  rotations applied to transform from the orbital frame to the
  spacecraft body frame.

  This class can be constructed from a QuaternionOrbitData,
  extracting the Euler angles from the quaternion. It provides
  both generic accessors (euler_angle1/2/3) and physical accessors
  (yaw/pitch/roll) that map to the correct rotation regardless of
  the Euler order.

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
  /// Constructor with yaw, pitch, roll and CartesianFixed position
  ///
  /// @param Tm - Time
  /// @param pos_cf - Position in CartesianFixed coordinates
  /// @param vel_fixed - Velocity in CartesianFixed coordinates
  /// @param Yaw - Yaw angle in degrees (rotation about Z-axis)
  /// @param Pitch - Pitch angle in degrees (rotation about Y-axis)
  /// @param Roll - Roll angle in degrees (rotation about X-axis)
  /// @param Euler_order - String specifying rotation order (default "ZYX")
  //-----------------------------------------------------------------------
  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianFixed>& pos_cf,
               const boost::array<double, 3>& vel_fixed,
               double Yaw, double Pitch, double Roll,
               const std::string& Euler_order = "ZYX");

  //-----------------------------------------------------------------------
  /// Constructor with yaw, pitch, roll and CartesianInertial position
  ///
  /// @param Tm - Time
  /// @param pos_ci - Position in CartesianInertial coordinates
  /// @param vel_inertial - Velocity in CartesianInertial coordinates
  /// @param Yaw - Yaw angle in degrees (rotation about Z-axis)
  /// @param Pitch - Pitch angle in degrees (rotation about Y-axis)
  /// @param Roll - Roll angle in degrees (rotation about X-axis)
  /// @param Euler_order - String specifying rotation order (default "ZYX")
  //-----------------------------------------------------------------------
  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianInertial>& pos_ci,
               const boost::array<double, 3>& vel_inertial,
               double Yaw, double Pitch, double Roll,
               const std::string& Euler_order = "ZYX");

  //-----------------------------------------------------------------------
  /// Destructor
  //-----------------------------------------------------------------------
  virtual ~YprOrbitData() {}

  virtual void print(std::ostream& Os) const;

  //-----------------------------------------------------------------------
  /// First Euler angle in degrees (depends on euler_order)
  /// For "ZYX": yaw, for "XYZ": roll, etc.
  //-----------------------------------------------------------------------
  double euler_angle1() const;

  //-----------------------------------------------------------------------
  /// Second Euler angle in degrees (depends on euler_order)
  /// For "ZYX": pitch, for "XYZ": pitch, etc.
  //-----------------------------------------------------------------------
  double euler_angle2() const;

  //-----------------------------------------------------------------------
  /// Third Euler angle in degrees (depends on euler_order)
  /// For "ZYX": roll, for "XYZ": yaw, etc.
  //-----------------------------------------------------------------------
  double euler_angle3() const;

  //-----------------------------------------------------------------------
  /// Yaw angle in degrees (rotation about Z-axis)
  //-----------------------------------------------------------------------
  double yaw() const { return yaw_; }

  //-----------------------------------------------------------------------
  /// Pitch angle in degrees (rotation about Y-axis)
  //-----------------------------------------------------------------------
  double pitch() const { return pitch_; }

  //-----------------------------------------------------------------------
  /// Roll angle in degrees (rotation about X-axis)
  //-----------------------------------------------------------------------
  double roll() const { return roll_; }

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

  void initialize_from_ypr(const Time& Tm,
                           const boost::shared_ptr<CartesianFixed>& pos_cf,
                           const boost::array<double, 3>& vel_fixed,
                           double Yaw, double Pitch, double Roll,
                           const std::string& Euler_order);

  void initialize_from_ypr(const Time& Tm,
                           const boost::shared_ptr<CartesianInertial>& pos_ci,
                           const boost::array<double, 3>& vel_inertial,
                           double Yaw, double Pitch, double Roll,
                           const std::string& Euler_order);

  static boost::math::quaternion<double>
  calc_orbital_to_ci(const boost::array<double, 3>& pos_ci,
                     const boost::array<double, 3>& vel_ci);

private:
  double yaw_;    // Rotation about Z-axis in degrees
  double pitch_;  // Rotation about Y-axis in degrees
  double roll_;   // Rotation about X-axis in degrees
  std::string euler_order_;  // e.g., "ZYX", "XYZ", "321"

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(YprOrbitData);
#endif
