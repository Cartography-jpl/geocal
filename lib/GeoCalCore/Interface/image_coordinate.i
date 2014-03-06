// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "image_coordinate.h"
%}

%base_import(generic_object)
%geocal_shared_ptr(GeoCal::ImageCoordinate);
%geocal_shared_ptr(GeoCal::VicarImageCoordinate);
namespace GeoCal {

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) ImageCoordinate &OUTPUT (GeoCal::ImageCoordinate temp) {
  $1 = &temp;
 }

 %typemap(argout) ImageCoordinate &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::ImageCoordinate> *), SWIG_POINTER_OWN));
 }
#endif

  class VicarImageCoordinate;
  class ImageCoordinate : public GenericObject {
  public:
    ImageCoordinate(double Line, double Sample);
    ImageCoordinate(const VicarImageCoordinate& Vic);
    ImageCoordinate();
    double line;
    double sample;
    std::string print_to_string() const;
    %pickle_init(1, self.line, self.sample)
  };
  class VicarImageCoordinate : public GenericObject {
  public:
    VicarImageCoordinate(double Line, double Sample);
    VicarImageCoordinate(const ImageCoordinate& Vic);
    VicarImageCoordinate();
    double line;
    double sample;
    std::string print_to_string() const;
    %pickle_init(1, self.line, self.sample)
  };

}

%template(Vector_ImageCoordinate) std::vector<GeoCal::ImageCoordinate>;
%template(Vector_ImageCoordinatePtr) std::vector<boost::shared_ptr<GeoCal::ImageCoordinate> >;