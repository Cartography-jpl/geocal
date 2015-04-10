#include "mspi_camera.h"
#include "mspi_config_file.h"
#include "geocal_quaternion.h"
#include "constant.h"
#include "geocal_serialize_support.h"
#include <boost/lexical_cast.hpp>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void MspiCamera::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionCamera);
  ar & GEOCAL_NVP(fname) 
    & GEOCAL_NVP_(granule_id)
    & GEOCAL_NVP_(epsilon)
    & GEOCAL_NVP_(psi)
    & GEOCAL_NVP_(theta)
    & GEOCAL_NVP_(boresight_angle)
    & GEOCAL_NVP_(yaw)
    & GEOCAL_NVP_(pitch)
    & GEOCAL_NVP_(roll)
    & GEOCAL_NVP_(gimbal_epsilon)
    & GEOCAL_NVP_(gimbal_psi)
    & GEOCAL_NVP_(gimbal_theta)
    & GEOCAL_NVP_(row_number)
    & GEOCAL_NVP_(paraxial_transform)
    & GEOCAL_NVP_(inversion);
}


GEOCAL_IMPLEMENT(MspiCamera);
#endif

//-----------------------------------------------------------------------
/// Read in the given configuration file.
//-----------------------------------------------------------------------

void MspiCamera::read_config_file(const std::string& File_name,
				  const std::string& Extra_config_file)
{
  fname = File_name;
  MspiConfigFile c(File_name);
  if(Extra_config_file != "")
    c.add_file(Extra_config_file);

//-------------------------------------------------------
// Some hardcoded values that we don't expect to change.
//-------------------------------------------------------

  sample_pitch_ = 0.010; // center-to-center sample spacing (millimeters)
  line_pitch_ = 0.010;  // pixel size in along row axis (millimeters)
  double dy = 0.016;    // center-to-center row spacing (millimeters)
  int nrow = 64;	// Number of rows in CCD
  frame_convention_ = QuaternionCamera::LINE_IS_Y;

//-------------------------------------------------------
// Value we read from the configuration file.
//-------------------------------------------------------

  boresight_angle_ = c.value<double>("boresight_angle") * Constant::deg_to_rad;
  yaw_ = c.value<double>("camera_yaw") * Constant::deg_to_rad;
  pitch_ = c.value<double>("camera_pitch") * Constant::deg_to_rad;
  roll_ = c.value<double>("camera_roll") * Constant::deg_to_rad;
  if(c.have_key("gimbal_epsilon")) {
    gimbal_epsilon_ = c.value<double>("gimbal_epsilon");
    gimbal_psi_ = c.value<double>("gimbal_psi");
    gimbal_theta_ = c.value<double>("gimbal_theta");
  } else {
    gimbal_epsilon_ = 0;
    gimbal_psi_ = 0;
    gimbal_theta_ = 0;
  }
  focal_length_ = c.value<double>("focal_length");
  double s_origin = c.value<double>("origin");
  nline_ = 1;
  nsamp_ = c.value<int>("number_sample");
  nband_ = c.value<int>("number_band");
  line_direction_ = (c.value<int>("direction") == 1 ?
		       QuaternionCamera::INCREASE_IS_NEGATIVE :
		       QuaternionCamera::INCREASE_IS_POSITIVE);
  sample_direction_ = (c.value<int>("pixel_order") == 1 ? 
		       QuaternionCamera::INCREASE_IS_NEGATIVE :
		       QuaternionCamera::INCREASE_IS_POSITIVE);
  inversion_ = c.value<int>("inversion");
  granule_id_ = c.value<std::string>("granule_id");
  
//--------------------------------------------------------------------------
/// Get mapping from band to row number
//--------------------------------------------------------------------------

  for(int b = 0; b < number_band(); ++b)
    row_number_.push_back(c.value<int>
			 ("band" + boost::lexical_cast<std::string>(b)));

//--------------------------------------------------------------------------
// Set up principal point for each band. Row origin is given by
// MSPI L1B2 ATB (equation 1.1)
//--------------------------------------------------------------------------

  for(int b = 0; b < number_band(); ++b) {
    double l_origin = line_dir() * dy * 
      (row_number_[b] + 0.5 - (nrow / 2.0)) / line_pitch();
    principal_point_.push_back(FrameCoordinate(l_origin,
					       s_origin));
  }
  
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
  // 
  // The negative values give a passive rotation, vs. active rotation
  // for positive values
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

void MspiCamera::dcs_to_focal_plane(int Band,
				    const boost::math::quaternion<double>& Dcs,
				    double& Xfp, double& Yfp) const
{
//---------------------------------------------------------
// Go to paraxial focal plane. Units are millimeters.
//---------------------------------------------------------

  double yf = (focal_length() / Dcs.R_component_4()) * (-Dcs.R_component_2());
  double xf = (focal_length() / Dcs.R_component_4()) * Dcs.R_component_3();

//-------------------------------------------------------------------------
// Transform paraxial focal plane coordinate to real focal plane coordinate.
// Units are millimeters.
//-------------------------------------------------------------------------
  
  paraxial_transform_->paraxial_to_real(row_number_[Band], xf, yf, 
					Xfp, Yfp);
}

// See base class for description
boost::math::quaternion<double> 
MspiCamera::focal_plane_to_dcs(int Band, double Xfp, double Yfp) const
{
//-------------------------------------------------------------------------
/// Convert to paraxial coordinates.
//-------------------------------------------------------------------------

  double xf, yf;
  paraxial_transform_->real_to_paraxial(row_number_[Band], Xfp, Yfp,
					xf, yf);

//-------------------------------------------------------------------------
/// Then to detector coordinates look vector.
//-------------------------------------------------------------------------

  return boost::math::quaternion<double>(0, -yf, xf, focal_length());
}


//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void MspiCamera::print(std::ostream& Os) const
{
  Os << "MspiCamera:\n"
     << "  File name: " << file_name() << "\n";
}

//-----------------------------------------------------------------------
/// Return the band number for the given row. 
//-----------------------------------------------------------------------

int MspiCamera::band_number(int Row_number) const
{
  std::vector<int>::const_iterator f = 
    std::find(row_number_.begin(), row_number_.end(), Row_number);
  if(f == row_number_.end()) {
    Exception e("Row number not assigned to camera model band: ");
    e << Row_number;
    throw e;
  }
  return (int)(f - row_number_.begin());
}

//-----------------------------------------------------------------------
/// Return angular separation (in radians) between the given reference and
/// target bands in the real focal plane.
/// (MSPI L1B2 ATB equation 14 -- *modified* to use Forigin in place of b0)
//-----------------------------------------------------------------------

double MspiCamera::angular_separation
(int Reference_band, int Target_band) const
{
  return
    (atan(line_pitch() * principal_point(Target_band).line / focal_length()) -
     atan(line_pitch() * principal_point(Reference_band).line / focal_length()))
    * inversion_;
}

//-----------------------------------------------------------------------
/// Return paraxial displacement (in pixel units) for the given frame
/// coordinate and band.  The paraxial transform equations are defined
/// such that the frame line coordinate is always assumed to be zero
/// (i.e. centered in the line for the given band).  Therefore values other
/// than zero for frame line will not produce a different result.
//-----------------------------------------------------------------------

void MspiCamera::paraxial_offset
(int Band,
 const FrameCoordinate& F,
 double& Line_offset,
 double& Sample_offset) const
{
  double xf_prime = 
    (F.sample - principal_point(Band).sample) * sample_pitch() * 
    samp_dir();
  double yf_prime = (F.line - principal_point(Band).line) * line_pitch()
    * line_dir();
  
  double xf, yf;
  paraxial_transform_->real_to_paraxial(row_number_[Band], xf_prime, yf_prime,
					xf, yf);
  Line_offset = inversion_ * (yf - yf_prime) / line_pitch();
  Sample_offset = (xf - xf_prime) / sample_pitch();
}
