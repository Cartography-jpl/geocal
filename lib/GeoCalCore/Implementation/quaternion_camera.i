// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "quaternion_camera.h"
%}
%base_import(generic_object)
%base_import(observer)
%base_import(camera)
%geocal_shared_ptr(GeoCal::QuaternionCamera);
namespace GeoCal {
  %copyctor QuaternionCamera;

class QuaternionCamera : public Camera {
public:
  enum FrameConvention { LINE_IS_X, LINE_IS_Y};
  enum FrameDirection { INCREASE_IS_POSITIVE, INCREASE_IS_NEGATIVE};
  QuaternionCamera(boost::math::quaternion<double> frame_to_sc_q, 
		   double Number_line, double Number_sample,
		   double Line_pitch, double Sample_pitch,
		   double Focal_length, 
		   const FrameCoordinate& Principal_point,
		   FrameConvention Frame_convention = LINE_IS_X,
		   FrameDirection Line_direction = INCREASE_IS_POSITIVE,
		   FrameDirection Sample_direction = INCREASE_IS_NEGATIVE);
  QuaternionCamera(boost::math::quaternion<double> frame_to_sc_q, 
		   double Number_line, double Number_sample,
		   double Line_pitch, double Sample_pitch,
		   double Focal_length, 
		   const FrameCoordinate& Principal_point,
		   FrameConvention Frame_convention,
		   FrameDirection Line_direction,
		   FrameDirection Sample_direction,
		   const blitz::Array<bool, 1>& Parameter_mask);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  %python_attribute_with_set(focal_length, double);
  %python_attribute_with_set(focal_length_with_derivative, 
			     AutoDerivative<double>);
  FrameCoordinate principal_point(int B) const;
  void principal_point(int B, const FrameCoordinate& Fc);
  const FrameCoordinate& principal_point_with_derivative(int B) const;
  void principal_point_with_derivative(int B, 
				       const FrameCoordinateWithDerivative& Fc);
  %python_attribute_with_set(line_pitch, double)
  %python_attribute_with_set(line_pitch_with_derivative, 
			     AutoDerivative<double>)
  %python_attribute_with_set(sample_pitch, double)
  %python_attribute_with_set(sample_pitch_with_derivative, 
			     AutoDerivative<double>)
  %python_attribute_with_set(frame_convention, FrameConvention)  
  %python_attribute_with_set(line_direction, FrameDirection)  
  %python_attribute_with_set(sample_direction, FrameDirection)  
  %python_attribute_with_set(frame_to_sc, boost::math::quaternion<double>)
  %python_attribute_with_set(frame_to_sc_with_derivative, 
			     boost::math::quaternion<AutoDerivative<double> >)
  %python_attribute_with_set(ypr, blitz::Array<double, 1>);
  %python_attribute_with_set(ypr_with_derivative, 
			     blitz::Array<AutoDerivative<double>, 1>);
  %python_attribute_with_set(euler, blitz::Array<double, 1>);
  %python_attribute_with_set(euler_with_derivative, 
			     blitz::Array<AutoDerivative<double>, 1>);
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
		   int Band) const;
  virtual FrameCoordinate focal_plane_to_fc(int Band, double Xfp, double Yfp)
    const;
  virtual FrameCoordinateWithDerivative focal_plane_to_fc
  (int Band, const AutoDerivative<double>& Xfp,
   const AutoDerivative<double>& Yfp) const;
  virtual void fc_to_focal_plane(const FrameCoordinate& Fc, int Band,
				 double& OUTPUT, double& OUTPUT) const;
  virtual void fc_to_focal_plane(const FrameCoordinateWithDerivative& Fc,
    int Band,
    AutoDerivative<double>& OUTPUT, AutoDerivative<double>& OUTPUT) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
				 int Band) const;
  virtual ScLookVector sc_look_vector(const DcsLookVector& Dlv) const;
  virtual DcsLookVector dcs_look_vector(const ScLookVector& Sl) const;
  virtual DcsLookVector dcs_look_vector(const FrameCoordinate& F, int Band) 
    const;
  %python_attribute_with_set(fit_epsilon, bool);
  %python_attribute_with_set(fit_beta, bool);
  %python_attribute_with_set(fit_delta, bool);
  %python_attribute_with_set(fit_line_pitch, bool);
  %python_attribute_with_set(fit_sample_pitch, bool);
  %python_attribute_with_set(fit_focal_length, bool);
  bool fit_principal_point_line(int Band = 0) const;
  void fit_principal_point_line(bool V, int Band = 0);
  bool fit_principal_point_sample(int Band = 0) const;
  void fit_principal_point_sample(bool V, int Band = 0);
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<double>& Dcs,
				  double& OUTPUT, double& OUTPUT) const;
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<AutoDerivative<double> >& Dcs,
				  AutoDerivative<double>& OUTPUT, 
				  AutoDerivative<double>& OUTPUT) const;
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double Xfp, double Yfp) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  focal_plane_to_dcs(int Band, const AutoDerivative<double>& Xfp, 
		     const AutoDerivative<double>& Yfp) const;
protected:
  QuaternionCamera();
  %pickle_serialization()
};
}

