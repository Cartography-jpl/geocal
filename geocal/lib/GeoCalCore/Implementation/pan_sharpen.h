#ifndef PAN_SHARPEN_H
#define PAN_SHARPEN_H
#include "calc_raster_multi_band.h"
#include "statistic.h"

namespace GeoCal {
  class PanSharpenHelper;
/****************************************************************//**
  This create a set of pan sharpened images from a pan band and a set
  of multispectral bands (at lower resolution).

  This uses the pan sharpening algorithm described in "WorldView-2
  Pan-Sharpening" by Chris Padwick et. al. (ASPRS 2010 Annual
  Conference).

  Because of the way this is calculated, it is most efficient to
  calculate all bands for a given tile at one time. So you should
  access all bands for a particular tile before going to the next
  tile for this object. Nothing bad happens if you don't, it is just
  slower to access.
*******************************************************************/

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
  virtual ~PanSharpen() {}

  // To help with debugging, these are the pieces that make up the pan
  // sharpen calculation. These pieces are subset to cover the same
  // area and are at the same resolution.
  boost::shared_ptr<RasterImage> psmooth, pansub;
  RasterImageMultiBandVariable mag, mulsub;
  Statistic psq_stat;
  Statistic isq_stat;
protected:
  virtual void calc(int Start_line, int Start_sample) const;
private:
  boost::shared_ptr<PanSharpenHelper> phelp;
  blitz::Array<double, 3> to_hcs(const blitz::Array<double, 3>& ncs) const;
  void to_ncs(const blitz::Array<double, 3>& hcs) const;
  void iadj(blitz::Array<double, 2> i, const blitz::Array<double, 2>& pan_data,
	    const blitz::Array<double, 2>& pansmooth_data) const;
  double sig0() const {return isq_stat.sigma(); }
  double sig1() const {return psq_stat.sigma();}
  double mu0() const {return isq_stat.mean();}
  double mu1() const {return psq_stat.mean();}
};
}
#endif
