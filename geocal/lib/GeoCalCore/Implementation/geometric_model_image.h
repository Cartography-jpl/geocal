#ifndef GEOMETRIC_MODEL_IMAGE_H
#define GEOMETRIC_MODEL_IMAGE_H
#include "calc_raster.h"
#include "geometric_model.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple adapter that applies a geometric model to an
  underlying image.

  This purpose of this kind of a class is to resample an image in
  such a way that it matches a first image (so for sample image 2 is
  image 1 rotated a bit and scaled - we apply the opposite to have
  image 2 match image 1).
*******************************************************************/

class GeometricModelImage : public CalcRaster {
public:
//-----------------------------------------------------------------------
/// Constructor. This takes underlying data, and a geometric model to
/// use to resample it.
///
/// Because we fill in data outside of the original image with O's
/// this image can be any size. So the size desired needs to be passed
/// in. 
//-----------------------------------------------------------------------

  GeometricModelImage(const boost::shared_ptr<RasterImage>& Data,
		      const boost::shared_ptr<GeometricModel>& Geom_model,
		      int Number_line, int Number_sample)
    : CalcRaster(Number_line, Number_sample), raw_data_(Data),
      model(Geom_model) {}
  virtual ~GeometricModelImage() {}
  virtual void print(std::ostream& Os) const;
  const boost::shared_ptr<RasterImage>& raw_data() const {return raw_data_;}
  const boost::shared_ptr<GeometricModel>& geometric_model() const 
  {return model;}
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImage> raw_data_;
  boost::shared_ptr<GeometricModel> model;
};
}
#endif
