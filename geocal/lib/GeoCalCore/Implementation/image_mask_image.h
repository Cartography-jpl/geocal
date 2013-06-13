#ifndef IMAGE_MASK_IMAGE_H
#define IMAGE_MASK_IMAGE_H
#include "image_mask.h"
#include "raster_image.h"

namespace GeoCal {
/****************************************************************//**
  This provides a ImageMask where the underlying data is a
  RasterImage. 
*******************************************************************/

class ImageMaskImage : public ImageMask {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  ImageMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0)
    : img(Img), maskv(Mask_value) 
  {
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageMaskImage() {}

//-----------------------------------------------------------------------
/// Underlying RasterImage.
//-----------------------------------------------------------------------

  const RasterImage& raster_image() const {return *img;}

//-----------------------------------------------------------------------
/// Underlying RasterImage.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImage>& 
  raster_image_ptr() const {return img;}

//-----------------------------------------------------------------------
/// Value indicating masked data.
//-----------------------------------------------------------------------

  int masked_value() const {return maskv;}
  virtual bool mask(int Line, int Sample) const
  {
    return (*img)(Line, Sample) == maskv;
  }
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<RasterImage> img; ///< Underlying image.
  int maskv;				    ///< Value that indicates
					    ///a masked point.
};
}
#endif
