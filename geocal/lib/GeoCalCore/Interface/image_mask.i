// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "image_mask.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::ImageMask);
%geocal_shared_ptr(GeoCal::CombinedImageMask);
namespace GeoCal {
class ImageMask: public GenericObject {
public:
  virtual bool mask(int Line, int Sample) const = 0;
  virtual bool mask_ic(const ImageCoordinate& Ic) const;
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;
  std::string print_to_string() const;
};

class CombinedImageMask: public ImageMask {
public:
  CombinedImageMask();
  CombinedImageMask(const std::vector<boost::shared_ptr<ImageMask> >& Ml);
  virtual bool mask(int Line, int Sample) const;
  std::vector<boost::shared_ptr<ImageMask> > mask_list;
  %pickle_init(1, self.mask_list)
};

}
%template(Vector_ImageMask) std::vector<boost::shared_ptr<GeoCal::ImageMask> >;

%extend std::vector<boost::shared_ptr<GeoCal::ImageMask> > {
  %pickle_init(1, list(self))
};
