#ifndef MSPI_CAMERA_H
#define MSPI_CAMERA_H
#include "quaternion_camera.h"
#include "mspi_paraxial_transform.h"
#include <boost/math/quaternion.hpp>
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This is the MSPI camera model.

  References:
  [1] Veljko Jovanovic, "Algorithm Theoretical Basis and
  processing software design consideration for GroundMSPI L1B2
  process", Rev A.1, April 4, 2012.
*******************************************************************/

class MspiCamera: public QuaternionCamera {
public:
//-----------------------------------------------------------------------
/// Constructor, which creates a MspiCamera from the given
/// configuration file.
//-----------------------------------------------------------------------

  MspiCamera(const std::string& File_name)
  { read_config_file(File_name); }

//-----------------------------------------------------------------------
/// Constructor, which creates a MspiCamera from the given
/// configuration file and then updates the parameters to the given value.
//-----------------------------------------------------------------------

  MspiCamera(const std::string& File_name, const blitz::Array<double, 1>& Parm)
  { read_config_file(File_name); parameter(Parm);}

  virtual ~MspiCamera() {}

  void read_config_file(const std::string& File_name);

//-----------------------------------------------------------------------
/// File name for MspiConfigFile.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}

//-----------------------------------------------------------------------
/// Epsilon angle, in radians.
//-----------------------------------------------------------------------

  double epsilon() const {return epsilon_;}

//-----------------------------------------------------------------------
/// Psi angle, in radians.
//-----------------------------------------------------------------------

  double psi() const {return psi_;}

//-----------------------------------------------------------------------
/// Theta angle, in radians.
//-----------------------------------------------------------------------

  double theta() const {return theta_;}

//-----------------------------------------------------------------------
/// Boresight angle, in radians.
//-----------------------------------------------------------------------

  double boresight_angle() const {return boresight_angle_;}

//-----------------------------------------------------------------------
/// Yaw angle, in radians.
//-----------------------------------------------------------------------

  double yaw() const {return yaw_;}

//-----------------------------------------------------------------------
/// Pitch angle, in radians.
//-----------------------------------------------------------------------

  double pitch() const {return pitch_;}

//-----------------------------------------------------------------------
/// Roll angle, in radians.
//-----------------------------------------------------------------------

  double roll() const {return roll_;}

//-----------------------------------------------------------------------
/// Focal length, in millimeters.
//-----------------------------------------------------------------------

  double focal_length() const {return focal_length_;}

//-----------------------------------------------------------------------
/// Center-to-center sample spacing (millimeters)
//-----------------------------------------------------------------------

  double dx() const {return dx_;}

//-----------------------------------------------------------------------
/// Pixel size in along row axis (millimeters)
//-----------------------------------------------------------------------

  double ypitch() const {return ypitch_;}

//-----------------------------------------------------------------------
/// Center-to-center line spacing
//-----------------------------------------------------------------------

  double dy() const {return dy_;}

//-----------------------------------------------------------------------
/// Sample origin. In principle this could depend on the row/band
/// number, for now this is constant.
//-----------------------------------------------------------------------

  double s_origin() const {return s_origin_;}

//-----------------------------------------------------------------------
/// Line direction. +1 is from horizon to zenith, -1 from zenith down
/// to horizon.
//-----------------------------------------------------------------------

  int line_direction() const {return line_direction_;}

//-----------------------------------------------------------------------
/// Pixel order in sample direction, +1 or -1 depending on
/// orientation.
//-----------------------------------------------------------------------

  int pixel_order() const {return pixel_order_;}

//-----------------------------------------------------------------------
/// This is the integration time in seconds.
///
/// The integration time is how long it takes the camera to collect
/// data. It is important to model this, so that we can include
/// movement of the spacecraft while the camera is in the middle of
/// collecting data. 
//-----------------------------------------------------------------------

  virtual double integration_time(int Band) const 
  { // Right now, this is a hardcoded value
    return 0.0355; 
  }

  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
private:
  void calc_cam_to_det();

//-----------------------------------------------------------------------
/// Return row origin in the real focal plane, for the given band.
/// (MSPI L1B2 ATB equation 1.1)
//-----------------------------------------------------------------------

  double row_origin(int Band) const
  { return -dy_ * (row_number[Band] + 0.5 - (nrow / 2.0)); }

  std::string fname;
  // Camera focal length, in millimeters.
  double focal_length_;
  // Center-to-center sample spacing
  double dx_;
  // Center-to-center line spacing
  double dy_;
  // Pixel size in along row axis (millimeters)
  double ypitch_;
  // Sample origin. In principle this could depend on the row/band
  // number, for now this is constant.
  double s_origin_;
  // Number of samples. In principle this could depend on the row/band
  // number, for now this is constant.
  int nrow;
  // Line direction. +1 is from horizon to zenith, -1 from zenith down
  // to horizon.
  int line_direction_;
  // Pixel order in sample direction, +1 or -1 depending on
  // orientation.
  int pixel_order_;
  // Camera angles, in radians
  double epsilon_, psi_, theta_, boresight_angle_, yaw_, pitch_, roll_;
  // Give the row number for each band.
  std::vector<int> row_number;
  // Go from detector to camera coordinates
  boost::math::quaternion<double> cam_to_det;
  // Go from detector to station coordinates
  boost::math::quaternion<double> station_to_det;
  // Transformation to and from the paraxial coordinates
  boost::shared_ptr<MspiParaxialTransform> paraxial_transform_;
};
}
#endif
