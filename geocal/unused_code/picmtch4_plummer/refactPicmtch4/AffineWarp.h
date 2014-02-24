/*===========================================================================
=                                                                           =
=                             AffineWarp                                    =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: AffineWarp

!ABSTRACT:

   This class calculates a Homography transformation and performs an affine
   warping on an image.


!DERIVED FROM:


!DESCRIPTION:


!PUBLIC MEMBERS:

    cv::Mat getTransformation(
         std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs,
	 std::vector<GeoCal::GeoCalCore::ImageCoor>& Target_locs)
      - Calculates a 3x2 transformation matrix based on points in the
        reference image and their corresponding location in a target
        location.

    cv::Mat warp(GeoCal::GeoCalCore::RasterImage& target, 
			 cv::Mat& transform);
     - Performs affine warping on an image with a given 3x2 
       transformation matrix.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:


!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef AFFINEWARP_H
#define AFFINEWARP_H

#include "cv.h"
#include <vector>
#include "raster_image.h"
#include "image_coor.h"

//***************************************************************************
// Declaration of class AffineWarp.
//***************************************************************************

class AffineWarp
{
 public:
  cv::Mat getTransformation
    (std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs,
     std::vector<GeoCal::GeoCalCore::ImageCoor>& Target_locs);

  cv::Mat warp(GeoCal::GeoCalCore::RasterImage& target, 
			 cv::Mat& transform);
};

#endif
