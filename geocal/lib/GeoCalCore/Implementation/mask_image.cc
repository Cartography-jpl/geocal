#include "mask_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Indicate if a particular point is masked. If true, the point is
/// masked and should not be used in processing (e.g., don't do image
/// matching)
//-----------------------------------------------------------------------

bool MaskImage::mask(const GroundCoordinate& Gc) const
{
  ImageCoordinate ic = img->coordinate(Gc);
  int i = (int) floor(ic.line);
  int j = (int) floor(ic.sample);
  if(i < 0 || i + 1 >= img->number_line() ||
     j < 0 || j + 1 >= img->number_sample())
    return false;
  return ((*img)(i, j)     ==maskv || (*img)(i, j + 1) ==maskv ||
	  (*img)(i + 1, j) ==maskv || (*img)(i + 1, j + 1) ==maskv);
}

//-----------------------------------------------------------------------
/// Indicated if a region is all masked or not. See the discussion in
/// the comments of Mask for detailed discussion of the check.
//-----------------------------------------------------------------------

bool MaskImage::region_masked(const GroundCoordinate& Ulc, 
			      const GroundCoordinate& Lrc) const
{
  ImageCoordinate ic1 = img->coordinate(Ulc);
  ImageCoordinate ic2 = img->coordinate(Lrc);
  int i1 = (int) floor(std::min(ic1.line, ic2.line));
  int j1 = (int) floor(std::min(ic1.sample, ic2.sample));
  int i2 = (int) ceil(std::max(ic1.line, ic2.line));
  int j2 = (int) ceil(std::max(ic1.sample, ic2.sample));

  if(i1 < 0 || i2 >= img->number_line() ||
     j1 < 0 || j2 >= img->number_sample())
    return false;
  return blitz::any((*img).read(i1, j1, (i2 - i1) + 1, (j2 - j1) + 1) == 
		    maskv);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void MaskImage::print(std::ostream& Os) const
{
  Os << "Mask image:\n"
     << "  Map projected image:\n"
     << *img
     << "  Mask value: " << maskv << "\n";
}


