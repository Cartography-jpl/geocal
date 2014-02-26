/*===========================================================================
=                                                                           =
=                                CodeLocation                               =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MISR
                              GeoCal Processing
                                                                        
            Copyright 2006, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

!C++=========================================================================

!CLASS NAME: CodeLocation

!ABSTRACT:  

   This class describes a location in code.

!DERIVED FROM:

!DESCRIPTION:

   This is little more than a structure. It contains the file and
   line number of a location in code, as well as the name of the
   function.

   The macro CODE_LOCATION is defined, which creates a CodeLocation
   object filling in information about the current location.

!PUBLIC MEMBERS:

   inline CodeLocation(const char* File, int Line , const char*
      Function) 
    - Constructor.

   inline const char* file() const
    - Return file name.
   
   inline const char* function() const
    - Return function name.

   inline int line() const
    - Return line number.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   const char* file_
    - File name.

   const char* function_
    - Function name.

   int line_
    - Line number.

!ASSOCIATED FUNCTIONS:

   inline std::ostream& operator<<(std::ostream& S, const CodeLocation& Cl)
    - Print object to stream.

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

   Note that we only copy the pointer to File and Function, we assume
   that these character strings remain in existance throughout the
   life of this class. To be safe, we should copy these strings for
   our own use. However, this class is used *everywhere*, an we want
   to minimize the overhead. There is no problem if the class is
   created through CODE_LOCATION, which is typically is. (Limitation)

!REVISION HISTORY:

  See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef CODE_LOCATION_H
#define CODE_LOCATION_H
#include <iostream>		// Definition of ostream.

namespace GeoCal {
  namespace GeoCalCore {
    class CodeLocation;
  }
}
//***************************************************************************
// Declaration of class CodeLocation.
//***************************************************************************

class GeoCal::GeoCalCore::CodeLocation {
public:
  inline	   CodeLocation(const char* File, int Line, const
				char* Function);  
  inline const char*	   
		   file() const;
  inline int	   line() const;
  inline const char*
		   function() const;
private:
  const char*	   file_;
  const char*	   function_;
  int		   line_;
};

//***************************************************************************
// Prototype of associated functions.
//***************************************************************************

namespace GeoCal {
  namespace GeoCalCore {
    inline std::ostream& operator<<(std::ostream& S, const CodeLocation& Cl);
  }
}
//***************************************************************************
// Include source of inline functions.
//***************************************************************************
#include "code_location.icc"

//-----------------------------------------------------------------------
// Define macro for creating a CodeLocation object for current code
// location. 
//-----------------------------------------------------------------------

#ifdef __GNUG__
#define CODE_LOCATION GeoCal::GeoCalCore::CodeLocation(__FILE__, __LINE__, \
				   __PRETTY_FUNCTION__) 
#else
#define CODE_LOCATION GeoCal::GeoCalCore::CodeLocation(__FILE__, __LINE__, \
				   "Not available") 
#endif
#endif
