// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "landsat7_global.h"
#include "image_ground_connection.h"
%}

%base_import(cart_lab_multifile)

%geocal_shared_ptr(GeoCal::Landsat7Global);
namespace GeoCal {
class Landsat7Global : public VicarCartLabMultifile {
public:
  enum Band { BAND1, BAND2, BAND3, BAND4, BAND5, BAND61, BAND62, BAND7, BAND8};
  Landsat7Global(const std::string& Dir, Band B);
  %python_attribute(band, Band);
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("Landsat7Global")
