#ifndef CONSTANT_RASTER_IMAGE_H
#define CONSTANT_RASTER_IMAGE_H
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple implementation of a RasterImage, where the data is 
  just a constant fixed value.
*******************************************************************/

class ConstantRasterImage : public RasterImageVariable {
public:
//-----------------------------------------------------------------------
/// Construct a ConstantRasterImage of the given size.
//-----------------------------------------------------------------------

  ConstantRasterImage(int Number_line, int Number_sample, int Value)
    : RasterImageVariable(Number_line, Number_sample),
      data_(Value)
    {
    }

//-----------------------------------------------------------------------
/// Construct a MemoryRasterImage of the given MapInfo.
//-----------------------------------------------------------------------

  ConstantRasterImage(const MapInfo& Mi, int Value)
    : RasterImageVariable(Mi),
      data_(Value)
  { }

  virtual ~ConstantRasterImage() {}

  virtual int unchecked_read(int Line, int Sample) const
  {
    return data_;
  }
  virtual double unchecked_read_double(int Line, int Sample) const
  {
    return (double) data_;
  }
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
  {
    range_check(Lstart, 0, number_line() - (Number_line - 1));
    range_check(Sstart, 0, number_sample() - (Number_sample - 1));
    for(int i = Lstart; i < Lstart + Number_line; ++i)
      for(int j = Sstart; j < Sstart + Number_sample; ++j, ++Res)
	*Res = data_;
  }
  virtual void unchecked_write(int Line, int Sample, int Val)
  {
    throw Exception("Not implemented");
  }
  virtual void unchecked_write(int Line, int Sample, double Val)
  {
    throw Exception("Not implemented");
  }
  virtual void print(std::ostream& Os) const
  {
    Os << "RasterImage of " << number_line() << " x " << number_sample() 
       << " constant value of " << data_ << "\n";
  }
private:
  int data_;			    /// <Underlying data.
  ConstantRasterImage() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(ConstantRasterImage);
#endif
