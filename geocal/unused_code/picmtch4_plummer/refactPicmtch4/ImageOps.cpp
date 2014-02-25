/*===========================================================================
=                                                                           =
=                             ImageOps                                      =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME:

!ABSTRACT:

   Image operation support functions.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#include "ImageOps.h"

//***************************************************************************
// void findBelowThres(
// cv::Mat& matrix, //matrix to check
// std::vector<cv::Point2f>& belThres, //locations in matrix below threshold
// double thresh = 0) //threshold to check
//***************************************************************************
void findBelowThres(cv::Mat& matrix, std::vector<cv::Point2f>& belThres, 
		    double thresh)
{
  for (int y = 0; y < matrix.rows; y++)
    for(int x = 0;x < matrix.cols; x++)
      if (matrix.at<uchar>(y, x) > thresh)
	belThres.push_back(cv::Point2f(x,y));
}

//***************************************************************************
// double getAmountNonZero(
// cv::Mat& img) matrix to check percentage of elements that are not zero
//***************************************************************************
double getAmountNonZero(cv::Mat& img)
{
  return ((double)countNonZero(img)/img.total());
}

/*=====================================================================

  getgrid

  extracts a subimage from an image.  the subimage is specified by
  a linear transformation and the point that (when transformed) becomes
  the center of the subimage.  ndim can be used to obtain only the
  central part of the array if the outer border is not needed, for
  example when picmtch4 has narrowed a search.


  arguments:

  1. img: input, GeoCal::GeoCalCore::RasterImage& img;
  the image to extract a chip from
  2. chip: output, float *chip;
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
  6. centrx: input, float centrx;
  trans(centrx,centry) will be the center
  of the sampled grid
  7. centry: input, float centry;
  trans(centrx,centry) will be the center
  of the sampled grid
  8. rmag: input, int rmag;
  applied to grid to obtain a larger (or
  smaller) area chip
  9. chop: output, int *chop;
  number of subimage points outside of
  input image plus zero points
*/

void getgrid(GeoCal::GeoCalCore::RasterImage& img, float* chip, 
	     int narray, int ndim, double trans[12], 
	     float centrx, float centry, float rmag[2], 
	     int* chop, float pcl, float pcs, int zerothr)
{
  /* the line,samp calcs are based on a .5 centered pixel, however,
     the main program also passes a .5 centered request.  These cancel
     and the pixels are centered like VICAR pixels.  Since both arrays
     are offset the same amount, there is no error introduced - alz */
   
  /* get the file dimensions */
 
  if (narray%2 == 1 || ndim%2 == 1) 
    return;
  //zmabend((char*)"getgrid error");
  int lnl = img.number_line();
  int lns = img.number_sample();
   
  /* generate the grid */

  int ndim2 = ndim*ndim;
  
  float gridx[ndim2];
  float gridy[ndim2];

  float xmin = 99999999.0; 
  float xmax = -99999999.0;
  float ymin = 99999999.0; 
  float ymax = -99999999.0;
 
  for (int i = 0; i < ndim; i++) {
    for (int j = 0; j < ndim; j++) {
      float px = centrx+rmag[0]*((float)(i-ndim/2)+1.0)+pcl;
      float py = centry+rmag[1]*((float)(j-ndim/2)+1.0)+pcs;

      gridx[i*ndim+j] = trans[0]*px+trans[1]*py+trans[2]+trans[3]*px*px+
	trans[4]*py*py+trans[5]*px*py;
      gridy[i*ndim+j] = trans[6]*px+trans[7]*py+trans[8]+trans[9]*px*px+
	trans[10]*py*py+trans[11]*px*py;
      xmin = MIN(xmin,gridx[i*ndim+j]);
      ymin = MIN(ymin,gridy[i*ndim+j]);
      xmax = MAX(xmax,gridx[i*ndim+j]);
      ymax = MAX(ymax,gridy[i*ndim+j]);
    }
  }
 
  /* malloc an input rectangle as bounding box, don't have to go outside image */

  int slbx = MAX(0,MIN(lnl-2,(int)(xmin-1.0)));
  int ssbx = MAX(0,MIN(lns-2,(int)(ymin-1.0)));
  int elbx = MAX(1,MIN(lnl-1,(int)(xmax+1.0)));
  int esbx = MAX(1,MIN(lns-1,(int)(ymax+1.0)));
  int nlbx = elbx-slbx+1;
  int nsbx = esbx-ssbx+1;

  /* read the image into the bounding box, outside immaterial */
  GeoCal::GeoCalCore::Array2d<int> subGrid;
  img.subset(slbx+1,ssbx,nlbx,nsbx,subGrid);
 
  /* now look up the image values, outside box is intercepted */

  *chop = 0;
  float fzerothr = (float)zerothr+0.01;
  for (int i = 0; i < ndim2; i++) {
    float rline = gridx[i]-0.5;
    float rsamp = gridy[i]-0.5;
    int iline = (int) (rline-0.5);
    int isamp = (int) (rsamp-0.5);
    if (iline < 0 || iline >= lnl-1 || isamp < 0 || isamp >= lns-1) { 
      chip[i] = 0.; 
      (*chop)++; 
    }
    else {
      double dvecl[2] = {0,0};
      double dvecu[2] = {0,0};

      if (subGrid.number_row() > iline-slbx+1 && 
	  subGrid.number_col() > isamp-ssbx+1) {
	dvecl[0] = (double) (subGrid(iline-slbx,isamp-ssbx));
	dvecl[1] = (double) (subGrid(iline-slbx,isamp-ssbx+1));
	dvecu[0] = (double) (subGrid(iline-slbx+1,isamp-ssbx));
	dvecu[1] = (double) (subGrid(iline-slbx+1,isamp-ssbx+1));
      }

      float rlfac = rline-iline-0.5;
      float rsfac = rsamp-isamp-0.5;
      float vl = (1.0-rsfac)*dvecl[0]+rsfac*dvecl[1];
      float vu = (1.0-rsfac)*dvecu[0]+rsfac*dvecu[1];
      chip[i] = (1.0-rlfac)*vl+rlfac*vu;
	 
      if (chip[i] < fzerothr) 
	(*chop)++;
    }
  }

  /* free storage */
   
  /* now center in the grid, save the prints below for getgrid verif */

  /*printf("new getgrid: print raw center:\n");
    int q = (narray/2-6)*narray+narray/2-6;
    for (int k=0;k<11;k++)
    {
    for (int j=0;j<11;j++) printf("%3.5f ",chip[q+j]);
    q += narray;
    printf("\n");
    }*/

  if (narray == ndim) 
    return;

  int bord = (narray-ndim)/2;
  for (int i = ndim-1; i >= 0; i--) {

    for (int j = 0; j < ndim; j++)
      chip[(i+bord)*narray+bord+j] = chip[i*ndim+j];
  }

  return;
}

//***************************************************************************
// void imageCoorToPoint(
// GeoCal::GeoCalCore::ImageCoor& coor, // input to convert
// cv::Point2f& pt) // converted point ouput
//***************************************************************************
void imageCoorToPoint(GeoCal::GeoCalCore::ImageCoor& coor, cv::Point2f& pt)
{
  pt = cv::Point2f(coor.line(),coor.sample());
}

//***************************************************************************
// void imageCoorToPoint(
// std::vector<GeoCal::GeoCalCore::ImageCoor>& coors, // input to convert
// std::vector<cv::Point2f>& pts) // converted points ouput
//***************************************************************************
void imageCoorToPoint(std::vector<GeoCal::GeoCalCore::ImageCoor>& coors,
		      std::vector<cv::Point2f>& pts)
{
  for (int i = 0; i < coors.size(); i++) {
    cv::Point2f point;
    imageCoorToPoint(coors[i],point);
    pts.push_back(point);
  }
}

//***************************************************************************
// bool makeGrid(
// GeoCal::GeoCalCore::RasterImage& img, //image to fit grid over
// int grid, //power of 2 to create ex: grid = 4, creates 16 cells
// std::vector<GeoCal::GeoCalCore::ImageCoor>& center) // center of each grid
//***************************************************************************
bool makeGrid(GeoCal::GeoCalCore::RasterImage& img, int grid, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& center)
{
  int row = 1;
  int col = 1;
  bool lastWasLine = true;
  bool found = false;

  grid = pow(2,grid);
  while (row*col < grid) {
    if (lastWasLine)
      col *= 2;
    else
      row *= 2;

    lastWasLine = !lastWasLine;
    if (grid == (row*col)) {
      found = true;
      break;
    }
  }

  if (!found)
    return false;


  int maxRow = img.number_line();
  int maxCol = img.number_sample();
  int lInc = maxRow/row;
  int sInc = maxCol/col;
  int line = lInc/2-1;
  int sample = sInc/2-1;
  while (line < maxRow) {
    int s_temp = sample;
    while (sample < maxCol) {
      center.push_back(GeoCal::GeoCalCore::ImageCoor(line, sample));
      sample += sInc;
    }
    sample = s_temp;
    line += lInc;
  }

  return true;
}

//***************************************************************************
// void matToRast(
// cv::Mat& src, // source to convert
// GeoCal::GeoCalCore::RasterImage& dst) // allocated destination buffer
//***************************************************************************
void matToRast(cv::Mat& src, GeoCal::GeoCalCore::RasterImage& dst)
{
  for(int i = 0; i < src.rows; i++) {
    uchar* rowPtr = src.ptr(i);

    for(int j = 0; j < src.cols; j++) {
      dst.write(i,j,rowPtr[j]);
    }
  }
}

//***************************************************************************
// int GeoCal::GeoCalCore::RasterImage::operator()(
// int Line, // line coor of pixel
// int Sample // sample coor of pixel
// ) const.
//***************************************************************************
cv::Mat rastToMat(GeoCal::GeoCalCore::RasterImage& src)
{
  cv::Size size = cv::Size(src.number_line(),src.number_sample());
  cv::Mat out(size,CV_32SC1);
  for(int i = 0; i < out.rows; i++) {
    uchar* rowPtr = out.ptr(i);

    for(int j = 0; j < out.cols; j++) {
      rowPtr[j] = src(i,j);
    }
  }

  return out;
}

//***************************************************************************
// cv::Mat subImage(
// cv::Mat& img, // image to extract subImage from
// cv::Point2f pt, // upper left corner to extract
// cv::Size size) // width and height of rectangle to extract
//***************************************************************************
cv::Mat subImage(cv::Mat& img, cv::Point2f pt, cv::Size size)
{
  cv::Rect rect = cv::Rect(pt, size);
  cv::Mat sub;
  sub.create(size,img.type());
  IplImage imgIpl = img;
  IplImage subIpl = sub;
  cvSetImageROI(&imgIpl, rect);
  cvCopyImage(&imgIpl, &subIpl);
  cvResetImageROI(&imgIpl);
  return sub;
}
