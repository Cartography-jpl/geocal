// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "raw_raster_image.h"
%}
%base_import(raster_image)
%geocal_shared_ptr(GeoCal::RawRasterImage);

namespace GeoCal {
class RawRasterImage : public RasterImage {
public:
  RawRasterImage(const std::string& Fname, int Band = 0);
  static std::vector<boost::shared_ptr<RasterImage> > 
    read_all(const std::string& Fname);
  %python_attribute(file_name, std::string)
  %python_attribute(band, int)
  %pickle_init(1, self.file_name, self.band)
};
}