#ifndef MAP_REPROJECTED_IMAGE_H
#define MAP_REPROJECTED_IMAGE_H
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  This is a RasterImage that has been resampled to a different
  map projection and/or a different resolution.

  To do this we do two steps:

  1. We calculate roughly what the difference in resolution is between
     the original and final MapInfo. We do this by looking at the
     center pixel of the original data and the pixel +1 in line and
     sample. We then use RasterAveraged to average the original data
     to roughly the resolution of the final MapInfo. If the final
     MapInfo is near the same resolution as the original, or if it has
     a higher resolution, then we don't do any averaging.

  2. We then interpolate the possibly averaged data to the final
     projection.

  It is ok if the final MapInfo contains areas outside of the original
  data. For any pixel outside of the original data, we just return a
  value of 0.

  This class calculates the reprojected data on the fly. Sometimes
  this is what you want, but if you are going to be using the
  resulting data a few times, you may want to use a
  MemoryRasterImage to generate a copy once and keep it in
  memory.
*******************************************************************/

class MapReprojectedImage : public RasterImageVariable {
public:
  MapReprojectedImage(const boost::shared_ptr<RasterImage> Img_in,
		      const MapInfo& Mi, int Line_avg = -1, int Sample_avg = -1);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MapReprojectedImage() {}

  virtual int unchecked_read(int Line, int Sample) const
  { return (int) unchecked_read_double(Line, Sample); }
  virtual double unchecked_read_double(int Line, int Sample) const;
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual void unchecked_write(int Line, int Sample, int Val);
  virtual void unchecked_write(int Line, int Sample, double Val);

  virtual void print(std::ostream& Os) const;
  boost::shared_ptr<RasterImage> raw_image_ptr() const {return img;}
private:
  boost::shared_ptr<RasterImage> img;
                               ///< Underlying data, possibly averaged
                               ///using RasterAveraged.
};

}
#endif
