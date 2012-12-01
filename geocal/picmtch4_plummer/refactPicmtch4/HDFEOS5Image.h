/*===========================================================================
=                                                                           =
=                             HDFEOS5Image                                  =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: HDFEOS5Image

!ABSTRACT:

   This is an interface to the HDF-EOS5 Image file that stores one of its
   datafields as a 2d array.

!DERIVED FROM:


!DESCRIPTION:


!PUBLIC MEMBERS:

    HDFEOS5Image(const std::string& filename, 
                 const std::string& grid, 
		 const std::string& datafield)
     - Create an object that reads in data from a given file and stores the data.

   inline virtual ~HDFEOS5Image()
     - Destructor.

   int operator()(int Line, int Sample) const
     - Implementation for the () operator returning the value at the given
       coordinate.

   void subset(int Lstart,int Sstart,
               int Number_line,int Number_sample, 
	       GeoCal::GeoCalCore::Array2d<int>& Res
	       ) const
     - Outputs the requested subimage

   void write(int Line, int Sample, int Val)
     - Write data to a given location

!PROTECTED MEMBERS:


!PRIVATE MEMBERS:

   GeoCal::GeoCalCore::Array2d<int> data_
     - Stored image data read in and scaled from the HDF-EOS 5 file

!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef _HDFEOS5IMAGE_H
#define _HDFEOS5IMAGE_H

#include <string.h>
#include "raster_image.h"
#include "image_coor.h"
#include "array2d.h"
#include <limits>

#include <HE5_HdfEosDef.h>

//***************************************************************************
// Declaration of class HDFEOS5Image.
//***************************************************************************

class HDFEOS5Image: public GeoCal::GeoCalCore::RasterImage
{
 public:
  HDFEOS5Image(const std::string& filename, 
	       const std::string& grid, 
	       const std::string& datafield);
  HDFEOS5Image(int Number_line, int Number_sample);
  int operator()(int Line, int Sample) const;
  void subset(int Lstart,int Sstart,int Number_line,int Number_sample, 
	      GeoCal::GeoCalCore::Array2d<int>& Res) const;
  void write(int Line, int Sample, int Val);
  ~HDFEOS5Image();
 private:
  GeoCal::GeoCalCore::Array2d<int> data_;
};

#endif
