#ifndef QUICK_MOSAIC_PROJECTED_H
#define QUICK_MOSAIC_PROJECTED_H
#include "calc_raster.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple mosaic of a set of RasterImages. The RasterImages
  should have a common projection and resolution, although in 
  general they will have difference bounding boxes.

  The data is calculated dynamically, using a simple algorithm of
  using the pixel value for the first RasterImage with a non-fill
  value. The RasterImages are checked in the order they appear in
  the input list, so if image A and B both have data but A comes
  before in the list then it is "on top" of B.

  No attempt is made to smooth edges or anything like that. The
  purpose of this class is for dynamically creating a mosiac (e.g, the
  RasterImages are themselves a CalcRaster like IgcMapProjected).
  If the images are actually files, you might want to use a existing
  program like fthfastmos or gdal_merge.py
*******************************************************************/

class QuickMosaic : public CalcRaster {
public:
  QuickMosaic(const MapInfo& Mi,
	      const std::vector<boost::shared_ptr<RasterImage> >& Img_list,
	      int Number_tile_line = -1,
	      int Number_tile_sample = -1,
	      double Fill_value = 0.0
	      );
  virtual ~QuickMosaic() {}

//-----------------------------------------------------------------------
/// Underlying image list
//-----------------------------------------------------------------------
  
  const std::vector<boost::shared_ptr<RasterImage> >& image_list() const
  {return img_list_;}
  double fill_value() const { return fill_value_;}
  virtual void print(std::ostream& Os) const;
protected:
  virtual void calc(int Lstart, int Sstart) const;
  QuickMosaic() {}
private:
  double fill_value_;
  std::vector<boost::shared_ptr<RasterImage> > img_list_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(QuickMosaic);
#endif
