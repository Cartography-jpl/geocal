// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "material_detect.h"
#include "raster_image_multi_band_variable.h"
#include "image_ground_connection.h"
%}
%base_import(calc_raster)
%import "raster_image_multi_band.i"
%geocal_shared_ptr(GeoCal::MaterialDetect);

namespace GeoCal {
class MaterialDetect : public CalcRaster {
public:
  MaterialDetect(const boost::shared_ptr<RasterImage>& Pan_data,
		 const boost::shared_ptr<RasterImage>& Pan_diff,
		 const boost::shared_ptr<RasterImageMultiBand>& Mulspect,
		 const blitz::Array<double, 1>& Pan_diff_threshold,
		 const blitz::Array<double, 1>& Spectral_diff_threshold,
		 const blitz::Array<int, 1>& Class_priority,
		 const std::string& Ibis_fname,
		 double Pan_shadow_threshold);
  %python_attribute(closest_material_raster_image,
		    boost::shared_ptr<RasterImage>)
  %python_attribute(material_raster_image,
		    boost::shared_ptr<RasterImageMultiBand>)
  boost::shared_ptr<RasterImage> material_raster_image(int Class_id) const;
  blitz::Array<double, 2> closest_material_dif(int Lstart, int Sstart, int Number_line,
					       int Number_sample) const;
  blitz::Array<double, 2> material_dif(int Lstart, int Sstart, int Number_line,
				       int Number_sample, int Class_id) const;
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};
}

// List of things "import *" will include
%python_export("MaterialDetect")
