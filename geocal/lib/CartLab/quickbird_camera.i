// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "quickbird_camera.h"
%}
%geocal_shared_ptr(QuickBirdCamera);

namespace GeoCal {
class QuickBirdCamera : public PushBroomCamera {
public:
  QuickBirdCamera();
  virtual ~QuickBirdCamera();
  virtual int number_sample(int Band) const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  %pythoncode {
def __reduce__(self):
  return _new_from_init, (self.__class__,)
  }
};

}
