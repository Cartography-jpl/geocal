/*===========================================================================
=                                                                           =
=                             HDFEOS5Image                                  =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: HDFEOS5Image

!ABSTRACT:

   This contains routines for the class HDFEOS5Image.  Assumes data
   is of type float.

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#include "HDFEOS5Image.h"

//***************************************************************************
// HDFEOS5Image::HDFEOS5Image(
// const std::string& filename,  //Name of file to open
// const std::string& grid,  //Grid object to read from
// const std::string& datafield)  //Field to obtain data
//***************************************************************************
HDFEOS5Image::HDFEOS5Image(const std::string& filename, 
			   const std::string& grid,  
			   const std::string& datafield)
{
  hid_t gridfileid;
  gridfileid = HE5_GDopen(filename.c_str(),H5F_ACC_RDONLY);

  hid_t gridid;
  gridid = HE5_GDattach(gridfileid,grid.c_str());

  long lines,samples;
  HE5_GDgridinfo(gridid,&lines,&samples,NULL,NULL);

  if (lines > INT_MAX || samples > INT_MAX)
    printf("HDFEOS5Image: File Truncated\n");

  number_line_ = (int)lines;
  number_sample_ = (int)samples;

  //------------------------------------------------------------------------
  // Read in the data from file, must be allocated on heap for larger images
  // or it can blow the stack even though is slower.
  //------------------------------------------------------------------------
  float** rawData = new float*[number_line()];
  for (int i = 0; i < number_line(); i++) {
    hssize_t start[2] = {i,0};
    hsize_t edge[2] = {1,number_sample()};

    rawData[i] = new float[number_sample()];
    HE5_GDreadfield(gridid,datafield.c_str(),start,NULL,edge,rawData[i]);
  }

  HE5_GDdetach(gridid);
  HE5_GDclose(gridfileid);

  data_.resize(lines,samples);
 
  //------------------------------------------------------------------------
  // Find the mininum and maximum values in the input.
  //------------------------------------------------------------------------
  double min = 0;
  double max = 0;
  bool first = true;
  for (int iline = 0 ; iline < number_line() ; iline++) {
    for (int isample = 0 ; isample < number_sample() ; isample++) {
      float value = rawData[iline][isample];
      if (first || value < min) {
	min = value;
	first = false;
      }
      if (first || value > max) {
	max = value;
	first = false;
      }
    }
  }

  //------------------------------------------------------------------------
  // Scale image data to integer range [0:MAX_INT]
  //------------------------------------------------------------------------

  double slope = std::numeric_limits<short int>::max() / (max - min);
  double intercept = -slope * min;
  for (int iline = 0 ; iline < number_line() ; iline++) {
    for (int isample = 0 ; isample < number_sample() ; isample++) {
      data_(iline,isample) = (int)floor(slope * rawData[iline][isample] + intercept);
    }
  }

  for (int i = 0; i < number_line(); i++)
    delete[] rawData[i];
  delete[] rawData;
}

//***************************************************************************
// HDFEOS5Image::HDFEOS5Image(
// int Number_line, // Number of lines in image data
// int Number_sample)  // Number of samples in image data
//***************************************************************************
HDFEOS5Image::HDFEOS5Image(int Number_line, int Number_sample) 
{
  number_line_ = (int)Number_line;
  number_sample_ = (int)Number_sample;
  data_.resize((long)number_line_,(long)number_sample_);
}

//***************************************************************************
// int HDFEOS5Image::operator()(
// int Line, // line coor of pixel
// int Sample // sample coor of pixel
// ) const.
//***************************************************************************  
int HDFEOS5Image::operator()(int Line, int Sample) const
{ 
  return data_(Line,Sample);
}

//***************************************************************************
// void HDFEOS5Image::subset(
//  int Lstart, // line coor for start
//  int Sstart, // sample coor for start
//  int Number_line,  // number of lines to extract
//  int Number_sample, // number of samples to extract
//  GeoCal::GeoCalCore::Array2d<int>& Res // output subimage
//  ) const
//***************************************************************************
void HDFEOS5Image::subset(int Lstart, int Sstart, int Number_line,
			  int Number_sample, 
			  GeoCal::GeoCalCore::Array2d<int>& Res) const 
{
  int nLine = MIN(Number_line,number_line_-Lstart);
  int nSample = MIN(Number_sample,number_sample_-Sstart);
  data_.subset(Lstart,Sstart,nLine,nSample,Res);
}

//***************************************************************************
// void HDFEOS5Image::write(
// int Line, // line coor to write
// int Sample, // sample coor to write
// int Val) // value to write
//***************************************************************************
void HDFEOS5Image::write(int Line, int Sample, int Val) 
{
  data_(Line,Sample) = Val;
}

//***************************************************************************
// HDFEOS5Image::~HDFEOS5Image() Destructor
//***************************************************************************  
HDFEOS5Image::~HDFEOS5Image() 
{
  // do nothing
}
