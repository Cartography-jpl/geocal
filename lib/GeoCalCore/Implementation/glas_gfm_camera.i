// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "glas_gfm_camera.h"
%}

%base_import(camera)
%import "quaternion_camera.i"

%geocal_shared_ptr(GeoCal::GlasGfmCamera);
namespace GeoCal {
class GlasGfmCamera : public Camera {
public:
  GlasGfmCamera(int Number_line = 1, int Number_sample = 256);
  GlasGfmCamera(const Camera& Cam, int Band, double Delta_sample,
		const std::string& Band_type = "N",
		double Band_wavelength = 1.45,
		double Focal_length_meter = 1.0,
		const Time& Focal_length_time = Time::time_j2000(0));
  GlasGfmCamera(const Camera& Cam, int Band, double Delta_line,
		double Delta_sample,
		const std::string& Band_type = "N",
		double Band_wavelength = 1.45,
		double Focal_length_meter = 1.0,
		const Time& Focal_length_time = Time::time_j2000(0));
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  void set_number_line(int V);
  void set_number_sample(int V);
  %python_attribute_with_set(focal_length, double);
  %python_attribute_with_set(focal_length_with_derivative, 
			     AutoDerivative<double>);

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
		   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
				 int Band) const;

  %python_attribute_with_set(frame_to_sc, boost::math::quaternion<double>);
  %python_attribute_with_set(frame_to_sc_with_derivative, boost::math::quaternion<AutoDerivative<double> >);
  %python_attribute_with_set(id, std::string);
  %python_attribute(sensor_type, std::string);
  %python_attribute_with_set(fit_epsilon, bool);
  %python_attribute_with_set(fit_beta, bool);
  %python_attribute_with_set(fit_delta, bool);
  %python_attribute_with_set(fit_focal_length, bool);
  %python_attribute_with_set(band_type, std::string);
  %python_attribute_with_set(band_wavelength, double);
  %python_attribute_with_set(band_index, std::vector<int>);
  %python_attribute_with_set(irepband, std::vector<std::string>);
  %python_attribute_with_set(isubcat, std::vector<std::string>);
  %python_attribute_with_set(focal_length_time, Time);
  %python_attribute_with_set(ppoff, blitz::Array<double, 1>);
  %python_attribute_with_set(angoff, blitz::Array<double, 1>);
  %python_attribute_with_set(sample_number_first, double);
  %python_attribute_with_set(delta_sample_pair, double);
  %python_attribute_with_set(field_alignment, blitz::Array<double, 2>);
  void field_alignment_fit(const Camera& Cam, double Delta_sample,
			   int Band=0);
  %python_attribute_with_set(field_angle_type, int);
  %python_attribute_with_set(field_angle_interpolation_type, int);
  %python_attribute_with_set(first_line_block, blitz::Array<double, 1>);
  %python_attribute_with_set(first_sample_block, blitz::Array<double, 1>);
  %python_attribute_with_set(delta_line_block, blitz::Array<double, 1>);
  %python_attribute_with_set(delta_sample_block, blitz::Array<double, 1>);
  const blitz::Array<double, 5>& field_alignment_block(int i) const;
  void field_alignment_block(int i, const blitz::Array<double, 5>& V);
  void field_alignment_block(const Camera& cam, double Delta_line,
			     double Delta_sample, int Band = 0);
  void compare_camera(const Camera& Cam, double& OUTPUT,
		      double& OUTPUT, int Band = 0) const;
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("GlasGfmCamera")
