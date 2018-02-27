// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "spot_camera.h"
%}
%base_import(camera)
%geocal_shared_ptr(GeoCal::SpotCamera);
namespace GeoCal {
class SpotCamera : public Camera {
public:
  SpotCamera(const std::vector<double>& Psi_x, 
	     const std::vector<double>& Psi_y);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
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
  %python_attribute(psi_x, std::vector<double>)
  %python_attribute(psi_y, std::vector<double>)
  %pickle_serialization();
};

}
// List of things "import *" will include
%python_export("SpotCamera")
