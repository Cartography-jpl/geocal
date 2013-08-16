/*===========================================================================
=                                                                           =
=                              DoubleComparator                              =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MISR
                                   DoubleComparator

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: DoubleComparator

!ABSTRACT:

   This contains support routines for the class DoubleComparator

!PARENT(S):

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "double_comparator.h"	// Definition of class.
#include <iomanip> 		// Definition of std::setprecision
#include <cmath> 		// Definition of std::fabs
#include <limits> 		// Definition of std::numeric_limits

/////////////////////////////////////////////////////////////////////////////
/// Construct double-precision float comparison tool with the given optional 
/// precision factors.  Default Relative_factor is 1. 
/// Default Absolute_factor is 0.  
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::DoubleComparator::DoubleComparator(
  double Relative_factor,
  double Absolute_factor
) 
  : afactor_(Absolute_factor),
    rfactor_(Relative_factor)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Compare two single-precision doubles.  If they are not equal, report
/// differences to the optional output stream and return true.  
/// Otherwise return false.  Default output stream is std::cerr.
///
/// NaN (not-a-number) for either or both inputs always returns true (not equal) 
/// +/-Inf (infinity) for either or both inputs always returns true (not equal) 
/////////////////////////////////////////////////////////////////////////////

bool 
MSPI::Shared::DoubleComparator::neq(double X, double Y, std::ostream& Out) const
{
  double alimit = std::numeric_limits<double>::epsilon() * afactor_;
  double rlimit = std::numeric_limits<double>::epsilon() * rfactor_ * std::fabs(X);
  double diff = std::fabs(X-Y);
  bool success = (diff <= std::max(alimit, rlimit));

  if (!success) {
    if (alimit > rlimit) {
      Out << std::setprecision(20) << std::scientific;
      Out << "exceeded alimit: abs(x-y)=" << diff 
	  << "  limit=" << alimit << "\n";
    } else {
      Out << std::setprecision(20) << std::scientific;
      Out << "exceeded rlimit: abs(x-y)=" << diff 
	  << "  limit=" << rlimit << "\n";
    }
  }
  return !success;
};
