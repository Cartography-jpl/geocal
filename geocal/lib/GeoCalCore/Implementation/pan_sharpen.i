// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "pan_sharpen.h"
%}

%geocal_shared_ptr(GeoCal::PanSharpen);

namespace GeoCal {
class PanSharpen : public CalcRasterMultiBand {
public:
  PanSharpen(const boost::shared_ptr<RasterImage>& Pan,
	     const RasterImageMultiBand& Mul,
	     bool Force_rpc = false,
	     bool Log_progress = true,
	     const boost::shared_ptr<RasterImage>& Pan_overview =
	     boost::shared_ptr<RasterImage>(),
	     const boost::shared_ptr<RasterImageMultiBand>& Mul_overview =
	     boost::shared_ptr<RasterImageMultiBand>());
  boost::shared_ptr<RasterImage> psmooth, pansub;
  RasterImageMultiBandVariable mag, mulsub;
  Statistic psq_stat;
  Statistic isq_stat;
protected:
  virtual void calc(int Start_line, int Start_sample) const;
};
}
