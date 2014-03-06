// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "spot_camera.h"
%}
%base_import(camera)
%geocal_shared_ptr(GeoCal::SpotCamera);
namespace GeoCal {
class SpotCamera : public PushBroomCamera {
public:
  SpotCamera(const std::vector<double>& Psi_x, 
	     const std::vector<double>& Psi_y);
  virtual int number_sample(int Band) const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  %python_attribute(psi_x, std::vector<double>)
  %python_attribute(psi_y, std::vector<double>)
  %pickle_init(1, self.psi_x, self.psi_y)
};

}