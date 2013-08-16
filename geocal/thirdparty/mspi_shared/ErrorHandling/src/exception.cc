/*===========================================================================
=                                                                           =
=                               Exception                                    =
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

   This contains support routines for the class Exception 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "exception.h"		// Definition of class.
#include <sstream> 		// Definition of std::ostringstream
#include <vector>
#include <execinfo.h>
#include <cstring> 		// Definition of strncpy
#include <cxxabi.h>
#include <cstdlib>              // Definition of malloc

//***************************************************************************
// Constructor definition.
//***************************************************************************

MSPI::Shared::Exception::Exception(const std::string& Message)
  : message_(Message)
{
  stack_trace_init();
}

//***************************************************************************
// Constructor definition.
//***************************************************************************

MSPI::Shared::Exception::Exception(const std::string& Message, 
				   const std::string& Filename, 
				   int Line_number)
  : message_(Message),
    filename_(Filename),
    line_number_(Line_number)
{
  stack_trace_init();
}

/////////////////////////////////////////////////////////////////////////////
/// Return error message.
/////////////////////////////////////////////////////////////////////////////

const char *
MSPI::Shared::Exception::what() const throw()
{
  return message_.c_str();
}

/////////////////////////////////////////////////////////////////////////////
/// Return line number and filename (if available)
/////////////////////////////////////////////////////////////////////////////

std::string
MSPI::Shared::Exception::where() const throw()
{
  if (filename_.size() > 0) {
    std::ostringstream msg;
    msg << "Line " << line_number_ << " in " << filename_;
    return msg.str();
  } else {
    return std::string("Location not available");
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Return stack trace.
/////////////////////////////////////////////////////////////////////////////

std::string
MSPI::Shared::Exception::stack_trace() const throw()
{
  return trace_;
}

//***************************************************************************
// Destructor definition.
//***************************************************************************

MSPI::Shared::Exception::~Exception() throw()
{
}

//***************************************************************************
// Generate stack trace
//***************************************************************************

void
MSPI::Shared::Exception::stack_trace_init() throw()
{
  //-----------------------------------------------------------
  // Generate stack trace.
  // This code is GCC specific.  It will likely not work with 
  // other compilers.
  // 
  // Requires -rdynamic compile flag.
  //-----------------------------------------------------------

  int max_stacktrace_depth = 256;
  std::vector<void*> buffer(max_stacktrace_depth,NULL);
  
  int number_of_addresses 
    = backtrace(&(buffer[0]), max_stacktrace_depth);
 
  char **strings 
    = backtrace_symbols(&(buffer[0]), number_of_addresses);
 
  for (int i = 0; i < number_of_addresses; ++i) {
    size_t max_name_width = 1024;
    char *demangled_name 
      = static_cast<char*>(malloc(max_name_width));

    char *begin = 0, *end = 0;
 
    for (char *j = strings[i]; *j; ++j) {
      if (*j == '(')
	begin = j;
      else if (*j == '+') 
	end = j;
    }
 
    if (0 != begin && 0 != end) {
      *begin++ = '\0'; *end = '\0';
      
      int ignore;
      char *returned_demangled_name 
	= abi::__cxa_demangle(begin, demangled_name, 
			      &max_name_width, &ignore);
      if (0 != returned_demangled_name){
	demangled_name = returned_demangled_name;
      } else {
	std::strncpy(demangled_name, begin, max_name_width);
	std::strncat(demangled_name, "()", max_name_width);
	demangled_name[max_name_width-1] = '\0';
      }
      trace_.append(demangled_name);
    } else {
      trace_.append(strings[i]);
    }
    trace_.append("\n"); 
    free(demangled_name);
  }
  free(strings);
}

