// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "quickbird_camera.h"
%}
%base_import(camera)
%geocal_shared_ptr(GeoCal::QuickBirdCamera);

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
  %pickle_init(1);
};

}
