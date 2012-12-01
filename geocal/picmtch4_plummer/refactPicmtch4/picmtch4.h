/*===========================================================================
=                                                                           =
=                             picmtch4                                      =
=                                                                           =
=============================================================================

!C++=========================================================================

!CLASS NAME: 

!ABSTRACT:

   Support functions used by MspiPCorrMatcher.

!DERIVED FROM:

!DESCRIPTION:

!PUBLIC MEMBERS:

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:
   
!ASSOCIATED FUNCTIONS:

   void calcLeastSquaresFit
   (double soln[12], int predfunc,int nEquationsMax, 
   std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
   std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors)
     - ouputs a least squares fit in soln between two sets of points

   void initialMatch(double New_guess[12],  
   GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3],
   GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3],
   int nEquationsMax, bool predfunc,
   HDFEOS5Image* New, double angoff)
     - creates an initial transformation between two triangles

   void refine(float corr[3][3], float* vloff,float* vsoff, bool* ireferr)
     - refines a match produced by MspiPcorrMatcher's rfit routine

   void throwout(int* throwcount, int nEquationsMax, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors)
     - throws out the worst point in the ImageCoor vectors using linear fit

   void tiltCoor(HDFEOS5Image* Img, GeoCal::GeoCalCore::ImageCoor TiePoints[3], 
	      double angoff)
     - rotates a triangle according to angoff

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

=============================================================================
!END=======================================================================*/

#ifndef _PICMTCH4_H
#define _PICMTCH4_H

#include <string.h>
#include "raster_image.h"
#include "image_coor.h"
#include "array2d.h"

#include <HE5_HdfEosDef.h>
#include "HDFEOS5Image.h"
#include <vector>

//***************************************************************************
// Declaration of class GeoCal::GeoCalCore::RasterImage.
//***************************************************************************

void calcLeastSquaresFit
(double soln[12],  bool predfunc, int nEquationsMax, 
 std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
 std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors);

void getgrid(GeoCal::GeoCalCore::RasterImage& img, float* chip, 
	     int narray, int ndim, double trans[12], 
	     float centrx, float centry, float rmag[2], 
	     int* chop, float pcl, float pcs, int zerothr);

void initialMatch(double New_guess[12],  
		  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3],
		  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3], 
		  int nEquationsMax, bool predfunc = false,
		  GeoCal::GeoCalCore::RasterImage* New = NULL, double angoff = 0);

void refine(float corr[3][3], float* vloff,float* vsoff, bool* ireferr);

void throwout(int* throwcount, int nEquationsMax, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& fstImgCoors, 
	      std::vector<GeoCal::GeoCalCore::ImageCoor>& sndImgCoors);

void tiltCoor(GeoCal::GeoCalCore::RasterImage* Img, 
	      GeoCal::GeoCalCore::ImageCoor TiePoints[3], double angoff);
#endif
