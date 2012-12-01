// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "frame_coordinate.h"
%}

%geocal_shared_ptr(FrameCoordinate);

namespace GeoCal {

class FrameCoordinate {
public:
  FrameCoordinate();
  FrameCoordinate(double L, double S);
  double line;
  double sample;
  std::string print_to_string() const;
  %pickle_init(self.line, self.sample)
};
}
