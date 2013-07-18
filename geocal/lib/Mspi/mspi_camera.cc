/*===========================================================================
=                                                                           =
=                              MspiCamera                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MSPI
                             GeoCal Processing

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: MspiCamera

!ABSTRACT:

   This contains support routines for the class MspiCamera 

!PARENT(S):

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "mspi_camera.h"	// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "geocal_exception.h" // Definition of Exception
#include <cmath> 		// Definition of std::cos
#include <iomanip>  		// Definition of std::setw
#include <fstream> 		// Definition of std::ofstream

using namespace GeoCal;

/////////////////////////////////////////////////////////////////////////////
/// Convert NEWMAT::ColumnVector to std::vector<double>
/////////////////////////////////////////////////////////////////////////////

inline std::vector<double> std_vector(const blitz::Array<double, 1>& 
				      Column_vector)
{
  return std::vector<double>(Column_vector.begin(), Column_vector.end());
}


/////////////////////////////////////////////////////////////////////////////
/// Convert std::vector<double> to NEWMAT::ColumnVector
/////////////////////////////////////////////////////////////////////////////

blitz::Array<double, 1> column_vector(const std::vector<double>& Vector)
{
  blitz::Array<double, 1> res((int) Vector.size());
  for (int i = 0 ; i < res.rows() ; i++)
    res(i) = Vector[i];
  return res;
}

/////////////////////////////////////////////////////////////////////////////
/// Initialize camera model using parameter in the given configuration file.
/////////////////////////////////////////////////////////////////////////////

// PushBroomCamera(,
// 		    ,
// 		    Config.get<int>("number_parameter")),

MspiCamera::MspiCamera
(
 const MSPI::Shared::ConfigFile& Config
 ) 
  : nband(Config.get<int>("number_band")),
    nsamp(Config.get<int>("number_sample")),
    filename(Config.filename()),
  pi_(std::acos(-1)),
    deg_to_rad_(pi_ / 180.0),
    focal_length_(Config.get<double>("focal_length")), /// focal length (millimeters)
    dx_(0.010),      ///< center-to-center sample spacing (millimeters)
    ypitch_(0.010),  ///< pixel size in along row axis (millimeters)
    dy_(0.016),      ///< center-to-center row spacing (millimeters)
    s_origin_(Config.get<double>("origin")),
    number_row_(64),
    direction_(Config.get<int>("direction")),
    pixel_order_(Config.get<int>("pixel_order")),
    boresight_angle_(Config.get<double>("boresight_angle") * deg_to_rad_), 
    camera_yaw_(Config.get<double>("camera_yaw") * deg_to_rad_),
    camera_pitch_(Config.get<double>("camera_pitch") * deg_to_rad_),
    camera_roll_(Config.get<double>("camera_roll") * deg_to_rad_),
    inversion_(Config.get<int>("inversion")),
    t_dc_(3,3),
    t_cd_(3,3),
    t_sc_(3,3),
    t_cs_(3,3),
    row_number_(Config.get<int>("number_band")),
    paraxial_transform_(Config),
    granule_id_(Config.get<std::string>("granule_id")),
    parameter_step_size_(Config.get<int>("number_parameter"))
{

  //--------------------------------------------------------------------------
  // Small-angle rotations between camera and detector coordinate systems. 
  // Nominally these are zero.
  //--------------------------------------------------------------------------

  double epsilon = 0;
  double psi = 0;
  double theta = 0;
  
  //--------------------------------------------------------------------------
  // Construct matrix for transforming camera coordinate system to detector 
  // coordinate system.
  //--------------------------------------------------------------------------

  blitz::firstIndex i1; blitz::secondIndex i2; blitz::thirdIndex i3;
  blitz::fourthIndex i4;
  {

    blitz::Array<double, 2> t_epsilon(3,3);
    t_epsilon = 0;
    t_epsilon(0,0) = std::cos(epsilon);
    t_epsilon(0,1) = std::sin(epsilon);
    t_epsilon(1,0) = -std::sin(epsilon);
    t_epsilon(1,1) = std::cos(epsilon);
    t_epsilon(2,2) = 1;

    blitz::Array<double, 2> t_psi(3,3);
    t_psi = 0;
    t_psi(0,0) = std::cos(psi);
    t_psi(0,2) = -std::sin(psi);
    t_psi(1,1) = 1;
    t_psi(2,0) = std::sin(psi);
    t_psi(2,2) = std::cos(psi);

    blitz::Array<double, 2> t_theta(3,3);
    t_theta = 0;
    t_theta(0,0) = 1;
    t_theta(1,1) = std::cos(theta);
    t_theta(1,2) = std::sin(theta);
    t_theta(2,1) = -std::sin(theta);
    t_theta(2,2) = std::cos(theta);

    blitz::Array<double, 2> t1(3, 3);
    t1 = sum(t_psi(i1, i3) * t_theta(i3, i2), i3);
    t_dc_ = sum(t_epsilon(i1, i3) * t1(i3, i2), i3);
  }

  //--------------------------------------------------------------------------
  // Construct matrix for transforming detector coordinate system to camera
  // coordinate system.
  //--------------------------------------------------------------------------

  t_cd_ = t_dc_(i2, i1);

  //-------------------------------------------------------------------------
  // Construct matrix for transforming camera coordinates to station 
  // coordinates and vice versa.
  //-------------------------------------------------------------------------

  init_camera_to_station_transform();

  //-------------------------------------------------------------------------
  // Initialize row numbers and wavelength indices for each band.
  // Setup reverse map from row number to band number.
  //-------------------------------------------------------------------------

  for (int iband = 0 ; iband < number_band() ; iband++) {
    std::ostringstream band_number_string;
    band_number_string << "band" << iband;

    int row_number = Config.get<int>(band_number_string.str());
    row_number_[iband] = row_number;
    
    band_number_[row_number_[iband]] = iband;
  }

  //-------------------------------------------------------------------------
  // Set suggested step sizes for calculating numerical derivatives.
  //-------------------------------------------------------------------------

  parameter_step_size_[0] = 0.1 * deg_to_rad_;  // yaw
  parameter_step_size_[1] = 0.1 * deg_to_rad_;  // pitch
  parameter_step_size_[2] = 0.1 * deg_to_rad_;  // roll

}

/////////////////////////////////////////////////////////////////////////////
/// Translate from look vector to frame coordinates for the
/// given band.
/////////////////////////////////////////////////////////////////////////////

FrameCoordinate
MspiCamera::frame_coordinate(
  const ScLookVector& Sl, 
  int Band
) const
{
  //-------------------------------------------------------------------------
  // Argument check: Band < 0 
  //                 Band > number_band()-1
  //-------------------------------------------------------------------------

  if (Band < 0) {
    Exception e;
    e << "Band < 0";
    throw e;
  }
  if (Band > number_band() - 1) {
    Exception e;
    e << "Band > number_band() - 1";
    throw e;
  }

  //-------------------------------------------------------------------------
  // Transform look vector to detector coordinate space.
  //-------------------------------------------------------------------------
 
  blitz::Array<double, 1> v_dcs(3);
  detector_look(Sl, v_dcs);

  //-------------------------------------------------------------------------
  // Project detector coordinate vector onto paraxial focal plane.  Units are
  // millimeters.
  //-------------------------------------------------------------------------

  double yf = ( focal_length_ / v_dcs(2) ) * (-v_dcs(0));
  double xf = ( focal_length_ / v_dcs(2) ) * v_dcs(1);

  //-------------------------------------------------------------------------
  // Transform paraxial focal plane coordinate to real focal plane coordinate.
  // Units are millimeters.
  //-------------------------------------------------------------------------

  double xf_prime;
  double yf_prime;
  
  paraxial_transform_.paraxial_to_real(row_number_[Band],xf,yf,xf_prime,yf_prime);

  //-------------------------------------------------------------------------
  // Transform Real Focal Plane Coordinate to Frame Coordinate
  //-------------------------------------------------------------------------

  double sample = (xf_prime / (dx_* pixel_order_) ) + s_origin_;
  double line = direction_ * (row_origin(Band) - yf_prime ) / ypitch_;

  //-------------------------------------------------------------------------
  // Return
  //-------------------------------------------------------------------------
  
  return FrameCoordinate(line, sample);
}

/////////////////////////////////////////////////////////////////////////////
/// Return the Jacobian of the frame coordinates with respect to the look
/// vector. The first row is the derivatives of the line with respect
/// to the look vector X, Y, and Z. The second row is the
/// derivatives of the sample.
/////////////////////////////////////////////////////////////////////////////

blitz::Array<double, 2>
MspiCamera::jacobian_look(
  const ScLookVector& Sl,
  int Band
) const
{
  boost::array<double, 3> d(Sl.direction());
  blitz::Array<double, 2> result(2,3);
  double pstep = 5e-7;
  for (int i = 0; i < 3 ; i++) {
    boost::array<double, 3> d0(d);
    boost::array<double, 3> d1(d);
    
    d0[i] -= pstep;
    d1[i] += pstep;

    ScLookVector sl0(d0);
    ScLookVector sl1(d1);

    FrameCoordinate fc0 = frame_coordinate(sl0, Band);
    FrameCoordinate fc1 = frame_coordinate(sl1, Band);
 
    result(0,i) = (fc1.line - fc0.line) / (pstep * 2.0);
    result(0,i) = (fc1.sample - fc0.sample) / (pstep * 2.0);
  }
  
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the Jacobian of the frame coordinates with respect to the parameters
/// The first row is the derivatives of line with respect to the parameters
/// The second row is the derivatives of the sample.
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<double,2>
MspiCamera::jacobian(
  const ScLookVector& Sl,
  int Band
) const
{
  double deg_to_rad = acos(-1.0) / 180.0;
  double pstep = 0.1 * deg_to_rad;
  
  MspiCamera camera_tmp0(*this);
  MspiCamera camera_tmp1(*this);

  std::vector<double> p = std_vector(parameter());

  boost::multi_array<double,2> result(boost::extents[2][p.size()]);

  for (size_t i = 0; i < p.size() ; i++) {
    std::vector<double> p0 = p;
    std::vector<double> p1 = p;

    p0[i] -= pstep * 0.5;
    p1[i] += pstep * 0.5;

    camera_tmp0.parameter(column_vector(p0));
    camera_tmp1.parameter(column_vector(p1));

    FrameCoordinate fc0 = camera_tmp0.frame_coordinate(Sl, Band);
    FrameCoordinate fc1 = camera_tmp1.frame_coordinate(Sl, Band);

    result[0][i] = (fc1.line - fc0.line) / pstep;
    result[1][i] = (fc1.sample - fc0.sample) / pstep;
  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Set the camera model parameters.  The given column vector includes the
/// following parameters:
///
///  index    parameter
///   0       camera to station yaw in radians
///   1       camera to station pitch in radians
///   2       camera to station roll in radians
/////////////////////////////////////////////////////////////////////////////

void    
MspiCamera::parameter(const blitz::Array<double, 1>& Pr)
{
  camera_yaw_ = Pr(0);
  camera_pitch_ = Pr(1);
  camera_roll_ = Pr(2);
  init_camera_to_station_transform();
}

/////////////////////////////////////////////////////////////////////////////
/// Get camera model parameters.  Returned column vector includes the
/// following:
///
///  index    parameter
///   0       camera to station yaw in radians
///   1       camera to station pitch in radians
///   2       camera to station roll in radians
/////////////////////////////////////////////////////////////////////////////

blitz::Array<double, 1>
MspiCamera::parameter() const
{
  blitz::Array<double, 1> result(3);
  result(0) = camera_yaw_;
  result(1) = camera_pitch_;
  result(2) = camera_roll_;
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Translate frame coordinate to unit vector in detector coordinate
/// system for the given band.
/////////////////////////////////////////////////////////////////////////////

void 
MspiCamera::detector_look(
  const FrameCoordinate& F,
  int Band,
  blitz::Array<double, 1>& Dcs_look
) const
{

  //-------------------------------------------------------------------------
  // Argument check: Band < 0 
  //                 Band > number_band()-1
  //-------------------------------------------------------------------------

  if (Band < 0) {
    Exception e;
    e << "Band < 0";
    throw e;
  }
  if (Band > number_band() - 1) {
    Exception e;
    e << "Band > number_band() - 1";
    throw e;
  }

  //-------------------------------------------------------------------------
  // Transform Frame Coordinate to Real Focal Plane Coordinate
  // Units are millimeters.
  //-------------------------------------------------------------------------

  double xf_prime;
  double yf_prime;
  real_focal_plane_coor(Band, F, xf_prime, yf_prime);
		     
  //-------------------------------------------------------------------------
  // Transform real focal plane coordinate to paraxial focal plane coordinate.
  // Units are millimeters.
  //-------------------------------------------------------------------------

  double xf;
  double yf;
  
  paraxial_transform_.real_to_paraxial(row_number_[Band],xf_prime,yf_prime,xf,yf);

  //-------------------------------------------------------------------------
  // Transform paraxial focal plane coordinate into unit vector in detector 
  // coordinate system.
  //-------------------------------------------------------------------------

  double u = 1.0 / std::sqrt(yf * yf + xf * xf + focal_length_ * focal_length_);
  Dcs_look.resize(3);
  Dcs_look(0) = -yf * u;
  Dcs_look(1) = xf * u;
  Dcs_look(2) = focal_length_ * u;

}

/////////////////////////////////////////////////////////////////////////////
/// Translate station look vector to unit vector in detector coordinate 
/// system.
/////////////////////////////////////////////////////////////////////////////

void 
MspiCamera::detector_look(
  const ScLookVector& Sl,
  blitz::Array<double, 1>& Dcs_look
) const
{
  blitz::firstIndex i1; blitz::secondIndex i2; blitz::thirdIndex i3;
  blitz::fourthIndex i4;
  //-------------------------------------------------------------------------
  // Transform look vector from station coordinates to camera
  // coordinates
  //
  // Transform look vector from camera coordinate space to detector coordinate
  // space.  Result is a unit vector because Sl.direction is a unit vector.
  //-------------------------------------------------------------------------

  blitz::Array<double, 1> dir(3);
  dir(0) = Sl.direction()[0];
  dir(1) = Sl.direction()[1];
  dir(2) = Sl.direction()[2];
  Dcs_look.resize(t_dc_.rows());
  blitz::Array<double, 1> t1(3);
  t1 = sum(t_cs_(i1, i2) * dir(i2), i2);
  Dcs_look = sum(t_dc_(i1, i2) * t1(i2), i2);
}

/////////////////////////////////////////////////////////////////////////////
/// Calculate camera to station transform.
/////////////////////////////////////////////////////////////////////////////

void 
MspiCamera::init_camera_to_station_transform()
{

  blitz::firstIndex i1; blitz::secondIndex i2; blitz::thirdIndex i3;
  blitz::fourthIndex i4;

  //-------------------------------------------------------------------------
  // Construct matrix for transforming camera coordinates to station 
  // coordinates.
  //-------------------------------------------------------------------------

  blitz::Array<double, 2> t_yaw(3,3);
  t_yaw = 0;
  t_yaw(0,0) = std::cos(camera_yaw_);
  t_yaw(0,1) = std::sin(camera_yaw_);
  t_yaw(1,0) = -std::sin(camera_yaw_);
  t_yaw(1,1) = std::cos(camera_yaw_);
  t_yaw(2,2) = 1;

  blitz::Array<double, 2> t_pitch(3,3);
  t_pitch = 0;
  t_pitch(0,0) = std::cos(camera_pitch_);
  t_pitch(0,2) = -std::sin(camera_pitch_);
  t_pitch(1,1) = 1;
  t_pitch(2,0) = std::sin(camera_pitch_);
  t_pitch(2,2) = std::cos(camera_pitch_);

  blitz::Array<double, 2> t_roll(3,3);
  t_roll = 0;
  t_roll(0,0) = 1;
  t_roll(1,1) = std::cos(camera_roll_);
  t_roll(1,2) = std::sin(camera_roll_);
  t_roll(2,1) = -std::sin(camera_roll_);
  t_roll(2,2) = std::cos(camera_roll_);
  
  blitz::Array<double, 2> t_boresight(3,3);
  t_boresight = 0;
  t_boresight(0,0) = std::cos(boresight_angle_);
  t_boresight(0,2) = -std::sin(boresight_angle_);
  t_boresight(1,1) = 1;
  t_boresight(2,0) = std::sin(boresight_angle_);
  t_boresight(2,2) = std::cos(boresight_angle_);

  blitz::Array<double, 2> t1(3, 3), t2(3, 3);
  t1 = sum(t_roll(i1, i3) * t_boresight(i3, i2), i3);
  t2 = sum(t_pitch(i1, i3) * t1(i3, i2), i3);
  t_sc_ = sum(t_yaw(i1, i3) * t2(i3, i2), i3);

  //-------------------------------------------------------------------------
  // Construct matrix for transforming station coordinates to camera coordinates
  //-------------------------------------------------------------------------

  t_cs_ = t_sc_(i2, i1);
}

/////////////////////////////////////////////////////////////////////////////
/// Write camera parameters to a file.
/////////////////////////////////////////////////////////////////////////////

void 
MspiCamera::write(
  const std::string& Filename  ///< File name.
) const
{
  double rad_to_deg = 180.0 / acos(-1.0);
  std::ofstream out(Filename.c_str());
  out.precision(3);
  out.setf(std::ios::left | std::ios::fixed);
  out << "# AirMSPI camera parameter file\n"
      << std::setw(16)
      << parameter()(1) * rad_to_deg << " : camera_yaw # Camera yaw angle in degrees\n"
      << std::setw(16)
      << parameter()(2) * rad_to_deg << " : camera_pitch # Camera pitch angle in degrees\n"
      << std::setw(16)
      << parameter()(3) * rad_to_deg << " : camera_roll # Camera roll angle in degrees\n";
}

