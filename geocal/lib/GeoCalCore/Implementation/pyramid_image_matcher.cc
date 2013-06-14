#include "pyramid_image_matcher.h"
#include "raster_averaged.h"
#include "null_deleter.h"
using namespace GeoCal;

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
		       suc,&diag);
    if(suc) {
      New_res.line = res_avg.line * fac;
      New_res.sample = res_avg.sample * fac;
      guess = New_res;
      Line_sigma = lsigma * fac;
      Sample_sigma = ssigma * fac;
      Success = true;
    } else {
      if(!Success && Diagnostic)
	*Diagnostic = diag;
      return;
    }
  }
}

void PyramidImageMatcher::print(std::ostream& Os) const
{
  Os << "PyramidImageMatcher:\n"
     << "  Start level: " << start_level_ << "\n"
     << "  Underlying matcher:\n" << match_ << "\n";
}

