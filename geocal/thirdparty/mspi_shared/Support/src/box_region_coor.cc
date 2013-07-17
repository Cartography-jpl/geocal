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

   This contains support routines for the class BoxRegionCoor 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "box_region_coor.h"	// Definition of class.
#include <algorithm> 		// Definition of std::max.

//***************************************************************************
// Definition of function
// BoxRegionCoor::BoxRegionCoor(double X1, double Y1, double X2,
// double Y2)
//***************************************************************************

MSPI::Shared::BoxRegionCoor::BoxRegionCoor(
double X1, 
double Y1, 
double X2, 
double Y2
)
  : min_x_(X1 < X2 ? X1 : X2),
    min_y_(Y1 < Y2 ? Y1 : Y2),
    max_x_(X1 < X2 ? X2 : X1),
    max_y_(Y1 < Y2 ? Y2 : Y1)
{
}

//***************************************************************************
// Definition of function
// BoxRegionCoor BoxRegionCoor::intersect(const BoxRegionCoor& Box)
//***************************************************************************

MSPI::Shared::BoxRegionCoor MSPI::Shared::BoxRegionCoor::intersect(
const MSPI::Shared::BoxRegionCoor& Box
) const
{
  double res_min_x = std::max(min_x_, Box.min_x());
  double res_min_y = std::max(min_y_, Box.min_y());
  double res_max_x = std::min(max_x_, Box.max_x());
  double res_max_y = std::min(max_y_, Box.max_y());

  if (res_min_x >= res_max_x ||
      res_min_y >= res_max_y) {
    res_min_x = 0.0;
    res_min_y = 0.0;
    res_max_x = 0.0;
    res_max_y = 0.0;
  }

  return BoxRegionCoor(res_min_x, res_min_y, res_max_x, res_max_y);
}

