// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_temp_file.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::GeoCalTempFile);

namespace GeoCal {
class GeoCalTempFile : public GenericObject {
public:
  GeoCalTempFile();
  %python_attribute(temp_fname, std::string);
};
}

// List of things "import *" will include
%python_export("GeoCalTempFile",)
