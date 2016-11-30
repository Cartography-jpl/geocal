// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "frame_coordinate.h"
%}
%base_import(generic_object)
%import "auto_derivative.i"
%geocal_shared_ptr(GeoCal::FrameCoordinate);
%geocal_shared_ptr(GeoCal::FrameCoordinateWithDerivative);

namespace GeoCal {

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) FrameCoordinate &OUTPUT (GeoCal::FrameCoordinate temp) {
  $1 = &temp;
 }

 %typemap(in,numinputs=0) FrameCoordinateWithDerivative &OUTPUT (GeoCal::FrameCoordinateWithDerivative temp) {
  $1 = &temp;
 }

 %typemap(argout) FrameCoordinate &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::FrameCoordinate> *), SWIG_POINTER_OWN));
 }

 %typemap(argout) FrameCoordinateWithDerivative &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::FrameCoordinateWithDerivative> *), SWIG_POINTER_OWN));
 }
#endif

class FrameCoordinate : public GenericObject {
public:
  FrameCoordinate();
  FrameCoordinate(double L, double S);
  double line;
  double sample;
  std::string print_to_string() const;
  %pickle_serialization();
};

class FrameCoordinateWithDerivative : public GenericObject {
public:
  FrameCoordinateWithDerivative();
  FrameCoordinateWithDerivative(AutoDerivative<double> L, 
				AutoDerivative<double> S);
  FrameCoordinateWithDerivative(const FrameCoordinate& F);
  AutoDerivative<double> line;
  AutoDerivative<double> sample;
  std::string print_to_string() const;
  %python_attribute(value, FrameCoordinate);
  %pickle_serialization();
};
}
