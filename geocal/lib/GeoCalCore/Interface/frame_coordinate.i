// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "frame_coordinate.h"
%}

%geocal_shared_ptr(GeoCal::FrameCoordinate);

namespace GeoCal {

class FrameCoordinate {
public:
  FrameCoordinate();
  FrameCoordinate(double L, double S);
  double line;
  double sample;
  std::string print_to_string() const;
  %pickle_init(1, self.line, self.sample)
};
}
