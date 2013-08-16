/*===========================================================================
=                                                                           =
=                               BoxRegionCoor                               =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                     Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: BoxRegionCoor

!ABSTRACT:

   This class defines a rectangular region in X,Y coordinates, and includes
   some simple operations on regions.

!DERIVED FROM:

!DESCRIPTION:

!PUBLIC MEMBERS:

   BoxRegionCoor(double X1, double Y1, double X2, double Y2)
     - Constructor.   The coordinates X1,Y1 and X2,Y2 define diagonally
       opposing corners of a rectangular region. 

   ~BoxRegionCoor()
     - Destructor.

   double min_x() const
     - Minimum X coordinate of region (or 0 if empty).

   double min_y() const
     - Minimum Y coordinate of region (or 0 if empty).

   double max_x() const
     - Maximum X coordinate of region (or 0 if empty).

   double max_y() const
     - Maximum Y coordinate of region (or 0 if empty).

   BoxRegionCoor intersect(const BoxRegionCoor& Box) const
     - Calculate and return the intersection of the current region with a
       given region.  If the result is empty, then the returned region will
       have all coordinates set to zero. 

   bool point_intersect(const double& X, const double& Y) const
     - Return true if the given point is inside the current region.
       Otherwise return false.
   
   bool operator==(const BoxRegionCoor &Box) const;
     - Compare regions for equality.

   bool operator!=(const BoxRegionCoor &Box) const;
     - Compare regions for inequality.

   bool is_empty(const BoxRegionCoor &Box) const;
     - Returns true if the region coordinates are all zero.  Otherwise
       returns false.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   double min_x_
     - Minimum X coordinate of region.

   double min_y_
     - Minimum Y coordinate of region.

   double max_x_
     - Maximum X coordinate of region.

   double max_y_
     - Maximum Y coordinate of region.

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef BOX_REGION_COOR_H
#define BOX_REGION_COOR_H

#include <cmath> 		// Definition of fabs
#include <limits>  		// Definition of epsilon

namespace MSPI {
  namespace Shared {
    class BoxRegionCoor;
  }
}

//***************************************************************************
// Declaration of class BoxRegionCoor.
//***************************************************************************

class MSPI::Shared::BoxRegionCoor {
public:
  BoxRegionCoor();
  BoxRegionCoor(double X1, double Y1, double X2, double Y2);
  
  inline double min_x() const;
  inline double min_y() const;
  inline double max_x() const;
  inline double max_y() const;

  BoxRegionCoor intersect(const BoxRegionCoor& Box) const;
  inline bool point_intersect(const double& X, const double& Y) const;

  inline bool operator==(const BoxRegionCoor& Box) const;
  inline bool operator!=(const BoxRegionCoor& Box) const;

  inline bool is_empty() const;

protected:

private:
  inline bool isEqual(const double& X, const double& Y) const;
  double min_x_;
  double min_y_;
  double max_x_;
  double max_y_;
};

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline 
MSPI::Shared::BoxRegionCoor::BoxRegionCoor()
  : min_x_(0),
    min_y_(0),
    max_x_(0),
    max_y_(0)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return minimum x coordinate of region.
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::BoxRegionCoor::min_x() const
{
  return min_x_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return minimum y coordinate of region.
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::BoxRegionCoor::min_y() const
{
  return min_y_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return maximum x coordinate of region.
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::BoxRegionCoor::max_x() const
{
  return max_x_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return maximum y coordinate of region.
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::BoxRegionCoor::max_y() const
{
  return max_y_;
}

/////////////////////////////////////////////////////////////////////////////
/// Compare two regions for equality.
/////////////////////////////////////////////////////////////////////////////

inline bool MSPI::Shared::BoxRegionCoor::operator==(const BoxRegionCoor& Box) const
{
  return (isEqual(min_x_, Box.min_x()) && isEqual(min_y_, Box.min_y()) &&
	  isEqual(max_x_, Box.max_x()) && isEqual(max_y_, Box.max_y()));
}

/////////////////////////////////////////////////////////////////////////////
/// Compare two regions for inequality.
/////////////////////////////////////////////////////////////////////////////

inline bool MSPI::Shared::BoxRegionCoor::operator!=(const BoxRegionCoor& Box) const
{
  return !(*this == Box);
}

/////////////////////////////////////////////////////////////////////////////
/// Compare two floating-point numbers for equality.
/////////////////////////////////////////////////////////////////////////////

inline bool MSPI::Shared::BoxRegionCoor::isEqual(const double &X, const double& Y) const
{
  return std::fabs(X-Y) <= 10 * std::numeric_limits<double>::epsilon() * std::fabs(X);
}

/////////////////////////////////////////////////////////////////////////////
/// Return true if the region is empty.  Otherwise return false.
/////////////////////////////////////////////////////////////////////////////

inline bool MSPI::Shared::BoxRegionCoor::is_empty() const
{
  return (min_x_ == 0 && max_x_ == 0 && min_y_ == 0 && max_y_ == 0);
}


/////////////////////////////////////////////////////////////////////////////
/// Return true if the given point intersects the region.
/////////////////////////////////////////////////////////////////////////////

inline bool 
MSPI::Shared::BoxRegionCoor::point_intersect(
const double& X, const double& Y
) const
{
  return !((X < min_x_ || X > max_x_ || Y < min_y_ || Y > max_y_));
}

#endif
