// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "sub_raster_image_multi_band.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image_multi_band_variable)
%geocal_shared_ptr(GeoCal::SubRasterImageMultiBand);
namespace GeoCal {
class SubRasterImageMultiBand : public RasterImageMultiBandVariable {
public:
  SubRasterImageMultiBand(const boost::shared_ptr<RasterImageMultiBand>& Rmb,
			  int Start_line, int Start_sample,
			  int Number_line, int Number_sample);
  %python_attribute(start_line, int)
  %python_attribute(start_sample, int)
  %python_attribute(full_raster_image_multi_band, 
		    boost::shared_ptr<RasterImageMultiBand>)
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("SubRasterImageMultiBand")
