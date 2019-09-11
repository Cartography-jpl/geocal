#ifndef CAMERA_RADIAL_DISTORTION_AND_TRANSFORM_H
#define CAMERA_RADIAL_DISTORTION_AND_TRANSFORM_H
#include "camera_radial_distortion.h"

namespace GeoCal {
/****************************************************************//**
  This is a CameraRadialDistortion, with the addition of a supplied 
  transform. This is the form of the HiRISE camera. I'm not sure how
  general this is, perhaps this should just be called HiRISE.
*******************************************************************/
class CameraRadialDistortionAndTransform : public CameraRadialDistortion {
public:
  CameraRadialDistortionAndTransform
  (boost::math::quaternion<double> Frame_to_sc_q,
   const blitz::Array<double, 1>& K_distort,
   double Number_line, double Number_sample,
   double Line_pitch, double Sample_pitch,
   double Focal_length,
   int Bin_mode,
   const blitz::Array<double, 1>& Ccd_off,
   const blitz::Array<double, 1>& Ccd_cen,
   const blitz::Array<double, 1>& T_off,
   const blitz::Array<double, 2>& T_m,
   const blitz::Array<double, 1>& Tinv_off,
   const blitz::Array<double, 2>& Tinv_m)
    : CameraRadialDistortion(Frame_to_sc_q, K_distort, Number_line,
			     Number_sample, Line_pitch, Sample_pitch,
			     Focal_length,
			     FrameCoordinate(Number_line / 2,
					     Number_sample /2)),
      bin_mode_(Bin_mode),
      ccd_off_(Ccd_off), ccd_cen_(Ccd_cen),
      t_off_(T_off.copy()), tinv_off_(Tinv_off.copy()),
      t_m_(T_m.copy()), tinv_m_(Tinv_m.copy())
  {}
  virtual ~CameraRadialDistortionAndTransform() {}

  int bin_mode() const { return bin_mode_;}
  void bin_mode(int V) { bin_mode_ = V; }

  const blitz::Array<double, 1>& ccd_off() const { return ccd_off_;}
  void ccd_off(const blitz::Array<double, 1>& V) { ccd_off_ = V;}

  void ccd_cen(const blitz::Array<double, 1>& V) { ccd_cen_ = V;}
  const blitz::Array<double, 1>& ccd_cen() const { return ccd_cen_;}

  const blitz::Array<double, 1>& t_off() const { return t_off_;}
  void t_off(const blitz::Array<double, 1>& V) { t_off_ = V;}

  const blitz::Array<double, 2>& t_m() const { return t_m_;}
  void t_m(const blitz::Array<double, 2>& V) { t_m_ = V;}

  const blitz::Array<double, 1>& tinv_off() const { return tinv_off_;}
  void tinv_off(const blitz::Array<double, 1>& V) { tinv_off_ = V;}

  const blitz::Array<double, 2>& tinv_m() const { return tinv_m_;}
  void tinv_m(const blitz::Array<double, 2>& V) { tinv_m_ = V;}

  virtual void print(std::ostream& Os) const;
  virtual FrameCoordinate focal_plane_to_fc(int Band, double Xfp, double Yfp)
    const;
  virtual FrameCoordinateWithDerivative focal_plane_to_fc
  (int Band, const AutoDerivative<double>& Xfp,
   const AutoDerivative<double>& Yfp) const;
  virtual void fc_to_focal_plane(const FrameCoordinate& Fc, int Band,
				 double& Xfp, double& Yfp) const;
  virtual void fc_to_focal_plane
  (const FrameCoordinateWithDerivative& Fc, int Band,
   AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const;
protected:
  CameraRadialDistortionAndTransform() {}
private:
  int bin_mode_;
  blitz::Array<double, 1> ccd_off_, ccd_cen_, t_off_, tinv_off_;
  blitz::Array<double, 2> t_m_, tinv_m_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(CameraRadialDistortionAndTransform);
#endif
  
