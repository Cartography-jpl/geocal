#ifndef IGC_SIMULATED_H
#define IGC_SIMILATED_H
#include "image_ground_connection.h"
#include "calc_raster.h"
namespace GeoCal {
/****************************************************************//**
  This is a RasterImage that is a simulated based on an underlying 
  map projected image. For each point in the image of an
  ImageGroundConnection, we calculate where that point appears on the
  ground and then assign a value to it from the map projected image. 

  This is essentially the inverse of IgcMapProjected, if you replace
  the image of an ImageGroundConnection with this simulated image and
  then run IgcMapProjected you'll get the original map projected image
  back (with changes from being resampled twice). This is similar to
  the old MisrSim program.

  We do this in two steps:

  1. We calculate roughly what the difference in resolution is between
     the original data and the image we are generating. We do this by
     looking at the center pixel of the original data and the pixel +1
     in line and sample. We then use RasterAveraged to average the
     original data to roughly the resolution of the final image. If
     the final image is near the same resolution as the original, or
     if it has a higher resolution, then we don't do any averaging.
     Alternatively, you can pass in the averaging factor (include a
     value of 1 which turns this behavior off).

  2. We then interpolate the possibly averaged data to the final
     image. 

  This class calculates the data on the fly. Sometimes
  this is what you want, but if you are going to be using the
  resulting data a few times, you may want to use a
  MemoryRasterImage to generate a copy once and keep it in
  memory.
*******************************************************************/

class IgcSimulated : public CalcRaster {
public:
  IgcSimulated(const boost::shared_ptr<ImageGroundConnection>& Igc,
	       const boost::shared_ptr<RasterImage>& Map_projected_image, 
	       int Avg_fact = -1,
	       bool Read_into_memory = true,
	       int Number_tile_line = -1,
	       int Number_tile_sample = -1,
	       double Fill_value = 0.0
	       );
  virtual ~IgcSimulated() {}
  virtual void print(std::ostream& Os) const;
  const boost::shared_ptr<ImageGroundConnection>& igc() const
  { return igc_; }
  int avg_factor() const { return avg_factor_; }
  bool read_into_memory() const { return read_into_memory_; }
  double fill_value() const {return fill_value_;}
  const boost::shared_ptr<RasterImage>& map_projected_image() const
  { return img_; }
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<ImageGroundConnection> igc_;
  int avg_factor_;
  double fill_value_;
  bool read_into_memory_;
  boost::shared_ptr<RasterImage> img_;
  boost::shared_ptr<RasterImage> img_avg_;
};
}
#endif
