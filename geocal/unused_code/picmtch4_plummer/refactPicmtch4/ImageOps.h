/*===========================================================================
=                                                                           =
=                             ImageOps                                      =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME:

!ABSTRACT:

   This class contains functions to perform some basic image operations.


!DERIVED FROM:


!DESCRIPTION:


!PUBLIC MEMBERS:

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

    void findBelowThres(cv::Mat& matrix, std::vector<cv::Point2f>& nonZeros, 
		    double thresh = 0);
      - Find the values in a matrix below a given threshold. Assumes single 
        channel.

    double getAmountNonZero(cv::Mat& img)
     - Returns the ratio of values in an array that is not zero.

    void getgrid(GeoCal::GeoCalCore::RasterImage& img, float* chip, 
		 int narray, int ndim, double trans[12], 
		 float centrx, float centry, float rmag[2], 
		 int* chop, float pcl, float pcs, int zerothr)
      - Extracts a subimage from the RasterImage.

    bool makeGrid(GeoCal::GeoCalCore::RasterImage& img, int grid, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& center)
      - Makes a grid over an image based on powers of two and returns
        its center.

    void matToRast(cv::Mat& src, GeoCal::GeoCalCore::RasterImage& dst)
      - converts OpenCV Mat to RasterImage, raster must be allocated.

    cv::Mat rastToMat(GeoCal::GeoCalCore::RasterImage& src)
      - converts a Raster Image to a OpenCV Mat.

    cv::Mat subImage(cv::Mat& img, cv::Point2f pt, cv::Size size)
      - Extracts a subimage from the cv::Mat.

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:


!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef IMAGEOPS_H
#define IMAGEOPS_H

#include "cv.h"
#include <vector>
#include "raster_image.h"
#include "image_coor.h"

//***************************************************************************
// Image support functions
//***************************************************************************


void findBelowThres(cv::Mat& matrix, std::vector<cv::Point2f>& belThres, 
		    double thresh = 0);
double getAmountNonZero(cv::Mat& img);
void getgrid(GeoCal::GeoCalCore::RasterImage& img, float* chip, 
	     int narray, int ndim, double trans[12], 
	     float centrx, float centry, float rmag[2], 
	     int* chop, float pcl, float pcs, int zerothr);
void imageCoorToPoint(GeoCal::GeoCalCore::ImageCoor& coor, cv::Point2f& pt);
void imageCoorToPoint(std::vector<GeoCal::GeoCalCore::ImageCoor>& coors,
		      std::vector<cv::Point2f>& pts);
bool makeGrid(GeoCal::GeoCalCore::RasterImage& img, int grid, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& center);
void matToRast(cv::Mat& src, GeoCal::GeoCalCore::RasterImage& dst);
cv::Mat rastToMat(GeoCal::GeoCalCore::RasterImage& src);
cv::Mat subImage(cv::Mat& img, cv::Point2f pt, cv::Size size);

#endif
