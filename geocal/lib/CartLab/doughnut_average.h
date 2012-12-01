#ifndef DOUGHNUT_AVERAGE_H
#define DOUGHNUT_AVERAGE_H
#include "calc_raster_multi_band.h"

namespace GeoCal {

/****************************************************************//**
  This class does a "doughnut average" of an underlying RasterImage.
  This produces the average value of all the pixels in a given window
  size, excluding a smaller doughnut. We exclude all points that a
  value of 0 in the average (e.g., the normal gore used in VICAR
  images), as well as all points that are beyond the edge of the
  image. We also preserve gore, so if a point has a value of 0 in any
  of the bands then we set the doughnut average to 0 for that point.

  Depending on the application, you may or may not want to include
  points that have gore within the doughnut window. You can specify
  this in the constructor. If "Allow_gore" is true, then any point
  other than the center can be gore and we just exclude it from the
  average. If "Allow_gore" is false, we set a value to 0 if any value
  in the window is 0. In all cases, we set a value to 0 if the center
  is 0.

  This is one of the building block used to do the "cvdnorm", you can
  consult that program for details. But basically this is used to
  examine small features (smaller than the doughnut whole) that are
  significantly different than the nearby background.
*******************************************************************/

class DoughnutAverage : public CalcRasterMultiBand {
public:
  DoughnutAverage(const boost::shared_ptr<RasterImageMultiBand>& Rimg, 
		  int Window_size = 11, int Doughnut_size = 5,
		  bool Allow_gore = true,
		  int Number_tile_line = -1, int Number_tile_sample = -1,
		  int Number_tile = 4);
  virtual ~DoughnutAverage() {}

  blitz::Array<double, 2> cvdnorm(int band, int Lstart, int Sstart,
				  int Number_line, int Number_sample) const;
  blitz::Array<double, 2> pandif(int Lstart, int Sstart,
				 int Number_line, int Number_sample) const;
  boost::shared_ptr<RasterImage> cvdnorm_raster_image(int band) const;
  boost::shared_ptr<RasterImage> pandif_raster_image() const;

//-----------------------------------------------------------------------
/// The window size.
//-----------------------------------------------------------------------

  int window_size() const {return whsize * 2 + 1;}

//-----------------------------------------------------------------------
/// The doughnut size.
//-----------------------------------------------------------------------
  int doughnut_size() const {return dhsize * 2 + 1;}

//-----------------------------------------------------------------------
/// If true, we allow gore in a window and just exclude it in the
/// average. Otherwise we set a point to 0 if any point in the window
/// is 0.
//-----------------------------------------------------------------------
  bool allow_gore() const {return allow_gore_;}

protected:
  virtual void calc(int Lstart, int Sstart) const; 
private:
  const boost::shared_ptr<RasterImageMultiBand> rimg;
  // This is the window and dougnut half size (it turns out this is a
  // more convenient number than the full window size).
  int whsize, dhsize;

  // If true, we allow gore in a window and just exclude it in the
  // average. Otherwise we set a point to 0 if any point in the window
  // is 0.
  bool allow_gore_;

  // The input data, with 0's propagated
  mutable blitz::Array<double, 3> input_data;
};
}
#endif

