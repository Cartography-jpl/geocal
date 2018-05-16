// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("GalileoCamera")
