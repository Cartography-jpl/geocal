#ifndef RAW_RASTER_IMAGE_H
#define RAW_RASTER_IMAGE_H
#include <boost/shared_ptr.hpp>
#include <vector>
#include "memory_raster_image.h"

namespace GeoCal {
/****************************************************************//**
  This uses libraw to read RAW format. This is the format used by
  some cameras, e.g., Cannon RAW format. You can see the details
  about what can be read at http://www.libraw.org.

  Note that although this reads the RAW format, the data is in the
  RAW space, the DN values don't mean much without translating them
  to RGB color space. You may want to instead use the program dcraw
  which converts to PPM or Tiff format and includes translating to
  color plus making 8 bit.
*******************************************************************/

class RawRasterImage : public RasterImageVariable {
 public:
  RawRasterImage(const std::string& Fname, int Band = 0);
  static std::vector<boost::shared_ptr<RasterImage> > 
    read_all(const std::string& Fname);
  virtual ~RawRasterImage() {}
//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const 
  {return data->unchecked_read(Line,Sample);}
  virtual double unchecked_read_double(int Line, int Sample) const 
  {return data->unchecked_read_double(Line,Sample);}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
    { data->read_ptr(Lstart, Sstart, Number_line, Number_sample, Res);}

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void unchecked_write(int Line, int Sample, int Val)
    { throw Exception("RawRasterImage doesn't implement write");}
  
  virtual void print(std::ostream& Os) const;
  const std::string& file_name() const {return fname;}
  int band() const {return band_;}
private:
  std::string fname;
  int band_;
  boost::shared_ptr<MemoryRasterImage> data;
};
}
#endif
