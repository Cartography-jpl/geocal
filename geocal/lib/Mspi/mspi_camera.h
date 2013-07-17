/*===========================================================================
=                                                                           =
=                                MspiCamera                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                  MspiCamera

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

=============================================================================
!END=======================================================================*/

#ifndef MSPI_CAMERA_H
#define MSPI_CAMERA_H

#include "GeoCalCore/InterfaceClass/Camera/push_broom_camera.h"
                                // Definition of PushBroomCamera.
#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "paraxial_transform.h"	  // Definition of ParaxialTransform
#include "MSPI-Shared/Config/src/config_file.h" 
				// Definition of MSPI::Shared::ConfigFile
#include <cmath> 		// Definition of std::atan
#include "GeoCalCore/InterfaceClass/Camera/frame_coor.h"
                                // Definition of FrameCoor
#include <sstream>		// Definition of std::ostringstream
#include "GeoCalCore/InterfaceClass/ScCoordinate/sc_look_vector.h"
                                // Definition of ScLookVector.
#include <boost/multi_array.hpp>
				// Definition of boost::multi_array

namespace GeoCal {
   namespace Mspi {
      class MspiCamera;
   }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief MSPI Camera Model
///
/// References:
/// [1] Veljko Jovanovic, ATB and processing software design consideration 
///     for GroundMSPI L1B2 process, Rev. A, July 16, 2009.
/////////////////////////////////////////////////////////////////////////////

class GeoCal::Mspi::MspiCamera: public GeoCal::GeoCalCore::PushBroomCamera {
public:
  MspiCamera(const MSPI::Shared::ConfigFile& Config);

  virtual ~MspiCamera();
  virtual GeoCal::GeoCalCore::FrameCoor
    frame_coor(const GeoCal::GeoCalCore::ScLookVector& Sl, int Band) const;
  virtual double
    frame_line_coor(const GeoCal::GeoCalCore::ScLookVector& Sl, int Band) const;
  virtual NEWMAT::Matrix 
    jacobian_look(const GeoCal::GeoCalCore::ScLookVector& Sl, int Band) const;
  boost::multi_array<double,2>
    jacobian(const GeoCal::GeoCalCore::ScLookVector& Sl, int Band) const;
  virtual void    
    parameter(const NEWMAT::ColumnVector& Pr);
  virtual NEWMAT::ColumnVector parameter() const;
  virtual GeoCal::GeoCalCore::ScLookVector 
    sc_coor(const GeoCal::GeoCalCore::FrameCoor& F, int Band) const;
  GeoCal::GeoCalCore::ScLookVector 
    sc_coor(const NEWMAT::ColumnVector& V_dcs) const; 
  void detector_look(const GeoCal::GeoCalCore::FrameCoor& F,int Band,
		     NEWMAT::ColumnVector& Dcs_look) const;
  void detector_look(const GeoCal::GeoCalCore::ScLookVector& Sl,
		     NEWMAT::ColumnVector& Dcs_look) const;
  double
    angular_separation(int Reference_band, 
		       int Target_band) const;
  void paraxial_offset(int Band,
		       const GeoCal::GeoCalCore::FrameCoor& F,
		       double& Line_offset,
		       double& Sample_offset) const;
  int row_number(int Band) const;
  int band_number(int Row_number) const;
  int inversion () const;
  const std::string& granule_id() const;
  const std::vector<double>& parameter_step_size() const;
  const NEWMAT::SymmetricMatrix& covariance() const;
  void covariance(const NEWMAT::SymmetricMatrix& Matrix);
  void write(const std::string& Filename) const;

protected:

private:
  double row_origin(int Band) const;
  void real_focal_plane_coor(int Band, 
			     const GeoCal::GeoCalCore::FrameCoor& F,
			     double& Xf_prime, 
			     double& Yf_prime) const;
  void init_camera_to_station_transform();

  double pi_; 		///< Pi
  double deg_to_rad_;   ///< Convert degress to radians.
  double focal_length_; ///< Effective focal length in millimeters.
  double dx_;           ///< center-to-center sample spacing (millimeters)
  double ypitch_;       ///< pixel size in along row axis (millimeters)
  double dy_;           ///< center-to-center row spacing (millimeters)
  double s_origin_;     ///< Detector pixel coordinate intersecting the
                        ///< negative Zdcs axis. (Figure 1 of [1])  
                        ///< Units are 0-based pixel centers.
                        ///<     0 = center of the first pixel
                        ///< 767.5 = midway between the two center pixels
                        ///<  1535 = center of last pixel
  int number_row_;
  int direction_;       ///< Direction of line motion with respect to camera axes.
  int pixel_order_;     ///< Direction of pixel number with respect to xf
                        ///< axis of detector.  (Figure 1 of [1])
                        ///<   1 = pixel number increasing as xf increasing
                        ///<  -1 = pixel number decreasing as xf increasing
  double boresight_angle_; ///< Defines a rotation around Yccs in transform between
                           ///< camera coordinates and station coordinates.
  double camera_yaw_;   ///< Camera to station yaw in radians.
  double camera_pitch_; ///< Camera to station pitch in radians.
  double camera_roll_;  ///< Camera to station roll in radians.
  int inversion_;       ///< Defines inversion of 180 deg around boresight axis prior to the scan
                        ///< this is specifcly needed for AirMSPI camera used on the ground

  NEWMAT::Matrix t_dc_; ///< Matrix to transform camera coordinates to detector coordinates.
  NEWMAT::Matrix t_cd_; ///< Matrix to transform detector coordinates to camera coordinates.
  NEWMAT::Matrix t_sc_; ///< Matrix to transform camera coordinates to station coordinates.
  NEWMAT::Matrix t_cs_; ///< Matrix to transform station coordinates to camera coordinates.
  std::vector<int> row_number_;   ///< Assigned row number for each band.
  std::map<int,int> band_number_; ///< Band number for each row.
  ParaxialTransform paraxial_transform_;
  std::string granule_id_;
  std::vector<double> parameter_step_size_;
                        ///< Suggested interval to use when stepping parameters for 
                        ///< calculation of numerical derivatives.
  NEWMAT::SymmetricMatrix covariance_;
};

/////////////////////////////////////////////////////////////////////////////
/// Update covariance matrix.
/////////////////////////////////////////////////////////////////////////////

inline void
GeoCal::Mspi::MspiCamera::covariance(const NEWMAT::SymmetricMatrix& Matrix)
{
  //------------------------------------------------------------------------
  // Argument check: Matrix.Nrows() != covariance_.Nrows()
  //                 Matrix.Ncols() != covariance_.Ncols()
  //------------------------------------------------------------------------

  if (Matrix.Nrows() != covariance_.Nrows()) {
    throw std::range_error("Matrix.Nrows() != covariance_.Nrows()");
  }
  if (Matrix.Ncols() != covariance_.Ncols()) {
    throw std::range_error("Matrix.Ncols() != covariance_.Ncols()");
  }

  //------------------------------------------------------------------------
  // Update matrix
  //------------------------------------------------------------------------

  covariance_ = Matrix;
}

/////////////////////////////////////////////////////////////////////////////
/// Return covariance matrix.
/////////////////////////////////////////////////////////////////////////////

inline const NEWMAT::SymmetricMatrix& 
GeoCal::Mspi::MspiCamera::covariance() const
{
  return covariance_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return vector of step sizes for each parameter.
/////////////////////////////////////////////////////////////////////////////

inline const std::vector<double>&
GeoCal::Mspi::MspiCamera::parameter_step_size() const
{
  return parameter_step_size_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return granule id.
/////////////////////////////////////////////////////////////////////////////

inline const std::string&
GeoCal::Mspi::MspiCamera::granule_id() const
{
  return granule_id_;
}

/////////////////////////////////////////////////////////////////////////////
/// Translate frame coordinates to look vector for the given band.
/////////////////////////////////////////////////////////////////////////////

inline GeoCal::GeoCalCore::ScLookVector 
GeoCal::Mspi::MspiCamera::sc_coor(
  const NEWMAT::ColumnVector& V_dcs
) const
{
  //-------------------------------------------------------------------------
  // Transform look vector from detector coordinate space to camera 
  // coordinate space.
  //-------------------------------------------------------------------------

  NEWMAT::ColumnVector v_ccs = t_cd_ * V_dcs;

  //-------------------------------------------------------------------------
  // Transform look vector from camera coordinates to station coordinates
  //-------------------------------------------------------------------------

  NEWMAT::ColumnVector v_scs = t_sc_ * v_ccs;

  //-------------------------------------------------------------------------
  // Return
  //-------------------------------------------------------------------------

  return GeoCal::GeoCalCore::ScLookVector(v_scs, 1.0);
}

/////////////////////////////////////////////////////////////////////////////
/// Translate frame coordinates to look vector for the given band.
/////////////////////////////////////////////////////////////////////////////

inline GeoCal::GeoCalCore::ScLookVector 
GeoCal::Mspi::MspiCamera::sc_coor(
  const GeoCal::GeoCalCore::FrameCoor& F,
  int Band
) const
{
  //-------------------------------------------------------------------------
  // Transform Frame Coordinate to look vectors in detector coordinate system.
  //-------------------------------------------------------------------------

  NEWMAT::ColumnVector v_dcs(3);
  detector_look(F,Band,v_dcs);

  //-------------------------------------------------------------------------
  // Translate detector coordinate to station coordinate.
  //-------------------------------------------------------------------------

  return sc_coor(v_dcs);
}

////////////////////////////////////////////////////////////////////////////
/// Return row origin in the real focal plane, for the given band.
/// (MSPI L1B2 ATB equation 1.1)
////////////////////////////////////////////////////////////////////////////

inline double
GeoCal::Mspi::MspiCamera::row_origin(int Band) const
{
  return -dy_ * (row_number_[Band] + 0.5 - (number_row_ / 2.0));
}

////////////////////////////////////////////////////////////////////////////
/// Return angular separation (in radians) between the given reference and
/// target bands in the real focal plane.
/// (MSPI L1B2 ATB equation 14 -- *modified* to use Forigin in place of b0)
////////////////////////////////////////////////////////////////////////////

inline double
GeoCal::Mspi::MspiCamera::angular_separation(
  int Reference_band, 
  int Target_band) const
{
  return ((std::atan(row_origin(Target_band)/focal_length_)-
	   std::atan(row_origin(Reference_band)/focal_length_))* (double)inversion_);
}

////////////////////////////////////////////////////////////////////////////
/// Return paraxial displacement (in pixel units) for the given frame
/// coordinate and band.  The paraxial transform equations are defined
/// such that the frame line coordinate is always assumed to be zero
/// (i.e. centered in the line for the given band).  Therefore values other
/// than zero for frame line will not produce a different result.
////////////////////////////////////////////////////////////////////////////

inline void
GeoCal::Mspi::MspiCamera::paraxial_offset(
  int Band, 
  const GeoCal::GeoCalCore::FrameCoor& F,
  double& Line_offset,
  double& Sample_offset
) const
{
  double xf_prime;
  double yf_prime;
  real_focal_plane_coor(Band, F, xf_prime, yf_prime);
  
  double xf;
  double yf;
  paraxial_transform_.real_to_paraxial(row_number_[Band],
				       xf_prime,
				       yf_prime,
				       xf,
				       yf);
  Line_offset = double (inversion_)* (yf - yf_prime) / ypitch_;
  Sample_offset = (xf - xf_prime) / dx_;
}  

////////////////////////////////////////////////////////////////////////////
/// Convert the given frame coordinate and band to real focal plane
/// coordinates, in milimeters.
////////////////////////////////////////////////////////////////////////////

inline void
GeoCal::Mspi::MspiCamera::real_focal_plane_coor(
  int Band, 
  const GeoCal::GeoCalCore::FrameCoor& F,
  double& Xf_prime, 
  double& Yf_prime
) const
{
  Xf_prime = dx_ * pixel_order_ * ( F.sample() - s_origin_ );
  Yf_prime = (row_origin(Band) - F.line() * ypitch_ / direction_);
}

////////////////////////////////////////////////////////////////////////////
/// Return detector row number associated with the given band.
////////////////////////////////////////////////////////////////////////////

inline int
GeoCal::Mspi::MspiCamera::row_number(int Band) const
{
  return row_number_[Band];
}

////////////////////////////////////////////////////////////////////////////
/// Return band number associated with the given detector row number.
/// Throws an exception if the row number is not assigned to a band.
////////////////////////////////////////////////////////////////////////////

inline int
GeoCal::Mspi::MspiCamera::band_number(int Row_number) const
{
  std::map<int,int>::const_iterator 
    h = band_number_.find(Row_number);
  
  if (h == band_number_.end()) {
    std::ostringstream message;
    message << "Row number not assigned to camera model band: " << Row_number;
    throw std::range_error(message.str());
  }

  return h->second;
}

inline int
GeoCal::Mspi::MspiCamera::inversion() const
{

  return inversion_;
}
#endif
