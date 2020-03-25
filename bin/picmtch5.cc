// This is picmtch4 code. We will slowly convert this to C++/geocal,
// but right now this is pretty much just cleaned up for C++
#include <cmath>
#include <zvproto.h>
#include <ibisfile.h>
#include <fftw3.h>
#include <algorithm>
#include <boost/make_shared.hpp>

#include "vicar_argument.h"
#include "ibis_file.h"
#include "constant.h"
#include "phase_correlation_matcher.h"
#include "quadratic_geometric_model.h"
#include "vicar_raster_image.h"
#include "geometric_model_image.h"

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

double rvecl[5] = {0.,1.,-1.,1.,-1.};
double rvecs[5] = {0.,1.,-1.,-1.,1.};

// Temporary, this will go away in a bit.
boost::shared_ptr<PhaseCorrelationMatcher> matcher;

/*=====================================================================

throwout

throws out the worst point from the a,b arrays using a linear fit.
     
*/

void throwout(GeometricTiePoints& Tpset)
{
  QuadraticGeometricModel gm(QuadraticGeometricModel::LINEAR);
  gm.fit_transformation(Tpset);
   
  /* calculate residuals and find the worst point */

  double rmax = -1;
  int imax = -1;
  blitz::Array<double, 2> x = Tpset.x();
  blitz::Array<double, 2> y = Tpset.y();
  for(int i=0;i< x.rows();i++) {
    ImageCoordinate icpred =
      gm.original_image_coordinate(ImageCoordinate(x(i,0), x(i, 1)));
    double rx = y(i,0)-icpred.line;
    double ry = y(i,1)-icpred.sample;
    double r = rx*rx+ry*ry;
    if(r > rmax) {
      imax = i;
      rmax = r;
    }
  }
   
  /* discard the worst point */
  if(imax >= 0)
    Tpset.remove_point(imax);
  return;
}

int main(int Argc, char *Argv[])
{
try {
  int chop,picout,srchdim;
  int retry,refinerr;
  double vloff, vsoff;
  double res,corr[3][3];
  double dline,dsamp;

  int status,parmcnt,o_unit;
  
  VicarArgument va(Argc, Argv);

  zifmessage(const_cast<char*>("picmtch5 version Thu Nov 09 2017"));
   
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
  if (msrc%2==1) msrc++;
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

  // Note, with the old picmtch4 the default value for fitmin was
  // 0.19. But for some bizarre reason, zvp used by picmtch4 returned
  // 0.0. To match the old behavior, we've replaced the defualt fitmin
  // with 0.0. 
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
 
  boost::shared_ptr<QuadraticGeometricModel> gm1
    (new QuadraticGeometricModel(QuadraticGeometricModel::LINEAR, 
				 rmagtae[0], rmagtae[1]));
 if (getw%2==0) throw Exception("zwind must be odd");
  retry = (nretry>1);
  if (retry&&gcpf) throw Exception("can't retry gcpf");

  /* open the ibis interface file */

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

  // Get elevation offset, which is either read from the ibis file or
  // set to zero.
  std::vector<ImageCoordinate> elvoff(ifile.number_row(), ImageCoordinate(0,0));
  if(elvcor) {
    const std::vector<double>& line_elv_offset = ifile.column<double>(11).data;
    const std::vector<double>& samp_elv_offset = ifile.column<double>(12).data;
    for(int i = 0; i < ifile.number_row(); ++i)
      elvoff[i] = ImageCoordinate(line_elv_offset[i], samp_elv_offset[i]);
  }
  // Second input line/sample, after doing height correction.
  blitz::Array<double, 2> second_hcorr(ifile.number_row(), 2);
  second_hcorr = 0.0;

  boost::shared_ptr<RasterImage> img1 = vicar_open(inpfname[0]);
  boost::shared_ptr<RasterImage> img2 = vicar_open(inpfname[1]);
  GeometricModelImage gimg1(img1, gm1, 
	    (int) floor(img1->number_line() * gm1->magnify_line() + 0.5),
	    (int) floor(img1->number_sample() * gm1->magnify_sample() + 0.5));
  if (gcpf) throw Exception("ground control point file not implemented yet");
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
    itie.resize(6);
    itie[0] = 1.0;
    itie[1] = 1.0;
    itie[2] = img1->number_line();
    itie[3] = 1.0;
    itie[4] = 1.0;
    itie[5] = img1->number_sample();
  }
  for(int iii=0;iii<ifile.number_row();iii++) {
    dline = tinv[0]*first_line_or_east[iii]+ 
      tinv[1]*first_samp_or_west[iii]+tinv[2];
    dsamp = tinv[3]*first_line_or_east[iii]+
      tinv[4]*first_samp_or_west[iii]+tinv[5];
    first_line[iii] = dline;
    first_samp[iii] = dsamp;
  }
  std::vector<ImageCoordinate> xinit;
  for(int iii=0;iii<6;iii+=2) {
    if (!geotie) {
      dline = tinv[0]*itie[iii]+tinv[1]*itie[iii+1]+tinv[2];
      dsamp = tinv[3]*itie[iii]+tinv[4]*itie[iii+1]+tinv[5];
    } else {
      dline = itie[iii];
      dsamp = itie[iii+1];
    }
    xinit.push_back(ImageCoordinate(dline, dsamp));
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
  std::vector<ImageCoordinate> yinit;
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
      yinit.push_back(ImageCoordinate(otie[iii], otie[iii+1]));
    }
  } else 
    for(int iii=0;iii<6;iii+=2) {
      dline = uinv[0]*otie[iii]+uinv[1]*otie[iii+1]+uinv[2];
      dsamp = uinv[3]*otie[iii]+uinv[4]*otie[iii+1]+uinv[5];
      yinit.push_back(ImageCoordinate(dline, dsamp));
    }
  /* open the first image file */
  // this should go away.
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
  
  // Rotate the initial mapping by the given angle
  if(fabs(angoff)>0.000001)
    for(int iii=0; iii < 3; ++iii) {
      double rv1 = yinit[iii].line-rctl;
      double rw1 = yinit[iii].sample-rcts;
      double rr = sqrt(rv1*rv1+rw1*rw1+0.0000001);
      double theta = atan2(rw1,rv1)-angoff;
      double rv2 = rr*cos(theta);
      double rw2 = rr*sin(theta);
      yinit[iii] = ImageCoordinate(rv2+rctl, rw2+rcts);
    }
  
  /* outer loop over the tiepoints or gcp's */
  /* gcp option inactive for now... I need a sample gcp data set */
  
  boost::shared_ptr<GeometricTiePoints> tpset =
    boost::make_shared<GeometricTiePoints>();
  for(int i = 0; i < 3; ++i)
    tpset->add_point(xinit[i], yinit[i]);
  // We want to replace the approximate tiepoints with image matching
  // as we start matching.
  tpset->start_replacing();
  int throwcount = 4; 
  int neqmin = std::max(autofit/2+throwcount,9);
  boost::shared_ptr<QuadraticGeometricModel> gm2
    (new QuadraticGeometricModel(tpset, neqmin,
				 (predfunc ?
   				  QuadraticGeometricModel::QUADRATIC :
   				  QuadraticGeometricModel::LINEAR),
   				 rmagtae[0], rmagtae[1]));
  GeometricModelImage gimg2(img2, gm2, 
   	    (int) floor(img2->number_line() * gm2->magnify_line() + 0.5),
   	    (int) floor(img2->number_sample() * gm2->magnify_sample() + 0.5));
  int lastneq = -1; 
  int autoix = 0; 
  int gotthresh = 0; 
  int scount = 0;
  int srchw = search-fftsize; 
  int msrchw = msrc-fftsize; 
  bool solved = false; 
  double vmaxfac = 0.9;
  int maxredo = std::min(nredo,ifile.number_row()); 
  double thr_resp = 4.0*thr_res;
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
      
      if(( ibigx ==0 || gm2->enough_tie_point_to_fit()) &&
	 tpset->number_point() != lastneq) {
	if (ibigx>0&&throwcount>0) {
	  solved = true;
	  printf("***auto fit:neq = %8d ***\n",tpset->number_point());
	}
	if (ibigx>0) {
	  for(int iii=0;iii<4;iii++) {
	    if (throwcount<=0) break;
	    throwout(*tpset);
	    --throwcount;
	  }
	  // Go ahead and fit transformation, even through we
	  // have reduced the number of points by throwing them
	  // out. Perhaps move this into throwout.
	  gm2->fit_transformation(*tpset);
	}
	if (scount%10==9) {
	  throwout(*tpset);
	  --throwcount;
	  gm2->fit_transformation(*tpset);
	}
	scount++;
	lastneq = tpset->number_point();
      }
         
      int choplimit1 = (int)(ifftsize*ifftsize*zerolim);
      int choplimit2 = (int)(ifftsize*ifftsize*zerolim2);
      int bigchoplimit2 = search*search/2;   /* see zerolimit in rfit
					      */
      // Perhaps this should be VicarImageCoordinate?
      ImageCoordinate first_ic(first_line[ibig]+rretry*rvecl[jbig],
			       first_samp[ibig]+rretry*rvecs[jbig]);
      // Not sure if this is ImageCoordinate or VicarImageCoordinate
      ImageCoordinate pcntr((int)(first_ic.line+0.5), 
			    (int)(first_ic.sample+0.5));
      ImageCoordinate pcntr2 = gm1->resampled_image_coordinate(pcntr);
      blitz::Array<double, 2> chip1_n =
	gimg1.interpolate(pcntr2.line - ifftsize / 2,
			  pcntr2.sample - ifftsize / 2,
			  ifftsize, ifftsize);
      chop = blitz::count(chip1_n < zerothr + 0.01);
      if (chop>choplimit1) {
	if (jbig==nretry-1) printf("point outside first image\n");
	continue;
      }
      
   /* get the second grid and do correlation, 2d grid can be larger */

      if(ifftsize==fftsize) 
	srchdim = fftsize+srchw; 
      else 
	srchdim = ifftsize;
      ImageCoordinate pcntr3 = gm1->resampled_image_coordinate
	(ImageCoordinate(pcntr.line + elvoff[ibig].line,
			 pcntr.sample + elvoff[ibig].sample));
      blitz::Array<double, 2> asrch_n =
	gimg2.interpolate(pcntr3.line - srchdim / 2,
			  pcntr3.sample - srchdim / 2,
			  srchdim, srchdim);
      chop = blitz::count(asrch_n < zerothr + 0.01);
      if((chop>bigchoplimit2)||((srchw==0)&&(chop>choplimit2))) {
	if(jbig==nretry-1) printf("point outside second image\n");
	continue;
      }
      ffthset = 1;
      printf("%5d (%7.1f %7.1f)%5dx%3.1f",
	     ibig+1,pcntr.line,pcntr.sample,srchw+ifftsize,
	     gm1->magnify_sample());
      
      if (srchw>0&&ifftsize<fftsize) {
	printf("#\n");
	continue;
      }
	 
       // Temporary, I imagine we'll pass this in constructor
      matcher->fftsize = ifftsize;
      matcher->search = search;
      matcher->rfit(&vloff,&vsoff,corr,chip1_n, asrch_n);
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
      if (refinerr!=0) { printf("refine err 2\n"); continue; }
      ImageCoordinate second_ic_offset_resampled(vloff, vsoff);
      
      ImageCoordinate
	first_ic_resampled = gm1->resampled_image_coordinate(first_ic);
      ImageCoordinate
	second_ic = gm2->original_image_coordinate(ImageCoordinate
           (first_ic_resampled.line + second_ic_offset_resampled.line,
	    first_ic_resampled.sample + second_ic_offset_resampled.sample));

      second_line[ibig] = second_ic.line;
      second_samp[ibig] = second_ic.sample;
      
      corr_val[ibig] = vmax;

      ImageCoordinate elvoff_resampled = 
	gm1->resampled_image_coordinate(elvoff[ibig]);
      ImageCoordinate
	second_ic_hcorr = gm2->original_image_coordinate(ImageCoordinate
           (first_ic_resampled.line + second_ic_offset_resampled.line +
	    elvoff_resampled.line,
	    first_ic_resampled.sample + second_ic_offset_resampled.sample +
	    elvoff_resampled.sample));
       
      second_hcorr(ibig, 0) = second_ic_hcorr.line;
      second_hcorr(ibig, 1) = second_ic_hcorr.sample;
      
      ImageCoordinate centr(first_ic.line-pcntr.line+ifftsize/2.0+.5,
			    first_ic.sample-pcntr.sample+ifftsize/2.0+.5);
      first_z[ibig] = matcher->getzvl(chip1_n,centr,getw,getr);
      if (first_z[ibig]<-998.0) { 
	printf("z val err\n"); 
	continue; 
      }
      ImageCoordinate acentr(search/2.0+0.5+vloff*gm1->magnify_line(),
			     search/2.0+0.5+vsoff*gm1->magnify_sample());
      second_z[ibig] = matcher->getzvl(asrch_n,acentr,getw,getr);
      if (second_z[ibig]<-998.0) { 
	printf("z val err\n"); 
	continue; 
      }
      ImageCoordinate second_ic_offset = 
	gm1->original_image_coordinate(second_ic_offset_resampled);
      res = sqrt(second_ic_offset.line * second_ic_offset.line +
		 second_ic_offset.sample * second_ic_offset.sample);
      if (res>thr_resp&&solved) {
	printf(" (%7.1f,%7.1f)  %7.3f*\n",second_ic_offset.line,
	       second_ic_offset.sample,vmax);
	continue;
      }
      if (solved) thr_resp = std::max(thr_resp*0.95,thr_res);
      line_res[ibig] = 1.;
      if(jbig>0&&ffthalf==0) { /* this is a resetting for the retry case */
	first_line[ibig] = first_ic.line;
	first_samp[ibig] = first_ic.sample;
	dline = t[0]*first_ic.line+t[1]*first_ic.sample+t[2];
	dsamp = t[3]*first_ic.line+t[4]*first_ic.sample+t[5];
	first_line_or_east[ibig] = dline;
	first_samp_or_west[ibig] = dline;
      }
      
      printf(" (%7.1f,%7.1f)  %7.3f\n",second_ic_offset.line,
	     second_ic_offset.sample,vmax);
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
	    tpset->add_point
	      (ImageCoordinate(first_line[ibigt],first_samp[ibigt]),
	       ImageCoordinate(second_line[ibigt],second_samp[ibigt]));
	  }
	}
	break;
      }
      if (autoix<autofit) break;
      if (vmax<rmcor) break;
      line_res[ibig] = 2.;
      tpset->add_point(first_ic,
		      ImageCoordinate(second_line[ibig],second_samp[ibig]));
      if (ibigx==0 || tpset->number_point()<neqmin) break;
      if (srchw>msrchw) vmaxfac = vmaxfac*1.05;
      if (gm1->magnify_line() > rmagmin[0]) 
	vmaxfac = std::max(0.50,vmaxfac*0.9825);
      srchw = std::min(std::max(msrchw,(4*srchw+2*(int)res)/5),srchw);
      if (srchw%2==1) srchw++;
      if (10*srchw<(11*msrchw+ifftsize)) srchw = msrchw;
      if (magshrk[0]=='y' && gm1->magnify_line() > rmagmin[0]) {
	gm1->magnify_line(std::max(gm1->magnify_line()*0.9,rmagmin[0]));
	gm2->magnify_line(gm1->magnify_line());
      }
      if (magshrk[0]=='y' && gm1->magnify_sample() >rmagmin[1]) {
	gm1->magnify_sample(std::max(gm1->magnify_sample()*0.9,rmagmin[1]));
	gm2->magnify_sample(gm1->magnify_sample());
      }
      break;
    }
  }
  
   /* print results of final fit */

  for(int iii=0;iii<ifile.number_row();iii++) {
    if (line_res[iii]>0.5) {
      ImageCoordinate first_ic_resampled = gm1->resampled_image_coordinate
	(ImageCoordinate(first_line[iii], first_samp[iii]));
      ImageCoordinate second_pred = 
	gm2->original_image_coordinate(first_ic_resampled);
      line_res[iii] = second_line[iii]-second_pred.line;
      samp_res[iii] = second_samp[iii]-second_pred.sample; 
    } else 
      line_res[iii] = -9999.;
  }
  blitz::Array<double, 1>& soln = gm2->transformation();
  blitz::Array<double, 1>& solninv = gm2->inverse_transformation();
  printf("\n\nfinal line fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n",
	 soln(0),soln(1),soln(2),soln(3),soln(4),soln(5));
  printf("final samp fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n",
	 soln(6),soln(7),soln(8),soln(9),soln(10),soln(11));
  if(tpset->number_point()>=neqmin) {
    printf("inv line fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n",
	   solninv(0),solninv(1),solninv(2),solninv(3),solninv(4),solninv(5));
    printf("inv samp fit %12.6f x %12.6f y %12.6f %12.6f x2 %12.6f y2%12.6f xy\n\n",
	   solninv(6),solninv(7),solninv(8),solninv(9),solninv(10),solninv(11));
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
