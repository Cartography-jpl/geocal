/*===========================================================================
=                                                                           =
=                                 NameValueParser                           =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MISR
                                  Config File

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: NameValueParser

=============================================================================
!END=======================================================================*/

#ifndef NAME_VALUE_PARSER_H
#define NAME_VALUE_PARSER_H

#include <string>		// Definition of std::string
#include <map> 			// Definition of std::map
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <vector>   		// Definition of std::vector

namespace MSPI {
  namespace Shared {
    class NameValueParser;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Parses a stream of tokens into name value pairs.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::NameValueParser {
public:
  NameValueParser();
  bool add_token(const std::string& Token);
  bool done() const;
  void reset();
  std::string name() const;
  std::vector<std::string> value() const;
protected:

private:
  std::string name_;
  std::vector<std::string> value_;
  bool done_;
  bool name_next_;
};

/////////////////////////////////////////////////////////////////////////////
/// Reset parser state to begin parsing a new token stream.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::NameValueParser::reset()
{
  name_.clear();
  value_.clear();
  done_ = false;
  name_next_ = false;
}

/////////////////////////////////////////////////////////////////////////////
/// Return flag indicating if parsing is done for the current name,value pair
/////////////////////////////////////////////////////////////////////////////

inline bool
MSPI::Shared::NameValueParser::done() const
{
  return done_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return variable name.  Throws an exception if parsing is not done.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::NameValueParser::name() const
{
  if (!done_) {
    throw MSPI_EXCEPTIONm("Parsing not done (name)");
  }
  return name_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return variable value parsed.   Throws an exception if parsing is not done.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string>
MSPI::Shared::NameValueParser::value() const
{
  if (!done_) {
    throw MSPI_EXCEPTIONm("Parsing not done (value)");
  }
  return value_;
}

#endif
