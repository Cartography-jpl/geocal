#include "oldpicmtch4.h"
#include <math.h>
#include <ctype.h>
#include "fftw3.h"


extern "C" {
#include "carto/cartoGtUtils.h"
}

void convertPoints(int geocord1, double tiePoints[6], float** lineSamp, int dim, int labnl, int labns, 
		   float** c_data /* = NULL */, int RefImageLocCount /* = 0 */)
{
  const char labelstr1[] = {"MODELTIEPOINTTAG=(0,0,0,.3,.3,0.0)\nMODELPIXELSCALETAG=(0.0006666666667,-0.0006666666667,0.0)\nPROJECTIONGEOKEY=20(CT_MILLERCYLINDRICAL)\nGTRASTERTYPEGEOKEY=2(RASTERPIXELISPOINT)\nGEOGELLIPSOIDGEOKEY=7030(ELLIPSE_WGS84)"};
  const char labelstr2[] = {"MODELTIEPOINTTAG=(-5.0,-4.0,0,.3,.3,0.0)\nMODELPIXELSCALETAG=(0.0006666666667,-0.0006666666667,0.0)\nPROJECTIONGEOKEY=20(CT_MILLERCYLINDRICAL)\nGTRASTERTYPEGEOKEY=2(RASTERPIXELISPOINT)\nGEOGELLIPSOIDGEOKEY=7030(ELLIPSE_WGS84)"};
  double t[6], tinv[6];

  if (!geocord1) {

    for (int i = 0; i < 6; i++) { 
      t[i] = 0.; 
      tinv[i] = 0.; 
    }
    t[0] = 1.; 
    t[4] = 1.;
    tinv[0] = 1.; 
    tinv[4] = 1.;
  }
  else {
    if (dim == 1)
      getGeoTIFFLabels(t,tinv,labelstr1,labnl,labns,dim);
    else
      getGeoTIFFLabels(t,tinv,labelstr2,labnl,labns,dim);
  }

  if (c_data != NULL) {
    for (int i = 0; i < RefImageLocCount; i++) {
      double dline = tinv[0]*(double)(c_data[0][i])+ tinv[1]*(double)(c_data[1][i])+tinv[2];
      double dsamp = tinv[3]*(double)(c_data[0][i])+ tinv[4]*(double)(c_data[1][i])+tinv[5];
      c_data[2][i] = (float)dline;
      c_data[3][i] = (float)dsamp;
    }
  }

  for (int i = 0; i < 6; i += 2) {
    double dline = tinv[0]*(double)(tiePoints[i])+tinv[1]*(double)(tiePoints[i+1])+tinv[2];
    double dsamp = tinv[3]*(double)(tiePoints[i])+tinv[4]*(double)(tiePoints[i+1])+tinv[5];
    lineSamp[0][i/2] = (float)dline;
    lineSamp[1][i/2] = (float)dsamp;
  }
}

void convertTiePoints(float** fstImgCoeff, float** sndImgCoeff, float** c_data, int geocord1, 
		      double itie[6], double otie[6], int geocord2, int RefImageLocCount, int labnl1, int labns1, int labnl2, int labns2)
{
  convertPoints(geocord1, itie, fstImgCoeff, 1, labnl1, labns1, c_data, RefImageLocCount);
  convertPoints(geocord2, otie, sndImgCoeff, 2, labnl2, labns2);
}

void convertGeoTIFFTiePoints(float** fstImgCoeff, float** sndImgCoeff, float** c_data, int geocord1, double itie[6], 
		   double otie[6], int geocord2, int RefImageLocCount, int labnl1, int labns1, int labnl2, int labns2, bool rotated /* = false */) 
{

  double u[6], uinv[6], dline, dsamp, tinv[6];
  //int status;
  const char labelstr1[] = {"MODELTIEPOINTTAG=(0,0,0,.3,.3,0.0)\nMODELPIXELSCALETAG=(0.0006666666667,-0.0006666666667,0.0)\nPROJECTIONGEOKEY=20(CT_MILLERCYLINDRICAL)\nGTRASTERTYPEGEOKEY=2(RASTERPIXELISPOINT)\nGEOGELLIPSOIDGEOKEY=7030(ELLIPSE_WGS84)"};
  const char labelstr2[] = {"MODELTIEPOINTTAG=(-5.0,-4.0,0,.3,.3,0.0)\nMODELPIXELSCALETAG=(0.0006666666667,-0.0006666666667,0.0)\nPROJECTIONGEOKEY=20(CT_MILLERCYLINDRICAL)\nGTRASTERTYPEGEOKEY=2(RASTERPIXELISPOINT)\nGEOGELLIPSOIDGEOKEY=7030(ELLIPSE_WGS84)"};
  const char labelstr3[] = {"MODELTIEPOINTTAG=(298.0,308.0,0,.3,.3,0.0)\nMODELTRANSFORMATIONTAG=(0.0,-0.0006666666667,0,0.5053333333436,-0.0006666666667,-0.0,0,0.4986666666766,0,0,0,0,0,0,0,1)\nPROJECTIONGEOKEY=20(CT_MILLERCYLINDRICAL)\nGTRASTERTYPEGEOKEY=2(RASTERPIXELISPOINT)\nGEOGELLIPSOIDGEOKEY=7030(ELLIPSE_WGS84)"};
  double t[6];

  if (!geocord1) {

    for (int i = 0; i < 6; i++) { 
      t[i] = 0.; 
      tinv[i] = 0.; 
    }
    t[0] = 1.; 
    t[4] = 1.;
    tinv[0] = 1.; 
    tinv[4] = 1.;
  }
  else {
    getGeoTIFFLabels(t,tinv,labelstr1,labnl1,labns1,1);
  }
  /*
    char* labelstr;
    int labnl, labns,status;
    if (!geocord1) 
    status = gtgetlab((char*)"inp",1,&labelstr,&labnl,&labns);
    if (!geocord1 && (status != 1))
    zmabend((char*)"Failed to get GeoTIFF label, first input");
    printf("%s %i %i\n",labelstr,labnl,labns);*/
  itie[0] = 1.0;
  itie[1] = 1.0;
  itie[2] = (double)labnl1;
  itie[3] = 1.0;
  itie[4] = 1.0;
  itie[5] = (double)labns1;

  for (int i = 0; i < RefImageLocCount; i++) {
    dline = tinv[0]*(double)(c_data[0][i])+
      tinv[1]*(double)(c_data[1][i])+tinv[2];
    dsamp = tinv[3]*(double)(c_data[0][i])+
      tinv[4]*(double)(c_data[1][i])+tinv[5];

    c_data[2][i] = (float)dline;
    c_data[3][i] = (float)dsamp;
  }

  for (int i = 0; i < 6; i += 2) {
    fstImgCoeff[0][i/2] = (float)itie[i];
    fstImgCoeff[1][i/2] = (float)itie[i+1];
  }

  if (rotated)
    getGeoTIFFLabels(u,uinv,labelstr3,labnl2,labns2,2);
  else
    getGeoTIFFLabels(u,uinv,labelstr2,labnl2,labns2,2);
  
  if (!geocord1) {
    getGeoTIFFLabels(t,tinv,labelstr1,labnl1,labns1,1);
  }

  for (int i = 0; i < 6; i += 2) {
    dline = t[0]*(double)(itie[i])+t[1]*(double)(itie[i+1])+t[2];
    dsamp = t[3]*(double)(itie[i])+t[4]*(double)(itie[i+1])+t[5];
    otie[i] = uinv[0]*dline+uinv[1]*dsamp+uinv[2];
    otie[i+1] = uinv[3]*dline+uinv[4]*dsamp+uinv[5];
    sndImgCoeff[0][i/2] = (float)otie[i];
    sndImgCoeff[1][i/2] = (float)otie[i+1];
  }
}

void getGeoTIFFLabels(double t[6], double tinv[6], const char* labelstr, int labnl, 
		      int labns, int gtgetlabval)
{
  int status;
  double corner[4];
  status = geofix((char*)labelstr,t,tinv,labnl,labns,corner);
  if (status!=1) {
    fprintf(stderr,"Failed to get mapping from GeoTIFF label");
    exit(1);
  }
  //zmabend((char*)"Failed to get mapping from GeoTIFF label");
}

/*=====================================================================

  getzvl

  gets a z value from a matrix by averaging over a window
  and uses bilinear interpolation for fractional pixel
  location

  arguments:

  1. imgCoeff: input, float *a;
  two dimensional square array stored as
  one dimensional (as in fortran)
  2. n: input, int n;
  array a is n x n
  3. coord: input, float coord[2];
  the pixel location to obtain z value
  4. nw: input, int nw;
  the window size to average over (must be odd)
  5. nr: input, int nr;
  the allowable number of zero values in the
  window; if exceeded -9999.0 is returned
*/

float getzvl(float* imgCoeff, int n, float coord[2],
	     int nw, int nr)
{
  int iline = (int) (coord[0]-.5);
  int isamp = (int) (coord[1]-.5);
  int ill = iline-nw/2;
  int jsl = isamp-nw/2;
  float flu = coord[0]-(float)iline-.5;
  float fsu = coord[1]-(float)isamp-.5;
  float fll = 1.0-flu;
  float fsl = 1.0-fsu;
  int ilu = ill+nw;
  int jsu = jsl+nw;

  if ((ill < 0) || (ilu > n)) 
    return (0.);
  if ((jsl < 0) || (jsu > n)) 
    return (0.);
 
  int ire = 0;
  float sum = 0.;
 
  for (int i = ill; i < ilu; i++) {
    for (int j = jsl; j < jsu; j++) {
      float val = imgCoeff[i*n+j];
      if (val < 0.5)
	ire++;
      if ( i== ill) 
	val *= fll;
      if (i == (ilu-1)) 
	val *= flu;
      if (j == jsl) 
	val *= fsl;
      if ( j== (jsu-1)) 
	val *= fsu;
      sum += val;
    }
  }

  if (ire <= nr)
    return (sum/(float)((nw-1)*(nw-1)));
  else 
    return(-9999.0);
}
