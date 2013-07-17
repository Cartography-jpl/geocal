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
#include "GeoCalCore/InterfaceClass/Camera/frame_coor.h"
                                // Definition of FrameCoor
#include "GeoCalCore/Support/Err/range_check.h"
                                // Definition of range_check.
#include "GeoCalCore/Support/Exception/exception.h" // Definition of Exception
#include <cmath> 		// Definition of std::cos
#include <iomanip>  		// Definition of std::setw
#include <fstream> 		// Definition of std::ofstream

/////////////////////////////////////////////////////////////////////////////
/// Convert NEWMAT::ColumnVector to std::vector<double>
/////////////////////////////////////////////////////////////////////////////

namespace {
  std::vector<double> std_vector(const NEWMAT::ColumnVector& Column_vector)
  {
    size_t size = Column_vector.Nrows();
    std::vector<double> result(size);
    for (size_t i = 0 ; i < size ; i++) {
      result[i] = Column_vector(i+1); // ColumnVector is 1-based
    }
    return result;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Convert std::vector<double> to NEWMAT::ColumnVector
/////////////////////////////////////////////////////////////////////////////

namespace {
  NEWMAT::ColumnVector column_vector(const std::vector<double>& Vector)
  {
    size_t size = Vector.size();
    NEWMAT::ColumnVector result(size);
    for (size_t i = 0 ; i < size ; i++) {
      result(i+1) = Vector[i]; // ColumnVector is 1-based
    }
    return result;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Initialize camera model using parameter in the given configuration file.
/////////////////////////////////////////////////////////////////////////////

GeoCal::Mspi::MspiCamera::MspiCamera(
  const MSPI::Shared::ConfigFile& Config
) : GeoCal::GeoCalCore::PushBroomCamera(Config.get<int>("number_band"),
					Config.get<int>("number_sample"),
					Config.get<int>("number_parameter")),
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
    row_number_(number_band()),
    paraxial_transform_(Config),
    granule_id_(Config.get<std::string>("granule_id")),
    parameter_step_size_(parameter_size()),
    covariance_(parameter_size())
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

  {
    NEWMAT::Matrix t_epsilon(3,3);
    t_epsilon = 0;
    t_epsilon(1,1) = std::cos(epsilon);
    t_epsilon(1,2) = std::sin(epsilon);
    t_epsilon(2,1) = -std::sin(epsilon);
    t_epsilon(2,2) = std::cos(epsilon);
    t_epsilon(3,3) = 1;

    NEWMAT::Matrix t_psi(3,3);
    t_psi = 0;
    t_psi(1,1) = std::cos(psi);
    t_psi(1,3) = -std::sin(psi);
    t_psi(2,2) = 1;
    t_psi(3,1) = std::sin(psi);
    t_psi(3,3) = std::cos(psi);

    NEWMAT::Matrix t_theta(3,3);
    t_theta = 0;
    t_theta(1,1) = 1;
    t_theta(2,2) = std::cos(theta);
    t_theta(2,3) = std::sin(theta);
    t_theta(3,2) = -std::sin(theta);
    t_theta(3,3) = std::cos(theta);

    t_dc_ = t_epsilon * t_psi * t_theta;
  }

  //--------------------------------------------------------------------------
  // Construct matrix for transforming detector coordinate system to camera
  // coordinate system.
  //--------------------------------------------------------------------------

  t_cd_ = t_dc_.t();

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
  // Set integration time in base class.
  //-------------------------------------------------------------------------

  for (int iband = 0 ; iband < number_band() ; iband++) {
    integration_time(iband, 0.0355);
  }

  //-------------------------------------------------------------------------
  // Set direction in base class.
  //-------------------------------------------------------------------------

  for (int iband = 0 ; iband < number_band() ; iband++) {
    direction(GeoCal::GeoCalCore::PushBroomCamera::FORWARD);
  }

  //-------------------------------------------------------------------------
  // Set suggested step sizes for calculating numerical derivatives.
  //-------------------------------------------------------------------------

  parameter_step_size_[0] = 0.1 * deg_to_rad_;  // yaw
  parameter_step_size_[1] = 0.1 * deg_to_rad_;  // pitch
  parameter_step_size_[2] = 0.1 * deg_to_rad_;  // roll

  //-------------------------------------------------------------------------
  // Initialize covariance to identity matrix
  //-------------------------------------------------------------------------

  covariance_ = 0;
  for (int i = 0 ; i < parameter_size() ; i++) {
    covariance_(i+1,i+1) = 1;  // NEWMAT::SymmetricMatrix is 1-based
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Destructor.
/////////////////////////////////////////////////////////////////////////////

GeoCal::Mspi::MspiCamera::~MspiCamera()
{
}

/////////////////////////////////////////////////////////////////////////////
/// Translate from look vector to frame coordinates for the
/// given band.
/////////////////////////////////////////////////////////////////////////////

GeoCal::GeoCalCore::FrameCoor 
GeoCal::Mspi::MspiCamera::frame_coor(
  const GeoCal::GeoCalCore::ScLookVector& Sl, 
  int Band
) const
{
  //-------------------------------------------------------------------------
  // Argument check: Band < 0 
  //                 Band > number_band()-1
  //-------------------------------------------------------------------------

  if (Band < 0) {
    GeoCal::GeoCalCore::Exception e;
    e << "Band < 0";
    throw e;
  }
  if (Band > number_band() - 1) {
    GeoCal::GeoCalCore::Exception e;
    e << "Band > number_band() - 1";
    throw e;
  }

  //-------------------------------------------------------------------------
  // Transform look vector to detector coordinate space.
  //-------------------------------------------------------------------------
 
  NEWMAT::ColumnVector v_dcs(3);
  detector_look(Sl, v_dcs);

  //-------------------------------------------------------------------------
  // Project detector coordinate vector onto paraxial focal plane.  Units are
  // millimeters.
  //-------------------------------------------------------------------------

  double yf = ( focal_length_ / v_dcs(3) ) * (-v_dcs(1));
  double xf = ( focal_length_ / v_dcs(3) ) * v_dcs(2);

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
  
  return GeoCal::GeoCalCore::FrameCoor(line, sample);
}

/////////////////////////////////////////////////////////////////////////////
/// Translate from look vector to frame line coordinates for
/// the given band.
/////////////////////////////////////////////////////////////////////////////

double
GeoCal::Mspi::MspiCamera::frame_line_coor(
  const GeoCal::GeoCalCore::ScLookVector& Sl,
  int Band
) const
{
  return frame_coor(Sl,Band).line();
}

/////////////////////////////////////////////////////////////////////////////
/// Return the Jacobian of the frame coordinates with respect to the look
/// vector. The first row is the derivatives of the line with respect
/// to the look vector X, Y, and Z. The second row is the
/// derivatives of the sample.
/////////////////////////////////////////////////////////////////////////////

NEWMAT::Matrix 
GeoCal::Mspi::MspiCamera::jacobian_look(
  const GeoCal::GeoCalCore::ScLookVector& Sl,
  int Band
) const
{
  NEWMAT::ColumnVector d(Sl.direction());
  NEWMAT::Matrix result(2,3);
  double pstep = 5e-7;
  for (int i = 1; i <= 3 ; i++) {
    NEWMAT::ColumnVector d0(d);
    NEWMAT::ColumnVector d1(d);
    
    d0(i) -= pstep;
    d1(i) += pstep;

    GeoCal::GeoCalCore::ScLookVector sl0(d0, Sl.length());
    GeoCal::GeoCalCore::ScLookVector sl1(d1, Sl.length());

    GeoCal::GeoCalCore::FrameCoor fc0 = frame_coor(sl0, Band);
    GeoCal::GeoCalCore::FrameCoor fc1 = frame_coor(sl1, Band);
 
    result(1,i) = (fc1.line() - fc0.line()) / (pstep * 2.0);
    result(2,i) = (fc1.sample() - fc0.sample()) / (pstep * 2.0);
  }
  
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the Jacobian of the frame coordinates with respect to the parameters
/// The first row is the derivatives of line with respect to the parameters
/// The second row is the derivatives of the sample.
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<double,2>
GeoCal::Mspi::MspiCamera::jacobian(
  const GeoCal::GeoCalCore::ScLookVector& Sl,
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

    GeoCal::GeoCalCore::FrameCoor fc0 = camera_tmp0.frame_coor(Sl, Band);
    GeoCal::GeoCalCore::FrameCoor fc1 = camera_tmp1.frame_coor(Sl, Band);

    result[0][i] = (fc1.line() - fc0.line()) / pstep;
    result[1][i] = (fc1.sample() - fc0.sample()) / pstep;
  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Set the camera model parameters.  The given column vector includes the
/// following parameters:
///
///  index    parameter
///   1       camera to station yaw in radians
///   2       camera to station pitch in radians
///   3       camera to station roll in radians
/////////////////////////////////////////////////////////////////////////////

void    
GeoCal::Mspi::MspiCamera::parameter(const NEWMAT::ColumnVector& Pr)
{
  camera_yaw_ = Pr(1);
  camera_pitch_ = Pr(2);
  camera_roll_ = Pr(3);
  init_camera_to_station_transform();
}

/////////////////////////////////////////////////////////////////////////////
/// Get camera model parameters.  Returned column vector includes the
/// following:
///
///  index    parameter
///   1       camera to station yaw in radians
///   2       camera to station pitch in radians
///   3       camera to station roll in radians
/////////////////////////////////////////////////////////////////////////////

NEWMAT::ColumnVector 
GeoCal::Mspi::MspiCamera::parameter() const
{
  NEWMAT::ColumnVector result(3);
  result(1) = camera_yaw_;
  result(2) = camera_pitch_;
  result(3) = camera_roll_;
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Translate frame coordinate to unit vector in detector coordinate
/// system for the given band.
/////////////////////////////////////////////////////////////////////////////

void 
GeoCal::Mspi::MspiCamera::detector_look(
  const GeoCal::GeoCalCore::FrameCoor& F,
  int Band,
  NEWMAT::ColumnVector& Dcs_look
) const
{

  //-------------------------------------------------------------------------
  // Argument check: Band < 0 
  //                 Band > number_band()-1
  //-------------------------------------------------------------------------

  if (Band < 0) {
    GeoCal::GeoCalCore::Exception e;
    e << "Band < 0";
    throw e;
  }
  if (Band > number_band() - 1) {
    GeoCal::GeoCalCore::Exception e;
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
  Dcs_look(1) = -yf * u;
  Dcs_look(2) = xf * u;
  Dcs_look(3) = focal_length_ * u;

}

/////////////////////////////////////////////////////////////////////////////
/// Translate station look vector to unit vector in detector coordinate 
/// system.
/////////////////////////////////////////////////////////////////////////////

void 
GeoCal::Mspi::MspiCamera::detector_look(
  const GeoCal::GeoCalCore::ScLookVector& Sl,
  NEWMAT::ColumnVector& Dcs_look
) const
{
  //-------------------------------------------------------------------------
  // Transform look vector from station coordinates to camera coordinates
  //-------------------------------------------------------------------------

  NEWMAT::ColumnVector v_ccs = t_cs_ * Sl.direction();

  //-------------------------------------------------------------------------
  // Transform look vector from camera coordinate space to detector coordinate
  // space.  Result is a unit vector because Sl.direction is a unit vector.
  //-------------------------------------------------------------------------

  Dcs_look = t_dc_ * v_ccs;
}

/////////////////////////////////////////////////////////////////////////////
/// Calculate camera to station transform.
/////////////////////////////////////////////////////////////////////////////

void 
GeoCal::Mspi::MspiCamera::init_camera_to_station_transform()
{

  //-------------------------------------------------------------------------
  // Construct matrix for transforming camera coordinates to station 
  // coordinates.
  //-------------------------------------------------------------------------

  NEWMAT::Matrix t_axes(3,3);
  
  NEWMAT::Matrix t_yaw(3,3);
  t_yaw = 0;
  t_yaw(1,1) = std::cos(camera_yaw_);
  t_yaw(1,2) = std::sin(camera_yaw_);
  t_yaw(2,1) = -std::sin(camera_yaw_);
  t_yaw(2,2) = std::cos(camera_yaw_);
  t_yaw(3,3) = 1;

  NEWMAT::Matrix t_pitch(3,3);
  t_pitch = 0;
  t_pitch(1,1) = std::cos(camera_pitch_);
  t_pitch(1,3) = -std::sin(camera_pitch_);
  t_pitch(2,2) = 1;
  t_pitch(3,1) = std::sin(camera_pitch_);
  t_pitch(3,3) = std::cos(camera_pitch_);

  NEWMAT::Matrix t_roll(3,3);
  t_roll = 0;
  t_roll(1,1) = 1;
  t_roll(2,2) = std::cos(camera_roll_);
  t_roll(2,3) = std::sin(camera_roll_);
  t_roll(3,2) = -std::sin(camera_roll_);
  t_roll(3,3) = std::cos(camera_roll_);
  
  t_axes = t_yaw * t_pitch * t_roll;

  NEWMAT::Matrix t_boresight(3,3);
  t_boresight = 0;
  t_boresight(1,1) = std::cos(boresight_angle_);
  t_boresight(1,3) = -std::sin(boresight_angle_);
  t_boresight(2,2) = 1;
  t_boresight(3,1) = std::sin(boresight_angle_);
  t_boresight(3,3) = std::cos(boresight_angle_);

  t_sc_ = t_axes * t_boresight;

  //-------------------------------------------------------------------------
  // Construct matrix for transforming station coordinates to camera coordinates
  //-------------------------------------------------------------------------

  t_cs_ = t_sc_.t();
}

/////////////////////////////////////////////////////////////////////////////
/// Write camera parameters to a file.
/////////////////////////////////////////////////////////////////////////////

void 
GeoCal::Mspi::MspiCamera::write(
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

