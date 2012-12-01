// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "spot_camera.h"
%}

%geocal_shared_ptr(SpotCamera);
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
  %pickle_init(self.psi_x, self.psi_y)
};

}
