/*===========================================================================
=                                                                           =
=                              ConfigFile                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                   Config File

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: ConfigFile

!ABSTRACT:

   This contains support routines for the class ConfigFile

!PARENT(S):

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "config_file.h"	// Definition of class.
#include <boost/regex.hpp> 	// Definition of boost::regex
#include "name_value_parser.h"  // Definition of NameValueParser
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <boost/tokenizer.hpp>  // Definition of boost::tokenizer
#include <fstream> 		// Definition of std::ifstream
#include <boost/regex.hpp> 	// Definition of boost::regex
#include <sstream> 		// Definition of std::ostringstream


/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ConfigFile::ConfigFile(
const std::string& Filename
) 
  : filename_(Filename)
{
  value_ = parse(Filename);
}

/////////////////////////////////////////////////////////////////////////////
/// Add content of the given file, replacing any existing variables 
/// with new values.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::ConfigFile::add_replace(
  const std::string& Filename
) 
{
  typedef std::map<std::string, std::vector<std::string> > mtype;
  mtype new_values = parse(Filename);

  for (mtype::iterator i = new_values.begin() ; i != new_values.end() ; ++i) {
    std::pair<mtype::iterator, bool> result = value_.insert(*i);
    if (!result.second) {
      result.first->second = i->second;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Return parsed content of the given file.
/////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::vector<std::string> >
MSPI::Shared::ConfigFile::parse(const std::string& Filename) 
{
  
  std::map<std::string, std::vector<std::string> > result;

  //---------------------------------------------------------------------
  // Open file
  //---------------------------------------------------------------------

  std::ifstream infile(Filename.c_str(), std::ifstream::in);
  if (!infile.good()) {
    throw MSPI_EXCEPTIONm("Trouble opening input file: "+Filename);
  }

  //---------------------------------------------------------------------
  // Initialize parser
  //---------------------------------------------------------------------

  NameValueParser parser;

  //---------------------------------------------------------------------
  // For each line in the input file...
  //---------------------------------------------------------------------

  std::string line;
  int line_counter = 0;
  while (std::getline(infile,line)) {
    line_counter++;

    //---------------------------------------------------------------------
    // Strip comments from this line.
    // '#' character and everything after it is a comment.
    //---------------------------------------------------------------------

    boost::match_results<std::string::const_iterator> match1;
    const boost::regex re1("^([^#]+)");
    if (!boost::regex_search(line,match1,re1)) {
      continue;
    }
    line.assign(match1[1].first,match1[1].second);
    
    //---------------------------------------------------------------------
    // Convert the line to tokens.
    //---------------------------------------------------------------------

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" \t", "[]:");
    tokenizer tok(line,sep);

    //---------------------------------------------------------------------
    // For each token...(begin loop)
    //---------------------------------------------------------------------

    for(tokenizer::iterator i=tok.begin(); i!=tok.end();++i) {

      //---------------------------------------------------------------------
      // Submit this token to the token parser.
      //---------------------------------------------------------------------

      try {
	bool success = parser.add_token(*i);

	//---------------------------------------------------------------------
	// If parsing is done, then add this name value pair, and reset the 
	// token parser the next cycle.
	// Don't allow the same name to be defined more than once.
	// Require values to have a size of at least 1.
	// Resubmit this token after resetting if it was not added successfully.
	//---------------------------------------------------------------------

	if (parser.done()) {
	  std::vector<std::string> value = parser.value();

	  if (value.size() < 1) {
	    std::ostringstream msg;
	    throw MSPI_EXCEPTIONm("Unexpected result: parser.value().size() < 1");
	  }

	  if (result.find(parser.name()) != result.end()) {
	    std::ostringstream msg;
	    msg << "Duplicate config parameter name: " << parser.name();
	    throw MSPI_EXCEPTIONm(msg.str());
	  }

	  result[parser.name()] = value;
	  parser.reset();

	  if (!success) {
	    parser.add_token(*i);
	  }

	}

      //---------------------------------------------------------------------
      // Catch any parse errors, and report the config file line number where
      // the error occurred.
      //---------------------------------------------------------------------

      } catch(MSPI::Shared::Exception& e) {
	std::ostringstream message;
	message << "Caught exception, ["<<e.what() << "], at line " 
		<< line_counter << " of " << Filename << ", line='" << line << "'";
	throw MSPI_EXCEPTIONm(message.str());
      }

      //---------------------------------------------------------------------
      // End loop for each token on this line.
      //---------------------------------------------------------------------

    }

    //---------------------------------------------------------------------
    // End loop for each line in the input file.
    //---------------------------------------------------------------------

  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////
// Private method to retrieve reference to vector associated with the given
// name.  Throws an exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

const std::vector<std::string>&
MSPI::Shared::ConfigFile::get(const std::string& Name) const
{
  std::map<std::string, std::vector<std::string> >::const_iterator f = value_.find(Name);
  if (f == value_.end()) {
    throw MSPI_EXCEPTIONm("Configuration parameter not found: '"+Name+
			  "', filename="+filename_);
  }
  return f->second;
}

/////////////////////////////////////////////////////////////////////////////
// Private function to convert an ascii string to a boolean.
// Throw an exception if the ascii string does not contain a recognizable
// boolean value.
/////////////////////////////////////////////////////////////////////////////

bool 
MSPI::Shared::ConfigFile::atobool(const std::string& String) const
{
  std::string copy = String;
  for (size_t i = 0 ; i < copy.size() ; i++) {
    copy[i] = tolower(copy[i]);
  }
  if (copy == "t" ||
      copy == "true" ||
      copy == ".true.") {
    return true;
  } else if (copy == "f" ||
	     copy == "false" ||
	     copy == ".false.") {
    return false;
  } else {
    std::ostringstream msg;
    msg << "Boolean value not recognized for string: '" << String << "', filename="+filename_;
    throw MSPI_EXCEPTIONm(msg.str());
  }
}

