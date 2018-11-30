// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "landsat7_panchromatic.h"
#include "image_ground_connection.h"
%}

%base_import(cart_lab_multifile)

%geocal_shared_ptr(GeoCal::Landsat7Panchromatic);
namespace GeoCal {
class Landsat7Panchromatic : public GdalCartLabMultifile {
public:
  Landsat7Panchromatic(const std::string& Dir);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("Landsat7Panchromatic")
