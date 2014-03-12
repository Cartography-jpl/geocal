#include "mspi_camera.h"
#include "mspi_config_file.h"
#include "geocal_quaternion.h"
#include "constant.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor, which creates a MspiCamera from the given
/// configuration file.
//-----------------------------------------------------------------------

MspiCamera::MspiCamera(const std::string& File_name)
  :fname(File_name)
{
  MspiConfigFile c(File_name);
  boresight_angle = c.value<double>("boresight_angle") * Constant::deg_to_rad;
  camera_yaw = c.value<double>("camera_yaw") * Constant::deg_to_rad;
  camera_pitch = c.value<double>("camera_pitch") * Constant::deg_to_rad;
  camera_roll = c.value<double>("camera_roll") * Constant::deg_to_rad;

//--------------------------------------------------------------------------
// Small-angle rotations between camera and detector coordinate systems. 
// Nominally these are zero.
//--------------------------------------------------------------------------

  double epsilon = 0;
  double psi = 0;
  double theta = 0;
  // Confirmed that old code had these angles negative. We need to
  // verify that this is actually what is intended
  det_to_cam = conj(quat_rot("ZYX", -epsilon, -psi, -theta));

  // Temp - By changing epsilon, psi, theta to nonzero and comparing
  // with old unit test confirmed this matches.

//--------------------------------------------------------------------------
// Setup rotation matrix. This is a side effect of setting the parameters.
//--------------------------------------------------------------------------

  parameter(parameter());
}

// See base class for description
Array<double, 1> MspiCamera::parameter() const
{
  Array<double, 1> res(3);
  res = camera_yaw, camera_pitch, camera_roll;
  return res;
}
// See base class for description
void MspiCamera::parameter(const Array<double, 1>& Parm)
{
  if(Parm.rows() != 3)
    throw Exception("Parameter must have a size of exactly 3");
  camera_yaw = Parm(0);
  camera_pitch = Parm(1);
  camera_roll = Parm(2);
  det_to_station = conj(quat_rot("ZYXY", -camera_yaw, -camera_pitch, 
				 -camera_roll, -boresight_angle)) * det_to_cam;
  std::cerr << quaternion_to_matrix(quat_rot_z(-camera_yaw));
}

// See base class for description
std::vector<std::string> MspiCamera::parameter_name() const
{
  std::vector<std::string> res;
  res.push_back("Yaw (radians)");
  res.push_back("Pitch (radians)");
  res.push_back("Roll (radians)");
  return res;
}

// See base class for description
FrameCoordinate MspiCamera::frame_coordinate
(const ScLookVector& Sl, int Band) const
{
}

// See base class for description
ScLookVector MspiCamera::sc_look_vector
(const FrameCoordinate& F, int Band) const
{
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void MspiCamera::print(std::ostream& Os) const
{
  Os << "MspiCamera:\n"
     << "  File name: " << file_name() << "\n";
}

