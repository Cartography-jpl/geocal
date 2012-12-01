/*===========================================================================
=                                                                           =
=                                 ImageCoor                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MISR
                             GeoCal Processing
                                                                        
            Copyright 2006, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

!C++=========================================================================

!CLASS NAME: ImageCoor

!ABSTRACT:

   This is a location in an image. It is little more than a structure.


!DERIVED FROM:


!DESCRIPTION:

   This is basically just a structure. In addition to the class, we
   also define an ostream operator<<, operator== and operator!=.
   Two image coordinates are considered equal by operator== if they
   line and sample are with .01 pixels of each other.

!PUBLIC MEMBERS:

   inline ImageCoor(double Line, double Sample)
     - Constructor.

   inline ImageCoor()
     - Default constructor.

   inline double line() const
     - Return line number of coordinate.

   inline double sample() const
     - Return sample number of coordinate.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   double line_
     - This is the line number.

   double sample_
     - This is the sample number.

!ASSOCIATED FUNCTIONS:

  std::ostream& operator<<(std::ostream& Os, const ImageCoor& Coor)
    - Write to a stream.

  bool operator==(const ImageCoor& C1, const ImageCoor& C2)
    - Compare. Two coordinates are the same if they are within 0.01 of
      each other.
  
  inline bool operator!=(const ImageCoor& C1, const ImageCoor& C2)
    - Compare. Two coordinates are the same if they are within 0.01 of
      each other.

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef IMAGE_COOR_H
#define IMAGE_COOR_H
#include <iostream>		// Definition of ostream.


namespace GeoCal {
  namespace GeoCalCore {
    class ImageCoor;
  }
}

//***************************************************************************
// Declaration of class GeoCal::GeoCalCore::ImageCoor.
//***************************************************************************

class GeoCal::GeoCalCore::ImageCoor {
public:
  inline           ImageCoor(double Line, double Sample);
  inline           ImageCoor();
  inline double    line() const;
  inline double    sample() const;
protected:
private:
  double           line_;
  double           sample_;
};

//***************************************************************************
// Free functions related to class.
//***************************************************************************

namespace GeoCal {
  namespace GeoCalCore {
    std::ostream& operator<<(std::ostream& Os, const ImageCoor& Coor);
    bool operator==(const ImageCoor& C1, const ImageCoor& C2);
    inline bool operator!=(const ImageCoor& C1, const ImageCoor& C2);
  }
}
//***************************************************************************
// Include source of inline functions.
//***************************************************************************

#include "image_coor.icc"
#endif

