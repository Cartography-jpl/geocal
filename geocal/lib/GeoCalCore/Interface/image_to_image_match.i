// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "image_to_image_match.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::ImageToImageMatch);
namespace GeoCal {

class ImageToImageMatch : public GenericObject {
public:
  virtual void match(const ImageCoordinate& Ic1, 
		     ImageCoordinate &OUTPUT,
		     double &OUTPUT, double &OUTPUT,
		     bool &OUTPUT, int *OUTPUT) const = 0;  
  std::string print_to_string() const;
};
}



