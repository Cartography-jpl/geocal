/*===========================================================================
=                                                                           =
=                               Exception                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                     Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: Exception

!ABSTRACT:

   Generic exception class, derived from std::exception.

!DERIVED FROM:

!DESCRIPTION:

!PUBLIC MEMBERS:

   Exception(const std::string& Message);
     - Constructor accepts string containing message returned by what() method.

   const char* what() const throw();
     - Return message.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   std::string message_

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef MSPI_EXCEPTION_H
#define MSPI_EXCEPTION_H

#include <exception> // Defintion of std::exception
#include <string>    // Definition of std::string

#define MSPI_EXCEPTIONm(message) MSPI::Shared::Exception(message, __FILE__, __LINE__)

namespace MSPI {
  namespace Shared {
    class Exception;
  }
}

//***************************************************************************
// Declaration of class Exception.
//***************************************************************************

class MSPI::Shared::Exception : public std::exception {
public:

  Exception(const std::string& Message);
  Exception(const std::string& Message, const std::string& Filename,
	    int Line_number);
  const char* what() const throw();
  std::string where() const throw();
  std::string stack_trace() const throw();
  ~Exception() throw();

protected:

private:
  void stack_trace_init() throw();
  std::string message_;
  std::string filename_;
  std::string trace_;
  int line_number_;
};

#endif
