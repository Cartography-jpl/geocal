#include "image_mask_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// This returns the bounding box of the unmasked area. This can be
/// useful for an image to chop off an outer edge that is completely
/// masked.
//-----------------------------------------------------------------------

void ImageMaskImage::unmasked_bounding_box(int& Minline, int &Minsample, 
					   int &Numberline,
					   int &Numbersample) const
{
  Minline = 0; 
  Minsample = 0; 
  int maxline = -1;
  int maxsample = -1;
  bool first = true;
  RASTER_FOREACH(*img, i, j)
    if((*img)(i, j) != maskv) {
      if(first) {
	Minline = i;
	maxline = i;
	Minsample = j;
	maxsample = j;
	first = false;
      } else {
	Minline = std::min(Minline, i);
	maxline = std::max(maxline, i);
	Minsample = std::min(Minsample, j);
	maxsample = std::max(maxsample, j);
      }
    }
  Numberline = maxline - Minline + 1;
  Numbersample = maxsample - Minsample + 1;
}

bool ImageMaskImage::area_any_masked
(int Line, int Sample, int Number_line, int Number_sample) const
{
  if(Line < 0 || Line + Number_line >= img->number_line() ||
     Sample < 0 || Sample + Number_sample >= img->number_sample()) {
    if(oh == OUTSIDE_MASKED)
      return true;
    if(oh == OUTSIDE_NOT_MASKED)
      return ImageMask::area_any_masked(Line, Sample, Number_line, 
					Number_sample);
    throw Exception("Point is outside of image");
  }
  return blitz::any((*img).read(Line, Sample, Number_line, Number_sample) == 
		    maskv);
}

bool ImageMaskImage::mask(int Line, int Sample) const
{
  if(Line < 0 || Line > img->number_line() - 1 ||
     Sample < 0 || Sample > img->number_sample() - 1) {
    if(oh == OUTSIDE_MASKED)
      return true;
    if(oh == OUTSIDE_NOT_MASKED)
      return false;
    throw Exception("Point is outside of image");
  }
  return (*img)(Line, Sample) == maskv;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ImageMaskImage::print(std::ostream& Os) const
{
  Os << "Image Mask image:\n"
     << "  Image:\n"
     << *img
     << "  Mask value: " << maskv << "\n";
}


