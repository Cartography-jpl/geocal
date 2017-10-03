#ifndef CONTEXT_SQRT_DECODE_IMAGE_H
#define CONTEXT_SQRT_DECODE_IMAGE_H
#include "calc_raster.h"
namespace GeoCal {
/****************************************************************//**
  The Mars Context camera uses sqrt encoding. This is described in the
  Context documentation "Software Interface Specification Context
  Camera (CTX) Standard Data Product" by M. Caplinger (available at
  https://pds-imaging.jpl.nasa.gov/data/mro/mars_reconnaissance_orbiter/ctx/mrox_2628/document,
  file ctxsis.pdf).

  This class handles decoding this data. This is a simple table 
  substitution.
*******************************************************************/
class ContextSqrtDecodeImage : public CalcRaster {
public:
  ContextSqrtDecodeImage(const boost::shared_ptr<RasterImage>& Raw_data)
    : CalcRaster(Raw_data->number_line(), Raw_data->number_sample(),
		 Raw_data->number_tile_line(), Raw_data->number_tile_sample()),
      raw_data_(Raw_data)
  {
  }
  virtual ~ContextSqrtDecodeImage() {}
  const boost::shared_ptr<RasterImage>& raw_data() const {return raw_data_;}
  virtual void print(std::ostream& Os) const { Os << "ContextSqrtDecodeImage"; }
protected:
  virtual void calc(int Lstart, int Sstart) const; 
private:
  boost::shared_ptr<RasterImage> raw_data_;
  ContextSqrtDecodeImage() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  static int data_value[256];
};
}
GEOCAL_EXPORT_KEY(ContextSqrtDecodeImage);
#endif
  
