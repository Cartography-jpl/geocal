#include "image_mask.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"
#include <boost/foreach.hpp>
#include <cmath>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ImageMask::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(ImageMask);
}

template<class Archive>
void OffsetImageMask::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(ImageMask);
  GEOCAL_BASE(OffsetImageMask, ImageMask);
  ar & GEOCAL_NVP_(im) & GEOCAL_NVP_(line_offset) & GEOCAL_NVP_(sample_offset);
}

template<class Archive>
void CombinedImageMask::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(ImageMask);
  GEOCAL_BASE(CombinedImageMask, ImageMask);
  ar & GEOCAL_NVP(mask_list);
}

GEOCAL_IMPLEMENT(ImageMask);
GEOCAL_IMPLEMENT(OffsetImageMask);
GEOCAL_IMPLEMENT(CombinedImageMask);
#endif

//-----------------------------------------------------------------------
/// Indicate if an ImageCoordinate is masked or not. We search for the
/// 4 neighbors around an ImageCoordinate are masked.
//-----------------------------------------------------------------------

bool ImageMask::mask_ic(const ImageCoordinate& Ic) const
{
  int ln = (int) floor(Ic.line);
  int smp = (int) floor(Ic.sample);
  return area_any_masked(ln, smp, (ln == Ic.line ? 1 : 2),
			 (smp == Ic.sample ? 1 : 2));
}

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

void OffsetImageMask::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OffsetImageMask:\n"
     << "  Line offset:   " << line_offset_ << "\n"
     << "  Sample offset: " << sample_offset_ << "\n"
     << "  Underlying mask:\n";
  opad << *im_ << "\n";
  opad.strict_sync();
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
