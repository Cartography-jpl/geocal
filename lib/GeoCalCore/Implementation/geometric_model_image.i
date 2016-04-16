// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geometric_model_image.h"
%}
%base_import(calc_raster)
%import "geometric_model.i"
%geocal_shared_ptr(GeoCal::GeometricModelImage);

namespace GeoCal {
class GeometricModelImage : public CalcRaster {
public:
  GeometricModelImage(const boost::shared_ptr<RasterImage>& Data,
		      const boost::shared_ptr<GeometricModel>& Geom_model,
		      int Number_line, int Number_sample,
		      double Fill_value = 0.0);
  %python_attribute(raw_data, boost::shared_ptr<RasterImage>)
  %python_attribute(geometric_model, boost::shared_ptr<GeometricModel>)
  %python_attribute(fill_value, double)
  %pickle_init(1, self.raw_data, self.geometric_model, self.number_line,
	       self.number_sample, self.fill_value)
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

}
