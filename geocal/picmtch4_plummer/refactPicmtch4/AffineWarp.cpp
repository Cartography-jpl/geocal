/*===========================================================================
=                                                                           =
=                             ImageOps                                      =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME:

!ABSTRACT:

   Image operation support functions.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/
#include "AffineWarp.h"
#include "ImageOps.h"

//***************************************************************************
// cv::Mat AffineWarp::getTransformation
// (std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs, //reference locs
// std::vector<GeoCal::GeoCalCore::ImageCoor>& Target_locs) // target locs
//***************************************************************************
cv::Mat AffineWarp::getTransformation
(std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs,
 std::vector<GeoCal::GeoCalCore::ImageCoor>& Target_locs)
{
  std::vector<cv::Point2f>& cvR_locs,cvT_locs;
  imageCoorToPoint(Ref_locs,cvR_locs);
  imageCoorToPoint(Target_locs,cvT_locs);

  cv::Mat H = findHomography(cv::Mat(cvT_locs),cv::Mat(cvR_locs),CV_RANSAC);
  cv::Mat transform(cv::Size(3,2),H.type());
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 2; j++)
      transform.at<double>(i,j) = H.at<double>(i,j);

  return transform;
}

//***************************************************************************
// cv::Mat AffineWarp::warp(
// GeoCal::GeoCalCore::RasterImage& target, // target image
// cv::Mat& transform) // transformation to use
//***************************************************************************
cv::Mat AffineWarp::warp(GeoCal::GeoCalCore::RasterImage& target, 
			 cv::Mat& transform)
{
  cv::Mat tarMat = rastToMat(target);
  cv::Point2f origin = cv::Point2f(0,0);
  cv::Mat warped = subImage(tarMat,origin,tarMat.size());
  warpAffine(target,warped,transform,warped.size());
  return warped;
}
