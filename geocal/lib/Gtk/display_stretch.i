// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "display_stretch.h"
%}

%geocal_markfunc(DisplayStretch);
%geocal_shared_ptr(DisplayStretch);

namespace GeoCal {
class DisplayStretch {
public:
  std::string print_to_string() const;
  virtual void stretch(const blitz::Array<int, 2>& Raw_data, 
       blitz::Array<unsigned char, 2>& Stretched_data) const  = 0;
};
}
