#ifndef CCORR_MATCHER_H
#define CCORR_MATCHER_H
#include "image_matcher.h"

namespace GeoCal {
/****************************************************************//**
  This class performs image matching. It does a normalized cross
  correlation match.
*******************************************************************/

class CcorrMatcher : public ImageMatcher {
public:
//-----------------------------------------------------------------------
/// Diagnostic codes indicates why a match failed.
//-----------------------------------------------------------------------

  enum Diagnostic {NO_FAIL = 0, IMAGE_MASKED = 1, TOO_CLOSE_TO_IMAGE_EDGE = 2,
		   VARIANCE_TOO_LOW = 3, CORRELATION_TO_LOW = 4};
  CcorrMatcher(int Target_nline = 37, int Target_nsamp = 37, int
	       Template_nline = 9, int Template_nsamp = 9, double Min_ccorr =
	       0.3, double Min_variance = 0);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~CcorrMatcher() {}
  virtual void match_mask(const RasterImage& Ref, const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, const
			  ImageCoordinate& New_guess, 
			  ImageCoordinate& New_res,
			  double& Line_sigma, double& Sample_sigma,
			  bool& Success,
			  int* Diagnostic = 0) const;

//-----------------------------------------------------------------------
/// Minimum correlation threshold.
//-----------------------------------------------------------------------

  double min_correlation() const {return min_correlation_;}

//-----------------------------------------------------------------------
/// Minimum variance threshold.
//-----------------------------------------------------------------------

  double min_variance() const {return min_variance_;}

//-----------------------------------------------------------------------
/// Number of lines in target window.
//-----------------------------------------------------------------------

  int target_number_line() const {return target_number_line_;}

//-----------------------------------------------------------------------
/// Number of samples in target window.
//-----------------------------------------------------------------------

  int target_number_sample() const {return target_number_sample_;}

//-----------------------------------------------------------------------
/// Number of lines in template window.
//-----------------------------------------------------------------------

  int template_number_line() const {return template_number_line_;}

//-----------------------------------------------------------------------
/// Number of samples in template window.
//-----------------------------------------------------------------------

  int template_number_sample() const {return template_number_sample_;}
  virtual void print(std::ostream& Os) const;
private:
  double min_correlation_;
  double min_variance_;
  int target_number_line_;
  int target_number_sample_;
  int template_number_line_;
  int template_number_sample_;
  mutable blitz::Array<int, 2> templ, target;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(CcorrMatcher);
#endif

