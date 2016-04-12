// This is picmtch4 code. We will slowly convert this to C++/geocal,
// but right now this is pretty much just cleaned up for C++
#include <cmath>
#include <zvproto.h>
#include <ibisfile.h>
#include <fftw3.h>
#include <algorithm>

#include "vicar_argument.h"
#include "ibis_file.h"
#include "constant.h"
#include "phase_correlation_matcher.h"
#include "vicar_image_coordinate.h"

extern "C" {
#include "carto/cartoMemUtils.h"
#include "carto/cartoGtUtils.h"
#include "carto/cartoSortUtils.h"
}

using namespace GeoCal;

/************************************************************************/
/* program picmtch5                                                     */
/*  image to image correlation, including gcp option  A. Zobrist 9/9/00 */
/************************************************************************/
/*  00-09 ...alz... initial version                                     */
/************************************************************************/

int lnlg[2],lnsg[2],i_unit[2];
std::vector<double> chip1, asrch;

double rvecl[5] = {0.,1.,-1.,1.,-1.};
double rvecs[5] = {0.,1.,-1.,-1.,1.};
double ident[12] = {1.,0.,0.,0.,0.,0.,0.,1.,0.,0.,0.,0.};

// Temporary, this will go away in a bit.
boost::shared_ptr<PhaseCorrelationMatcher> matcher;

/*=====================================================================

getzvl

gets a z value from a matrix by averaging over a window
and uses bilinear interpolation for fractional pixel
location

arguments:

     1. a: input, double *a;
	two dimensional square array stored as
	one dimensional (as in fortran)
     2. n: input, int n;
	array a is n x n
     3. coord: input, double coord[2];
	the pixel location to obtain z value
     4. nw: input, int nw;
	the window size to average over (must be odd)
     5. nr: input, int nr;
	the allowable number of zero values in the
	window; if exceeded -9999.0 is returned
*/

double getzvl(const std::vector<double>& a,int n, double coord[2],int nw,int nr)
{
   int iline = (int)(coord[0]-.5);
   int isamp = (int)(coord[1]-.5);
   int ill = iline-nw/2;
   int jsl = isamp-nw/2;
   double flu = coord[0]-iline-.5;
   double fsu = coord[1]-isamp-.5;
   double fll = 1.0-flu;
   double fsl = 1.0-fsu;
   int ilu = ill+nw;
   int jsu = jsl+nw;
   if (ill<0||ilu>n) return(0.);
   if (jsl<0||jsu>n) return(0.);

   int ire = 0;
   double sum = 0.;
   for(int i=ill;i<ilu;i++)
     for(int j=jsl;j<jsu;j++) {
       double val = a[i*n+j];
       if (val<0.5) ire++;
       if (i==ill) val *= fll;
       if (i==(ilu-1)) val *= flu;
       if (j==jsl) val *= fsl;
       if (j==(jsu-1)) val *= fsu;
       sum += val;
     }
   if (ire<=nr) 
     return(sum/((nw-1)*(nw-1)));
   else 
     return(-9999.0);
}

/*=====================================================================

getgrid

extracts a subimage from an image.  the subimage is specified by
a linear transformation and the point that (when transformed) becomes
the center of the subimage.  ndim can be used to obtain only the
central part of the array if the outer border is not needed, for
example when picmtch5 has narrowed a search.


arguments:

     1. filenum: input, int filenum;
	the filenumber returned by mc_open
     2. chip: output, double *chip;
	holds the ndim x ndim subimage as a
	one dimensional (fortran-like) array
	always floating format
     3. narray: input, int narray;
	the dimension of the chip array
     4. ndim: input, int ndim;
	how much data to put into the chip
	center; ndim <= narray, both are even
     5. trans: input, double trans[12];
	the quadratic transformation that shapes
	the grid.
     6. centrx: input, double centrx;
	trans(centrx,centry) will be the center
	of the sampled grid
     7. centry: input, double centry;
	trans(centrx,centry) will be the center
	of the sampled grid
     8. rmag: input, int rmag;
	applied to grid to obtain a larger (or
	smaller) area chip
     9. chop: output, int *chop;
	number of subimage points outside of
	input image plus zero points
*/

void getgrid(int filenum,std::vector<double>& chip,int narray,
	     int ndim,double trans[12],
	     double centrx,double centry,double rmag[2],int* chop,
	     double pcl,double pcs,int zerothr)
{
   /* the line,samp calcs are based on a .5 centered pixel, however,
   the main program also passes a .5 centered request.  These cancel
   and the pixels are centered like VICAR pixels.  Since both arrays
   are offset the same amount, there is no error introduced - alz */
   
   /* get the file dimensions */

   if (narray%2==1||ndim%2==1) throw Exception("getgrid error");
   int lnl = lnlg[filenum-1];
   int lns = lnsg[filenum-1];
   
   /* generate the grid */

   int ndim2 = ndim*ndim;
   std::vector<double> gridx(ndim2);
   std::vector<double> gridy(ndim2);
   
   double xmin = 99999999.0; double xmax = -99999999.0;
   double ymin = 99999999.0; double ymax = -99999999.0;
   for(int i=0;i<ndim;i++)
     for(int j=0;j<ndim;j++) {
       double px = centrx+rmag[0]*((i-ndim/2)+1.0)+pcl;
       double py = centry+rmag[1]*((j-ndim/2)+1.0)+pcs;
       gridx[i*ndim+j] = trans[0]*px+trans[1]*py+trans[2]+trans[3]*px*px+
	 trans[4]*py*py+trans[5]*px*py;
       gridy[i*ndim+j] = trans[6]*px+trans[7]*py+trans[8]+trans[9]*px*px+
	 trans[10]*py*py+trans[11]*px*py;
       xmin = std::min(xmin,gridx[i*ndim+j]);
       ymin = std::min(ymin,gridy[i*ndim+j]);
       xmax = std::max(xmax,gridx[i*ndim+j]);
       ymax = std::max(ymax,gridy[i*ndim+j]);
     }
   
   /* malloc an input rectangle as bounding box, don't have to go outside image */

   int slbx = std::max(0,std::min(lnl-2,(int)(xmin-1.0)));
   int ssbx = std::max(0,std::min(lns-2,(int)(ymin-1.0)));
   int elbx = std::max(1,std::min(lnl-1,(int)(xmax+1.0)));
   int esbx = std::max(1,std::min(lns-1,(int)(ymax+1.0)));
   int nlbx = elbx-slbx+1;
   int nsbx = esbx-ssbx+1;
   short int **imbuf;
   mz_alloc2((unsigned char ***)&imbuf,nlbx,nsbx,sizeof(short int));
   
   /* read the image into the bounding box, outside immaterial */
   
   for(int i=0;i<nlbx;i++)
     zvread(i_unit[filenum-1],imbuf[i],
	    "LINE",slbx+i+1,"SAMP",ssbx+1,"NSAMPS",nsbx,NULL);
   
   /* now look up the image values, outside box is intercepted */

   *chop = 0;
   double fzerothr = zerothr+0.01;
   for(int i=0;i<ndim2;i++) {
     double rline = gridx[i]-0.5;
     double rsamp = gridy[i]-0.5;
     int iline = (int) (rline-0.5);
     int isamp = (int) (rsamp-0.5);
     if (iline<0 || iline>=lnl-1 || isamp<0 || isamp>=lns-1) { 
       chip[i] = 0.; 
       (*chop)++; 
     } else {
       double dvecl0 = imbuf[iline-slbx][isamp-ssbx];
       double dvecl1 = imbuf[iline-slbx][isamp-ssbx+1];
       double dvecu0 = imbuf[iline-slbx+1][isamp-ssbx];
       double dvecu1 = imbuf[iline-slbx+1][isamp-ssbx+1];
       double rlfac = rline-iline-0.5;
       double rsfac = rsamp-isamp-0.5;
       double vl = (1.0-rsfac)*dvecl0+rsfac*dvecl1;
       double vu = (1.0-rsfac)*dvecu0+rsfac*dvecu1;
       chip[i] =((1.0-rlfac)*vl+rlfac*vu);
       if (chip[i]<fzerothr) 
	 (*chop)++;
     }
   }
   
   /* free storage */

   mz_free2((unsigned char **)imbuf,nlbx);
   
   /* now center in the grid, save the prints below for getgrid verif */

   if (narray==ndim) return;
   int bord = (narray-ndim)/2;
   for(int i=ndim-1;i>=0;i--)
     for(int j=0;j<ndim;j++)
       chip[(i+bord)*narray+bord+j] = chip[i*ndim+j];
}


/*=====================================================================

throwout

throws out the worst point from the a,b arrays using a linear fit.


arguments:

     1. throwcount: input, int *throwcount;
	decremented by one for one throwout
     2. a: input/output, double **a;
	coefficients for the matched point
     3. b: input/output, double **b;
	coefficients for the matched point
     4. neq: input/output, int *neq;
	decremented by one for one throwout
     5. neqmax: input, int neqmax;
	the 2d dimension of the a and b arrays,
	the first dimension is fixed at 2
     
*/

void throwout(int* throwcount,double** a,double** b,int* neq,int neqmax)
{
  int ierror,imax=0;
  double rx,ry,rmax;
  double soln[12];

  const double eps = 1e-7;
  std::vector<double> aa(neqmax*6);
  std::vector<double> bb(neqmax);
  std::vector<double> resid(neqmax);
   
  /* solve lsq (linear), keeping quadratic form for consistency with main */
   
  for(int ix=0;ix<2;ix++) {
    for(int i=0;i<*neq;i++) {
      aa[i] = a[0][i];
      aa[i+(*neq)] = a[1][i];
      aa[i+2*(*neq)] = 1.;
      aa[i+3*(*neq)] = a[0][i]*a[0][i];
      aa[i+4*(*neq)] = a[1][i]*a[1][i];
      aa[i+5*(*neq)] = a[0][i]*a[1][i];
      bb[i] = b[ix][i];
    }
    matcher->lsqfit(&aa[0],&bb[0],(*neq),3,&soln[ix*6],eps,&ierror);
    for(int j=3;j<6;j++) soln[ix*6+j] = 0.0;
  }
   
  /* calculate residuals */
   
  for(int i=0;i<*neq;i++) {
    rx = b[0][i]-(soln[0]*a[0][i]+soln[1]*a[1][i]+soln[2]);
    ry = b[1][i]-(soln[6]*a[0][i]+soln[7]*a[1][i]+soln[8]);
    resid[i] = rx*rx+ry*ry;
  }
   
  /* discard the worst point */
   
  rmax = -1.0;
  for(int i=0;i<*neq;i++) {
    if (resid[i]<=rmax) continue;
    rmax = resid[i];
    imax = i;
  }
   
  for(int i=imax;i<(*neq)-1;i++) {
    a[0][i] = a[0][i+1];
    a[1][i] = a[1][i+1];
    b[0][i] = b[0][i+1];
    b[1][i] = b[1][i+1];
  }
   
  (*throwcount)--;
  (*neq)--;
  return;
}

int main(int Argc, char *Argv[])
{
try {
  int   minsrch;
   
  double **a,**b;

  int ierror;
  int chop,picout,srchdim;
  int retry,refinerr,ndim;
  double vloff, vsoff;
  double resl,ress,res,corr[3][3];
  double ocentr[2];
  double dline,dsamp;
  double soln[12],solninv[12];

  int status,parmcnt,o_unit;
  const double eps = 1e-7;
  
  VicarArgument va(Argc, Argv);

  zifmessage(const_cast<char*>("picmtch5 version Wed Apr 08 2016"));
   
  /* get the basic parameters */
   
  bool gcpf = va.has_keyword("gcpf");
  bool elvcor = va.has_keyword("elvcor");
  std::vector<double> itie  = va.arg<std::vector<double> >("itie");
  std::vector<double> otie = va.arg<std::vector<double> >("otie");
  if (itie.size()!=otie.size()) 
    throw Exception("itie count must equal otie count");
  if (itie.size()!=0&&itie.size()!=6) 
    throw Exception("itie/otie count must be 0 or 6");

  std::vector<double> rmagtae = va.arg<std::vector<double> >("magnify");
  if (rmagtae.size()==1) 
    throw Exception("magnify needs 2 inputs, or default = no inputs"); 
  std::string magshrk = va.arg<std::string>("magshrk");
  std::vector<double> rmagmin = va.arg<std::vector<double> >("magmin");
  double thr_res = va.arg<double>("thr_res");
  double angoff = va.arg<double>("angoff") * Constant::deg_to_rad;
  int search = va.arg<int>("search");
  int msrc = va.arg<int>("minsrch");
  int fftsize = va.arg<int>("fftsize");
  int zerothr = va.arg<int>("zerothr");
  // Make sure numbers are even.
  if (search%2==1) search++;
  if (minsrch%2==1) minsrch++;
  if (fftsize%2==1) fftsize++;
  bool predfunc = va.has_keyword("QUAD");
  int nredo = va.arg<int>("redo");
  if (msrc<fftsize) msrc = fftsize;
  if (search<fftsize) search = fftsize;
  if (search<msrc) search = msrc;

  bool nohpf = va.has_keyword("nohpf");
  bool subpix = !va.has_keyword("nosubpix");
  bool geocord1 = va.has_keyword("geocord1");
  bool geocord2 = va.has_keyword("geocord2");
  
  int getw = va.arg<int>("zwind");
  int getr = va.arg<int>("zreject");
  double zerolim = va.arg<double>("zerolim");
  double zerolim2 = va.arg<double>("zerolim2");
  if (zerolim2<-0.5) zerolim2 = zerolim;
  zerolim *= 0.01;
  zerolim2 *= 0.01;
   
  std::vector<double> retryparm = va.arg<std::vector<double> >("retry");
  int nretry = (int)retryparm[0]+1;
  double rretry = retryparm[1];
  double tretry = retryparm[2];
  int ffthalf = va.arg<int>("ffthalf");
  if (ffthalf>0) {
    if (nretry>1) 
      throw Exception("can't use both RETRY and FFTHALF parameters");
    nretry = ffthalf+1;
  }

  double rmcor = va.arg<double>("fitmin");
  int autofit = va.arg<int>("autofit");
  if (autofit>0&&autofit<12)
    throw Exception("autofit must be at least 12; or zero (autofit off)");
  std::vector<double> vmaxvec;
  std::vector<int> vmaxix;
  if (autofit>0) {
    vmaxvec.resize(autofit);
    vmaxix.resize(autofit);
  }
 
  double rmag[2];
  rmag[0] = rmagtae[0]; rmag[1] = rmagtae[1];
  if (getw%2==0) throw Exception("zwind must be odd");
  retry = (nretry>1);
  if (retry&&gcpf) throw Exception("can't retry gcpf");

  /* open the ibis interface file */

  // Come back to this and replace with IbisFile
  std::vector<std::string> inpfname = va.arg<std::vector<std::string> >("inp");
  IbisFile ifile(inpfname[2], IbisFile::UPDATE);
  if (ifile.number_col()<11) throw Exception("ibis file needs 11 columns");
  // Give columns name, to make it clearer what we are doing here.
  std::vector<double>& first_line_or_east = ifile.column<double>(0).data;
  std::vector<double>& first_samp_or_west = ifile.column<double>(1).data;
  std::vector<double>& first_line = ifile.column<double>(2).data;
  std::vector<double>& first_samp = ifile.column<double>(3).data;
  std::vector<double>& first_z = ifile.column<double>(4).data;
  std::vector<double>& second_line = ifile.column<double>(5).data;
  std::vector<double>& second_samp = ifile.column<double>(6).data;
  std::vector<double>& second_z = ifile.column<double>(7).data;
  std::vector<double>& corr_val = ifile.column<double>(8).data;
  std::vector<double>& line_res = ifile.column<double>(9).data;
  std::vector<double>& samp_res = ifile.column<double>(10).data;

  // Mark file as updated, so we know to write this out at the end
  ifile.mark_updated();

  // Get elevation offset, which is either read from the ibis file are
  // set to zero.
  blitz::Array<double, 2> elvoff(ifile.number_row(), 2);
  if(elvcor) {
    const std::vector<double>& line_elv_offset = ifile.column<double>(11).data;
    const std::vector<double>& samp_elv_offset = ifile.column<double>(12).data;
    for(int i = 0; i < elvoff.rows(); ++i) {
      elvoff(i, 0) = line_elv_offset[i];
      elvoff(i, 1) = samp_elv_offset[i];
    }
  } else
    elvoff = 0.0;
  // Second input line/sample, after doing height correction.
  blitz::Array<double, 2> second_hcorr(ifile.number_row(), 2);
  second_hcorr = 0.0;

  if (gcpf) throw Exception("ground control point file not implemented yet");
  int neqmax = ifile.number_row()+3+nredo;
  std::vector<double> aa(neqmax*6);
  std::vector<double> bb(neqmax);
  // May want to replace with blitz array, or some other structure.
  mz_alloc2((unsigned char ***)&a,2,neqmax,sizeof(double));
  mz_alloc2((unsigned char ***)&b,2,neqmax,sizeof(double));
  chip1.resize(fftsize*fftsize);
  asrch.resize(search*search);
  matcher.reset(new PhaseCorrelationMatcher(fftsize,search));
  // Tempory, this should be done in constructor
  matcher->nohpf = nohpf;
   
   /* is an output requested */
   
   status = zvpcnt((char*)"out",&parmcnt);
   picout = parmcnt>0;
   if (picout) {
     status=zvunit( &o_unit,(char*)"OUT",1,NULL);
     status=zvopen( o_unit,"U_NL",32,"U_NS",ifile.number_row()*32,
		    "OP","WRITE","OPEN_ACT","SA","IO_ACT","SA",NULL);
   }
   
   /* map points to line-samp according to image corners
      and parms, also convert itie */
 
   double t[6], tinv[6];
   if (!geocord1) {
     for(int iii=0;iii<6;iii++) { t[iii] = 0.; tinv[iii] = 0.; }
     t[0] = 1.; t[4] = 1.;
     tinv[0] = 1.; tinv[4] = 1.;
   } else {
     char* labelstr;
     int labnl, labns;
     double corner[4];
     status = gtgetlab((char*)"inp",1,&labelstr,&labnl,&labns);
     if (status!=1) throw Exception("Failed to get GeoTIFF label, first input");
     status = geofix(labelstr,t,tinv,labnl,labns,corner);
     if (status!=1)
       throw Exception("Failed to get mapping from GeoTIFF label, first input");
   }
   bool geotie = (itie.size()==0);
   if (geotie) {
     char* labelstr;
     int labnl, labns;
     if (!geocord1) status = gtgetlab((char*)"inp",1,&labelstr,&labnl,&labns);
     if (!geocord1&&status!=1)
       throw Exception("Failed to get GeoTIFF label, first input");
     itie.resize(6);
     itie[0] = 1.0;
     itie[1] = 1.0;
     itie[2] = labnl;
     itie[3] = 1.0;
     itie[4] = 1.0;
     itie[5] = labns;
   }
   for(int iii=0;iii<ifile.number_row();iii++) {
     dline = tinv[0]*first_line_or_east[iii]+ 
       tinv[1]*first_samp_or_west[iii]+tinv[2];
     dsamp = tinv[3]*first_line_or_east[iii]+
       tinv[4]*first_samp_or_west[iii]+tinv[5];
     first_line[iii] = dline;
     first_samp[iii] = dsamp;
   }
   for(int iii=0;iii<6;iii+=2) {
     if (!geotie) {
       dline = tinv[0]*itie[iii]+tinv[1]*itie[iii+1]+tinv[2];
       dsamp = tinv[3]*itie[iii]+tinv[4]*itie[iii+1]+tinv[5];
     } else {
       dline = itie[iii];
       dsamp = itie[iii+1];
     }
     a[0][iii/2] = dline;
     a[1][iii/2] = dsamp;
   }
   
   /* convert otie */

   double u[6],uinv[6];
   if (otie.size()==6&&!geocord2) {
     for(int iii=0;iii<6;iii++) { 
       u[iii] = 0.; 
       uinv[iii] = 0.; 
     }
     u[0] = 1.; u[4] = 1.; uinv[0] = 1.; uinv[4] = 1.;
   } else {
     char* labelstr;
     int labnl, labns;
     double corner[4];
     status = gtgetlab((char*)"inp",2,&labelstr,&labnl,&labns);
     if (status!=1) throw Exception("Failed to get GeoTIFF label, second input");
     status = geofix(labelstr,u,uinv,labnl,labns,corner);
     if (status!=1)
       throw Exception("Failed to get mapping from GeoTIFF label, second input");
   }
   if (otie.size()==0) {
     if (!geocord1) {
       char* labelstr;
       int labnl, labns;
       double corner[4];
       status = gtgetlab((char*)"inp",1,&labelstr,&labnl,&labns);
       if (status!=1) throw Exception("Failed to get GeoTIFF label, first input");
       status = geofix(labelstr,t,tinv,labnl,labns,corner);
       if (status!=1)
	 throw Exception("Failed to get mapping from GeoTIFF label, first input");
     }
     otie.resize(6);
     for(int iii=0;iii<6;iii+=2) {
       dline = t[0]*itie[iii]+t[1]*itie[iii+1]+t[2];
       dsamp = t[3]*itie[iii]+t[4]*itie[iii+1]+t[5];
       otie[iii] = uinv[0]*dline+uinv[1]*dsamp+uinv[2];
       otie[iii+1] = uinv[3]*dline+uinv[4]*dsamp+uinv[5];
       b[0][iii/2] = otie[iii];
       b[1][iii/2] = otie[iii+1];
     }
   } else 
     for(int iii=0;iii<6;iii+=2) {
       dline = uinv[0]*otie[iii]+uinv[1]*otie[iii+1]+uinv[2];
       dsamp = uinv[3]*otie[iii]+uinv[4]*otie[iii+1]+uinv[5];
       b[0][iii/2] = dline;
       b[1][iii/2] = dsamp;
     }
   
   /* open the first image file */
   
   status = zvunit(&i_unit[0],(char*)"INP",1,NULL);
   status = zvopen(i_unit[0],"OPEN_ACT","SA","IO_ACT","SA",
			"U_FORMAT","HALF",NULL);
   int ityp1;
   zvget(i_unit[0],"NL",&lnlg[0],"NS",&lnsg[0],"PIX_SIZE",&ityp1,NULL);
   
   /* open the second image file */

   status = zvunit(&i_unit[1],(char*)"INP",2,NULL);
   status = zvopen(i_unit[1],"OPEN_ACT","SA","IO_ACT","SA",
			"U_FORMAT","HALF",NULL);
   int ityp2;
   zvget(i_unit[1],"NL",&lnlg[1],"NS",&lnsg[1],"PIX_SIZE",&ityp2,NULL);
   double rctl = 0.5*lnlg[1];
   double rcts = 0.5*lnsg[1];
   
   /* outer loop over the tiepoints or gcp's */
   /* gcp option inactive for now... I need a sample gcp data set */

   int neq = 3; 
   int lastneq = -1; 
   int autoix = 0; 
   int gotthresh = 0; 
   int throwcount = 4; 
   int scount = 0;
   int srchw = search-fftsize; 
   int msrchw = msrc-fftsize; 
   bool solved = false; 
   double vmaxfac = 0.9;
   int maxredo = std::min(nredo,ifile.number_row()); 
   double thr_resp = 4.0*thr_res;
   int neqmin = std::max(autofit/2+4,9);
   printf("  seq             point     srch       convergence     corr\n");
   for(int ibigx=0;ibigx<(ifile.number_row()+maxredo);ibigx++) {
     int ibig = ibigx%ifile.number_row();
     first_z[ibig] = -9999.;
     second_line[ibig] = -9999.;
     second_samp[ibig] = -9999.;
     second_z[ibig] = -9999.;
     corr_val[ibig] = -9999.;
     line_res[ibig] = 0.;
     samp_res[ibig] = -9999.;
      
     int ifftsize = fftsize;  
     int ffthset = 0;
     for(int jbig=0;jbig<nretry;jbig++) {
       if(jbig>0&&ffthalf>0) {
	 if(ibigx<autofit*2) continue;
	 if(ffthset==1) continue;
	 ifftsize /= 2;
       }
       ffthset = 0;
	 
       /*  do linear fit of tiepoints and matches to predict match */
       /*  inverse linear fit needed for getgrid outputs */
       /*  can't mix 3 initial points with updates */
       
       if(((ibigx==0)||(neq>=neqmin))&&(neq!=lastneq)) {
	 if (ibigx>0&&throwcount>0) {
	   solved = true;
	   printf("***auto fit:neq = %8d ***\n",neq);
	 }
	 if (ibigx>0) 
	   for(int iii=0;iii<4;iii++) {
	     if (throwcount<=0) break;
	     throwout(&throwcount,a,b,&neq,neqmax);
	   }
	 if (scount%10==9) throwout(&throwcount,a,b,&neq,neqmax);
	 scount++;
	 lastneq = neq;
	 for(int ix=0;ix<2;ix++) {
	   for(int iii=0;iii<neq;iii++) {
	     aa[iii] = a[0][iii];
	     aa[iii+neq] = a[1][iii];
	     aa[iii+2*neq] = 1.;
	     aa[iii+3*neq] = a[0][iii]*a[0][iii];
	     aa[iii+4*neq] = a[1][iii]*a[1][iii];
	     aa[iii+5*neq] = a[0][iii]*a[1][iii];
	     if (fabs(angoff)>0.000001&&ibigx==0&&ix==0) {
	       double rv1 = b[0][iii]-rctl;
	       double rw1 = b[1][iii]-rcts;
	       double rr = sqrt(rv1*rv1+rw1*rw1+0.0000001);
	       double theta = atan2(rw1,rv1)-angoff;
	       double rv2 = rr*cos(theta);
	       double rw2 = rr*sin(theta);
	       b[0][iii] = rv2+rctl;
	       b[1][iii] = rw2+rcts;
	     }
	     bb[iii] = b[ix][iii];
	   }
	   if (predfunc&&neq>11)
	     matcher->lsqfit(&aa[0],&bb[0],neq,6,&soln[ix*6],eps,&ierror);
	   else {
	     matcher->lsqfit(&aa[0],&bb[0],neq,3,&soln[ix*6],eps,&ierror);
	     for(int jjj=3;jjj<6;jjj++) soln[ix*6+jjj] = 0.0;
	   }
	 }
       }
       if (ibigx==0) neq = 0;
         
       int choplimit1 = (int)(ifftsize*ifftsize*zerolim);
       int choplimit2 = (int)(ifftsize*ifftsize*zerolim2);
       int bigchoplimit2 = search*search/2;   /* see zerolimit in rfit */
       double xlt = first_line[ibig]+rretry*rvecl[jbig];
       double xst = first_samp[ibig]+rretry*rvecs[jbig];
       double pcntr[2], centr[2];
       pcntr[0] = (int)(xlt+0.5);
       pcntr[1] = (int)(xst+0.5);
       VicarImageCoordinate centr(xlt-pcntr[0]+ifftsize/2.0+.5,
				  xst-pcntr[1]+ifftsize/2.0+.5);
       getgrid(1,chip1,ifftsize,ifftsize,ident,pcntr[0],pcntr[1],rmag,&chop,
               0.0,0.0,zerothr);
       if (chop>choplimit1) {
	 if (jbig==nretry-1) printf("point outside first image\n");
	 continue;
       }

   /* get the second grid and do correlation, 2d grid can be larger */

       ndim = std::min(search,srchw+ifftsize+2);
       if (ndim%2==1) ndim++;
       getgrid(2,asrch,search,ndim,soln,pcntr[0],pcntr[1],rmag,&chop,
               elvoff(ibig,0),elvoff(ibig,1),zerothr);
       if((chop>bigchoplimit2)||((srchw==0)&&(chop>choplimit2))) {
	 if(jbig==nretry-1) printf("point outside second image\n");
	 continue;
       }
       ffthset = 1;
       printf("%5d (%7.1f %7.1f)%5dx%3.1f",
	      ibig+1,pcntr[0],pcntr[1],srchw+ifftsize,rmag[1]);
         
       if (srchw>0&&ifftsize<fftsize) {
	 printf("#\n");
	 continue;
       }
	 
       if (ifftsize==fftsize) srchdim = fftsize+srchw; else srchdim = ifftsize;
       int ilin = (search-srchdim)/2;
       int jsmp = ilin;
	 

       // Temporary, I imagine we'll pass this in constructor
       matcher->fftsize = ifftsize;
       matcher->search = search;
       matcher->rfit(ilin,jsmp,&vloff,&vsoff,corr,srchdim,&chip1[0],&asrch[0]);

       double vmax = matcher->correlation_last_match();

       if(vmax<0.00001) { printf("refine err 1a\n"); continue; }
       if(vmax<tretry) { printf("  low correlation\n"); continue; }
         
       if(jbig==0&&vmax<(vmaxfac*rmcor)&&(ibigx>(autofit*2))) {
	 vmaxfac = std::max(0.50,vmaxfac*0.99825); 
	 printf("*\n");
	 continue;
       }
	 
       refinerr = 0;
       if(subpix) 
	 matcher->refine(corr,&vloff,&vsoff,&refinerr);
       vloff *= rmag[0];
       vsoff *= rmag[1];
       if (refinerr!=0) { printf("refine err 2\n"); continue; }
       ocentr[0] = (ilin-(search-srchdim)*0.5)*rmag[0]+vloff;
       ocentr[1] = (jsmp-(search-srchdim)*0.5)*rmag[1]+vsoff;
       double wl = xlt+ocentr[0];
       double ws = xst+ocentr[1];

       second_line[ibig] = (soln[0]*wl+soln[1]*ws+soln[2]+soln[3]*wl*wl+
			    soln[4]*ws*ws+soln[5]*wl*ws);
       second_samp[ibig] = (soln[6]*wl+soln[7]*ws+soln[8]+soln[9]*wl*wl+
			    soln[10]*ws*ws+soln[11]*wl*ws);
       corr_val[ibig] = vmax;
         
       wl += elvoff(ibig, 0);
       ws += elvoff(ibig, 1);
       
       second_hcorr(ibig, 0) = soln[0]*wl+soln[1]*ws+soln[2]+
	 soln[3]*wl*wl+ soln[4]*ws*ws+ soln[5]*wl*ws;
       second_hcorr(ibig, 1) = soln[6]*wl+soln[7]*ws+soln[8]+
	 soln[9]*wl*wl+soln[10]*ws*ws+soln[11]*wl*ws;
	
       first_z[ibig] = getzvl(chip1,ifftsize,centr,getw,getr);
       if (first_z[ibig]<-998.0) { printf("z val err\n"); continue; }
       VicarImageCoordinate acentr(search/2.0+0.5+vloff,
				   search/2.0+0.5+vsoff);
       second_z[ibig] = getzvl(asrch,search,acentr,getw,getr);
       if (second_z[ibig]<-998.0) { printf("z val err\n"); continue; }
	 
       resl = fabs(ocentr[0]);
       ress = fabs(ocentr[1]);
       res = sqrt(resl*resl+ress*ress);
       if (res>thr_resp&&solved) {
	 printf(" (%7.1f,%7.1f)  %7.3f*\n",ocentr[0],ocentr[1],vmax);
	 continue;
       }
       if (solved) thr_resp = std::max(thr_resp*0.95,thr_res);
       line_res[ibig] = 1.;
       if(jbig>0&&ffthalf==0) { /* this is a resetting for the retry case */
	 first_line[ibig] = xlt;
	 first_samp[ibig] = xst;
	 dline = t[0]*xlt+t[1]*xst+t[2];
	 dsamp = t[3]*xlt+t[4]*xst+t[5];
	 first_line_or_east[ibig] = dline;
	 first_samp_or_west[ibig] = dline;
       }

       printf(" (%7.1f,%7.1f)  %7.3f\n",ocentr[0],ocentr[1],vmax);
       if (autoix<autofit) {
	 vmaxvec[autoix] = vmax;
	 vmaxix[autoix++] = ibig+1;
       }
       if (!gotthresh&&(autoix>=autofit)&&autofit>0) {
	 sort8(&vmaxvec[0],&vmaxix[0],autoix);
	 rmcor = vmaxvec[autoix/2];
	 printf("***auto thresh  = %8.4f ***\n",rmcor);
	 gotthresh = 1;
	 /* recover points above newly set threshold */
	 for(int ie=0;ie<autoix;ie++) {
	   if (vmaxvec[ie]>rmcor) {
	     int ibigt = vmaxix[ie]-1;
	     line_res[ibigt] = 2.;
	     a[0][neq] = first_line[ibigt];
	     a[1][neq] = first_samp[ibigt];
	     b[0][neq] = second_line[ibigt];
	     b[1][neq++] = second_samp[ibigt];
	   }
	 }
	 break;
       }
       if (autoix<autofit) break;
       if (vmax<rmcor) break;
       line_res[ibig] = 2.;
       a[0][neq] = xlt;
       a[1][neq] = xst;
       b[0][neq] = second_line[ibig];
       b[1][neq++] = second_samp[ibig];
       if (neq<neqmin) break;
       if (srchw>msrchw) vmaxfac = vmaxfac*1.05;
       if (rmag[0]>rmagmin[0]) vmaxfac = std::max(0.50,vmaxfac*0.9825);
       srchw = std::min(std::max(msrchw,(4*srchw+2*(int)res)/5),srchw);
       if (srchw%2==1) srchw++;
       if (10*srchw<(11*msrchw+ifftsize)) srchw = msrchw;
       if (magshrk[0]=='y'&&rmag[0]>rmagmin[0]) 
	 rmag[0] = std::max(rmag[0]*0.9,rmagmin[0]);
       if (magshrk[0]=='y'&&rmag[1]>rmagmin[1]) 
	 rmag[1] = std::max(rmag[1]*0.9,rmagmin[1]);
       break;
     }
   }

   /* print results of final fit */

   for(int iii=0;iii<ifile.number_row();iii++) {
     if (line_res[iii]>0.5) {
       line_res[iii] = second_line[iii]- 
	 (first_line[iii]*soln[0]+first_samp[iii]*soln[1]+soln[2]+
	  first_line[iii]*first_line[iii]*soln[3]+ 
	  first_samp[iii]*first_samp[iii]*soln[4]+
	  first_line[iii]*first_samp[iii]*soln[5]);
       samp_res[iii] = second_samp[iii]- 
	 (first_line[iii]*soln[6]+ first_samp[iii]*soln[7]+ soln[8]+
	  first_line[iii]*first_line[iii]*soln[9]+
	  first_samp[iii]*first_samp[iii]*soln[10]+
	  first_line[iii]*first_samp[iii]*soln[11]);
     } else 
       line_res[iii] = -9999.;
   }
   for(int ix=0;ix<2;ix++) {
     for(int iii=0;iii<neq;iii++) {
       aa[iii] = b[0][iii];
       aa[iii+neq] = b[1][iii];
       aa[iii+2*neq] = 1.;
       aa[iii+3*neq] = b[0][iii]*b[0][iii];
       aa[iii+4*neq] = b[1][iii]*b[1][iii];
       aa[iii+5*neq] = b[0][iii]*b[1][iii];
       bb[iii] = a[ix][iii];
     }
     if (predfunc&&neq>11)
       matcher->lsqfit(&aa[0],&bb[0],neq,6,&solninv[ix*6],eps,&ierror);
     else 
       if (neq>=neqmin) {
	 matcher->lsqfit(&aa[0],&bb[0],neq,3,&solninv[ix*6],eps,&ierror);
	 for(int jjj=3;jjj<6;jjj++) solninv[ix*6+jjj] = 0.0;
       }
   }
   printf("\n\nfinal line fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n",
	  soln[0],soln[1],soln[2],soln[3],soln[4],soln[5]);
   printf("final samp fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n",
	  soln[6],soln[7],soln[8],soln[9],soln[10],soln[11]);
   if(neq>=neqmin) {
     printf("inv line fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n",
	    solninv[0],solninv[1],solninv[2],solninv[3],solninv[4],solninv[5]);
     printf("inv samp fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n\n",
	    solninv[6],solninv[7],solninv[8],solninv[9],solninv[10],solninv[11]);
   }
   
   /* correct columns 6,7 for the predictor elevation offset (elvcor) */

   for(int jjj=0;jjj<ifile.number_row();jjj++) {
     second_line[jjj] = second_hcorr(jjj, 0);
     second_samp[jjj] = second_hcorr(jjj, 1);
   }
   // Oddly, it is a VICAR convention to return a 1 for success
   // (rather than the normal 0 used in most unix programs).
   return 1;
} catch(const Exception& E) {
  zmabend(const_cast<char*>(E.what()));
}
}
