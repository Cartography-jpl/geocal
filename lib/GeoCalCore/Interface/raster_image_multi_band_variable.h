#ifndef RASTER_IMAGE_MULTI_BAND_VARIABLE_H
#define RASTER_IMAGE_MULTI_BAND_VARIABLE_H
#include "raster_image_multi_band.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  A common implementation of RasterImageMultiBand is to just store a
  vector of RasterImage. This class gives the common behavior.
*******************************************************************/

class RasterImageMultiBandVariable : public RasterImageMultiBand {
public:
  RasterImageMultiBandVariable() { }
  RasterImageMultiBandVariable
  (const std::vector<boost::shared_ptr<RasterImage> >& Raster_image) 
    : raster_image_(Raster_image) 
  { }
  void add_raster_image(const boost::shared_ptr<RasterImage>& Ri)
  { raster_image_.push_back(Ri);}
  virtual ~RasterImageMultiBandVariable() {}
  virtual int number_band() const { return (int) raster_image_.size(); }
  virtual const RasterImage& raster_image(int band) const 
  { range_check(band, 0, number_band());
    return *(raster_image_[band]);
  }
  virtual RasterImage& raster_image(int band)
  { range_check(band, 0, number_band());
    return *(raster_image_[band]);
  }
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band) const
  {
    range_check(band, 0, number_band());
    return raster_image_[band];
  }
protected:
  std::vector<boost::shared_ptr<RasterImage> > raster_image_;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};


}

GEOCAL_EXPORT_KEY(RasterImageMultiBandVariable);

#endif

