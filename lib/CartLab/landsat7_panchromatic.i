// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "landsat7_panchromatic.h"
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
