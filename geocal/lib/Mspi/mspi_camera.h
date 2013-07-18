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

#include "camera.h"
                                // Definition of PushBroomCamera.
#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "paraxial_transform.h"	  // Definition of ParaxialTransform
#include "MSPI-Shared/Config/src/config_file.h" 
				// Definition of MSPI::Shared::ConfigFile
#include <cmath> 		// Definition of std::atan
#include <sstream>		// Definition of std::ostringstream
#include <boost/multi_array.hpp>
				// Definition of boost::multi_array
#include <blitz/array.h>

namespace GeoCal {

/////////////////////////////////////////////////////////////////////////////
/// @brief MSPI Camera Model
///
/// References:
/// [1] Veljko Jovanovic, ATB and processing software design consideration 
///     for GroundMSPI L1B2 process, Rev. A, July 16, 2009.
/////////////////////////////////////////////////////////////////////////////

class MspiCamera: public PushBroomCamera {
public:
  MspiCamera(const MSPI::Shared::ConfigFile& Config);

  virtual ~MspiCamera() {}
  virtual int number_band() const { return nband; }
  virtual int number_sample(int Band) const {return nsamp; }
  virtual FrameCoordinate
    frame_coordinate(const ScLookVector& Sl, int Band) const;
  virtual blitz::Array<double, 2>
    jacobian_look(const ScLookVector& Sl, int Band) const;
  boost::multi_array<double,2>
    jacobian(const ScLookVector& Sl, int Band) const;
  virtual void    
  parameter(const blitz::Array<double, 1>& Pr);
  virtual blitz::Array<double, 1> parameter() const;
  virtual ScLookVector 
    sc_look_vector(const FrameCoordinate& F, int Band) const;
  ScLookVector 
    sc_coor(const blitz::Array<double, 1>& V_dcs) const; 
  void detector_look(const FrameCoordinate& F,int Band,
		     blitz::Array<double, 1>& Dcs_look) const;
  void detector_look(const ScLookVector& Sl,
		     blitz::Array<double, 1>& Dcs_look) const;
  double
    angular_separation(int Reference_band, 
		       int Target_band) const;
  void paraxial_offset(int Band,
		       const FrameCoordinate& F,
		       double& Line_offset,
		       double& Sample_offset) const;
  int row_number(int Band) const;
  int band_number(int Row_number) const;
  int inversion () const;
  const std::string& granule_id() const;
  const std::vector<double>& parameter_step_size() const;
  void write(const std::string& Filename) const;
  virtual void print(std::ostream& Os) const
  {
    Os << "MspiCamera:\n"
       << "  Filename: " << filename << "\n";
  }
protected:

private:
  int nband, nsamp;
  std::string filename;
  double row_origin(int Band) const;
  void real_focal_plane_coor(int Band, 
			     const FrameCoordinate& F,
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

  blitz::Array<double, 2> t_dc_; ///< Matrix to transform camera coordinates to detector coordinates.
  blitz::Array<double, 2> t_cd_; ///< Matrix to transform detector coordinates to camera coordinates.
  blitz::Array<double, 2> t_sc_; ///< Matrix to transform camera coordinates to station coordinates.
  blitz::Array<double, 2> t_cs_; ///< Matrix to transform station coordinates to camera coordinates.
  std::vector<int> row_number_;   ///< Assigned row number for each band.
  std::map<int,int> band_number_; ///< Band number for each row.
  ParaxialTransform paraxial_transform_;
  std::string granule_id_;
  std::vector<double> parameter_step_size_;
                        ///< Suggested interval to use when stepping parameters for 
                        ///< calculation of numerical derivatives.
};

/////////////////////////////////////////////////////////////////////////////
/// Return vector of step sizes for each parameter.
/////////////////////////////////////////////////////////////////////////////

inline const std::vector<double>&
MspiCamera::parameter_step_size() const
{
  return parameter_step_size_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return granule id.
/////////////////////////////////////////////////////////////////////////////

inline const std::string&
MspiCamera::granule_id() const
{
  return granule_id_;
}

/////////////////////////////////////////////////////////////////////////////
/// Translate frame coordinates to look vector for the given band.
/////////////////////////////////////////////////////////////////////////////

inline ScLookVector 
MspiCamera::sc_coor(
  const blitz::Array<double, 1>& V_dcs
) const
{
  blitz::firstIndex i1; blitz::secondIndex i2;

  //-------------------------------------------------------------------------
  // Transform look vector from detector coordinate space to camera 
  // coordinate space.
  //-------------------------------------------------------------------------

  blitz::Array<double, 1> v_ccs(t_cd_.rows());
  v_ccs = blitz::sum(t_cd_(i1, i2) * V_dcs(i2), i2);

  //-------------------------------------------------------------------------
  // Transform look vector from camera coordinates to station coordinates
  //-------------------------------------------------------------------------

  blitz::Array<double, 1> v_scs(t_sc_.rows());
  v_scs = blitz::sum(t_sc_(i1, i2) * v_ccs(i2));

  //-------------------------------------------------------------------------
  // Return
  //-------------------------------------------------------------------------

  return ScLookVector(v_scs(0), v_scs(1), v_scs(2));
}

/////////////////////////////////////////////////////////////////////////////
/// Translate frame coordinates to look vector for the given band.
/////////////////////////////////////////////////////////////////////////////

inline ScLookVector 
MspiCamera::sc_look_vector(
  const FrameCoordinate& F,
  int Band
) const
{
  //-------------------------------------------------------------------------
  // Transform Frame Coordinate to look vectors in detector coordinate system.
  //-------------------------------------------------------------------------

  blitz::Array<double, 1> v_dcs(3);
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
MspiCamera::row_origin(int Band) const
{
  return -dy_ * (row_number_[Band] + 0.5 - (number_row_ / 2.0));
}

////////////////////////////////////////////////////////////////////////////
/// Return angular separation (in radians) between the given reference and
/// target bands in the real focal plane.
/// (MSPI L1B2 ATB equation 14 -- *modified* to use Forigin in place of b0)
////////////////////////////////////////////////////////////////////////////

inline double
MspiCamera::angular_separation(
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
MspiCamera::paraxial_offset(
  int Band, 
  const FrameCoordinate& F,
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
MspiCamera::real_focal_plane_coor(
  int Band, 
  const FrameCoordinate& F,
  double& Xf_prime, 
  double& Yf_prime
) const
{
  Xf_prime = dx_ * pixel_order_ * ( F.sample - s_origin_ );
  Yf_prime = (row_origin(Band) - F.line * ypitch_ / direction_);
}

////////////////////////////////////////////////////////////////////////////
/// Return detector row number associated with the given band.
////////////////////////////////////////////////////////////////////////////

inline int
MspiCamera::row_number(int Band) const
{
  return row_number_[Band];
}

////////////////////////////////////////////////////////////////////////////
/// Return band number associated with the given detector row number.
/// Throws an exception if the row number is not assigned to a band.
////////////////////////////////////////////////////////////////////////////

inline int
MspiCamera::band_number(int Row_number) const
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
MspiCamera::inversion() const
{

  return inversion_;
}
}
#endif
