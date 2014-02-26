// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "sub_raster_image.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::SubRasterImage);
namespace GeoCal {
class SubRasterImage : public RasterImageVariable {
public:
  SubRasterImage(const boost::shared_ptr<RasterImage>& Raster_image,
		 int Start_line, int Start_sample,
		 int Number_line, int Number_sample);
  SubRasterImage(const boost::shared_ptr<RasterImage>& Raster_image,
     const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
     int boundary = 0);
  %python_attribute(start_line, int)
  %python_attribute(start_sample, int)
  %python_attribute2(full_raster_image, full_raster_image_ptr, boost::shared_ptr<RasterImage>)
  %pickle_init(1, self.full_raster_image,
	       self.start_line, self.start_sample,
	       self.number_line, self.number_sample)
};
}
