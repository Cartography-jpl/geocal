// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geocal_time.h"
#include "galileo_camera.h"
%}
%base_import(quaternion_camera)
%geocal_shared_ptr(GeoCal::GalileoCamera);
namespace GeoCal {

class GalileoCamera: public QuaternionCamera {
public:
  GalileoCamera();
  %pickle_init(1);
};
}
