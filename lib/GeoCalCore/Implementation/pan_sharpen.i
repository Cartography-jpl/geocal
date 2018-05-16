// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "pan_sharpen.h"
%}
%base_import(calc_raster_multi_band)
%import "statistic.i"
%geocal_shared_ptr(GeoCal::PanSharpen);

namespace GeoCal {
class PanSharpen : public CalcRasterMultiBand {
public:
  PanSharpen(const boost::shared_ptr<RasterImage>& Pan,
	     const boost::shared_ptr<RasterImageMultiBand>& Mul,
	     bool Force_rpc = false,
	     bool Log_progress = true);
protected:
  virtual void calc(int Start_line, int Start_sample) const;
};
}

// List of things "import *" will include
%python_export("PanSharpen")
