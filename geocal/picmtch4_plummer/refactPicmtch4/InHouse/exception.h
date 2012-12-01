/*===========================================================================
=                                                                           =
=                                 Exception                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MISR
                              GeoCal Processing

             Copyright 2002, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: Exception

!ABSTRACT:

   This is the base of the exception hierachy for GeoCal code. This can be
   written to like a stream to contain information about the
   exception. This is derived from the standard library std::exception.

!DERIVED FROM:

   std::exception

!DESCRIPTION:

!PUBLIC MEMBERS:

   inline Exception() throw()
     - Constructor.

     REQUIRE: Nothing
     PROMISE: Nothing

   inline Exception(const Exception& E) throw()
     - Copy Constructor.

     REQUIRE: Nothing
     PROMISE: Nothing

   inline virtual ~Exception() throw()
     - Destructor.

     REQUIRE: Nothing
     PROMISE: Nothing

   template<class T> inline Exception& operator<<(const T& V) throw()
     - Write to exception.

   virtual inline const char* what() const throw()
     - Return string in exception.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   std::ostringstream s_
    - Stream.

   std::string scratch_
     - Scratch variable used by what(). See what() for comments

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <sstream>		// Definition of ostringstream.
namespace GeoCal {
  namespace GeoCalCore {
    class Exception;
  }
}

//***************************************************************************
// Declaration of class Exception.
//***************************************************************************

class GeoCal::GeoCalCore::Exception: public std::exception {
public:
  inline           Exception() throw();
  inline           Exception(const Exception& E) throw();
  inline virtual   ~Exception() throw();
  template<class T> inline Exception& operator<<(const T& V) throw();
  virtual inline const char* what() const throw();
protected:
private:
  std::ostringstream 
		   s_;
  mutable std::string 
		   scratch_;
};


//***************************************************************************
// Include source of inline functions.
//***************************************************************************

#include "exception.icc"
#endif


