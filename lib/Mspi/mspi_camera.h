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

  MspiCamera(const std::string& File_name, 
	     const std::string& Extra_config_file = "")
  { read_config_file(File_name, Extra_config_file); }

//-----------------------------------------------------------------------
/// Constructor, which creates a MspiCamera from the given
/// configuration file and then updates the parameters to the given value.
//-----------------------------------------------------------------------

  MspiCamera(const std::string& File_name, const blitz::Array<double, 1>& Parm)
  { read_config_file(File_name); parameter(Parm);}

  virtual ~MspiCamera() {}

  void read_config_file(const std::string& File_name,
			const std::string& Extra_config_file = "");

//-----------------------------------------------------------------------
/// File name for MspiConfigFile.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}
  void file_name(const std::string& Fname)
  {fname = Fname; paraxial_transform_->file_name(Fname); }

//-----------------------------------------------------------------------
/// Paraxial transform
//-----------------------------------------------------------------------

  boost::shared_ptr<MspiParaxialTransform> paraxial_transform() const
  { return paraxial_transform_; }
  
//-----------------------------------------------------------------------
/// Granule ID. This is metadata, found in the configuration file.
//-----------------------------------------------------------------------

  const std::string& granule_id() const {return granule_id_;}
  void granule_id(const std::string& Gid) {granule_id_ = Gid;}

//-----------------------------------------------------------------------
/// Epsilon angle, in degrees.
//-----------------------------------------------------------------------

  double epsilon() const {return epsilon_;}

//-----------------------------------------------------------------------
/// Psi angle, in degrees.
//-----------------------------------------------------------------------

  double psi() const {return psi_;}

//-----------------------------------------------------------------------
/// Indicate if the camera has "inversion" indicated in the
/// configuration.
//-----------------------------------------------------------------------

  bool inversion() const { return inversion_ == -1; }

//-----------------------------------------------------------------------
/// Theta angle, in degrees.
//-----------------------------------------------------------------------

  double theta() const {return theta_;}

//-----------------------------------------------------------------------
/// Boresight angle, in degrees.
//-----------------------------------------------------------------------

  double boresight_angle() const {return boresight_angle_;}

//-----------------------------------------------------------------------
/// Yaw angle, in degrees.
//-----------------------------------------------------------------------

  double yaw() const {return yaw_.value();}
  AutoDerivative<double> yaw_with_derivative() const { return yaw_; }

//-----------------------------------------------------------------------
/// Pitch angle, in degrees.
//-----------------------------------------------------------------------

  double pitch() const {return pitch_.value();}
  AutoDerivative<double> pitch_with_derivative() const { return pitch_; }

//-----------------------------------------------------------------------
/// Roll angle, in degrees.
//-----------------------------------------------------------------------

  double roll() const {return roll_.value();}
  AutoDerivative<double> roll_with_derivative() const { return roll_; }

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
  virtual blitz::Array<double, 1> parameter() const
  { return parameter_with_derivative().value(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  {  parameter_with_derivative(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<bool, 1> parameter_mask() const 
  { return parameter_mask_; }
  virtual void parameter_mask(const blitz::Array<bool, 1>& Pm);
  virtual void print(std::ostream& Os) const;
  virtual void notify_update()
  {
    notify_update_do(*this);
  }
protected:
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<double>& Dcs,
				  double& Xfp, double& Yfp) const;
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<AutoDerivative<double> >& Dcs,
				  AutoDerivative<double>& Xfp, 
				  AutoDerivative<double>& Yfp) const;
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const;
private:
  std::string fname, granule_id_;
  // Camera angles, in degrees
  double epsilon_, psi_, theta_, boresight_angle_;
  AutoDerivative<double> yaw_, pitch_, roll_;
  // Give the row number for each band.
  std::vector<int> row_number_;
  // Transformation to and from the paraxial coordinates
  boost::shared_ptr<MspiParaxialTransform> paraxial_transform_;
  int inversion_;
  blitz::Array<bool, 1> parameter_mask_;
				// Mask of parameters we are fitting for.

  MspiCamera() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(MspiCamera);
#endif
