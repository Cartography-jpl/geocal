/*===========================================================================
=                                                                           =
=                             ImageVis                                       =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: ImageVis

!ABSTRACT:

   This class contains a set of functions to visualize images and their
   data.


!DERIVED FROM:


!DESCRIPTION:


!PUBLIC MEMBERS:

    enum RGBColor {BLUE,GREEN,RED}
      - Used as input to some drawing functions that output colors.

    cv::Mat& convertGrayToSingleColor(cv::Mat img, RGBColor colorType)
      - Ouputs a given image as an RGB image of a single color.

    void displayAllMatchedPoints(cv::Mat ref, cv::Mat target, 
                                 std::vector<cv::Point2f>& Ref_locs, 
				 std::vector<cv::Point2f>& Results)
      - Draws lines between points in two images and displays the 
        result.

    void displayCombinedWarped(cv::Mat ref, cv::Mat target, cv::Mat warped)
      - displays reference and target image overlaying each other and
        the reference and warped image overlaying each other.

    void displayEntireMatchedRegion(cv::Mat ref, std::vector<cv::Rect>& rects)
      - Draws a set of rectangles on an image and displays the result.

    void displayResultsFromPoints(IplImage *ref, 
				  std::vector<cv::Point2f>& Ref_locs, 
				  std::vector<cv::Point2f>& Results)
      - Draws rectangles on sub images based on the given points in an image 
        and displays the result.
     
    void displaySingleMatch(cv::Mat ref, cv::Mat target, 
			    cv::Point2f ref_loc, cv::Point2f new_loc, 
			    bool printMatchingPoints = false)
      - Displays the two given images and optionally prints the
        coordinates they are located at.

    bool drawImBorder(cv::Mat src, cv::Mat dst, bool showBorder = true)
      - Draws a border that encloses the non-zero values from one image on a
        second image and optionally displays the result.

    bool drawImBoundingBox(cv::Mat img, cv::Rect& box, bool showBox = false)
      - Draws a box in an image that bounds non-zero values and optionally
        displays the result.

    void drawPoint(cv::Mat& ref, cv::Point2f& center,bool rgb = false);
      - Draws a single point on an input image.

    cv::Scalar ImageVis::randomColor(cv::RNG& rng)
      - Returns random color

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

    Most functions assume a grayscale image as input.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef IMAGEVIS_H
#define IMAGEVIS_H

#include "cv.h"
#include <vector>
#include "ImageOps.h"

//***************************************************************************
// Declaration of class ImageVis.
//***************************************************************************

class ImageVis
{
 public:
  enum RGBColor {BLUE,GREEN,RED};
  cv::Mat convertGrayToSingleColor(cv::Mat& img, RGBColor colorType);

  void displayAllMatchedPoints(cv::Mat& ref, cv::Mat& target, 
			       std::vector<cv::Point2f>& Ref_locs, 
			       std::vector<cv::Point2f>& Results);
  void displayCombinedWarped(cv::Mat ref, cv::Mat target, cv::Mat warped);
  void displayEntireMatchedRegion(cv::Mat& ref, std::vector<cv::Rect>& rects);

  void displayResultsFromPoints(cv::Mat& ref, 
				std::vector<cv::Point2f>& Ref_locs, 
				std::vector<cv::Point2f>& Results);

  void displaySingleMatch(cv::Mat& ref, cv::Mat& target, 
			  cv::Point2f& ref_loc, cv::Point2f& new_loc, 
			  bool printMatchingPoints = false);

  bool drawImBorder(cv::Mat& img, cv::Mat& img2, bool showBorder = true);
  bool drawImBoundingBox(cv::Mat& img, cv::Rect& box, bool showBox = false);
  void drawPoint(cv::Mat& ref, cv::Point2f& center,bool rgb = false);
  cv::Scalar randomColor(cv::RNG& rng);
};
#endif
