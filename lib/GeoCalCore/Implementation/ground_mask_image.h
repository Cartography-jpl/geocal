#ifndef GROUND_MASK_IMAGE_H
#define GROUND_MASK_IMAGE_H
#include "ground_mask.h"
#include "raster_image.h"

namespace GeoCal {
/****************************************************************//**
  This provides a GroundMask where the underlying data is a
  RasterImage. 

  We use the underlying map projection of the RasterImage we
  defining a region. 

  For any point, we look at the four neighboring pixels. If any of the
  pixels are the mask_value, we say the point is masked, otherwise it
  is not masked. If we are outside of the RasterImage, then we
  say it is not masked or masked based on the flag passed in the
  constructor. 

  An example of this kind of Mask is the Land/Water mask used by
  Vicar ("world_30as_lwm.img")
*******************************************************************/

class GroundMaskImage : public GroundMask {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  GroundMaskImage(boost::shared_ptr<RasterImage> Img, int Mask_value = 0,
		  bool Outside_is_masked = false)
    : img(Img), maskv(Mask_value), outside_is_masked_(Outside_is_masked) 
  {
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GroundMaskImage() {}

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

//-----------------------------------------------------------------------
/// Indicated if outside of the image is masked or not.
//-----------------------------------------------------------------------

  bool outside_is_masked() const {return outside_is_masked_; }

  virtual bool mask(const GroundCoordinate& Gc) const;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const;
  virtual void print(std::ostream& Os) const;
protected:
  GroundMaskImage() {}
private:
  boost::shared_ptr<RasterImage> img; ///< Underlying image.
  int maskv;				    ///< Value that indicates
					    ///a masked point.
  bool outside_is_masked_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(GroundMaskImage);
#endif
