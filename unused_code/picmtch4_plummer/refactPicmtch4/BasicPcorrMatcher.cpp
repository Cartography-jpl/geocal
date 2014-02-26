#include "BasicPcorrMatcher.h"
#include "ImageOps.h"
#include "fftw3.h"

//***************************************************************************
// void BasicPcorrMatcher::phase_correlation(
// cv::Mat& ref, //reference image
// cv::Mat& tpl, //target image
// cv::Mat& poc) //result
//***************************************************************************
void BasicPcorrMatcher::phase_correlation(cv::Mat& ref, cv::Mat& tpl, 
					  cv::Mat& poc)
{
  int 	i, j, k;
  double	tmp;
	
  /* get image properties */
  int width  	 = ref.cols;
  int height   = ref.rows;
  int step     = (int) ref.step1();
  int fft_size = width * height;

  /* setup pointers to images */
  uchar 	*ref_data = ( uchar* ) ref.data;
  uchar 	*tpl_data = ( uchar* ) tpl.data;
  double 	*poc_data = ( double* )poc.data;
	
  /* allocate FFTW input and output arrays */
  fftw_complex *img1 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
  fftw_complex *img2 = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );
  fftw_complex *res  = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * width * height );	
	
  /* setup FFTW plans */
  fftw_plan fft_img1 = fftw_plan_dft_1d( width * height, img1, img1, FFTW_FORWARD,  FFTW_ESTIMATE );
  fftw_plan fft_img2 = fftw_plan_dft_1d( width * height, img2, img2, FFTW_FORWARD,  FFTW_ESTIMATE );
  fftw_plan ifft_res = fftw_plan_dft_1d( width * height, res,  res,  FFTW_BACKWARD, FFTW_ESTIMATE );
	
  /* load images' data to FFTW input */
  for( i = 0, k = 0 ; i < height ; i++ ) {
    for( j = 0 ; j < width ; j++, k++ ) {
      img1[k][0] = ( double )ref_data[i * step + j];
      img1[k][1] = 0.0;
			
      img2[k][0] = ( double )tpl_data[i * step + j];
      img2[k][1] = 0.0;
    }
  }
	
  /* obtain the FFT of img1 */
  fftw_execute( fft_img1 );
	
  /* obtain the FFT of img2 */
  fftw_execute( fft_img2 );
	
  /* obtain the cross power spectrum */
  for( i = 0; i < fft_size ; i++ ) {
    res[i][0] = ( img2[i][0] * img1[i][0] ) - ( img2[i][1] * ( -img1[i][1] ) );
    res[i][1] = ( img2[i][0] * ( -img1[i][1] ) ) + ( img2[i][1] * img1[i][0] );

    tmp = sqrt( pow( res[i][0], 2.0 ) + pow( res[i][1], 2.0 ) );

    res[i][0] /= tmp;
    res[i][1] /= tmp;
  }

  /* obtain the phase correlation array */
  fftw_execute(ifft_res);

  /* normalize and copy to result image */
  for( i = 0 ; i < fft_size ; i++ ) {
    poc_data[i] = res[i][0] / ( double )fft_size;
  }

  /* deallocate FFTW arrays and plans */
  fftw_destroy_plan( fft_img1 );
  fftw_destroy_plan( fft_img2 );
  fftw_destroy_plan( ifft_res );
  fftw_free( img1 );
  fftw_free( img2 );
  fftw_free( res );	
}

//***************************************************************************
// bool BasicPcorrMatcher::rangeCheck(
// cv::Point2f& searchLoc, // search location
// cv::Mat& img, // image searching in
// cv::Point2f& refLoc) // center of area to search in
//***************************************************************************
bool BasicPcorrMatcher::rangeCheck(cv::Point2f& searchLoc, cv::Mat& img, 
				   cv::Point2f& refLoc)
{
  int offset = fft_size_/2;
  cv::Point2f upperLeftRef = 
    cv::Point2f(searchLoc.x-offset,searchLoc.y-offset);

  cv::Point2f bottomRightRef = 
    cv::Point2f(searchLoc.x+offset,searchLoc.y+offset);

  offset = search_window_/2;
  cv::Point2f bottomRightWindow = cv::Point2f(refLoc.x+offset,refLoc.y+offset);

  if (upperLeftRef.x < 0 || upperLeftRef.y < 0)
    return false;
  if (bottomRightRef.x > bottomRightWindow.x || 
      bottomRightRef.y > bottomRightWindow.y)
    return false;

  return true;
}

//***************************************************************************
// BasicPcorrMatcher::BasicPcorrMatcher(
// int fft_size, //size of the fft to use to search
// int search_window, //size of the search window
// double thresh) // percentage that an image has to have nonzero data
//***************************************************************************
BasicPcorrMatcher::BasicPcorrMatcher(int fft_size, int search_window, 
				     double thresh)
{
  fft_size_ = fft_size;
  search_window_ = search_window;
  zero_thresh_ = thresh;
}

//***************************************************************************
// void BasicPcorrMatcher::match(
// cv::Mat& Ref, //reference image
// cv::Mat& New, //target image
// cv::Point2f& Ref_loc, //reference location
// cv::Point2f& New_guess, //guess in new location
// cv::Point2f& Result, //output, location in target image
// bool* Success, //match was successfull
// double* Cov) //correlation between images
//***************************************************************************
void BasicPcorrMatcher::match(cv::Mat& Ref, cv::Mat& New, 
			      cv::Point2f& Ref_loc, cv::Point2f& New_guess, 
			      cv::Point2f& Result, bool* Success, double* Cov)
{
  int offset = fft_size_/2;
  cv::Point2f searchRef = cv::Point2f(Ref_loc.x-offset,Ref_loc.y-offset);
  offset = search_window_/2;
  cv::Point2f searchNew = cv::Point2f(New_guess.x-offset,New_guess.y-offset);
  
  *Cov = 0;
  *Success = false;
  if (searchRef.x < 0 || searchRef.y < 0)
    return;

  while (rangeCheck(searchNew,New,New_guess)) {
    cv::Mat subRef = subImage(Ref,searchRef,cv::Size(fft_size_,fft_size_));
    cv::Mat subNew = subImage(New,searchNew,cv::Size(fft_size_,fft_size_));

    /* create a new image, to store phase correlation result */
    cv::Mat poc;
    poc.create(subNew.size(), IPL_DEPTH_64F);
	
    /* get phase correlation of input images */
    if (getAmountNonZero(subRef) > zero_thresh_ 
	&& getAmountNonZero(subNew) > zero_thresh_) 
      phase_correlation(subRef, subNew, poc);
    else
      continue;
      
    /* find the maximum value and its location */
    cv::Point minloc, maxloc;
    double  minval, maxval;
    minMaxLoc(poc, &minval, &maxval, &minloc, &maxloc);
      
    if (maxval > *Cov) {
      Result = cv::Point2f(New_guess.x+maxloc.x,New_guess.y-maxloc.y);
      *Success = true;
      *Cov = maxval;
    }

    searchNew = cv::Point2f(searchNew.x+5,searchNew.y);
    if (!rangeCheck(searchNew,New,New_guess))
      searchNew = cv::Point2f(New_guess.x-offset,searchNew.y+5);
  }
}
