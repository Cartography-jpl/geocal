// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "frame_coordinate.h"
%}
%base_import(generic_object)

%geocal_shared_ptr(GeoCal::FrameCoordinate);

namespace GeoCal {

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) FrameCoordinate &OUTPUT (GeoCal::FrameCoordinate temp) {
  $1 = &temp;
 }

 %typemap(argout) FrameCoordinate &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::FrameCoordinate> *), SWIG_POINTER_OWN));
 }
#endif

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
