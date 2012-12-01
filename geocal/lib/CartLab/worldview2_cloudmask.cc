#include "worldview2_cloudmask.h"
#include "magnify_replicate.h"
#include "raster_averaged.h"
using namespace GeoCal;
using namespace blitz;

// This is the helper class that handles the cloud calculation once we
// have averaged the data.

class WorldView2CloudMaskHelper : public CalcRaster {
public:
  WorldView2CloudMaskHelper(const boost::shared_ptr<RasterImage>& Band_1,
			    const boost::shared_ptr<RasterImage>& Band_8,
			    double Cloud_threshold, double Shadow_threshold)
    : CalcRaster(*Band_1), band_1(Band_1), 
      band_8(Band_8), cloud_threshold(Cloud_threshold),
      shadow_threshold(Shadow_threshold)
  { }
  virtual ~WorldView2CloudMaskHelper() {}
  virtual void calc(int Lstart, int Sstart) const
  {
    Array<double, 2> b1(band_1->read_double(Lstart, Sstart, data.rows(),
					    data.cols()));
    Array<double, 2> b8(band_8->read_double(Lstart, Sstart, data.rows(),
					    data.cols()));
    data = where(b1 > cloud_threshold, (double) WorldView2CloudMask::CLOUDY,
		 where(b1 > 0 && (b8 - b1) / b1 < shadow_threshold,
		       (double) WorldView2CloudMask::SHADOW,
		       (double) WorldView2CloudMask::CLEAR));
  }
private:
  boost::shared_ptr<RasterImage> band_1, band_8;
  double cloud_threshold, shadow_threshold;
};

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

WorldView2CloudMask::WorldView2CloudMask(
const RasterImageMultiBand&
Mul_spec_data, int Min_cloud_size,
double Cloud_threshold, double Shadow_threshold)
: CalcRaster(Mul_spec_data.raster_image(0))
{
  boost::shared_ptr<RasterImage> b1avg
    (new RasterAveraged(Mul_spec_data.raster_image_ptr(0), Min_cloud_size, 
			Min_cloud_size, true));
  boost::shared_ptr<RasterImage> b8avg
    (new RasterAveraged(Mul_spec_data.raster_image_ptr(7), Min_cloud_size, 
			Min_cloud_size, true));
  boost::shared_ptr<RasterImage> h
    (new WorldView2CloudMaskHelper(b1avg, b8avg, Cloud_threshold, 
				   Shadow_threshold));
  cloud_data.reset(new MagnifyReplicate(h, Min_cloud_size));
}

void WorldView2CloudMask::calc(int Lstart, int Sstart) const
{
  // Need to be careful of edges, because when we average and then
  // magnify the image it can be smaller than the original image.
  data = CLEAR;
  if(Lstart >= cloud_data->number_line() - 1 ||
     Sstart >= cloud_data->number_sample() - 1)
    return;
  Range r1(0, std::min(data.rows(), cloud_data->number_line() - Lstart) - 1);
  Range r2(0, std::min(data.cols(), cloud_data->number_sample() - Sstart) - 1);
  data(r1, r2) = 
    cloud_data->read_double(Lstart, Sstart, r1.length(), r2.length());
}
