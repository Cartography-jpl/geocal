#include "phase_correlation_matcher.h"
#include <blitz/array.h>

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor. 
/// \param Template_size Size of the area that we do use in the first 
///        image. This was called FFTSIZE in Al Zobrist's picmtch4
///        program. Note that this must be even.
/// \param Search_size Size to search in the second image. This should
///        be > Template_size, and even.
//-----------------------------------------------------------------------

PhaseCorrelationMatcher::PhaseCorrelationMatcher(
int Template_size, 
int Search_size
)
: fftsize(Template_size),
  search(Search_size)
{
  range_min_check(Template_size, 0);
  range_min_check(Search_size, Template_size);
  if(Template_size % 2 != 0 ||
     Search_size % 2 != 0)
    throw Exception("Template and search size need to be even");
}

// See base class for description

void PhaseCorrelationMatcher::match_mask
(const RasterImage& Ref, 
 const ImageMask& Ref_mask,			 
 const RasterImage& New, 
 const ImageMask& New_mask,
 const ImageCoordinate& Ref_loc, 
 const ImageCoordinate& New_guess, 
 ImageCoordinate& New_res,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  Success = false;		// We'll change this once we succeed.
  int ref_line = int(Ref_loc.line) - template_size() / 2;
  int ref_sample = int(Ref_loc.sample) - template_size() / 2;
  int new_line = int(New_guess.line) - search_size() / 2;
  int new_sample = int(New_guess.sample) - search_size() / 2;

//-----------------------------------------------------------------------
/// Check if we have enough data in the images, if not we indicate
/// that we are too close to the edge and fail.
///
/// TODO Al's code allows some amount of the image to be over the edge
///      We may want to look into that, but for now just fail.
//-----------------------------------------------------------------------

  if(ref_line < 0 || ref_line + template_size() > Ref.number_line()   ||
     ref_sample < 0 || ref_sample + template_size() > Ref.number_sample() ||
     new_line < 0 || new_line + search_size() > New.number_line()   ||
     new_sample < 0 || new_sample + search_size() > New.number_sample()) {
    if(Diagnostic)
      *Diagnostic = TOO_CLOSE_TO_IMAGE_EDGE;
    return;
  }

//-----------------------------------------------------------------------
// Check if any of the date we want to use is masked.
//-----------------------------------------------------------------------

  if(Ref_mask.area_any_masked(ref_line, ref_sample, template_size(),
			      template_size()) ||
     New_mask.area_any_masked(new_line, new_sample, search_size(),
			      search_size())) {
    if(Diagnostic)
      *Diagnostic = IMAGE_MASKED;
    return;
  }
  Array<double, 2> chip1 = Ref.read_double(ref_line, ref_sample, 
					   template_size(), template_size());
  Array<double, 2> asrch = New.read_double(new_line, new_sample,
					   search_size(), search_size());
  std::cerr << "Chip1:\n" << chip1 << "\n"
	    << "Asrch:\n" << asrch << "\n";
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void PhaseCorrelationMatcher::print(std::ostream& Os) const
{
  Os << "Phase correlation matcher:\n"
     << "  Template size: " << template_size() << "\n"
     << "  Search size:   " << search_size() << "\n";
}
