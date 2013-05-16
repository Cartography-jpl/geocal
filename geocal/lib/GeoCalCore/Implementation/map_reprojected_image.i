// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "map_reprojected_image.h"
%}

%geocal_shared_ptr(GeoCal::MapReprojectedImage);
namespace GeoCal {
class MapReprojectedImage : public RasterImageVariable {
public:
  MapReprojectedImage(const boost::shared_ptr<RasterImage> Img_in,
		      const MapInfo& Mi);
  %python_attribute2(raw_image, raw_image_ptr, boost::shared_ptr<RasterImage>)
  virtual void write(int Line, int Sample, int Val);
  %pickle_init(1, self.raw_image, self.map_info)
};
}
