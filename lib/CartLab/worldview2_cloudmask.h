#ifndef WORLDVIEW2_CLOUDMASK_H
#define WORLDVIEW2_CLOUDMASK_H
#include "raster_image_multi_band.h"
#include "calc_raster.h"
namespace GeoCal {
/****************************************************************//**
  This create a cloud mask using WorldView2 mulispectral data. This
  cloud mask has two parts:

  1. A simple threshold on the coastal blue band to detect clouds.
  2. A ratio comparision of (band 8 - band 1) / band 1 < threshold
     to detect cloud shadows.

  To help with false positives, we have a minimum cloud size. This 
  is used to average the data, and only if the lower resolution data
  shows a cloud or a shadow do we mark a pixel as cloudy or in shadow.

  We use reflectance data. It can be convenient to scale the
  reflectance data (e.g., go from 0 to 32767 to give half word VICAR
  data). This class doesn't care, as long as the Cloud_threshold is
  scaled the same way.
*******************************************************************/
class WorldView2CloudMask : public CalcRaster {
public:
  enum {CLEAR = 0, SHADOW = 1, CLOUDY = 2};
  WorldView2CloudMask(const RasterImageMultiBand&
		      Mul_spec_data, int Min_cloud_size,
		      double Cloud_threshold, double Shadow_threshold);
  virtual ~WorldView2CloudMask() {}
protected:
  virtual void calc(int Lstart, int Sstart) const;
  WorldView2CloudMask() {};
private:
  boost::shared_ptr<RasterImage> cloud_data;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(WorldView2CloudMask);
#endif
