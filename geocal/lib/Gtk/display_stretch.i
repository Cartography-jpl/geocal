// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "display_stretch.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::DisplayStretch);

namespace GeoCal {
class DisplayStretch : public GenericObject {
public:
  std::string print_to_string() const;
  virtual void stretch(const blitz::Array<int, 2>& Raw_data, 
       blitz::Array<unsigned char, 2>& Stretched_data) const  = 0;
};
}
