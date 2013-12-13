#ifndef PHASE_CORRELATION_MATCHER_H
#define PHASE_CORRELATION_MATCHER_H
#include "image_matcher.h"
namespace GeoCal {
/****************************************************************//**
  This class performs image matching. It uses phase correlation. This
  is an adaption of Al Zobrist's picmtch4 AFIDS procedure. 

  That program tries to match a number of points as one executable
  call. This includes both the matching, and the update of the
  predictor for the matching. We have extracted out just the image
  matching, we have other functionality for handling the predictor.

  The original code was based on the paper:
      C.  D.  Kuglin and D.  C. Hines, "The phase correlation 
     image alignment method," Proc.  IEEE 1975 International 
     Conference on Cybernetics and Society, September, 1975, 
     pp. 163-165.
*******************************************************************/

class PhaseCorrelationMatcher : public ImageMatcher {
public:
  enum Diagnostic {NO_FAIL = 0, IMAGE_MASKED = 1, TOO_CLOSE_TO_IMAGE_EDGE = 2};
  PhaseCorrelationMatcher(int Template_size = 32, int Search_size = 32);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~PhaseCorrelationMatcher() {}

  virtual void match_mask(const RasterImage& Ref, const ImageMask& Ref_mask,
			  const RasterImage& New, 
			  const ImageMask& New_mask,
			  const ImageCoordinate& Ref_loc, const
			  ImageCoordinate& New_guess, 
			  ImageCoordinate& New_res,
			  double& Line_sigma, double& Sample_sigma,
			  bool& Success,
			  int* Diagnostic = 0) const;

/// TODO Go through and change the variable names used here. But for
/// now, match Al's code

//-----------------------------------------------------------------------
/// Size of the area that we do use in the first image. This was
/// called FFTSIZE in Al Zobrist's picmtch4 program. Note that this must
/// be a power of 2. There is a note in picmtch4 that this should be
/// at least 32 and a power of 2, but this isn't actually required by
/// the code, just that it is even.
//-----------------------------------------------------------------------

  int template_size() const { return fftsize; }

//-----------------------------------------------------------------------
/// The search size. This should be >= template_size().
//-----------------------------------------------------------------------
  int search_size() const {return search; }

  virtual void print(std::ostream& Os) const;
private:
  int fftsize, search;
};

}
#endif
