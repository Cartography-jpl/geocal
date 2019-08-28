// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "glas_gfm_camera.h"
%}

%base_import(camera)

%geocal_shared_ptr(GeoCal::GlasGfmCamera);
namespace GeoCal {
class GlasGfmCamera : public Camera {
public:
  GlasGfmCamera();
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
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
  
  %python_attribute_with_set(id, std::string);
  %python_attribute(sensor_type, std::string);
  %python_attribute_with_set(band_type, std::string);
  %python_attribute_with_set(band_wavelength, double);
  %python_attribute_with_set(band_index, std::vector<int>);
  %python_attribute_with_set(irepband, std::vector<std::string>);
  %python_attribute_with_set(isubcat, std::vector<std::string>);
  %python_attribute_with_set(focal_length_time, Time);
  %python_attribute(ppoff, blitz::Array<double, 1>);
  %python_attribute(angoff, blitz::Array<double, 1>);
  %python_attribute_with_set(sample_number_first, double);
  %python_attribute_with_set(delta_sample_pair, double);
  %python_attribute_with_set(field_alignment, blitz::Array<double, 2>);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("GlasGfmCamera")
