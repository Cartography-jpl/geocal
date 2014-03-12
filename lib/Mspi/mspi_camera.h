#ifndef MSPI_CAMERA_H
#define MSPI_CAMERA_H
#include "camera.h"
#include <boost/math/quaternion.hpp>

namespace GeoCal {
/****************************************************************//**
  This is the MSPI camera model.

  References:
  [1] Veljko Jovanovic, "Algorithm Theoretical Basis and
  processing software design consideration for GroundMSPI L1B2
  process", Rev A.1, April 4, 2012.
*******************************************************************/

class MspiCamera: public Camera {
public:
  MspiCamera(const std::string& File_name);
  virtual ~MspiCamera() {}
//-----------------------------------------------------------------------
/// File name for MspiConfigFile.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}

//-----------------------------------------------------------------------
/// Number of bands in camera. 
//-----------------------------------------------------------------------

  virtual int number_band() const { return 1; }

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const { return 1; }

//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const { return 1; }

  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual void print(std::ostream& Os) const;
  // This should go away
  boost::math::quaternion<double> detector_look(const ScLookVector& Sl) const
  { return conj(det_to_station) * Sl.look_quaternion(); }
//private:
  std::string fname;
  // Camera angles, in radians
  double camera_yaw, camera_pitch, camera_roll, boresight_angle;
  // Go from detector to camera coordinates
  boost::math::quaternion<double> det_to_cam;
  // Go from detector to station coordinates
  boost::math::quaternion<double> det_to_station;
};
}
#endif
