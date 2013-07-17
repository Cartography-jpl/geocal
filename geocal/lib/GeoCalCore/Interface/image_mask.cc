#include "image_mask.h"
#include <boost/foreach.hpp>
using namespace GeoCal;


//-----------------------------------------------------------------------
/// Indicated if a area has any masking or not. See the discussion in
/// the comments of Mask for detailed discussion of the check.
//-----------------------------------------------------------------------

bool ImageMask::area_any_masked(int Line, int Sample, int Number_line,
				int Number_sample) const
{
  for(int ln = Line; ln < Line + Number_line; ++ln)
    for(int smp = Sample; smp < Sample + Number_sample; ++smp)
      if(mask(ln, smp))
	return true;
  return false;
}

// See base class for description

bool CombinedImageMask::mask(int Line, int Sample) const
{
  BOOST_FOREACH(boost::shared_ptr<ImageMask> m, mask_list) {
    if(m->mask(Line, Sample))
      return true;
  }
  return false;
}

// See base class for description

bool CombinedImageMask::area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const
{
  BOOST_FOREACH(boost::shared_ptr<ImageMask> m, mask_list) {
    if(m->area_any_masked(Line, Sample, Number_line, Number_sample))
      return true;
  }
  return false;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CombinedImageMask::print(std::ostream& Os) const
{
  if(mask_list.size() ==0)
    Os << "Empty CombinedImageMask\n";
  else {
    Os << "CombinedImageMask: \n";
    int i = 0;
    BOOST_FOREACH(boost::shared_ptr<ImageMask> m, mask_list) {
      Os << "  Mask " << ++i << ":\n" << *m;
    }
  }
}