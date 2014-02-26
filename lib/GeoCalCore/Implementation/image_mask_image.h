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
  enum OutsideHandling { OUTSIDE_MASKED, OUTSIDE_NOT_MASKED, OUTSIDE_ERROR };
//-----------------------------------------------------------------------
/// Constructor.
///
/// Values outside of the Image may or may not want to be marked as
/// masked. 
//-----------------------------------------------------------------------

  ImageMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0,
		 OutsideHandling Handling = OUTSIDE_MASKED)
    : img(Img), maskv(Mask_value), oh(Handling)
  {
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageMaskImage() {}

//-----------------------------------------------------------------------
/// Handling for data outside of image.
//-----------------------------------------------------------------------
  OutsideHandling outside_handling() const {return oh; }
//-----------------------------------------------------------------------
/// Underlying RasterImage.
//-----------------------------------------------------------------------

  const RasterImage& raster_image() const {return *img;}

//-----------------------------------------------------------------------
/// Underlying RasterImage.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImage>& 
  raster_image_ptr() const {return img;}

  void unmasked_bounding_box(int& Minline, int &Minsample, int &Numberline,
			     int &Numbersample) const;

//-----------------------------------------------------------------------
/// Value indicating masked data.
//-----------------------------------------------------------------------

  int masked_value() const {return maskv;}
  virtual bool mask(int Line, int Sample) const;
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<RasterImage> img; ///< Underlying image.
  int maskv;				    ///< Value that indicates
					    ///a masked point.
  OutsideHandling oh;
};
}
#endif
