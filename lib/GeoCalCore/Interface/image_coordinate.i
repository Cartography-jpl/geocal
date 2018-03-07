// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "image_coordinate.h"
%}

%base_import(generic_object)
%import "auto_derivative.i"
%geocal_shared_ptr(GeoCal::ImageCoordinate);
%geocal_shared_ptr(GeoCal::ImageCoordinateWithDerivative);
%geocal_shared_ptr(GeoCal::VicarImageCoordinate);
namespace GeoCal {

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) ImageCoordinate &OUTPUT (GeoCal::ImageCoordinate temp) {
  $1 = &temp;
 }

 %typemap(argout) ImageCoordinate &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::ImageCoordinate> *), SWIG_POINTER_OWN));
 }

 %typemap(in,numinputs=0) ImageCoordinate &OUTPUT2 (GeoCal::ImageCoordinate temp) {
  $1 = &temp;
 }

 %typemap(argout) ImageCoordinate &OUTPUT2 {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::ImageCoordinate> *), SWIG_POINTER_OWN));
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
    %pickle_serialization();
  };
  class VicarImageCoordinate : public GenericObject {
  public:
    VicarImageCoordinate(double Line, double Sample);
    VicarImageCoordinate(const ImageCoordinate& Vic);
    VicarImageCoordinate();
    double line;
    double sample;
    std::string print_to_string() const;
    %pickle_serialization();
  };
  class ImageCoordinateWithDerivative : public GenericObject {
  public:
    ImageCoordinateWithDerivative(const AutoDerivative<double>& Line, 
				  const AutoDerivative<double>& Sample);
    ImageCoordinateWithDerivative();
    AutoDerivative<double> line;
    AutoDerivative<double> sample;
    ImageCoordinate value() const;
    std::string print_to_string() const;
    %pickle_serialization();
  };
}

%template(Vector_ImageCoordinate) std::vector<GeoCal::ImageCoordinate>;
%template(Vector_ImageCoordinatePtr) std::vector<boost::shared_ptr<GeoCal::ImageCoordinate> >;
// List of things "import *" will include
%python_export("ImageCoordinate", "VicarImageCoordinate", "ImageCoordinateWithDerivative", "Vector_ImageCoordinate", "Vector_ImageCoordinatePtr")
