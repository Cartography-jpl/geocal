#ifndef CAMERA_RATIONAL_POLYNOMIAL_H
#define CAMERA_RATIONAL_POLYNOMIAL_H
#include "quaternion_camera.h"

namespace GeoCal {
/****************************************************************//**
  This is a QuaternionCamera where the nonlinearity of the camera
  is described as a rational polynomial in the line and sample. This
  is the formulation Mike Burl uses in his matlab code, and we want to
  implement this so we can take his camera models for use in GeoCal.
*******************************************************************/

class CameraRationalPolyomial : public QuaternionCamera {
public:
//-----------------------------------------------------------------------
/// This makes a QuaternionCamera using Mike Burls conventions. He
/// works with (u,v), which is what we call (sample,line) (so
/// reversed). xi is the field angle of a pixel. He doesn't have an
/// explicit focal length (since he works in radians), so be
/// convention we set the focal length to 1 and then xi is what we use
/// for line and sample pitch. The principle point is (V0, U0)
/// He uses the term Pld_q_c for what we call Frame_to_sc_q.
///
/// *** NOTE *** I may have this reversed, if so we might need an
/// inverse in here
//-----------------------------------------------------------------------
  
  CameraRationalPolyomial(int Nu, int Nv, double Xi, double U0,
			  double V0,
			  boost::math::quaternion<double> Pld_q_c,
			  const blitz::Array<double, 2>& Kappa,
			  const blitz::Array<double, 2>& Kappa_inverse)
    : QuaternionCamera(Pld_q_c, Nv, Nu, Xi, Xi, 1.0,
		       FrameCoordinate(V0, U0), QuaternionCamera::LINE_IS_Y),
      kappa_(Kappa.copy()),
      kappa_inverse_(Kappa_inverse)
  {
  }
  virtual ~CameraRationalPolyomial() {}
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
/// Kappa, used to describe nonlinearity.
//-----------------------------------------------------------------------

  const blitz::Array<double, 2>& kappa() const {return kappa_;}

//-----------------------------------------------------------------------
/// Inverse of Kappa
//-----------------------------------------------------------------------

  const blitz::Array<double, 2>& kappa_inverse() const {return kappa_inverse_;}

  blitz::Array<double, 1> apply_rational(const blitz::Array<double, 1>& X,
					 const blitz::Array<double, 2>& Coeff)
    const;
  blitz::Array<double, 1> construct_chi_matrix(const blitz::Array<double, 1>& X,
					       int ord) const;
private:
  blitz::Array<double, 2> kappa_, kappa_inverse_;
  CameraRationalPolyomial() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(CameraRationalPolyomial);
#endif

