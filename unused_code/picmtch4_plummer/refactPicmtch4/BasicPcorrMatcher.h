/*===========================================================================
=                                                                           =
=                             MspiPcorrMatcher                              =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: MisrCcorrMatcher

!ABSTRACT:

   This is an Adapter (see Design Patterns by Gamma et. al) that makes the
   a basic phase correlation image matcher based on a version from 
   Nashruddin Amin.


!DERIVED FROM:


!DESCRIPTION:


!PUBLIC MEMBERS:
    BasicPcorrMatcher(int fft_size, int search_window, double thresh)
      - Constructor
    void match(cv::Mat& Ref, cv::Mat& New, 
	       cv::Point2f& Ref_loc, cv::Point2f& New_guess, 
	       cv::Point2f& Result, bool* Success, double* Cov);
      - perform the phase correlation matching at the given points

!PROTECTED MEMBERS:
    bool rangeCheck(cv::Point2f& searchLoc, cv::Mat& img, cv::Point2f& refLoc)
      - Checks if the new location to search is still in the search window and
        the image.
!PRIVATE MEMBERS:
    void phase_correlation(cv::Mat& ref, cv::Mat& tpl, cv::Mat& poc)
      - Calculate the phase correlation between the two images
    int fft_size_;
      - Size of the fft to use.
    int search_window_;
      - Size of the search window to look through.
    double zero_thresh_;
      - Image chip must have more than this percentage of zero values.
   
!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

  Has only been tested using the entire image as the fft, current version
  is untested.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef BASICPCORRMATCHER_H
#define BASICPCORRMATCHER_H

#include "cv.h"

//***************************************************************************
// Declaration of class BasicPcorrMatcher.
//***************************************************************************

class BasicPcorrMatcher
{
 public:
  BasicPcorrMatcher(int fft_size, int search_window, double thresh);

  void match(cv::Mat& Ref, cv::Mat& New, 
	     cv::Point2f& Ref_loc, cv::Point2f& New_guess, 
	     cv::Point2f& Result, bool* Success, double* Cov);
 protected:
  bool rangeCheck(cv::Point2f& searchLoc, cv::Mat& img, cv::Point2f& refLoc);
 private:
  void phase_correlation(cv::Mat& ref, cv::Mat& tpl, cv::Mat& poc);
  int fft_size_, search_window_;
  double zero_thresh_;
};

#endif
