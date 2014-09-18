// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

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
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  %python_attribute_with_set(focal_length, double)
  const FrameCoordinate& principal_point(int B) const;
  void principal_point(int B, const FrameCoordinate& Fc);
  %python_attribute_with_set(line_pitch, double)
  %python_attribute_with_set(sample_pitch, double)
  %python_attribute_with_set(frame_convention, FrameConvention)  
  %python_attribute_with_set(line_direction, FrameDirection)  
  %python_attribute_with_set(sample_direction, FrameDirection)  
  %python_attribute_with_set(frame_to_sc, boost::math::quaternion<double>)
  %python_attribute_with_set(ypr, blitz::Array<double, 1>);
  %python_attribute_with_set(euler, blitz::Array<double, 1>);
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual ScLookVector sc_look_vector(const DcsLookVector& Dlv) const;
  virtual DcsLookVector dcs_look_vector(const ScLookVector& Sl) const;
  virtual DcsLookVector dcs_look_vector(const FrameCoordinate& F, int Band) 
    const;
protected:
  QuaternionCamera();
  virtual void dcs_to_focal_plane(int Band,
				  const boost::math::quaternion<double>& Dcs,
				  double& Xfp, double& Yfp) const;
  virtual boost::math::quaternion<double> 
  focal_plane_to_dcs(int Band, double& Xfp, double& Yfp) const;


  %pickle_init(1, self.frame_to_sc, self.number_line(0), self.number_sample(0), 
	       self.line_pitch, self.sample_pitch, self.focal_length, 
	       self.principal_point(0), 
	       self.frame_convention, self.line_direction, 
	       self.sample_direction)
};
}

