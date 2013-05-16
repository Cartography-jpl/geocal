// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "frame_coordinate.h"
%}
%base_import(generic_object)

%geocal_shared_ptr(GeoCal::FrameCoordinate);

namespace GeoCal {

class FrameCoordinate : public GenericObject {
public:
  FrameCoordinate();
  FrameCoordinate(double L, double S);
  double line;
  double sample;
  std::string print_to_string() const;
  %pickle_init(1, self.line, self.sample)
};
}
