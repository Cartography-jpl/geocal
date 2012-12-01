/*===========================================================================
=                                                                           =
=                             MspiPcorrMatcher                              =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: MspiPcorrMatcher

!ABSTRACT:

   This contains routines for the class MspiPcorrMatcher.

!REVISION HISTORY:
   image to image correlation  A. Zobrist 9/9/00 
   00-09 ...alz... initial version 
=============================================================================
!END=======================================================================*/

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "fftw3.h"

#include "MspiPcorrMatcher.h"
#include "image_coor.h"

extern "C" {
#include "carto/cartoSortUtils.h"
}

//***************************************************************************
// Globals used for calculations, should remain const
//***************************************************************************
float rvecl[5] = {0.,1.,-1.,1.,-1.};
float rvecs[5] = {0.,1.,-1.,-1.,1.};
double ident[12] = {1.,0.,0.,0.,0.,0.,0.,1.,0.,0.,0.,0.};

//***************************************************************************
// MspiPcorrMatcher::MspiPcorrMatcher
// int Search_window = 128, int FFT_size = 32, 
// int Min_search_window = 32, bool FFT_half = false, 
// int nRedo = 12, int Autofit = 0, bool Mag_shrk = false, 
// double Rmagtae[2] = NULL, double Rmagmin[2] = NULL, 
// double Thr_res = 10.0, double Fit_min = 0, 
// bool Predfunc = false, bool Nohpf = false, 
// bool Subpix = true, float Retryparm[3] = NULL, 
// float Zero_limit = 5.0, float Zero_limit2 = -1.0, 
// int Zero_thr = 0, double Predictor_adjustment[2] = NULL)
//
// See class definition for comments on function of each variable
//***************************************************************************
MspiPcorrMatcher::MspiPcorrMatcher(int Search_window, int FFT_size, 
				   int Min_search_window, bool FFT_half, 
				   int nRedo, int Autofit, bool Mag_shrk, 
				   double Rmagtae[2], double Rmagmin[2], 
				   double Thr_res, double Fit_min, 
				   bool Predfunc, bool Nohpf, 
				   bool Subpix, float Retryparm[3], 
				   float Zero_limit, float Zero_limit2, 
				   int Zero_thr, double Predictor_adjustment[2])
{
  nredo_ = nRedo;
  search_window_ = Search_window;
  fft_size_ = FFT_size;
  fft_half_ = FFT_half;
  autofit_ = Autofit;
  mag_shrk_ = Mag_shrk;
  zero_limit_ = Zero_limit;
  zero_limit2_ = Zero_limit2;

  if (Rmagmin) {
    rmagmin_[0] = Rmagmin[0];
    rmagmin_[1] = Rmagmin[1];
  }
  else {
    rmagmin_[0] = 1;
    rmagmin_[1] = 1;
  }

  thr_res_ = Thr_res;
  predfunc_ = Predfunc;
  subpix_ = Subpix;
  fit_min_ = Fit_min;
  min_search_window_ = Min_search_window;
  zero_thr_ = Zero_thr;
  
  if (Predictor_adjustment) {
    predictor_adjustment_[0] = Predictor_adjustment[0];
    predictor_adjustment_[1] = Predictor_adjustment[1];
  }
  else {
    predictor_adjustment_[0] = 0;
    predictor_adjustment_[1] = 0;
  }

  if (search_window_%2 == 1) 
    search_window_++;
  if (min_search_window_%2 == 1) 
    min_search_window_++;
  if (fft_size_%2 == 1) 
    fft_size_++;
  
  if (min_search_window_ < fft_size_)
    min_search_window_ = fft_size_;
  if (search_window_ < fft_size_) 
    search_window_ = fft_size_;
  if (search_window_ < min_search_window_) 
    search_window_ = min_search_window_;
   
  zero_limit_ *= 0.01;
  
  if (zero_limit2_ < -0.5) 
    zero_limit2_ = zero_limit_*100.0;

  zero_limit2_ *= 0.01;
  
  if (Retryparm) {
    n_retry_ = (int) Retryparm[0]+1;
    r_retry_ = (double) Retryparm[1];
    t_retry_ = (double) Retryparm[2];
  }
  else {
    n_retry_ = 1;
    r_retry_ = 0;
    t_retry_ = 0;
  }
 
  if (fft_half_) {
    if (n_retry_ > 1) {
      GeoCal::GeoCalCore::Exception e;
      e << "can't use both RETRY and FFTHALF parameters";
      throw e;
    }

    n_retry_ = fft_half_+1;
  }

  if (autofit_ > 0 && autofit_ < 12) {
    GeoCal::GeoCalCore::Exception e;
    e << "autofit must be at least 12; or zero (autofit off)";
    throw e;
  }

  if (autofit_ > 0) {
    vmaxvec_ = new float[autofit_];
    vmaxix_ = new int[autofit_];
  }
  else {
    vmaxvec_ = NULL;
    vmaxix_ = NULL;
  }

  if (Rmagtae) {
    rmag_[0] = Rmagtae[0]; 
    rmag_[1] = Rmagtae[1];
  }
  else {
    rmag_[0] = 1;
    rmag_[1] = 1;
  }

  no_hpf_ = Nohpf;

  chip1_ = new float[fft_size_*fft_size_];
  asrch_ = new float[search_window_*search_window_];
}

//***************************************************************************
// MspiPcorrMatcher::~MspiPcorrMatcher()
// Destructor
//***************************************************************************
MspiPcorrMatcher::~MspiPcorrMatcher()
{
  delete[] chip1_;
  delete[] asrch_;
  
  if (vmaxvec_)
    delete[] vmaxvec_;
  if (vmaxix_)
    delete[] vmaxix_;
}

//***************************************************************************
// void MspiPcorrMatcher::match
// (GeoCal::GeoCalCore::RasterImage& Ref, //reference image 
// GeoCal::GeoCalCore::RasterImage& New, //target image
// std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs, //reference points
// double New_guess[12], //initial guess at transformation 
// std::vector<GeoCal::GeoCalCore::ImageCoor>& Results, //result outpoints
// std::vector<float*>& Cov, //Correlation between images
// std::vector<bool>& Success)  //if match was successful
//***************************************************************************
void MspiPcorrMatcher::match
(GeoCal::GeoCalCore::RasterImage& Ref, GeoCal::GeoCalCore::RasterImage& New,
 std::vector<GeoCal::GeoCalCore::ImageCoor>& Ref_locs,
 double New_guess[12],
 std::vector<GeoCal::GeoCalCore::ImageCoor>& Results, 
 std::vector<float*>& Cov, std::vector<bool>& Success)
{  
  double rctl = 0.5*New.number_line();
  double rcts = 0.5*New.number_sample();
  
  int lastneq = 3; 
  int autoix = 0; 
  bool gotthresh = false; 
  int throwcount = 4; 
  int scount = 1;
  int srchw = search_window_-fft_size_; 
  int msrchw = min_search_window_-fft_size_; 
  bool solved = false; 
  double vmaxfac = 0.9;
  int maxredo = MIN(nredo_,Ref_locs.size());
  double thr_resp = 4.0*thr_res_;
  int nEquationsMin = MAX(autofit_/2+4,9);

  //------------------------------------------------------------------------
  // Save original guess for retry case
  //------------------------------------------------------------------------

  double orig_Newguess[12];
  memcpy(orig_Newguess,New_guess,12*sizeof(double));
 
  std::vector<GeoCal::GeoCalCore::ImageCoor> fstImgCoors;
  std::vector<GeoCal::GeoCalCore::ImageCoor> sndImgCoors;
  std::vector<GeoCal::GeoCalCore::ImageCoor> Refined_guess;

  nEquations_max_ = Ref_locs.size()+3+nredo_;

  for (int i = 0; i < Ref_locs.size(); i++) {
    float* c_data = new float[3];
    memset(c_data,0,3*sizeof(float));
    Cov.push_back(c_data);
  }

  //------------------------------------------------------------------------
  // Set initial values
  //------------------------------------------------------------------------

  Results.insert(sndImgCoors.begin(),Ref_locs.size(),
		 GeoCal::GeoCalCore::ImageCoor(-9999.0,-9999.0));
  Refined_guess.insert(sndImgCoors.begin(),Ref_locs.size(),
		       GeoCal::GeoCalCore::ImageCoor(-9999.0,-9999.0));
  Success.insert(Success.begin(),Ref_locs.size(),false);
 
  for (int ibigx = 0; ibigx < (Ref_locs.size()+maxredo); ibigx++) {
    int ibig = ibigx%Ref_locs.size();
    Cov[ibig][0] = -9999.;
    Cov[ibig][1] = 0.;
    Cov[ibig][2] = -9999.;
    Results[ibig] = GeoCal::GeoCalCore::ImageCoor(-9999.0,-9999.0);
    int ifftsize = fft_size_;  
    bool ffthset = false;

    GeoCal::GeoCalCore::ImageCoor Ref_loc = Ref_locs[ibig];
    for (int jbig = 0; jbig < n_retry_; jbig++) {
  
      if ( (jbig > 0) && (fft_half_ > 0) ) {
	if (ibigx < autofit_*2) 
	  continue;
	if (ffthset) 
	  continue;

	ifftsize /= 2;
      }

      ffthset = false;
	 
      //------------------------------------------------------------------------
      // do linear fit of tiepoints and matches to predict match 
      // inverse linear fit needed for getgrid outputs 
      // can't mix 3 initial points with updates
      //------------------------------------------------------------------------

      if ( ((fstImgCoors.size() >= nEquationsMin)) 
	   && (fstImgCoors.size() != lastneq) ) 
	{
	  updateMatch(fstImgCoors,sndImgCoors,New_guess,rctl,rcts,ibigx,
		      scount,&throwcount,&solved);
	  scount++;
	  lastneq = fstImgCoors.size();
	}

      float xlt = Ref_loc.line()+r_retry_*rvecl[jbig];
      float xst = Ref_loc.sample()+r_retry_*rvecs[jbig];
      Ref_loc = GeoCal::GeoCalCore::ImageCoor(xlt,xst);
 
      float res;
      if (!refinePrediction(Ref,New,Ref_loc,New_guess,srchw,ifftsize,&ffthset,
			    &vmaxfac,&res,thr_resp,jbig,ibigx,solved,
			    Refined_guess[ibig],Results[ibig],Cov[ibig]))
	continue;

      if (solved) 
	thr_resp = MAX(thr_resp*0.95,thr_res_);

      Cov[ibig][1] = 1.;
      
      //------------------------------------------------------------------------
      // Resetting for retry case
      //------------------------------------------------------------------------

      if (jbig > 0 && !fft_half_) {
	double line = orig_Newguess[0]*(double)xlt+orig_Newguess[1]
	  *(double)xst+orig_Newguess[2];

	double sample = orig_Newguess[3]*(double)xlt+orig_Newguess[4]
	  *(double)xst+orig_Newguess[5];

	Ref_locs[ibig] = GeoCal::GeoCalCore::ImageCoor(line,sample);
      }

      if (autoix < autofit_) {
	vmaxvec_[autoix] = Cov[ibig][0];
	vmaxix_[autoix++] = ibig+1;
      }

      if (!gotthresh && (autoix >= autofit_) && (autofit_ > 0)) {
	sort7(vmaxvec_,vmaxix_,autoix);
	fit_min_ = vmaxvec_[autoix/2];
	printf("***auto thresh  = %8.4f ***\n",fit_min_);
	gotthresh = true;

	//-------------------------------------------------------------------
	// recover points above newly set threshold
	//-------------------------------------------------------------------

	for (int ie = 0; ie < autoix; ie++) {
	  if (vmaxvec_[ie] > fit_min_) {
	    int ibigt = vmaxix_[ie]-1;
	    Cov[ibig][1] = 2.;
	    
	    fstImgCoors.push_back
	      (GeoCal::GeoCalCore::ImageCoor(Ref_locs[ibigt].line(),
					     Ref_locs[ibigt].sample())
	       );

	    sndImgCoors.push_back( Refined_guess[ibigt]);
	  }
	}

	break;
      }

      if (autoix < autofit_) 
	break;
      if (Cov[ibig][0] < fit_min_) 
	break;

      Cov[ibig][1] = 2.;
      fstImgCoors.push_back(Ref_loc);
      sndImgCoors.push_back(Refined_guess[ibig]);
      
      if (fstImgCoors.size() != sndImgCoors.size()) {
	GeoCal::GeoCalCore::Exception e;
	e << "Number of Coordinates for each image must be equal\n";
	throw e;
      }
      
      if (fstImgCoors.size() < nEquationsMin) 
	break;

      if (srchw > msrchw) 
	vmaxfac = vmaxfac*1.05;
      if (rmag_[0] > rmagmin_[0]) 
	vmaxfac = MAX(0.50,vmaxfac*0.9825);
      srchw = MIN(MAX(msrchw,(4*srchw+2*(int)res)/5),srchw);

      if (srchw%2 == 1) 
	srchw++;
      if (10*srchw < (11*msrchw+ifftsize)) 
	srchw = msrchw;
      if (mag_shrk_ && (rmag_[0] > rmagmin_[0])) 
	rmag_[0] = MAX(rmag_[0]*0.9,rmagmin_[0]);
      if (mag_shrk_ && (rmag_[1] > rmagmin_[1])) 
	rmag_[1] = MAX(rmag_[1]*0.9,rmagmin_[1]);

      break;
    }
  }

  //------------------------------------------------------------------------
  // Calculate line and sample residuals and check for successesful
  //  matches
  //------------------------------------------------------------------------

  for (int i = 0; i < (Ref_locs.size()); i++) {
    if (Cov[i][1] > 0.5) {
      double Ref_line = Ref_locs[i].line();
      double Ref_sample = Ref_locs[i].sample();
      Cov[i][1] = Refined_guess[i].line()-
	(Ref_line*New_guess[0]+Ref_sample*New_guess[1]+New_guess[2]+
	 Ref_line*Ref_line*New_guess[3]+
	 Ref_sample*Ref_sample*New_guess[4]+
	 Ref_line*Ref_sample*New_guess[5]);
      Cov[i][2] = Refined_guess[i].sample()-
	(Ref_line*New_guess[6]+Ref_sample*New_guess[7]+New_guess[8]+
	 Ref_line*Ref_line*New_guess[9]+
	 Ref_sample*Ref_sample*New_guess[10]+
	 Ref_line*Ref_sample*New_guess[11]);
    }
    else 
      Cov[i][1] = -9999.;

    if (Results[i].line() > 0 && Results[i].sample() > 0)
      Success[i] = true;
  }
  
  return;
}

//***************************************************************************
// bool MspiPcorrMatcher::getGrids(
// GeoCal::GeoCalCore::RasterImage& Ref, //reference image
// GeoCal::GeoCalCore::RasterImage& New, //target image
// float pcntr[2], //center of image chip in Ref
// int choplimit1, //most that can be chopped from Ref subimage
// double New_guess[12], //transformation from Ref to New
// int choplimit2, //most that can be chopped from New subimage
// int srchw, //search window
// int ifftsize) //FFT size
//***************************************************************************
bool MspiPcorrMatcher::getGrids(GeoCal::GeoCalCore::RasterImage& Ref, 
				GeoCal::GeoCalCore::RasterImage& New, 
				float pcntr[2], int choplimit1,
				double New_guess[12],int choplimit2, 
				int srchw, int ifftsize)
{
  int chop;
  int bigchoplimit2 = search_window_*search_window_/2; // see zerolimit in rfit
  
  getgrid(Ref,chip1_,ifftsize,ifftsize,ident,pcntr[0],pcntr[1],
	  rmag_,&chop,0.0,0.0,zero_thr_);

  if (chop > choplimit1) {

    //if (jbig == n_retry_-1) 
    //printf("point outside first image\n");
    return false;
  }

  //------------------------------------------------------------------------
  // get the second grid and do correlation, 2d grid can be larger
  //------------------------------------------------------------------------

  int ndim = MIN(search_window_,srchw+ifftsize+2);
  if (ndim%2 == 1) 
    ndim++;
  
  getgrid(New,asrch_,search_window_,ndim,New_guess,pcntr[0],pcntr[1],
	  rmag_,&chop, predictor_adjustment_[0],predictor_adjustment_[1],
	  zero_thr_);

  if ((chop > bigchoplimit2) || ((srchw == 0) && (chop > choplimit2)) ) {

    //if (jbig == n_retry_-1) 
      //printf("point outside second image\n");
    return false;
  }

  return true;
}


//***************************************************************************
// bool MspiPcorrMatcher::refinePrediction
// (GeoCal::GeoCalCore::RasterImage& Ref, //reference image
// GeoCal::GeoCalCore::RasterImage& New, //target image
// GeoCal::GeoCalCore::ImageCoor& Ref_loc, //location in ref image to match
// double New_guess[12], //transformation from Ref to New
// int srchw, //search window
// int ifftsize, //FFT size
// bool* ffthset, 
// double* vmaxfac,
// float* res, 
// double thr_resp, 
// int jbig, //counter for retry on same image
// int ibigx, //index of current Ref_loc
// bool solved, 
// GeoCal::GeoCalCore::ImageCoor& Refined_guess, //output guess
// GeoCal::GeoCalCore::ImageCoor& Results, //output result
// float* Cov) //Correlation
//***************************************************************************
bool MspiPcorrMatcher::refinePrediction
(GeoCal::GeoCalCore::RasterImage& Ref, GeoCal::GeoCalCore::RasterImage& New, 
 GeoCal::GeoCalCore::ImageCoor& Ref_loc, double New_guess[12], int srchw, 
 int ifftsize, bool* ffthset, double* vmaxfac, float* res, double thr_resp, 
 int jbig, int ibigx, bool solved, 
 GeoCal::GeoCalCore::ImageCoor& Refined_guess, 
 GeoCal::GeoCalCore::ImageCoor& Results, float* Cov)
{

  //------------------------------------------------------------------------
  // Get chips to match between the images
  //------------------------------------------------------------------------

  int choplimit1 = (int) (ifftsize*ifftsize*zero_limit_);
  int choplimit2 = (int) (ifftsize*ifftsize*zero_limit2_);

  float pcntr[2];
  pcntr[0] = (int) (Ref_loc.line()+0.5);
  pcntr[1] = (int) (Ref_loc.sample()+0.5);
  if (!getGrids(Ref,New,pcntr,choplimit1,New_guess,choplimit2,srchw,ifftsize))
    return false;

  *ffthset = true;

  //printf("%5d (%7.1f %7.1f)%5dx%3.1f",
  //	 ibig+1,pcntr[0],pcntr[1],srchw+ifftsize,rmag_[1]);
  if ((srchw > 0) && (ifftsize < fft_size_)) {
    //printf("#\n");
    return false;
  }
  
  //------------------------------------------------------------------------
  // Calculate the correlation between the chips from the images
  //------------------------------------------------------------------------

  int srchdim;
  if (ifftsize == fft_size_) 
    srchdim = fft_size_+srchw; 
  else 
    srchdim = ifftsize;
  int ilin = (search_window_-srchdim)/2;
  int jsmp = ilin;
     
  float vloff, vsoff, corr[3][3], vmax;	 
  rfit(ilin,jsmp,&vmax,&vloff,&vsoff,corr,srchdim,ifftsize);

  if (vmax < 0.00001) { 
    //printf("refine err 1a\n"); 
    return false; 
  }
  if (vmax < t_retry_) { 
    //printf("  low correlation\n"); 
    return false; 
  }
        	 
  if ( (jbig == 0) && (vmax < ((*vmaxfac)*fit_min_)) 
       && (ibigx > (autofit_*2)) ) 
    {
      *vmaxfac = MAX(0.50,(*vmaxfac)*0.99825); 
      //printf("*\n");
      return false;
    }

  //------------------------------------------------------------------------
  // refine the match produced by rfit
  //------------------------------------------------------------------------

  bool refinerr = false;
  if (subpix_) 
    refine(corr,&vloff,&vsoff,&refinerr);

  vloff *= rmag_[0];
  vsoff *= rmag_[1];
  if (refinerr) { 
    //printf("refine err 2\n"); 
    return false; 
  }

  Cov[0] = vmax;

  //------------------------------------------------------------------------
  // Calculate center of matched grid
  //------------------------------------------------------------------------
  
  float ocentr[2];
  ocentr[0] = ((float)ilin-(search_window_-srchdim)*0.5)*rmag_[0]+vloff;
  ocentr[1] = ((float)jsmp-(search_window_-srchdim)*0.5)*rmag_[1]+vsoff;
  float wl = Ref_loc.line()+ocentr[0];
  float ws = Ref_loc.sample()+ocentr[1];

  float New_line = New_guess[0]*wl+New_guess[1]*ws+New_guess[2]+New_guess[3]*
    wl*wl+New_guess[4]*ws*ws+New_guess[5]*wl*ws;

  float New_sample = New_guess[6]*wl+New_guess[7]*ws+New_guess[8]+New_guess[9]*
    wl*wl+New_guess[10]*ws*ws+New_guess[11]*wl*ws;
  Refined_guess = GeoCal::GeoCalCore::ImageCoor(New_line,New_sample);

  //------------------------------------------------------------------------
  // Account for predictor adjustment in result
  //------------------------------------------------------------------------
    
  wl += predictor_adjustment_[0];
  wl += predictor_adjustment_[1];
  
  double Result_line = New_guess[0]*wl+New_guess[1]*ws+New_guess[2]+
    New_guess[3]*wl*wl+New_guess[4]*ws*ws+New_guess[5]*wl*ws;
  double Result_sample = New_guess[6]*wl+New_guess[7]*ws+New_guess[8]+
    New_guess[9]*wl*wl+New_guess[10]*ws*ws+New_guess[11]*wl*ws;

  Results = GeoCal::GeoCalCore::ImageCoor(New_line,New_sample);
  
  float resl = fabs(ocentr[0]);
  float ress = fabs(ocentr[1]);
  *res = sqrt((double)(resl*resl+ress*ress));
  if ((*res > thr_resp) && solved) {
    //printf(" (%7.1f,%7.1f)  %7.3f*\n",ocentr[0],ocentr[1],vmax);
    return false;
  }

  //printf(" (%7.1f,%7.1f)  %7.3f\n",ocentr[0],ocentr[1],vmax);
  return true;
}

/*=====================================================================

  rfit

  uses fft to compute correlation function on two images stored
  in the specific global arrays chip1 and asrch.  Chip1 is a fixed
  fftsize x fftsize array and asrch is a larger array for correlation
  matching.  The correlation area is a srchdim x srchdim subset of the
  larger array.

  the method is fftinverse(fft(a) * conj(fft(b))

  arguments:

  1. ilin: input, int ilin;
  line offset into asrch (the larger array)
  2. jsmp: input, int jsmp;
  sample offset into asrch (the larger array)
  3. vmax: output, float *vmax;
  the max correlation value
  4. vloff: output, float *vloff;
  the line offset of the peak match relative to the
  center of chip1 (16.0,16.0)
  5. vsoff: output, float *vsoff;
  the sample offset of the peak match relative to the
  center of chip1 (16.0,16.0)
  6. corr: output, float corr[3][3]
  the 3 x 3 part of the correlation centered at the
  peak, for use in subpixel refinement
  7. srchdim: input, int srchdim;
  The current size of the search area in chip 2
  8. fftsize: input, int fftsiz
  the dimension of the fft. usually 32, 64, 128 or 256

*/

void MspiPcorrMatcher::rfit(int ilin, int jsmp, float* vmax, float* vloff,
			    float* vsoff, float corr[3][3],
			    int srchdim, int fftsize)
{
  int quadmark = srchdim/2;
  int mincor = fftsize/6-1;
  int maxcor = srchdim-mincor;   
  float avg = 0.0;
  int fftLength = search_window_*search_window_;
  fftw_complex img1_FFTin[fftLength], img1_FFTout[fftLength];
  fftw_complex img2_FFTin[fftLength], img2_FFTout[fftLength];

  for (int i = 0; i < fftsize; i++) {

    for (int j = 0; j < fftsize; j++)
      avg += chip1_[j*fftsize+i];
  }
  avg /= (float) (fftsize*fftsize);

  for (int i = 0; i < srchdim; i++) {
    for (int j = 0; j < srchdim; j++) {
      img2_FFTin[j*srchdim+i][0] = asrch_[(ilin+j)*search_window_+(jsmp+i)];
      img2_FFTin[j*srchdim+i][1] = 0.0;
      img1_FFTin[j*srchdim+i][0] = avg;
      img1_FFTin[j*srchdim+i][1] = 0.0;
    }
  }

  int koff = (srchdim-fftsize)/2;
  for (int i = 0; i < fftsize; i++) {

    for (int j = 0; j < fftsize; j++)
      img1_FFTin[(j+koff)*srchdim+i+koff][0] = chip1_[j*fftsize+i];
  }
   
  fftw_plan p = fftw_plan_dft_2d(srchdim,srchdim,img1_FFTin,
				 img1_FFTout,FFTW_FORWARD,FFTW_ESTIMATE);

  fftw_execute(p);
  fftw_destroy_plan(p);
   
  p = fftw_plan_dft_2d(srchdim,srchdim,img2_FFTin,img2_FFTout,
		       FFTW_FORWARD,FFTW_ESTIMATE);

  fftw_execute(p);
  fftw_destroy_plan(p);
      
  img1_FFTout[0][0] = 0.;
  img1_FFTout[0][1] = 0.;
  if (!no_hpf_) {

    for (int i = 1; i < srchdim; i++) {
      img1_FFTout[i][0] = 0.;
      img1_FFTout[i][1] = 0.;
      img1_FFTout[i*srchdim][0] = 0.;
      img1_FFTout[i*srchdim][1] = 0.;
    }
  }

  for (int i = 0; i < srchdim; i++) {
    for (int j = 0; j < srchdim; j++) {
      float bij = img1_FFTout[j*srchdim+i][0]*img2_FFTout[j*srchdim+i][0]+
	img1_FFTout[j*srchdim+i][1]*img2_FFTout[j*srchdim+i][1];
      float bij1 = img1_FFTout[j*srchdim+i][0]*img2_FFTout[j*srchdim+i][1]-
	img1_FFTout[j*srchdim+i][1]*img2_FFTout[j*srchdim+i][0];
      float v = sqrt((double)(bij*bij+bij1*bij1));

      if (v < 1.e-6) 
	v = 1.e-6;

      img2_FFTin[j*srchdim+i][0] = bij/v;
      img2_FFTin[j*srchdim+i][1] = bij1/v;
    }
  }

  p = fftw_plan_dft_2d(srchdim,srchdim,img2_FFTin,img2_FFTout,
		       FFTW_BACKWARD,FFTW_ESTIMATE);
  fftw_execute(p);
  fftw_destroy_plan(p);

  //------------------------------------------------------------------------
  // quadrant swap
  //------------------------------------------------------------------------

  for (int i = 0; i < quadmark; i++) {
    for (int j = 0; j < quadmark; j++) {
      float t = img2_FFTout[i*srchdim+j][0];

      img2_FFTout[i*srchdim+j][0] = 
	img2_FFTout[(i+quadmark)*srchdim+j+quadmark][0];

      img2_FFTout[(i+quadmark)*srchdim+j+quadmark][0] = t;
      t = img2_FFTout[(i+quadmark)*srchdim+j][0];

      img2_FFTout[(i+quadmark)*srchdim+j][0] = 
	img2_FFTout[i*srchdim+j+quadmark][0];

      img2_FFTout[i*srchdim+j+quadmark][0] = t;
    }
  }

  float tvmax = -1.e20;
  int ixmax = mincor; 
  int jxmax = mincor;

  for (int i = mincor; i < maxcor; i++) {
    for (int j = mincor; j < maxcor; j++) {

      float rv = img2_FFTout[j*srchdim+i][0];
      if (rv > tvmax) { 
	tvmax = rv; 
	ixmax = i; 
	jxmax = j; 
      }
    }
  }

  if (tvmax < 0.0) 
    tvmax = 0.0;
   
  //------------------------------------------------------------------------
  //  normalized for varying footprints by three lines below
  //------------------------------------------------------------------------

  float ttemp = log10((double)srchdim)/log10((double)2.0);
  ttemp = ttemp*ttemp;
  *vmax = tvmax*10.0/(srchdim*ttemp*ttemp);
  *vloff = (float)jxmax-quadmark;
  *vsoff = (float)ixmax-quadmark;

  for (int i = 0; i < 3; i++) {

    for (int j = 0; j < 3; j++)
      corr[j][i] = img2_FFTout[(jxmax+j-1)*srchdim+ixmax+i-1][0];
  }

  return;
}

//***************************************************************************
// void MspiPcorrMatcher::updateMatch
// (std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, //coors in ref.
// std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors, //coors in target
// double New_guess[12], //transformation matrix from reference to target
// double rctl,
// double rcts, 
// int ibigx, //Ref_loc being tested
// int scount,
// int* throwcount,
// bool* solved) 
//***************************************************************************
void MspiPcorrMatcher::updateMatch
(std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
 std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors,
 double New_guess[12], double rctl, double rcts, 
 int ibigx, int scount, int* throwcount, bool* solved) 
{
  if ((ibigx > 0) && (*throwcount > 0)) {
    *solved = true;
    printf("***auto fit:nEquations = %8d ***\n",fstImgCoors.size());
  }

  if (ibigx > 0) {
    for (int i = 0; i < 4; i++) {
      if (*throwcount <= 0) 
	break;

      throwout(throwcount,nEquations_max_,fstImgCoors,sndImgCoors);
    }
  }

  if (scount%10 == 9) 
    throwout(throwcount,nEquations_max_,fstImgCoors,sndImgCoors);

  calcLeastSquaresFit(New_guess,predfunc_,nEquations_max_,
		      fstImgCoors,sndImgCoors);
}


