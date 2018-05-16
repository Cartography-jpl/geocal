// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "raster_subsample.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::RasterSubSample);

namespace GeoCal {
class RasterSubSample : public RasterImageVariable {
public:
  RasterSubSample(const boost::shared_ptr<RasterImage>& Data,
		 int Number_line_per_pixel, 
		 int Number_sample_per_pixel);
  %python_attribute2(high_resolution_image, high_resolution_image_ptr,
		     boost::shared_ptr<RasterImage>)
  %python_attribute(number_line_per_pixel, int)
  %python_attribute(number_sample_per_pixel, int)
  %pickle_init(1, self.high_resolution_image,
	       self.number_line_per_pixel, self.number_sample_per_pixel)
};

}
// List of things "import *" will include
%python_export("RasterSubSample")
