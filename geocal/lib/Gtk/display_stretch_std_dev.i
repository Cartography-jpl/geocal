// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "display_stretch_std_dev.h"
%}

%geocal_markfunc(DisplayStretchStdDev);
%geocal_shared_ptr(DisplayStretchStdDev);
namespace GeoCal {
class DisplayStretchStdDev : public DisplayStretch {
public:
  DisplayStretchStdDev(double Min_sig = -2.0, double Max_sig = 2.0);
  virtual void stretch(const blitz::Array<int, 2>& Raw_data, 
       blitz::Array<unsigned char, 2>& Stretched_data) const;
};
}
