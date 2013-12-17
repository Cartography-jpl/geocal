// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "geometric_model.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::GeometricModel);

namespace GeoCal {
%nodefaultctor GeometricModel;

class GeometricModel: public GenericObject {
public:
  RasterImageTileIterator(const RasterImage& Ri);
  virtual ImageCoordinate image_coordinate(const ImageCoordinate& Resampled_Ic)
    const;
  std::string print_to_string() const;
};

}

