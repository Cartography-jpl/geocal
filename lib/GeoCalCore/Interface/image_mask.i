// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "image_mask.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::ImageMask);
%geocal_shared_ptr(GeoCal::OffsetImageMask);
%geocal_shared_ptr(GeoCal::CombinedImageMask);
namespace GeoCal {
class ImageMask: public GenericObject {
public:
  virtual bool mask(int Line, int Sample) const = 0;
  virtual bool mask_ic(const ImageCoordinate& Ic) const;
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;
  std::string print_to_string() const;
  %pickle_serialization();
};

class OffsetImageMask: public ImageMask {
public:
  OffsetImageMask(const boost::shared_ptr<ImageMask> Im_original,
		  double Line_offset, double Sample_offset);

  %python_attribute(original_image_mask, boost::shared_ptr<ImageMask>);
  %python_attribute(line_offset, double);
  %python_attribute(sample_offset, double);
  %pickle_serialization();
};
  
class CombinedImageMask: public ImageMask {
public:
  CombinedImageMask();
  CombinedImageMask(const std::vector<boost::shared_ptr<ImageMask> >& Ml);
  virtual bool mask(int Line, int Sample) const;
  std::vector<boost::shared_ptr<ImageMask> > mask_list;
  %pickle_serialization();
};

}
%template(Vector_ImageMask) std::vector<boost::shared_ptr<GeoCal::ImageMask> >;

