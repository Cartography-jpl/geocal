#include "mspi_camera.h"
#include "mspi_config_file.h"
#include "geocal_quaternion.h"
#include "constant.h"
#include <boost/lexical_cast.hpp>

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Read in the given configuration file.
//-----------------------------------------------------------------------

void MspiCamera::read_config_file(const std::string& File_name)
{
  fname = File_name;
  MspiConfigFile c(File_name);

  //focal_length_; Done
  //nband_; Done
  //nline_; Done
  //nsamp_; Done
  //line_pitch_; Done
  //sample_pitch_; Done
  //principal_point_;
  frame_convention_ = QuaternionCamera::LINE_IS_Y;
  //line_direction_; Done
  //sample_direction_; Done

  //-------------------------------------------------------
  // Some hardcoded values that we don't expect to change.
  //-------------------------------------------------------

  sample_pitch_ = 0.010; // center-to-center sample spacing (millimeters)
  line_pitch_ = 0.010;  // pixel size in along row axis (millimeters)
  dy_ = 0.016;      // center-to-center row spacing (millimeters)
  nrow = 64;	    // Number of rows in CCD

  //-------------------------------------------------------
  // Value we read from the configuration file.
  //-------------------------------------------------------

  boresight_angle_ = c.value<double>("boresight_angle") * Constant::deg_to_rad;
  yaw_ = c.value<double>("camera_yaw") * Constant::deg_to_rad;
  pitch_ = c.value<double>("camera_pitch") * Constant::deg_to_rad;
  roll_ = c.value<double>("camera_roll") * Constant::deg_to_rad;
  focal_length_ = c.value<double>("focal_length");
  s_origin_ = c.value<double>("origin");
  nline_ = 1;
  nsamp_ = c.value<int>("number_sample");
  nband_ = c.value<int>("number_band");
  line_direction_ = (c.value<int>("direction") == 1 ?
		       QuaternionCamera::INCREASE_IS_NEGATIVE :
		       QuaternionCamera::INCREASE_IS_POSITIVE);
  sample_direction_ = (c.value<int>("pixel_order") == 1 ? 
		       QuaternionCamera::INCREASE_IS_POSITIVE :
		       QuaternionCamera::INCREASE_IS_NEGATIVE);

  // This doesn't get used for anything yet, so don't bother reading this.
  // inversion_ = c.value<int>("inversion")),

//--------------------------------------------------------------------------
/// Get mapping from band to row number
//--------------------------------------------------------------------------

  for(int b = 0; b < number_band(); ++b)
    row_number.push_back(c.value<int>
			 ("band" + boost::lexical_cast<std::string>(b)));
  
//--------------------------------------------------------------------------
// Set up transformation to and from paraxial coordinates.
//--------------------------------------------------------------------------

  paraxial_transform_.reset(new MspiParaxialTransform(File_name));

//--------------------------------------------------------------------------
// Small-angle rotations between camera and detector coordinate systems. 
// Nominally these are zero.
//--------------------------------------------------------------------------

  epsilon_ = 0;
  psi_ = 0;
  theta_ = 0;

//--------------------------------------------------------------------------
// Setup rotation matrix. This is a side effect of setting the parameters.
//--------------------------------------------------------------------------

  parameter(parameter());

  // Do anything with suggested step size from old code?
}

// See base class for description
blitz::Array<double, 1> MspiCamera::parameter() const
{
  Array<double, 1> res(3);
  res = yaw_, pitch_, roll_;
  return res;
}
// See base class for description
void MspiCamera::parameter(const blitz::Array<double, 1>& Parm)
{
  if(Parm.rows() != 3)
    throw Exception("Parameter must have a size of exactly 3");
  yaw_ = Parm(0);
  pitch_ = Parm(1);
  roll_ = Parm(2);
  // Confirmed that old code had these angles negative. We need to
  // verify that this is actually what is intended, but it does match
  // the old code.
  frame_to_sc_ = quat_rot("ZYXYXYZ", -yaw(), -pitch(), 
			  -roll(), -boresight_angle(), theta(), psi(), 
			  epsilon());
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
  range_check(Band, 0, number_band());
  
//--------------------------------------------------------
// Go to detector coordinate space.
//--------------------------------------------------------

  boost::math::quaternion<double> dcs = 
    conj(frame_to_sc_) * Sl.look_quaternion() * frame_to_sc_ / Sl.length();

//---------------------------------------------------------
// Then to paraxial focal plane. Units are millimeters.
//---------------------------------------------------------

  double x = dcs.R_component_2();
  double y = dcs.R_component_3();
  double z = dcs.R_component_4();
  double yf = (focal_length() / z) * (-x);
  double xf = (focal_length() / z) * y;

//-------------------------------------------------------------------------
// Transform paraxial focal plane coordinate to real focal plane coordinate.
// Units are millimeters.
//-------------------------------------------------------------------------
  
  double xf_prime, yf_prime;
  paraxial_transform_->paraxial_to_real(row_number[Band], xf, yf, 
					xf_prime, yf_prime);
  
//-------------------------------------------------------------------------
/// Then translate to frame coordinates.
//-------------------------------------------------------------------------
  
  FrameCoordinate res;
  res.sample = s_origin_ + xf_prime / sample_pitch_ * samp_dir();
  res.line = line_dir() * (-row_origin(Band) / line_pitch_) + 
    line_dir() * yf_prime / line_pitch_;
  return res;
}

// See base class for description
ScLookVector MspiCamera::sc_look_vector
(const FrameCoordinate& F, int Band) const
{
  range_check(Band, 0, number_band());

//-------------------------------------------------------------------------
/// Convert to real focal plane coordinate (in millimeters)
//-------------------------------------------------------------------------

  double xf_prime = (F.sample - s_origin_) * sample_pitch() * samp_dir();
  double yf_prime = (F.line - line_dir() * (-row_origin(Band) / line_pitch_)) *
    line_pitch() * line_dir();

//-------------------------------------------------------------------------
/// Then to paraxial coordinates.
//-------------------------------------------------------------------------

  double xf, yf;
  paraxial_transform_->real_to_paraxial(row_number[Band], xf_prime, yf_prime,
					xf, yf);

//-------------------------------------------------------------------------
/// Then to spacecraft coordinates.
//-------------------------------------------------------------------------

  boost::math::quaternion<double> dcs(0, -yf, xf, focal_length_);
  dcs /= sqrt(yf * yf + xf * xf + focal_length_ * focal_length_);
  return ScLookVector(frame_to_sc_ * dcs * conj(frame_to_sc_));
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void MspiCamera::print(std::ostream& Os) const
{
  Os << "MspiCamera:\n"
     << "  File name: " << file_name() << "\n";
}

