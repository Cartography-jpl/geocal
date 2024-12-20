// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "paint_class.h"
#include "image_ground_connection.h"
%}
%base_import(calc_raster_multi_band)
%geocal_shared_ptr(GeoCal::PaintClass);

namespace GeoCal {
class PaintClass : public CalcRasterMultiBand {
public:
  PaintClass(const boost::shared_ptr<RasterImage>& Class_id_img,
	     const boost::shared_ptr<RasterImage>& Pan_img,
	     const blitz::Array<int, 2>& Color,
	     int Class_id_first_color);
  %pickle_serialization();
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

}
// List of things "import *" will include
%python_export("PaintClass")
