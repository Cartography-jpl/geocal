#ifndef CAMERA_RADIAL_DISTORTION_H
#define CAMERA_RADIAL_DISTORTION_H
#include "quaternion_camera.h"

namespace GeoCal {
/****************************************************************//**
  This is a QuaternionCamera where the nonlinearity of the camera is
  described by a radial model 
  
  r^2 = x^2 + y^2
  dr/r = k0 + r^2*(k1 + r^2*k2)
  x_corrected = x - (dr/r)*x
  y_corrected = y - (dr/r)*y

  Note this is similar to but not identical to the Brown-Conrady
  distortion model, see
  https://en.wikipedia.org/wiki/Distortion_(optics). 
  This is a common used by SPICE camera models, among other uses.

  We currently hardcode this to maximum 3rd order in k, we could easily
  generalize this if that ends up being useful.
*******************************************************************/
class CameraRadialDistortion : public QuaternionCamera {
public:
//-----------------------------------------------------------------------
/// Create a QuaternionCamera with a radial distortion model given
/// by kdistort. We currently limit kdistort.rows() <= 4, although we
/// could relax that if it ends up being useful.  
//-----------------------------------------------------------------------
  
  CameraRadialDistortion(boost::math::quaternion<double> Frame_to_sc_q,
			  const blitz::Array<double, 1>& K_distort,
			  double Number_line, double Number_sample,
			  double Line_pitch, double Sample_pitch,
			  double Focal_length, 
			  const FrameCoordinate& Principal_point,
			  QuaternionCamera::FrameConvention Frame_convention =
			  QuaternionCamera::LINE_IS_X,
			  QuaternionCamera::FrameDirection Line_direction =
			  QuaternionCamera::INCREASE_IS_POSITIVE,
			  QuaternionCamera::FrameDirection Sample_direction =
			  QuaternionCamera::INCREASE_IS_POSITIVE)
  : QuaternionCamera(Frame_to_sc_q, Number_line, Number_sample, Line_pitch,
		     Sample_pitch, Focal_length, Principal_point,
		     Frame_convention, Line_direction, Sample_direction),
    k_distort_(K_distort.copy()),
    max_r2_filled_in(false)
  {
    if(k_distort_.rows() > 4)
      throw Exception("Right now, only support k_distort.rows <= 4");
  }
  CameraRadialDistortion(boost::math::quaternion<double> Frame_to_sc_q, 
			  const blitz::Array<double, 1>& K_distort,
			  double Number_line, double Number_sample,
			  double Line_pitch, double Sample_pitch,
			  double Focal_length, 
			  const FrameCoordinate& Principal_point,
			  QuaternionCamera::FrameConvention Frame_convention,
			  QuaternionCamera::FrameDirection Line_direction,
			  QuaternionCamera::FrameDirection Sample_direction,
			  const blitz::Array<bool, 1>& Parameter_mask)
  : QuaternionCamera(Frame_to_sc_q, Number_line, Number_sample, Line_pitch,
		     Sample_pitch, Focal_length, Principal_point,
		     Frame_convention, Line_direction, Sample_direction,
		     Parameter_mask),
    k_distort_(K_distort.copy()),
    max_r2_filled_in(false)
  {
    if(k_distort_.rows() > 4)
      throw Exception("Right now, only support k_distort.rows <= 4");
  }
  virtual void print(std::ostream& Os) const;
  void dcs_to_focal_plane
  (int Band, const boost::math::quaternion<double>& Dcs,
   double& Xfp, double& Yfp) const;
  void dcs_to_focal_plane
  (int Band, const boost::math::quaternion<AutoDerivative<double> >& Dcs,
   AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const;
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const;

//-----------------------------------------------------------------------
/// K_distort, used to describe nonlinearity.
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& k_distort() const {return k_distort_;}

protected:
  CameraRadialDistortion() : max_r2_filled_in(false) {}
private:
  blitz::Array<double, 1> k_distort_;
  double dr_over_r_calc(double r2) const
  { if(k_distort_.rows() == 3)
      return k_distort_(0) + r2 * (k_distort_(1) + r2 * k_distort_(2));
    if(k_distort_.rows() == 4)
      return k_distort_(0) + r2 * (k_distort_(1) + r2 * (k_distort_(2) + r2 * k_distort_(3)));
    throw Exception("Unsupported k_distort_ size");
  }
  double dr_over_r_calc_from_x_y(double x, double y) const;
  mutable bool max_r2_filled_in;
  mutable double max_rp2;
  mutable double max_r2;
  mutable double max_dr_over_r;
  void fill_in_max() const;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(CameraRadialDistortion);
#endif
