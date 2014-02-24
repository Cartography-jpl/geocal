/*===========================================================================
=                                                                           =
=                             MspiPcorrMatcher                              =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: MisrCcorrMatcher

!ABSTRACT:

   This is an Adapter (see Design Patterns by Gamma et. al) that makes the
   shared code phase correlation matcher originally written by A.L. Zobrist
   look like a ImageMatcher class.


!DERIVED FROM:


!DESCRIPTION:


!PUBLIC MEMBERS:

    MspiPcorrMatcher(int Search_window = 128, int FFT_size = 32, 
                    int Min_search_window = 32, bool FFT_half = false, 
		    int Autofit = 0, bool Mag_shrk = false,
		    double Rmagtae[2] = NULL, double Rmagmin[2] = NULL, 
		    double Thr_res = 10.0, double Fit_min = 0, 
		    bool Predfunc = false, bool Nohpf = false, 
		    bool Subpix = true, float Retryparm[3] = NULL, 
		    float Zero_limit = 5.0, float Zero_limit2 = -1.0, 
		    int Zero_thr = 0, double Predictor_adjustment[2] = NULL)
     - Create an object for doing phase correlation, using the given
       constants.

   void match(GeoCal::GeoCalCore::RasterImage& Ref, 
	     GeoCal::GeoCalCore::RasterImage& New,
	     std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs,
	     double New_guess[12],
	     std::vector<GeoCal::GeoCalCore::ImageCoor>& Results,
	     std::vector<float*>& Cov,
	     std::vector<bool>& Success)
     - Perform phase correlation matching.

   inline virtual ~MspiPcorrMatcher()
     - Destructor.

!PROTECTED MEMBERS:

   bool getGrids(GeoCal::GeoCalCore::RasterImage& Ref, 
                 GeoCal::GeoCalCore::RasterImage& New, float pcntr[2], 
		 int choplimit1, double New_guess[12],int choplimit2, 
		 int srchw, int ifftsize)
     - Obtain the chips to perform the image matching on.

   bool refinePrediction(GeoCal::GeoCalCore::RasterImage& Ref, 
                         GeoCal::GeoCalCore::RasterImage& New,
			 GeoCal::GeoCalCore::ImageCoor& Ref_loc, 
			 double New_guess[12], int srchw, int ifftsize, 
			 bool* ffthset,double* vmaxfac, float* res, 
			 double thr_resp, int jbig, int ibigx, bool solved, 
			 GeoCal::GeoCalCore::ImageCoor& Refined_guess, 
			 GeoCal::GeoCalCore::ImageCoor& Results, float* Cov);
     - Refine the match given a prediction.

   void rfit(int ilin, int jsmp, float* vmax, float* vloff,float* vsoff, 
	    float corr[3][3], int srchdim, int fftsize);
     - Uses fft to compute the correlation function on two images.

   void updateMatch(std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
		   std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors,
		   double New_guess[12], double rctl, double rcts, 
		   int ibigx, int scount, int* throwcount, bool* solved)
     - Recalculate the prediction between the two images.

!PRIVATE MEMBERS:

   int nredo_
     - This will recalculate the user-selected number of points over again at 
       the end of processing.  Usually, the predictor has optimized by the end 
       of processing, so the results will be better.

   int min_search_window_
     - Minimum of the search area.

   int search_window_
     - Search area size (s X s).

   int fft_size_
     - fft array size, best values power of 2; at least 32.

   int autofit_
     - sets the fit_min threshhold automatically using the first
       n correlations. Must be at least 12, or zero.

   int predfunc_
     - type of function used to predict matching location, 0 for linear
       and 1 for quad.

   int zero_thr_
     - Allows default 0 to be set to a higher number for water, etc.

   int n_retry_
     - Number of retries near a location if a certain correlation threshold 
       is not acheived.

   int nEquations_max_
     - Maximum number of equations, set to 
       number of reference locations+3+nredo_.

   bool mag_shrk_
     - When true the magnify parameter will shrink by .9 for every good 
       fitting point. Only applies after fit to points is obtained.  Once 
       magmin is reached, then the shrink is no longer applied.

   bool subpix_
     - Specifies that subpixel correlation is to be calculated.

   bool no_hpf_
     - Specifies the default high pass filter in the correlation is to be 
       shut off.  The high pass filter consists of zeroing the low order 
       row and column of the DFT matrices.

   bool fft_half_
     - will do retries at half size if zero_limit is exceeded (near edge).

   float zero_limit_
     - Do not allow correlation if either image area exceeds this percentage 
       of zeros.

   float zero_limit2_
     - Sets zero_limit for second image, -1 sets it to same as first.

   float rmag_[2]
     - Magnification of sampling

   double rmagmin_[2]
     - Specifies the minimum values for rmag_ when mag_shrk_ is used.

   double thr_res_
     - Prevents offset correlations (further than this value from the 
       predicted location) from entering into the model fit from image 1 to 
       image 2.

   double fit_min_
     - Prevents weak correlations from entering intot he model fit of image 1 
       to image 2.

   double r_retry_
     - The retry pattern to move to the corners of a square 
       (+/- r_retry_, +/- r_retry_) from the original point

   double t_retry_
     - Correlation threshold required to stop the retry at each point.

   double predictor_adjustment_[2]
     - Values by which to adjust the predictor.

!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

   Due to interative nature of the least squares routine, the results will 
   vary slightly depending on the machine.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef _PICMTCH4MATCHER_H
#define _PICMTCH4MATCHER_H

#include "picmtch4.h"
#include "HDFEOS5Image.h"
#include "fftw3.h"

//***************************************************************************
// Declaration of class MspiPcorrMatcher.
//***************************************************************************

class MspiPcorrMatcher
{
 public:
  MspiPcorrMatcher(int Search_window = 128, int FFT_size = 32, 
		   int Min_search_window = 32, bool FFT_half = false, 
		   int nRedo = 12, int Autofit = 0, bool Mag_shrk = false, 
		   double Rmagtae[2] = NULL, double Rmagmin[2] = NULL, 
		   double Thr_res = 10.0, double Fit_min = 0, 
		   bool Predfunc = false, bool Nohpf = false, 
		   bool Subpix = true, float Retryparm[3] = NULL, 
		   float Zero_limit = 5.0, float Zero_limit2 = -1.0, 
		   int Zero_thr = 0, double Predictor_adjustment[2] = NULL);
  ~MspiPcorrMatcher();
  void match(GeoCal::GeoCalCore::RasterImage& Ref, 
	     GeoCal::GeoCalCore::RasterImage& New,
	     std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs,
	     double New_guess[12],
	     std::vector<GeoCal::GeoCalCore::ImageCoor>& Results,
	     std::vector<float*>& Cov,
	     std::vector<bool>& Success);
 protected:
  bool getGrids(GeoCal::GeoCalCore::RasterImage& Ref, 
		GeoCal::GeoCalCore::RasterImage& New, float pcntr[2], 
		int choplimit1, double New_guess[12], int choplimit2, 
		int srchw, int ifftsize);
  bool refinePrediction(GeoCal::GeoCalCore::RasterImage& 
			Ref, GeoCal::GeoCalCore::RasterImage& New, 
			GeoCal::GeoCalCore::ImageCoor& Ref_loc, 
			double New_guess[12], int srchw, int ifftsize, 
			bool* ffthset,double* vmaxfac, float* res, 
			double thr_resp, int jbig, int ibigx, bool solved, 
			GeoCal::GeoCalCore::ImageCoor& Refined_guess, 
			GeoCal::GeoCalCore::ImageCoor& Results, float* Cov);
  void rfit(int ilin, int jsmp, float* vmax, float* vloff,float* vsoff, 
	    float corr[3][3], int srchdim, int fftsize);
  void updateMatch(std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
		   std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors,
		   double New_guess[12], double rctl, double rcts, 
		   int ibigx, int scount, int* throwcount, bool* solved);
 private:
  int nredo_, min_search_window_, search_window_, fft_size_, autofit_;
  int predfunc_, zero_thr_, n_retry_, *vmaxix_, nEquations_max_;
  bool mag_shrk_, subpix_, no_hpf_, fft_half_;
  float zero_limit_, zero_limit2_, *vmaxvec_, *chip1_, *asrch_, rmag_[2];
  double rmagmin_[2], thr_res_, fit_min_, r_retry_, t_retry_;
  double predictor_adjustment_[2];
};

#endif
