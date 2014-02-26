/*===========================================================================
=                                                                           =
=                                RasterImage                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MISR
                             GeoCal Processing
                                                                        
            Copyright 2006, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

!C++=========================================================================

!CLASS NAME: RasterImage

!ABSTRACT:

   This is a general image class. Conceptually, an image is just a two
   dimensional array of integers, although it is not necessarily implemented
   this way. This is an abstract class, containing behavior common to all
   images.

!DERIVED FROM:


!DESCRIPTION:

   This is an abstract base class that captures the behavious common
   to all images. It handles keeping track of the size of the image,
   but all other behaviour needs to be defined by subclasses.

   It is common for Raster data to be stored on disk as a series of
   tiles. It can be significantly more efficient to access and write 
   the data in tiles. To support this, we provide a "preferred" tile
   size given by number_tile_line() and number_tile_sample(). Classes
   using a RasterImage are free to ignore this tiling - data can be
   read and written in any order desired, and the only impact of tiles
   is in the performance.

   The default tile size is the entire image, derived classes can
   supply other values if desired.

!PUBLIC MEMBERS:

   inline RasterImage(int Number_line, int Number_sample)
     - Constructor for a given sized image.

   inline RasterImage()
     - Constructor.

   inline virtual ~RasterImage()
     - Destructor.

   inline double interpolate(double Line, double Sample) const
     - This does a bilinear interpolation of the data for fractional
       Line and Sample.

     REQUIRE: 0 <= Line < number_line() - 1
     REQUIRE: 0 <= Sample < number_sample() - 1
     PROMISE: Nothing

   inline double interpolate(const ImageCoor& Ic) const
     - This does a bilinear interpolation of the data.

     REQUIRE: 0 <= Ic.line() < number_line() - 1
     REQUIRE: 0 <= Ic.sample() < number_sample() - 1
     PROMISE: Nothing

   inline int number_line() const
     - Return number of lines in the image.

   inline int number_sample() const
     - Return number of samples in image.

   virtual inline int number_tile_line() const
     - Number of lines in the preferred tile size. Default is the
       entire image.

   virtual inline int number_tile_sample() const
     - Number of lines in the preferred tile size. Default is the
       entire image.

   virtual int operator()(int Line, int Sample) const = 0
     - Return pixel value at given line and sample.

   virtual void subset(int Lstart, int Sstart, int Number_line, int
     Number_sample, Array2d<int>& Res) const = 0
     - Return a subset of the image.

   virtual void write(int Line, int Sample, int Val) = 0
     - Write the pixel value to the given location.

!PROTECTED MEMBERS:

   int number_line_
     - Number of lines in image. Can be written to be derived classes.

   int number_sample_
     - Number of samples in image. Can be written to by derived classes.

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef RASTER_IMAGE_H
#define RASTER_IMAGE_H
#include "array2d.h"
                                // Definition of Array2d.
#include "image_coor.h"
				// Definition of ImageCoor.

namespace GeoCal {
  namespace GeoCalCore {
    class RasterImage;
  }
}

//***************************************************************************
// Declaration of class GeoCal::GeoCalCore::RasterImage.
//***************************************************************************

class GeoCal::GeoCalCore::RasterImage {
public:
  inline           RasterImage(int Number_line, int Number_sample);
  inline           RasterImage();
  inline virtual   ~RasterImage();
  inline double	   interpolate(double Line, double Sample) const;
  inline double	   interpolate(const ImageCoor& Ic) const;
  inline int       number_line() const;
  inline int       number_sample() const;
  virtual inline int 
		   number_tile_line() const;
  virtual inline int 
		   number_tile_sample() const;
  virtual int      operator()(int Line, int Sample) const = 0;
  virtual void     subset(int Lstart, int Sstart, int Number_line, int
                      Number_sample, Array2d<int>& Res) const = 0;
  virtual void     write(int Line, int Sample, int Val) = 0;
protected:
  int              number_line_;
  int              number_sample_;
private:
};

//***************************************************************************
// Include source of inline functions.
//***************************************************************************

#include "raster_image.icc"
#endif

