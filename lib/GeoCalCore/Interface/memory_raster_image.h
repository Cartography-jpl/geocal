#ifndef MEMORY_RASTER_IMAGE_H
#define MEMORY_RASTER_IMAGE_H
#include "raster_image_variable.h"
#include <blitz/array.h>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This is a simple implementation of a RasterImage, where the data is 
  just stored in a 2D array. 

  This is useful both for testing and to read all of another
  RasterImage into memory.

  Note that this stores data as int type, so floating point data gets
  truncated to int.
*******************************************************************/

class MemoryRasterImage : public RasterImageVariable {
public:
//-----------------------------------------------------------------------
/// Construct a MemoryRasterImage of the given size.
//-----------------------------------------------------------------------

  MemoryRasterImage(int Number_line = 0, int Number_sample = 0)
    : RasterImageVariable(Number_line, Number_sample),
      data_(Number_line, Number_sample)
    {
    }

//-----------------------------------------------------------------------
/// Construct a MemoryRasterImage of the given size, and fill with
/// given value (useful in particular for unit tests that just need
/// "an image" without caring about the content)
//-----------------------------------------------------------------------

  MemoryRasterImage(int Number_line, int Number_sample, int Fill_value)
    : RasterImageVariable(Number_line, Number_sample),
      data_(Number_line, Number_sample)
    {
      data_ = Fill_value;
    }

  MemoryRasterImage(const RasterImage& Img, int Number_line_to_read = -1, 
		    int Number_sample_to_read = -1);

//-----------------------------------------------------------------------
/// Construct a MemoryRasterImage of the given MapInfo.
//-----------------------------------------------------------------------

  MemoryRasterImage(const MapInfo& Mi)
    : RasterImageVariable(Mi),
      data_(number_line(), number_sample())
  { }

  virtual ~MemoryRasterImage() {}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  blitz::Array<int, 2>& data() {return  data_;}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  const blitz::Array<int, 2>& data() const {return  data_;}


  virtual int unchecked_read(int Line, int Sample) const
  {
    return data_(Line,Sample);
  }
  virtual double unchecked_read_double(int Line, int Sample) const
  {
    return (double) data_(Line,Sample);
  }
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
  {
    range_check(Lstart, 0, number_line() - (Number_line - 1));
    range_check(Sstart, 0, number_sample() - (Number_sample - 1));
    for(int i = Lstart; i < Lstart + Number_line; ++i)
      for(int j = Sstart; j < Sstart + Number_sample; ++j, ++Res)
	*Res = data_(i,j);
  }
  virtual void unchecked_write(int Line, int Sample, int Val)
  {
    data_(Line, Sample) = Val;
  }
  virtual void unchecked_write(int Line, int Sample, double Val)
  {
    data_(Line, Sample) = (int) Val;
  }
  virtual void print(std::ostream& Os) const;
 private:
  blitz::Array<int, 2> data_; ///< Underlying data.
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a variation of MemoryRasterImage where the data comes from
  outside of this class.
*******************************************************************/

template<class T> class MemoryRasterImageRef: public RasterImageVariable {
public:
//-----------------------------------------------------------------------
/// Construct a MemoryRasterImageRes of the given size, pointing to
/// the given data. Note that the lifetime of the data is managed
/// outside of this class.
//-----------------------------------------------------------------------

  MemoryRasterImageRef(T* d, int Number_line, int Number_sample)
    : RasterImageVariable(Number_line, Number_sample),
      data_(d, boost::extents[Number_line][Number_sample])
    {
    }

//-----------------------------------------------------------------------
/// Variation of constructor that takes ownership of an underlying
/// vector with the data. This can give a simpler interface in
/// instances where we don't want to maintain ownership outside of
/// this class.
//-----------------------------------------------------------------------

  MemoryRasterImageRef(const typename boost::shared_ptr<std::vector<T> >& 
		       Raw_data, 
		       typename std::vector<T>::size_type Offset, 
		       int Number_line, int Number_sample)
    : RasterImageVariable(Number_line, Number_sample),
      data_(&(*Raw_data)[Offset], boost::extents[Number_line][Number_sample]),
      raw_data_(Raw_data)
    {
    }

  virtual ~MemoryRasterImageRef() {}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  boost::multi_array_ref<T, 2>& data() {return  data_;}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  const boost::multi_array_ref<T, 2>& data() const {return  data_;}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  {
    return (int) data_[Line][Sample];
  }

  virtual double unchecked_read_double(int Line, int Sample) const
  {
    return (double) data_[Line][Sample];
  }

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
  {
    range_check(Lstart, 0, number_line() - (Number_line - 1));
    range_check(Sstart, 0, number_sample() - (Number_sample - 1));
    for(int i = Lstart; i < Lstart + Number_line; ++i)
      for(int j = Sstart; j < Sstart + Number_sample; ++j, ++Res)
	*Res = (int) data_[i][j];
  }

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void unchecked_write(int Line, int Sample, int Val)
  {
    data_[Line][Sample] = (T) Val;
  }
  virtual void unchecked_write(int Line, int Sample, double Val)
  {
    data_[Line][Sample] = (T) Val;
  }

  
//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {
      Os << "MemoryRasterImageRef of " << number_line() << " x " 
	 << number_sample() << " all in memory\n";
  }
 private:
  boost::multi_array_ref<T, 2> data_; ///< Underlying data.
  boost::shared_ptr<std::vector<T> > raw_data_;
};

}

GEOCAL_EXPORT_KEY(MemoryRasterImage);
#endif
