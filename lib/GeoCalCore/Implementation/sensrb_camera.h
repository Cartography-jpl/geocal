#ifndef SENSRB_CAMERA_H
#define SENSRB_CAMERA_H
#include "quaternion_camera.h"

namespace GeoCal {
/****************************************************************//**
  This is a QuaternionCamera where the nonlinearity of the camera is
  described by a radial model 
  
  r^2 = x^2 + y^2
  dr/r = k1 * r^2 + k2 * r^4 + k3 * r^3
  x_corrected = x + (dr/r)*x + p1 * (r^2 + 2 x^2) + 2 p2 * x * y + b1 * x + b2 * y
  y_corrected = x + (dr/r)*x + p2 *(r^2 + 2 y^2) + 2 p1 * x * y 

  Note this is similar to but not identical to the Brown-Conrady
  distortion model, see
  https://en.wikipedia.org/wiki/Distortion_(optics). 

  Note, I think we need to have the various constants in mm rather
  than pixel - I think this is required by MSP library and normally we
  do this in x,y space - not line/sample space.

  This is the camera model used by the SENSRB TRE in NITF files.

  The convention used by SENSRB is that line is in the +y direction,
  sample is the -x direction (so different than the default for
  QuaternionCamera class). This is the "Camera" coordinates, there is
  another (and different) coordinate system called "Sensor". See
  "SENSRB Profile Frame Image-To-Ground Transformation Description" by 
  Michael J. Lenihan.

  The coordinate system for the angles is "SENSOR_ANGLE_MODEL = 1"
  which actually has a bit of a odd orientation. It appears to be
  something like the pilot holding the camera in front of his
  face. This means that all angles 0 actually doesn't point the camera
  towards the ground (most of our models have had nadir pointing
  camera have an identity quaternion). 

  There are other SENSOR_ANGLE_MODEL (2 and 3), but the MSP library
  doesn't support these. We could add support for these in the
  future if useful (e.g., change quaternion_to_sensor_angle to take
  the model number).

  The actual camera model just as a normal frame_to_sc() like other
  QuaternionCamera. We handle mapping this too and from the sensor
  angles used NITF through the various static functions (e.g.,
  quaternion_to_sensor_angle). 
*******************************************************************/
class SensrbCamera : public QuaternionCamera {
public:
//-----------------------------------------------------------------------
/// Create a QuaternionCamera with a radial distortion model given
/// by kdistort. We currently limit kdistort.rows() <= 3, although we
/// could relax that if it ends up being useful.
//-----------------------------------------------------------------------
  SensrbCamera(const boost::math::quaternion<double>& Frame_to_sc_q,
	       double K1, double K2, double K3, double P1, double P2,
	       double B1, double B2, double Radial_distort_limit,
	       int Number_line, int Number_sample,
	       double Line_pitch, double Sample_pitch,
	       double Focal_length, 
	       const FrameCoordinate& Principal_point,
	       const std::string& Detection_type = "VIS",
	       const std::string& Calibration_date = "20000101",
	       QuaternionCamera::FrameConvention Frame_convention =
	       QuaternionCamera::LINE_IS_Y,
	       QuaternionCamera::FrameDirection Line_direction =
	       QuaternionCamera::INCREASE_IS_POSITIVE,
	       QuaternionCamera::FrameDirection Sample_direction =
	       QuaternionCamera::INCREASE_IS_NEGATIVE)
  : QuaternionCamera(Frame_to_sc_q,
		     Number_line, Number_sample, Line_pitch,
		     Sample_pitch, Focal_length, Principal_point,
		     Frame_convention, Line_direction, Sample_direction),
    calibration_date_(Calibration_date),
    detection_type_(Detection_type),
    p_distort_(7),
    radial_distort_limit_(Radial_distort_limit),
    max_r2_filled_in(false)
  {
    p_distort_ = K1, K2, K3, P1, P2, B1, B2;
  }  
  virtual void print(std::ostream& Os) const;
  void dcs_to_focal_plane
  (int Band, const boost::math::quaternion<double>& Dcs,
   double& Xfp, double& Yfp) const;
  void dcs_to_focal_plane
  (int Band, const boost::math::quaternion<AutoDerivative<double> >& Dcs,
   AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const;
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const;

//-----------------------------------------------------------------------
/// Parameters describing nonlinearity
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& p_distort() const {return p_distort_;}

//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double k1() const { return p_distort_(0);}
  void k1(double V) { p_distort_(0) = V; }

//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double k2() const { return p_distort_(1);}
  void k2(double V) { p_distort_(1) = V; }
  
//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double k3() const { return p_distort_(2);}
  void k3(double V) { p_distort_(2) = V; }

//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double p1() const { return p_distort_(3);}
  void p1(double V) { p_distort_(3) = V; }

//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double p2() const { return p_distort_(4);}
  void p2(double V) { p_distort_(4) = V; }

//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double b1() const { return p_distort_(5);}
  void b1(double V) { p_distort_(5) = V; }

//-----------------------------------------------------------------------
/// Parameter describing nonlinearity.
//-----------------------------------------------------------------------

  double b2() const { return p_distort_(6);}
  void b2(double V) { p_distort_(6) = V; }

//-----------------------------------------------------------------------
/// Limit that radial distortion is valid for.
//-----------------------------------------------------------------------

  double radial_distort_limit() const { return radial_distort_limit_;}
  void radial_distort_limit(double V) { radial_distort_limit_ = V; }

//-----------------------------------------------------------------------
/// Calibration date. This is metadata used in the SENSRB TRE. This
/// should be in "YYYYMMDD" format, e.g. "20000102" for January 2, 2000.
//-----------------------------------------------------------------------
  const std::string& calibration_date() const { return calibration_date_; }
  void calibration_date(const std::string& V) { calibration_date_ = V; }

//-----------------------------------------------------------------------
/// Detection type. Specifies the detection spectrum. This is metadata
/// used in the SENSRB TRE.
//-----------------------------------------------------------------------
  const std::string& detection_type() const { return detection_type_; }
  void detection_type(const std::string& V) { detection_type_ = V; }

  static void quaternion_to_sensor_angle
  (const boost::math::quaternion<double>& Frame_to_sc_or_ned,
   double& Sensor_angle_1, double& Sensor_angle_2, double& Sensor_angle_3);
  static boost::math::quaternion<double> sensor_angle_to_quaternion
  (double Sensor_angle_1, double Sensor_angle_2, double Sensor_angle_3);
private:
  std::string calibration_date_, detection_type_;
  blitz::Array<double, 1> p_distort_;
  double radial_distort_limit_;
  mutable bool max_r2_filled_in;
  mutable double max_rp2;
  mutable double max_r2;
  void fill_in_max() const;
  SensrbCamera() : p_distort_(7), max_r2_filled_in(false) {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SensrbCamera);
#endif
