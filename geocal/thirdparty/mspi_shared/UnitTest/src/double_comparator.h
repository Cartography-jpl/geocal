/*===========================================================================
=                                                                           =
=                                 DoubleComparator                           =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                  Shared

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: DoubleComparator

=============================================================================
!END=======================================================================*/

#ifndef DOUBLE_COMPARATOR_H
#define DOUBLE_COMPARATOR_H

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include <iostream>		// Definition of std::ostream

namespace MSPI {
  namespace Shared {
    class DoubleComparator;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Double-precision float comparison tool.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::DoubleComparator {
public:
  DoubleComparator(double Relative_factor = 1.0, double Absolute_factor = 0.0);
  bool neq(double X, double Y, std::ostream& Output_stream = std::cerr) const;
protected:
private:
 double afactor_;
 double rfactor_;
};

#endif
