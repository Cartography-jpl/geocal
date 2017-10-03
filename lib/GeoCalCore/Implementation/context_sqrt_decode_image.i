// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "context_sqrt_decode_image.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::ContextSqrtDecodeImage);

namespace GeoCal {
class ContextSqrtDecodeImage : public CalcRaster {
public:
  ContextSqrtDecodeImage(const boost::shared_ptr<RasterImage>& Raw_data);
  %python_attribute(raw_data, boost::shared_ptr<RasterImage>);
protected:
  virtual void calc(int Lstart, int Sstart) const = 0;
  %pickle_serialization()
};

}
