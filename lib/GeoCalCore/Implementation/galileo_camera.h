#ifndef GALILEO_CAMERA_H
#define GALILEO_CAMERA_H
#include "quaternion_camera.h"

namespace GeoCal {
/****************************************************************//**
  This is the galileo camera, including the nonlinearity correction.
  Right now we just have hardcoded everything, but this comes from 
  the spice kernel available in 
  unit_test_data/galileo_kernel/ik/gll36001.ti. It wouldn't be that
  hard to update this to read the parameters from the spice kernel,
  but there isn't any particular reason right now to do so.
*******************************************************************/

class GalileoCamera: public QuaternionCamera {
public:
  GalileoCamera();
  virtual ~GalileoCamera() {};
  virtual void dcs_to_focal_plane(int Band, 
				  const boost::math::quaternion<double>& Dcs,
				  double& Xfp, double& Yfp) const;
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<AutoDerivative<double> >& Dcs,
				  AutoDerivative<double>& Xfp, 
				  AutoDerivative<double>& Yfp) const;
  virtual boost::math::quaternion<double> focal_plane_to_dcs
  (int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const;
  virtual void print(std::ostream& Os) const
  { Os << "Galileo camera (based on spice kernel file gll36001.ti)"; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(GalileoCamera);
#endif
