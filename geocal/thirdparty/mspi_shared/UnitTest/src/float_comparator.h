/*===========================================================================
=                                                                           =
=                                 FloatComparator                           =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                  Shared

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: FloatComparator

=============================================================================
!END=======================================================================*/

#ifndef FLOAT_COMPARATOR_H
#define FLOAT_COMPARATOR_H

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include <iostream>		// Definition of std::ostream

namespace MSPI {
  namespace Shared {
    class FloatComparator;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Single-precision float comparison tool.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::FloatComparator {
public:
  FloatComparator(float Relative_factor = 1.0, float Absolute_factor = 0.0);
  bool neq(float X, float Y, std::ostream& Output_stream = std::cerr) const;
protected:
private:
 float afactor_;
 float rfactor_;
};

#endif
