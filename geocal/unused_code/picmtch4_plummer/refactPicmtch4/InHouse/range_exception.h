/*===========================================================================
=                                                                           =
=                              RangeException                               =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MISR
                              GeoCal Processing

             Copyright 2002, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: RangeException

!ABSTRACT:

   Used to indicate an out of range condition.

!DERIVED FROM:

   Exception

!DESCRIPTION:

!PUBLIC MEMBERS:

   inline RangeException() throw()
     - Constructor.

     REQUIRE: Nothing
     PROMISE: Nothing

   inline virtual ~RangeException() throw()
     - Destructor.

     REQUIRE: Nothing
     PROMISE: Nothing

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef RANGE_EXCEPTION_H
#define RANGE_EXCEPTION_H
#include "exception.h"
                                // Definition of Exception.
namespace GeoCal {
  namespace GeoCalCore {
    class RangeException;
  }
}

//***************************************************************************
// Declaration of class RangeException.
//***************************************************************************

class GeoCal::GeoCalCore::RangeException: public Exception {
public:
  inline           RangeException() throw();
  inline virtual   ~RangeException() throw();
protected:
private:
};


//***************************************************************************
// Include source of inline functions.
//***************************************************************************

#include "range_exception.icc"
#endif


