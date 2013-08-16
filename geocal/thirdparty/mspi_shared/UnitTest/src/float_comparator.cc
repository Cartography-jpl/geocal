/*===========================================================================
=                                                                           =
=                              FloatComparator                              =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MISR
                                   FloatComparator

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: FloatComparator

!ABSTRACT:

   This contains support routines for the class FloatComparator

!PARENT(S):

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "float_comparator.h"	// Definition of class.
#include <iomanip> 		// Definition of std::setprecision
#include <cmath> 		// Definition of std::fabs
#include <limits> 		// Definition of std::numeric_limits

/////////////////////////////////////////////////////////////////////////////
/// Construct single-precision float comparison tool with the given optional 
/// precision factors.  Default Relative_factor is 1. 
/// Default Absolute_factor is 0.  
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::FloatComparator::FloatComparator(
  float Relative_factor,
  float Absolute_factor
) 
  : afactor_(Absolute_factor),
    rfactor_(Relative_factor)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Compare two single-precision floats.  If they are not equal, report
/// differences to the optional output stream and return true.  
/// Otherwise return false.  Default output stream is std::cerr.
///
/// NaN (not-a-number) for either or both inputs always returns true (not equal) 
/// +/-Inf (infinity) for either or both inputs always returns true (not equal) 
/////////////////////////////////////////////////////////////////////////////

bool 
MSPI::Shared::FloatComparator::neq(float X, float Y, std::ostream& Out) const
{
  float alimit = std::numeric_limits<float>::epsilon() * afactor_;
  float rlimit = std::numeric_limits<float>::epsilon() * rfactor_ * std::fabs(X);
  float diff = std::fabs(X-Y);
  bool success = (diff <= std::max(alimit, rlimit));

  if (!success) {
    if (alimit > rlimit) {
      Out << std::setprecision(10) << std::scientific;
      Out << "exceeded alimit: abs(x-y)=" << diff 
	  << "  limit=" << alimit << "\n";
    } else {
      Out << std::setprecision(10) << std::scientific;
      Out << "exceeded rlimit: abs(x-y)=" << diff 
	  << "  limit=" << rlimit << "\n";
    }
  }
  return !success;
};
