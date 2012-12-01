/*===========================================================================
=                                                                           =
=                                range_check.h                              =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MISR
                              GeoCal Processing
                                                                        
            Copyright 2006, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

!C++-INC=====================================================================

!MODULE NAME: range_check.h

!DESCRIPTION: 

   This defines the template range_check_template, which does range
   checking, and a macro range_check that supplies some of the
   information range_check_template needs. Calling protocol:
   range_check(val, min, max). An error is generated if val is not in
   the range [min, max). Eventually, this error will be a thrown
   exception. For now, an error message is printed and the program
   terminates.

   range_min_check is just like range_check, except that there is no
   maximum value. Similarly, range_max_check has no minimum value.

!DEPENDENCIES:

   code_location.h

!REVISION HISTORY:

  See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef RANGE_CHECK_H
#define RANGE_CHECK_H
#include "code_location.h"
                                // Definition of class CodeLocation
#include "range_exception.h"
				// Definition of RangeException. 

namespace GeoCal {
  namespace GeoCalCore {
template <class T> inline void range_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Min,		// Minimum allowed value.
const T&	   Max,		// Maximum allowed value.
const CodeLocation&
		   Cl		// Location where range_check_template
				// was called.
)
{
  if(Val < Min ||
     !(Val < Max)) {
    RangeException e;
    e << "Out of range error in " << Cl << "\n"
      << "Value:           " << Val << "\n"
      << "Minimum allowed: " << Min << "\n"
      << "Maximum allowed: " << Max;
    throw e;
  }
}

template <class T> inline void range_inclusive_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Min,		// Minimum allowed value.
const T&	   Max,		// Maximum allowed value.
const CodeLocation&
		   Cl		// Location where range_check_template
				// was called.
)
{
  if(Val < Min ||
     (Val > Max)) {
    RangeException e;
    e << "Out of range error in " << Cl << "\n"
      << "Value:           " << Val << "\n"
      << "Minimum allowed: " << Min << "\n"
      << "Maximum allowed: " << Max;
    throw e;
  }
}

template <class T> inline void range_min_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Min,		// Minimum allowed value.
const CodeLocation&
		   Cl		// Location where range_check_template
				// was called.
)
{
  if(Val < Min) {
    RangeException e;
    e << "Out of range error in " << Cl << "\n"
      << "Value:           " << Val << "\n"
      << "Minimum allowed: " << Min;
    throw e;
  }
}

template <class T> inline void range_max_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Max,		// Maximum allowed value.
const CodeLocation&
		   Cl		// Location where range_check_template
				// was called.
)
{
  if(!(Val < Max)) {
    RangeException e;
    e << "Out of range error in " << Cl << "\n"
      << "Value:           " << Val << "\n"
      << "Maximum allowed: " << Max;
    throw e;
  }
}
}
}

#define range_check(V, Min, Max) \
      GeoCal::GeoCalCore::range_check_template(V, Min, Max, CODE_LOCATION)

#define range_inclusive_check(V, Min, Max) \
      GeoCal::GeoCalCore::range_inclusive_check_template(V, Min, Max, CODE_LOCATION)

#define range_min_check(V, Min) \
      GeoCal::GeoCalCore::range_min_check_template(V, Min, CODE_LOCATION)

#define range_max_check(V, Max) \
      GeoCal::GeoCalCore::range_max_check_template(V, Max, CODE_LOCATION)

#endif
