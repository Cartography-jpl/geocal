#include "pyramid_image_matcher.h"
#include "geocal_serialize_support.h"
#include "raster_averaged.h"
#include "null_deleter.h"
#include "ostream_pad.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PyramidImageMatcher::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(ImageMatcher);
  GEOCAL_BASE(PyramidImageMatcher, ImageMatcher);
  ar & GEOCAL_NVP_(match)
    & GEOCAL_NVP_(start_level)
    & GEOCAL_NVP_(only_accept_finest_resolution);
}


GEOCAL_IMPLEMENT(PyramidImageMatcher);
#endif

// See base class for description
void PyramidImageMatcher::match_mask
(const RasterImage& Ref, const ImageMask& Ref_mask,
 const RasterImage& New, 
 const ImageMask& New_mask,
 const ImageCoordinate& Ref_loc, const
 ImageCoordinate& New_guess, 
 ImageCoordinate& New_res,
 double& Line_sigma, double& Sample_sigma,
 bool& Success,
 int* Diagnostic) const
{
  Success = false;
  ImageCoordinate guess = New_guess;
  for(int lv = start_level_; lv > 0; --lv) {
    int fac = 1 << lv;
    RasterAveraged refavg(const_pointer(Ref), fac, fac);
    RasterAveraged newavg(const_pointer(New), fac, fac);
    ImageMaskAveraged refmaskavg(const_pointer(Ref_mask), fac, fac);
    ImageMaskAveraged newmaskavg(const_pointer(New_mask), fac, fac);
    ImageCoordinate ref_loc_avg(Ref_loc.line / fac, Ref_loc.sample / fac);
    ImageCoordinate new_guess_avg(guess.line / fac, guess.sample / fac);
    ImageCoordinate res_avg;
    double lsigma, ssigma;
    bool suc;
    int diag;
    match_->match_mask(refavg, refmaskavg, newavg, newmaskavg,
		       ref_loc_avg, new_guess_avg, res_avg, lsigma, ssigma,
		       suc, &diag);
    if(suc && (lv == 1 || !only_accept_finest_resolution_)) {
      New_res.line = res_avg.line * fac;
      New_res.sample = res_avg.sample * fac;
      guess = New_res;
      Line_sigma = lsigma * fac;
      Sample_sigma = ssigma * fac;
      Success = true;
      if(Diagnostic)
	*Diagnostic = diag;
    } else {
      if(!Success && Diagnostic)
	*Diagnostic = diag;
      return;
    }
  }
}

void PyramidImageMatcher::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "PyramidImageMatcher:\n"
     << "  Start level: " << start_level_ << "\n"
     << "  Only accept finest resolution matches: " 
     << only_accept_finest_resolution_ << "\n"
     << "  Underlying matcher:\n";
  opad << *match_;
  opad.strict_sync();
}

