/*===========================================================================
=                                                                           =
=                             picmtch4                                      =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME:

!ABSTRACT:

   Support functions used by MspiPcorrMatcher class

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#include "picmtch4.h"
#include <math.h>
#include <ctype.h>
#include "fftw3.h"

extern "C" {
#include "carto/cartoLsqUtils.h"
}

#define EPS 1.e-7

//***************************************************************************
// void calcLeastSquaresFit
// (double soln[12], // output, line fit
// bool predfunc, // linear(false) or quad(true)
// int nEquationsMax, // max number of equations
// std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, // Ref img pts
// std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors) // Target img pts
//*************************************************************************** 
void calcLeastSquaresFit
(double soln[12], bool predfunc,int nEquationsMax, 
 std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
 std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors)
{
  double aMatrix[nEquationsMax*6], bVector[nEquationsMax];
  int ierror;
  int nCoor = fstImgCoors.size();
  for (int ix = 0; ix < 2; ix++) {
    for (int i = 0; i < nCoor; i++) {
      GeoCal::GeoCalCore::ImageCoor Ref_loc = fstImgCoors[i];
      GeoCal::GeoCalCore::ImageCoor New_loc = sndImgCoors[i];
      
      double RefLine = Ref_loc.line();
      double RefSample = Ref_loc.sample();
      aMatrix[i] = RefLine;
      aMatrix[i+nCoor] = RefSample;
      aMatrix[i+2*nCoor] = 1.;
      aMatrix[i+3*nCoor] = RefLine*RefLine;
      aMatrix[i+4*nCoor] = RefSample*RefSample;
      aMatrix[i+5*nCoor] = RefLine*RefSample;
    
      if (ix == 0)
	bVector[i] = New_loc.line();
      else
	bVector[i] = New_loc.sample();
    }

    if (predfunc && (nCoor > 11))
      lsqfit(aMatrix,bVector,nCoor,6,&soln[ix*6],EPS,&ierror);
    else {
      lsqfit(aMatrix,bVector,nCoor,3,&soln[ix*6],EPS,&ierror);

      for (int j = 3; j < 6; j++) 
	soln[ix*6+j] = 0.0;
    }

  } 
}

//***************************************************************************
// void initialMatch(
// double New_guess[12],  // output, transformation from Ref to New image
// GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3], //Tiepoints from Ref
// GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3], //Tiepoints from New
// int nEquationsMax, // max number of equations
// bool predfunc = false, // liner(false) or quadratic(true)
// GeoCal::GeoCalCore::RasterImage* New = NULL, // target image
// double angoff = 0) //angle to rotate by
//***************************************************************************
void initialMatch(double New_guess[12],  
		  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3],
		  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3],
		  int nEquationsMax, bool predfunc,
		  GeoCal::GeoCalCore::RasterImage* New, double angoff)
{

  angoff /= 57.29577;
  if ((fabs(angoff) > 0.000001) && New != NULL) {
    tiltCoor(New,NewImageTiePoints,angoff);
  }
 
  std::vector<GeoCal::GeoCalCore::ImageCoor> fstImgCoors;
  std::vector<GeoCal::GeoCalCore::ImageCoor> sndImgCoors;

  for (int i = 0; i < 3; i++) {
    fstImgCoors.push_back(RefImageTiePoints[i]);
    sndImgCoors.push_back(NewImageTiePoints[i]);
  }

  calcLeastSquaresFit(New_guess,predfunc,nEquationsMax,fstImgCoors,sndImgCoors);
}

/*=====================================================================

  refine

  refines the match produced by routine rfit

  uses the 3 x 3 center of the fft correlation matrix and fits
  a general quadratic (least squares), then sets the partial
  derivatives in x and y to zero to find the max.  The max
  must be within 0.7 pixels of the center to qualify (otherwise
  the center itself is returned).

  arguments:

  1. corr: input, float corr[3][3];
  center of the correlation matrix as
  returned by rfit
  2. vloff: input,output, float *vloff;
  the match produced by routine rfit at
  the exact center of the 3 x 3, output
  is the refined value
  3. vsoff: input,output, float *vsoff;
  the match produced by routine rfit at
  the exact center of the 3 x 3, output
  is the refined value
  4. ireferr: input,output, int *ireferr;
  error return, 0 if OK, 1 if error
*/

void refine(float corr[3][3], float* vloff,float* vsoff, bool* ireferr)
{
  int ierror;
  double aMatrix[54],bVector[9],soln[6],imx,jmx;
   
  *ireferr = false;
 
  for (int i = 0; i < 3; i++) {
    float ivar = (float) i - 1.0;
    for (int j = 0; j < 3; j++) {
      float jvar = (float) j - 1.0;
      int iq = i*3+j;
      aMatrix[iq] = jvar*jvar;
      aMatrix[iq+9] = jvar*ivar;
      aMatrix[iq+18] = ivar*ivar;
      aMatrix[iq+27] = jvar;
      aMatrix[iq+36] = ivar;
      aMatrix[iq+45] = 1.;
      bVector[iq] = corr[i][j];
    }
  }

  lsqfit(aMatrix,bVector,9,6,soln,EPS,&ierror);

  if (ierror != 0 || soln[0] == 0 || (4.0*soln[2]*soln[0] == soln[1]*soln[1]) ) { 
    printf("sing rfit"); 
    *ireferr = true; 
    return; 
  }

  imx = (soln[1]*soln[3]-2.0*soln[4]*soln[0])/(4.0*soln[2]*soln[0]-soln[1]*soln[1]);
  jmx = -(soln[1]*imx+soln[3])/(2.0*soln[0]);

  if (imx*imx+jmx*jmx>=2.0) { 
    *ireferr = true; 
    return; 
  }

  *vloff = *vloff+imx;
  *vsoff = *vsoff+jmx;
  return;
}

/*=====================================================================

  throwout

  throws out the worst point from the ImageCoor vectors using a linear fit.


  arguments:

  1. throwcount: input, int *throwcount;
  decremented by one for one throwout
  2. nEquationsMax: input, int nEquationsMax;
  the 2d dimension of the a and b arrays,
  the first dimension is fixed at 2
  3. fstImgCoors: input/output, 
  std::vector<GeoCal::GeoCalCore::ImageCoor> fstImgCoors;
  coefficients for the matched point
  4. sndImgCoors: input/output,
  std::vector<GeoCal::GeoCalCore::ImageCoor> sndImgCoors;
  coefficients for the matched point 
*/

void throwout(int* throwcount, int nEquationsMax, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors)
{
  double resid[nEquationsMax],soln[12]; 
   
  /* solve lsq (linear), keeping quadratic form for consistency with main */
  calcLeastSquaresFit(soln,0,nEquationsMax,fstImgCoors,sndImgCoors);

  /* calculate residuals */
  for (int i = 0; i < fstImgCoors.size(); i++) {
    float rx = sndImgCoors[i].line()-(soln[0]*fstImgCoors[i].line()+soln[1]*fstImgCoors[i].sample()+soln[2]);
    float ry = sndImgCoors[i].sample()-(soln[6]*fstImgCoors[i].line()+soln[7]*fstImgCoors[i].sample()+soln[8]);
    resid[i] = rx*rx+ry*ry;
  }
   
  /* discard the worst point */
   
  float rmax = -1.0;
  int imax = 0;
 
  for (int i = 0; i < fstImgCoors.size(); i++) {
 
    if (resid[i] <= rmax) 
      continue;

    rmax = resid[i];
    imax = i;
  }
 
  fstImgCoors.erase(fstImgCoors.begin()+imax);
  sndImgCoors.erase(sndImgCoors.begin()+imax);
 
  /* clean up and return */

  (*throwcount)--;
  return;
}

//***************************************************************************
// void tiltCoor(
// GeoCal::GeoCalCore::RasterImage* Img, // image coordinates are in
// GeoCal::GeoCalCore::ImageCoor TiePoints[3], // coordinates to rotate
// double angoff) // angle to rotate
//***************************************************************************
void tiltCoor(GeoCal::GeoCalCore::RasterImage* Img, 
	      GeoCal::GeoCalCore::ImageCoor TiePoints[3], double angoff)
{
  double rctl = 0.5*Img->number_line();
  double rcts = 0.5*Img->number_sample();
  for (int i = 0; i < 3; i++) {
    double rv1 = TiePoints[i].line()-rctl;
    double rw1 = TiePoints[i].sample()-rcts;
    double rr = sqrt(rv1*rv1+rw1*rw1+0.0000001);
    double theta = atan2(rw1,rv1)-angoff;
    double rv2 = rr*cos(theta);
    double rw2 = rr*sin(theta);
    TiePoints[i] = GeoCal::GeoCalCore::ImageCoor(rv2+rctl,rw2+rcts);
  }
}


