#include "image_mask_image.h"

using namespace GeoCal;

bool ImageMaskImage::area_any_masked
(int Line, int Sample, int Number_line, int Number_sample) const
{
  if(Line < 0 || Line + Number_line >= img->number_line() ||
     Sample < 0 || Sample + Number_sample >= img->number_sample())
    return true;
  return blitz::any((*img).read(Line, Sample, Number_line, Number_sample) == 
		    maskv);
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


