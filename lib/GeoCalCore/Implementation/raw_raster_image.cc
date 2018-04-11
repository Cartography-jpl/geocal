#include "raw_raster_image.h"
#include "geocal_config.h"
#ifdef HAVE_LIBRAW
#include "libraw.h"
#endif

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Read in the given file with the given Band (0 based).
//-----------------------------------------------------------------------

RawRasterImage::RawRasterImage(const std::string& Fname, int Band)
: fname(Fname), band_(Band)
{
  range_check(Band, 0, 3);
#ifdef HAVE_LIBRAW 
  LibRaw t;
  t.open_file(Fname.c_str());
  number_sample_ = t.imgdata.sizes.width;
  number_line_ = t.imgdata.sizes.height;
  number_tile_line_ = number_line_;
  number_tile_sample_ = number_tile_sample_;
  data.reset(new MemoryRasterImage(number_line(), number_sample()));
  t.unpack();
  for(int i = 0; i < number_line(); ++i)
    for(int j = 0; j < number_sample(); ++j)
      data->write(i, j, t.imgdata.image[i*number_sample() + j][Band]);
  t.recycle();
#else
  throw Exception("Software was not built with libraw");
#endif
}

//-----------------------------------------------------------------------
/// These read all three bands of a file at once. This is faster than
/// reading each band separately.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<RasterImage> > 
RawRasterImage::read_all_bands(const std::string& Fname)
{
#ifdef HAVE_LIBRAW 
  LibRaw t;
  t.open_file(Fname.c_str());
  int number_sample = t.imgdata.sizes.width;
  int number_line = t.imgdata.sizes.height;
  std::vector<boost::shared_ptr<RasterImage> > res(3);
  res[0].reset(new MemoryRasterImage(number_line, number_sample));
  res[1].reset(new MemoryRasterImage(number_line, number_sample));
  res[2].reset(new MemoryRasterImage(number_line, number_sample));
  t.unpack();
  for(int i = 0; i < number_line; ++i)
    for(int j = 0; j < number_sample; ++j) {
      res[0]->write(i, j, t.imgdata.image[i*number_sample + j][0]);
      res[1]->write(i, j, t.imgdata.image[i*number_sample + j][1]);
      res[2]->write(i, j, t.imgdata.image[i*number_sample + j][2]);
    }
  t.recycle();
  return res;
#else
  throw Exception("Software was not built with libraw");
#endif
}

//-----------------------------------------------------------------------
/// Print to an output stream.
//-----------------------------------------------------------------------

void RawRasterImage::print(std::ostream& Os) const
{
  Os << "RawRasterImage\n"
     << "  File name: " << fname << "\n"
     << "  Band:      " << band_ << "\n";
}

