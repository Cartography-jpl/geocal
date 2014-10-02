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
/// Granule ID. This is metadata, found in the configuration file.
//-----------------------------------------------------------------------

  const std::string& granule_id() const {return granule_id_;}

//-----------------------------------------------------------------------
/// Epsilon angle, in radians.
//-----------------------------------------------------------------------

  double epsilon() const {return epsilon_;}

//-----------------------------------------------------------------------
/// Psi angle, in radians.
//-----------------------------------------------------------------------

  double psi() const {return psi_;}

//-----------------------------------------------------------------------
/// Indicate if the camera has "inversion" indicated in the
/// configuration.
//-----------------------------------------------------------------------

  bool inversion() const { return inversion_ == -1; }

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

//-----------------------------------------------------------------------
/// Return the camera row number for the given band. This ends up
/// being used in other places (e.g., the "Row Table" in L1B1 files),
/// so we make this available. 
//-----------------------------------------------------------------------
  int row_number(int Band) const {
    range_check(Band, 0, number_band());
    return row_number_[Band];
  }
  double angular_separation(int Reference_band, int Target_band) const;
  void paraxial_offset(int Band,
		       const FrameCoordinate& F,
		       double& Line_offset,
		       double& Sample_offset) const;
  int band_number(int Row_number) const;
  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual void print(std::ostream& Os) const;
protected:
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<double>& Dcs,
				  double& Xfp, double& Yfp) const;
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<AutoDerivative<double> >& Dcs,
				  AutoDerivative<double>& Xfp, 
				  AutoDerivative<double>& Yfp) const
  { throw Exception("No Implemented yet"); }
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const
  {
    throw Exception("No Implemented yet");
  }
private:
  std::string fname, granule_id_;
  // Camera angles, in radians
  double epsilon_, psi_, theta_, boresight_angle_, yaw_, pitch_, roll_;
  // Give the row number for each band.
  std::vector<int> row_number_;
  // Transformation to and from the paraxial coordinates
  boost::shared_ptr<MspiParaxialTransform> paraxial_transform_;
  int inversion_;
};
}
#endif
