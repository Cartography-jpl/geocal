#ifndef PYRAMID_IMAGE_MATCH_H
#define PYRAMID_IMAGE_MATCH_H
#include "image_matcher.h"

namespace GeoCal {
/****************************************************************//**
  This takes an existing ImageMatcher, and it uses it on images in
  a pyramid fashion. We start with a given power of 2 (e.g., 8),
  average the raster data by that value and perform image matching.

  If that is successful, we then take the result of the previous 
  match as the initial guess, then match on the finer grid by a factor
p  of 2 (e.g., 4). If successful, we then proceed to the next level,
  ending at the full resolution image.

  If we match any level, then we call the match successful. However,
  the Line and Sample sigmas will get scaled the same way - so data
  matched at a coarser resolution is less accurate than matched at the
  finer resolution.
*******************************************************************/

class PyramidImageMatcher : public ImageMatcher {
public:
//-----------------------------------------------------------------------
/// Constructor. The start level is how coarse to do the initial
/// matching. By default we accept any level of matching (just with a
/// larger error), but you can optionally specify that we only accept
/// a point if we can match at the finest resolution
//-----------------------------------------------------------------------
  PyramidImageMatcher(const boost::shared_ptr<ImageMatcher>& Im,
		      int start_level, 
		      bool Only_accept_finest_resolution = false)
    : match_(Im), start_level_(start_level), 
      only_accept_finest_resolution_(Only_accept_finest_resolution)
  {
    range_min_check(start_level, 1);
  }

  virtual ~PyramidImageMatcher() {}

//-----------------------------------------------------------------------
/// Underlying image matcher.
//-----------------------------------------------------------------------

  boost::shared_ptr<ImageMatcher> underlying_matcher() const
  { return match_; }

//-----------------------------------------------------------------------
/// If true, we only accept points that can be matched at the finest 
/// resolution.
//-----------------------------------------------------------------------

  bool only_accept_finest_resolution() const 
  {return only_accept_finest_resolution_;}

//-----------------------------------------------------------------------
/// Starting level, so we average by 2^start_level.
//-----------------------------------------------------------------------

  int start_level() const {return start_level_;}

  virtual void match_mask(const RasterImage& Ref, const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, const
			  ImageCoordinate& New_guess, 
			  ImageCoordinate& New_res,
			  double& Line_sigma, double& Sample_sigma,
			  bool& Success,
			  int* Diagnostic = 0) const;
  virtual void print(std::ostream& Os) const;
protected:
  PyramidImageMatcher() {}
private:
  boost::shared_ptr<ImageMatcher> match_;
  int start_level_;
  bool only_accept_finest_resolution_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PyramidImageMatcher);
#endif


