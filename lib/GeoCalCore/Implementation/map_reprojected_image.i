// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "map_reprojected_image.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::MapReprojectedImage);
namespace GeoCal {
class MapReprojectedImage : public RasterImageVariable {
public:
  MapReprojectedImage(const boost::shared_ptr<RasterImage> Img_in,
		      const MapInfo& Mi);
  %python_attribute2(raw_image, raw_image_ptr, boost::shared_ptr<RasterImage>)
  %pickle_init(1, self.raw_image, self.map_info)
};
}
// List of things "import *" will include
%python_export("MapReprojectedImage")
