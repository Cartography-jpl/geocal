// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "raw_raster_image.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image)
%geocal_shared_ptr(GeoCal::RawRasterImage);

namespace GeoCal {
class RawRasterImage : public RasterImage {
public:
  RawRasterImage(const std::string& Fname, int Band = 0);
  static std::vector<boost::shared_ptr<RasterImage> > 
    read_all_bands(const std::string& Fname);
  %python_attribute(file_name, std::string)
  %python_attribute(band, int)
  %pickle_init(1, self.file_name, self.band)
};
}
// List of things "import *" will include
%python_export("RawRasterImage")
