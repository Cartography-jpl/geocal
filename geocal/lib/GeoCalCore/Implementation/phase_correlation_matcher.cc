#include "phase_correlation_matcher.h"
#include <blitz/array.h>

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor. 
/// \param Template_size Size of the area that we do use in the first 
///        image. This was called FFTSIZE in Al Zobrist's picmtch4
///        program. Note that this must be even.
/// \param Search_size Size to search in the second image. This should
///        be > Template_size, and even.
//-----------------------------------------------------------------------

PhaseCorrelationMatcher::PhaseCorrelationMatcher(
int Template_size, 
int Search_size
)
: fftsize(Template_size),
  search(Search_size),
  nohpf(false),
  subpix(true)
{
  range_min_check(Template_size, 0);
  range_min_check(Search_size, Template_size);
  if(Template_size % 2 != 0 ||
     Search_size % 2 != 0)
    throw Exception("Template and search size need to be even");
  afftin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * search*search);
  afftout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * search*search);
  bfftin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * search*search);
  bfftout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * search*search);
}

PhaseCorrelationMatcher::~PhaseCorrelationMatcher() 
{
  fftw_free(afftin);
  fftw_free(afftout);
  fftw_free(bfftin);
  fftw_free(bfftout);
}

// See base class for description

void PhaseCorrelationMatcher::match_mask
(const RasterImage& Ref, 
 const ImageMask& Ref_mask,			 
 const RasterImage& New, 
 const ImageMask& New_mask,
 const ImageCoordinate& Ref_loc, 
 const ImageCoordinate& New_guess, 
 ImageCoordinate& New_res,
 double& Line_sigma, double& Sample_sigma,
 bool& Success, int* Diagnostic) const
{
  Success = false;		// We'll change this once we succeed.
  int ref_line = int(Ref_loc.line) - template_size() / 2;
  int ref_sample = int(Ref_loc.sample) - template_size() / 2;
  int new_line = int(New_guess.line) - search_size() / 2;
  int new_sample = int(New_guess.sample) - search_size() / 2;

//-----------------------------------------------------------------------
/// Check if we have enough data in the images, if not we indicate
/// that we are too close to the edge and fail.
///
/// TODO Al's code allows some amount of the image to be over the edge
///      We may want to look into that, but for now just fail.
//-----------------------------------------------------------------------

  if(ref_line < 0 || ref_line + template_size() > Ref.number_line()   ||
     ref_sample < 0 || ref_sample + template_size() > Ref.number_sample() ||
     new_line < 0 || new_line + search_size() > New.number_line()   ||
     new_sample < 0 || new_sample + search_size() > New.number_sample()) {
    if(Diagnostic)
      *Diagnostic = TOO_CLOSE_TO_IMAGE_EDGE;
    return;
  }

//-----------------------------------------------------------------------
// Check if any of the date we want to use is masked.
//-----------------------------------------------------------------------

  if(Ref_mask.area_any_masked(ref_line, ref_sample, template_size(),
			      template_size()) ||
     New_mask.area_any_masked(new_line, new_sample, search_size(),
			      search_size())) {
    if(Diagnostic)
      *Diagnostic = IMAGE_MASKED;
    return;
  }
  Array<double, 2> chip1 = Ref.read_double(ref_line, ref_sample, 
					   template_size(), template_size());
  Array<double, 2> asrch = New.read_double(new_line, new_sample,
					   search_size(), search_size());
  Array<float, 2> chip1f(chip1.shape()), asrchf(asrch.shape());
  chip1f = cast<float>(chip1);
  asrchf = cast<float>(asrch);
  // This can be smaller than search, but we don't support that yet.
  int srchdim = search;
  int ilin = (search - srchdim) / 2;
  int jsmp = ilin;
  float vloff, vsoff, corr[3][3];
  rfit(ilin, jsmp, &vloff, &vsoff, corr, srchdim, chip1f.data(), 
       asrchf.data());
  int referr = 0;
  if(subpix)
    refine(corr, &vloff, &vsoff, &referr);
  std::cerr << "vloff: " << vloff << "\n"
	    << "vsoff: " << vsoff << "\n"
	    << "vmax: " << vmax << "\n";
  // Add threshold test here
  if(!referr) {
    New_res.line = new_line + search_size() / 2 + vloff;
    New_res.sample = new_sample + search_size() / 2 + vsoff;
    Success = true;
    if(Diagnostic)
      *Diagnostic = NO_FAIL;
    if(subpix) {
      Line_sigma = 0.1;
      Sample_sigma = 0.1;
    } else {
      Line_sigma = 0.5;
      Sample_sigma = 0.5;
    }
  } else {
    if(Diagnostic)
      *Diagnostic = REFINE_FAILED;
  }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void PhaseCorrelationMatcher::print(std::ostream& Os) const
{
  Os << "Phase correlation matcher:\n"
     << "  Template size: " << template_size() << "\n"
     << "  Search size:   " << search_size() << "\n";
}

//-----------------------------------------------------------------------
/// rfit
/// 
/// uses fft to compute correlation function on two images stored
/// in the specific global arrays chip1 and asrch.  Chip1 is a fixed
/// fftsize x fftsize array and asrch is a larger array for correlation
/// matching.  The correlation area is a srchdim x srchdim subset of the
/// larger array.
/// the method is fftinverse(fft(a) * conj(fft(b))
/// \param ilin: input, int ilin;
///	line offset into asrch (the larger array)
/// \param jsmp: input, int jsmp;
///	sample offset into asrch (the larger array)
/// \param vloff: output, float *vloff;
///	the line offset of the peak match relative to the
///	center of chip1 (16.0,16.0)
/// \param vsoff: output, float *vsoff;
///	the sample offset of the peak match relative to the
///	center of chip1 (16.0,16.0)
/// \param corr: output, float corr[3][3]
///	the 3 x 3 part of the correlation centered at the
///	peak, for use in subpixel refinement
/// \param srchdim: input, int srchdim;
///	The current size of the search area in chip 2
//-----------------------------------------------------------------------

void PhaseCorrelationMatcher::rfit
(int ilin,int jsmp,float* vloff,float* vsoff,float corr[3][3],
 int srchdim, float *chip1, float* asrch) const
{
   int i,j,ixmax,jxmax,koff;
   int quadmark,mincor,maxcor;
   float t,v,bij,bij1,tvmax,rv,ttemp,avg;
   fftw_plan p;

   quadmark = srchdim/2;
   mincor = fftsize/6-1;
   maxcor = srchdim-mincor;
   
   avg = 0.0;
   for (i=0;i<fftsize;i++)
      for (j=0;j<fftsize;j++)
	 avg += chip1[j*fftsize+i];
   avg /= (float)(fftsize*fftsize);
   
   /*printf("ZERO DETECT CODE ON:ilin %d\n",ilin);  save for debugging*/
   for (i=0;i<srchdim;i++)
      for (j=0;j<srchdim;j++)
	 {
	 bfftin[j*srchdim+i][0] = asrch[(ilin+j)*search+(jsmp+i)];
         /*if (bfftin[j*srchdim+i][0]<120&&(i>56)&&(j>56)&&(i<72)&&(j<72))
            printf("B-ZERO:i,j,bfftin[j*srchdim+i][0] %d %d %7.1f\n",
               i,j,bfftin[j*srchdim+i][0]);*/
	 bfftin[j*srchdim+i][1] = 0.0;
	 afftin[j*srchdim+i][0] = avg;
	 afftin[j*srchdim+i][1] = 0.0;
	 }
   koff = (srchdim-fftsize)/2;
   for (i=0;i<fftsize;i++)
      for (j=0;j<fftsize;j++)
         {
	 afftin[(j+koff)*srchdim+i+koff][0] = chip1[j*fftsize+i];
         /*if (chip1[j*fftsize+i]==0) printf("A-ZERO:i,j %d %d\n",i,j);*/
         }
   
   p = fftw_plan_dft_2d(srchdim,srchdim,afftin,afftout,FFTW_FORWARD,FFTW_ESTIMATE);
   fftw_execute(p);
   fftw_destroy_plan(p);
   
   p = fftw_plan_dft_2d(srchdim,srchdim,bfftin,bfftout,FFTW_FORWARD,FFTW_ESTIMATE);
   fftw_execute(p);
   fftw_destroy_plan(p);
      
   afftout[0][0] = 0.;
   afftout[0][1] = 0.;
   if (!nohpf) for (i=1;i<srchdim;i++)
      {
      afftout[i][0] = 0.;
      afftout[i][1] = 0.;
      afftout[i*srchdim][0] = 0.;
      afftout[i*srchdim][1] = 0.;
      }
   
   for (i=0;i<srchdim;i++)
      for (j=0;j<srchdim;j++)
	 {
	 bij = afftout[j*srchdim+i][0]*bfftout[j*srchdim+i][0]+
               afftout[j*srchdim+i][1]*bfftout[j*srchdim+i][1];
	 bij1 = afftout[j*srchdim+i][0]*bfftout[j*srchdim+i][1]-
               afftout[j*srchdim+i][1]*bfftout[j*srchdim+i][0];
	 v = sqrt((double)(bij*bij+bij1*bij1));
	 if (v<1.e-6) v = 1.e-6;
	 bfftin[j*srchdim+i][0] = bij/v;
	 bfftin[j*srchdim+i][1] = bij1/v;
	 }
   p = fftw_plan_dft_2d(srchdim,srchdim,bfftin,bfftout,FFTW_BACKWARD,FFTW_ESTIMATE);
   fftw_execute(p);
   fftw_destroy_plan(p);

   /* quadrant swap */
   
   for (i=0;i<quadmark;i++)
      for (j=0;j<quadmark;j++)
	 {
	 t = bfftout[i*srchdim+j][0];
	 bfftout[i*srchdim+j][0] = bfftout[(i+quadmark)*srchdim+j+quadmark][0];
	 bfftout[(i+quadmark)*srchdim+j+quadmark][0] = t;
	 t = bfftout[(i+quadmark)*srchdim+j][0];
	 bfftout[(i+quadmark)*srchdim+j][0] = bfftout[i*srchdim+j+quadmark][0];
	 bfftout[i*srchdim+j+quadmark][0] = t;
	 }

   tvmax = -1.e20;
   ixmax = mincor; jxmax = mincor;
   for (i=mincor;i<maxcor;i++)
      for (j=mincor;j<maxcor;j++)
	 {
	 rv = bfftout[j*srchdim+i][0];
	 if (rv>tvmax) { tvmax = rv; ixmax = i; jxmax = j; }
	 }
   if (tvmax<0.0) tvmax = 0.0;
   
   /* normalized for varying footprints by three lines below */
   
   ttemp = log10((double)srchdim)/log10((double)2.0);
   ttemp = ttemp*ttemp;
   vmax = tvmax*10.0/(srchdim*ttemp*ttemp);
   *vloff = (float)jxmax-quadmark;
   *vsoff = (float)ixmax-quadmark;
   /*printf("ilin,srchdim,*vloff,*vsoff,quadmark,koff %d %d %7.1f %7.1f %d %d\n",
           ilin,srchdim,*vloff,*vsoff,quadmark,koff);*/
   
   for (i=0;i<3;i++)
      for (j=0;j<3;j++)
	 corr[j][i] = bfftout[(jxmax+j-1)*srchdim+ixmax+i-1][0];

   return;
}

//-----------------------------------------------------------------------
/// refine
///
/// refines the match produced by routine rfit
///
/// uses the 3 x 3 center of the fft correlation matrix and fits
/// a general quadratic (least squares), then sets the partial
/// derivatives in x and y to zero to find the max.  The max
/// must be within 0.7 pixels of the center to qualify (otherwise
/// the center itself is returned).
///
/// \param corr: input, float corr[3][3];
///	center of the correlation matrix as
///	returned by rfit
/// \param vloff: input,output, float *vloff;
///	the match produced by routine rfit at
///	the exact center of the 3 x 3, output
///	is the refined value
/// \param vsoff: input,output, float *vsoff;
///	the match produced by routine rfit at
///	the exact center of the 3 x 3, output
///	is the refined value
/// \param ireferr: input,output, int *ireferr;
///	error return, 0 if OK, 1 if error
//-----------------------------------------------------------------------

void PhaseCorrelationMatcher::refine(float corr[3][3],float* vloff,
				     float* vsoff,int * ireferr) const
{
   int i,j,iq,ierror;
   double a[54],b[9],s[6],jvar,ivar,imx,jmx,eps;
   
   *ireferr = 0;
   for (i=0;i<3;i++)
      {
      ivar = (float)i-1.0;
      for (j=0;j<3;j++)
	 {
	 jvar = (float)j-1.0;
	 iq = i*3+j;
	 a[iq] = jvar*jvar;
	 a[iq+9] = jvar*ivar;
	 a[iq+18] = ivar*ivar;
	 a[iq+27] = jvar;
	 a[iq+36] = ivar;
	 a[iq+45] = 1.;
	 b[iq] = corr[i][j];
	 }
      }
   eps=1.e-7;
   lsqfit(a,b,9,6,s,eps,&ierror);
   if (ierror!=0 || s[0]==0 || (4.0*s[2]*s[0]==s[1]*s[1]))
      { printf("sing rfit"); *ireferr = 1; return; }
   imx = (s[1]*s[3]-2.0*s[4]*s[0])/(4.0*s[2]*s[0]-s[1]*s[1]);
   jmx = -(s[1]*imx+s[3])/(2.0*s[0]);
   if (imx*imx+jmx*jmx>=2.0) { *ireferr = 1; return; }
   *vloff = *vloff+imx;
   *vsoff = *vsoff+jmx;
   return;
}

//-----------------------------------------------------------------------
/// lsqfit
///
/// lsqfit solves for the minimum least squares fit (for ax=r, the minimum
/// over all x of L2 norm of r-ax)
///
/// The matrix a is stored by column order
/// \param a: input and output, double *a;
///	m by n coefficient matrix, destroyed.
/// \param  r: input and output, double *r;
///	input right hand m-vector.
/// \param  m: input, int m;
///	number of linear equations.
/// \param  n: input, int n;
///	number of independent coords; dimension of x.
/// \param x: output, double *x;
///	solution vector.
/// \param eps: input double eps;
///	gaussian pivot tolerance (usually set to 1.e-14)
/// \param ierror: output int *ierror;
///	result 0=OK; K=singular at kth column
///	-1=zero matrix a; -2=m<n
//-----------------------------------------------------------------------

void PhaseCorrelationMatcher::lsqfit( double * a, double * r, int m, int n, double * x, double eps, int * ierror ) const
{
   double *buf,*alznorm; int *ipiv;
   int i,j,k,il,iu,kpiv=0,id,jl,ii,kl;
   double piv,h,sig,tol,beta,alzmax;

   buf=(double *)malloc(16*n);
   ipiv=(int *)malloc(4*n);
   alznorm=(double *)malloc(8*n);
   
   /* normalize the columns, then fix solution at end */
   
   for (i=0;i<n;i++)
      {
      alzmax = fabs(a[i*m]);
      for (j=1;j<m;j++) if (fabs(a[i*m+j])>alzmax) alzmax = fabs(a[i*m+j]);
      if (alzmax>0.0) for (j=0;j<m;j++) a[i*m+j] /= alzmax;
      alznorm[i] = alzmax;
      }

   /* end of normalize the columns */

   if (m<n) { *ierror = -2; goto done; }
   piv = 0.;
   iu = 0;
   for (k=1;k<=n;k++)
      {
      ipiv[k-1] = k;
      h = 0.;
      il = iu+1;
      iu = iu+m;
      for (i=il;i<=iu;i++) h = h+a[i-1]*a[i-1];
      buf[k-1] = h;
      if (h<=piv) continue;
      piv = h;
      kpiv = k;
      }
   if (piv<=0.) { *ierror = -1; goto done; }
   sig = sqrt(piv);
   tol = sig*fabs(eps);
   
   il = -m;
   for (k=1;k<=n;k++)
      {
      il = il+m+1;
      iu = il+m-k;
      i = kpiv-k;
      if (i>0)
	 {
	 h = buf[k-1];
	 buf[k-1] = buf[kpiv-1];
	 buf[kpiv-1] = h;
	 id = i*m;
	 for (i=il;i<=iu;i++)
	    {
	    j = i+id;
	    h = a[i-1];
	    a[i-1] = a[j-1];
	    a[j-1] = h;
	    }
	 }
      if (k>1)
	 {
	 sig = 0.;
	 for (i=il;i<=iu;i++) sig = sig+a[i-1]*a[i-1];
	 sig = sqrt((double)sig);
	 if (sig<tol) { *ierror = k-1; goto done; }
	 }
      h = a[il-1];
      if (h<0.) sig = -sig;
      ipiv[kpiv-1] = ipiv[k-1];
      ipiv[k-1] = kpiv;
      beta = h+sig;
      a[il-1] = beta;
      beta = 1./(sig*beta);
      j = n+k;
      buf[j-1] = -sig;
      if (k<n)
	 {
	 piv = 0.;
	 id = 0;
	 jl = k+1;
	 kpiv = jl;
	 for (j=jl;j<=n;j++)
	    {
	    id = id+m;
	    h = 0.;
	    for (i=il;i<=iu;i++)
	       {
	       ii = i+id;
	       h = h+a[i-1]*a[ii-1];
	       }
	    h = beta*h;
	    for (i=il;i<=iu;i++)
	       {
	       ii = i+id;
	       a[ii-1] = a[ii-1]-a[i-1]*h;
	       }
	    ii = il+id;
	    h = buf[j-1]-a[ii-1]*a[ii-1];
	    buf[j-1] = h;
	    if (h<=piv) continue;
	    piv = h;
	    kpiv = j;
	    }
	 }
      h = 0.;
      ii = il;
      for (i=k;i<=m;i++)
	 {
	 h = h+a[ii-1]*r[i-1];
	 ii = ii+1;
	 }
      h = beta*h;
      ii = il;
      for (i=k;i<=m;i++)
	 {
	 r[i-1] = r[i-1]-a[ii-1]*h;
	 ii = ii+1;
	 }
      }

   *ierror = 0;
   piv = 1./buf[2*n-1];
   x[n-1] = piv*r[n-1];
   if (n>1)
      {
      jl = (n-1)*m+n;
      for (j=2;j<=n;j++)
	 {
	 jl = jl-m-1;
	 k = n+n+1-j;
	 piv = 1./buf[k-1];
	 kl = k-n;
	 id = ipiv[kl-1]-kl;
	 il = 2-j;
	 h = r[kl-1];
	 il = il+n;
	 iu = il+j-2;
	 ii = jl;
	 for (i=il;i<=iu;i++)
	    {
	    ii = ii+m;
	    h = h-a[ii-1]*x[i-1];
	    }
	 i = il-1;
	 ii = i+id;
	 x[i-1] = x[ii-1];
	 x[ii-1] = piv*h;
	 }
      }
      
   done:
   free(buf);
   free(ipiv);
   for (i=0;i<n;i++)
      if (x[i]!=0.0)
         {
         for (j=0;j<n;j++) x[j] /= alznorm[j];
         return;
         }
   *ierror = -1;
   return;
}
