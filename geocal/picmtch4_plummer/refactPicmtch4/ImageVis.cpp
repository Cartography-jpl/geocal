/*===========================================================================
=                                                                           =
=                             ImageVis                                      =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: ImageVis

!ABSTRACT:

   Member functions of ImageVis class.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/
#include "ImageVis.h"
#include "highgui.h"

//***************************************************************************
// cv::Mat ImageVis::convertGrayToSingleColor(
// cv::Mat& img, // image to convert
// RGBColor colorType) // color to convert image to
//***************************************************************************
cv::Mat ImageVis::convertGrayToSingleColor(cv::Mat& img, RGBColor colorType)
{
  cv::Mat colorImg;
  cvtColor(img,colorImg,CV_GRAY2BGR);
  std::vector<cv::Mat> channels;
  split(colorImg,channels);
  switch (colorType) {
  case BLUE:
    channels[RED] = cv::Scalar(0);
    channels[GREEN] = cv::Scalar(0);
    break;
  case GREEN:
    channels[BLUE] = cv::Scalar(0);
    channels[RED] = cv::Scalar(0);
    break;
  case RED:
    channels[BLUE] = cv::Scalar(0);
    channels[GREEN] = cv::Scalar(0);
    break;
  }

  merge(channels,colorImg);
  return colorImg;
}

//***************************************************************************
// void ImageVis::displayAllMatchedPoints(
// cv::Mat& ref, // reference image
// cv::Mat& target, // target image
// std::vector<cv::Point2f>& Ref_locs,  // location in reference image 
// std::vector<cv::Point2f>& Results) // corresponding loc. in target image
//***************************************************************************
void ImageVis::displayAllMatchedPoints(cv::Mat& ref, cv::Mat& target, 
				       std::vector<cv::Point2f>& Ref_locs, 
				       std::vector<cv::Point2f>& Results)
{
  //------------------------------------------------------------------------
  // Create image to copy input images into and get each images location
  // in that image
  //------------------------------------------------------------------------
  cv::Size sizeBothImages = ref.size();
  IplImage* bothImages = 
    cvCreateImage(sizeBothImages,ref.depth(),ref.channels());

  cv::Rect leftSubImage = cv::Rect(cv::Point2f(0,0),ref.size());
  cv::Rect rightSubImage = cv::Rect(ref.rows,0,target.cols*2,target.rows);
  cvSetImageROI(bothImages,leftSubImage);

  //------------------------------------------------------------------------
  // Copy both images into the combined image
  //------------------------------------------------------------------------
  IplImage refIpl = ref;
  IplImage targetIpl = target;
  cvCopyImage(&refIpl,bothImages);
  cvSetImageROI(bothImages,rightSubImage);
  cvCopyImage(&targetIpl,bothImages);
  cvResetImageROI(bothImages);

  //------------------------------------------------------------------------
  // Convert Grayscale image to a color image and draw lines between matched
  // points
  //------------------------------------------------------------------------
  cv::Mat colorImg;
  cv::Mat bothMat = bothImages;
  cvtColor(bothMat,colorImg,CV_GRAY2RGB);
  int sndImgStart = ref.cols;
  int thickness = 5;
  int lineType = 8;
  int shift = 0;
  cv::RNG rng(0xFFFFFFFF);

  for (int i = 0; i < Ref_locs.size();i++) {
    cv::Point2f newResultLoc = 
      cv::Point2f(Results[i].x+sndImgStart,Results[i].y);

    line(colorImg,Ref_locs[i],newResultLoc,randomColor(rng),
	 thickness,lineType,shift);
  }

  cv::namedWindow( "AllMatches", CV_WINDOW_AUTOSIZE );	
	
  imshow( "AllMatches", colorImg );
	
  cv::waitKey( 0 );

  cv::destroyWindow( "AllMatches" );
}

//***************************************************************************
// void ImageVis::displayCombinedWarped(
// cv::Mat ref, // Reference image
// cv::Mat target, // Target image
// cv::Mat warped) // Warped target image
//***************************************************************************
void ImageVis::displayCombinedWarped(cv::Mat ref, cv::Mat target, 
				     cv::Mat warped)
{
  cv::Mat combinedOrig,combinedWarped;

  addWeighted(ref,1,target,1,0,combinedOrig);
  addWeighted(ref,1,warped,1,0,combinedWarped);
 
  cv::namedWindow( "Target", CV_WINDOW_NORMAL );
  cv::namedWindow( "Reference", CV_WINDOW_NORMAL );	

  imshow( "Target", combinedWarped );
  imshow( "Reference", combinedOrig );
  cv::waitKey( 0 );

  cv::destroyWindow( "Target" );
  cv::destroyWindow( "Reference" );
}

//***************************************************************************
// void ImageVis::displayEntireMatchedRegion(
// cv::Mat& ref, // image to draw on
// std::vector<cv::Rect>& rects) // rectangles that have been matched
//***************************************************************************
void ImageVis::displayEntireMatchedRegion(cv::Mat& ref, 
					  std::vector<cv::Rect>& rects)
{
  int colorIntensity = 1000000; // make sure rectangles are white
  int thickness = 5;
  int line_type = 8;
  int shift = 0;
  for (int i = 0; i < rects.size();i++)
    rectangle(ref,rects[i],colorIntensity,thickness,line_type,shift);
  
  cv::namedWindow( "MatchedCells", CV_WINDOW_NORMAL );
  imshow("MatchedCells",ref);
  cv::waitKey( 0 );
  cv::destroyWindow( "MatchedCells" );
}

//***************************************************************************
// void ImageVis::displayResultsFromPoints(
// cv::Mat& ref, // image to draw on
// std::vector<cv::Point2f>& Ref_locs, // location from reference image 
// std::vector<cv::Point2f>& Results) // Results from matching
//
// This function should really take in the Success vector from 
// MspiPCorrMatcher instead of the Results vector.
//***************************************************************************
void ImageVis::displayResultsFromPoints(cv::Mat& ref, 
					std::vector<cv::Point2f>& Ref_locs, 
					std::vector<cv::Point2f>& Results)
{
  std::vector<cv::Rect> rects;
  int width;
  int height = Ref_locs[1].y - Ref_locs[0].y;
  for (int i = 0; i < Ref_locs.size()-1; i++) {
    if (Ref_locs[i+1].x != Ref_locs[i].x) {
      width = Ref_locs[i+1].x - Ref_locs[i].x;
      break;
    }
  }

  for (int i = 0; i < Ref_locs.size(); i++) {
    if (Results[i].x >=0 && Ref_locs[i].x >= 0)
      rects.push_back(cv::Rect(Ref_locs[i].x-width/2, Ref_locs[i].y-height/2, width, height));
  }

  displayEntireMatchedRegion(ref,rects);
}

//***************************************************************************
// void ImageVis::displaySingleMatch(
// cv::Mat& ref, // reference image
// cv::Mat& target, // target image
// cv::Point2f& ref_loc, // location in reference image
// cv::Point2f& new_loc, // matched location in target image
// bool printMatchingPoints = false) // print the coordinate values to screen
//***************************************************************************
void ImageVis::displaySingleMatch(cv::Mat& ref, cv::Mat& target, 
				  cv::Point2f& ref_loc, cv::Point2f& new_loc, 
				  bool printMatchingPoints)
{
  if (printMatchingPoints) {
    printf("Reference Point: (%g, %g)  Target Point: (%g, %g)\n", 
	   ref_loc.x, ref_loc.y, new_loc.x, new_loc.y);
  }

  cv::Mat new_image = cv::Mat::zeros( ref.size(), ref.type() );
  cv::Mat new_image2 = cv::Mat::zeros( target.size(), target.type() );
  double alpha = 0.9; // contrast
  int beta = 50; // brightness
  for(int i = 0; i < ref.rows; i++) {
    uchar* rowPtr = ref.ptr(i);
    uchar* rowPtr2 = new_image.ptr(i);
    uchar* rowPtrb = target.ptr(i);
    uchar* rowPtrb2 = new_image2.ptr(i);
    for(int j = 0; j < ref.cols; j++) {
      rowPtr2[j] = cv::saturate_cast<uchar>( alpha*( rowPtr[j] + beta ) );
      rowPtrb2[j] = cv::saturate_cast<uchar>( alpha*( rowPtrb[j] + beta ) );
    }
  }
  cv::namedWindow( "Target", CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "Reference", CV_WINDOW_AUTOSIZE );	

  imshow("Target",new_image2);
  imshow("Reference",new_image);
	
  cv::waitKey( 0 );

  cv::destroyWindow( "Target" );
  cv::destroyWindow( "Reference" );
}

//***************************************************************************
// bool ImageVis::drawImBorder(
// cv::Mat& src, // image to locate bounding box
// cv::Mat& dst, // image to draw on
// bool showBorder = false) // display result
//***************************************************************************
bool ImageVis::drawImBorder(cv::Mat& src, cv::Mat& dst, bool showBorder)
{
  std::vector<cv::Point2f> left;
  std::vector<cv::Point2f> right;

  //------------------------------------------------------------------------
  // get bounding box
  //------------------------------------------------------------------------
  for (int i = 0; i < src.cols;i++) {
    cv::Mat col = src.col(i);
    std::vector<cv::Point2f> nonZeros;
    findBelowThres(src,nonZeros);
    int nonZeroCount = nonZeros.size();

    if (nonZeroCount > 0) {
      right.push_back(cv::Point2f(i, nonZeros[nonZeroCount-1].y));
      left.push_back(cv::Point2f(i, nonZeros[0].y));
    }

  }

  if (right.size() == 0 || left.size() == 0 || right.size() != left.size())
    return false;
 
  //------------------------------------------------------------------------
  // Draw the border
  //------------------------------------------------------------------------

  cv::Scalar color = CV_RGB(1000, 1000, 1000); // CV_RGB( Red, Green, Blue)
  int thickness = 5;
  int line_type = 8;
  int shift = 0;
  int contours = 2;
  bool isClosed = false;
  cv::Point pt[2][left.size()];
  for (int i = 0; i < left.size(); i++) {
    pt[0][i] = cv::Point(left[i].x,left[i].y);
    pt[1][i] = cv::Point(right[i].x,right[i].y);
  }

  const cv::Point* ppt[2] = {pt[0], pt[1]};
  int npt[] = {left.size(),right.size()};
    
  cv::polylines(dst,ppt,npt,contours,isClosed,color,thickness,line_type,shift);
    
  if (showBorder) {
    cv::namedWindow( "Border", CV_WINDOW_NORMAL );
    imshow("Border",dst);
    cv::waitKey( 0 );
    cv::destroyWindow( "Border" );
  }
  
  return true;
}

//***************************************************************************
// bool ImageVis::drawImBoundingBox(
// cv::Mat& img, // image to find box
// cv::Rect& box, // bounding box
// bool showBox = false) // display result
//***************************************************************************
bool ImageVis::drawImBoundingBox(cv::Mat& img, cv::Rect& box, bool showBox)
{
  int left = -1;
  int right = -1;
  int top = -1;
  int bottom = -1;
  for (int i = 0; i < img.cols;i++) {
    cv::Mat row = img.row(i);
    int nonZeros = countNonZero(row);
    if (top < 0 && nonZeros > 0)
      top = i;

    if (nonZeros > 0)
      bottom = i;
  }

  for (int i = 0; i < img.rows;i++) {
    cv::Mat col = img.col(i);
    int nonZeros = countNonZero(col);
    if (left < 0 && nonZeros > 0)
      left = i;
    
    if (nonZeros > 0)
      right = i;
  }

  if (left >= 0 && right >= 0 && bottom >= 0 && top >= 0)
    box = cv::Rect(left,top,right-left,bottom-top);
  else
    return false;

  if (showBox) {
    std::vector<cv::Rect> rects;
    rects.push_back(box);
    displayEntireMatchedRegion(img,rects);
  }
  
  return true;
}

//***************************************************************************
// void ImageVis::drawPoint(
// cv::Mat& ref, // image to draw on
// cv::Point2f& center, // center of point to draw
// bool rgb = false) // is image RGB
//***************************************************************************
void ImageVis::drawPoint(cv::Mat& ref, cv::Point2f& center, bool rgb)
{
  int radius = 8;
  cv::Scalar color;

  if (!rgb)
    color = CV_RGB(1000, 1000, 1000); // White point if grayscale
  else
    color = CV_RGB(1000,0,0); // Red point if is color image

  int thickness = 5;
  int line_type = 8;
  int shift = 0;
  circle(ref,center,radius,color,thickness,line_type,shift);
}

//***************************************************************************
// cv::Scalar ImageVis::randomColor(
// cv::RNG& rng) // random number generator to produce color with
//***************************************************************************
cv::Scalar ImageVis::randomColor(cv::RNG& rng)
{
    int icolor = (unsigned)rng;
    return cv::Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}
