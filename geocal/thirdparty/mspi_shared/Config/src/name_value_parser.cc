/*===========================================================================
=                                                                           =
=                              NameValueParser                              =
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

!ABSTRACT:

   This contains support routines for the class NameValueParser

!PARENT(S):

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "name_value_parser.h"		
				// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::NameValueParser::NameValueParser()
  : done_(false),
    name_next_(false)
{
  reset();
}

/////////////////////////////////////////////////////////////////////////////
/// Add the next token.  Returns true if the token is added successfully.
/// Returns false if the token cannot be added.
/// Throws an exception if an invalid token sequence is detected.
/// 
/// Parsing rules are as follows:
/// 1) Any token following a ":" is the variable name.
/// 2) Any token which is not a name or a ":" is a variable value.
/// 3) The variable value must contain at least one element.
/// 4) Parsing ends at the variable name.
///
/// Valid token sequences are as follows:
/// 1) For scalar variables:
///      value : name
/// 2) For array variables:
///      value value ... value : name
/////////////////////////////////////////////////////////////////////////////

bool
MSPI::Shared::NameValueParser::NameValueParser::add_token(
  const std::string& Token
)
{
  if (done_) {
    return false;
  }

  if (Token == ":") {
    name_next_ = true;

    if (value_.size() < 1) {
      throw MSPI_EXCEPTIONm("Unexpected token: '"+Token+"'");
    }

    return true;
  }
  
  if (name_next_) {
    name_ = Token;
    done_ = true;
    return true;
  }

  value_.push_back(Token);

  return true;
}
