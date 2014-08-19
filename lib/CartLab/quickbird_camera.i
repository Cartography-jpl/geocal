// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geocal_time.h"
#include "quickbird_camera.h"
%}
%base_import(quaternion_camera)
%geocal_shared_ptr(GeoCal::QuickBirdCamera);

namespace GeoCal {
class QuickBirdCamera : public QuaternionCamera {
public:
  QuickBirdCamera();
  virtual ~QuickBirdCamera();
  %pickle_init(1);
};

}
