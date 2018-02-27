// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "landsat7_global.h"
%}

%base_import(cart_lab_multifile)

%geocal_shared_ptr(GeoCal::Landsat7Global);
namespace GeoCal {
class Landsat7Global : public VicarCartLabMultifile {
public:
  enum Band {BAND62};
  Landsat7Global(const std::string& Dir, Band B);
  %python_attribute(band, Band);
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("Landsat7Global")
